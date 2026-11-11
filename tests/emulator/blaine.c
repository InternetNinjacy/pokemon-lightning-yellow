// Integration acceptance for the separately owner-merged Blaine change.
#include "constants/heal_locations.h"
static void blaineFixture(unsigned count) {
 daycareFixture(1);core->busWrite8(core,SYM_gPlayerPartyCount,count);
 for(unsigned i=count;i<6;i++)clearBytes(player(i),ABI_MON_SIZE);
 fossilWarp(MAP_GROUP(MAP_CINNABAR_ISLAND_GYM),MAP_NUM(MAP_CINNABAR_ISLAND_GYM),3,3);
 unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,8,MAP_NUM(MAP_CINNABAR_ISLAND_GYM),MAP_GROUP(MAP_CINNABAR_ISLAND_GYM));
 CHECK(object<ABI_OBJECT_COUNT);core->busWrite8(core,SYM_gSelectedObjectEvent,object);
}
static void blaineVictoryTests(void) {
 blaineFixture(2);CALL(SetLastHealLocationWarp,HEAL_LOCATION_CINNABAR_ISLAND);
 for(unsigned i=0;i<2;i++){CALL(CreateMon,player(i),SPECIES_MEWTWO,100,31,1,i,0,0);monput(player(i),ABI_DATA_MOVE1,MOVE_SWIFT);monput(player(i),ABI_DATA_PP1,99);}
 CHECK(CALL(BagGetQuantityByItemId,ITEM_TM38)==0);fossilStart(SYM_CinnabarIsland_Gym_EventScript_Blaine);waitAction();
 unsigned f,quiet=0,seen=0;const unsigned species[]={SPECIES_DONPHAN,SPECIES_PIDGEOT,SPECIES_SANDSLASH,SPECIES_AERODACTYL,SPECIES_FLYGON};
 for(f=0;f<40000;f++) {
  if(fieldScriptDone()){if(++quiet==30)break;frames(1,0);continue;}quiet=0;
  unsigned key=0;
  if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_BattleMainCB2){
   for(unsigned b=1;b<4;b+=2)if(core->busRead16(core,bmon(b)+ABI_BMON_HP))for(unsigned i=0;i<5;i++)if(core->busRead16(core,bmon(b)+ABI_BMON_SPECIES)==species[i])seen|=1U<<i;
   if(f%8==0){key=2;unsigned exec=core->busRead32(core,SYM_gBattleControllerExecFlags);
    for(unsigned b=0;b<4;b+=2)if(exec&(1U<<b)){
     unsigned fn=controller(b),c;
     if(fn==SYM_HandleInputChooseAction){c=core->busRead8(core,SYM_gActionSelectionCursor+b);key=c&2?64:c&1?32:1;break;}
     if(fn==SYM_HandleInputChooseMove){c=core->busRead8(core,SYM_gMoveSelectionCursor+b);key=c&2?64:c&1?32:1;break;}
     if(fn==SYM_HandleInputChooseTarget){key=1;break;}
    }
   }
  }else if(f%8==0)key=2;
  frames(1,key);
 }
 if(f==40000)fprintf(stderr,"Blaine win timeout cb%08x outcome%u flags%u/%u/%u\n",core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),core->busRead8(core,SYM_gBattleOutcome),CALL(FlagGet,FLAG_DEFEATED_BLAINE),CALL(FlagGet,FLAG_BADGE07_GET),CALL(FlagGet,FLAG_GOT_TM38_FROM_BLAINE));
 CHECK(f<40000);CHECK(seen==31);CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_WON);
 CHECK(CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_LEADER_BLAINE));CHECK(CALL(FlagGet,FLAG_DEFEATED_BLAINE));CHECK(CALL(FlagGet,FLAG_BADGE07_GET));CHECK(CALL(FlagGet,FLAG_GOT_TM38_FROM_BLAINE));CHECK(CALL(BagGetQuantityByItemId,ITEM_TM38)==1);
 flashRoundTrip();CHECK(CALL(FlagGet,FLAG_BADGE07_GET));CHECK(CALL(FlagGet,FLAG_GOT_TM38_FROM_BLAINE));CHECK(CALL(BagGetQuantityByItemId,ITEM_TM38)==1);
 CALL(CB2_ContinueSavedGame);frames(480,0);CHECK(fieldScriptDone());
 unsigned obj=CALL(GetObjectEventIdByLocalIdAndMap,8,MAP_NUM(MAP_CINNABAR_ISLAND_GYM),MAP_GROUP(MAP_CINNABAR_ISLAND_GYM));CHECK(obj<ABI_OBJECT_COUNT);core->busWrite8(core,SYM_gSelectedObjectEvent,obj);
 fossilStart(SYM_CinnabarIsland_Gym_EventScript_Blaine);fossilClose(1);CHECK(CALL(BagGetQuantityByItemId,ITEM_TM38)==1);CHECK(CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_LEADER_BLAINE));
 printf("PASS: actual Blaine Double victory/all five opponents seen, original badge/TM38/defeat script, flash Continue and nonduplicating revisit in%u frames; controlled players, unchanged opponent party\n",f);
 CHECK(core->loadState(core,bootState));
}
// Compute the actual decrypted BoxPokemon checksum from the 12 encrypted
// secure words without calling ROM routines or modifying the running battle.
// A changing *stored* checksum alone does not establish corruption.
static unsigned blaineCalculatedChecksum(unsigned slot) {
 uint32_t base=enemy(slot),key=core->busRead32(core,base)^core->busRead32(core,base+4);
 unsigned sum=0;
 for(unsigned word=0;word<12;word++) {
  uint32_t decrypted=core->busRead32(core,base+0x20+word*4)^key;
  sum+=(decrypted&0xffff)+(decrypted>>16);
 }
 return sum&0xffff;
}
// Audit only raw encrypted party header bytes while the unchanged native
// battle-intro frames run. No ROM calls or writes are made during tracing.
static void blaineWaitActionWithAudit(void) {
 unsigned frame,seenCreated=0,seenCorrupt=0,seenChecksumChange=0,seenCipherMismatch=0;
 unsigned firstChecksum=0,lastChecksum=0,checksumChanges=0;
 for(frame=0;frame<2400;frame++) {
  unsigned flags=core->busRead8(core,enemy(0)+0x13);
  if((flags&2) && !seenCreated) {
   seenCreated=1;
   firstChecksum=lastChecksum=core->busRead16(core,enemy(0)+0x1c);
   fprintf(stderr,"BLAINE_PARTY_CREATED frame=%u flags=%02x checksum=%04x callback=%08x\\n",
     frame,flags,core->busRead16(core,enemy(0)+0x1c),
     core->busRead32(core,SYM_gMain+ABI_MAIN_CB2));
  }
  if(seenCreated && !seenChecksumChange
     && core->busRead16(core,enemy(0)+0x1c)!=firstChecksum) {
   seenChecksumChange=1;
   fprintf(stderr,"BLAINE_CHECKSUM_CHANGE_FIRST frame=%u from=%04x to=%04x flags=%02x personality=%08x ot_id=%08x callback=%08x battle_main=%08x exec=%08x\\n",
    frame,firstChecksum,core->busRead16(core,enemy(0)+0x1c),flags,
    core->busRead32(core,enemy(0)),core->busRead32(core,enemy(0)+4),
    core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),
    core->busRead32(core,SYM_gBattleMainFunc),
    core->busRead32(core,SYM_gBattleControllerExecFlags));
  }
  if(seenCreated) {
   unsigned checksum=core->busRead16(core,enemy(0)+0x1c);
   unsigned calculated=blaineCalculatedChecksum(0);
   if(checksum!=calculated && !seenCipherMismatch) {
    seenCipherMismatch=1;
    fprintf(stderr,"BLAINE_CIPHER_FIRST_INVALID frame=%u stored=%04x computed=%04x flags=%02x callback=%08x\\n",
     frame,checksum,calculated,flags,core->busRead32(core,SYM_gMain+ABI_MAIN_CB2));
   }
   if(checksum!=lastChecksum) {
    if(checksumChanges<16)
     fprintf(stderr,"BLAINE_CHECKSUM_TRANSITION n=%u frame=%u from=%04x to=%04x computed=%04x valid=%u flags=%02x callback=%08x\\n",
      checksumChanges,frame,lastChecksum,checksum,calculated,checksum==calculated,flags,
      core->busRead32(core,SYM_gMain+ABI_MAIN_CB2));
    checksumChanges++;
    lastChecksum=checksum;
   }
  }
  if((flags&1) && !seenCorrupt) {
   seenCorrupt=1;
   fprintf(stderr,"BLAINE_PARTY_BAD_EGG_FIRST frame=%u flags=%02x checksum=%04x callback=%08x battle_main=%08x exec=%08x\\n",
     frame,flags,core->busRead16(core,enemy(0)+0x1c),
     core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),
     core->busRead32(core,SYM_gBattleMainFunc),
     core->busRead32(core,SYM_gBattleControllerExecFlags));
  }
  if(controller(0)==SYM_HandleInputChooseAction)break;
  frames(1,(frame%12)<3?1:0);
 }
 if(frame==2400)fprintf(stderr,"BLAINE_AUDIT_TIMEOUT flags=%02x\\n",core->busRead8(core,enemy(0)+0x13));
 CHECK(frame<2400);
 // Preserve the unmodified native turn and capture the exact boundary at
 // which the host subsequently calls GetMonData3. The constructor can be
 // observed mid-encryption across several frames; only a stable checksum
 // mismatch or a getter-induced flag change is evidence of corruption.
 fprintf(stderr,"BLAINE_AUDIT_STOP frame=%u stored=%04x calculated=%04x flags=%02x controller=%08x\\n",
  frame,core->busRead16(core,enemy(0)+0x1c),blaineCalculatedChecksum(0),
  core->busRead8(core,enemy(0)+0x13),controller(0));
 // The previous exact-head run was valid at controller-stop but invalid
 // three frames later. Observe each real emulated frame and the 12 raw secure
 // words, without ROM calls, save-state restores or changes to game memory.
 uint32_t previousSecure[12];
 for(unsigned w=0;w<12;w++)previousSecure[w]=core->busRead32(core,enemy(0)+0x20+4*w);
 for(unsigned step=1;step<=3;step++) {
  unsigned beforeStored=core->busRead16(core,enemy(0)+0x1c);
  unsigned beforeComputed=blaineCalculatedChecksum(0);
  unsigned beforeFlags=core->busRead8(core,enemy(0)+0x13);
  unsigned beforeController=controller(0);
  unsigned beforeCb=core->busRead32(core,SYM_gMain+ABI_MAIN_CB2);
  frames(1,0);
  unsigned changes=0;
  for(unsigned w=0;w<12;w++) {
   uint32_t current=core->busRead32(core,enemy(0)+0x20+4*w);
   if(current!=previousSecure[w]) {
    fprintf(stderr,"BLAINE_SECURE_WORD_CHANGED step=%u word=%u before=%08x after=%08x\\n",
     step,w,previousSecure[w],current);
    changes++;
   }
   previousSecure[w]=current;
  }
  fprintf(stderr,"BLAINE_LAST_FRAMES step=%u stored=%04x/%04x computed=%04x/%04x flags=%02x/%02x secure_changes=%u controller=%08x/%08x cb=%08x/%08x exec=%08x\\n",
   step,beforeStored,core->busRead16(core,enemy(0)+0x1c),
   beforeComputed,blaineCalculatedChecksum(0),beforeFlags,
   core->busRead8(core,enemy(0)+0x13),changes,beforeController,controller(0),
   beforeCb,core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),
   core->busRead32(core,SYM_gBattleControllerExecFlags));
 }
 fprintf(stderr,"BLAINE_AUDIT_EXIT stored=%04x calculated=%04x flags=%02x controller=%08x\\n",
  core->busRead16(core,enemy(0)+0x1c),blaineCalculatedChecksum(0),
  core->busRead8(core,enemy(0)+0x13),controller(0));
}
static void blaineTests(void) {
 blaineFixture(1);fossilStart(SYM_CinnabarIsland_Gym_EventScript_Blaine);fossilClose(1);
 CHECK(!(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_TRAINER));
 CHECK(!CALL(FlagGet,FLAG_BADGE07_GET));CHECK(!CALL(FlagGet,FLAG_DEFEATED_BLAINE));
 blaineFixture(2);
 // Exercise the real NPC battle without an injected constructor call and
 // snapshot restore immediately beforehand. The previous audit proved the
 // constructor initially valid, but the subsequent native battle contained
 // corrupted encrypted enemy records. Keep both strict checks, isolated.
 fossilStart(SYM_CinnabarIsland_Gym_EventScript_Blaine);blaineWaitActionWithAudit();
 CHECK(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_TRAINER);
 CHECK(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_DOUBLE);
 CHECK(core->busRead8(core,SYM_gBattlersCount)==4);
 const unsigned species[]={SPECIES_DONPHAN,SPECIES_PIDGEOT,SPECIES_SANDSLASH,SPECIES_AERODACTYL,SPECIES_FLYGON};
 const unsigned levels[]={42,42,43,44,47};
 for(unsigned i=0;i<5;i++){
  unsigned beforeFlags=core->busRead8(core,enemy(i)+0x13);
  unsigned beforeStored=core->busRead16(core,enemy(i)+0x1c);
  unsigned beforeCalculated=blaineCalculatedChecksum(i);
  unsigned actualSpecies=monget(enemy(i),ABI_DATA_SPECIES);
  unsigned afterSpeciesFlags=core->busRead8(core,enemy(i)+0x13);
  unsigned afterSpeciesStored=core->busRead16(core,enemy(i)+0x1c);
  unsigned afterSpeciesCalculated=blaineCalculatedChecksum(i);
  unsigned actualLevel=monget(enemy(i),ABI_DATA_LEVEL);
  if(i==0 || beforeFlags!=afterSpeciesFlags || beforeStored!=beforeCalculated)
   fprintf(stderr,"BLAINE_GETMON_BOUNDARY slot=%u before_flags=%02x before_checksum=%04x/%04x after_species_flags=%02x after_checksum=%04x/%04x species=%u level=%u\\n",
    i,beforeFlags,beforeStored,beforeCalculated,afterSpeciesFlags,afterSpeciesStored,afterSpeciesCalculated,actualSpecies,actualLevel);
  if(actualSpecies!=species[i]||actualLevel!=levels[i]){
   // 412 is SPECIES_EGG: discriminate corrupt encrypted party records from
   // incorrect trainer selection without changing canonical expectations.
   fprintf(stderr,"BLAINE_NATIVE_DIAGNOSTIC index=%u species=%u expected_species=%u level=%u expected_level=%u enemy_count=%u battler_count=%u flags=%08x bad_egg=%u is_egg=%u trainer=%u battle_left=%u battle_right=%u\\n",
    i,actualSpecies,species[i],actualLevel,levels[i],core->busRead8(core,SYM_gEnemyPartyCount),
    core->busRead8(core,SYM_gBattlersCount),core->busRead32(core,SYM_gBattleTypeFlags),
    monget(enemy(i),ABI_DATA_SANITY_IS_BAD_EGG),monget(enemy(i),ABI_DATA_IS_EGG),
    core->busRead16(core,SYM_gTrainerBattleOpponent_A),
    core->busRead16(core,bmon(1)+ABI_BMON_SPECIES),core->busRead16(core,bmon(3)+ABI_BMON_SPECIES));
   // Isolate a corrupted encrypted party record from the correctly loaded
   // battle battler; never rewrite/rekey Pokémon or bypass the assertion.
   fprintf(stderr,"BLAINE_ENCRYPTED_RECORD index=%u personality=%08x ot_id=%08x stored_checksum=%04x stored_flags=%02x party_index_left=%u party_index_right=%u\\n",
    i,monget(enemy(i),ABI_DATA_PERSONALITY),monget(enemy(i),ABI_DATA_OT_ID),
    core->busRead16(core,enemy(i)+0x1c),core->busRead8(core,enemy(i)+0x13),
    core->busRead16(core,SYM_gBattlerPartyIndexes+2),
    core->busRead16(core,SYM_gBattlerPartyIndexes+6));
  }
  CHECK(actualSpecies==species[i]);CHECK(actualLevel==levels[i]);
 }
 CHECK(core->busRead16(core,bmon(1)+ABI_BMON_SPECIES)==SPECIES_DONPHAN);
 CHECK(core->busRead16(core,bmon(3)+ABI_BMON_SPECIES)==SPECIES_PIDGEOT);
 CHECK(core->busRead8(core,bmon(3)+ABI_BMON_TYPE1)==TYPE_FLYING);
 CHECK(core->busRead8(core,bmon(3)+ABI_BMON_TYPE2)==TYPE_FLYING);
 core->busWrite32(core,SYM_gBattleMoveDamage,100);CALL(TypeCalc,MOVE_EARTHQUAKE,1,3);
 CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==0);
 printf("PASS: actual Blaine NPC refuses one usable Pokemon, starts trainer Double with correct five-level party/Donphan-Pidgeot leads and native Earthquake ally immunity\n");
 // Discard the modified type-check fixture; victory uses a fresh native battle.
 CHECK(core->loadState(core,bootState));
 blaineVictoryTests();
 // Independent constructor acceptance runs only AFTER both native battle
 // paths, so injected ROM calls cannot contaminate the battle under test.
 blaineFixture(2);
 core->busWrite32(core,SYM_gBattleTypeFlags,BATTLE_TYPE_TRAINER|BATTLE_TYPE_DOUBLE);
 CHECK(CALL(CreateNPCTrainerParty,SYM_gEnemyParty,TRAINER_LEADER_BLAINE)==5);
 const unsigned constructorSpecies[]={SPECIES_DONPHAN,SPECIES_PIDGEOT,SPECIES_SANDSLASH,SPECIES_AERODACTYL,SPECIES_FLYGON};
 const unsigned constructorLevels[]={42,42,43,44,47};
 for(unsigned i=0;i<5;i++){
  unsigned actualSpecies=monget(enemy(i),ABI_DATA_SPECIES);
  unsigned actualLevel=monget(enemy(i),ABI_DATA_LEVEL);
  unsigned badEgg=monget(enemy(i),ABI_DATA_SANITY_IS_BAD_EGG);
  if(actualSpecies!=constructorSpecies[i]||actualLevel!=constructorLevels[i]||badEgg)
   fprintf(stderr,"BLAINE_CONSTRUCTOR_DIAGNOSTIC slot=%u species=%u expected=%u level=%u expectedLevel=%u badEgg=%u\\n",
    i,actualSpecies,constructorSpecies[i],actualLevel,constructorLevels[i],badEgg);
  CHECK(actualSpecies==constructorSpecies[i]);
  CHECK(actualLevel==constructorLevels[i]);
  CHECK(!badEgg);
 }
 printf("PASS: isolated native Blaine five-member encrypted party constructor after complete battle acceptance\\n");
 CHECK(core->loadState(core,bootState));
}
