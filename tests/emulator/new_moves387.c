// BTL-005 #387 real-ROM Discharge: correct Electric special stat, true
// all-others Singles/Doubles targeting, one PP and 30%/Serene Grace60%.
static void discharge387Tests(void){
 for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,MOVE_DISCHARGE);
  unsigned flags=core->busRead32(core,SYM_gBattleTypeFlags);
  damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,1);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,220);
  for(unsigned b=1;b<(two?4:2);b++){
   core->busWrite16(core,bmon(b)+ABI_BMON_DEFENSE,999);
   core->busWrite16(core,bmon(b)+ABI_BMON_SPDEFENSE,100);
  }
  unsigned expected=baseDamage(MOVE_DISCHARGE,0,0,0);
  CHECK(expected>0&&expected<1000);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_DISCHARGE);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  chooseMove(0,0,1);if(two)chooseMove(2,0,1);
  waitActionBudget(4800);
  CHECK(monget(player(0),ABI_DATA_PP1)==14);
  CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000);
  if(two){
   CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)<1000);
   CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)<1000);
  }
 }
 effectBattleFixture(0,MOVE_DISCHARGE);
 unsigned script=CALL(AllocZeroed,32);CHECK(script);
 void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
 const unsigned rolls[]={29,30,59,60};
 for(unsigned serene=0;serene<2;serene++)for(unsigned r=0;r<4;r++){
  CHECK(core->loadState(core,state));
  core->busWrite8(core,SYM_gBattlerAttacker,0);
  core->busWrite8(core,SYM_gBattlerTarget,1);
  core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,serene?ABILITY_SERENE_GRACE:ABILITY_NONE);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_NONE);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_DISCHARGE);
  core->busWrite8(core,SYM_gMoveResultFlags,0);
  core->busWrite32(core,SYM_gHitMarker,0);
  core->busWrite8(core,SYM_gBattleCommunication+MOVE_EFFECT_BYTE,MOVE_EFFECT_PARALYSIS);
  core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(rolls[r]));
  core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);
  CALL(Cmd_seteffectwithchance);
  CHECK(!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_PARALYSIS)==(rolls[r]<(serene?60:30)));
 }
 CHECK(core->loadState(core,state));CALL(Free,script);free(state);
 CHECK(core->loadState(core,bootState));
 printf("PASS: native Discharge #387 Singles/Doubles all-other-battler HP targeting, Electric special AI, 1 PP, 30/60%% paralysis\n");
}
