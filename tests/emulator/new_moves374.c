// Tier2 combinations of existing Gen III effects: real ROM calls and full turns.
static const unsigned moves374[]={MOVE_ICICLE_CRASH,MOVE_DRAGON_PULSE,MOVE_DRAGON_RUSH,MOVE_SNARL,MOVE_DARK_PULSE,MOVE_NIGHT_DAZE,MOVE_NIGHT_SLASH};
static const unsigned pp374[]={10,10,10,15,15,10,15};
static const unsigned power374[]={85,85,100,55,80,85,70};
static const unsigned accuracy374[]={90,100,75,95,100,95,100};
static unsigned special374(unsigned m){return m==1||m==3||m==4||m==5;}
static void moves374Fixture(unsigned two,unsigned m){
 effectBattleFixture(two,moves374[m]);unsigned flags=core->busRead32(core,SYM_gBattleTypeFlags);damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
 CHECK(core->busRead8(core,SYM_gBattlersCount)==(two?4:2));CHECK(!!(flags&BATTLE_TYPE_DOUBLE)==two);
 core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
 core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,special374(m)?1:200);core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,special374(m)?200:1);
 core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,600);
 for(unsigned b=1;b<(two?4:2);b+=2){
  core->busWrite16(core,bmon(b)+ABI_BMON_DEFENSE,100);core->busWrite16(core,bmon(b)+ABI_BMON_SPDEFENSE,100);
  core->busWrite8(core,bmon(b)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);core->busWrite16(core,bmon(b)+ABI_BMON_SPEED,1);
 }
}
static void moves374Tests(void){
 // Shared native live/AI stat selection. Compare an existing same-category move,
 // not a copied damage formula. Minimize must not alter Gen IV Dragon Rush power.
 for(unsigned m=0;m<7;m++){
  moves374Fixture(0,m);unsigned expected=baseDamage(special374(m)?MOVE_FLAMETHROWER:MOVE_ICE_PUNCH,0,power374[m],0);
  CHECK(baseDamage(moves374[m],0,0,0)==expected);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);core->busWrite16(core,SYM_gCurrentMove,moves374[m]);CALL(AI_CalcDmg,0,1);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  if(m==2){core->busWrite32(core,SYM_gStatuses3+4,STATUS3_MINIMIZED);CHECK(baseDamage(MOVE_DRAGON_RUSH,0,0,0)==expected);}
 }
 // Exact new accuracy combinations, including Night Daze's 95 despite borrowing
 // Faint Attack's art and Dragon Rush's 75 even against a minimized target.
 moves374Fixture(0,0);unsigned script=CALL(AllocZeroed,16);CHECK(script);void *snap=malloc(core->stateSize(core));CHECK(snap&&core->saveState(core,snap));
 for(unsigned m=0;m<7;m++)for(unsigned minimized=0;minimized<(m==2?2:1);minimized++){
  unsigned hits=0;
  for(unsigned roll=0;roll<100;roll++){
   CHECK(core->loadState(core,snap));core->busWrite8(core,SYM_gBattlerAttacker,0);core->busWrite8(core,SYM_gBattlerTarget,1);core->busWrite16(core,SYM_gCurrentMove,moves374[m]);
   core->busWrite8(core,SYM_gMoveResultFlags,0);core->busWrite32(core,SYM_gHitMarker,0);
   if(minimized)core->busWrite32(core,SYM_gStatuses3+4,STATUS3_MINIMIZED);
   for(unsigned b=0;b<4;b++){core->busWrite8(core,script+1+b,(script+12)>>(8*b));}
   core->busWrite8(core,script+5,moves374[m]&255);core->busWrite8(core,script+6,moves374[m]>>8);
   core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(roll));CALL(Cmd_accuracycheck);
   unsigned hit=core->busRead32(core,SYM_gBattlescriptCurrInstr)==script+7;CHECK(hit==(roll<accuracy374[m]));hits+=hit;
  }
  CHECK(hits==accuracy374[m]);
 }
 // New data/effect combinations use their real native secondary handler. Boundary
 // checks establish chance, applied stat and Shield Dust/Substitute suppression.
 const unsigned secondaryMoves[]={0,2,3,4,5},effects[]={MOVE_EFFECT_FLINCH,MOVE_EFFECT_FLINCH,MOVE_EFFECT_SP_ATK_MINUS_1,MOVE_EFFECT_FLINCH,MOVE_EFFECT_ACC_MINUS_1},chances[]={30,20,100,20,40};
 for(unsigned i=0;i<5;i++)for(unsigned guard=0;guard<3;guard++)for(unsigned boundary=0;boundary<2;boundary++){
  CHECK(core->loadState(core,snap));unsigned m=secondaryMoves[i],roll=boundary?chances[i]:chances[i]-1;
  if(roll==100)continue;
  core->busWrite8(core,SYM_gBattlerAttacker,0);core->busWrite8(core,SYM_gBattlerTarget,1);core->busWrite16(core,SYM_gCurrentMove,moves374[m]);core->busWrite8(core,SYM_gMoveResultFlags,0);core->busWrite32(core,SYM_gHitMarker,0);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,guard==1?ABILITY_SHIELD_DUST:ABILITY_NONE);if(guard==2)core->busWrite32(core,bmon(1)+ABI_BMON_STATUS2,STATUS2_SUBSTITUTE);
  core->busWrite8(core,SYM_gBattlerByTurnOrder,0);core->busWrite8(core,SYM_gBattlerByTurnOrder+1,1);core->busWrite8(core,SYM_gCurrentTurnActionNumber,0);
  core->busWrite8(core,SYM_gBattleCommunication+MOVE_EFFECT_BYTE,effects[i]);core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(roll));core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_seteffectwithchance);
  unsigned applied=m==3?core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_SPATK)==5:m==5?core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_ACC)==5:!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_FLINCHED);
  CHECK(applied==(!guard&&!boundary));
 }
 CHECK(core->loadState(core,snap));CALL(Free,script);free(snap);
 printf("PASS: moves374–380 native live/AI category power, exact accuracy, GenIV Dragon Rush Minimize negatives and secondary boundary/guard checks\n");
 for(unsigned m=0;m<7;m++)for(unsigned two=0;two<2;two++){
  moves374Fixture(two,m);unsigned raw=baseDamage(moves374[m],0,0,0);void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned hit=0;
  for(unsigned trial=0;trial<8&&!hit;trial++){
   CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0x37400000+m*1237+two*17+trial*7919);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);waitAction();unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);hit=dealt>0;
   CHECK(monget(player(0),ABI_DATA_PP1)==pp374[m]-1);CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
   CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)==0); // no Dragon Breath paralysis, Night Shade fixed damage, etc.
   if(hit){CHECK(dealt>=raw*85/100&&dealt<=raw);if(m==3)CHECK(core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_SPATK)==5);}
   if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);if(m==3){if(core->busRead16(core,bmon(3)+ABI_BMON_HP)<1000)CHECK(core->busRead8(core,bmon(3)+ABI_BMON_STAGES+STAT_SPATK)==5);}else CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
  }
  CHECK(hit);free(state);printf("PASS: actual %s move%u single-hit category damage/onePP/target scope and approved existing-art playback; no imported artwork mechanics\n",two?"Doubles":"Singles",moves374[m]);
 }
 // Snarl's per-opponent sound handling and guaranteed SpAtk loss are independent.
 for(unsigned shield=0;shield<2;shield++){
  moves374Fixture(1,3);if(shield)core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_SOUNDPROOF);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned both=0;
  for(unsigned trial=0;trial<8&&!both;trial++){
   CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0x5A4A1000+trial*7919);
   chooseMove(0,0,1);chooseMove(2,0,1);waitAction();
   unsigned a=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP),b=1000-core->busRead16(core,bmon(3)+ABI_BMON_HP);both=(shield||a)&&b;
   if(shield){CHECK(a==0);CHECK(core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_SPATK)==6);}
   if(a){CHECK(core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_SPATK)==5);}
   if(b)CHECK(core->busRead8(core,bmon(3)+ABI_BMON_STAGES+STAT_SPATK)==5);
   CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(monget(player(0),ABI_DATA_PP1)==14);
  }
  CHECK(both);free(state);
  effectBattleFixture(0,MOVE_SPLASH);core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,shield?ABILITY_SOUNDPROOF:ABILITY_NONE);
  for(unsigned k=0;k<4;k++){core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*k,MOVE_SNARL);core->busWrite8(core,bmon(1)+ABI_BMON_PP+k,15);}
  core->busWrite8(core,SYM_gActiveBattler,1);CALL(BattleAI_SetupAIData);if(shield)CALL(RecordAbilityBattle,0,ABILITY_SOUNDPROOF);
  unsigned ai=core->busRead32(core,core->busRead32(core,SYM_gBattleResources)+ABI_BRES_AI);core->busWrite32(core,ai+ABI_AI_FLAGS,1);CALL(BattleAI_ChooseMoveOrAction);
  for(unsigned k=0;k<4;k++)CHECK(core->busRead8(core,ai+ABI_AI_SCORE+k)==(shield?90:100));
 }
 CHECK(core->loadState(core,bootState));printf("PASS: actual Snarl both-foe damage/SpAtk loss, one Soundproof recipient immunity, unaffected ally and known-ability AI penalty\n");
}
