// Real encrypted records, actual flash load/recovery and Leech Life battle turns.
static unsigned ppBoxGet(uint32_t p,unsigned f){return CALL(GetBoxMonData3,p,f,0);}
static void ppBoxSet(uint32_t p,unsigned f,unsigned v){core->busWrite32(core,scratch,v);CALL(SetBoxMonData,p,f,scratch);}
static void ppLegacyMon(unsigned slot,unsigned bonus,unsigned current){
 CALL(CreateMon,enemy(0),SPECIES_BULBASAUR,50,31,1,0x12345678,1,0x89ABCDEF);
 for(unsigned i=0;i<4;i++)CALL(SetMonMoveSlot,enemy(0),MOVE_TACKLE,i);
 CALL(SetMonMoveSlot,enemy(0),MOVE_LEECH_LIFE,slot);
 monput(enemy(0),ABI_DATA_PP_BONUSES,(255&~(3U<<(2*slot)))|(bonus<<(2*slot)));
 monput(enemy(0),ABI_DATA_PP1+slot,current);
}
static void ppCapMatrix(void){
 CHECK(core->loadState(core,bootState));initSave();
 const unsigned max[]={10,12,14,16};
 for(unsigned slot=0;slot<4;slot++)for(unsigned bonus=0;bonus<4;bonus++){
  const unsigned values[]={0,5,max[bonus],max[bonus]+1,15+3*bonus};
  for(unsigned i=0;i<5;i++){
   ppLegacyMon(slot,bonus,values[i]);copyBytes(enemy(1),enemy(0),ABI_BOX_SIZE);
   unsigned expected=values[i]>max[bonus]?max[bonus]:values[i];
   ppBoxSet(enemy(1),ABI_DATA_PP1+slot,expected);
   unsigned char exact[ABI_BOX_SIZE];readBytes(enemy(1),exact,sizeof(exact));
   CHECK(CALL(ClampApprovedMovePP,enemy(0))==(values[i]>max[bonus]));sameBytes(enemy(0),exact,sizeof(exact));
   CHECK(ppBoxGet(enemy(0),ABI_DATA_PP1+slot)==expected);CHECK(!ppBoxGet(enemy(0),ABI_DATA_SANITY_IS_BAD_EGG));
   CHECK(!CALL(ClampApprovedMovePP,enemy(0)));sameBytes(enemy(0),exact,sizeof(exact));
  }
 }
 // Every GenIII secure-block permutation, varied OT keys and actual Shiny state.
 for(unsigned order=0;order<24;order++){
  unsigned pid=0x12345678U+order,ot=(pid>>16)^(pid&65535)^((order&1)?8:0);
  CALL(CreateMon,enemy(0),SPECIES_BULBASAUR,50,31,1,pid,1,ot);
  CALL(SetMonMoveSlot,enemy(0),MOVE_LEECH_LIFE,0);monput(enemy(0),ABI_DATA_PP_BONUSES,3);monput(enemy(0),ABI_DATA_PP1,24);
  if(order==0){monput(enemy(0),ABI_DATA_IS_EGG,1);monput(enemy(0),ABI_DATA_UNUSED_RIBBONS,11);}
  unsigned shiny=CALL(IsMonShiny,enemy(0));CHECK(shiny==!(order&1));
  copyBytes(enemy(1),enemy(0),ABI_BOX_SIZE);ppBoxSet(enemy(1),ABI_DATA_PP1,16);unsigned char exact[ABI_BOX_SIZE];readBytes(enemy(1),exact,sizeof(exact));
  CHECK(CALL(ClampApprovedMovePP,enemy(0)));sameBytes(enemy(0),exact,sizeof(exact));CHECK(CALL(IsMonShiny,enemy(0))==shiny);
 }
 for(unsigned bonus=0;bonus<4;bonus++){
  ppLegacyMon(0,0,5);CALL(SetMonMoveSlot,enemy(0),MOVE_ROAR,2);monput(enemy(0),ABI_DATA_PP_BONUSES,bonus<<4);monput(enemy(0),ABI_DATA_PP3,255);
  CHECK(CALL(ClampApprovedMovePP,enemy(0)));CHECK(monget(enemy(0),ABI_DATA_PP3)==20+4*bonus);CHECK(monget(enemy(0),ABI_DATA_PP1)==5);
 }
 // An unrelated move's invalid PP is not silently repaired by this feature.
 ppLegacyMon(0,0,5);CALL(SetMonMoveSlot,enemy(0),MOVE_SILVER_WIND,2);monput(enemy(0),ABI_DATA_PP3,255);
 unsigned char raw[ABI_BOX_SIZE];readBytes(enemy(0),raw,sizeof(raw));CHECK(!CALL(ClampApprovedMovePP,enemy(0)));sameBytes(enemy(0),raw,sizeof(raw));
 ppLegacyMon(0,0,15);core->busWrite8(core,enemy(0)+ABI_BOX_SECURE,core->busRead8(core,enemy(0)+ABI_BOX_SECURE)^1);
 readBytes(enemy(0),raw,sizeof(raw));CHECK(!CALL(ClampApprovedMovePP,enemy(0)));sameBytes(enemy(0),raw,sizeof(raw));
 ppLegacyMon(0,0,15);monput(enemy(0),ABI_DATA_SANITY_IS_BAD_EGG,1);readBytes(enemy(0),raw,sizeof(raw));CHECK(!CALL(ClampApprovedMovePP,enemy(0)));sameBytes(enemy(0),raw,sizeof(raw));
 ppLegacyMon(0,0,15);monput(enemy(0),ABI_DATA_SPECIES,1000);readBytes(enemy(0),raw,sizeof(raw));CHECK(!CALL(ClampApprovedMovePP,enemy(0)));sameBytes(enemy(0),raw,sizeof(raw));
 clearBytes(enemy(0),ABI_BOX_SIZE);core->busWrite8(core,enemy(0)+ABI_BOX_SECURE,123);readBytes(enemy(0),raw,sizeof(raw));CHECK(!CALL(ClampApprovedMovePP,enemy(0)));sameBytes(enemy(0),raw,sizeof(raw));
 printf("PASS: encrypted PP cap matrix all4 slots/all4 PP-Up counts, zero/spent/at/above/legacy maxima, all24 block orders/Shiny/tagged-Egg identity, idempotence, unchanged Roar maxima, corrupt/BadEgg/unknown/empty and unrelated-move preservation\n");
}
static void ppFullPc(void){
 daycareFixture(0);ppLegacyMon(0,3,24);
 for(unsigned box=0;box<14;box++)for(unsigned slot=0;slot<30;slot++)copyBytes(CALL(GetBoxedMonPtr,box,slot),enemy(0),ABI_BOX_SIZE);
 CALL(NormalizeLoadedMovePP);
 for(unsigned box=0;box<14;box++)for(unsigned slot=0;slot<30;slot++)CHECK(ppBoxGet(CALL(GetBoxedMonPtr,box,slot),ABI_DATA_PP1)==16);
 printf("PASS: all420 full-PC encrypted records normalized within the guarded ARM invocation budget\n");
}
static void ppSaveSites(void){
 daycareFixture(1);uint32_t r=CALL(GetRoute5Daycare),f=fourCare();
 carePair(r,SPECIES_BULBASAUR,SPECIES_DITTO);carePair(f,SPECIES_BULBASAUR,SPECIES_DITTO);
 pendingEgg(1);pendingEgg(0);
 ppLegacyMon(0,3,24);copyBytes(player(0),enemy(0),ABI_BOX_SIZE);
 uint32_t first=CALL(GetBoxedMonPtr,0,0),last=CALL(GetBoxedMonPtr,13,29);
 copyBytes(first,enemy(0),ABI_BOX_SIZE);copyBytes(last,enemy(0),ABI_BOX_SIZE);
 copyBytes(r,enemy(0),ABI_BOX_SIZE);copyBytes(f,enemy(0),ABI_BOX_SIZE);
 // Mail and accumulated experience are opaque metadata and must remain intact.
 for(unsigned i=ABI_BOX_SIZE;i<ABI_CARE_STEPS;i++){core->busWrite8(core,r+i,i*7+3);core->busWrite8(core,f+i,i*11+5);}
 core->busWrite32(core,r+ABI_CARE_STEPS,12345);core->busWrite32(core,f+ABI_CARE_STEPS,54321);
 unsigned char route[400],island[ABI_CARE_SIZE];readBytes(r5save(),route,sizeof(route));readBytes(f,island,sizeof(island));
 copyBytes(enemy(1),enemy(0),ABI_BOX_SIZE);ppBoxSet(enemy(1),ABI_DATA_PP1,16);
 unsigned char expected[ABI_BOX_SIZE];readBytes(enemy(1),expected,sizeof(expected));
 memcpy(route+ABI_R5_CARE,expected,sizeof(expected));memcpy(island,expected,sizeof(expected));
 flashRoundTrip();sameBytes(player(0),expected,sizeof(expected));sameBytes(careSave1()+ABI_SAVE1_PARTY,expected,sizeof(expected));
 sameBytes(CALL(GetBoxedMonPtr,0,0),expected,sizeof(expected));sameBytes(CALL(GetBoxedMonPtr,13,29),expected,sizeof(expected));sameBytes(r5save(),route,sizeof(route));sameBytes(fourCare(),island,sizeof(island));
 flashRoundTrip();sameBytes(r5save(),route,sizeof(route));sameBytes(fourCare(),island,sizeof(island));
 // Import remains lazy; the complete read-only legacy recovery record survives.
 ppLegacyMon(1,0,15);uint32_t legacy=careSave1()+ABI_SAVE1_LEGACY_CARE;clearBytes(legacy,ABI_CARE_MON_SIZE);copyBytes(legacy,enemy(0),ABI_BOX_SIZE);core->busWrite32(core,legacy+ABI_CARE_STEPS,23456);for(unsigned i=ABI_BOX_SIZE;i<ABI_CARE_STEPS;i++)core->busWrite8(core,legacy+i,i*13+7);
 unsigned char old[ABI_CARE_MON_SIZE];readBytes(legacy,old,sizeof(old));clearBytes(r5save(),400);flashRoundTrip();sameBytes(legacy,old,sizeof(old));
 for(unsigned i=0;i<400;i++)CHECK(core->busRead8(core,r5save()+i)==0);
 copyBytes(enemy(1),enemy(0),ABI_BOX_SIZE);ppBoxSet(enemy(1),ABI_DATA_PP2,10);unsigned char migrated[ABI_CARE_MON_SIZE];memcpy(migrated,old,sizeof(old));readBytes(enemy(1),migrated,ABI_BOX_SIZE);
 r=CALL(GetRoute5Daycare);CHECK(r);sameBytes(r,migrated,sizeof(migrated));sameBytes(legacy,old,sizeof(old));flashRoundTrip();sameBytes(CALL(GetRoute5Daycare),migrated,sizeof(migrated));sameBytes(legacy,old,sizeof(old));
 // Unknown and inconsistent recognized Route5 saves are never rewritten.
 core->busWrite8(core,r5save()+ABI_R5_VERSION,99);unsigned char unknown[400];readBytes(r5save(),unknown,sizeof(unknown));flashRoundTrip();sameBytes(r5save(),unknown,sizeof(unknown));CHECK(!CALL(GetRoute5Daycare));sameBytes(legacy,old,sizeof(old));
 core->busWrite8(core,r5save()+ABI_R5_VERSION,1);core->busWrite8(core,r5save()+ABI_R5_SHINY,2);readBytes(r5save(),unknown,sizeof(unknown));flashRoundTrip();sameBytes(r5save(),unknown,sizeof(unknown));CHECK(!CALL(GetRoute5Daycare));
 printf("PASS: actual flash load caps live/saved party, first/last PC and both independent pending daycares; lazy legacy import caps active copy only, preserves recovery/mail/steps/egg identity; unknown/corrupt Route5 stays byte-identical\n");
}
static void ppBackupRecovery(void){
 daycareFixture(0);ppLegacyMon(0,0,15);copyBytes(player(0),enemy(0),ABI_BOX_SIZE);
 CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,111);CHECK(CALL(TrySavingData,0)==1);
 CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,222);CHECK(CALL(TrySavingData,0)==1);
 unsigned counter=core->busRead32(core,SYM_gSaveCounter);void *data=NULL;size_t size=core->savedataClone(core,&data);CHECK(size==0x20000&&data);
 unsigned offset=(counter%2)*14*ABI_SECTOR_SIZE+ABI_SECTOR_SIGNATURE;
 ((unsigned char*)data)[offset]^=1;CHECK(core->savedataRestore(core,data,size,true));free(data);
 clearBytes(careSave1(),ABI_SAVE1_SIZE);clearBytes(careSave2(),ABI_SAVE2_SIZE);clearBytes(SYM_gPlayerParty,6*ABI_MON_SIZE);
 CHECK(CALL(LoadGameSave,0)==255);CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==111);CHECK(monget(player(0),ABI_DATA_PP1)==10);CHECK(ppBoxGet(careSave1()+ABI_SAVE1_PARTY,ABI_DATA_PP1)==10);
 // NO_FLASH and EMPTY leave above-max records alone rather than migrating junk.
 ppLegacyMon(0,0,15);copyBytes(player(0),enemy(0),ABI_BOX_SIZE);unsigned char exact[ABI_BOX_SIZE];readBytes(player(0),exact,sizeof(exact));
 core->busWrite8(core,SYM_gFlashMemoryPresent,0);CHECK(CALL(LoadGameSave,0)==255);sameBytes(player(0),exact,sizeof(exact));core->busWrite8(core,SYM_gFlashMemoryPresent,1);
 CALL(SavePlayerParty);unsigned char *blank=malloc(0x20000);CHECK(blank);memset(blank,255,0x20000);CHECK(core->savedataRestore(core,blank,0x20000,true));
 CHECK(CALL(LoadGameSave,0)==0);sameBytes(player(0),exact,sizeof(exact));
 // Known signatures but bad checksums: INVALID, not recovered ERROR.
 memset(blank,0,0x20000);for(unsigned sector=0;sector<28;sector++){
  uint32_t sig=0x08012025;memcpy(blank+sector*ABI_SECTOR_SIZE+ABI_SECTOR_SIGNATURE,&sig,4);blank[sector*ABI_SECTOR_SIZE+ABI_SECTOR_CHECKSUM]=1;
 }
 CHECK(core->savedataRestore(core,blank,0x20000,true));free(blank);CHECK(CALL(LoadGameSave,0)==2);sameBytes(player(0),exact,sizeof(exact));
 printf("PASS: actual latest-slot corruption recovers older save with ERROR status, correct money and PP cap; NO_FLASH/EMPTY/INVALID skip migration\n");
}

