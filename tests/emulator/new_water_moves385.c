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
  void *snap=malloc(core->stateSize(core));CHECK(snap&&core->saveState(core,snap));
  unsigned hits=0;
  for(unsigned trial=0;trial<40&&!hits;trial++){
   CHECK(core->loadState(core,snap));
   core->busWrite32(core,SYM_gRngValue,0x38538600U+trial*7919U+two*17U+m*31U);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   if(m==1 && two){
    // The first battler is automatically locked into subsequent Rollout turns,
    // but its Doubles partner still requires a real player decision each turn.
    // Exercise that controller instead of waiting forever for a menu that is
    // deliberately suppressed during the multi-turn lock.
    unsigned frame,partnerChoices=0,sawLock=0;
    for(frame=0;frame<4800;frame++){
     unsigned status=core->busRead32(core,bmon(0)+ABI_BMON_STATUS2);
     sawLock|=!!(status&STATUS2_MULTIPLETURNS);
     if(controller(0)==SYM_HandleInputChooseAction && !(status&STATUS2_MULTIPLETURNS))
      break;
     if(controller(2)==SYM_HandleInputChooseAction){
      CHECK(partnerChoices<5);
      chooseMove(2,0,1);
      partnerChoices++;
     }else{
      frames(1,(frame%12)<3?1:0);
     }
    }
    if(frame==4800)fprintf(stderr,"TIDAL_DOUBLE_TIMEOUT partnerChoices=%u sawLock=%u ctrl0=%08x ctrl2=%08x status2=%08x script=%08x exec=%08x move=%u hp=%u pp=%u\\n",
       partnerChoices,sawLock,controller(0),controller(2),core->busRead32(core,bmon(0)+ABI_BMON_STATUS2),core->busRead32(core,SYM_gBattlescriptCurrInstr),core->busRead32(core,SYM_gBattleControllerExecFlags),core->busRead16(core,SYM_gCurrentMove),core->busRead16(core,bmon(1)+ABI_BMON_HP),monget(player(0),ABI_DATA_PP1));
    CHECK(frame<4800);
    CHECK(sawLock || core->busRead16(core,bmon(1)+ABI_BMON_HP)==1000); // first-turn miss can cancel before a lock is visible
    frames(3,0);
   }else waitActionBudget(4800);
   unsigned hp=core->busRead16(core,bmon(1)+ABI_BMON_HP);
   CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
   CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
   if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
   if(hp<1000){
    // A forced native Rollout sequence may execute several damaging turns before
    // waitActionBudget returns control. Compare the cumulative damage against
    // the sum of the five approved stage powers, not a single base-power hit.
    // Vortex Spin still uses its original strict single-hit bound.
    unsigned maxDamage=expected*(m==1?31U:1U); // 1+2+4+8+16
    if(1000-hp>maxDamage)fprintf(stderr,"WATER385_DAMAGE_MISMATCH move=%u two=%u trial=%u hp=%u dealt=%u expected_single=%u max_sequence=%u dynamic=%u status2=%08x flags=%08x seed=%08x\\n",moves[m],two,trial,hp,1000-hp,expected,maxDamage,core->busRead32(core,SYM_gDynamicBasePower),core->busRead32(core,bmon(0)+ABI_BMON_STATUS2),core->busRead32(core,SYM_gBattleTypeFlags),0x38538600U+trial*7919U+two*17U+m*31U);
    CHECK(1000-hp<=maxDamage);
    hits++;
   }
   if(m==0)CHECK(!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_WRAPPED));
  }
  CHECK(hits);
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
 // Require the approved Defense Curl doubling for BOTH native Rollout and
 // Tidal Crash, with identical five-hit lock and miss cleanup.
 for(unsigned original=0;original<2;original++)for(unsigned curl=0;curl<2;curl++){
  const unsigned move=original?MOVE_ROLLOUT:MOVE_TIDAL_CRASH;
  effectBattleFixture(0,move);
  script=CALL(AllocZeroed,32);CHECK(script);
  core->busWrite8(core,SYM_gBattlerAttacker,0);
  core->busWrite8(core,SYM_gBattlerTarget,1);
  core->busWrite16(core,SYM_gCurrentMove,move);
  core->busWrite32(core,SYM_gMoveResultFlags,0);
  core->busWrite32(core,bmon(0)+ABI_BMON_STATUS2,curl?STATUS2_DEFENSE_CURL:0);
  core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);
  for(unsigned step=0;step<5;step++){
   CALL(Cmd_rolloutdamagecalculation);
   unsigned expected=(30U<<step)*(curl?2U:1U);
   CHECK(core->busRead32(core,SYM_gDynamicBasePower)==expected);
   CHECK(!!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS)==(step<4));
   if(step<4)CHECK(core->busRead16(core,SYM_gLockedMoves)==move);
  }
  CALL(Free,script);
  effectBattleFixture(0,move); // independent miss termination fixture
  script=CALL(AllocZeroed,32);CHECK(script);
  core->busWrite8(core,SYM_gBattlerAttacker,0);
  core->busWrite16(core,SYM_gCurrentMove,move);
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
 printf("PASS: native Vortex Spin exact 30/60%% confusion, zero wrapping; Tidal Crash and Rollout 30/60/120/240/480 with Defense Curl doubling; five-turn lock/miss cleanup\n");
}
