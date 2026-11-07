// Native ROM execution for Bug Bite #363 and Chitin Crush #364.
static void bugStatdropTests(void){
 const unsigned moves[]={MOVE_BUG_BITE,MOVE_CHITIN_CRUSH};
 const unsigned stats[]={STAT_SPATK,STAT_DEF};
 for(unsigned m=0;m<2;m++)for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,moves[m]);
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,200);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,1);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,999);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_NORMAL);
  damageStats();
  unsigned expected=baseDamage(moves[m],0,0,0);
  CHECK(expected>25&&expected<100);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
  core->busWrite16(core,SYM_gCurrentMove,moves[m]);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  void *snapshot=malloc(core->stateSize(core));CHECK(snapshot&&core->saveState(core,snapshot));
  unsigned lowered=0,notLowered=0;
  for(unsigned trial=0;trial<50&&(!lowered||!notLowered);trial++){
   CHECK(core->loadState(core,snapshot));
   core->busWrite32(core,SYM_gRngValue,0xBA770000+m*317+trial*7919);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   waitAction();
   unsigned damage=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
   CHECK(damage<=expected);
   CHECK(monget(player(0),ABI_DATA_PP1)==14);
   CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)==0);
   unsigned stage=core->busRead8(core,bmon(1)+ABI_BMON_STAGES+stats[m]);
   CHECK(stage==5||stage==6);
   CHECK(core->busRead8(core,bmon(1)+ABI_BMON_STAGES+(m?STAT_SPATK:STAT_DEF))==6);
   if(stage==5){CHECK(damage>0);lowered++;}else if(damage>0)notLowered++;
   if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
  }
  CHECK(lowered&&notLowered);free(snapshot);
 }
 CHECK(core->loadState(core,bootState));
 printf("PASS: native Singles/Doubles Bug Bite and Chitin Crush: physical damage/AI, independent30%% effects, onePP, single target, approved visuals\n");
}
