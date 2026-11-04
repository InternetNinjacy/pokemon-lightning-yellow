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
static void weatherRun(void){
 waitAction();unsigned cursor=core->busRead8(core,SYM_gActionSelectionCursor);if(!(cursor&2))press(128);cursor=core->busRead8(core,SYM_gActionSelectionCursor);if(!(cursor&1))press(16);CHECK(core->busRead8(core,SYM_gActionSelectionCursor)==3);press(1);
}
static void weatherWaitEndCallback(uint32_t callback){
 unsigned f;for(f=0;f<30000 && !core->busRead8(core,SYM_gBattleOutcome);f++){
  unsigned key=0;if(f%8==0){key=2;if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_BattleMainCB2){unsigned fn=controller(0);if(fn==SYM_HandleInputChooseAction){unsigned c=core->busRead8(core,SYM_gActionSelectionCursor);key=c&2?64:c&1?32:1;}if(fn==SYM_HandleInputChooseMove){unsigned c=core->busRead8(core,SYM_gMoveSelectionCursor);key=c&2?64:c&1?32:1;}}}frames(1,key);
 }CHECK(f<30000);
 // Observe the actual callback at instruction granularity; a frame can span it.
 struct ARMCore *cpu=core->cpu;unsigned last=core->frameCounter(core),steps;
 for(steps=0;steps<60000000;steps++){
  unsigned pc=cpu->gprs[15];if(pc>=callback && pc<callback+6)break;
  if((steps&63)==0){unsigned now=core->frameCounter(core);if(now!=last){last=now;core->setKeys(core,now%12<3?2:0);}}
  core->step(core);
 }CHECK(steps<60000000);uint32_t returnAddress=cpu->gprs[14]&~1U;
 uint32_t seed=core->busRead32(core,SYM_gRngValue),next=seed*1103515245U+24691U;
 const unsigned values[]={WEATHER_RAIN,WEATHER_DROUGHT,WEATHER_SANDSTORM,WEATHER_SNOW};unsigned expected=values[(next>>16)&3];
 CALL(SetSavedWeather,expected==WEATHER_RAIN?WEATHER_SNOW:WEATHER_RAIN);CHECK(core->busRead32(core,SYM_gRngValue)==seed);
 for(steps=0;steps<4000000;steps++){unsigned pc=cpu->gprs[15];if(pc>=returnAddress && pc<returnAddress+6)break;core->step(core);}CHECK(steps<4000000);
 core->setKeys(core,0);CHECK(CALL(GetSav1Weather)==expected);CHECK(core->busRead32(core,SYM_gRngValue)==next);
 CHECK(core->busRead8(core,weatherContext()+4)==expected);
}
static void weatherSetAndWait(unsigned weather){
 CALL(SetWeather,weather);unsigned f;uint32_t wp=core->busRead32(core,SYM_gWeatherPtr);for(f=0;f<1200;f++){frames(1,0);if(CALL(GetCurrentWeather)==weather && core->busRead8(core,wp+ABI_WEATHER_PAL_STATE)==3 && core->busRead8(core,wp+ABI_WEATHER_GFX))break;}if(f==1200)fprintf(stderr,"weather render target%u current%u gfx%u complete%u pal%u\n",weather,CALL(GetCurrentWeather),core->busRead8(core,wp+ABI_WEATHER_GFX),core->busRead8(core,wp+ABI_WEATHER_COMPLETE),core->busRead8(core,wp+ABI_WEATHER_PAL_STATE));CHECK(f<1200);if(weather==WEATHER_SANDSTORM)CHECK(core->busRead8(core,wp+ABI_WEATHER_SAND_CREATED));
}
static void weatherCapture(const char *name){const char *dir=getenv("LIGHTNING_WEATHER_CAPTURE_DIR");if(dir){char path[256];snprintf(path,sizeof(path),"%s/%s.ppm",dir,name);screenshot(path);}}
static void weatherIntegrationTests(void){
 daycareFixture(1);clockAt(12);
 for(unsigned i=0;i<16;i++)core->busWrite8(core,weatherContext()+i,0xA9);
 weatherWarp(MAP_CINNABAR_ISLAND,10,10);frames(180,0);
 CHECK(CALL(GetSav1Weather)==WEATHER_DROUGHT);CHECK(CALL(GetCurrentWeather)==WEATHER_DROUGHT);
 uint32_t wp=core->busRead32(core,SYM_gWeatherPtr);CHECK(core->busRead8(core,wp+ABI_WEATHER_COMPLETE));
 CHECK(core->busRead8(core,wp+ABI_WEATHER_GAMMA)==0);CHECK(core->busRead8(core,wp+ABI_WEATHER_PAL_STATE)==3);weatherCapture("sun");
 clockAt(18);CHECK(CALL(DynamicWeatherForBattle)==WEATHER_NONE);frames(120,0);CHECK(CALL(GetSav1Weather)==WEATHER_NONE);CHECK(CALL(GetCurrentWeather)==WEATHER_NONE);
 clockAt(6);frames(120,0);CHECK(CALL(GetSav1Weather)==WEATHER_NONE);
 for(unsigned i=8;i<16;i++)CHECK(core->busRead8(core,weatherContext()+i)==0xA9);
 // Native GPIO absence must use daytime fallback, then recover without a warp.
 struct GBA *gba=core->board;unsigned devices=gba->memory.hw.devices;gba->memory.hw.devices&=~HW_RTC;
 CHECK(!CALL(RtcGetHour,scratch));clockAt(23);CALL(SetWeather,WEATHER_DROUGHT);frames(120,0);CHECK(CALL(GetSav1Weather)==WEATHER_DROUGHT);
 gba->memory.hw.devices=devices;frames(120,0);CHECK(CALL(GetSav1Weather)==WEATHER_NONE);
 printf("PASS: actual Cinnabar sunlight map load/render/fade completes, sunset mechanical/render conversion, no dawn restore, absent GPIO fallback and immediate recovered-night synchronization\n");
 CALL(VarSet,VAR_MAP_SCENE_ONE_ISLAND_HARBOR,3);CALL(VarSet,VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F,7);
 clockAt(12);weatherWarp(MAP_ONE_ISLAND,6,10);weatherSetAndWait(WEATHER_SNOW);frames(120,0);weatherCapture("snow");
 weatherWarp(MAP_ONE_ISLAND_POKEMON_CENTER_1F,7,8);CHECK(CALL(GetSav1Weather)==WEATHER_NONE);CHECK(core->busRead8(core,weatherContext()+4)==WEATHER_SNOW);CHECK(core->busRead8(core,weatherContext()+5)==1);
 weatherWarp(MAP_ONE_ISLAND_POKEMON_CENTER_2F,7,6);CHECK(core->busRead8(core,weatherContext()+4)==WEATHER_SNOW);
 flashRoundTrip();CHECK(core->busRead8(core,weatherContext()+4)==WEATHER_SNOW);CHECK(core->busRead8(core,weatherContext()+5)==1);
 weatherWarp(MAP_ONE_ISLAND,6,10);CHECK(CALL(WeatherContextValid));CHECK(core->busRead8(core,weatherContext()+5)==0);
 // Native Sunny Day move/animation must not replace persistent field Rain.
 weatherSetAndWait(WEATHER_RAIN);frames(120,0);weatherCapture("rain");monput(player(0),ABI_DATA_MOVE1,MOVE_SUNNY_DAY);monput(player(0),ABI_DATA_PP1,5);
 CALL(CreateMon,SYM_gEnemyParty,SPECIES_RATTATA,5,0,1,0,0,0);CALL(StartWildBattle);waitAction();CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_RAIN);
 chooseMove(0,0,1);waitAction();CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_SUN);CHECK(CALL(GetSav1Weather)==WEATHER_RAIN);
 weatherRun();waitOverworld(1);CHECK(CALL(GetSav1Weather)==WEATHER_RAIN);
 printf("PASS: actual Sunny Day move and animation replace battle Rain only; original field Rain remains on Run/return\n");
 for(unsigned i=8;i<16;i++)CHECK(core->busRead8(core,weatherContext()+i)==0xA9);
 const unsigned maps[]={MAP_NAVEL_ROCK_HARBOR,MAP_BIRTH_ISLAND_HARBOR,MAP_MT_EMBER_SUMMIT,MAP_SSANNE_DECK};
 for(unsigned i=0;i<4;i++){weatherWarp(maps[i],6,6);CHECK(CALL(WeatherContextValid));unsigned weather=CALL(GetSav1Weather);CHECK(i!=0||weather==WEATHER_RAIN);CHECK(i!=1||weather==WEATHER_SNOW||weather==WEATHER_SANDSTORM);CHECK(i!=2||weather==WEATHER_DROUGHT||weather==WEATHER_SANDSTORM);}
 weatherWarp(MAP_MT_EMBER_SUMMIT_PATH_1F,4,4);CHECK(CALL(GetSav1Weather)==WEATHER_NONE);
 printf("PASS: actual nested indoor weather suspension/flash context retention/outdoor return, explicit harbor/summit/deck scopes, untouched cave weather and save tail\n");
 // Real wild battle on Cycling Road, with a controlled enemy fixture.
 weatherWarp(MAP_ROUTE17,6,10);weatherSetAndWait(WEATHER_SANDSTORM);frames(120,0);weatherCapture("sand");frames(60,0);weatherCapture("sand_later");frames(60,0);weatherCapture("sand_later2");CALL(CreateMon,SYM_gEnemyParty,SPECIES_RATTATA,5,0,1,0,0,0);CALL(StartWildBattle);waitAction();
 CHECK(core->busRead8(core,SYM_gBattlersCount)==2);weatherRun();weatherWaitEndCallback(SYM_CB2_EndWildBattle);waitOverworld(1);
 CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);CHECK(CALL(WeatherContextValid));
 CHECK(core->busRead8(core,weatherContext()+3)==MAP_NUM(MAP_ROUTE17));
 unsigned weather=CALL(GetSav1Weather);CHECK(weather==WEATHER_RAIN||weather==WEATHER_DROUGHT||weather==WEATHER_SANDSTORM||weather==WEATHER_SNOW);
 printf("PASS: actual Cycling Road wild battle/Run callback executes exactly one measured native RNG draw and stores the predicted post-battle outcome\n");
 CALL(CreateMon,player(0),SPECIES_MEWTWO,100,31,1,0,0,0);monput(player(0),ABI_DATA_MOVE1,MOVE_SWIFT);monput(player(0),ABI_DATA_PP1,20);
 fossilStart(SYM_SaffronCity_Dojo_EventScript_Ranger1);waitAction();CHECK(core->busRead8(core,SYM_gBattleOutcome)==0);weatherWaitEndCallback(SYM_CB2_EndTrainerBattle);waitOverworld(1);fossilClose(1);
 CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_WON);CHECK(CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_RANGER_CAMP_1));
 printf("PASS: actual controlled Cycling Road trainer victory callback executes exactly one measured native reroll, then returns to field/post-battle script\n");
 CHECK(core->loadState(core,bootState));
}
static void dynamicWeatherTests(void){weatherPolicyTests();weatherIntegrationTests();}
