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
static void vendorTests(void) {
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
