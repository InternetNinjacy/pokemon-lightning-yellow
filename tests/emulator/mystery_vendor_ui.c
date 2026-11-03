// Native shared-script UI cases on every original map/object.
struct VendorSite {unsigned group,map,local,x,y;uint32_t entry;};
static const struct VendorSite vendorSites[]={
 {MAP_GROUP(MAP_ROUTE4_POKEMON_CENTER_1F),MAP_NUM(MAP_ROUTE4_POKEMON_CENTER_1F),2,1,3,SYM_Route4_PokemonCenter_1F_EventScript_MagikarpSalesman},
 {MAP_GROUP(MAP_CERULEAN_CITY_HOUSE1),MAP_NUM(MAP_CERULEAN_CITY_HOUSE1),1,6,5,SYM_CeruleanCity_House1_EventScript_BadgeGuy},
 {MAP_GROUP(MAP_VERMILION_CITY),MAP_NUM(MAP_VERMILION_CITY),1,22,11,SYM_VermilionCity_EventScript_Woman},
 {MAP_GROUP(MAP_ROUTE10_POKEMON_CENTER_1F),MAP_NUM(MAP_ROUTE10_POKEMON_CENTER_1F),3,2,3,SYM_Route10_PokemonCenter_1F_EventScript_FatMan},
 {MAP_GROUP(MAP_LAVENDER_TOWN),MAP_NUM(MAP_LAVENDER_TOWN),2,12,12,SYM_LavenderTown_EventScript_WorkerM},
 {MAP_GROUP(MAP_CELADON_CITY),MAP_NUM(MAP_CELADON_CITY),6,30,24,SYM_CeladonCity_EventScript_OldMan2},
 {MAP_GROUP(MAP_FUCHSIA_CITY),MAP_NUM(MAP_FUCHSIA_CITY),8,36,20,SYM_FuchsiaCity_EventScript_OldMan},
 {MAP_GROUP(MAP_SAFFRON_CITY),MAP_NUM(MAP_SAFFRON_CITY),10,21,23,SYM_SaffronCity_EventScript_WorkerM},
 {MAP_GROUP(MAP_CINNABAR_ISLAND),MAP_NUM(MAP_CINNABAR_ISLAND),2,11,11,SYM_CinnabarIsland_EventScript_OldMan},
 {MAP_GROUP(MAP_ONE_ISLAND),MAP_NUM(MAP_ONE_ISLAND),3,13,9,SYM_OneIsland_EventScript_BaldingMan},
};
static const uint32_t vendorIntros[]={SYM_MysteryVendor_Text_Intro01,SYM_MysteryVendor_Text_Intro02,SYM_MysteryVendor_Text_Intro03,SYM_MysteryVendor_Text_Intro04,SYM_MysteryVendor_Text_Intro05,SYM_MysteryVendor_Text_Intro06,SYM_MysteryVendor_Text_Intro07,SYM_MysteryVendor_Text_Intro08,SYM_MysteryVendor_Text_Intro09,SYM_MysteryVendor_Text_Intro10};
static void vendorTextMatches(uint32_t source) {
 unsigned i;
 for(i=0;i<1024;i++) {
  unsigned expected=core->busRead8(core,source+i),actual=core->busRead8(core,SYM_gStringVar4+i);
  if(actual!=expected)fprintf(stderr,"Vendor text mismatch at%u actual%02x expected%02x\n",i,actual,expected);
  CHECK(actual==expected);if(expected==255)break;
 }
 CHECK(i<1024);
}
static void vendorMap(unsigned v) {
 vendorFixture();const struct VendorSite *s=&vendorSites[v];
 if(v==7) {
  CALL(FlagSet,FLAG_HIDE_SAFFRON_CIVILIANS);
  fossilWarp(s->group,s->map,s->x,s->y+1);
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,s->local,s->map,s->group)>=ABI_OBJECT_COUNT);
  CALL(FlagClear,FLAG_HIDE_SAFFRON_CIVILIANS);
 }
 fossilWarp(s->group,s->map,s->x,s->y+1);
 unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,s->local,s->map,s->group);
 CHECK(object<ABI_OBJECT_COUNT);core->busWrite8(core,SYM_gSelectedObjectEvent,object);
}
static void vendorPrompt(unsigned v) {
 fossilStart(vendorSites[v].entry);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
 CHECK(core->busRead16(core,SYM_gSpecialVar_0x8009)==v);vendorTextMatches(vendorIntros[v]);
}
static void vendorMoneyClosed(void) {
 unsigned window=core->busRead8(core,SYM_sMoneyBoxWindowId);
 CHECK(window<32);
 CHECK(core->busRead8(core,SYM_gWindows+window*ABI_WINDOW_SIZE+ABI_WINDOW_BG)==255);
}
static void vendorMessageContains(uint32_t text) {
 unsigned length=0;while(length<32 && core->busRead8(core,text+length)!=255)length++;
 CHECK(length>0 && length<32);unsigned found=0;
 for(unsigned at=0;at<1024-length;at++) {
  unsigned i;for(i=0;i<length && core->busRead8(core,SYM_gStringVar4+at+i)==core->busRead8(core,text+i);i++);
  if(i==length){found=1;break;}
 }
 CHECK(found);
}
static void vendorRename(void) {
 unsigned f;
 for(f=0;f<900;f++) {
  frames(1,0);
  if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_NamingScreen)break;
 }
 CHECK(f<900);frames(120,0);
 for(unsigned i=0;i<12;i++)press(2); // erase the old species name, without touching party data
 press(1);press(8);frames(30,0);press(1); // A, Start to OK, A confirms
 fossilClose(1);vendorMoneyClosed();
}
static void vendorUiTests(void) {
 for(unsigned v=0;v<10;v++) {
  vendorMap(v);unsigned mapTemp=CALL(VarGet,VAR_TEMP_1);CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,700);
  vendorPrompt(v);press(2);fossilClose(1);vendorMoneyClosed();vendorTextMatches(SYM_MysteryVendor_Text_Declined);
  CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==700);CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==3);CHECK(!CALL(MysteryVendorStatus));
  CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,699);vendorPrompt(v);press(1);fossilClose(1);vendorMoneyClosed();vendorTextMatches(SYM_MysteryVendor_Text_Poor);
  CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==699);CHECK(!CALL(MysteryVendorStatus));
  CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,700);vendorPrompt(v);press(1);
  fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput); // actual post-delivery nickname choice
  CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==0);CHECK(CALL(MysteryVendorStatus)==MYSTERY_VENDOR_SOLD);
  CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==4);
  if(v==0) {
   press(1);vendorRename();CALL(GetMonData3,player(3),ABI_DATA_NICKNAME,testStack);
   CHECK(core->busRead8(core,testStack)==0xBB && core->busRead8(core,testStack+1)==255);
  } else {press(2);fossilClose(1);vendorMoneyClosed();}
  fossilStart(vendorSites[v].entry);fossilClose(1);vendorTextMatches(SYM_MysteryVendor_Text_AlreadySold);
  CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==0);CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==4);
  CHECK(CALL(VarGet,VAR_TEMP_1)==mapTemp);
  printf("PASS: actual vendor%u map/object, unique intro, No/699/700, post-commit nickname prompt, repeat and money-window cleanup\n",v+1);
 }
 // PC nickname callbacks must update the actual delivered box, not an unrelated
 // party slot; use outdoor Cinnabar to exercise return-to-map integration too.
 for(unsigned key=0;key<2;key++) {
 vendorMap(8);if(key)CALL(FlagSet,FLAG_HIDE_POKEMON_MANSION_B1F_SECRET_KEY);
 CALL(VarSet,VAR_TEMP_1,key);
 for(unsigned i=0;i<6;i++)CALL(CreateMon,player(i),SPECIES_PIKACHU,20,31,1,i,0,0);
 core->busWrite8(core,SYM_gPlayerPartyCount,6);CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,700);
 vendorPrompt(8);press(1);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
 unsigned box=core->busRead16(core,SYM_gSpecialVar_MonBoxId),pos=core->busRead16(core,SYM_gSpecialVar_MonBoxPos);
 unsigned species=CALL(GetBoxMonDataAt,box,pos,ABI_DATA_SPECIES);CHECK(CALL(IsSpeciesInCustomPokedex,species));
 press(1);vendorRename();CALL(GetAndCopyBoxMonDataAt,box,pos,ABI_DATA_NICKNAME,testStack);
 CHECK(core->busRead8(core,testStack)==0xBB && core->busRead8(core,testStack+1)==255);
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==6);CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==0);
 CHECK(CALL(GetBoxMonDataAt,box,pos,ABI_DATA_SPECIES)==species);
  vendorMessageContains(SYM_gSpeciesNames+species*(POKEMON_NAME_LENGTH+1));
  vendorMessageContains(CALL(GetBoxNamePtr,box));
 flashRoundTrip();CALL(GetAndCopyBoxMonDataAt,box,pos,ABI_DATA_NICKNAME,testStack);CHECK(core->busRead8(core,testStack)==0xBB);
 CHECK(CALL(VarGet,VAR_TEMP_1)==key);
 }
 unsigned box,pos,species;
  vendorMap(0);vendorFillCapacity();CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,700);
  vendorPrompt(0);press(1);fossilClose(1);vendorMoneyClosed();vendorTextMatches(SYM_MysteryVendor_Text_NoRoom);
  CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==700);CHECK(!CALL(MysteryVendorStatus));
  CALL(ZeroBoxMonAt,3,7);vendorPrompt(0);press(1);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
  box=core->busRead16(core,SYM_gSpecialVar_MonBoxId);pos=core->busRead16(core,SYM_gSpecialVar_MonBoxPos);
  CHECK(box==3 && pos==7);species=CALL(GetBoxMonDataAt,box,pos,ABI_DATA_SPECIES);
  press(2);fossilClose(1);vendorMoneyClosed();
  vendorMessageContains(SYM_gSpeciesNames+species*(POKEMON_NAME_LENGTH+1));vendorMessageContains(CALL(GetBoxNamePtr,box));
  CALL(GetAndCopyBoxMonDataAt,box,pos,ABI_DATA_NICKNAME,testStack);
  for(unsigned i=0;i<POKEMON_NAME_LENGTH+1;i++){unsigned c=core->busRead8(core,SYM_gSpeciesNames+species*(POKEMON_NAME_LENGTH+1)+i);CHECK(core->busRead8(core,testStack+i)==c);if(c==255)break;}
  CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==0);
 printf("PASS: actual party and outdoor-PC nickname keyboard A, correct delivered record, no extra charge/award, flash persistence; Saffron visibility preserved\n");
}
