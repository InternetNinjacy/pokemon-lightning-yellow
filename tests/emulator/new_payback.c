// Tier3 target-action state: no inference from damage, speed or the last actor.
static unsigned paybackActed(void){return core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_ACTED;}
static void paybackFixture(unsigned two){
 effectBattleFixture(two,MOVE_PAYBACK);unsigned flags=core->busRead32(core,SYM_gBattleTypeFlags);damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
 CHECK(core->busRead8(core,SYM_gBattlersCount)==(two?4:2));CHECK(!!(flags&BATTLE_TYPE_DOUBLE)==two);
 core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
}
static void paybackTests(void){
 if(!getenv("LIGHTNING_PAYBACK_SWITCH_ONLY")){
 paybackFixture(1);CHECK(ABI_BATTLESTRUCT_SIZE==512);CHECK(ABI_BATTLESTRUCT_ACTED==0x1EC);
 unsigned base=baseDamage(MOVE_ICE_PUNCH,0,50,0),doublePower=baseDamage(MOVE_ICE_PUNCH,0,100,0);CHECK(doublePower>base);
 for(unsigned mask=0;mask<16;mask++){
  core->busWrite8(core,paybackActed(),mask);CHECK(baseDamage(MOVE_PAYBACK,0,0,0)==((mask&2)?doublePower:base));
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);core->busWrite16(core,SYM_gCurrentMove,MOVE_PAYBACK);CALL(AI_CalcDmg,0,1);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==((mask&2)?doublePower:base));
  for(unsigned b=0;b<4;b++)CHECK(CALL(BattlerHasActedThisTurn,b)==!!(mask&(1U<<b)));
 }
 CHECK(!CALL(BattlerHasActedThisTurn,4));CHECK(!CALL(BattlerHasActedThisTurn,255));
 core->busWrite8(core,paybackActed(),2);core->busWrite8(core,SYM_gActiveBattler,1);CALL(SwitchInClearSetData);CHECK(core->busRead8(core,paybackActed())==2);
 CALL(TurnValuesCleanUp,1);CHECK(core->busRead8(core,paybackActed())==2);CALL(TurnValuesCleanUp,0);CHECK(core->busRead8(core,paybackActed())==0);
 // Actual action dispatcher marks the completed target slot in arbitrary order.
 const unsigned order[]={3,1,2,0};
 for(unsigned i=0,mask=0;i<4;i++){
  core->busWrite8(core,SYM_gBattlerByTurnOrder+i,order[i]);core->busWrite8(core,SYM_gCurrentTurnActionNumber,i);core->busWrite8(core,SYM_gCurrentActionFuncId,ABI_ACTION_NOTHING_FAINTED);CALL(RunTurnActionsFunctions);
  mask|=1U<<order[i];CHECK(core->busRead8(core,paybackActed())==mask);
 }
 printf("PASS: native Payback16 target-action masks/shared live-AI power, arbitrary actual completion order, switch identity retained, turn reset and unchanged512-byte ABI\n");
 for(unsigned scenario=0;scenario<6;scenario++){
  unsigned two=scenario>=2&&scenario<=4,boost=scenario==1||scenario==3||scenario==5;
  paybackFixture(two);core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,200);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,boost?300:1);
  if(two)core->busWrite16(core,bmon(3)+ABI_BMON_SPEED,scenario==3?1:300);
  if(scenario==5)core->busWrite32(core,bmon(1)+ABI_BMON_STATUS1,3); // target consumes its action asleep
  unsigned expected=baseDamage(MOVE_ICE_PUNCH,0,boost?100:50,0),observed=0,f;
  chooseMove(0,0,1);if(two)chooseMove(2,0,1);
  for(f=0;f<4800;f++){
   if(controller(0)==SYM_HandleInputChooseAction)break;
   frames(1,(f%12)<3?1:0);
   if(!observed&&core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000){CHECK(core->busRead16(core,SYM_gBattleMovePower)==(boost?100:50));CHECK(!!(core->busRead8(core,paybackActed())&2)==boost);if(scenario==2||scenario==4)CHECK(core->busRead8(core,paybackActed())&8);observed=1;}
  }
  CHECK(f<4800&&observed);frames(3,0);unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);CHECK(dealt>=expected*85/100&&dealt<=expected);CHECK(monget(player(0),ABI_DATA_PP1)==9);
  CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
  CHECK(core->busRead8(core,paybackActed())==0);
  printf("PASS: actual Payback scenario%u %s target-specific power%u, no damage prerequisite, onePP, existing Revenge art and no stale next-turn mask\n",scenario,two?"Doubles":"Singles",boost?100:50);
 }
 }
 // Drive real switch scripts/controller callbacks, including GenIV's replacement
 // target bonus. Only the opponent's chosen action is controlled by this fixture.
 for(unsigned two=0;two<2;two++){
  paybackFixture(two);unsigned reserve=two?2:1;
  CALL(CreateMon,enemy(reserve),SPECIES_SNORLAX,50,31,1,54321,0,1234);
  monput(enemy(reserve),ABI_DATA_HP,1000);monput(enemy(reserve),ABI_DATA_MAX_HP,1000);
  for(unsigned k=0;k<4;k++)CALL(SetMonMoveSlot,enemy(reserve),MOVE_SPLASH,k);
  core->busWrite8(core,SYM_gEnemyPartyCount,reserve+1);
  core->busWrite32(core,SYM_gBattleTypeFlags,core->busRead32(core,SYM_gBattleTypeFlags)|BATTLE_TYPE_TRAINER);
  // Stop before the turn sorter rather than modifying its resulting turn order.
  if(two)chooseMove(0,0,1);
  unsigned last=two?2:0;waitController(last,SYM_HandleInputChooseAction);press(1);waitController(last,SYM_HandleInputChooseMove);frames(1,1);
  unsigned f;for(f=0;f<240;f++){
   if((core->busRead32(core,SYM_gBattleMainFunc)&~1U)==SYM_SetActionsAndBattlersTurnOrder)break;
   frames(1,0);
  }
  CHECK(f<240);
  core->busWrite8(core,SYM_gChosenActionByBattler+1,ABI_ACTION_SWITCH);
  core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_SWITCH_IN+1,reserve);
  unsigned switched=0,hit=0;
  for(f=0;f<4800;f++){
   frames(1,(f%12)<3?1:0);
   if(core->busRead16(core,SYM_gBattlerPartyIndexes+2)==reserve&&(core->busRead8(core,paybackActed())&2)){
    switched=1;core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
    if(!hit&&core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000){CHECK(core->busRead16(core,SYM_gBattleMovePower)==100);hit=1;}
   }
   if(hit&&controller(0)==SYM_HandleInputChooseAction)break;
  }
  CHECK(f<4800&&switched&&hit);CHECK(monget(player(0),ABI_DATA_PP1)==9);
  CHECK(monget(enemy(0),ABI_DATA_HP)==1000);CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
  if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
  CHECK(core->busRead8(core,paybackActed())==0);
  printf("PASS: actual Payback %s opponent switch/replacement, GenIV100power, withdrawn mon preserved, onePP and return to action menu\n",two?"Doubles":"Singles");
 }
 CHECK(core->loadState(core,bootState));
}
