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
  unsigned hits=0,misses=0,fiveHits=0;
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
    unsigned frame, lastHitFrame=0;
    // The ROM's five actual battle turns exceed a single-action frame budget.
    // Keep the original strict 4800-frame watchdog for EACH hit, with a
    // separate finite five-turn bound. Never infer success from elapsed time.
    const unsigned perHitFrameBudget=4800;
    const unsigned sequenceFrameBudget=5*perHitFrameBudget;
    unsigned lastScript=0,lastExec=0,lastController=0,lastCount=0;
    unsigned transitions=0;
    for(frame=0;frame<sequenceFrameBudget;frame++){
     if(frame-lastHitFrame>=perHitFrameBudget){
      fprintf(stderr,"TIDAL_HIT_WATCHDOG two=%u trial=%u count=%u waited=%u script=%08x exec=%08x ctrl0=%08x ctrl2=%08x status0=%08x\\n",
       two,trial,count,frame-lastHitFrame,core->busRead32(core,SYM_gBattlescriptCurrInstr),
       core->busRead32(core,SYM_gBattleControllerExecFlags),controller(0),controller(2),
       core->busRead32(core,bmon(0)+ABI_BMON_STATUS2));
      CHECK(frame-lastHitFrame<perHitFrameBudget);
     }
     if(two && count>=4){
      unsigned scriptNow=core->busRead32(core,SYM_gBattlescriptCurrInstr);
      unsigned execNow=core->busRead32(core,SYM_gBattleControllerExecFlags);
      unsigned ctrlNow=controller(2);
      if(scriptNow!=lastScript||execNow!=lastExec||ctrlNow!=lastController||count!=lastCount){
       if(transitions<48)fprintf(stderr,"TIDAL_STEP5_TRACE frame=%u count=%u script=%08x exec=%08x ctrl0=%08x ctrl2=%08x action2=%u move2=%u status0=%08x\\n",frame,count,scriptNow,execNow,controller(0),ctrlNow,core->busRead8(core,SYM_gActionSelectionCursor+2),core->busRead8(core,SYM_gMoveSelectionCursor+2),core->busRead32(core,bmon(0)+ABI_BMON_STATUS2));
       transitions++;
       lastScript=scriptNow;lastExec=execNow;lastController=ctrlNow;lastCount=count;
      }
     }
     if(frame>0&&controller(0)==SYM_HandleInputChooseAction)break;
     // The locked Rollout user does not reselect, but its Doubles partner
     // must still submit a normal action on each additional turn. Without
     // that input the test can stall at the partner's action menu indefinitely.
     if(two&&controller(2)==SYM_HandleInputChooseAction){
      chooseMove(2,0,1);
     }else if(two&&controller(2)==SYM_HandleInputChooseMove){
      // A controller can advance to the partner's move menu after the
      // prior action helper returned; finish that actual native menu.
      CHECK(core->busRead8(core,SYM_gMoveSelectionCursor+2)==0);
      press(1);
     }else if(two&&controller(2)==SYM_HandleInputChooseTarget){
      // A delayed selected-target menu is likewise real controller input.
      // Never accept an unintended ally or second foe as the target.
      for(unsigned turn=0;turn<6&&core->busRead8(core,SYM_gMultiUsePlayerCursor)!=1;turn++)press(16);
      CHECK(core->busRead8(core,SYM_gMultiUsePlayerCursor)==1);
      press(1);
     }
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
      lastHitFrame=frame;
      hp=current;
     }
    }
    if(frame==sequenceFrameBudget)fprintf(stderr,"TIDAL_DOUBLES_TIMEOUT two=%u trial=%u count=%u hp=%u lock=%08x ctrl=%08x/%08x/%08x/%08x script=%08x\\n",two,trial,count,hp,core->busRead32(core,bmon(0)+ABI_BMON_STATUS2),controller(0),controller(1),controller(2),controller(3),core->busRead32(core,SYM_gBattlescriptCurrInstr));
    CHECK(frame<sequenceFrameBudget);frames(3,0);
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
    if(count==5){
     fiveHits++;
     CHECK(!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS));
    }
   }
  }
  CHECK(hits);
  if(m==1){CHECK(misses);CHECK(fiveHits);} // genuine miss plus all five real Singles hits.
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
 // Only original Rollout inherits Defense Curl's damage bonus.
 // Tidal Crash shares five-turn progression and miss cleanup, never Curl doubling.
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
   unsigned expected=(30U<<step)*(original&&curl?2U:1U);
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
 printf("PASS: native Vortex Spin exact 30/60%% confusion, zero wrapping; Tidal Crash never receives Curl doubling; original Rollout doubles after Defense Curl; five-turn lock/miss cleanup\n");
}

static void waterMoves385SavePersistence(void){
 // Exercise real flash serialization, not a transient mGBA CPU snapshot.
 CHECK(core->loadState(core,bootState));
 GBASavedataForceType(&((struct GBA*)core->board)->memory.savedata,SAVEDATA_FLASH1M);
 void *oldFlash=NULL;
 size_t oldSize=core->savedataClone(core,&oldFlash);
 CHECK(oldSize==0x20000&&oldFlash);
 unsigned char *blank=malloc(0x20000);CHECK(blank);
 memset(blank,255,0x20000);
 CHECK(core->savedataRestore(core,blank,0x20000,true));
 free(blank);
 initSave();create(SPECIES_SQUIRTLE,50);
 core->busWrite8(core,SYM_gPlayerPartyCount,1);
 CALL(SetMonMoveSlot,player(0),MOVE_VORTEX_SPIN,0);
 CALL(SetMonMoveSlot,player(0),MOVE_TIDAL_CRASH,1);
 monput(player(0),ABI_DATA_PP1,17);
 monput(player(0),ABI_DATA_PP2,9);
 monput(player(0),ABI_DATA_PP_BONUSES,5);
 unsigned char saved[ABI_BOX_SIZE];
 readBytes(player(0),saved,sizeof(saved));
 CHECK(CALL(TrySavingData,0)==1);
 for(unsigned k=0;k<ABI_SAVE1_SIZE;k+=4)core->busWrite32(core,SYM_gSaveBlock1+k,0);
 for(unsigned k=0;k<ABI_SAVE2_SIZE;k+=4)core->busWrite32(core,SYM_gSaveBlock2+k,0);
 CALL(SetMonMoveSlot,player(0),MOVE_SPLASH,0);
 CALL(SetMonMoveSlot,player(0),MOVE_SPLASH,1);
 CHECK(CALL(LoadGameSave,0)==1);
 sameBytes(player(0),saved,sizeof(saved));
 CHECK(monget(player(0),ABI_DATA_MOVE1)==MOVE_VORTEX_SPIN);
 CHECK(monget(player(0),ABI_DATA_MOVE2)==MOVE_TIDAL_CRASH);
 CHECK(monget(player(0),ABI_DATA_PP1)==17);
 CHECK(monget(player(0),ABI_DATA_PP2)==9);
 CHECK(monget(player(0),ABI_DATA_PP_BONUSES)==5);
 CHECK(!monget(player(0),ABI_DATA_SANITY_IS_BAD_EGG));
 CHECK(core->savedataRestore(core,oldFlash,oldSize,true));
 free(oldFlash);
 CHECK(core->loadState(core,bootState));
 printf("PASS: native water moves385-386 IDs, spent PP, PP bonuses and encrypted record survive actual flash save/load\\n");
}
