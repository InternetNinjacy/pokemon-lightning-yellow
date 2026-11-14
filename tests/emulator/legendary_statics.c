// Issue #234: real compiled map/object and legendary-battle assertions.
// These tests stage post-League flags and map entry. This does not certify a
// complete uninterrupted story playthrough or normal geographical access.
static void legendaryStationaryNativeTests(void)
{
 static const unsigned maps[]={MAP_ROUTE10, MAP_MT_EMBER_SUMMIT_PATH_1F,
                               MAP_SEAFOAM_ISLANDS_B4F, MAP_SEVEN_ISLAND_TANOBY_RUINS};
 static const unsigned localIds[]={11,1,5,5};
 static const unsigned x[]={8,4,30,105}, y[]={41,8,9,12};
 static const unsigned approachX[]={8,3,29,104}, approachY[]={42,8,9,12};
 static const unsigned species[]={SPECIES_RAIKOU,SPECIES_ENTEI,SPECIES_SUICUNE,SPECIES_MEW};
 static const unsigned fought[]={FLAG_FOUGHT_LIGHTNING_RAIKOU,FLAG_FOUGHT_LIGHTNING_ENTEI,
                                 FLAG_FOUGHT_LIGHTNING_SUICUNE,FLAG_FOUGHT_LIGHTNING_MEW};
 static const unsigned hide[]={FLAG_HIDE_LIGHTNING_RAIKOU,FLAG_HIDE_LIGHTNING_ENTEI,
                               FLAG_HIDE_LIGHTNING_SUICUNE,FLAG_HIDE_LIGHTNING_MEW};
 static const unsigned scripts[]={SYM_Route10_EventScript_LightningRaikou,
                                  SYM_MtEmber_SummitPath_1F_EventScript_LightningEntei,
                                  SYM_SeafoamIslands_B4F_EventScript_LightningSuicune,
                                  SYM_SevenIsland_TanobyRuins_EventScript_LightningMew};

 for(unsigned i=0;i<4;i++){
  CHECK(core->loadState(core,bootState));
  daycareFixture(1);
  CALL(FlagClear,FLAG_SYS_GAME_CLEAR);
  CALL(FlagClear,fought[i]);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),approachX[i],approachY[i]);
  CHECK(CALL(FlagGet,hide[i]));
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))==ABI_OBJECT_COUNT);
  // New-League completion must reveal the single authored stationary object.
  CALL(FlagSet,FLAG_SYS_GAME_CLEAR);
  fossilWarp(MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE),MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE),3,6);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),approachX[i],approachY[i]);
  CHECK(!CALL(FlagGet,hide[i]));
  CHECK(!CALL(FlagGet,fought[i]));
  unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
  CHECK(object<ABI_OBJECT_COUNT);
  CHECK(CALL(GetObjectEventIdByXY,x[i]+7,y[i]+7)==object);
  CHECK(CALL(MapGridGetCollisionAt,x[i]+7,y[i]+7)==0);
  // Execute the actual assembled event script; no fake encounter allocation.
  CALL(CreateMon,player(0),SPECIES_JOLTEON,100,31,1,12345,0,1234);
  core->busWrite8(core,SYM_gSelectedObjectEvent,object);
  CALL(VarSet,VAR_LAST_TALKED,localIds[i]);
  fossilStart(scripts[i]);
  waitAction();
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(monget(enemy(0),ABI_DATA_LEVEL)==50);
  weatherRun();
  waitOverworld(2);
  // The original static-legendary Run path announces that the mon flew away.
  // Advance that mandatory text using actual A-button input.
  for(unsigned f=0;f<2400&&!fieldScriptDone();f++)frames(1,(f%12)<3?1:0);
  if(!fieldScriptDone()) fprintf(stderr,"LEGENDARY event%u after Run: outcome=%u fought=%u hide=%u playerLocked=%u\\n",i,core->busRead8(core,SYM_gBattleOutcome),CALL(FlagGet,fought[i]),CALL(FlagGet,hide[i]),CALL(ArePlayerFieldControlsLocked));
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);
  CHECK(!CALL(ArePlayerFieldControlsLocked));
  CHECK(!CALL(FlagGet,fought[i]));
  // Running cannot consume the owner's capture opportunity.
  fossilWarp(MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE),MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE),3,6);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),approachX[i],approachY[i]);
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))<ABI_OBJECT_COUNT);
  // Independent one-time flags must survive flash save/continue and reentry.
  CALL(FlagSet,fought[i]);
  flashRoundTrip();
  CHECK(CALL(FlagGet,fought[i]));
  fossilWarp(MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE),MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE),3,6);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),approachX[i],approachY[i]);
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))==ABI_OBJECT_COUNT);
  printf("PASS native legendary %u: pre-League hidden, post-League object, real Lv50 battle, Run/retry and persisted resolved flag\\n",i);
 }
 CHECK(core->loadState(core,bootState));
}
