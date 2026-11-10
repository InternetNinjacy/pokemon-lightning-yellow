// BTL-005 #385-386: exercise the real compiled ROM in Singles and Doubles.
// This is an acceptance smoke suite, not a substitute for full Rollout
// five-turn, Defense Curl, and save/PP persistence regression coverage.
static void waterMoves385Tests(void){
 const unsigned moves[]={MOVE_VORTEX_SPIN,MOVE_TIDAL_CRASH};
 const unsigned pp[]={25,20};
 for(unsigned m=0;m<2;m++)for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,moves[m]);
  unsigned flags=core->busRead32(core,SYM_gBattleTypeFlags);damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,180);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,1);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,999);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
  unsigned expected=baseDamage(moves[m],0,0,0);
  CHECK(expected>0&&expected<1000);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite16(core,SYM_gCurrentMove,moves[m]);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  // Rollout's native status2 lock can play all five attacks before the
  // action chooser returns. Never compare their cumulative HP loss to the
  // first hit's 30-power reference. Record each real ROM hit separately.
  unsigned perHit[5]={0};
  if(m==1){
   CHECK(!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_DEFENSE_CURL));
   CHECK(!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS));
   for(unsigned step=0;step<5;step++){
    perHit[step]=baseDamage(MOVE_TIDAL_CRASH,0,30U<<step,0);
    CHECK(perHit[step]>0);
   }
  }
  void *snap=malloc(core->stateSize(core));CHECK(snap&&core->saveState(core,snap));
  unsigned hits=0,misses=0;
  for(unsigned trial=0;trial<(m?64:40)&&(!hits||(m==1&&!misses));trial++){
   CHECK(core->loadState(core,snap));
   core->busWrite32(core,SYM_gRngValue,0x38538600U+trial*7919U+two*17U+m*31U);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   unsigned count=0,hp=1000;
   if(m==0){
    waitActionBudget(4800);
    hp=core->busRead16(core,bmon(1)+ABI_BMON_HP);
    count=(hp<1000);
   }else{
    unsigned frame;
    for(frame=0;frame<4800;frame++){
     if(frame>0&&controller(0)==SYM_HandleInputChooseAction)break;
     frames(1,(frame%12)<3?1:0);
     unsigned current=core->busRead16(core,bmon(1)+ABI_BMON_HP);
     CHECK(current<=hp);
     if(current<hp){
      CHECK(count<5);
      unsigned dealt=hp-current;
      if(dealt<perHit[count]*85/100||dealt>perHit[count])
       fprintf(stderr,"TIDAL_HIT_MISMATCH two=%u trial=%u step=%u dealt=%u range=%u..%u status2=%08x\n",
               two,trial,count,dealt,perHit[count]*85/100,perHit[count],
               core->busRead32(core,bmon(0)+ABI_BMON_STATUS2));
      CHECK(dealt>=perHit[count]*85/100&&dealt<=perHit[count]);
      count++;
      hp=current;
     }
    }
    CHECK(frame<4800);frames(3,0);
   }
   CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
   CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
   if(two){
    CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);
    CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);
   }
   if(!m){
    if(hp<1000){CHECK(hp>=1000-expected);hits++;}
    CHECK(!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_WRAPPED));
   }else{
    if(count)hits++;
    else misses++;
    if(!two&&count==5)
     CHECK(!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS));
   }
  }
  CHECK(hits);
  if(m==1)CHECK(misses); // 90% accuracy must produce genuine native misses.
  free(snap);
  printf("PASS: native water move %u %s physical AI damage, single-target actual animation, PP, no trap\\n",moves[m],two?"Doubles":"Singles");
 }
 CHECK(core->loadState(core,bootState));
}

// Direct native command acceptance supplements real Singles/Doubles turns above:
// exact custom confusion chance (including Serene Grace) and all five native
// Rollout powers, Defense Curl doubling, lock expiration and miss cleanup.
static void waterMoves385Mechanics(void){
 effectBattleFixture(0,MOVE_VORTEX_SPIN);
 unsigned script=CALL(AllocZeroed,32);CHECK(script);
 void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
 const unsigned rolls[]={29,30,59,60};
 for(unsigned serene=0;serene<2;serene++)for(unsigned r=0;r<4;r++){
  CHECK(core->loadState(core,state));
  core->busWrite8(core,SYM_gBattlerAttacker,0);
  core->busWrite8(core,SYM_gBattlerTarget,1);
  core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,serene?ABILITY_SERENE_GRACE:ABILITY_NONE);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_NONE);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_VORTEX_SPIN);
  core->busWrite8(core,SYM_gMoveResultFlags,0);
  core->busWrite32(core,SYM_gHitMarker,0);
  core->busWrite8(core,SYM_gBattleCommunication+MOVE_EFFECT_BYTE,MOVE_EFFECT_CONFUSION);
  core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(rolls[r]));
  core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);
  CALL(Cmd_seteffectwithchance);
  unsigned status2=core->busRead32(core,bmon(1)+ABI_BMON_STATUS2);
  CHECK(!!(status2&STATUS2_CONFUSION)==(rolls[r]<(serene?60:30)));
  CHECK(!(status2&STATUS2_WRAPPED));
 }
 CHECK(core->loadState(core,state));CALL(Free,script);free(state);
 for(unsigned curl=0;curl<2;curl++){
  effectBattleFixture(0,MOVE_TIDAL_CRASH);
  script=CALL(AllocZeroed,32);CHECK(script);
  core->busWrite8(core,SYM_gBattlerAttacker,0);
  core->busWrite8(core,SYM_gBattlerTarget,1);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_TIDAL_CRASH);
  core->busWrite32(core,SYM_gMoveResultFlags,0);
  core->busWrite32(core,bmon(0)+ABI_BMON_STATUS2,curl?STATUS2_DEFENSE_CURL:0);
  core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);
  for(unsigned step=0;step<5;step++){
   CALL(Cmd_rolloutdamagecalculation);
   CHECK(core->busRead32(core,SYM_gDynamicBasePower)==(30U<<step)*(curl?2:1));
   CHECK(!!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS)==(step<4));
  }
  CALL(Free,script);
  effectBattleFixture(0,MOVE_TIDAL_CRASH); // independent miss termination fixture
  script=CALL(AllocZeroed,32);CHECK(script);
  core->busWrite8(core,SYM_gBattlerAttacker,0);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_TIDAL_CRASH);
  core->busWrite32(core,SYM_gMoveResultFlags,0);
  core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);
  CALL(Cmd_rolloutdamagecalculation);
  CHECK(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS);
  core->busWrite32(core,SYM_gMoveResultFlags,MOVE_RESULT_MISSED);
  CALL(Cmd_rolloutdamagecalculation);
  CHECK(!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS));
  CALL(Free,script);
 }
 CHECK(core->loadState(core,bootState));
 printf("PASS: native Vortex Spin exact 30/60%% confusion, zero wrapping; Tidal Crash 30/60/120/240/480 power, Defense Curl doubling, five-turn lock/miss cleanup\n");
}
