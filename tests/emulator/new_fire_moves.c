// Actual compiled appended moves; no synthetic move IDs or animation patches.
static void newFireMoveTests(void){
 const unsigned moves[]={MOVE_FLAME_FANG,MOVE_FLASHFIRE,MOVE_BLAZE_RUSH};
 const unsigned pp[]={15,30,10},damage[]={81,37,90};
 for(unsigned m=0;m<3;m++){
  effectBattleFixture(0,moves[m]);damageStats();CHECK(baseDamage(moves[m],0,0,0)==damage[m]);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,999);core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,1);
  CHECK(baseDamage(moves[m],0,0,0)==damage[m]);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
  core->busWrite16(core,SYM_gCurrentMove,moves[m]);CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==damage[m]);
  for(unsigned two=0;two<2;two++){
   effectBattleFixture(two,moves[m]);
   core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
   core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,200);core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
   core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,300);core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,1);
   void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned flinch=0,plain=0;
   for(unsigned trial=0;trial<40;trial++){
    CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0xF1AE0000+m*97+trial*7919);
    chooseMove(0,0,1);if(two)chooseMove(2,0,1);waitAction();
    unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
    CHECK(dealt>=damage[m]*85/100&&dealt<=damage[m]);
    CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
    CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)==0); // No imported Flame Fang burn.
    CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000-(m==2?(dealt/4?dealt/4:1):0));
    if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
    unsigned remaining=0;for(unsigned slot=0;slot<4;slot++)remaining+=core->busRead8(core,bmon(1)+ABI_BMON_PP+slot);
    flinch+=remaining==160;plain+=remaining==159;
    if(m|| (flinch&&plain))break;
   }
   if(!m)CHECK(flinch&&plain);
   free(state);
   printf("PASS: appended Fire move%u actual %s animation/damage/onePP/selected target, recoil%u and flinch controls%u/%u\n",moves[m],two?"Double":"Single",m==2,flinch,plain);
  }
 }
 // Reusing Hi Jump Kick artwork must not import crash damage on miss/protect.
 for(unsigned guard=0;guard<3;guard++){
  effectBattleFixture(0,guard==2?MOVE_FLASHFIRE:MOVE_BLAZE_RUSH);
  if(guard==0)for(unsigned slot=0;slot<4;slot++){
   core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*slot,MOVE_PROTECT);core->busWrite8(core,bmon(1)+ABI_BMON_PP+slot,10);
  }
  if(guard==1){core->busWrite8(core,bmon(0)+ABI_BMON_STAGES+STAT_ACC,0);core->busWrite8(core,bmon(1)+ABI_BMON_STAGES+STAT_EVASION,12);}
  if(guard==2)core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_FLASH_FIRE);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned prevented=0;
  for(unsigned trial=0;trial<20&&!prevented;trial++){
   CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0xB1A2E000+trial*7919);
   chooseMove(0,0,1);waitAction();
   if(core->busRead16(core,bmon(1)+ABI_BMON_HP)==1000){
    CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);prevented=1;
    CHECK(monget(player(0),ABI_DATA_PP1)==(guard==2?29:9));
   }
  }
  CHECK(prevented);free(state);
 }
 printf("PASS: Blaze Rush no recoil/crash on Protect or miss; Flash Fire ability blocks Flashfire normally\n");
 // Priority+1 must strike before a faster ordinary attack.
 effectBattleFixture(0,MOVE_FLASHFIRE);
 core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,1);core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,300);
 for(unsigned slot=0;slot<4;slot++){core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*slot,MOVE_TACKLE);core->busWrite8(core,bmon(1)+ABI_BMON_PP+slot,35);}
 chooseMove(0,0,1);unsigned seen=0;
 for(unsigned frame=0;frame<1200&&!seen;frame++){
  frames(1,(frame%12)<2?1:0);unsigned ours=core->busRead16(core,bmon(0)+ABI_BMON_HP),theirs=core->busRead16(core,bmon(1)+ABI_BMON_HP);
  if(ours<1000||theirs<1000){CHECK(theirs<1000&&ours==1000);seen=1;}
 }
 CHECK(seen);waitAction();CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)<1000);
 // CPU snapshots do not restore flash. Isolate this new-save test from earlier
 // full-suite saves, and restore their flash afterwards for later groups.
 CHECK(core->loadState(core,bootState));
 GBASavedataForceType(&((struct GBA*)core->board)->memory.savedata,SAVEDATA_FLASH1M);
 void *oldFlash=NULL;size_t oldSize=core->savedataClone(core,&oldFlash);CHECK(oldSize==0x20000&&oldFlash);
 // Negative control: resetting RAM counters alone must NOT supersede an
 // existing higher-counter save. This reproduces the full-suite failure.
 initSave();create(SPECIES_BULBASAUR,20);core->busWrite8(core,SYM_gPlayerPartyCount,1);
 core->busWrite32(core,SYM_gSaveCounter,0x100000);CHECK(CALL(TrySavingData,0)==1);CHECK(CALL(TrySavingData,0)==1);
 CHECK(core->loadState(core,bootState));initSave();create(SPECIES_CHARMANDER,50);
 core->busWrite8(core,SYM_gPlayerPartyCount,1);CHECK(CALL(TrySavingData,0)==1);
 CHECK(CALL(LoadGameSave,0)==1);CHECK(monget(player(0),ABI_DATA_SPECIES)==SPECIES_BULBASAUR);
 CHECK(core->loadState(core,bootState));
 unsigned char *blank=malloc(0x20000);CHECK(blank);memset(blank,255,0x20000);
 CHECK(core->savedataRestore(core,blank,0x20000,true));
 void *verify=NULL;size_t size=core->savedataClone(core,&verify);
 CHECK(size==0x20000&&verify&&!memcmp(blank,verify,size));free(verify);free(blank);
 initSave();create(SPECIES_CHARMANDER,50);
 core->busWrite8(core,SYM_gPlayerPartyCount,1);
 for(unsigned slot=0;slot<3;slot++){CALL(SetMonMoveSlot,player(0),moves[slot],slot);monput(player(0),ABI_DATA_PP1+slot,slot+1);}
 monput(player(0),ABI_DATA_PP_BONUSES,63);
 unsigned char saved[ABI_BOX_SIZE];readBytes(player(0),saved,sizeof(saved));
 CHECK(CALL(TrySavingData,0)==1);
 for(unsigned i=0;i<ABI_SAVE1_SIZE;i+=4)core->busWrite32(core,SYM_gSaveBlock1+i,0);
 for(unsigned i=0;i<ABI_SAVE2_SIZE;i+=4)core->busWrite32(core,SYM_gSaveBlock2+i,0);
 CALL(SetMonMoveSlot,player(0),MOVE_SPLASH,0);CHECK(CALL(LoadGameSave,0)==1);
 sameBytes(player(0),saved,sizeof(saved));
 for(unsigned slot=0;slot<3;slot++){CHECK(monget(player(0),ABI_DATA_MOVE1+slot)==moves[slot]);CHECK(monget(player(0),ABI_DATA_PP1+slot)==slot+1);}
 CHECK(!monget(player(0),ABI_DATA_SANITY_IS_BAD_EGG));
 CHECK(core->savedataRestore(core,oldFlash,oldSize,true));free(oldFlash);
 CHECK(core->loadState(core,bootState));
 printf("PASS: new IDs355..357, spent PP/PP Ups and exact encrypted record persist through real flash save/load\n");
 printf("PASS: Flashfire+1 acts before faster normal-priority opponent, all three appended moves use physical stats and native AI\n");
}
