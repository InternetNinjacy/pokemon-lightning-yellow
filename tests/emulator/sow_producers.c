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
  CHECK(CALL(VarGet,VAR_RESULT)==TRUE);
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(monget(enemy(0),ABI_DATA_LEVEL)==levels[i]);
  CHECK(monget(enemy(0),ABI_DATA_HELD_ITEM)==0);
  CHECK(CALL(GetMonAbility,enemy(0))==abilities[i]);
  for(unsigned j=0;j<4;j++)CHECK(monget(enemy(0),ABI_DATA_MOVE1+j)==moves[i][j]);
 }
 CALL(CreateScriptedWildMon,SPECIES_PIDGEY,10,0);
 CALL(VarSet,VAR_0x8004,0);
 CALL(SowPrepareScriptedEncounter);
 CHECK(CALL(VarGet,VAR_RESULT)==FALSE);
 printf("PASS: actual scripted Corphish/Glalie/Tyranitar created at approved levels, all 12 exact moves and PP, no items, individual Drizzle/Snow Warning and native Sand Stream; invalid species fails closed\n");
 CHECK(core->loadState(core,bootState));
}
