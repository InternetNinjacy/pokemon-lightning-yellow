#include "constants/special_weather.h"
static void sowLocation(unsigned map){
 core->busWrite8(core,careSave1()+ABI_SAVE1_LOCATION,MAP_GROUP(map));
 core->busWrite8(core,careSave1()+ABI_SAVE1_LOCATION+1,MAP_NUM(map));
}
static unsigned sowSeed(unsigned pick,unsigned count){
 unsigned seed=0;while((((seed*1103515245U+24691U)>>16)%count)!=pick)seed++;return seed;
}
static void specialWeatherTests(void){
 const unsigned maps[]={MAP_ROUTE12,MAP_ROUTE21_NORTH,MAP_ROUTE21_SOUTH,MAP_ROUTE23};
 const unsigned events[]={0,1,1,2},forced[]={WEATHER_RAIN,WEATHER_SNOW,WEATHER_SNOW,WEATHER_SANDSTORM};
 const unsigned active[]={FLAG_SOW_CORPHISH_ACTIVE,FLAG_SOW_GLALIE_ACTIVE,FLAG_SOW_GLALIE_ACTIVE,FLAG_SOW_TYRANITAR_ACTIVE};
 const unsigned resolved[]={FLAG_SOW_CORPHISH_RESOLVED,FLAG_SOW_GLALIE_RESOLVED,FLAG_SOW_GLALIE_RESOLVED,FLAG_SOW_TYRANITAR_RESOLVED};
 const unsigned baseline[][4]={{WEATHER_RAIN,WEATHER_SNOW},{WEATHER_RAIN,WEATHER_DROUGHT},{WEATHER_RAIN,WEATHER_DROUGHT},{WEATHER_RAIN,WEATHER_DROUGHT,WEATHER_SANDSTORM,WEATHER_SNOW}};
 for(unsigned m=0;m<4;m++){
  daycareFixture(1);clockAt(12);weatherWarp(maps[m],6,10);
  CALL(VarSet,0x8004,events[m]);CALL(SpecialWeatherActivateScript);CHECK(CALL(VarGet,0x800D));
  unsigned f;for(f=0;f<1200&&!CALL(SpecialWeatherRestorationReady);f++)frames(1,0);CHECK(f<1200);
  CHECK(CALL(GetSav1Weather)==forced[m]);CHECK(CALL(GetCurrentWeather)==forced[m]);
  CHECK(CALL(DynamicWeatherForBattle)==forced[m]);
  flashRoundTrip();CHECK(CALL(FlagGet,active[m]));CHECK(!CALL(FlagGet,resolved[m]));
  CHECK(CALL(DynamicWeatherForBattle)==forced[m]);
  unsigned seed=core->busRead32(core,SYM_gRngValue);
  CHECK(!CALL(SpecialWeatherFinishAftermath,events[m],B_OUTCOME_RAN));
  CHECK(!CALL(SpecialWeatherFinishAftermath,events[m],B_OUTCOME_LOST));
  CALL(DynamicWeatherAfterBattle);CHECK(core->busRead32(core,SYM_gRngValue)==seed);
  CHECK(CALL(FlagGet,active[m])&&!CALL(FlagGet,resolved[m]));
  if(m==1){
   sowLocation(MAP_ROUTE21_SOUTH);CHECK(CALL(DynamicWeatherForMap,WEATHER_SUNNY)==WEATHER_SNOW);
   CHECK(core->busRead32(core,SYM_gRngValue)==seed);
   weatherWarp(MAP_ROUTE21_SOUTH,6,10);CHECK(CALL(GetSav1Weather)==WEATHER_SNOW);
   sowLocation(maps[m]);
  }
  // Direct policy boundary, separate from unrelated NPC RNG during a live warp.
  sowLocation(m==0?MAP_ROUTE13:MAP_ROUTE22);core->busWrite32(core,SYM_gRngValue,0);
  unsigned elsewhere=CALL(DynamicWeatherForMap,WEATHER_SUNNY);CHECK(elsewhere==(m==0?WEATHER_NONE:WEATHER_RAIN));
  sowLocation(maps[m]);CHECK(CALL(DynamicWeatherForMap,WEATHER_SUNNY)==forced[m]);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
  for(unsigned time=0;time<3;time++)for(unsigned outcome=0;outcome<2;outcome++)for(unsigned pick=0;pick<(m==3?4:2);pick++){
   CHECK(core->loadState(core,state));clockAt(time?23:12);
   struct GBA *gba=core->board;unsigned devices=gba->memory.hw.devices;
   if(time==2){gba->memory.hw.devices&=~HW_RTC;CHECK(!CALL(RtcGetHour,scratch));}
   unsigned value=sowSeed(pick,m==3?4:2),next=value*1103515245U+24691U;
   core->busWrite32(core,SYM_gRngValue,value);
   CHECK(CALL(SpecialWeatherFinishAftermath,events[m],outcome?B_OUTCOME_CAUGHT:B_OUTCOME_WON));
   unsigned expected=baseline[m][pick];if(time==1&&expected==WEATHER_DROUGHT)expected=WEATHER_NONE;
   CHECK(core->busRead32(core,SYM_gRngValue)==next);CHECK(CALL(GetSav1Weather)==expected);
   CHECK(CALL(FlagGet,resolved[m])&&!CALL(FlagGet,active[m]));
   CHECK(!CALL(SpecialWeatherFinishAftermath,events[m],B_OUTCOME_CAUGHT));CHECK(!CALL(SpecialWeatherActivate,events[m]));
   CALL(DynamicWeatherAfterBattle);CALL(DynamicWeatherAfterBattle);
   CHECK(CALL(DynamicWeatherForMap,WEATHER_SUNNY)==expected);
   CHECK(core->busRead32(core,SYM_gRngValue)==next);
   gba->memory.hw.devices=devices;
  }
  free(state);clockAt(12);
  flashRoundTrip();CHECK(CALL(FlagGet,resolved[m])&&!CALL(FlagGet,active[m]));CHECK(!CALL(SpecialWeatherActivate,events[m]));
  printf("PASS: native SOW event%u map%u: actual forced field/render/battle policy, active/resolved flash persistence, outcome gating, every day/night/invalid-clock result and exactly one measured roll/no callback reroll\n",events[m],maps[m]);
 }
 // An ordinary real battle on an overridden route must not resolve its SOW.
 daycareFixture(1);clockAt(12);weatherWarp(MAP_ROUTE12,6,10);CHECK(CALL(SpecialWeatherActivate,0));
 for(unsigned f=0;f<1200&&!CALL(SpecialWeatherRestorationReady);f++)frames(1,0);
 CALL(SetMonMoveSlot,player(0),MOVE_SUNNY_DAY,0);
 CALL(CreateMon,enemy(0),SPECIES_RATTATA,5,0,1,0,0,0);CALL(StartWildBattle);waitAction();
 CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_RAIN);
 chooseMove(0,0,1);waitAction();CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_SUN);
 CHECK(CALL(GetSav1Weather)==WEATHER_RAIN);weatherRun();waitOverworld(1);
 CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);
 CHECK(CALL(FlagGet,FLAG_SOW_CORPHISH_ACTIVE)&&!CALL(FlagGet,FLAG_SOW_CORPHISH_RESOLVED));
 CHECK(CALL(GetSav1Weather)==WEATHER_RAIN);
 printf("PASS: actual ordinary Route12 battle starts forced Rain, Sunny Day changes battle only, real Run/field return preserves unresolved override\n");
 // Real script interpreter: aftermath marker, common resolver, native visual wait,
 // next script statement and player control only after real weather completion.
 daycareFixture(1);clockAt(12);weatherWarp(MAP_ROUTE12,6,10);CHECK(CALL(SpecialWeatherActivate,0));
 for(unsigned f=0;f<1200&&!CALL(SpecialWeatherRestorationReady);f++)frames(1,0);
 CHECK(CALL(SpecialWeatherRestorationReady));
 unsigned script=CALL(Alloc,16);CHECK(script);unsigned char bytes[12]={0x29,FLAG_TEMP_2&255,FLAG_TEMP_2>>8,0x04,0,0,0,0,0x29,FLAG_TEMP_1&255,FLAG_TEMP_1>>8,0x02};
 unsigned target=SYM_SpecialWeather_EventScript_FinishAftermath;for(unsigned i=0;i<4;i++)bytes[4+i]=(target>>(8*i))&255;
 for(unsigned i=0;i<sizeof(bytes);i++)core->busWrite8(core,script+i,bytes[i]);
 CALL(VarSet,0x8004,0);CALL(VarSet,0x8005,B_OUTCOME_WON);CALL(FlagClear,FLAG_TEMP_1);CALL(FlagClear,FLAG_TEMP_2);
 void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned observed=0;
 for(unsigned trial=0;trial<16&&!observed;trial++){
  CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,trial);fossilStart(script);
  unsigned waited=0,f;
  for(f=0;f<1200&&!CALL(FlagGet,FLAG_TEMP_1);f++){
   frames(1,0);
   if(CALL(FlagGet,FLAG_SOW_CORPHISH_RESOLVED)){
    CHECK(CALL(FlagGet,FLAG_TEMP_2));
    if(!CALL(SpecialWeatherRestorationReady)){CHECK(!CALL(FlagGet,FLAG_TEMP_1));CHECK(CALL(ArePlayerFieldControlsLocked));waited++;}
   }
  }
  if(f==1200){unsigned wp=core->busRead32(core,SYM_gWeatherPtr);fprintf(stderr,"SOW wait current=%u saved=%u ready=%u complete=%u gfx=%u pal=%u result=%u markers=%u/%u locked=%u\n",CALL(GetCurrentWeather),CALL(GetSav1Weather),CALL(SpecialWeatherRestorationReady),core->busRead8(core,wp+ABI_WEATHER_COMPLETE),core->busRead8(core,wp+ABI_WEATHER_GFX),core->busRead8(core,wp+ABI_WEATHER_PAL_STATE),CALL(VarGet,0x800D),CALL(FlagGet,FLAG_TEMP_1),CALL(FlagGet,FLAG_TEMP_2),CALL(ArePlayerFieldControlsLocked));}
  CHECK(f<1200);CHECK(CALL(SpecialWeatherRestorationReady));frames(3,0);CHECK(!CALL(ArePlayerFieldControlsLocked));
  observed=waited>0;
 }
 CHECK(observed);free(state);CALL(Free,script);CHECK(core->loadState(core,bootState));
 printf("PASS: actual event-script aftermath marker precedes resolution, native wait holds script/player controls until weather graphics/palette finish; no invented event placement or farewell\n");
}
