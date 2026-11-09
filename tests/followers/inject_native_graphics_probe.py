#!/usr/bin/env python3
"""Install a temporary native mGBA follower graphics memory probe into a checkout of CURRENT main.

Run in a temporary CI checkout, never commit changes to production source.
The probe tests real ARM sprite tile allocator semantics under mGBA, NOT on-map
follower render correctness, OAM appearance or all-world-state VRAM feasibility.
"""
from pathlib import Path

runner = Path("tests/emulator/runner.c")
bridge = Path("tests/emulator/run.py")
assert runner.is_file() and bridge.is_file(), "Run from current main source root"
c = runner.read_text()
p = bridge.read_text()
if "LIGHTNING_FOLLOWER_GFX_ONLY" in c:
    raise RuntimeError("Probe already applied")
anchor = "int main(int argc,char **argv) {"
assert c.count(anchor) == 1
probe = r"""
// Isolated ROM-native sprite tile allocator test. All CALLED routines execute
// ARM code in the built FireRed ROM, not equivalent host mocks.
static void followerGraphicsNativeProbe(void) {
    CHECK(core->loadState(core,bootState));
    CALL(ResetSpriteData);
    uint32_t small = CALL(AllocSpriteTiles,16); // 32x32 indexed 4bpp
    CHECK(small != 0xFFFFFFFFU);
    uint32_t large = CALL(AllocSpriteTiles,64); // Steelix 64x64 indexed 4bpp
    CHECK(large != 0xFFFFFFFFU);
    CHECK(large >= small + 16);
    for (unsigned tile=small;tile<small+16;tile++)
        CHECK(CALL(SpriteTileAllocBitmapOp,tile,2) != 0);
    for (unsigned tile=large;tile<large+64;tile++)
        CHECK(CALL(SpriteTileAllocBitmapOp,tile,2) != 0);
    printf("PASS: native mGBA simultaneous 32x32/64x64 tile allocations: small=%u large=%u\n",small,large);

    // A release is tested via the game's own bitmap allocator reset.
    // A production follower must use controlled ownership, not this global
    // reset which deliberately resets all non-reserved object tile allocations.
    CHECK(CALL(AllocSpriteTiles,0) == 0);
    for (unsigned tile=small;tile<small+16;tile++)
        CHECK(CALL(SpriteTileAllocBitmapOp,tile,2) == 0);
    for (unsigned tile=large;tile<large+64;tile++)
        CHECK(CALL(SpriteTileAllocBitmapOp,tile,2) == 0);
    uint32_t first=CALL(AllocSpriteTiles,64);
    CHECK(first != 0xFFFFFFFFU);
    printf("PASS: native mGBA 64x64 allocation succeeds after releasing tile bitmap\n");

    // Exhaust tile allocator through the actual ROM until -1; then prove
    // failed allocation does not allocate tiles or poison subsequent reset.
    unsigned successful=1;
    uint32_t previous=first;
    for (unsigned attempt=0;attempt<64;attempt++) {
        uint32_t next=CALL(AllocSpriteTiles,64);
        if (next == 0xFFFFFFFFU) break;
        CHECK(next >= previous+64);
        previous=next;
        successful++;
    }
    CHECK(successful > 1 && successful < 64);
    CHECK(CALL(AllocSpriteTiles,64) == 0xFFFFFFFFU);
    CHECK(CALL(AllocSpriteTiles,16) == 0xFFFFFFFFU);
    printf("PASS: native mGBA deterministic OBJ tile exhaustion, %u simultaneous 64x64 allocations\n",successful);
    CHECK(CALL(AllocSpriteTiles,0) == 0);
    CHECK(CALL(AllocSpriteTiles,64) != 0xFFFFFFFFU);
    CHECK(CALL(AllocSpriteTiles,0) == 0);
    printf("PASS: native mGBA allocation failure recovers without persistent tile-bitmap corruption\n");
}
// Also exercise the actual FireRed graphics setup path and real OAM descriptors.
// Unlike the allocator-only test, these create and free rendered sprites using
// the existing FireRed Snorlax (32x32) and Seagallop (64x64) ROM artwork.
static void followerGraphicsObjectProbe(void) {
    CHECK(core->loadState(core,bootState));
    CALL(ResetSpriteData);
    uint32_t small=CALL(CreateVirtualObject,109,1,10,10,0,1);
    uint32_t big=CALL(CreateVirtualObject,108,2,11,10,0,1);
    CHECK(small < 64 && big < 64 && small != big);
    uint32_t smallPtr=SYM_gSprites + small*ABI_FOLLOW_SPRITE_SIZE;
    uint32_t bigPtr=SYM_gSprites + big*ABI_FOLLOW_SPRITE_SIZE;
    uint32_t smallOam=smallPtr + ABI_FOLLOW_OAM_OFFSET;
    uint32_t bigOam=bigPtr + ABI_FOLLOW_OAM_OFFSET;
    CHECK((core->busRead16(core,smallOam+2)>>14)==2);
    CHECK((core->busRead16(core,bigOam+2)>>14)==3);
    uint32_t smallTile=core->busRead16(core,smallOam+4)&1023;
    uint32_t bigTile=core->busRead16(core,bigOam+4)&1023;
    CHECK(bigTile>=smallTile+16 || smallTile>=bigTile+64);
    for(unsigned i=0;i<16;i++)
        CHECK(CALL(SpriteTileAllocBitmapOp,smallTile+i,2)!=0);
    for(unsigned i=0;i<64;i++)
        CHECK(CALL(SpriteTileAllocBitmapOp,bigTile+i,2)!=0);
    CALL(BuildOamBuffer);
    CHECK((core->busRead16(core,bigOam+2)>>14)==3);
    CALL(LoadOam); // execute actual FireRed CPU copy to hardware OBJ OAM
    unsigned smallHardware=0,bigHardware=0;
    for(unsigned i=0;i<128;i++) {
        uint32_t entry=0x07000000U+i*8;
        uint32_t size=core->busRead16(core,entry+2)>>14;
        uint32_t tile=core->busRead16(core,entry+4)&1023;
        if(size==2 && tile==smallTile) smallHardware++;
        if(size==3 && tile==bigTile) bigHardware++;
    }
    CHECK(smallHardware>=1 && bigHardware>=1);
    printf("PASS: native mGBA hardware OAM contains both 32x32 and 64x64 sprite entries\n");
    // Stage the EXACT pinned Steelix source frame in emulated hardware OBJ VRAM,
    // replacing only this disposable Seagallop test object's graphics tiles.
    // Use 16-bit bus writes: GBA OBJ VRAM does not support ordinary byte writes.
    for (unsigned i=0;i<2048;i+=2) {
        uint32_t address=0x06010000U+bigTile*32+i;
        uint16_t packed=followerSteelixFrames[0][i]|(followerSteelixFrames[0][i+1]<<8);
        core->busWrite16(core,address,packed);
        CHECK(core->busRead16(core,address)==packed);
    }
    uint32_t bigPalette=(core->busRead16(core,bigOam+4)>>12)&15;
    for (unsigned i=0;i<16;i++) {
        uint32_t address=0x05000200U+bigPalette*32+i*2;
        core->busWrite16(core,address,followerSteelixNormalPalette[i]);
        CHECK(core->busRead16(core,address)==followerSteelixNormalPalette[i]);
    }
    printf("PASS: exact pinned Steelix 64x64 frame and normal palette staged with readback from emulated OBJ VRAM/PLTT\n");

    printf("PASS: native mGBA Snorlax and Seagallop sprites both created with 32/64 OAM and allocated tiles\n");
    CALL(DestroySprite,smallPtr);
    for(unsigned i=0;i<16;i++)
        CHECK(CALL(SpriteTileAllocBitmapOp,smallTile+i,2)==0);
    CALL(DestroySprite,bigPtr);
    for(unsigned i=0;i<64;i++)
        CHECK(CALL(SpriteTileAllocBitmapOp,bigTile+i,2)==0);
    uint32_t again=CALL(CreateVirtualObject,108,3,11,10,0,1);
    CHECK(again<64);
    uint32_t ptr=SYM_gSprites + again*ABI_FOLLOW_SPRITE_SIZE;
    CHECK((core->busRead16(core,ptr+ABI_FOLLOW_OAM_OFFSET+2)>>14)==3);
    CALL(DestroySprite,ptr);
    printf("PASS: native mGBA 32/64 sprite destruction and 64x64 re-creation release graphics resources\n");
}
static uint64_t followerGraphicsFrameHash(unsigned *colored) {
    const void *image;
    size_t stride;
    core->getPixels(core,&image,&stride);
    const color_t *px=(const color_t *)image;
    uint32_t background=((uint32_t)px[0])&0xFFFFFFU;
    uint64_t hash=1469598103934665603ULL;
    *colored=0;
    for(unsigned y=48;y<112;y++) {
        for(unsigned x=80;x<144;x++) {
            uint32_t color=((uint32_t)px[y*stride+x])&0xFFFFFFU;
            if(color!=background) (*colored)++;
            hash^=color;
            hash*=1099511628211ULL;
        }
    }
    return hash;
}

// Render EXACT SIX Steelix sprites via mGBA GPU after staging indexed native
// tiles and palettes. Park game CPU to avoid unrelated title callbacks wiping
// test graphics, while the emulator's LCD clock still draws real scanlines.
// Graphics-only transaction: acquire and verify successor before destroying
// predecessor. This exists only in the native emulator test harness; not in
// player-facing follower, map, party or save code.
static uint32_t followerAtomicSwap(uint32_t oldId,unsigned gfx,unsigned virtualId) {
    uint32_t replacement=CALL(CreateVirtualObject,gfx,virtualId,10,10,0,1);
    if(replacement>=64) return oldId;
    if(oldId<64) {
        uint32_t original=SYM_gSprites+oldId*ABI_FOLLOW_SPRITE_SIZE;
        CALL(DestroySprite,original);
    }
    return replacement;
}
static uint32_t followerSpriteTile(uint32_t id) {
    return core->busRead16(core,SYM_gSprites+id*ABI_FOLLOW_SPRITE_SIZE+ABI_FOLLOW_OAM_OFFSET+4)&1023;
}
static uint32_t followerSpriteSize(uint32_t id) {
    return core->busRead16(core,SYM_gSprites+id*ABI_FOLLOW_SPRITE_SIZE+ABI_FOLLOW_OAM_OFFSET+2)>>14;
}
static void followerAtomicSwapNativeProbe(void) {
    CHECK(core->loadState(core,bootState));
    CALL(ResetSpriteData);
    uint32_t small=CALL(CreateVirtualObject,109,14,10,10,0,1);
    CHECK(small<64 && followerSpriteSize(small)==2);
    unsigned smallTile=followerSpriteTile(small);
    uint32_t large=followerAtomicSwap(small,108,15);
    CHECK(large<64 && large!=small && followerSpriteSize(large)==3);
    for(unsigned i=0;i<16;i++) CHECK(CALL(SpriteTileAllocBitmapOp,smallTile+i,2)==0);
    unsigned largeTile=followerSpriteTile(large);
    for(unsigned i=0;i<64;i++) CHECK(CALL(SpriteTileAllocBitmapOp,largeTile+i,2)!=0);
    uint32_t back=followerAtomicSwap(large,109,16);
    CHECK(back<64 && back!=large && followerSpriteSize(back)==2);
    for(unsigned i=0;i<64;i++) CHECK(CALL(SpriteTileAllocBitmapOp,largeTile+i,2)==0);
    CHECK(followerSpriteTile(back)+16<=1024);
    CALL(DestroySprite,SYM_gSprites+back*ABI_FOLLOW_SPRITE_SIZE);
    printf("PASS: native 32->64->32 acquire-first swap releases predecessor tile ranges\n");

    // Real ROM-native allocation under OBJ VRAM pressure. Keep predecessor
    // 32x32 alive while filling the remaining tile space with other sprites.
    CALL(ResetSpriteData);
    uint32_t original=CALL(CreateVirtualObject,109,17,10,10,0,1);
    CHECK(original<64 && followerSpriteSize(original)==2);
    unsigned originalTile=followerSpriteTile(original);
    uint32_t blockers[24];
    unsigned count=0;
    for(;count<24;count++) {
        uint32_t id=CALL(CreateVirtualObject,108,20+count,11,10,0,1);
        if(id>=64) break;
        blockers[count]=id;
    }
    CHECK(count>=10 && count<24);
    uint32_t aborted=followerAtomicSwap(original,108,50);
    CHECK(aborted==original && followerSpriteSize(original)==2);
    CHECK(followerSpriteTile(original)==originalTile);
    for(unsigned i=0;i<16;i++) CHECK(CALL(SpriteTileAllocBitmapOp,originalTile+i,2)!=0);
    printf("PASS: native failed 32->64 swap preserves original sprite and its 16 tile allocations\n");
    // Remove ONE blocker; successor can be allocated while old remains live.
    CALL(DestroySprite,SYM_gSprites+blockers[count-1]*ABI_FOLLOW_SPRITE_SIZE);
    uint32_t resumed=followerAtomicSwap(original,108,51);
    CHECK(resumed<64 && resumed!=original && followerSpriteSize(resumed)==3);
    for(unsigned i=0;i<16;i++) CHECK(CALL(SpriteTileAllocBitmapOp,originalTile+i,2)==0);
    unsigned resumedTile=followerSpriteTile(resumed);
    for(unsigned i=0;i<64;i++) CHECK(CALL(SpriteTileAllocBitmapOp,resumedTile+i,2)!=0);
    CALL(DestroySprite,SYM_gSprites+resumed*ABI_FOLLOW_SPRITE_SIZE);
    for(unsigned i=0;i<count-1;i++)
        CALL(DestroySprite,SYM_gSprites+blockers[i]*ABI_FOLLOW_SPRITE_SIZE);
    CHECK(CALL(AllocSpriteTiles,64)!=0xFFFFFFFFU);
    printf("PASS: native resource-starved swap retries safely and cleans up under sprite crowding\n");
}

// Real field-map callback smoke test: reuse the already-accepted Day Care
// fixture, which boots an actual map with player, NPC graphics and field tasks.
// This is still a virtual-object graphics probe, not follower map persistence.
static void followerGraphicsMapTransitionProbe(void) {
    daycareFixture(1); // real Route 5 Day Care map, 360 emulated field frames
    CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
    uint32_t routeSmall=CALL(CreateVirtualObject,109,52,3,5,0,1);
    CHECK(routeSmall<64 && followerSpriteSize(routeSmall)==2);
    uint32_t routeBig=followerAtomicSwap(routeSmall,108,53);
    CHECK(routeBig<64 && routeBig!=routeSmall && followerSpriteSize(routeBig)==3);
    CALL(BuildOamBuffer);
    CALL(LoadOam);
    CHECK(followerSpriteSize(routeBig)==3);
    CALL(DestroySprite,SYM_gSprites+routeBig*ABI_FOLLOW_SPRITE_SIZE);
    printf("PASS: native Route 5 map creates, switches and releases 32x32/64x64 graphics with real NPC scene\n");

    // Cross an actual FireRed field warp and reload to another populated map.
    // Every old virtual sprite handle is invalid after CB2_LoadMap; recreate
    // only from current map resources, never trust stale OAM or bitmap state.
    CALL(SetWarpDestination,MAP_GROUP(MAP_FOUR_ISLAND_POKEMON_DAY_CARE),
         MAP_NUM(MAP_FOUR_ISLAND_POKEMON_DAY_CARE),0xFFFFFFFFU,3,6);
    CALL(WarpIntoMap);
    CALL(ResetInitialPlayerAvatarState);
    CALL(CB2_LoadMap);
    frames(360,0);
    CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
    uint32_t islandSmall=CALL(CreateVirtualObject,109,54,3,5,0,1);
    CHECK(islandSmall<64 && followerSpriteSize(islandSmall)==2);
    uint32_t islandBig=followerAtomicSwap(islandSmall,108,55);
    CHECK(islandBig<64 && islandBig!=islandSmall && followerSpriteSize(islandBig)==3);
    CALL(BuildOamBuffer);
    CALL(LoadOam);
    unsigned islandTile=followerSpriteTile(islandBig);
    for(unsigned i=0;i<64;i++)
        CHECK(CALL(SpriteTileAllocBitmapOp,islandTile+i,2)!=0);
    CALL(DestroySprite,SYM_gSprites+islandBig*ABI_FOLLOW_SPRITE_SIZE);
    for(unsigned i=0;i<64;i++)
        CHECK(CALL(SpriteTileAllocBitmapOp,islandTile+i,2)==0);
    printf("PASS: native actual Route 5 -> Four Island map reload and size-atomic graphics recreation/cleanup\n");
}

// Exercise the production graphics resource owner, compiled into the temporary
// ROM, with the ACTUAL pinned Steelix 64x64 frame data and its own OBJ palette.
static void followerProductionEventProbe(void) {
    CHECK(core->loadState(core,bootState));
    CALL(ResetSpriteData);
    CALL(FollowerGraphics_InvalidateAfterSpriteReset);
    CHECK(CALL(FollowerGraphics_NativeSmall,110,88,2)==0);
    CHECK(CALL(FollowerGraphics_BindObjectEvent,0,0,12,12,0)!=0);
    unsigned eventId=CALL(FollowerGraphics_GetObjectEventId);
    CHECK(eventId<ABI_OBJECT_COUNT);
    unsigned event=SYM_gObjectEvents+eventId*ABI_FOLLOW_OBJECT_SIZE;
    CHECK((core->busRead8(core,event)&1)!=0);
    CHECK(core->busRead8(core,event+4)==CALL(FollowerGraphics_GetSpriteId));
    CHECK(core->busRead8(core,event+5)==109);
    CHECK(CALL(FollowerGraphics_NativeSteelix,110,88,2)==0);
    CHECK(core->busRead8(core,event+4)==CALL(FollowerGraphics_GetSpriteId));
    CHECK(core->busRead8(core,event+5)==108);
    for(unsigned frame=0;frame<6;frame++)
        CHECK(CALL(FollowerGraphics_SetFrame,frame)!=0);
    CALL(FollowerGraphics_Release);
    CHECK((core->busRead8(core,event)&1)==0);
    CHECK(CALL(FollowerGraphics_GetObjectEventId)==ABI_OBJECT_COUNT);
    printf("PASS: PRODUCTION dedicated ObjectEvent slot acquired, updated on 32->Steelix64, released safely\n");

    // Force all 16 actual ObjectEvent slots busy without destroying NPCs.
    CHECK(CALL(FollowerGraphics_NativeSmall,110,88,2)==0);
    for(unsigned i=0;i<ABI_OBJECT_COUNT;i++) {
        unsigned p=SYM_gObjectEvents+i*ABI_FOLLOW_OBJECT_SIZE;
        core->busWrite8(core,p,core->busRead8(core,p)|1);
    }
    CHECK(CALL(FollowerGraphics_BindObjectEvent,0,0,12,12,0)==0);
    CHECK(CALL(FollowerGraphics_GetObjectEventId)==ABI_OBJECT_COUNT);
    CHECK(CALL(FollowerGraphics_GetSize)==0);
    CHECK(CALL(FollowerGraphics_GetSpriteId)==64);
    CALL(FollowerGraphics_Release);
    printf("PASS: PRODUCTION full 16-slot ObjectEvent pool refuses registration without corrupting graphics owner\n");

    // Registered graphics object survives a true Route5 field map, but the
    // owned event/sprite must be detached before a field warp rebuild.
    daycareFixture(1);
    CALL(FollowerGraphics_InvalidateAfterSpriteReset);
    CHECK(CALL(FollowerGraphics_NativeSmall,100,80,1)==0);
    CHECK(CALL(FollowerGraphics_BindObjectEvent,MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE),
               MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE),10,10,0)!=0);
    unsigned first=CALL(FollowerGraphics_GetObjectEventId);
    CHECK(first<ABI_OBJECT_COUNT);
    CHECK(CALL(FollowerGraphics_NativeSteelix,100,80,1)==0);
    CHECK(core->busRead8(core,SYM_gObjectEvents+first*ABI_FOLLOW_OBJECT_SIZE+4)
          ==CALL(FollowerGraphics_GetSpriteId));
    frames(30,0); // Actual map update loops with the graphics-owned event alive.
    CHECK((core->busRead8(core,SYM_gObjectEvents+first*ABI_FOLLOW_OBJECT_SIZE)&1)!=0);
    CHECK(core->busRead8(core,SYM_gObjectEvents+first*ABI_FOLLOW_OBJECT_SIZE+4)
          ==CALL(FollowerGraphics_GetSpriteId));
    CALL(FollowerGraphics_DetachForMapTransition);
    CHECK((core->busRead8(core,SYM_gObjectEvents+first*ABI_FOLLOW_OBJECT_SIZE)&1)==0);
    CALL(SetWarpDestination,MAP_GROUP(MAP_FOUR_ISLAND_POKEMON_DAY_CARE),
         MAP_NUM(MAP_FOUR_ISLAND_POKEMON_DAY_CARE),0xFFFFFFFFU,3,6);
    CALL(WarpIntoMap);
    CALL(ResetInitialPlayerAvatarState);
    CALL(CB2_LoadMap);
    frames(360,0);
    CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
    CALL(FollowerGraphics_InvalidateAfterSpriteReset);
    CHECK(CALL(FollowerGraphics_NativeSteelix,100,80,1)==0);
    CHECK(CALL(FollowerGraphics_BindObjectEvent,MAP_GROUP(MAP_FOUR_ISLAND_POKEMON_DAY_CARE),
               MAP_NUM(MAP_FOUR_ISLAND_POKEMON_DAY_CARE),10,10,0)!=0);
    unsigned islandId=CALL(FollowerGraphics_GetObjectEventId);
    CHECK(islandId<ABI_OBJECT_COUNT);
    frames(30,0);
    CHECK((core->busRead8(core,SYM_gObjectEvents+islandId*ABI_FOLLOW_OBJECT_SIZE)&1)!=0);
    CHECK(core->busRead8(core,SYM_gObjectEvents+islandId*ABI_FOLLOW_OBJECT_SIZE+4)
          ==CALL(FollowerGraphics_GetSpriteId));
    CALL(FollowerGraphics_DetachForMapTransition);
    CHECK((core->busRead8(core,SYM_gObjectEvents+islandId*ABI_FOLLOW_OBJECT_SIZE)&1)==0);
    printf("PASS: PRODUCTION owned ObjectEvent released before Route5->FourIsland warp and rebound after reload\n");
}

static void followerProductionOwnerProbe(void) {
    CHECK(core->loadState(core,bootState));
    CALL(ResetSpriteData);
    CALL(FollowerGraphics_InvalidateAfterSpriteReset);
    CHECK(CALL(FollowerGraphics_NativeSmall,120,86,2)==0);
    CHECK(CALL(FollowerGraphics_GetSize)==32);
    uint32_t small=CALL(FollowerGraphics_GetSpriteId);
    CHECK(small<64 && followerSpriteSize(small)==2);
    uint32_t smallTile=followerSpriteTile(small);
    uint32_t smallPalette=CALL(FollowerGraphics_GetPaletteSlot);
    CHECK(smallPalette<16);
    CHECK(CALL(FollowerGraphics_NativeSteelix,120,86,2)==0);
    CHECK(CALL(FollowerGraphics_GetSize)==64);
    uint32_t big=CALL(FollowerGraphics_GetSpriteId);
    CHECK(big<64 && big!=small && followerSpriteSize(big)==3);
    CHECK(CALL(FollowerGraphics_GetPaletteSlot)<16);
    for(unsigned i=0;i<16;i++) CHECK(CALL(SpriteTileAllocBitmapOp,smallTile+i,2)==0);
    uint32_t largeTile=followerSpriteTile(big);
    for(unsigned i=0;i<64;i++) CHECK(CALL(SpriteTileAllocBitmapOp,largeTile+i,2)!=0);
    for(unsigned frame=0;frame<6;frame++) {
        CHECK(CALL(FollowerGraphics_SetFrame,frame)!=0);
        for(unsigned i=0;i<2048;i+=2) {
            uint16_t expected=followerSteelixFrames[frame][i]|(followerSteelixFrames[frame][i+1]<<8);
            CHECK(core->busRead16(core,0x06010000U+largeTile*32+i)==expected);
        }
    }
    CHECK(CALL(FollowerGraphics_NativeSmall,120,86,2)==0);
    uint32_t back=CALL(FollowerGraphics_GetSpriteId);
    CHECK(back<64 && followerSpriteSize(back)==2);
    for(unsigned i=0;i<64;i++) CHECK(CALL(SpriteTileAllocBitmapOp,largeTile+i,2)==0);
    CALL(FollowerGraphics_Release);
    CHECK(CALL(FollowerGraphics_GetSpriteId)==64);
    printf("PASS: PRODUCTION graphics owner 32->Steelix64->32, six native frame tiles, palette and cleanup\n");

    // Exhaust tiles with actual ROM sprite objects while owner holds its
    // normal 32x32 slot, then check failure is truly no-op for old resources.
    CALL(ResetSpriteData);
    CALL(FollowerGraphics_InvalidateAfterSpriteReset);
    CHECK(CALL(FollowerGraphics_NativeSmall,110,90,1)==0);
    uint32_t kept=CALL(FollowerGraphics_GetSpriteId);
    uint32_t keptTile=followerSpriteTile(kept);
    uint32_t keptPal=CALL(FollowerGraphics_GetPaletteSlot);
    uint32_t blockers[24];unsigned count=0;
    for(;count<24;count++) {
        uint32_t id=CALL(CreateVirtualObject,108,30+count,11,10,0,1);
        if(id>=64) break;
        blockers[count]=id;
    }
    CHECK(count>=10 && count<24);
    CHECK(CALL(FollowerGraphics_NativeSteelix,110,90,1)==3);
    CHECK(CALL(FollowerGraphics_GetSpriteId)==kept);
    CHECK(CALL(FollowerGraphics_GetSize)==32);
    CHECK(CALL(FollowerGraphics_GetPaletteSlot)==keptPal);
    for(unsigned i=0;i<16;i++) CHECK(CALL(SpriteTileAllocBitmapOp,keptTile+i,2)!=0);
    CALL(DestroySprite,SYM_gSprites+blockers[count-1]*ABI_FOLLOW_SPRITE_SIZE);
    CHECK(CALL(FollowerGraphics_NativeSteelix,110,90,1)==0);
    CHECK(CALL(FollowerGraphics_GetSize)==64);
    CALL(FollowerGraphics_Release);
    for(unsigned i=0;i<count-1;i++)
        CALL(DestroySprite,SYM_gSprites+blockers[i]*ABI_FOLLOW_SPRITE_SIZE);
    CHECK(CALL(AllocSpriteTiles,64)!=0xFFFFFFFFU);
    CALL(AllocSpriteTiles,0);
    printf("PASS: PRODUCTION 32->Steelix64 allocation failure preserved previous sprite and palette; retry freed all resources\n");

    daycareFixture(1);
    CALL(FollowerGraphics_InvalidateAfterSpriteReset);
    CHECK(CALL(FollowerGraphics_NativeSmall,100,80,1)==0);
    CHECK(CALL(FollowerGraphics_NativeSteelix,100,80,1)==0);
    CHECK(CALL(FollowerGraphics_SetFrame,3)!=0);
    CALL(FollowerGraphics_DetachForMapTransition);
    CHECK(CALL(FollowerGraphics_GetSpriteId)==64);
    CALL(SetWarpDestination,MAP_GROUP(MAP_FOUR_ISLAND_POKEMON_DAY_CARE),
         MAP_NUM(MAP_FOUR_ISLAND_POKEMON_DAY_CARE),0xFFFFFFFFU,3,6);
    CALL(WarpIntoMap);
    CALL(ResetInitialPlayerAvatarState);
    CALL(CB2_LoadMap);
    frames(360,0);
    CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
    CALL(FollowerGraphics_InvalidateAfterSpriteReset);
    CHECK(CALL(FollowerGraphics_NativeSteelix,100,80,1)==0);
    CHECK(CALL(FollowerGraphics_SetFrame,5)!=0);
    CALL(FollowerGraphics_Release);
    printf("PASS: PRODUCTION dedicated graphics owner detached/recreated after Route5->Four Island real map reload\n");
}

static void followerGraphicsFrameBufferProbe(void) {
    CHECK(core->loadState(core,bootState));
    CALL(ResetSpriteData);
    uint32_t id=CALL(CreateVirtualObject,108,9,0,0,0,1);
    CHECK(id<64);
    uint32_t info=SYM_gSprites+id*ABI_FOLLOW_SPRITE_SIZE+ABI_FOLLOW_OAM_OFFSET;
    CHECK((core->busRead16(core,info+2)>>14)==3);
    unsigned tile=core->busRead16(core,info+4)&1023;
    unsigned palette=(core->busRead16(core,info+4)>>12)&15;
    CHECK(tile+64<=1024);
    // Keep the sole test sprite in a stable visible spot with no background
    // or title-scene OAM interference.
    for(unsigned n=0;n<128;n++)
        core->busWrite16(core,0x07000000U+n*8,0x0200);
    core->busWrite16(core,0x07000000,48);
    core->busWrite16(core,0x07000002,80|(3<<14));
    core->busWrite16(core,0x07000004,tile|(palette<<12));
    core->busWrite16(core,0x05000000,0x7FFF); // white LCD backdrop
    core->busWrite16(core,0x04000050,0);       // no color blending
    core->busWrite16(core,0x04000000,0x1040);  // OBJ enabled, 1D 4bpp mapping
    // Halt ordinary game callbacks while letting mGBA's video hardware run.
    core->busWrite16(core,0x04000208,0); // IME off
    core->busWrite16(core,0x0203FFF8,0xE7FE); // Thumb infinite branch
    setreg("cpsr",0xBF);
    setreg("pc",0x0203FFF8);
    uint64_t hashes[6];
    unsigned counts[6];
    for(unsigned frame=0;frame<6;frame++) {
        for(unsigned i=0;i<2048;i+=2) {
            uint16_t word=followerSteelixFrames[frame][i]|(followerSteelixFrames[frame][i+1]<<8);
            core->busWrite16(core,0x06010000U+tile*32+i,word);
        }
        for(unsigned i=0;i<16;i++)
            core->busWrite16(core,0x05000200U+palette*32+i*2,followerSteelixNormalPalette[i]);
        core->runFrame(core);
        core->runFrame(core);
        hashes[frame]=followerGraphicsFrameHash(&counts[frame]);
        printf("Steelix GPU frame %u: visible_pixels=%u hash=%llu\n",frame,counts[frame],(unsigned long long)hashes[frame]);
        CHECK(counts[frame]>100 && counts[frame]<4096);
    }
    CHECK(hashes[0]!=hashes[2] && hashes[2]!=hashes[4]);
    for(unsigned i=0;i<2048;i+=2) {
        uint16_t word=followerSteelixFrames[0][i]|(followerSteelixFrames[0][i+1]<<8);
        core->busWrite16(core,0x06010000U+tile*32+i,word);
    }
    for(unsigned i=0;i<16;i++)
        core->busWrite16(core,0x05000200U+palette*32+i*2,followerSteelixShinyPalette[i]);
    core->runFrame(core);
    core->runFrame(core);
    unsigned shinyCount=0;
    uint64_t shinyHash=followerGraphicsFrameHash(&shinyCount);
    CHECK(shinyCount==counts[0]);
    CHECK(shinyHash!=hashes[0]);
    printf("PASS: mGBA GPU framebuffer rendered all six actual Steelix directional cells and distinguished Shiny palette\n");
}


"""
c=c.replace('#include "symbols.h"', '#include "symbols.h"\n#include "follower_steelix_frame.h"', 1)
c=c.replace(anchor, probe+"\n"+anchor)
anchor2="    if(getenv(\"LIGHTNING_BALL_WEATHER_ONLY\"))"
assert anchor2 in c
c=c.replace(anchor2, '    if(getenv("LIGHTNING_FOLLOWER_GFX_ONLY")){followerGraphicsNativeProbe();followerGraphicsObjectProbe();followerAtomicSwapNativeProbe();followerGraphicsMapTransitionProbe();followerProductionOwnerProbe();followerProductionEventProbe();followerGraphicsFrameBufferProbe();printf("PASS: %u focused follower graphics memory assertions\\n",checks);core->deinit(core);free(pixels);free(bootState);return 0;}\n'+anchor2, 1)
required_anchor="required += ['NextThunderWaveTarget','gAbsentBattlerFlags']"
assert required_anchor in p
p=p.replace(required_anchor, "required += ['AllocSpriteTiles','SpriteTileAllocBitmapOp','ResetSpriteData','CreateVirtualObject','BuildOamBuffer','LoadOam','DestroySprite','gSprites','SetWarpDestination','WarpIntoMap','ResetInitialPlayerAvatarState','CB2_LoadMap','CB2_Overworld','FollowerGraphics_NativeSmall','FollowerGraphics_NativeSteelix','FollowerGraphics_GetSpriteId','FollowerGraphics_GetSize','FollowerGraphics_GetPaletteSlot','FollowerGraphics_SetFrame','FollowerGraphics_Release','FollowerGraphics_DetachForMapTransition','FollowerGraphics_InvalidateAfterSpriteReset','FollowerGraphics_BindObjectEvent','FollowerGraphics_GetObjectEventId','gObjectEvents']\n"+required_anchor, 1)
# ABI offsets are compiled by the game's own ARM compiler, never host guessed.
abi_anchor = "expressions = {"
assert abi_anchor in p
p = p.replace(abi_anchor, """expressions = {
 'FOLLOW_SPRITE_SIZE': 'sizeof(struct Sprite)',
 'FOLLOW_OAM_OFFSET': 'offsetof(struct Sprite,oam)',
 'FOLLOW_OBJECT_SIZE': 'sizeof(struct ObjectEvent)',""", 1)