static void ppLeechLifeBattles(void){
 wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);startSpecial();waitAction();damageStats();
 CHECK(baseDamage(MOVE_LEECH_LIFE,0,0,0)==10);
 core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,400);CHECK(baseDamage(MOVE_LEECH_LIFE,0,0,0)==10);
 core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,100);CHECK(baseDamage(MOVE_LEECH_LIFE,0,0,0)==19);
 damageStats();CHECK(baseDamage(MOVE_LEECH_LIFE,SIDE_STATUS_REFLECT,0,0)==10);CHECK(baseDamage(MOVE_LEECH_LIFE,SIDE_STATUS_LIGHTSCREEN,0,0)==6);
 core->busWrite32(core,bmon(0)+ABI_BMON_STATUS1,STATUS1_BURN);CHECK(baseDamage(MOVE_LEECH_LIFE,0,0,0)==10);
 for(unsigned two=0;two<2;two++)for(unsigned ooze=0;ooze<2;ooze++){
  // CPU snapshots do not restore flash; the previous recovery test corrupts it.
  unsigned char *clean=malloc(0x20000);CHECK(clean);memset(clean,255,0x20000);
  GBASavedataForceType(&((struct GBA*)core->board)->memory.savedata,SAVEDATA_FLASH1M);
  CHECK(core->savedataRestore(core,clean,0x20000,true));free(clean);
  wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),two);
  for(unsigned party=0;party<=two;party++)for(unsigned move=0;move<4;move++)CALL(SetMonMoveSlot,player(party),party==0&&move==0?MOVE_LEECH_LIFE:MOVE_SPLASH,move);
  CHECK(CALL(TryStartLandEncounter,landInfo(),0));waitAction();
  core->busWrite16(core,SYM_gBattleWeather,0);
  for(unsigned side=0;side<2;side++)core->busWrite16(core,SYM_gSideStatuses+2*side,0);
  for(unsigned battler=0;battler<(two?4:2);battler++){
   core->busWrite8(core,bmon(battler)+ABI_BMON_ABILITY,ABILITY_NONE);
   core->busWrite8(core,bmon(battler)+ABI_BMON_TYPE1,TYPE_NORMAL);core->busWrite8(core,bmon(battler)+ABI_BMON_TYPE2,TYPE_NORMAL);
   if(battler&1){core->busWrite32(core,bmon(battler)+ABI_BMON_STATUS1,3);monput(enemy(battler>>1),ABI_DATA_STATUS,3);}
  }
  for(unsigned b=0;b<2;b++){
   uint32_t mon=b?enemy(0):player(0);monput(mon,ABI_DATA_MAX_HP,1000);monput(mon,ABI_DATA_HP,500);
   core->busWrite16(core,bmon(b)+ABI_BMON_MAX_HP,1000);core->busWrite16(core,bmon(b)+ABI_BMON_HP,500);
  }
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ooze?ABILITY_LIQUID_OOZE:ABILITY_NONE);
  chooseMove(0,0,1);if(two)chooseMove(2,0,1);waitAction();
  unsigned damage=500-core->busRead16(core,bmon(1)+ABI_BMON_HP);CHECK(damage>0&&damage<500);
  unsigned heal=damage/2;if(!heal)heal=1;
  CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==(ooze?500-heal:500+heal));
  CHECK(monget(player(0),ABI_DATA_HP)==(ooze?500-heal:500+heal));CHECK(monget(player(0),ABI_DATA_PP1)==9);
  press(16);press(128);press(1);waitOverworld(1);CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);
  flashRoundTrip();CHECK(monget(player(0),ABI_DATA_PP1)==9);CHECK(monget(player(0),ABI_DATA_HP)==(ooze?500-heal:500+heal));
  printf("PASS: actual %s Leech Life turn/animation, %u HP damage, %s%u HP,9PP, Run/field/flash persistence\n",two?"Double":"Single",damage,ooze?"Liquid Ooze loss":"drain heal",heal);
 }
 // Execute the real item-effect engine and ordinary healing, not a PP mock.
 daycareFixture(0);CALL(SetMonMoveSlot,player(0),MOVE_LEECH_LIFE,0);monput(player(0),ABI_DATA_PP1,0);
 CHECK(!CALL(ExecuteTableBasedItemEffect,player(0),ITEM_ETHER,0,0));CHECK(monget(player(0),ABI_DATA_PP1)==10);
 CHECK(!CALL(ExecuteTableBasedItemEffect,player(0),ITEM_PP_UP,0,0));CHECK(CALL(CalculatePPWithBonus,MOVE_LEECH_LIFE,monget(player(0),ABI_DATA_PP_BONUSES),0)==12);CHECK(monget(player(0),ABI_DATA_PP1)==12);
 monput(player(0),ABI_DATA_PP1,1);CALL(MonRestorePP,player(0));CHECK(monget(player(0),ABI_DATA_PP1)==12);
 printf("PASS: native Special80 damage/stats/screens/burn and Ether/PP Up/ordinary restore use10/12PP; no Bag UI claim\n");
}
static void movePpCompatibilityTests(void){
 ppCapMatrix();ppFullPc();ppSaveSites();ppBackupRecovery();ppLeechLifeBattles();CHECK(core->loadState(core,bootState));
}
