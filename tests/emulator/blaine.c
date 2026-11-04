// Integration acceptance for the separately owner-merged Blaine change.
static void blaineFixture(unsigned count) {
 daycareFixture(1);core->busWrite8(core,SYM_gPlayerPartyCount,count);
 for(unsigned i=count;i<6;i++)clearBytes(player(i),ABI_MON_SIZE);
 fossilWarp(MAP_GROUP(MAP_CINNABAR_ISLAND_GYM),MAP_NUM(MAP_CINNABAR_ISLAND_GYM),3,3);
 unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,8,MAP_NUM(MAP_CINNABAR_ISLAND_GYM),MAP_GROUP(MAP_CINNABAR_ISLAND_GYM));
 CHECK(object<ABI_OBJECT_COUNT);core->busWrite8(core,SYM_gSelectedObjectEvent,object);
}
static void blaineTests(void) {
 blaineFixture(1);fossilStart(SYM_CinnabarIsland_Gym_EventScript_Blaine);fossilClose(1);
 CHECK(!(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_TRAINER));
 CHECK(!CALL(FlagGet,FLAG_BADGE07_GET));CHECK(!CALL(FlagGet,FLAG_DEFEATED_BLAINE));
 blaineFixture(2);fossilStart(SYM_CinnabarIsland_Gym_EventScript_Blaine);waitAction();
 CHECK(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_TRAINER);
 CHECK(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_DOUBLE);
 CHECK(core->busRead8(core,SYM_gBattlersCount)==4);
 const unsigned species[]={SPECIES_DONPHAN,SPECIES_PIDGEOT,SPECIES_SANDSLASH,SPECIES_AERODACTYL,SPECIES_FLYGON};
 const unsigned levels[]={42,42,43,44,47};
 for(unsigned i=0;i<5;i++){CHECK(monget(enemy(i),ABI_DATA_SPECIES)==species[i]);CHECK(monget(enemy(i),ABI_DATA_LEVEL)==levels[i]);}
 CHECK(core->busRead16(core,bmon(1)+ABI_BMON_SPECIES)==SPECIES_DONPHAN);
 CHECK(core->busRead16(core,bmon(3)+ABI_BMON_SPECIES)==SPECIES_PIDGEOT);
 CHECK(core->busRead8(core,bmon(3)+ABI_BMON_TYPE1)==TYPE_FLYING);
 CHECK(core->busRead8(core,bmon(3)+ABI_BMON_TYPE2)==TYPE_FLYING);
 core->busWrite32(core,SYM_gBattleMoveDamage,100);CALL(TypeCalc,MOVE_EARTHQUAKE,1,3);
 CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==0);
 printf("PASS: actual Blaine NPC refuses one usable Pokemon, starts trainer Double with correct five-level party/Donphan-Pidgeot leads and native Earthquake ally immunity\n");
 // Deliberately modified damage fixture is discarded; no victory/return claim.
 CHECK(core->loadState(core,bootState));
}
