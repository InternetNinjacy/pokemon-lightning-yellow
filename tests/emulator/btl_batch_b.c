// BTL-005 Batch B real-ROM Singles/Doubles acceptance (staging, not passed).
static void batchBNativeTests(void){
 const unsigned moves[]={MOVE_DISCHARGE,MOVE_THUNDER_FANG,MOVE_WILD_CHARGE,
  MOVE_WOOD_HAMMER,MOVE_DRAIN_PUNCH,MOVE_BRAVE_BIRD,MOVE_ZEN_HEADBUTT,
  MOVE_CROSS_POISON,MOVE_POISON_JAB,MOVE_GUNK_SHOT};
 const unsigned pp[]={15,15,15,15,10,15,15,20,20,5};
 for(unsigned m=0;m<10;m++)for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,moves[m]);
  unsigned flags=core->busRead32(core,SYM_gBattleTypeFlags);
  damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
  CHECK(!!(flags&BATTLE_TYPE_DOUBLE)==two);
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,175);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,175);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,100);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
  if(m==4){
   // A damaged attacker must receive healing from real Drain Punch damage.
   core->busWrite16(core,bmon(0)+ABI_BMON_HP,500);
   monput(player(0),ABI_DATA_HP,500);
  }
  unsigned raw=baseDamage(moves[m],0,0,0);
  CHECK(raw>0&&raw<900);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite16(core,SYM_gCurrentMove,moves[m]);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==raw);
  void *snap=malloc(core->stateSize(core));CHECK(snap&&core->saveState(core,snap));
  unsigned hit=0;
  for(unsigned trial=0;trial<16&&!hit;trial++){
   CHECK(core->loadState(core,snap));
   core->busWrite32(core,SYM_gRngValue,0xB7100000U+m*17001U+two*107U+trial*7919U);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   waitActionBudget(4800);
   unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
   CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
   if(dealt){
    hit++;
    unsigned estimated=m==0&&two?raw*3/4:raw;
    CHECK(dealt+2>=estimated*85/100&&dealt<=estimated+2);
    unsigned ours=core->busRead16(core,bmon(0)+ABI_BMON_HP);
    if(m==2)CHECK(ours<1000&&1000-ours>=dealt/4&&1000-ours<=dealt/4+2);
    else if(m==3||m==5)CHECK(ours<1000&&1000-ours>=dealt/3&&1000-ours<=dealt/3+2);
    else if(m==4)CHECK(ours>500&&ours<=500+dealt/2+2);
    else CHECK(ours==1000);
   }
   if(two){
    if(m==0){
     CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)<1000);
     CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)<1000);
    }else{
     CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);
     CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);
    }
   }
  }
  CHECK(hit);
  free(snap);
  printf("PASS: Batch B move%u %s actual ROM animation, target scope, attack damage, HP interaction, AI and one PP\\n",moves[m],two?"Doubles":"Singles");
 }
 CHECK(core->loadState(core,bootState));
}

