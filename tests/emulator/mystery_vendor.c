#include "constants/mystery_vendor.h"
static const unsigned vendorSpecies[]={SPECIES_BULBASAUR,SPECIES_SQUIRTLE,SPECIES_CHARMANDER,SPECIES_KANGASKHAN,SPECIES_PINSIR,SPECIES_FEEBAS,SPECIES_CHANSEY,SPECIES_DRATINI,SPECIES_ABSOL,SPECIES_MAGIKARP};
static unsigned vendorIndex(unsigned species) {
 for(unsigned i=0;i<10;i++)if(vendorSpecies[i]==species)return i;
 CHECK(0);return 10;
}
static void vendorFixture(void) {
 daycareFixture(1);CALL(ResetPokemonStorageSystem);
 CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,70000);
 core->busWrite16(core,SYM_gSpecialVar_0x8009,0);
}
static void vendorResetFlags(void) {
 CALL(FlagClear,FLAG_BOUGHT_MAGIKARP);
 for(unsigned i=0;i<19;i++)CALL(FlagClear,0x300+i);
}
static void vendorIdentity(unsigned result,unsigned before,unsigned *species,unsigned *shiny) {
 unsigned ot,pid;
 if(result==0) {
  uint32_t mon=player(before);*species=monget(mon,ABI_DATA_SPECIES);
  CHECK(monget(mon,ABI_DATA_LEVEL)==5);CHECK(monget(mon,ABI_DATA_HELD_ITEM)==0);
  CHECK(CALL(IsTradedMon,mon));ot=monget(mon,ABI_DATA_OT_ID);pid=monget(mon,ABI_DATA_PERSONALITY);
  CALL(GetMonData3,mon,ABI_DATA_OT_NAME,scratch);
  *shiny=CALL(IsMonShiny,mon);
 } else {
  CHECK(result==1);unsigned box=core->busRead16(core,SYM_gSpecialVar_MonBoxId),slot=core->busRead16(core,SYM_gSpecialVar_MonBoxPos);
  *species=CALL(GetBoxMonDataAt,box,slot,ABI_DATA_SPECIES);
  CHECK(CALL(GetBoxMonDataAt,box,slot,ABI_DATA_HELD_ITEM)==0);
  ot=CALL(GetBoxMonDataAt,box,slot,ABI_DATA_OT_ID);pid=CALL(GetBoxMonDataAt,box,slot,ABI_DATA_PERSONALITY);
  CALL(GetAndCopyBoxMonDataAt,box,slot,ABI_DATA_OT_NAME,scratch);
  CALL(BoxMonToMon,CALL(GetBoxedMonPtr,box,slot),enemy(0));CHECK(CALL(IsTradedMon,enemy(0)));
  *shiny=CALL(IsMonShiny,enemy(0));CHECK(*shiny==(((ot>>16)^(ot&65535)^(pid>>16)^(pid&65535))<8));
 }
 CHECK(ot==~0x78563412U);
 const unsigned char name[]={0xC7,0xED,0xE7,0xE8,0xD9,0xE6,0xED,0xFF};
 sameBytes(scratch,name,sizeof(name));
 if(!CALL(IsSpeciesInCustomPokedex,*species))fprintf(stderr,"Vendor species=%u result=%u ot=%08x pid=%08x\n",*species,result,ot,pid);
 CHECK(CALL(IsSpeciesInCustomPokedex,*species));
 CHECK(CALL(GetSetPokedexFlag,CALL(SpeciesToNationalPokedexNum,*species),ABI_FLAG_GET_CAUGHT));
}
static void vendorFillCapacity(void) {
 for(unsigned i=0;i<6;i++)CALL(CreateMon,player(i),SPECIES_PIKACHU,20,31,1,i,0,0);
 core->busWrite8(core,SYM_gPlayerPartyCount,6);
 for(unsigned box=0;box<14;box++)for(unsigned pos=0;pos<30;pos++)copyBytes(CALL(GetBoxedMonPtr,box,pos),player(0),ABI_BOX_SIZE);
}
static void vendorCapacityTests(void) {
 vendorFixture();
 vendorFillCapacity();
 CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,700);
 unsigned seed=core->busRead32(core,SYM_gRngValue);
 CHECK(CALL(PurchaseMysteryPokemon)==2);CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==700);
 CHECK(core->busRead32(core,SYM_gRngValue)==seed);CHECK(!CALL(MysteryVendorStatus));
 CALL(ZeroBoxMonAt,3,7);CHECK(CALL(PurchaseMysteryPokemon)==1);
 CHECK(core->busRead16(core,SYM_gSpecialVar_MonBoxId)==3 && core->busRead16(core,SYM_gSpecialVar_MonBoxPos)==7);
 unsigned species,shiny;vendorIdentity(1,6,&species,&shiny);
 CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==0);
 printf("PASS: full party plus420 occupied PC slots preserves money/flags/RNG; freeing box3 slot7 delivers there exactly once\n");
}
#include "constants/battle_script_commands.h"
#include "constants/battle_string_ids.h"
static unsigned vendorExp(unsigned slot,unsigned value) {
 uint32_t bs=core->busRead32(core,SYM_gBattleStruct);
 core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPT_EXP_STATE,2);
 core->busWrite8(core,bs+ABI_BATTLESTRUCT_EXP_GETTER,slot);
 core->busWrite8(core,bs+ABI_BATTLESTRUCT_SENT_IN,1);
 core->busWrite8(core,bs+ABI_BATTLESTRUCT_WILD_SONG,1);
 core->busWrite16(core,bs+ABI_BATTLESTRUCT_EXP_VALUE,value);
 core->busWrite32(core,SYM_gBattleControllerExecFlags,0);
 core->busWrite8(core,scratch,0);core->busWrite8(core,scratch+1,BS_OPPONENT1);
 core->busWrite32(core,SYM_gBattlescriptCurrInstr,scratch);
 CALL(Cmd_getexp);
 return core->busRead32(core,SYM_gBattleMoveDamage);
}
static void vendorBattleTests(void) {
 wildFixture(MAP_GROUP(MAP_MT_MOON_1F),MAP_NUM(MAP_MT_MOON_1F),1);
 CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,700);
 core->busWrite16(core,SYM_gSpecialVar_0x8009,0);
 CHECK(CALL(PurchaseMysteryPokemon)==0);
 unsigned char purchased[ABI_MON_SIZE];readBytes(player(2),purchased,sizeof(purchased));
 startSpecial();waitAction();
 // Compare the actual engine's EXP calculation/text path for an ordinary
 // recipient and the delivered foreign recipient, including a switched slot.
 unsigned a=vendorExp(0,100);CHECK(a==100);
 copyBytes(player(1),player(2),ABI_MON_SIZE);
 unsigned b=vendorExp(1,100);CHECK(b==150);
 CHECK(core->busRead16(core,SYM_gBattleTextBuff2+2)==STRINGID_ABOOSTED);
 CALL(BufferStringBattle,STRINGID_PKMNGAINEDEXP);
 unsigned found=0;
 for(unsigned i=0;i<240;i++) {
  const char *word="boosted";unsigned k;
  for(k=0;word[k] && core->busRead8(core,SYM_gDisplayedStringBattle+i+k)==encoded(word[k]);k++);
  if(!word[k]){found=1;break;}
 }
 CHECK(found);
 core->busWrite32(core,SYM_gBattleTypeFlags,BATTLE_TYPE_TRAINER);
 CHECK(vendorExp(0,100)==150);CHECK(vendorExp(1,100)==225);
 for(unsigned i=0;i<ABI_MON_SIZE;i++)core->busWrite8(core,player(0)+i,purchased[i]);
 CALL(CopyPlayerPartyMonToBattleData,0,0);
 core->busWrite8(core,SYM_gBattlerPartyIndexes,0);
 core->busWrite8(core,SYM_gBattlerAttacker,0);core->busWrite8(core,SYM_gBattlerTarget,1);
 core->busWrite16(core,SYM_gCurrentMove,MOVE_POUND);core->busWrite8(core,SYM_gCurrMovePos,0);
 core->busWrite32(core,SYM_gBattleTypeFlags,0);
 for(unsigned i=0;i<4;i++){core->busWrite16(core,bmon(0)+ABI_BMON_MOVES+2*i,i?0:MOVE_POUND);core->busWrite8(core,bmon(0)+ABI_BMON_PP+i,i?0:35);}
 const unsigned flags[]={0,FLAG_BADGE02_GET,FLAG_BADGE04_GET,FLAG_BADGE06_GET,FLAG_BADGE08_GET};
 const unsigned bounds[]={10,30,50,70,100};
 for(unsigned stage=0;stage<5;stage++) {
  if(flags[stage])CALL(FlagSet,flags[stage]);
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,bounds[stage]);
  for(unsigned n=0;n<32;n++)CHECK(CALL(IsMonDisobedient)==0);
  if(stage<4) {
   core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,bounds[stage]+1);
   unsigned disobey=0;
   for(unsigned n=0;n<128;n++)disobey+=CALL(IsMonDisobedient)!=0;
   CHECK(disobey>0);
  }
 }
 for(unsigned stage=1;stage<5;stage++)CALL(FlagClear,flags[stage]);
 CALL(CreateMon,player(0),SPECIES_PIKACHU,99,31,1,123,0,0);CALL(CopyPlayerPartyMonToBattleData,0,0);
 for(unsigned n=0;n<128;n++)CHECK(CALL(IsMonDisobedient)==0);
 printf("PASS: actual Cmd_getexp100/150 including switched recipient, boosted engine text, native badge10/30/50/70/all obedience and own level99 control\n");
}
static void vendorOrderAndReloadTests(void) {
 vendorFixture();
 for(unsigned order=0;order<10;order++) {
  vendorResetFlags();CALL(ResetPokemonStorageSystem);CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,7000);
  for(unsigned i=1;i<6;i++)clearBytes(player(i),ABI_MON_SIZE);
  core->busWrite8(core,SYM_gPlayerPartyCount,1);
  unsigned seen=0;
  unsigned permutation[10],shuffle=0x1AD73093U+order;
  for(unsigned i=0;i<10;i++)permutation[i]=i;
  for(unsigned i=9;i>0;i--){shuffle=1664525U*shuffle+1013904223U;unsigned j=shuffle%(i+1),tmp=permutation[i];permutation[i]=permutation[j];permutation[j]=tmp;}
  // Seeded Fisher-Yates fixture orders, separate from the game RNG algorithm.
  for(unsigned n=0;n<10;n++) {
   unsigned v=permutation[n];
   core->busWrite16(core,SYM_gSpecialVar_0x8009,v);
   unsigned before=core->busRead8(core,SYM_gPlayerPartyCount),species,shiny;
   unsigned r=CALL(PurchaseMysteryPokemon);vendorIdentity(r,before,&species,&shiny);
   unsigned bit=1U<<vendorIndex(species);CHECK(!(seen&bit));seen|=bit;
  }
  CHECK(seen==1023);CHECK(!CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY));
 }
 vendorFixture();CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,700);CHECK(CALL(TrySavingData,0)==1);
 CHECK(CALL(PurchaseMysteryPokemon)==0);unsigned first=monget(player(3),ABI_DATA_PERSONALITY);
 clearBytes(careSave1(),ABI_SAVE1_SIZE);clearBytes(careSave2(),ABI_SAVE2_SIZE);
 clearBytes(SYM_gPlayerParty,6*ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,0);
 CHECK(CALL(LoadGameSave,0)==1);
 CHECK(!CALL(MysteryVendorStatus));CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==700);
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==3);
 core->busWrite32(core,SYM_gRngValue,0x90AD17E3);CHECK(CALL(PurchaseMysteryPokemon)==0);
 CHECK(monget(player(3),ABI_DATA_PERSONALITY)!=first);
 printf("PASS: ten varied seller orders yield exact pool; pre-sale flash reload restores unsold/money/party and allows a fresh identity\n");
}
static void vendorTests(void) {
 vendorCapacityTests();vendorBattleTests();vendorOrderAndReloadTests();
 vendorFixture();
 const unsigned char playerName[]={0xC7,0xED,0xE7,0xE8,0xD9,0xE6,0xED,0xFF};
 for(unsigned i=0;i<8;i++)core->busWrite8(core,careSave2()+ABI_SAVE2_PLAYER_NAME+i,playerName[i]);
 unsigned seed=core->busRead32(core,SYM_gRngValue);
 CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,699);
 CHECK(CALL(PurchaseMysteryPokemon)==MYSTERY_VENDOR_POOR);
 CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==699);CHECK(core->busRead32(core,SYM_gRngValue)==seed);
 CHECK(!CALL(MysteryVendorStatus));
 CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,7000);
 for(unsigned i=0;i<10;i++){unsigned d=CALL(SpeciesToNationalPokedexNum,vendorSpecies[i]);CALL(GetSetPokedexFlag,d,ABI_FLAG_SET_SEEN);CALL(GetSetPokedexFlag,d,ABI_FLAG_SET_CAUGHT);}
 unsigned seen=0;
 for(unsigned v=0;v<10;v++) {
  core->busWrite16(core,SYM_gSpecialVar_0x8009,v);
  unsigned before=core->busRead8(core,SYM_gPlayerPartyCount),species,shiny;
  unsigned r=CALL(PurchaseMysteryPokemon);vendorIdentity(r,before,&species,&shiny);
  unsigned bit=1U<<vendorIndex(species);CHECK(!(seen&bit));seen|=bit;
  CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==7000-700*(v+1));
  CHECK(CALL(MysteryVendorStatus)==MYSTERY_VENDOR_SOLD);
  seed=core->busRead32(core,SYM_gRngValue);
  CHECK(CALL(PurchaseMysteryPokemon)==MYSTERY_VENDOR_SOLD);CHECK(core->busRead32(core,SYM_gRngValue)==seed);
  flashRoundTrip();CHECK(CALL(MysteryVendorStatus)==MYSTERY_VENDOR_SOLD);
 }
 CHECK(seen==1023);
 printf("PASS: native ten-vendor pool depleted exactly once; party and PC foreign Mystery identity, money, Dex flags and actual flash persistence\n");
 vendorFixture();CALL(FlagSet,FLAG_BOUGHT_MAGIKARP);
 CHECK(CALL(MysteryVendorStatus)==MYSTERY_VENDOR_SOLD);CHECK(CALL(FlagGet,FLAG_MYSTERY_SPECIES_09));
 core->busWrite16(core,SYM_gSpecialVar_0x8009,1);CHECK(!CALL(MysteryVendorStatus));
 printf("PASS: historical fixed Magikarp purchase migrates without reselling vendor or species\n");
 vendorFixture();unsigned hits[10]={0},shinyHits[10]={0},shinies=0;
 for(unsigned i=0;i<10000;i++) {
  vendorResetFlags();CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,700);
  for(unsigned slot=1;slot<6;slot++)clearBytes(player(slot),ABI_MON_SIZE);
  core->busWrite8(core,SYM_gPlayerPartyCount,1);
  unsigned x=0x705A9D13U+i;x=(x^(x>>16))*0x7FEB352DU;x=(x^(x>>15))*0x846CA68BU;x^=x>>16;
  core->busWrite32(core,SYM_gRngValue,x);
  CHECK(CALL(PurchaseMysteryPokemon)==0);unsigned species,shiny;vendorIdentity(0,1,&species,&shiny);
  hits[vendorIndex(species)]++;shinyHits[vendorIndex(species)]+=shiny;shinies+=shiny;
 }
 CHECK(shinies>=8800 && shinies<=9200);
 double chi=0;for(unsigned i=0;i<10;i++){double d=hits[i]-1000.0;chi+=d*d/1000.0;}
 CHECK(chi<40);
 double joint=0;for(unsigned i=0;i<10;i++){double a=shinyHits[i]-900.0,b=hits[i]-shinyHits[i]-100.0;joint+=a*a/900.0+b*b/100.0;}
 CHECK(joint<60);
 printf("PASS:10000 actual native purchases shiny=%u, species chi-square=%.3f, species/shiny joint chi-square=%.3f; seed=mix32(0x705A9D13+i)\n",shinies,chi,joint);
}
