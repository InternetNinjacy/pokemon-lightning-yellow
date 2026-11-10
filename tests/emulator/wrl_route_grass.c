// WRL-009: native map grass, compiled encounter tables, and player-driven steps.
// A controlled warp to a grass patch is NOT proof of a natural route from the
// previous town. These tests deliberately leave that separate acceptance gate open.
static void wrlRouteGrassFixture(unsigned group,unsigned map,unsigned x,unsigned y) {
    // Reuse the native overworld fixture with two healthy level-50 Pokemon.
    wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);
    // The map load can relocate save blocks via MoveSaveBlocks_ResetHeap.
    // The tests below must read gSaveBlock1Ptr, never the old fixed buffer.
    CALL(SetWarpDestination,group,map,0xFFFFFFFF,x,y);
    CALL(WarpIntoMap);
    CALL(ResetInitialPlayerAvatarState);
    CALL(CB2_LoadMap);
    frames(360,0);
    CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
}
static uint32_t wrlLiveSaveBlock1(void) {
    return core->busRead32(core,SYM_gSaveBlock1Ptr);
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
    unsigned previousY=7,transitionLogs=0;
    for(frame=0;frame<3000;frame++) {
        if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)!=SYM_CB2_Overworld)
            break;
        frames(1,128); // GBA DOWN is bit 7 (0x80); 0x40 is UP into Cerulean City.
        CALL(PlayerGetDestCoords,scratch,scratch+2);
        unsigned currentX=core->busRead16(core,scratch);
        unsigned currentY=core->busRead16(core,scratch+2);
        CHECK(currentX==gridX);
        // Bounded native coordinate trace: a map transition or coordinate jump
        // can skip the y=10..15 sampling window without implying a blocked ledge.
        if ((frame<5 || currentY!=previousY) && transitionLogs<24) {
            fprintf(stderr,"WRL Route5 north coordinate frame=%u grid=(%u,%u) previousY=%u overworld=%08x\n",
                    frame,currentX,currentY,previousY,core->busRead32(core,SYM_gMain+ABI_MAIN_CB2));
            transitionLogs++;
        }
        previousY=currentY;
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

