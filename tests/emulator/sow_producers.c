// Verify the actual ARM scripted-wild producer's approved battle data and ability.
static void sowScriptedProducerTests(void)
{
 static const unsigned species[]={SPECIES_CORPHISH,SPECIES_GLALIE,SPECIES_TYRANITAR};
 static const unsigned levels[]={30,40,55};
 static const unsigned abilities[]={ABILITY_DRIZZLE,ABILITY_SNOW_WARNING,ABILITY_SAND_STREAM};
 static const unsigned moves[][4]={
  {MOVE_BUBBLE_BEAM,MOVE_KNOCK_OFF,MOVE_PROTECT,MOVE_VICE_GRIP},
  {MOVE_ICE_BEAM,MOVE_CRUNCH,MOVE_HEADBUTT,MOVE_PROTECT},
  {MOVE_CRUNCH,MOVE_ROCK_SLIDE,MOVE_THRASH,MOVE_SCARY_FACE},
 };
 daycareFixture(1);
 for(unsigned i=0;i<3;i++){
  CALL(CreateScriptedWildMon,species[i],levels[i],0);
  CALL(VarSet,VAR_0x8004,i);
  CALL(SowPrepareScriptedEncounter);
  CHECK(CALL(VarGet,VAR_RESULT)==1);
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(monget(enemy(0),ABI_DATA_LEVEL)==levels[i]);
  CHECK(monget(enemy(0),ABI_DATA_HELD_ITEM)==0);
  CHECK(CALL(GetMonAbility,enemy(0))==abilities[i]);
  for(unsigned j=0;j<4;j++){
   CHECK(monget(enemy(0),ABI_DATA_MOVE1+j)==moves[i][j]);
   CHECK(monget(enemy(0),ABI_DATA_PP1+j)>0);
  }
 }
 // Fail closed for invalid event selectors and mismatched levels/items,
 // even when the species itself is otherwise eligible.
 CALL(CreateScriptedWildMon,SPECIES_CORPHISH,30,0);
 CALL(VarSet,VAR_0x8004,99);
 CALL(SowPrepareScriptedEncounter);
 CHECK(CALL(VarGet,VAR_RESULT)==0);
 CALL(CreateScriptedWildMon,SPECIES_CORPHISH,29,0);
 CALL(VarSet,VAR_0x8004,0);
 CALL(SowPrepareScriptedEncounter);
 CHECK(CALL(VarGet,VAR_RESULT)==0);
 CALL(CreateScriptedWildMon,SPECIES_CORPHISH,30,ITEM_POTION);
 CALL(SowPrepareScriptedEncounter);
 CHECK(CALL(VarGet,VAR_RESULT)==0);
 CALL(CreateScriptedWildMon,SPECIES_PIDGEY,10,0);
 CALL(VarSet,VAR_0x8004,0);
 CALL(SowPrepareScriptedEncounter);
 CHECK(CALL(VarGet,VAR_RESULT)==0);
 printf("PASS: actual scripted Corphish/Glalie/Tyranitar created at approved levels, all 12 exact moves and PP, no items, individual Drizzle/Snow Warning and native Sand Stream; invalid species fails closed\n");
 CHECK(core->loadState(core,bootState));
}


// Execute the real compiled event entry points, rather than only calling
// the isolated mon constructor. A-button pathfinding and visual OAM checks
// require the separate in-map interaction/framebuffer acceptance gate.
static void sowScriptedRunTests(void)
{
 static const unsigned maps[]={MAP_ROUTE12,MAP_ROUTE21_NORTH,MAP_ROUTE23};
 static const unsigned localIds[]={15,7,8};
 static const unsigned startX[]={19,13,13},startY[]={88,8,146};
 static const unsigned scripts[]={SYM_Route12_EventScript_SowCorphish,
                                  SYM_Route21_North_EventScript_SowGlalie,
                                  SYM_Route23_EventScript_SowTyranitar};
 static const unsigned species[]={SPECIES_CORPHISH,SPECIES_GLALIE,SPECIES_TYRANITAR};
 static const unsigned abilities[]={ABILITY_DRIZZLE,ABILITY_SNOW_WARNING,ABILITY_SAND_STREAM};
 static const unsigned active[]={FLAG_SOW_CORPHISH_ACTIVE,FLAG_SOW_GLALIE_ACTIVE,FLAG_SOW_TYRANITAR_ACTIVE};
 static const unsigned resolved[]={FLAG_SOW_CORPHISH_RESOLVED,FLAG_SOW_GLALIE_RESOLVED,FLAG_SOW_TYRANITAR_RESOLVED};
 for(unsigned i=0;i<3;i++){
  CHECK(core->loadState(core,bootState));
  daycareFixture(1);
  // Escape acceptance must not depend on the default Lv20 Pikachu losing
  // the Gen III speed-based escape roll against Glalie/Tyranitar.
  CALL(CreateMon,player(0),SPECIES_JOLTEON,100,31,1,12345,0,1234);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),startX[i],startY[i]);
  CHECK(CALL(FlagGet,active[i]));
  CHECK(!CALL(FlagGet,resolved[i]));
  unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
  CHECK(object<16);
  core->busWrite8(core,SYM_gSelectedObjectEvent,object);
  CALL(VarSet,VAR_LAST_TALKED,localIds[i]);
  fossilStart(scripts[i]);
  waitAction();
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(CALL(GetMonAbility,enemy(0))==abilities[i]);
  weatherRun();
  waitOverworld(2);
  for(unsigned f=0;f<240&&!fieldScriptDone();f++)frames(1,0);
  CHECK(fieldScriptDone());
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);
  CHECK(CALL(FlagGet,active[i]));
  CHECK(!CALL(FlagGet,resolved[i]));
  // The same compiled object script must still create a valid retry.
  fossilStart(scripts[i]);
  waitAction();
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(CALL(GetMonAbility,enemy(0))==abilities[i]);
  weatherRun();
  waitOverworld(2);
  for(unsigned f=0;f<240&&!fieldScriptDone();f++)frames(1,0);
  CHECK(fieldScriptDone());
  CHECK(!CALL(FlagGet,resolved[i]));
  printf("PASS: SOW event%u real compiled script Run/retry retains event and ability\\n",i);
 }
 CHECK(core->loadState(core,bootState));
}