source_anchor = "source='#include \"save.h\""
assert source_anchor in p
p = p.replace(source_anchor, "source='#include \"sprite.h\"\\n#include \"save.h\"", 1)
# These fixture symbols only exist in the temporary current-main checkout.
# No copyrighted/pinned sprite bytes or project ROM are committed.
production = Path("src/follower_graphics.c")
asset_fixture = """
#include "../tests/emulator/follower_steelix_frame.h"
static const struct FollowerGraphicsAsset sNativeSmallAsset = {
    .frames = {
        followerTestSmallFrames[0], followerTestSmallFrames[1],
        followerTestSmallFrames[2], followerTestSmallFrames[3],
        followerTestSmallFrames[4], followerTestSmallFrames[5],
    },
    .palette = followerTestSmallNormalPalette,
    .size = 32,
};
static const struct FollowerGraphicsAsset sNativeSteelixAsset = {
    .frames = {
        followerSteelixFrames[0], followerSteelixFrames[1],
        followerSteelixFrames[2], followerSteelixFrames[3],
        followerSteelixFrames[4], followerSteelixFrames[5],
    },
    .palette = followerSteelixNormalPalette,
    .size = 64,
};
enum FollowerGraphicsResult FollowerGraphics_NativeSmall(s16 x, s16 y, u8 priority) {
    return FollowerGraphics_Swap(&sNativeSmallAsset,x,y,priority);
}
enum FollowerGraphicsResult FollowerGraphics_NativeSteelix(s16 x, s16 y, u8 priority) {
    return FollowerGraphics_Swap(&sNativeSteelixAsset,x,y,priority);
}
"""
production.write_text(production.read_text()+"\n"+asset_fixture)
runner.write_text(c)
bridge.write_text(p)
print("INJECTED: read-only-to-source CI-only ARM allocator probe; no gameplay change")
