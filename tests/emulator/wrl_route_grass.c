// WRL-009: native map grass, compiled encounter tables, and player-driven steps.
// A controlled warp to a grass patch is NOT proof of a natural route from the
// previous town. These tests deliberately leave that separate acceptance gate open.
static void wrlRouteGrassFixture(unsigned group,unsigned map,unsigned x,unsigned y) {
    // Reuse the native overworld fixture with two healthy level-50 Pokemon.
    wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);
    CALL(SetWarpDestination,group,map,0xFFFFFFFF,x,y);
    CALL(WarpIntoMap);
    CALL(ResetInitialPlayerAvatarState);
    CALL(CB2_LoadMap);
    frames(360,0);
    CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
}
static unsigned wrlRouteGrassAt(unsigned x,unsigned y) {
    return CALL(MapGridGetMetatileAttributeAt,x,y,ABI_META_ENCOUNTER)==ABI_TILE_LAND
        && !CALL(MapGridGetCollisionAt,x,y);
}
static void wrlRouteApprovedMon(unsigned route,unsigned mon,unsigned level) {
    if(route==5) {
        CHECK(mon==SPECIES_RATTATA || mon==SPECIES_SKITTY || mon==SPECIES_DITTO);
        CHECK(level>=10 && level<=15);
    } else {
        CHECK(route==8);
        CHECK(mon==SPECIES_PONYTA || mon==SPECIES_HOUNDOUR
           || mon==SPECIES_DROWZEE || mon==SPECIES_CACNEA
           || mon==SPECIES_NATU);
        CHECK(level>=16 && level<=21);
    }
}
static void wrlRouteNativeGrassTests(void) {
    for(unsigned kind=0;kind<2;kind++) {
        const unsigned route=kind?8:5;
        const unsigned group=kind?MAP_GROUP(MAP_ROUTE8):MAP_GROUP(MAP_ROUTE5);
        const unsigned map=kind?MAP_NUM(MAP_ROUTE8):MAP_NUM(MAP_ROUTE5);
        const unsigned width=kind?72:48,height=kind?20:40;
        wrlRouteGrassFixture(group,map,kind?65:24,kind?8:18);
        unsigned tiles=0;
        for(unsigned y=7;y<height+7;y++)
            for(unsigned x=7;x<width+7;x++)tiles+=!!wrlRouteGrassAt(x,y);
        CHECK(tiles>0);
        CHECK(!CALL(TryDoDoubleWildBattle)); // Both routes are ordinary singles.
        unsigned info=landInfo(),found[3]={0};
        core->busWrite32(core,SYM_gRngValue,0xB6C50908U+route);
        for(unsigned i=0;i<1000;i++) {
            CHECK(CALL(TryStartLandEncounter,info,0));
            CHECK(!(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_DOUBLE));
            CHECK(monget(enemy(1),ABI_DATA_SPECIES)==SPECIES_NONE);
            unsigned species=monget(enemy(0),ABI_DATA_SPECIES);
            wrlRouteApprovedMon(route,species,monget(enemy(0),ABI_DATA_LEVEL));
            if(route==5) {
                found[0]+=species==SPECIES_RATTATA;
                found[1]+=species==SPECIES_SKITTY;
                found[2]+=species==SPECIES_DITTO;
            } else {
                found[0]+=species==SPECIES_DROWZEE;
            }
            CALL(ResetTasks); // Drop only the unexecuted trial transition.
        }
        CHECK(found[0]>0);
        if(route==5){CHECK(found[1]>0);CHECK(found[2]>0);}
        printf("PASS: Route %u compiled singles, %u non-colliding grass tiles, 1000 native encounter draws (targets %u/%u/%u)\n",
               route,tiles,found[0],found[1],found[2]);

        // Now use the real overworld walker. Find two neighboring ordinary
        // grass tiles, warp onto the first, and press the D-pad to trigger
        // an actual wild battle (no direct encounter function is called).
        wrlRouteGrassFixture(group,map,kind?65:24,kind?8:18);
        unsigned gx=0,gy=0,forward=0,backward=0;
        for(unsigned y=8;y<height+6 && !forward;y++)
            for(unsigned x=8;x<width+6 && !forward;x++) {
                if(!wrlRouteGrassAt(x,y))continue;
                if(wrlRouteGrassAt(x+1,y)){gx=x;gy=y;forward=16;backward=32;}
                else if(wrlRouteGrassAt(x,y+1)){gx=x;gy=y;forward=64;backward=128;}
            }
        CHECK(forward!=0);
        CALL(SetWarpDestination,group,map,0xFFFFFFFF,gx-7,gy-7);
        CALL(WarpIntoMap);
        CALL(ResetInitialPlayerAvatarState);
        CALL(CB2_LoadMap);
        frames(240,0);
        CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
        CALL(PlayerGetDestCoords,scratch,scratch+2);
        CHECK(wrlRouteGrassAt(core->busRead16(core,scratch),core->busRead16(core,scratch+2)));
        unsigned frame;
        for(frame=0;frame<4800;frame++) {
            frames(1,((frame/24)%2)?backward:forward);
            if(core->busRead8(core,SYM_gBattlersCount)==2
                && (core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)!=SYM_CB2_Overworld)
                break;
        }
        core->setKeys(core,0);
        CHECK(frame<4800);
        waitActionBudget(2400);
        CHECK(core->busRead8(core,SYM_gBattlersCount)==2);
        wrlRouteApprovedMon(route,monget(enemy(0),ABI_DATA_SPECIES),
                            monget(enemy(0),ABI_DATA_LEVEL));
        press(128);press(16);press(1);
        waitOverworld(1);
        CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);
        printf("PASS: Route %u player D-pad steps in real grass triggered and escaped native single battle after %u frames\n",
               route,frame);
    }
    CHECK(core->loadState(core,bootState));
}

