static void resetEvent(void){memset(&save,0,sizeof(save));memset(eventFlags,0,sizeof(eventFlags));hour=12;valid=1;fixed=0;}
int main(void){
 const unsigned maps[]={MAP_ROUTE12,MAP_ROUTE21_NORTH,MAP_ROUTE21_SOUTH,MAP_ROUTE23};
 const unsigned events[]={0,1,1,2},forced[]={WEATHER_RAIN,WEATHER_SNOW,WEATHER_SNOW,WEATHER_SANDSTORM};
 const unsigned base[][4]={{WEATHER_RAIN,WEATHER_SNOW},{WEATHER_RAIN,WEATHER_DROUGHT},{WEATHER_RAIN,WEATHER_DROUGHT},{WEATHER_RAIN,WEATHER_DROUGHT,WEATHER_SANDSTORM,WEATHER_SNOW}};
 unsigned checks=0;
 for(unsigned m=0;m<4;m++)for(unsigned time=0;time<3;time++)for(unsigned outcome=0;outcome<2;outcome++)for(unsigned roll=0;roll<(m==3?4:2);roll++){
  resetEvent();hour=time?23:12;valid=time!=2;
  save.location.mapGroup=MAP_GROUP(maps[m]);save.location.mapNum=MAP_NUM(maps[m]);gMapHeader.mapType=MAP_TYPE_ROUTE;gMapHeader.weather=WEATHER_SUNNY;
  unsigned before=draws;assert(SpecialWeatherActivate(events[m]));assert(draws==before&&save.weather==forced[m]);
  assert(DynamicWeatherForMap(WEATHER_SUNNY)==forced[m]);assert(DynamicWeatherForBattle()==forced[m]);DynamicWeatherAfterBattle();assert(draws==before);
  for(unsigned reject=0;reject<=8;reject++)if(reject!=B_OUTCOME_WON&&reject!=B_OUTCOME_CAUGHT){assert(!SpecialWeatherFinishAftermath(events[m],reject));assert(draws==before&&save.weather==forced[m]);checks++;}
  fixed=roll;assert(SpecialWeatherFinishAftermath(events[m],outcome?B_OUTCOME_CAUGHT:B_OUTCOME_WON));
  unsigned expected=base[m][roll];if(time==1&&expected==WEATHER_DROUGHT)expected=WEATHER_NONE;
  assert(draws==before+1&&save.weather==expected&&current==expected);
  assert(FlagGet(sSpecialWeatherPolicies[events[m]].resolved)&&!FlagGet(sSpecialWeatherPolicies[events[m]].active));
  assert(!SpecialWeatherActivate(events[m]));assert(!SpecialWeatherFinishAftermath(events[m],B_OUTCOME_WON));
  DynamicWeatherAfterBattle();assert(DynamicWeatherForMap(WEATHER_SUNNY)==expected);assert(draws==before+1);checks+=10;
 }
 resetEvent();save.location.mapGroup=MAP_GROUP(MAP_ROUTE21_NORTH);save.location.mapNum=MAP_NUM(MAP_ROUTE21_NORTH);
 assert(SpecialWeatherActivate(1));unsigned before=draws;save.location.mapNum=MAP_NUM(MAP_ROUTE21_SOUTH);
 assert(DynamicWeatherForMap(WEATHER_SUNNY)==WEATHER_SNOW&&draws==before);
 save.location.mapNum=MAP_NUM(MAP_ROUTE22);assert(!SpecialWeatherFinishAftermath(1,B_OUTCOME_WON));assert(DynamicWeatherForMap(WEATHER_SUNNY)==WEATHER_RAIN&&draws==before+1);
 assert(!SpecialWeatherActivate(3));gSpecialVar_0x8004=256;gSpecialVar_0x8005=B_OUTCOME_WON;SpecialWeatherResolveScript();assert(!gSpecialVar_Result);
 gWeatherPtr->weatherChangeComplete=0;assert(!SpecialWeatherRestorationReady());
 gWeatherPtr->weatherChangeComplete=1;gWeatherPtr->weatherGfxLoaded=1;gWeatherPtr->palProcessingState=WEATHER_PAL_STATE_IDLE;current=save.weather;assert(SpecialWeatherRestorationReady());
 printf("PASS: %u special-route assertions: all4 map IDs, capture/defeat, all baseline outcomes, night/invalid RTC, rejected outcomes, no boundary/double reroll and no adjacent leakage (host stubs)\n",checks);
}
