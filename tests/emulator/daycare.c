// Compiled-ROM daycare/crypto/hatching fixtures, sharing guarded ARM invocation.
#include "constants/daycare.h"
#include "constants/game_stat.h"
static uint32_t careSave1(void) {return core->busRead32(core,SYM_gSaveBlock1Ptr);}
static uint32_t careSave2(void) {return core->busRead32(core,SYM_gSaveBlock2Ptr);}
static uint32_t r5save(void) {return careSave1()+ABI_SAVE1_ROUTE5;}
static uint32_t fourCare(void) {return careSave1()+ABI_SAVE1_CARE;}
static void clearBytes(uint32_t p,unsigned n) {for(unsigned i=0;i<n;i++)core->busWrite8(core,p+i,0);}
static void readBytes(uint32_t p,void *dest,unsigned n) {for(unsigned i=0;i<n;i++)((unsigned char*)dest)[i]=core->busRead8(core,p+i);}
static void sameBytes(uint32_t p,const void *expected,unsigned n) {for(unsigned i=0;i<n;i++){if(core->busRead8(core,p+i)!=((const unsigned char*)expected)[i])fprintf(stderr,"Byte mismatch address%08x offset%u actual%02x expected%02x\n",p,i,core->busRead8(core,p+i),((const unsigned char*)expected)[i]);CHECK(core->busRead8(core,p+i)==((const unsigned char*)expected)[i]);}}
static void copyBytes(uint32_t dest,uint32_t src,unsigned n) {for(unsigned i=0;i<n;i++)core->busWrite8(core,dest+i,core->busRead8(core,src+i));}
static void careLocation(unsigned route5) {
 core->busWrite8(core,careSave1()+ABI_SAVE1_LOCATION,route5?MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE):MAP_GROUP(MAP_FOUR_ISLAND_POKEMON_DAY_CARE));
 core->busWrite8(core,careSave1()+ABI_SAVE1_LOCATION+1,route5?MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE):MAP_NUM(MAP_FOUR_ISLAND_POKEMON_DAY_CARE));
}
static void daycareFixture(unsigned route5) {
 CHECK(core->loadState(core,bootState));
 // A CPU snapshot is not a flash-file snapshot. Give each independent fixture
 // fresh in-memory flash; round trips within that fixture retain the same flash.
 GBASavedataForceType(&((struct GBA*)core->board)->memory.savedata,SAVEDATA_FLASH1M);
 unsigned char *blank=malloc(0x20000);CHECK(blank);memset(blank,0xFF,0x20000);
 CHECK(core->savedataRestore(core,blank,0x20000,true));
 void *verify=NULL;size_t bytes=core->savedataClone(core,&verify);
 CHECK(bytes==0x20000 && verify && !memcmp(blank,verify,bytes));free(verify);free(blank);
 initSave();CALL(DisableNationalPokedex);
 core->busWrite8(core,careSave2()+ABI_SAVE2_PLAYER_NAME,0xCA);
 core->busWrite8(core,careSave2()+ABI_SAVE2_PLAYER_NAME+1,0xFF);
 for(unsigned i=0;i<4;i++)core->busWrite8(core,careSave2()+ABI_SAVE2_OTID+i,(0x78563412U>>(8*i))&255);
 CALL(ResetTasks);CALL(ZeroPlayerPartyMons);
 CALL(FlagSet,FLAG_SYS_POKEMON_GET);CALL(FlagSet,FLAG_SYS_POKEDEX_GET);
 for(unsigned i=0;i<3;i++)CALL(CreateMon,player(i),SPECIES_PIKACHU,20,31,1,i?255:0,0,0);
 core->busWrite8(core,SYM_gPlayerPartyCount,3);
 CALL(SetWarpDestination,route5?MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE):MAP_GROUP(MAP_FOUR_ISLAND_POKEMON_DAY_CARE),route5?MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE):MAP_NUM(MAP_FOUR_ISLAND_POKEMON_DAY_CARE),0xFFFFFFFF,3,6);
 CALL(WarpIntoMap);CALL(ResetInitialPlayerAvatarState);
 core->busWrite32(core,SYM_gMain,0);CALL(CB2_LoadMap);frames(360,0);
 CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
 CHECK(!CALL(IsNationalPokedexEnabled));
}
static void careParent(uint32_t care,unsigned slot,unsigned species,unsigned pid,unsigned ot,unsigned item) {
 uint32_t dest=care+slot*ABI_CARE_MON_SIZE;
 clearBytes(dest,ABI_CARE_MON_SIZE);
 CALL(CreateMon,enemy(0),species,20,31,1,pid,1,ot);
 monput(enemy(0),ABI_DATA_HELD_ITEM,item);
 copyBytes(dest,enemy(0),ABI_BOX_SIZE);
}
static void carePair(uint32_t care,unsigned a,unsigned b) {
 clearBytes(care,ABI_CARE_SIZE);
 careParent(care,0,a,0,0x11223344,0);
 careParent(care,1,b,255,0x55667788,0);
}
static uint32_t pendingEgg(unsigned route5) {
 careLocation(route5);CALL(TriggerPendingDaycareEgg);
 uint32_t care=CALL(GetActiveDaycare);CHECK(core->busRead16(core,care+ABI_CARE_PENDING));
 return care;
}
static unsigned pickEgg(unsigned route5) {
 careLocation(route5);unsigned count=core->busRead8(core,SYM_gPlayerPartyCount);
 CALL(GiveEggFromDaycare);CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==count+1);
 CHECK(monget(player(count),ABI_DATA_IS_EGG));return count;
}
static void flashRoundTrip(void) {
 CHECK(CALL(TrySavingData,0)==1);
 clearBytes(careSave1(),ABI_SAVE1_SIZE);clearBytes(careSave2(),ABI_SAVE2_SIZE);
 CHECK(CALL(LoadGameSave,0)==1);
}
static void migrationTests(void) {
 daycareFixture(1);
 // Create an actual pre-feature deposit, with real encrypted data and mail.
 CHECK(CALL(GiveMailToMon,player(0),ITEM_ORANGE_MAIL)!=255);
 unsigned mail=monget(player(0),ABI_DATA_MAIL);
 core->busWrite16(core,careSave1()+ABI_SAVE1_MAIL+mail*ABI_MAIL_SIZE+ABI_MAIL_WORDS,0x1234);
 uint32_t legacy=careSave1()+ABI_SAVE1_LEGACY_CARE;
 CALL(StorePokemonInDaycare,player(0),legacy);
 core->busWrite32(core,legacy+ABI_CARE_STEPS,1234);
 unsigned char old[ABI_CARE_MON_SIZE],other[ABI_CARE_SIZE];
 readBytes(legacy,old,sizeof(old));carePair(fourCare(),SPECIES_RATTATA,SPECIES_DITTO);
 careLocation(0);CALL(TriggerPendingDaycareEgg);readBytes(fourCare(),other,sizeof(other));
 clearBytes(r5save(),400);flashRoundTrip();
 uint32_t care=CALL(GetRoute5Daycare);CHECK(care==r5save()+ABI_R5_CARE);
 sameBytes(care,old,sizeof(old));sameBytes(legacy,old,sizeof(old));sameBytes(fourCare(),other,sizeof(other));
 flashRoundTrip();sameBytes(CALL(GetRoute5Daycare),old,sizeof(old));
 careLocation(1);core->busWrite16(core,SYM_gSpecialVar_0x8004,0);CALL(GetDaycareCost);
 CHECK(core->busRead16(core,SYM_gSpecialVar_0x8005)==100*(CALL(GetNumLevelsGainedFromDaycare)+1));
 CHECK(CALL(TakePokemonFromDaycare)==SPECIES_PIKACHU);
 unsigned slot=core->busRead8(core,SYM_gPlayerPartyCount)-1;
 CHECK(monget(player(slot),ABI_DATA_HELD_ITEM)==ITEM_ORANGE_MAIL);
 unsigned restored=monget(player(slot),ABI_DATA_MAIL);CHECK(restored!=255);
 CHECK(core->busRead16(core,careSave1()+ABI_SAVE1_MAIL+restored*ABI_MAIL_SIZE+ABI_MAIL_WORDS)==0x1234);
 CHECK(monget(player(slot),ABI_DATA_PERSONALITY)==0);
 CHECK(!monget(player(slot),ABI_DATA_SANITY_IS_BAD_EGG));
 CHECK(CALL(GetDaycarePokemonCount)==0);CHECK(CALL(GetRoute5Daycare)==care);
 sameBytes(fourCare(),other,sizeof(other));
 // Recognized unknown version never erases migrated parents or redirects to FI.
 core->busWrite16(core,r5save()+4,2);CHECK(!CALL(GetRoute5Daycare));
 sameBytes(fourCare(),other,sizeof(other));
 printf("PASS: real legacy deposit/mail/1234 steps migrated byte-for-byte, flash reload, fees/withdrawal and Four Island isolation\n");
}
static void compatibilityTests(void) {
 daycareFixture(1);uint32_t care=CALL(GetRoute5Daycare);
 const unsigned cases[][5]={
  {SPECIES_PIKACHU,0,SPECIES_PIKACHU,255,70},
  {SPECIES_PIKACHU,0,SPECIES_DITTO,255,50},
  {SPECIES_PIKACHU,255,SPECIES_DITTO,0,50},
  {SPECIES_MAGNEMITE,0,SPECIES_DITTO,0,50},
  {SPECIES_PIKACHU,0,SPECIES_MAGIKARP,255,0},
  {SPECIES_DITTO,0,SPECIES_DITTO,255,0},
  {SPECIES_MAGNEMITE,0,SPECIES_MAGNEMITE,255,0},
  {SPECIES_PICHU,0,SPECIES_DITTO,0,0},
  {SPECIES_MEWTWO,0,SPECIES_DITTO,0,0},
  {SPECIES_PIKACHU,255,SPECIES_PIKACHU,255,0}};
 CHECK(CALL(GetDaycareState)==DAYCARE_NO_MONS);
 for(unsigned i=0;i<sizeof(cases)/sizeof(*cases);i++) {
  careParent(care,0,cases[i][0],cases[i][1],1,0);
  careParent(care,1,cases[i][2],cases[i][3],2,0);
  unsigned score=CALL(GetDaycareCompatibilityScore,care);
  if(score!=cases[i][4])fprintf(stderr,"compat case%u actual%u expected%u\n",i,score,cases[i][4]);
  CHECK(score==cases[i][4]);
 }
 carePair(care,SPECIES_PIKACHU,SPECIES_DITTO);
 carePair(fourCare(),SPECIES_RATTATA,SPECIES_DITTO);
 // Neither site has an eligible parent-step checkpoint; party hatching is once.
 CALL(CreateEgg,player(3),SPECIES_PICHU,0);core->busWrite8(core,SYM_gPlayerPartyCount,4);
 monput(player(3),ABI_DATA_FRIENDSHIP,5);
 core->busWrite8(core,fourCare()+ABI_CARE_COUNTER,254);
 core->busWrite8(core,care+ABI_CARE_COUNTER,254);
 CHECK(!CALL(ShouldEggHatch));CHECK(monget(player(3),ABI_DATA_FRIENDSHIP)==4);
 CHECK(core->busRead32(core,care+ABI_CARE_STEPS)==1);
 CHECK(core->busRead32(core,fourCare()+ABI_CARE_STEPS)==1);
 CHECK(!CALL(ShouldEggHatch));CHECK(monget(player(3),ABI_DATA_FRIENDSHIP)==4);
 // Real steps, standard compatibility chance, each site's own pending state.
 for(unsigned i=0;i<10000 && (!core->busRead16(core,care+ABI_CARE_PENDING)||!core->busRead16(core,fourCare()+ABI_CARE_PENDING));i++)CALL(ShouldEggHatch);
 CHECK(core->busRead16(core,care+ABI_CARE_PENDING));CHECK(core->busRead16(core,fourCare()+ABI_CARE_PENDING));
 unsigned char pending[400],island[ABI_CARE_SIZE];readBytes(r5save(),pending,400);readBytes(fourCare(),island,sizeof(island));
 flashRoundTrip();sameBytes(r5save(),pending,400);sameBytes(fourCare(),island,sizeof(island));
 careLocation(1);core->busWrite32(core,SYM_gRngValue,0xBAC0FFEE);CALL(TriggerPendingDaycareEgg);
 sameBytes(r5save(),pending,400);sameBytes(fourCare(),island,sizeof(island));
 // Failed pickup while party full retains the identity and both pending eggs.
 for(unsigned i=4;i<6;i++)CALL(CreateMon,player(i),SPECIES_PIKACHU,20,31,1,255,0,0);
 core->busWrite8(core,SYM_gPlayerPartyCount,6);careLocation(1);CALL(GiveEggFromDaycare);
 sameBytes(r5save(),pending,400);sameBytes(fourCare(),island,sizeof(island));
 // Free fixture slots and collect Route5 only; island's parents/pending remain.
 clearBytes(player(3),3*ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,3);
 unsigned egg=pickEgg(1);CHECK(CALL(IsMonShiny,player(egg))==pending[ABI_R5_SHINY]);
 sameBytes(fourCare(),island,sizeof(island));flashRoundTrip();CHECK(monget(player(egg),ABI_DATA_IS_EGG));
 printf("PASS: compatibility/Ditto/genderless/Undiscovered restrictions, both pending Eggs, full-party retry, independent flash data and single hatch-step progression\n");
}
static void hatchScene(unsigned slot) {
 // A raw LoadGameSave restores serialized objects but is not a Continue/map
 // transition. Reinitialize the actual field before driving a live hatch scene.
 CALL(SetWarpDestination,MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE),MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE),0xFFFFFFFF,3,6);
 CALL(WarpIntoMap);CALL(ResetInitialPlayerAvatarState);CALL(CB2_LoadMap);frames(360,0);
 core->busWrite16(core,SYM_gSpecialVar_0x8004,slot);
 CALL(ScriptContext_SetupScript,SYM_EventScript_EggHatch);
 unsigned f,started=0;
 for(f=0;f<3600;f++) {
  uint32_t cb=core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U;
  if(cb==SYM_CB2_EggHatch_0 || cb==SYM_CB2_EggHatch_1)started=1;
  frames(1,(f%12)<3?(started?2:1):0);
  // Never reenter GetMonData while the live callback may be between decrypt
  // and re-encrypt. Observe only raw controller/script completion, then inspect
  // the finished mon once the ordinary field has resumed.
  if(started && (core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld
   && core->busRead8(core,SYM_sGlobalScriptContextStatus)==2)break;
 }
 if(f==3600){fprintf(stderr,"Hatch timeout egg=%u cb1=%08x cb2=%08x script=%u specialslot=%u\n",monget(player(slot),ABI_DATA_IS_EGG),core->busRead32(core,SYM_gMain),core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),CALL(ScriptContext_IsEnabled),core->busRead16(core,SYM_gSpecialVar_0x8004));screenshot("/tmp/daycare-hatch-timeout.ppm");}
 CHECK(f<3600);CHECK(!monget(player(slot),ABI_DATA_IS_EGG));
 printf("PASS: actual hatch animation/buttons/overworld return in %u frames\n",f);
}
static void offspringTests(void) {
 daycareFixture(1);uint32_t care=CALL(GetRoute5Daycare);
 const unsigned cases[][4]={
  {SPECIES_NIDORAN_F,0,SPECIES_NIDORAN_F,0},
  {SPECIES_NIDORAN_F,0,SPECIES_NIDORAN_M,1},
  {SPECIES_ILLUMISE,0,SPECIES_ILLUMISE,0},
  {SPECIES_ILLUMISE,0,SPECIES_VOLBEAT,1},
  {SPECIES_MARILL,ITEM_SEA_INCENSE,SPECIES_AZURILL,2},
  {SPECIES_MARILL,0,SPECIES_MARILL,2},
  {SPECIES_WOBBUFFET,ITEM_LAX_INCENSE,SPECIES_WYNAUT,2},
  {SPECIES_WOBBUFFET,0,SPECIES_WOBBUFFET,2},
  {SPECIES_MAGNEMITE,0,SPECIES_MAGNEMITE,2}};
 for(unsigned i=0;i<sizeof(cases)/sizeof(*cases);i++) {
  carePair(care,cases[i][0],SPECIES_DITTO);
  careParent(care,0,cases[i][0],0,1,cases[i][1]);
  careLocation(1);
  for(unsigned n=0;n<100;n++) {
   core->busWrite16(core,care+ABI_CARE_PENDING,0);
   CALL(SetRoute5PendingEgg,care,cases[i][3]==1);
   if(cases[i][3]!=0 || !(core->busRead16(core,care+ABI_CARE_PENDING)&EGG_GENDER_MALE))break;
  }
  unsigned egg=pickEgg(1);CHECK(monget(player(egg),ABI_DATA_SPECIES)==cases[i][2]);
  clearBytes(player(egg),ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,3);
 }
 carePair(care,SPECIES_PIKACHU,SPECIES_PIKACHU);
 // Father Wish egg move and Thunderbolt TM; three IVs still inherited by
 // unchanged GenIII routine. Mother0IV, father31IV gives an observable bound.
 CALL(CreateMon,enemy(0),SPECIES_PIKACHU,20,0,1,0,1,1);copyBytes(care,enemy(0),ABI_BOX_SIZE);
 CALL(CreateMon,enemy(0),SPECIES_PIKACHU,20,31,1,255,1,2);
 monput(enemy(0),ABI_DATA_MOVE1,MOVE_WISH);monput(enemy(0),ABI_DATA_MOVE2,MOVE_THUNDERBOLT);
 monput(enemy(0),ABI_DATA_MOVE3,MOVE_NONE);monput(enemy(0),ABI_DATA_MOVE4,MOVE_NONE);
 copyBytes(care+ABI_CARE_MON_SIZE,enemy(0),ABI_BOX_SIZE);
 pendingEgg(1);unsigned egg=pickEgg(1),wish=0,thunder=0,inherited=0;
 CHECK(monget(player(egg),ABI_DATA_SPECIES)==SPECIES_PICHU);
 for(unsigned i=0;i<4;i++){unsigned m=monget(player(egg),ABI_DATA_MOVE1+i);wish|=m==MOVE_WISH;thunder|=m==MOVE_THUNDERBOLT;}
 CHECK(wish && thunder);
 for(unsigned i=0;i<6;i++){unsigned iv=monget(player(egg),ABI_DATA_HP_IV+i);CHECK(iv<=31);inherited+=iv==0||iv==31;}
 CHECK(inherited>=3);
 printf("PASS: Nidoran/Illumise branches, genderless Ditto offspring, incense babies, father egg/TM moves and inherited IVs\n");
}
static void identityAndHatchTests(void) {
 daycareFixture(1);uint32_t care=CALL(GetRoute5Daycare);
 carePair(care,SPECIES_RATTATA,SPECIES_DITTO);
 for(unsigned shiny=0;shiny<2;shiny++) {
  careLocation(1);
  for(unsigned i=0;i<4;i++)core->busWrite8(core,careSave2()+ABI_SAVE2_OTID+i,(0x78563412U>>(8*i))&255);
  for(unsigned n=0;n<100;n++) {
   core->busWrite16(core,care+ABI_CARE_PENDING,0);CALL(TriggerPendingDaycareEgg);
   if(core->busRead8(core,r5save()+ABI_R5_SHINY)==shiny)break;
  }
  CHECK(core->busRead8(core,r5save()+ABI_R5_SHINY)==shiny);
  unsigned egg=pickEgg(1);uint32_t pid=monget(player(egg),ABI_DATA_PERSONALITY);
  CHECK(CALL(IsMonShiny,player(egg))==shiny);
  unsigned gender=CALL(GetMonGender,player(egg)),ability=monget(player(egg),ABI_DATA_ABILITY_NUM);
  unsigned iv[6];for(unsigned i=0;i<6;i++)iv[i]=monget(player(egg),ABI_DATA_HP_IV+i);
  flashRoundTrip();CHECK(monget(player(egg),ABI_DATA_PERSONALITY)==pid);CHECK(CALL(IsMonShiny,player(egg))==shiny);
  // Ownership handoff fixture: receiving trainer has a different ID, exactly
  // the condition the normal hatching code uses after an Egg trade. No link
  // handshake is claimed by this isolated native-data test.
  for(unsigned i=0;i<4;i++)core->busWrite8(core,careSave2()+ABI_SAVE2_OTID+i,(0xBADC0FFEU>>(8*i))&255);
  if(shiny)hatchScene(egg);
  else {core->busWrite16(core,SYM_gSpecialVar_0x8004,egg);CALL(ScriptHatchMon);}
  CHECK(!monget(player(egg),ABI_DATA_IS_EGG));CHECK(CALL(IsMonShiny,player(egg))==shiny);
  CHECK(monget(player(egg),ABI_DATA_OT_ID)==0xBADC0FFEU);
  CHECK(monget(player(egg),ABI_DATA_PERSONALITY)%25==pid%25);
  CHECK(CALL(GetMonGender,player(egg))==gender);
  CHECK(monget(player(egg),ABI_DATA_ABILITY_NUM)==ability);
  CHECK(monget(player(egg),ABI_DATA_UNUSED_RIBBONS)==0);
  for(unsigned i=0;i<6;i++)CHECK(monget(player(egg),ABI_DATA_HP_IV+i)==iv[i]);
  flashRoundTrip();CHECK(CALL(IsMonShiny,player(egg))==shiny);CHECK(!monget(player(egg),ABI_DATA_IS_EGG));
  clearBytes(player(egg),ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,3);
  care=CALL(GetRoute5Daycare); // overworld return may relocate both save blocks
 }
 // Ordinary untagged gift/Island Eggs retain their original PID and normal OT
 // reassignment; the global shiny routines have not changed.
 CALL(CreateEgg,player(3),SPECIES_PICHU,0);core->busWrite8(core,SYM_gPlayerPartyCount,4);
 uint32_t pid=monget(player(3),ABI_DATA_PERSONALITY);
 CHECK(monget(player(3),ABI_DATA_UNUSED_RIBBONS)==0);
 core->busWrite16(core,SYM_gSpecialVar_0x8004,3);CALL(ScriptHatchMon);
 CHECK(monget(player(3),ABI_DATA_PERSONALITY)==pid);
 CHECK(monget(player(3),ABI_DATA_OT_ID)==0xBADC0FFEU);
 printf("PASS: actual stored Shiny outcomes, ownership-change hatch data/animation, nature/gender/ability/IV preservation and post-hatch flash reload; untagged PID unchanged\n");
}
static void statisticalEggTests(void) {
 daycareFixture(1);uint32_t care=CALL(GetRoute5Daycare);
 carePair(care,SPECIES_RATTATA,SPECIES_DITTO);
 clearBytes(player(1),5*ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,1);
 unsigned shiny=0,trans[2][2]={{0}},last=0,bins[10]={0};
 for(unsigned i=0;i<10000;i++) {
  uint32_t seed=0x2A551701U+i;
  seed=(seed^(seed>>16))*0x7FEB352DU;seed=(seed^(seed>>15))*0x846CA68BU;seed^=seed>>16;
  core->busWrite32(core,SYM_gRngValue,seed);
  CALL(TriggerPendingDaycareEgg);
  uint32_t pid=core->busRead32(core,r5save()+ABI_R5_PID),ot=core->busRead32(core,r5save()+ABI_R5_OT);
  unsigned outcome=core->busRead8(core,r5save()+ABI_R5_SHINY);
  unsigned egg=pickEgg(1);
  CHECK(monget(player(egg),ABI_DATA_PERSONALITY)==pid);
  CHECK(monget(player(egg),ABI_DATA_OT_ID)==ot);
  unsigned actual=CALL(IsMonShiny,player(egg));CHECK(actual==outcome);
  CHECK(monget(player(egg),ABI_DATA_ABILITY_NUM)==(pid&1));
  CHECK(!monget(player(egg),ABI_DATA_SANITY_IS_BAD_EGG));
  shiny+=actual;bins[i/1000]+=actual;if(i)trans[last][actual]++;last=actual;
  clearBytes(player(egg),ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,1);
 }
 CHECK(shiny>=4700&&shiny<=5300);
 for(unsigned i=0;i<10;i++)CHECK(bins[i]>400&&bins[i]<600);
 for(unsigned i=0;i<2;i++)for(unsigned j=0;j<2;j++)CHECK(trans[i][j]>2200&&trans[i][j]<2800);
 printf("PASS: 10000 FULL actual ARM-generated Route5 Eggs: %u Shiny/%u non-Shiny, IsMonShiny on encrypted stored data; seed=mix32(0x2A551701+i); transitions=%u/%u/%u/%u\n",shiny,10000-shiny,trans[0][0],trans[0][1],trans[1][0],trans[1][1]);
 // Four Island is sampled with the same varied seed harness. It never receives
 // the marker or 50% transformation. A sample is supporting evidence; unchanged
 // source/generator behavior and deterministic identity tests establish scope.
 careLocation(0);carePair(fourCare(),SPECIES_RATTATA,SPECIES_DITTO);unsigned islandShiny=0;
 for(unsigned i=0;i<10000;i++) {
  uint32_t seed=0x2A551701U+i;
  seed=(seed^(seed>>16))*0x7FEB352DU;seed=(seed^(seed>>15))*0x846CA68BU;seed^=seed>>16;
  core->busWrite32(core,SYM_gRngValue,seed);CALL(TriggerPendingDaycareEgg);
  unsigned egg=pickEgg(0);islandShiny+=CALL(IsMonShiny,player(egg));
  CHECK(monget(player(egg),ABI_DATA_UNUSED_RIBBONS)==0);
  clearBytes(player(egg),ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,1);
 }
 CHECK(islandShiny<20);
 printf("PASS: control10000 actual Four Island Eggs: %u Shiny, no Route5 marker/transformation (normal odds, not a precise rate proof)\n",islandShiny);
}