// WRL-009: genuine overworld MAP CONNECTION from Cerulean City into Route 5.
// Unlike the northern ledge test, this fixture begins in Cerulean City, uses
// only player D-pad movement, and must observe the live saved map-group/map-num
// change before proceeding to the approved Route 5 grass. This does not prove
// the full new-game quest path to Cerulean City.
static void wrlCeruleanToRoute5ConnectionTests(void) {
    // Begin below Cerulean's Cut-tree chokepoint (26,32) on the sole
    // collision-free south lane (26,39). This isolates the actual map
    // connection; it does NOT prove that the player can reach this point
    // naturally before Cut or through the full story route.
    // Earlier apparent Route 1 readings were from a stale save-block buffer,
    // not evidence of a spontaneous transition to Route 5.
    const unsigned x=26, startY=33, gridX=x+7;
    wrlRouteGrassFixture(MAP_GROUP(MAP_CERULEAN_CITY),
                         MAP_NUM(MAP_CERULEAN_CITY),x,startY);
    // LoadMap relocates SaveBlock1. Read the live pointer, not the stale
    // initial buffer (which still contains Route 1, map 3.19).
    CHECK(wrlLiveSaveBlock1()>=0x02000000 && wrlLiveSaveBlock1()+ABI_SAVE1_SIZE<=ABI_EWRAM_END);
    CALL(PlayerGetDestCoords,scratch,scratch+2);
    fprintf(stderr,"WRL Cerulean fixture expected=(%u,%u) actual=(%u,%u) start=(%u,%u) grid=(%u,%u) cb2=%08x\n",
            MAP_GROUP(MAP_CERULEAN_CITY),MAP_NUM(MAP_CERULEAN_CITY),
            core->busRead8(core,wrlLiveSaveBlock1()+4),
            core->busRead8(core,wrlLiveSaveBlock1()+5),x,startY,
            core->busRead16(core,scratch),core->busRead16(core,scratch+2),
            core->busRead32(core,SYM_gMain+ABI_MAIN_CB2));
    CHECK(core->busRead8(core,wrlLiveSaveBlock1()+4)==MAP_GROUP(MAP_CERULEAN_CITY));
    CHECK(core->busRead8(core,wrlLiveSaveBlock1()+5)==MAP_NUM(MAP_CERULEAN_CITY));
    CALL(PlayerGetDestCoords,scratch,scratch+2);
    CHECK(core->busRead16(core,scratch)==gridX);
    CHECK(core->busRead16(core,scratch+2)==startY+7);
    // Diagnose the *actual* southern walking lane without changing map data.
    // A fixed DOWN input can be blocked by a city tile even when the map
    // connection itself is valid. Probe the native loaded metatile collision.
    for(unsigned lane=18;lane<=33;lane++) {
        unsigned blocked=0;
        for(unsigned gy=38;gy<=46;gy++)
            if(CALL(MapGridGetCollisionAt,lane+7,gy))
                blocked|=1U<<(gy-38);
        fprintf(stderr,"WRL Cerulean south lane x=%u collisionRows38to46=%03x\\n",lane,blocked);
    }
    fprintf(stderr,"WRL Cerulean chosen lane x=%u nextTileCollision=%u\\n",
            x,CALL(MapGridGetCollisionAt,gridX,startY+9));
    unsigned transitioned=0,grassReached=0,frame,routeFrames=0;
    unsigned lastGroup=MAP_GROUP(MAP_CERULEAN_CITY),lastMap=MAP_NUM(MAP_CERULEAN_CITY);
    for(frame=0;frame<3000;frame++) {
        frames(1,128); // Real GBA DOWN (0x80), not an injected map warp.
        unsigned group=core->busRead8(core,wrlLiveSaveBlock1()+4);
        unsigned map=core->busRead8(core,wrlLiveSaveBlock1()+5);
        if((group!=lastGroup || map!=lastMap) && frame<1800)
            fprintf(stderr,"WRL Cerulean->Route5 native map change frame=%u (%u,%u)->(%u,%u)\n",
                    frame,lastGroup,lastMap,group,map);
        lastGroup=group;lastMap=map;
        if(group==MAP_GROUP(MAP_ROUTE5) && map==MAP_NUM(MAP_ROUTE5))
            transitioned=1;
        if(!transitioned)continue;
        routeFrames++;
        if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)!=SYM_CB2_Overworld)
            continue;
        CALL(PlayerGetDestCoords,scratch,scratch+2);
        unsigned gx=core->busRead16(core,scratch);
        unsigned gy=core->busRead16(core,scratch+2);
        if(gx==gridX && wrlRouteGrassAt(gx,gy)) {
            grassReached=1;
            fprintf(stderr,"WRL Cerulean->Route5 grass grid=(%u,%u) frame=%u\n",gx,gy,frame);
            break;
        }
    }
    core->setKeys(core,0);
    if(!transitioned || !grassReached) {
        CALL(PlayerGetDestCoords,scratch,scratch+2);
        fprintf(stderr,"WRL Cerulean->Route5 failure transition=%u grass=%u frames=%u routeFrames=%u map=(%u,%u) grid=(%u,%u) cb2=%08x\n",
                transitioned,grassReached,frame,routeFrames,lastGroup,lastMap,
                core->busRead16(core,scratch),core->busRead16(core,scratch+2),
                core->busRead32(core,SYM_gMain+ABI_MAIN_CB2));
    }
    CHECK(transitioned);
    CHECK(grassReached);
    printf("PASS: native Cerulean City -> Route 5 map connection and player D-pad traversal into approved grass in %u frames\n",frame);
    CHECK(core->loadState(core,bootState));
}


