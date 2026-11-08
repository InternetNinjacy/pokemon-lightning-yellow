// Actual ARM functions and actual turn processing; no species ability slots changed.
static void weatherAbilityTests(void)
{
 wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),0);
 CHECK(CALL(TryStartLandEncounter,landInfo(),0));waitAction();
 unsigned script=CALL(AllocZeroed,16);CHECK(script);
 void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
 const unsigned abilities[]={ABILITY_NONE,ABILITY_SAND_VEIL,ABILITY_ICE_BODY,ABILITY_OVERCOAT};
 for(unsigned w=0;w<2;w++)for(unsigned a=0;a<4;a++)for(unsigned t=0;t<18;t++){
  CHECK(core->loadState(core,state));
  core->busWrite8(core,SYM_gBattlerAttacker,0);
  core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,abilities[a]);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_NONE);
  core->busWrite8(core,bmon(0)+ABI_BMON_TYPE1,t);core->busWrite8(core,bmon(0)+ABI_BMON_TYPE2,t);
  core->busWrite16(core,bmon(0)+ABI_BMON_MAX_HP,160);
  core->busWrite32(core,SYM_gStatuses3,0);
  core->busWrite16(core,SYM_gBattleWeather,w?B_WEATHER_HAIL_PERMANENT:B_WEATHER_SANDSTORM_PERMANENT);
  core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_weatherdamage);
  unsigned immune=abilities[a]==ABILITY_OVERCOAT||(w?(t==TYPE_ICE||abilities[a]==ABILITY_ICE_BODY):(t==TYPE_GROUND||abilities[a]==ABILITY_SAND_VEIL));
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==(immune?0:10));
 }
 const unsigned weathers[]={0,B_WEATHER_HAIL_PERMANENT,B_WEATHER_RAIN_PERMANENT,B_WEATHER_SUN_PERMANENT,B_WEATHER_SANDSTORM_PERMANENT};
 for(unsigned who=0;who<2;who++)for(unsigned a=0;a<2;a++)for(unsigned w=0;w<5;w++)for(unsigned hpCase=0;hpCase<4;hpCase++)for(unsigned suppress=0;suppress<3;suppress++){
  CHECK(core->loadState(core,state));
  core->busWrite8(core,bmon(who)+ABI_BMON_ABILITY,a?ABILITY_ICE_BODY:ABILITY_RAIN_DISH);
  core->busWrite8(core,bmon(who^1)+ABI_BMON_ABILITY,suppress==1?ABILITY_CLOUD_NINE:suppress==2?ABILITY_AIR_LOCK:ABILITY_NONE);
  core->busWrite16(core,bmon(who)+ABI_BMON_MAX_HP,hpCase==3?15:160);
  core->busWrite16(core,bmon(who)+ABI_BMON_HP,hpCase==0?80:hpCase==1?160:hpCase==2?0:10);
  core->busWrite16(core,SYM_gBattleWeather,weathers[w]);
  unsigned effect=CALL(AbilityBattleEffects,1,who,0,0,0);
  unsigned heals=!suppress&&(hpCase==0||hpCase==3)&&(a?w==1:w==2);
  CHECK(effect==heals);
  if(heals){CHECK((int32_t)core->busRead32(core,SYM_gBattleMoveDamage)==(hpCase==3?-1:-10));CHECK(core->busRead8(core,SYM_gLastUsedAbility)==(a?ABILITY_ICE_BODY:ABILITY_RAIN_DISH));}
 }
 CHECK(core->loadState(core,state));
 // Actual switch-in ability script/animation and indefinite Hail, not just a flag.
 unsigned prior=core->busRead32(core,SYM_gBattleMainFunc);
 core->busWrite16(core,SYM_gBattleWeather,B_WEATHER_SUN_PERMANENT);
 core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,ABILITY_SNOW_WARNING);
 CHECK(CALL(AbilityBattleEffects,0,0,0,0,0));
 CHECK(core->busRead16(core,SYM_gBattleWeather)==B_WEATHER_HAIL_PERMANENT);
 unsigned f;for(f=0;f<2400;f++){frames(1,(f%12)<3?1:0);if(core->busRead32(core,SYM_gBattleMainFunc)==prior)break;}CHECK(f<2400);
 CHECK(!CALL(AbilityBattleEffects,0,0,0,0,0));
 printf("PASS: actual Snow Warning switch-in callback, existing hail message/animation and weather-form script complete; permanent Hail replaces Sun, repeated activation suppressed\n");
 free(state);CALL(Free,script);
 // Whole turns prove residual damage/healing dispatch, not only helper arithmetic.
 const unsigned turnAbility[]={ABILITY_ICE_BODY,ABILITY_OVERCOAT,ABILITY_NONE,ABILITY_RAIN_DISH,ABILITY_ICE_BODY,ABILITY_OVERCOAT,ABILITY_OVERCOAT,ABILITY_ICE_BODY,ABILITY_OVERCOAT};
 const unsigned turnWeather[]={B_WEATHER_HAIL_PERMANENT,B_WEATHER_HAIL_PERMANENT,B_WEATHER_HAIL_PERMANENT,B_WEATHER_RAIN_PERMANENT,B_WEATHER_RAIN_PERMANENT,B_WEATHER_SANDSTORM_PERMANENT,B_WEATHER_HAIL_PERMANENT,B_WEATHER_HAIL_PERMANENT,B_WEATHER_HAIL_PERMANENT};
 const unsigned expected[]={260,240,220,260,240,240,240,240,240};
 for(unsigned s=0;s<9;s++){
  wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),0);
  for(unsigned k=0;k<4;k++)CALL(SetMonMoveSlot,player(0),MOVE_SPLASH,k);
  CHECK(CALL(TryStartLandEncounter,landInfo(),0));waitAction();
  for(unsigned b=0;b<2;b++){
   unsigned mon=b?enemy(0):player(0);
   core->busWrite8(core,bmon(b)+ABI_BMON_ABILITY,b?(s==7?ABILITY_CLOUD_NINE:s==8?ABILITY_ICE_BODY:ABILITY_NONE):turnAbility[s]);
   core->busWrite8(core,bmon(b)+ABI_BMON_TYPE1,TYPE_NORMAL);core->busWrite8(core,bmon(b)+ABI_BMON_TYPE2,TYPE_NORMAL);
   core->busWrite16(core,bmon(b)+ABI_BMON_HP,b&&s!=8?320:240);core->busWrite16(core,bmon(b)+ABI_BMON_MAX_HP,320);
   monput(mon,ABI_DATA_HP,b&&s!=8?320:240);monput(mon,ABI_DATA_MAX_HP,320);
   for(unsigned k=0;k<4;k++){core->busWrite16(core,bmon(b)+ABI_BMON_MOVES+2*k,MOVE_SPLASH);core->busWrite8(core,bmon(b)+ABI_BMON_PP+k,40);CALL(SetMonMoveSlot,mon,MOVE_SPLASH,k);}
  }
  core->busWrite16(core,SYM_gBattleWeather,turnWeather[s]);
  chooseMove(0,0,1);waitAction();
  unsigned hp=core->busRead16(core,bmon(0)+ABI_BMON_HP);if(hp!=expected[s])fprintf(stderr,"weather ability turn%u HP%u expected%u\n",s,hp,expected[s]);CHECK(hp==expected[s]);
  CHECK(monget(player(0),ABI_DATA_HP)==expected[s]);
  if(s==8){CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==260);CHECK(monget(enemy(0),ABI_DATA_HP)==260);}
  if(s==0){
   for(unsigned turn=0;turn<5;turn++){
    chooseMove(0,0,1);waitAction();CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_HAIL_PERMANENT);
    CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==(turn<3?280+20*turn:320));
   }
   // A real weather move replaces permanent Hail; expiry never reinstates it.
   CALL(SetMonMoveSlot,player(0),MOVE_SUNNY_DAY,0);
   core->busWrite16(core,bmon(0)+ABI_BMON_MOVES,MOVE_SUNNY_DAY);core->busWrite8(core,bmon(0)+ABI_BMON_PP,5);
   chooseMove(0,0,1);waitAction();CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_SUN_TEMPORARY);
   CHECK(!(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_HAIL));
   for(unsigned turn=0;turn<4;turn++){chooseMove(0,1,1);waitAction();}
   CHECK(!(core->busRead16(core,SYM_gBattleWeather)&(B_WEATHER_SUN|B_WEATHER_HAIL)));
  }
 }
 CHECK(core->loadState(core,bootState));
 printf("PASS: actual Ice Body/Overcoat/Rain Dish/ordinary turns: exact HP, real healing script, both residual protections, non-Hail no heal and Cloud Nine suppression; assignments are test fixtures only\n");
}
