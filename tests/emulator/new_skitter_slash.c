// BTL-005 #361 native execution, independent of the earlier Ghost/Fire batches.
static void skitterSlashTests(void){
 for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,MOVE_SKITTER_SLASH);
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,200);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,1);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,999);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_NORMAL);
  damageStats();
  unsigned expected=baseDamage(MOVE_SKITTER_SLASH,0,0,0);
  CHECK(expected>=30&&expected<=40);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_SKITTER_SLASH);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  chooseMove(0,0,1);
  if(two)chooseMove(2,0,1);
  waitAction(); // Actual Fury Cutter visual must return to battle control.
  unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
  CHECK(dealt>0&&dealt<=expected);
  CHECK(monget(player(0),ABI_DATA_PP1)==14);
  CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)==0);
  CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
  if(two){
   CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);
   CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);
  }
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==0);
 }
 CHECK(core->loadState(core,bootState));
 printf("PASS: actual Singles/Doubles Skitter Slash #361 physical damage, AI, onePP, no status, selected-target only, native Fury Cutter visual termination\n");
}
