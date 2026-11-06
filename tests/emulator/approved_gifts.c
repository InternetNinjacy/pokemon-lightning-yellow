#include "constants/map_event_ids.h"
// Compiled gift identities, real scripted guards, native hatch and save handling.
static void giftProbabilityTests(void){
 daycareFixture(1);
 const unsigned species[]={SPECIES_ARON,SPECIES_MAGBY};
 for(unsigned k=0;k<2;k++){
  unsigned shiny=0;
  for(unsigned i=0;i<10000;i++){
   unsigned ot=0x13572468U+i*0x9E3779B9U;
   for(unsigned b=0;b<4;b++)core->busWrite8(core,careSave2()+ABI_SAVE2_OTID+b,ot>>(8*b));
   core->busWrite32(core,SYM_gRngValue,0xA7020000U+i*7919);
   CALL(CreateHalfShinyGiftEgg,enemy(0),species[k]);
   CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[k]);CHECK(monget(enemy(0),ABI_DATA_OT_ID)==ot);CHECK(monget(enemy(0),ABI_DATA_IS_EGG));
   CHECK(monget(enemy(0),ABI_DATA_UNUSED_RIBBONS)==0);shiny+=!!CALL(IsMonShiny,enemy(0));
  }
  CHECK(shiny>=4700&&shiny<=5300);
  printf("PASS: native species%u actual stored Shiny %u/10000, controlled seeds0xA7020000+i*7919 and varied playerOT; no Route5 tag\n",species[k],shiny);
 }
}
static void waterGiftTests(void){
 daycareFixture(1);fossilWarp(MAP_GROUP(MAP_FIVE_ISLAND_WATER_LABYRINTH),MAP_NUM(MAP_FIVE_ISLAND_WATER_LABYRINTH),10,10);
 monput(player(0),ABI_DATA_FRIENDSHIP,0);fossilStart(SYM_FiveIsland_WaterLabyrinth_EventScript_EggGentleman);fossilClose(1);
 CHECK(!CALL(FlagGet,FLAG_GOT_TOGEPI_EGG));CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==3);
 for(unsigned i=3;i<6;i++){CALL(CreateMon,player(i),SPECIES_PIKACHU,20,31,1,i,0,0);}
 core->busWrite8(core,SYM_gPlayerPartyCount,6);monput(player(0),ABI_DATA_FRIENDSHIP,255);
 fossilStart(SYM_FiveIsland_WaterLabyrinth_EventScript_EggGentleman);fossilClose(1);CHECK(!CALL(FlagGet,FLAG_GOT_TOGEPI_EGG));CHECK(CALL(FlagGet,FLAG_NO_ROOM_FOR_TOGEPI_EGG));
 unsigned seed=core->busRead32(core,SYM_gRngValue);CHECK(CALL(ScriptGiveMagbyEgg)==MON_CANT_GIVE);CHECK(seed==core->busRead32(core,SYM_gRngValue));
 for(unsigned i=3;i<6;i++){clearBytes(player(i),ABI_MON_SIZE);}
 core->busWrite8(core,SYM_gPlayerPartyCount,3);flashRoundTrip();
 fossilStart(SYM_FiveIsland_WaterLabyrinth_EventScript_EggGentleman);fossilClose(1);CHECK(CALL(FlagGet,FLAG_GOT_TOGEPI_EGG));CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==4);
 CHECK(monget(player(3),ABI_DATA_SPECIES)==SPECIES_MAGBY);CHECK(monget(player(3),ABI_DATA_IS_EGG));unsigned pid=monget(player(3),ABI_DATA_PERSONALITY),shiny=CALL(IsMonShiny,player(3));
 flashRoundTrip();fossilStart(SYM_FiveIsland_WaterLabyrinth_EventScript_EggGentleman);fossilClose(1);CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==4);CHECK(monget(player(3),ABI_DATA_PERSONALITY)==pid);
 hatchScene(3);CHECK(monget(player(3),ABI_DATA_SPECIES)==SPECIES_MAGBY);CHECK(!monget(player(3),ABI_DATA_IS_EGG));CHECK(CALL(IsMonShiny,player(3))==shiny);flashRoundTrip();CHECK(CALL(IsMonShiny,player(3))==shiny);
 printf("PASS: actual Water Labyrinth low friendship/full party/deferred claim/one-time flags, saved Magby egg identity, native hatch/nickname refusal and Shiny persistence\n");
}
static void tangoGiftTests(void){
 for(unsigned pc=0;pc<2;pc++){
  vendorFixture();fossilWarp(MAP_GROUP(MAP_CELADON_CITY_CONDOMINIUMS_ROOF_ROOM),MAP_NUM(MAP_CELADON_CITY_CONDOMINIUMS_ROOF_ROOM),7,5);
  if(pc){vendorFillCapacity();clearBytes(CALL(GetBoxedMonPtr,0,0),ABI_BOX_SIZE);}
  fossilStart(SYM_CeladonCity_Condominiums_RoofRoom_EventScript_EeveeBall);fossilClose(1);CHECK(CALL(FlagGet,FLAG_GOT_EEVEE));
  uint32_t mon=pc?enemy(0):player(3);if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),mon);
  CHECK(monget(mon,ABI_DATA_SPECIES)==SPECIES_TANGELA);CHECK(monget(mon,ABI_DATA_LEVEL)==25);CALL(GetMonData3,mon,ABI_DATA_NICKNAME,scratch);textEquals(scratch,"TANGO");
  unsigned pid=monget(mon,ABI_DATA_PERSONALITY);flashRoundTrip();if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),mon);
  CHECK(monget(mon,ABI_DATA_PERSONALITY)==pid);CALL(GetMonData3,mon,ABI_DATA_NICKNAME,scratch);textEquals(scratch,"TANGO");CHECK(CALL(FlagGet,FLAG_GOT_EEVEE));CHECK(CALL(FlagGet,FLAG_HIDE_EEVEE_BALL));
  fossilWarp(MAP_GROUP(MAP_CELADON_CITY_CONDOMINIUMS_ROOF_ROOM),MAP_NUM(MAP_CELADON_CITY_CONDOMINIUMS_ROOF_ROOM),7,5);
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,LOCALID_EEVEE_POKEBALL,MAP_NUM(MAP_CELADON_CITY_CONDOMINIUMS_ROOF_ROOM),MAP_GROUP(MAP_CELADON_CITY_CONDOMINIUMS_ROOF_ROOM))>=ABI_OBJECT_COUNT);
 }
 vendorFixture();vendorFillCapacity();CHECK(CALL(ScriptGiveTangoTangela)==MON_CANT_GIVE);CHECK(!CALL(FlagGet,FLAG_GOT_EEVEE));
 fossilWarp(MAP_GROUP(MAP_CELADON_CITY_CONDOMINIUMS_ROOF_ROOM),MAP_NUM(MAP_CELADON_CITY_CONDOMINIUMS_ROOF_ROOM),7,5);
 fossilStart(SYM_CeladonCity_Condominiums_RoofRoom_EventScript_EeveeBall);fossilClose(1);CHECK(!CALL(FlagGet,FLAG_GOT_EEVEE));CHECK(!CALL(FlagGet,FLAG_HIDE_EEVEE_BALL));
 CHECK(CALL(GetObjectEventIdByLocalIdAndMap,LOCALID_EEVEE_POKEBALL,MAP_NUM(MAP_CELADON_CITY_CONDOMINIUMS_ROOF_ROOM),MAP_GROUP(MAP_CELADON_CITY_CONDOMINIUMS_ROOF_ROOM))<ABI_OBJECT_COUNT);
 printf("PASS: actual rooftop TangelaLv25 TANGO party/PC script delivery and flash identity; full capacity helper fails without award flag\n");
}
static void approvedGiftTests(void){giftProbabilityTests();waterGiftTests();tangoGiftTests();CHECK(core->loadState(core,bootState));}
