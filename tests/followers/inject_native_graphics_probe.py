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
// the existing FireRed Pikachu (32x32) and Seagallop (64x64) ROM artwork.
static void followerGraphicsObjectProbe(void) {
    CHECK(core->loadState(core,bootState));
    CALL(ResetSpriteData);
    uint32_t small=CALL(CreateVirtualObject,120,1,10,10,0,1);
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
    printf("PASS: native mGBA Pikachu and Seagallop sprites both created with 32/64 OAM and allocated tiles\n");
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

"""
c=c.replace(anchor, probe+"\n"+anchor)
anchor2="    if(getenv(\"LIGHTNING_BALL_WEATHER_ONLY\"))"
assert anchor2 in c
c=c.replace(anchor2, '    if(getenv("LIGHTNING_FOLLOWER_GFX_ONLY")){followerGraphicsNativeProbe();followerGraphicsObjectProbe();printf"PASS: %u focused follower graphics memory assertions\\n",checks);core->deinit(core);free(pixels);free(bootState);return 0;}\n'+anchor2, 1)
required_anchor="required += ['NextThunderWaveTarget','gAbsentBattlerFlags']"
assert required_anchor in p
p=p.replace(required_anchor, "required += ['AllocSpriteTiles','SpriteTileAllocBitmapOp','ResetSpriteData','CreateVirtualObject','BuildOamBuffer','DestroySprite','gSprites']\n"+required_anchor,1)
# ABI offsets are compiled by the game's own ARM compiler, never host guessed.
abi_anchor = "expressions = {"
assert abi_anchor in p
p = p.replace(abi_anchor, """expressions = {
 'FOLLOW_SPRITE_SIZE': 'sizeof(struct Sprite)',
 'FOLLOW_OAM_OFFSET': 'offsetof(struct Sprite,oam)',""", 1)
source_anchor = "source='#include \"save.h\""
assert source_anchor in p
p = p.replace(source_anchor, "source='#include \"sprite.h\"\\n#include \"save.h\"", 1)
runner.write_text(c)
bridge.write_text(p)
print("INJECTED: read-only-to-source CI-only ARM allocator probe; no gameplay change")