static void batchBExactMechanics(void){
 const unsigned moves[]={MOVE_DISCHARGE,MOVE_THUNDER_FANG,MOVE_WILD_CHARGE,
  MOVE_WOOD_HAMMER,MOVE_DRAIN_PUNCH,MOVE_BRAVE_BIRD,MOVE_ZEN_HEADBUTT,
  MOVE_CROSS_POISON,MOVE_POISON_JAB,MOVE_GUNK_SHOT};
 const unsigned accuracies[]={100,95,100,100,100,100,90,100,100,80};
 effectBattleFixture(0,MOVE_DISCHARGE);
 unsigned script=CALL(AllocZeroed,16);CHECK(script);
 void *snap=malloc(core->stateSize(core));CHECK(snap&&core->saveState(core,snap));
 for(unsigned m=0;m<10;m++){
  unsigned hits=0;
  for(unsigned roll=0;roll<100;roll++){
   CHECK(core->loadState(core,snap));
   core->busWrite8(core,SYM_gBattlerAttacker,0);
   core->busWrite8(core,SYM_gBattlerTarget,1);
   core->busWrite16(core,SYM_gCurrentMove,moves[m]);
   core->busWrite8(core,SYM_gMoveResultFlags,0);
   core->busWrite32(core,SYM_gHitMarker,0);
   for(unsigned i=0;i<4;i++)core->busWrite8(core,script+1+i,(script+12)>>(8*i));
   core->busWrite8(core,script+5,moves[m]&255);
   core->busWrite8(core,script+6,moves[m]>>8);
   core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);
   core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(roll));
   CALL(Cmd_accuracycheck);
   unsigned hit=core->busRead32(core,SYM_gBattlescriptCurrInstr)==script+7;
   CHECK(hit==(roll<accuracies[m]));
   hits+=hit;
  }
  CHECK(hits==accuracies[m]);
 }
 // Each fang outcome is independent: paralysis and flinch each roll 10%;
 // Serene Grace doubles either independently, not their combined probability.
 const unsigned secondaryMoves[]={MOVE_DISCHARGE,MOVE_THUNDER_FANG,
  MOVE_THUNDER_FANG,MOVE_ZEN_HEADBUTT,MOVE_CROSS_POISON,
  MOVE_POISON_JAB,MOVE_GUNK_SHOT};
 const unsigned effects[]={MOVE_EFFECT_PARALYSIS,MOVE_EFFECT_PARALYSIS,
  MOVE_EFFECT_FLINCH,MOVE_EFFECT_FLINCH,MOVE_EFFECT_POISON,
  MOVE_EFFECT_POISON,MOVE_EFFECT_POISON};
 const unsigned percent[]={30,10,10,20,10,30,30};
 for(unsigned m=0;m<7;m++)for(unsigned serene=0;serene<2;serene++)
 for(unsigned guard=0;guard<4;guard++)for(unsigned boundary=0;boundary<2;boundary++){
  CHECK(core->loadState(core,snap));
  unsigned t=percent[m]*(serene?2:1),roll=boundary?t:t-1;
  core->busWrite8(core,SYM_gBattlerAttacker,0);
  core->busWrite8(core,SYM_gBattlerTarget,1);
  core->busWrite16(core,SYM_gCurrentMove,secondaryMoves[m]);
  core->busWrite8(core,SYM_gMoveResultFlags,0);
  core->busWrite32(core,SYM_gHitMarker,0);
  core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,serene?ABILITY_SERENE_GRACE:ABILITY_NONE);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,
    guard==1?ABILITY_SHIELD_DUST:guard==3?ABILITY_INNER_FOCUS:ABILITY_NONE);
  if(guard==2)core->busWrite32(core,bmon(1)+ABI_BMON_STATUS2,STATUS2_SUBSTITUTE);
  core->busWrite8(core,SYM_gBattlerByTurnOrder,0);
  core->busWrite8(core,SYM_gBattlerByTurnOrder+1,1);
  core->busWrite8(core,SYM_gCurrentTurnActionNumber,0);
  core->busWrite8(core,SYM_gBattleCommunication+MOVE_EFFECT_BYTE,effects[m]);
  core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(roll));
  core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);
  CALL(Cmd_seteffectwithchance);
  unsigned flinch=(m==2||m==3);
  unsigned applied=flinch?
    !!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_FLINCHED):
    !!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&
      (effects[m]==MOVE_EFFECT_PARALYSIS?STATUS1_PARALYSIS:STATUS1_POISON));
  CHECK(applied==(!boundary&&(guard==0||(guard==3&&!flinch))));
 }
 CHECK(core->loadState(core,snap));CALL(Free,script);free(snap);
 printf("PASS: Batch B native exact 80/90/95/100 accuracy, independent Thunder Fang effects and 10/20/30%% secondary boundaries with ability guards\\n");
}