static unsigned fieldScriptDone(void) {
 return (core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld
     && core->busRead8(core,SYM_sGlobalScriptContextStatus)==2;
}
static void closeCareService(unsigned key) {
 unsigned f;
 for(f=0;f<1800;f++) {
  frames(1,(f%12)<3?key:0);
  if(f>20 && fieldScriptDone())break;
 }
 if(f==1800){fprintf(stderr,"CARE timeout pc%08x lr%08x sp%08x callback%08x\n",((struct ARMCore*)core->cpu)->gprs[15],((struct ARMCore*)core->cpu)->gprs[14],((struct ARMCore*)core->cpu)->gprs[13],core->busRead32(core,SYM_gMain+ABI_MAIN_CB2));screenshot("/tmp/daycare-service-timeout.ppm");}
 CHECK(f<1800);frames(6,0);
}
static void walkCare(unsigned x,unsigned y,unsigned route5) {
 // BFS over actual ROM collision tiles; no warp past furniture to reach an NPC.
 int from[108],queue[108],head=0,tail=0;
 for(unsigned i=0;i<108;i++)from[i]=-2;
 CALL(PlayerGetDestCoords,scratch,scratch+2);
 unsigned sx=core->busRead16(core,scratch)-7,sy=core->busRead16(core,scratch+2)-7,start=sy*12+sx,goal=y*12+x;
 CHECK(sx<12&&sy<9);queue[tail++]=start;from[start]=-1;
 const int dx[]={0,0,-1,1},dy[]={-1,1,0,0};
 while(head<tail && from[goal]==-2) {
  int p=queue[head++],px=p%12,py=p/12;
  for(unsigned d=0;d<4;d++) {
   int nx=px+dx[d],ny=py+dy[d];
   if(nx<0||nx>=12||ny<0||ny>=7)continue;
   if(route5?((ny==4)&&(nx==2||nx==4)):(nx==2&&ny==2))continue;
   int n=ny*12+nx;if(from[n]!=-2 || CALL(MapGridGetCollisionAt,nx+7,ny+7))continue;
   from[n]=p;queue[tail++]=n;
  }
 }
 CHECK(from[goal]!=-2);
 unsigned path[108],n=0;for(int p=goal;p!=(int)start;p=from[p])path[n++]=p;
 int pos=start;
 while(n) {
  int next=path[--n],diff=next-pos;unsigned key=diff==-12?64:diff==12?128:diff==-1?32:16;
  unsigned ticks;
  for(ticks=0;ticks<80;ticks++) {
   frames(1,key);CALL(PlayerGetDestCoords,scratch,scratch+2);
   if(core->busRead16(core,scratch)==(unsigned)(next%12+7)
    && core->busRead16(core,scratch+2)==(unsigned)(next/12+7))break;
  }
  CHECK(ticks<80);frames(20,0);CALL(PlayerGetDestCoords,scratch,scratch+2);
  if(core->busRead16(core,scratch)!=(unsigned)(next%12+7)||core->busRead16(core,scratch+2)!=(unsigned)(next/12+7))fprintf(stderr,"WALK from%d,%d want%d,%d got%u,%u key%u\n",pos%12,pos/12,next%12,next/12,core->busRead16(core,scratch)-7,core->busRead16(core,scratch+2)-7,key);
  CHECK(core->busRead16(core,scratch)==(unsigned)(next%12+7));
  CHECK(core->busRead16(core,scratch+2)==(unsigned)(next/12+7));pos=next;
 }
 press(64); // face the caretaker immediately north; actor collision stops movement
}
static void talkCare(void) {
 press(1);CHECK(core->busRead8(core,SYM_sGlobalScriptContextStatus)!=2);
}
static void openCareParty(void) {
 talkCare();unsigned f;
 for(f=0;f<1800;f++) {
  frames(1,(f%12)<3?1:0);
  if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_UpdatePartyMenu)break;
 }
 if(f==1800)screenshot("/tmp/daycare-party-timeout.ppm");
 CHECK(f<1800);frames(24,0);
}
static void depositCareUi(void) {
 unsigned before=core->busRead8(core,SYM_gPlayerPartyCount);
 openCareParty();CHECK(CALL(GetCursorSelectionMonId)==0);press(1);
 unsigned f;
 for(f=0;f<1800 && core->busRead8(core,SYM_gPlayerPartyCount)==before;f++)frames(1,(f%12)<3?1:0);
 if(f==1800)screenshot("/tmp/daycare-deposit-timeout.ppm");
 CHECK(f<1800);CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==before-1);
 closeCareService(2);
}
static void daycareUiTests(void) {
 daycareFixture(1);CALL(SetGameStat,GAME_STAT_USED_DAYCARE,0);
 CHECK(!CALL(MapGridGetCollisionAt,2+7,4+7));CHECK(!CALL(MapGridGetCollisionAt,4+7,4+7));
 CHECK(CALL(MapGridGetElevationAt,2+7,4+7)==CALL(MapGridGetElevationAt,4+7,4+7));
 walkCare(2,5,1);talkCare();closeCareService(1);
 CHECK(CALL(GetDaycareState)==DAYCARE_NO_MONS);
 walkCare(4,5,1);openCareParty();press(2);closeCareService(1);
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==3);CHECK(CALL(GetDaycarePokemonCount)==0);
 CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,10000);
 depositCareUi();CHECK(CALL(GetDaycarePokemonCount)==1);
 depositCareUi();CHECK(CALL(GetDaycarePokemonCount)==2);
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==1);
 uint32_t care=CALL(GetRoute5Daycare);
 CHECK(core->busRead32(core,care)==0);CHECK(core->busRead32(core,care+ABI_CARE_MON_SIZE)==255);
 CHECK(CALL(GetGameStat,GAME_STAT_USED_DAYCARE)==2);
 // Both sexes were accepted through the same caretaker and same two slots.
 core->busWrite32(core,care+ABI_CARE_STEPS,10000);
 core->busWrite32(core,care+ABI_CARE_MON_SIZE+ABI_CARE_STEPS,20000);
 unsigned cost=0;
 for(unsigned i=0;i<2;i++){core->busWrite16(core,SYM_gSpecialVar_0x8004,i);CALL(GetDaycareCost);cost+=core->busRead16(core,SYM_gSpecialVar_0x8005);}
 CHECK(cost>200 && cost<10000);
 CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,0);
 talkCare();closeCareService(1);
 CHECK(CALL(GetDaycarePokemonCount)==2);CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==1);
 CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==0);
 CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,10000);
 talkCare();closeCareService(1); // select both parents, approve standard fees
 CHECK(CALL(GetDaycarePokemonCount)==0);
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==3);
 CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==10000-cost);
 CHECK(monget(player(1),ABI_DATA_LEVEL)>20 && monget(player(2),ABI_DATA_LEVEL)>20);
 printf("PASS: actual walking/NPC scripts, deposit menu cancellation, both-sex two-slot deposits, quest usage stat, experience and %u paid withdrawal cost\n",cost);

 daycareFixture(1);clearBytes(player(1),5*ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,1);
 walkCare(4,5,1);talkCare();closeCareService(1);
 CHECK(CALL(GetDaycarePokemonCount)==0);CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==1);
 printf("PASS: actual Route5 caretaker refuses last usable Pokemon\n");
 daycareFixture(1);monput(player(1),ABI_DATA_HP,0);monput(player(2),ABI_DATA_HP,0);
 walkCare(4,5,1);openCareParty();press(1);closeCareService(1);
 CHECK(CALL(GetDaycarePokemonCount)==0);CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==3);
 printf("PASS: actual Route5 caretaker rejects depositing the last healthy mon with other fainted party members\n");

 daycareFixture(1);care=CALL(GetRoute5Daycare);carePair(care,SPECIES_PIKACHU,SPECIES_DITTO);pendingEgg(1);
 walkCare(2,5,1);unsigned char pending[400];readBytes(r5save(),pending,400);
 for(unsigned i=3;i<6;i++)CALL(CreateMon,player(i),SPECIES_PIKACHU,20,31,1,255,0,0);
 core->busWrite8(core,SYM_gPlayerPartyCount,6);
 talkCare();closeCareService(1);sameBytes(r5save(),pending,400);
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==6);
 clearBytes(player(3),3*ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,3);
 talkCare();closeCareService(2);sameBytes(r5save(),pending,400);
 talkCare();closeCareService(1);
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==4);CHECK(monget(player(3),ABI_DATA_IS_EGG));
 CHECK(CALL(IsMonShiny,player(3))==pending[ABI_R5_SHINY]);CHECK(!core->busRead16(core,CALL(GetRoute5Daycare)+ABI_CARE_PENDING));
 printf("PASS: actual egg-caretaker full-party rejection, refusal keeps pending identity, acceptance yields the same genuine Egg\n");
 screenshot(getenv("LIGHTNING_DAYCARE_CAPTURE"));

 daycareFixture(0);walkCare(2,4,0);depositCareUi();CHECK(CALL(GetDaycarePokemonCount)==1);
 unsigned char route5[400];readBytes(r5save(),route5,400);
 core->busWrite16(core,SYM_gSpecialVar_0x8004,0);CHECK(CALL(TakePokemonFromDaycare)==SPECIES_PIKACHU);
 sameBytes(r5save(),route5,400);CHECK(CALL(GetDaycarePokemonCount)==0);
 printf("PASS: actual original Four Island map/NPC deposit and native withdrawal remain independent from Route5\n");
}