// WRL-009: verify a genuine, player-driven Lavender Town -> Route 8 map
// connection. This fixture is deliberately staged in Lavender Town near the
// west edge. It is NOT proof of normal new-game access to Lavender Town, nor
// a proof that the Route 8 grass can be reached from the arrival tile.
// Route 8 grass and its approved Drowzee draws are separately tested above.
static void wrlLavenderToRoute8ConnectionTests(void) {
    // Try several city-edge rows because not every row is walkable. Each
    // candidate uses the live collision map and actual LEFT D-pad input;
    // neither the route transition nor a battle is injected by the test.
    const unsigned candidateY[]={10,11,12,9,13,8,14,7,15,6,16,5,17,4,18,3};
    unsigned transitioned=0,selectedY=0,selectedFrame=0,attempts=0;
    for(unsigned i=0;i<sizeof(candidateY)/sizeof(candidateY[0]);i++) {
        unsigned y=candidateY[i],gy=y+7;
        wrlRouteGrassFixture(MAP_GROUP(MAP_LAVENDER_TOWN),
                            MAP_NUM(MAP_LAVENDER_TOWN),1,y);
        CHECK(wrlLiveSaveBlock1()>=0x02000000
           && wrlLiveSaveBlock1()+ABI_SAVE1_SIZE<=ABI_EWRAM_END);
        CHECK(core->busRead8(core,wrlLiveSaveBlock1()+4)==MAP_GROUP(MAP_LAVENDER_TOWN));
        CHECK(core->busRead8(core,wrlLiveSaveBlock1()+5)==MAP_NUM(MAP_LAVENDER_TOWN));
        unsigned edge=CALL(MapGridGetCollisionAt,7,gy);
        unsigned near=CALL(MapGridGetCollisionAt,8,gy);
        unsigned inner=CALL(MapGridGetCollisionAt,9,gy);
        fprintf(stderr,"WRL Lavender west candidate y=%u collision grid x7/8/9=%u/%u/%u\n",
                y,edge,near,inner);
        if(near || inner)continue;
        attempts++;
        for(unsigned frame=0;frame<900;frame++) {
            frames(1,32); // GBA LEFT is bit 5 (0x20).
            unsigned group=core->busRead8(core,wrlLiveSaveBlock1()+4);
            unsigned map=core->busRead8(core,wrlLiveSaveBlock1()+5);
            if(group==MAP_GROUP(MAP_ROUTE8) && map==MAP_NUM(MAP_ROUTE8)) {
                transitioned=1;selectedY=y;selectedFrame=frame;
                fprintf(stderr,"WRL Lavender->Route8 live map transition at y=%u frame=%u\n",
                        y,frame);
                break;
            }
            // Stop rather than accidentally accepting an unrelated map.
            if(group!=MAP_GROUP(MAP_LAVENDER_TOWN)
               || map!=MAP_NUM(MAP_LAVENDER_TOWN)) {
                fprintf(stderr,"WRL Lavender candidate y=%u entered unexpected map=(%u,%u)\n",
                        y,group,map);
                break;
            }
        }
        core->setKeys(core,0);
        if(transitioned)break;
    }
    if(!transitioned)
        fprintf(stderr,"WRL Lavender->Route8 transition not reached in %u walkable candidate lanes\n",attempts);
    CHECK(transitioned);
    printf("PASS: native Lavender Town -> Route 8 player LEFT D-pad map connection at y=%u in %u frames\n",
           selectedY,selectedFrame);
    CHECK(core->loadState(core,bootState));
}