// WRL-009: start on the *north edge* of Route 5, not inside the grass.
// The compiled map has a south-facing ledge at y=4. Drive the actual
// overworld walker through that ledge into the first approved grass row.
// This proves in-map movement from the northern entry corridor; it does NOT
// prove the separate Cerulean City -> Route 5 map-connection transition.
static void wrlRoute5NorthernLedgeWalkTests(void) {
    const unsigned x=23, gridX=x+7, ledgeGridY=4+7, firstGrassGridY=5+7;
    wrlRouteGrassFixture(MAP_GROUP(MAP_ROUTE5),MAP_NUM(MAP_ROUTE5),x,0);
    CALL(PlayerGetDestCoords,scratch,scratch+2);
    CHECK(core->busRead16(core,scratch)==gridX);
    CHECK(core->busRead16(core,scratch+2)==7);
    CHECK(!wrlRouteGrassAt(gridX,7));
    CHECK(CALL(MapGridGetCollisionAt,gridX,ledgeGridY));
    CHECK(wrlRouteGrassAt(gridX,firstGrassGridY));
    unsigned reached=0,highestY=7,seenRows=0,frame;
    for(frame=0;frame<3000;frame++) {
        if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)!=SYM_CB2_Overworld)
            break;
        frames(1,64); // Hold DOWN; do not warp or inject a wild encounter.
        CALL(PlayerGetDestCoords,scratch,scratch+2);
        unsigned currentX=core->busRead16(core,scratch);
        unsigned currentY=core->busRead16(core,scratch+2);
        CHECK(currentX==gridX);
        // Diagnose whether the walker visits the expected grass row and whether
        // native map attributes differ while standing on it (no map edits).
        if(currentY>=10 && currentY<=15 && !(seenRows & (1U<<(currentY-10)))) {
            seenRows |= 1U<<(currentY-10);
            fprintf(stderr,"WRL Route5 north walk row=%u encounter=%u collision=%u targetRowEncounter=%u targetRowCollision=%u\n",
                    currentY,CALL(MapGridGetMetatileAttributeAt,currentX,currentY,ABI_META_ENCOUNTER),
                    CALL(MapGridGetCollisionAt,currentX,currentY),
                    CALL(MapGridGetMetatileAttributeAt,gridX,firstGrassGridY,ABI_META_ENCOUNTER),
                    CALL(MapGridGetCollisionAt,gridX,firstGrassGridY));
        }
        if(currentY>highestY)highestY=currentY;
        if(wrlRouteGrassAt(currentX,currentY)) {reached=1;break;}
    }
    if (!reached) {
        CALL(PlayerGetDestCoords,scratch,scratch+2);
        fprintf(stderr,"WRL Route5 north-edge walk blocked: highest grid Y=%u, final grid=(%u,%u), frames=%u, CB2=%08x, expected grass Y=%u\\n",
                highestY,core->busRead16(core,scratch),core->busRead16(core,scratch+2),frame,
                core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),firstGrassGridY);
    }
    CHECK(reached);
    CHECK(highestY>=firstGrassGridY);
    printf("PASS: Route 5 native north-edge D-pad walk crossed south-facing collision ledge into approved grass in %u frames\n",frame);
    CHECK(core->loadState(core,bootState));
}
