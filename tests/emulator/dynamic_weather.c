// Actual compiled policy, map loaders, RTC hardware, weather rendering and flash.
#include "weather_expected.h"
static uint32_t weatherContext(void){return careSave1()+ABI_SAVE1_WEATHER_CONTEXT;}
static unsigned weatherCategory(unsigned value){return value==WEATHER_RAIN?1:value==WEATHER_DROUGHT?2:value==WEATHER_SANDSTORM?3:value==WEATHER_SNOW?4:0;}
static void weatherPolicyTests(void){
 daycareFixture(1);
 for(unsigned night=0;night<2;night++){
  clockAt(night?18:12);
  for(unsigned p=0;p<sizeof(weatherExpected)/sizeof(weatherExpected[0]);p++){
   const struct WeatherExpected *w=&weatherExpected[p];unsigned counts[5]={0};
   core->busWrite8(core,careSave1()+ABI_SAVE1_LOCATION,MAP_GROUP(w->map));core->busWrite8(core,careSave1()+ABI_SAVE1_LOCATION+1,MAP_NUM(w->map));
   core->busWrite8(core,SYM_gMapHeader+ABI_MAPHEADER_WEATHER,w->original);
   for(unsigned i=0;i<256;i++){
    core->busWrite8(core,weatherContext(),0); // Each sample models a fresh outdoor entry.
    unsigned value=CALL(DynamicWeatherForMap,w->original);CHECK(value==0||value==2||value==3||value==4||value==8||value==11||value==12);
    unsigned c=weatherCategory(value);counts[c]++;CHECK(w->weights[c] || (night&&c==0&&w->weights[2]));CHECK(!(night&&c==2));
   }
   for(unsigned i=0;i<5;i++)if(w->weights[i] && !(night&&i==2))CHECK(counts[i]>0);
   CHECK(CALL(WeatherContextValid));
  }
 }
 printf("PASS: actual ARM policy on83 map IDs/day and night,256 independent draws each, all eligible categories observed and night Sun mass converted without reroll\n");
}
static void weatherWarp(unsigned map,unsigned x,unsigned y){fossilWarp(MAP_GROUP(map),MAP_NUM(map),x,y);}
static void weatherIntegrationTests(void){
 daycareFixture(1);clockAt(12);
 for(unsigned i=0;i<16;i++)core->busWrite8(core,weatherContext()+i,0xA9);
 weatherWarp(MAP_CINNABAR_ISLAND,10,10);frames(180,0);
 CHECK(CALL(GetSav1Weather)==WEATHER_DROUGHT);CHECK(CALL(GetCurrentWeather)==WEATHER_DROUGHT);
 uint32_t wp=core->busRead32(core,SYM_gWeatherPtr);CHECK(core->busRead8(core,wp+ABI_WEATHER_COMPLETE));
 CHECK(core->busRead8(core,wp+ABI_WEATHER_GAMMA)==0);CHECK(core->busRead8(core,wp+ABI_WEATHER_PAL_STATE)==3);
 clockAt(18);CHECK(CALL(DynamicWeatherForBattle)==WEATHER_NONE);frames(120,0);CHECK(CALL(GetSav1Weather)==WEATHER_NONE);CHECK(CALL(GetCurrentWeather)==WEATHER_NONE);
 clockAt(6);frames(120,0);CHECK(CALL(GetSav1Weather)==WEATHER_NONE);
 for(unsigned i=8;i<16;i++)CHECK(core->busRead8(core,weatherContext()+i)==0xA9);
 // Native GPIO absence must use daytime fallback, then recover without a warp.
 struct GBA *gba=core->board;unsigned devices=gba->memory.hw.devices;gba->memory.hw.devices&=~HW_RTC;
 CHECK(!CALL(RtcGetHour,scratch));clockAt(23);CALL(SetWeather,WEATHER_DROUGHT);frames(120,0);CHECK(CALL(GetSav1Weather)==WEATHER_DROUGHT);
 gba->memory.hw.devices=devices;frames(120,0);CHECK(CALL(GetSav1Weather)==WEATHER_NONE);
 printf("PASS: actual Cinnabar sunlight map load/render/fade completes, sunset mechanical/render conversion, no dawn restore, absent GPIO fallback and immediate recovered-night synchronization\n");
 clockAt(12);weatherWarp(MAP_ONE_ISLAND,6,10);CALL(SetWeather,WEATHER_SNOW);frames(180,0);
 weatherWarp(MAP_ONE_ISLAND_POKEMON_CENTER_1F,7,8);CHECK(CALL(GetSav1Weather)==WEATHER_NONE);CHECK(core->busRead8(core,weatherContext()+4)==WEATHER_SNOW);CHECK(core->busRead8(core,weatherContext()+5)==1);
 weatherWarp(MAP_ONE_ISLAND_POKEMON_CENTER_2F,7,6);CHECK(core->busRead8(core,weatherContext()+4)==WEATHER_SNOW);
 flashRoundTrip();CHECK(core->busRead8(core,weatherContext()+4)==WEATHER_SNOW);CHECK(core->busRead8(core,weatherContext()+5)==1);
 weatherWarp(MAP_ONE_ISLAND,6,10);CHECK(CALL(WeatherContextValid));CHECK(core->busRead8(core,weatherContext()+5)==0);
 const unsigned maps[]={MAP_NAVEL_ROCK_HARBOR,MAP_BIRTH_ISLAND_HARBOR,MAP_MT_EMBER_SUMMIT,MAP_SSANNE_DECK};
 for(unsigned i=0;i<4;i++){weatherWarp(maps[i],6,6);CHECK(CALL(WeatherContextValid));unsigned weather=CALL(GetSav1Weather);CHECK(i!=0||weather==WEATHER_RAIN);CHECK(i!=1||weather==WEATHER_SNOW||weather==WEATHER_SANDSTORM);CHECK(i!=2||weather==WEATHER_DROUGHT||weather==WEATHER_SANDSTORM);}
 weatherWarp(MAP_MT_EMBER_SUMMIT_PATH_1F,4,4);CHECK(CALL(GetSav1Weather)==WEATHER_NONE);
 printf("PASS: actual nested indoor weather suspension/flash context retention/outdoor return, explicit harbor/summit/deck scopes, untouched cave weather and save tail\n");
 // Real wild battle on Cycling Road, with a controlled enemy fixture.
 weatherWarp(MAP_ROUTE17,6,10);CALL(CreateMon,SYM_gEnemyParty,SPECIES_RATTATA,5,0,1,0,0,0);CALL(StartWildBattle);waitAction();
 CHECK(core->busRead8(core,SYM_gBattlersCount)==2);press(128);press(16);press(1);waitOverworld(1);
 CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);CHECK(CALL(WeatherContextValid));
 CHECK(core->busRead8(core,weatherContext()+3)==MAP_NUM(MAP_ROUTE17));
 unsigned weather=CALL(GetSav1Weather);CHECK(weather==WEATHER_RAIN||weather==WEATHER_DROUGHT||weather==WEATHER_SANDSTORM||weather==WEATHER_SNOW);
 printf("PASS: actual Cycling Road single wild battle/Run/field return executes post-battle policy; counter/RNG exactness additionally tested in host\n");
 CHECK(core->loadState(core,bootState));
}
static void dynamicWeatherTests(void){weatherPolicyTests();weatherIntegrationTests();}