// Trainer sight on Route 8 is part of natural access. Advance its real
// field dialogue, battle menu, victory and return; never set trainer flags.
static void wrlRoute8FinishTrainerBattle(void) {
    unsigned started=0,won=0,quiet=0,f;
    for(f=0;f<36000;f++) {
        unsigned cb=core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U;
        if(cb==SYM_BattleMainCB2) {
            started=1;
            CHECK(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_TRAINER);
        }
        if(started && core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_WON)
            won=1;
        if(won && fieldScriptDone()) {
            if(++quiet==30)break;
            frames(1,0);
            continue;
        }
        quiet=0;
        // Bounded native diagnostics: distinguish menu-input deadlock from
        // ineffective attacks or trainer-script transition without bypasses.
        if(f==0 || f==600 || f==3000 || f==9000 || f==18000 || f==27000) {
            fprintf(stderr,"WRL Route8 battle trace frame=%u started=%u won=%u cb=%08x controller=%08x exec=%08x outcome=%u playerHP=%u enemyHP=%u actionCursor=%u moveCursor=%u trainer=%u bufferCmd=%u textActive=%u textState=%u scriptPtr=%08x scriptOp=%u expState=%u\\n",
                    f,started,won,cb,controller(0),
                    core->busRead32(core,SYM_gBattleControllerExecFlags),
                    core->busRead8(core,SYM_gBattleOutcome),
                    core->busRead16(core,bmon(0)+ABI_BMON_HP),
                    core->busRead16(core,bmon(1)+ABI_BMON_HP),
                    core->busRead8(core,SYM_gActionSelectionCursor),
                    core->busRead8(core,SYM_gMoveSelectionCursor),
                    core->busRead16(core,SYM_gTrainerBattleOpponent_A),
                    core->busRead8(core,SYM_gBattleBufferA),
                    core->busRead8(core,SYM_sTextPrinters+ABI_TEXT_ACTIVE),
                    core->busRead8(core,SYM_sTextPrinters+ABI_TEXT_STATE),
                    core->busRead32(core,SYM_gBattlescriptCurrInstr),
                    core->busRead8(core,core->busRead32(core,SYM_gBattlescriptCurrInstr)),
                    core->busRead8(core,SYM_gBattleScripting+ABI_SCRIPT_EXP_STATE));
        }
        unsigned key=0;
        if(f%8==0) {
            key=1; // Real trainer pre/post-battle dialogue.
            // CONTROLLER_CHOOSEPOKEMON (22) opens the actual party menu.
            // After the foe faints, A repeatedly selects the active mon and
            // cannot dismiss the optional switch prompt. Press B to decline
            // switching and return to the real trainer battle instead.
            if(started && cb!=SYM_BattleMainCB2 && cb!=SYM_CB2_Overworld
               && core->busRead8(core,SYM_gBattleBufferA)==22)
                key=2;
            else if(cb==SYM_BattleMainCB2
               && (core->busRead32(core,SYM_gBattleControllerExecFlags)&1U)) {
                unsigned fn=controller(0);
                if(fn==SYM_HandleInputChooseAction) {
                    unsigned c=core->busRead8(core,SYM_gActionSelectionCursor);
                    key=c&2?64:c&1?32:1;
                } else if(fn==SYM_HandleInputChooseMove) {
                    unsigned c=core->busRead8(core,SYM_gMoveSelectionCursor);
                    key=c&2?64:c&1?32:1;
                } else if(fn==SYM_HandleInputChooseTarget)key=1;
                else {
                    // After a real knockout, advance active mandatory battle
                    // text with A; keep B for other non-menu prompts.
                    unsigned fainted=core->busRead16(core,bmon(1)+ABI_BMON_HP)==0;
                    unsigned textActive=core->busRead8(core,SYM_sTextPrinters+ABI_TEXT_ACTIVE);
                    key=(fainted && textActive)?1:2;
                }
            }
        }
        frames(1,key);
    }
    if(f==36000)
        fprintf(stderr,"WRL Route8 trainer timeout started=%u won=%u callback=%08x outcome=%u controls=%u script=%u\n",
                started,won,core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),
                core->busRead8(core,SYM_gBattleOutcome),
                CALL(ArePlayerFieldControlsLocked),CALL(ScriptContext_IsEnabled));
    CHECK(started && won && f<36000);
    CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
    printf("PASS: native Route 8 trainer sight interruption resolved by real battle victory in %u frames\n",f);
}

