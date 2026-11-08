// Native Earth Power #371: real compiled GBA in Singles and Doubles.
static void earthPowerTests(void){
 for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,MOVE_EARTH_POWER);
  damageStats();
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,1);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,200);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,999);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,100);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
  unsigned expected=baseDamage(MOVE_EARTH_POWER,0,0,0);
  CHECK(expected>60&&expected<110);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_EARTH_POWER);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
  unsigned dropped=0,plain=0;
  for(unsigned trial=0;trial<90&&(!dropped||!plain);trial++){
   CHECK(core->loadState(core,state));
   core->busWrite32(core,SYM_gRngValue,0xEAA70000+trial*7919+two*31);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   waitActionBudget(4800);
   unsigned stage=core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_SPDEF);
   unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
   CHECK(stage==5||stage==6);
   CHECK(dealt>0&&dealt<=expected);
   CHECK(monget(player(0),ABI_DATA_PP1)==9);
   CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
   if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
   dropped+=stage==5;plain+=stage==6;
  }
  CHECK(dropped&&plain);free(state);
  printf("PASS: native Earth Power #371 %s Special/AI damage, SpDef -1/normal, onePP, selected target, Earthquake art termination\n",two?"Doubles":"Singles");
 }
 effectBattleFixture(0,MOVE_EARTH_POWER);
 core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_FLYING);
 core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_FLYING);
 chooseMove(0,0,1);waitActionBudget(4800);
 CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==1000);
 CHECK(core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_SPDEF)==6);
 CHECK(monget(player(0),ABI_DATA_PP1)==9);
 CHECK(core->loadState(core,bootState));
 printf("PASS: Earth Power Ground/Flying immunity, no imported status, no new artwork or distribution\n");
}
