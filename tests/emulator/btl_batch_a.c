// BTL-005 Batch A: native Singles/Doubles acceptance on real ROM.
// The dedicated staging entry point does not certify unimplemented B/C slots.
static void batchANativeTests(void){
 const unsigned moves[]={
  MOVE_SEED_BOMB,MOVE_PETAL_BLIZZARD,MOVE_AURA_SPHERE,MOVE_FOCUS_BLAST,
  MOVE_FLASH_CANNON,MOVE_MIRROR_SHOT,MOVE_IRON_HEAD,MOVE_BULLET_PUNCH,
  MOVE_AIR_SLASH,MOVE_HEART_STAMP
 };
 const unsigned pp[]={15,15,20,5,10,10,15,30,15,25};
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
  core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,m==7?1:500);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,m==7?500:1);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
  if(m==7){
   // Positive priority control versus a faster opponent using Swift.
   for(unsigned slot=0;slot<4;slot++){
    CALL(SetMonMoveSlot,enemy(0),MOVE_SWIFT,slot);
    core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*slot,MOVE_SWIFT);
    core->busWrite8(core,bmon(1)+ABI_BMON_PP+slot,20);
   }
  }
  unsigned raw=baseDamage(moves[m],0,0,0);
  CHECK(raw>0&&raw<900);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite16(core,SYM_gCurrentMove,moves[m]);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==raw);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
  // Full exact accuracy and proc/ability boundaries are tested separately;
  // this suite exercises real battle turns and actual animation completion.
  unsigned hit=0;
  for(unsigned t=0;t<12&&!hit;t++){
   CHECK(core->loadState(core,state));
   core->busWrite32(core,SYM_gRngValue,0xBA700000U + m*19001U + two*101U + t*7919U);
   chooseMove(0,0,1);
   if(two)chooseMove(2,0,1);
   unsigned frame=0,seen=0;
   for(;frame<4800;frame++){
    if(controller(0)==SYM_HandleInputChooseAction)break;
    frames(1,(frame%12)<3?1:0);
    if(!seen&&core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000){
     if(m==7)CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
     seen=1;
    }
   }
   CHECK(frame<4800);
   frames(3,0);
   unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
   CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
   if(dealt){
    // baseDamage uses the real production calculation, including the
    // approved 75% all-other-battler modifier for Petal Blizzard in Doubles.
    // Do not apply spread damage a second time in the acceptance oracle.
    unsigned expected=raw;
    if(dealt<expected*85/100-2||dealt>expected+2)
     fprintf(stderr,"BATCH_A_DAMAGE m=%u two=%u expected=%u dealt=%u\\n",m,two,expected,dealt);
    CHECK(dealt+2>=expected*85/100&&dealt<=expected+2);
    hit++;
   }
   if(two){
    if(m==1){
     CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)<1000);
     CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)<1000);
    }else{
     CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);
     CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);
    }
   }
   // Native secondary effects and immunity/ability guards are verified
   // by batchAExactMechanics across every percentage boundary.
  }
  CHECK(hit);
  free(state);
  printf("PASS: Batch A move%u %s native targeting, damage, actual animation, PP and partial proc/priority probe (flinch acceptance pending)\\n",moves[m],two?"Doubles":"Singles");
 }
 CHECK(core->loadState(core,bootState));
}

static void batchAExactMechanics(void){
 const unsigned moves[]={
  MOVE_SEED_BOMB,MOVE_PETAL_BLIZZARD,MOVE_AURA_SPHERE,MOVE_FOCUS_BLAST,
  MOVE_FLASH_CANNON,MOVE_MIRROR_SHOT,MOVE_IRON_HEAD,MOVE_BULLET_PUNCH,
  MOVE_AIR_SLASH,MOVE_HEART_STAMP
 };
 const unsigned accuracy[]={100,100,0,70,100,85,100,100,95,100};
 effectBattleFixture(0,MOVE_SEED_BOMB);
 unsigned script=CALL(AllocZeroed,16);CHECK(script);
 void *snapshot=malloc(core->stateSize(core));CHECK(snapshot&&core->saveState(core,snapshot));
 // Every possible percent-roll from the actual native accuracy command.
 // The 0 sentinel is genuine accuracy bypass, not a 0%-hit move.
 for(unsigned m=0;m<10;m++){
  unsigned hits=0;
  for(unsigned roll=0;roll<100;roll++){
   CHECK(core->loadState(core,snapshot));
   core->busWrite8(core,SYM_gBattlerAttacker,0);
   core->busWrite8(core,SYM_gBattlerTarget,1);
   core->busWrite16(core,SYM_gCurrentMove,moves[m]);
   core->busWrite8(core,SYM_gMoveResultFlags,0);
   core->busWrite32(core,SYM_gHitMarker,0);
   if(m==2){
    core->busWrite8(core,bmon(0)+ABI_BMON_STAGES+STAT_ACC,0);
    core->busWrite8(core,bmon(1)+ABI_BMON_STAGES+STAT_EVASION,12);
   }
   for(unsigned i=0;i<4;i++)core->busWrite8(core,script+1+i,(script+12)>>(8*i));
   core->busWrite8(core,script+5,moves[m]&255);
   core->busWrite8(core,script+6,moves[m]>>8);
   core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);
   core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(roll));
   CALL(Cmd_accuracycheck);
   unsigned hit=core->busRead32(core,SYM_gBattlescriptCurrInstr)==script+7;
   CHECK(hit==(accuracy[m]==0||roll<accuracy[m]));
   hits+=hit;
  }
  CHECK(hits==(accuracy[m]==0?100:accuracy[m]));
 }
 // Exact native secondary-effect boundaries and protections, including
 // Serene Grace, Shield Dust, Substitute and Inner Focus for flinch.
 const unsigned secMoves[]={MOVE_FOCUS_BLAST,MOVE_FLASH_CANNON,MOVE_MIRROR_SHOT,
                            MOVE_IRON_HEAD,MOVE_AIR_SLASH,MOVE_HEART_STAMP};
 const unsigned effects[]={MOVE_EFFECT_SP_DEF_MINUS_1,MOVE_EFFECT_SP_DEF_MINUS_1,
                           MOVE_EFFECT_ACC_MINUS_1,MOVE_EFFECT_FLINCH,
                           MOVE_EFFECT_FLINCH,MOVE_EFFECT_FLINCH};
 const unsigned chances[]={10,10,30,30,30,30};
 for(unsigned m=0;m<6;m++)for(unsigned serene=0;serene<2;serene++)
 for(unsigned guard=0;guard<4;guard++)for(unsigned boundary=0;boundary<2;boundary++){
  CHECK(core->loadState(core,snapshot));
  unsigned threshold=chances[m]*(serene?2:1);
  unsigned roll=threshold-(boundary?0:1);
  core->busWrite8(core,SYM_gBattlerAttacker,0);
  core->busWrite8(core,SYM_gBattlerTarget,1);
  core->busWrite16(core,SYM_gCurrentMove,secMoves[m]);
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
  unsigned applied= m==2 ?
    core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_ACC)==5 :
    m<2 ? core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_SPDEF)==5 :
    !!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_FLINCHED);
  CHECK(applied==(!boundary && (guard==0||(guard==3&&m<3))));
 }
 CHECK(core->loadState(core,snapshot));CALL(Free,script);free(snapshot);
 printf("PASS: Batch A exact native 0/70/85/95/100 accuracy and 10/30%% secondary effects with Serene Grace/Shield Dust/Substitute/Inner Focus protections\\n");
}