// WRL-009: follow the Lavender -> Route 8 connection with a real player
// walk to approved grass. Actual unavoidable trainer battles must be won
// with native inputs; no trainer flags, teleports, or map data are altered.
// This remains a staged Lavender fixture, not proof of new-game progression.
static void wrlLavenderToRoute8GrassWalkTests(void) {
    // A single west-edge entry row can be separated from grass by real
    // trainers. Probe each authored Lavender west connection row rather than
    // treating one staged entry as the only possible natural approach.
    // The selected entry must still cross via actual LEFT D-pad movement.
    enum { W=90,H=40,N=W*H,MAX_REPLANS=24 };
    unsigned char blocked[N]={0};
    // Block only actual trainer objects, not their sight lines. A sight
    // encounter must be resolved by a real trainer battle, not a flag edit.
    blocked[21*W+69]=1; // Julia, source (62,14) plus 7-tile border.
    blocked[22*W+62]=1; // Rich, source (55,15) plus border.
    const unsigned candidateY[]={10,9,11,8,12,7,13,6,14,5,15,4,16,3,17,18};
    unsigned transitionFrame=0,selectedY=0,selected=0,transitionCount=0;
    for(unsigned i=0;i<sizeof(candidateY)/sizeof(candidateY[0]);i++) {
        unsigned y=candidateY[i],transitioned=0,frame;
        wrlRouteGrassFixture(MAP_GROUP(MAP_LAVENDER_TOWN),
                            MAP_NUM(MAP_LAVENDER_TOWN),1,y);
        for(frame=0;frame<900;frame++) {
            frames(1,32); // Real LEFT D-pad; never warp into Route 8.
            unsigned group=core->busRead8(core,wrlLiveSaveBlock1()+4);
            unsigned map=core->busRead8(core,wrlLiveSaveBlock1()+5);
            if(group==MAP_GROUP(MAP_ROUTE8) && map==MAP_NUM(MAP_ROUTE8)) {
                transitioned=1;break;
            }
            if(group!=MAP_GROUP(MAP_LAVENDER_TOWN)
               || map!=MAP_NUM(MAP_LAVENDER_TOWN))break;
        }
        core->setKeys(core,0);
        if(!transitioned) {
            fprintf(stderr,"WRL Lavender->Route8 grass candidate y=%u no map transition in %u frames\\n",y,frame);
            continue;
        }
        transitionCount++;
        frames(120,0);
        CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
        CHECK(core->busRead8(core,wrlLiveSaveBlock1()+4)==MAP_GROUP(MAP_ROUTE8));
        CHECK(core->busRead8(core,wrlLiveSaveBlock1()+5)==MAP_NUM(MAP_ROUTE8));
        CALL(PlayerGetDestCoords,scratch,scratch+2);
        unsigned sx=core->busRead16(core,scratch),sy=core->busRead16(core,scratch+2);
        CHECK(sx>=7 && sx<79 && sy>=7 && sy<27);
        // Diagnose collision-connected grass while excluding trainer sight.
        // A candidate is accepted only if a full native D-pad walk can then
        // prove it, not merely because this topological probe finds a path.
        unsigned char seen[N]={0};
        unsigned queue[N],front=0,back=0,start=sy*W+sx,grass=0;
        seen[start]=1;queue[back++]=start;
        while(front<back) {
            unsigned cur=queue[front++],x=cur%W,cy=cur/W;
            if(wrlRouteGrassAt(x,cy)){grass=1;break;}
            const int ddx[4]={0,0,-1,1},ddy[4]={-1,1,0,0};
            for(unsigned d=0;d<4;d++) {
                int nx=(int)x+ddx[d],ny=(int)cy+ddy[d];
                if(nx<7 || nx>=79 || ny<7 || ny>=27)continue;
                unsigned next=(unsigned)ny*W+(unsigned)nx;
                if(seen[next] || blocked[next]
                   || CALL(MapGridGetCollisionAt,nx,ny))continue;
                seen[next]=1;queue[back++]=next;
            }
        }
        fprintf(stderr,"WRL Lavender->Route8 grass candidate y=%u arrival=(%u,%u) grassPath=%u examined=%u transitionFrame=%u\\n",
                y,sx,sy,grass,front,frame);
        if(grass){selected=1;selectedY=y;transitionFrame=frame;break;}
    }
    if(!selected)
        fprintf(stderr,"WRL Lavender->Route8: no trainer-sight-free grass path from %u successful Lavender west-entry rows; real trainer battles may be required\\n",transitionCount);
    CHECK(selected);
    fprintf(stderr,"WRL Lavender->Route8 selected west entry y=%u after %u frames\\n",
            selectedY,transitionFrame);
    // Strengthen the staged party for legitimate trainer battles. This
    // changes only the test's player party, never ROM encounter data.
    CALL(CreateMon,player(0),SPECIES_MEWTWO,100,31,1,12345,0,1234);
    monput(player(0),ABI_DATA_MOVE1,MOVE_PSYCHIC);
    monput(player(0),ABI_DATA_PP1,99);
    monput(player(0),ABI_DATA_MOVE2,MOVE_SWIFT);
    monput(player(0),ABI_DATA_PP2,99);
    const int dx[4]={0,0,-1,1},dy[4]={-1,1,0,0};
    const unsigned keys[4]={64,128,32,16};
    unsigned totalFrames=0,totalSteps=0,replans=0,reached=0,trainerWins=0;
    for(unsigned attempt=0;attempt<=MAX_REPLANS;attempt++) {
        CALL(PlayerGetDestCoords,scratch,scratch+2);
        unsigned sx=core->busRead16(core,scratch),sy=core->busRead16(core,scratch+2);
        CHECK(sx>=7 && sx<79 && sy>=7 && sy<27);
        if(wrlRouteGrassAt(sx,sy)){reached=1;break;}
        unsigned char visited[N]={0},directions[N]={0};
        int parent[N];
        unsigned queue[N],front=0,back=0,start=sy*W+sx,goal=N;
        visited[start]=1;parent[start]=-1;queue[back++]=start;
        while(front<back) {
            unsigned cur=queue[front++],x=cur%W,y=cur/W;
            if(wrlRouteGrassAt(x,y)){goal=cur;break;}
            for(unsigned d=0;d<4;d++) {
                int nx=(int)x+dx[d],ny=(int)y+dy[d];
                if(nx<7 || nx>=79 || ny<7 || ny>=27)continue;
                unsigned next=(unsigned)ny*W+(unsigned)nx;
                if(visited[next] || blocked[next]
                   || CALL(MapGridGetCollisionAt,nx,ny))continue;
                visited[next]=1;parent[next]=(int)cur;
                directions[next]=(unsigned char)keys[d];
                queue[back++]=next;
            }
        }
        fprintf(stderr,"WRL Lavender->Route8 grass BFS from=(%u,%u) reachable=%u examined=%u replans=%u transitionFrame=%u\n",
                sx,sy,goal!=N,front,replans,transitionFrame);
        CHECK(goal!=N);
        unsigned path[N],length=0;
        for(unsigned cur=goal;cur!=start;cur=(unsigned)parent[cur]) {
            CHECK(length<N);
            path[length++]=cur;
        }
        CHECK(length>0);
        unsigned needsReplan=0;
        for(unsigned step=length;step>0;step--) {
            unsigned target=path[step-1],tx=target%W,ty=target/W;
            unsigned key=directions[target],moved=0;
            CALL(PlayerGetDestCoords,scratch,scratch+2);
            unsigned prevX=core->busRead16(core,scratch);
            unsigned prevY=core->busRead16(core,scratch+2);
            for(unsigned frame=0;frame<96;frame++) {
                frames(1,key);
                totalFrames++;
                CALL(PlayerGetDestCoords,scratch,scratch+2);
                unsigned px=core->busRead16(core,scratch);
                unsigned py=core->busRead16(core,scratch+2);
                if(px==tx && py==ty){moved=1;break;}
                if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)!=SYM_CB2_Overworld)
                    break;
            }
            core->setKeys(core,0);
            // A trainer can intercept the player while walking toward the
            // planned tile; the actual engine must resolve that battle.
            if(CALL(ArePlayerFieldControlsLocked)
               || CALL(ScriptContext_IsEnabled)
               || (core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)!=SYM_CB2_Overworld) {
                fprintf(stderr,"WRL Route8 trainer interruption target=(%u,%u) moved=%u\n",tx,ty,moved);
                wrlRoute8FinishTrainerBattle();
                trainerWins++;
                replans++;
                needsReplan=1;
                break;
            }
            CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
            CHECK(core->busRead8(core,wrlLiveSaveBlock1()+4)==MAP_GROUP(MAP_ROUTE8));
            CHECK(core->busRead8(core,wrlLiveSaveBlock1()+5)==MAP_NUM(MAP_ROUTE8));
            if(!moved) {
                CALL(PlayerGetDestCoords,scratch,scratch+2);
                unsigned px=core->busRead16(core,scratch);
                unsigned py=core->busRead16(core,scratch+2);
                fprintf(stderr,"WRL Route8 object-blocked target=(%u,%u) actual=(%u,%u) prior=(%u,%u) metatileCollision=%u replan=%u\n",
                        tx,ty,px,py,prevX,prevY,
                        CALL(MapGridGetCollisionAt,tx,ty),replans+1);
                fprintf(stderr,"WRL Route8 stall fieldControlsLocked=%u scriptEnabled=%u\\n",
                        CALL(ArePlayerFieldControlsLocked),CALL(ScriptContext_IsEnabled));
                // Replan only if the attempted native step actually stayed
                // in place. Unexpected movement is a genuine test failure.
                CHECK(px==prevX && py==prevY);
                blocked[target]=1;
                replans++;
                needsReplan=1;
                frames(18,0);
                break;
            }
            totalSteps++;
            frames(18,0); // Finish the walking animation before next turn.
            if(wrlRouteGrassAt(tx,ty)){reached=1;break;}
            if(CALL(ArePlayerFieldControlsLocked) || CALL(ScriptContext_IsEnabled)) {
                wrlRoute8FinishTrainerBattle();
                trainerWins++;
                replans++;
                needsReplan=1;
                break;
            }
        }
        if(reached)break;
        CHECK(needsReplan);
    }
    CHECK(reached);
    CALL(PlayerGetDestCoords,scratch,scratch+2);
    CHECK(wrlRouteGrassAt(core->busRead16(core,scratch),
                          core->busRead16(core,scratch+2)));
    printf("PASS: native Lavender -> Route 8 connection followed by %u actual D-pad steps to approved grass in %u walking frames (%u replans, %u trainer victories)\n",
           totalSteps,totalFrames,replans,trainerWins);
    CHECK(core->loadState(core,bootState));
}
