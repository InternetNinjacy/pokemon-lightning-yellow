// Approved Ghost additions execute real compiled effects, never borrowed mechanics.
static void ghostMoveFixture(unsigned two,unsigned move){
 effectBattleFixture(two,move);
 core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
 core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,200);core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,50);
 core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,300);
 core->busWrite16(core,bmon(0)+ABI_BMON_HP,500);monput(player(0),ABI_DATA_HP,500);
 core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,200);
 core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,1);
 core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_FIRE);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_FIRE);
}
static void newGhostMoveTests(void){
 const unsigned moves[]={MOVE_SOUL_DRAIN,MOVE_WEARY_TOUCH,MOVE_DREAM_HAUNT},expected[]={46,68,8};
 for(unsigned m=0;m<3;m++){
  ghostMoveFixture(0,moves[m]);damageStats();
  CHECK(baseDamage(moves[m],0,0,0)==expected[m]);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
  core->busWrite16(core,SYM_gCurrentMove,moves[m]);CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected[m]);
 }
 // Base damage/AI double POWER, not final damage, with no accumulation or wakeup.
 ghostMoveFixture(0,MOVE_DREAM_HAUNT);damageStats();
 core->busWrite32(core,bmon(1)+ABI_BMON_STATUS1,5);
 CHECK(baseDamage(MOVE_DREAM_HAUNT,0,0,0)==14);
 core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
 core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
 core->busWrite16(core,SYM_gCurrentMove,MOVE_DREAM_HAUNT);CALL(AI_CalcDmg,0,1);
 CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==14);
 CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)==5);
 core->busWrite32(core,bmon(1)+ABI_BMON_STATUS1,0);CHECK(baseDamage(MOVE_DREAM_HAUNT,0,0,0)==8);
 for(unsigned two=0;two<2;two++){
  for(unsigned ooze=0;ooze<2;ooze++){
   ghostMoveFixture(two,MOVE_SOUL_DRAIN);if(ooze)core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_LIQUID_OOZE);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);waitAction();
   unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP),heal=dealt/2?dealt/2:1;
   CHECK(dealt>0);CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==(ooze?500-heal:500+heal));
   CHECK(monget(player(0),ABI_DATA_PP1)==14);
   if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
  }
  ghostMoveFixture(two,MOVE_WEARY_TOUCH);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned sleep=0,plain=0;
  for(unsigned trial=0;trial<40&&(!sleep||!plain);trial++){
   CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0x6A057000+trial*7919);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);waitAction();
   unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);CHECK(dealt>=57&&dealt<=68);
   unsigned status=core->busRead32(core,bmon(1)+ABI_BMON_STATUS1);CHECK(!(status&~STATUS1_SLEEP));
   sleep+=!!(status&STATUS1_SLEEP);plain+=!status;
   CHECK(monget(player(0),ABI_DATA_PP1)==14);CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==500);
   if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
  }
  CHECK(sleep&&plain);free(state);
  for(unsigned asleep=0;asleep<2;asleep++){
   ghostMoveFixture(two,MOVE_DREAM_HAUNT);if(asleep)core->busWrite32(core,bmon(1)+ABI_BMON_STATUS1,5);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);waitAction();
   unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);CHECK(dealt>=(asleep?11:6)&&dealt<=(asleep?14:8));
   CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==500);CHECK(monget(player(0),ABI_DATA_PP1)==14);
   if(asleep)CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_SLEEP);
   if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
  }
  printf("PASS: actual %s Soul Drain heal/Ooze, Weary Touch sleep/plain and Dream Haunt awake/sleep power/no drain/no wake, native composites and onePP\n",two?"Double":"Single");
 }
 for(unsigned m=0;m<3;m++){
  ghostMoveFixture(0,moves[m]);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_NORMAL);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_NORMAL);
  chooseMove(0,0,1);waitAction();CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==1000);
  CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==500);CHECK(!core->busRead32(core,bmon(1)+ABI_BMON_STATUS1));CHECK(monget(player(0),ABI_DATA_PP1)==14);
 }
 CHECK(core->loadState(core,bootState));
 printf("PASS: original Normal immunity preserved for all3 new Ghost moves; physical/special stat selection and conditional AI damage\n");
}