static void batchAFlashPersistence(void){
 const unsigned moves[]={MOVE_SEED_BOMB,MOVE_PETAL_BLIZZARD,MOVE_AURA_SPHERE,
  MOVE_FOCUS_BLAST,MOVE_FLASH_CANNON,MOVE_MIRROR_SHOT,MOVE_IRON_HEAD,
  MOVE_BULLET_PUNCH,MOVE_AIR_SLASH,MOVE_HEART_STAMP,MOVE_SPLASH,MOVE_SPLASH};
 const unsigned maxPp[]={15,15,20,5,10,10,15,30,15,25,40,40};
 CHECK(core->loadState(core,bootState));
 GBASavedataForceType(&((struct GBA*)core->board)->memory.savedata,SAVEDATA_FLASH1M);
 void *oldFlash=NULL;
 size_t flashSize=core->savedataClone(core,&oldFlash);
 CHECK(flashSize==0x20000&&oldFlash);
 unsigned char *blank=malloc(0x20000);CHECK(blank);
 memset(blank,255,0x20000);
 CHECK(core->savedataRestore(core,blank,0x20000,true));
 free(blank);
 initSave();
 unsigned char saved[3][ABI_BOX_SIZE];
 for(unsigned p=0;p<3;p++){
  CALL(CreateMon,player(p),SPECIES_SQUIRTLE,50,31,1,12345+p,0,1234+p);
  for(unsigned slot=0;slot<4;slot++){
   unsigned i=p*4+slot;
   CALL(SetMonMoveSlot,player(p),moves[i],slot);
   monput(player(p),ABI_DATA_PP1+slot,maxPp[i]-1);
  }
  monput(player(p),ABI_DATA_PP_BONUSES,p==0?0:p==1?21:42);
  readBytes(player(p),saved[p],sizeof(saved[p]));
 }
 core->busWrite8(core,SYM_gPlayerPartyCount,3);
 CHECK(CALL(TrySavingData,0)==1);
 for(unsigned k=0;k<ABI_SAVE1_SIZE;k+=4)core->busWrite32(core,SYM_gSaveBlock1+k,0);
 for(unsigned k=0;k<ABI_SAVE2_SIZE;k+=4)core->busWrite32(core,SYM_gSaveBlock2+k,0);
 for(unsigned p=0;p<3;p++)for(unsigned slot=0;slot<4;slot++)
  CALL(SetMonMoveSlot,player(p),MOVE_POUND,slot);
 CHECK(CALL(LoadGameSave,0)==1);
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==3);
 for(unsigned p=0;p<3;p++){
  sameBytes(player(p),saved[p],sizeof(saved[p]));
  for(unsigned slot=0;slot<4;slot++){
   unsigned i=p*4+slot;
   CHECK(monget(player(p),ABI_DATA_MOVE1+slot)==moves[i]);
   CHECK(monget(player(p),ABI_DATA_PP1+slot)==maxPp[i]-1);
  }
  CHECK(monget(player(p),ABI_DATA_PP_BONUSES)==(p==0?0:p==1?21:42));
  CHECK(!monget(player(p),ABI_DATA_SANITY_IS_BAD_EGG));
 }
 CHECK(core->savedataRestore(core,oldFlash,flashSize,true));
 free(oldFlash);
 CHECK(core->loadState(core,bootState));
 printf("PASS: Batch A all ten move IDs, spent PP, PP bonuses and encrypted Pokemon records persist through actual flash save/load\\n");
}