static void hatchTimingTests(void) {
 daycareFixture(1);uint32_t care=CALL(GetRoute5Daycare);
 CALL(CreateEgg,player(3),SPECIES_PICHU,0);core->busWrite8(core,SYM_gPlayerPartyCount,4);
 monput(player(3),ABI_DATA_FRIENDSHIP,1);
 core->busWrite8(core,care+ABI_CARE_COUNTER,254);core->busWrite8(core,fourCare()+ABI_CARE_COUNTER,254);
 CHECK(!CALL(ShouldEggHatch));CHECK(monget(player(3),ABI_DATA_FRIENDSHIP)==0);
 for(unsigned i=0;i<255;i++)CHECK(!CALL(ShouldEggHatch));
 CHECK(CALL(ShouldEggHatch));CHECK(core->busRead16(core,SYM_gSpecialVar_0x8004)==3);
 printf("PASS: original256-step hatch-cycle spacing retained; zero friendship selects the correct party Egg once\n");
}
static void daycareTests(void) {
 CHECK(ABI_SAVE1_SIZE==0x3D68 && ABI_SAVE1_ROUTE5==0x348C && ABI_SAVE1_CARE==0x2F80 && ABI_SAVE1_LEGACY_CARE==0x3C98);
 migrationTests();compatibilityTests();offspringTests();identityAndHatchTests();hatchTimingTests();statisticalEggTests();daycareUiTests();
}
