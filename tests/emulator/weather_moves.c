// Actual battle-script command execution plus real Rain Zap Cannon turns.
#include "weather_abilities.c"
#include "weather_gym_abilities.c"
#include "special_glalie.c"
static void weatherMoveTests(void){
 specialGlalieTests();
 if(getenv("LIGHTNING_SPECIAL_GLALIE_ONLY"))return;
 if(!getenv("LIGHTNING_WEATHER_GYM_ABILITIES_ONLY"))weatherAbilityTests();
 weatherGymAbilityTests();
 if(getenv("LIGHTNING_WEATHER_ABILITIES_ONLY")||getenv("LIGHTNING_WEATHER_GYM_ABILITIES_ONLY"))return;
 wildFixture(MAP_GROUP(MAP_MT_MOON_1F),MAP_NUM(MAP_MT_MOON_1F),1);startSpecial();waitAction();damageStats();
 uint32_t script=CALL(AllocZeroed,16);CHECK(script);
 core->busWrite16(core,bmon(0)+ABI_BMON_MAX_HP,160);core->busWrite32(core,SYM_gStatuses3,0);
 for(unsigned a=0;a<18;a++)for(unsigned b=0;b<18;b++){
  core->busWrite8(core,bmon(0)+ABI_BMON_TYPE1,a);core->busWrite8(core,bmon(0)+ABI_BMON_TYPE2,b);
  core->busWrite16(core,SYM_gBattleWeather,B_WEATHER_SANDSTORM_PERMANENT);core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_weatherdamage);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==(a==TYPE_GROUND||b==TYPE_GROUND?0:10));
 }
 core->busWrite8(core,bmon(0)+ABI_BMON_TYPE1,TYPE_ROCK);core->busWrite8(core,bmon(0)+ABI_BMON_TYPE2,TYPE_STEEL);
 const unsigned hp[]={1,15,16,17,32};for(unsigned i=0;i<5;i++){
  core->busWrite16(core,bmon(0)+ABI_BMON_MAX_HP,hp[i]);core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_weatherdamage);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==(hp[i]<16?1:hp[i]/16));
 }
 core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,ABILITY_SAND_VEIL);core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_weatherdamage);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==0);core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,ABILITY_NONE);
 for(unsigned ability=0;ability<2;ability++){
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ability?ABILITY_AIR_LOCK:ABILITY_CLOUD_NINE);core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_weatherdamage);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==0);
 }core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_NONE);
 core->busWrite16(core,SYM_gBattleWeather,B_WEATHER_HAIL);core->busWrite8(core,bmon(0)+ABI_BMON_TYPE1,TYPE_ICE);core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_weatherdamage);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==0);
 // Interpreter layout is opcode, fail pointer, move; heap data cannot overlap the guarded return stub.
 core->busWrite8(core,script,1);for(unsigned i=0;i<4;i++)core->busWrite8(core,script+1+i,(script+12)>>(8*i));core->busWrite8(core,script+5,MOVE_ZAP_CANNON&255);core->busWrite8(core,script+6,MOVE_ZAP_CANNON>>8);
 core->busWrite16(core,SYM_gCurrentMove,MOVE_ZAP_CANNON);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_NORMAL);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_NORMAL);
 for(unsigned w=0;w<5;w++){
  unsigned hits=0;core->busWrite16(core,SYM_gBattleWeather,w==2?B_WEATHER_SUN_PERMANENT:w?B_WEATHER_RAIN_PERMANENT:0);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,w==3?ABILITY_CLOUD_NINE:w==4?ABILITY_AIR_LOCK:ABILITY_NONE);
  for(unsigned i=0;i<1000;i++){
   core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);core->busWrite8(core,SYM_gMoveResultFlags,0);core->busWrite32(core,SYM_gHitMarker,0);core->busWrite32(core,SYM_gRngValue,0x5A900000+i*7919);
   core->busWrite8(core,bmon(0)+ABI_BMON_STAGES+STAT_ACC,w==1?0:6);core->busWrite8(core,bmon(1)+ABI_BMON_STAGES+STAT_EVASION,w==1?12:6);
   CALL(Cmd_accuracycheck);unsigned pointer=core->busRead32(core,SYM_gBattlescriptCurrInstr);if(pointer!=script+7&&pointer!=script+12)fprintf(stderr,"accuracy w%u i%u ptr%08x script%08x flags%u atk%u target%u move%u ability%u\n",w,i,pointer,script,core->busRead8(core,SYM_gMoveResultFlags),core->busRead8(core,SYM_gBattlerAttacker),core->busRead8(core,SYM_gBattlerTarget),core->busRead16(core,SYM_gCurrentMove),core->busRead8(core,bmon(1)+ABI_BMON_ABILITY));CHECK(pointer==script+7||pointer==script+12);hits+=pointer==script+7;
  }
  if(w==1)CHECK(hits==1000);else CHECK(hits>400&&hits<600);
  printf("PASS: native Zap Cannon accuracy weather%u hits%u/1000; Rain bypasses worst accuracy/evasion, outside remains original50%%\n",w,hits);
 }
 CALL(Free,script);
 for(unsigned scenario=0;scenario<3;scenario++){
  wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),0);
  for(unsigned k=0;k<4;k++)CALL(SetMonMoveSlot,player(0),k?MOVE_SPLASH:MOVE_ZAP_CANNON,k);
  CHECK(CALL(TryStartLandEncounter,landInfo(),0));waitAction();core->busWrite16(core,SYM_gBattleWeather,B_WEATHER_RAIN_PERMANENT);
  for(unsigned b=0;b<2;b++){
   core->busWrite8(core,bmon(b)+ABI_BMON_ABILITY,ABILITY_NONE);core->busWrite8(core,bmon(b)+ABI_BMON_TYPE1,b&&scenario==1?TYPE_GROUND:TYPE_NORMAL);core->busWrite8(core,bmon(b)+ABI_BMON_TYPE2,b&&scenario==1?TYPE_GROUND:TYPE_NORMAL);
   core->busWrite16(core,bmon(b)+ABI_BMON_HP,1000);core->busWrite16(core,bmon(b)+ABI_BMON_MAX_HP,1000);monput(b?enemy(0):player(0),ABI_DATA_HP,1000);monput(b?enemy(0):player(0),ABI_DATA_MAX_HP,1000);
  }
  for(unsigned k=0;k<4;k++){unsigned move=scenario==2?MOVE_PROTECT:MOVE_SPLASH;core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*k,move);core->busWrite8(core,bmon(1)+ABI_BMON_PP+k,10);CALL(SetMonMoveSlot,enemy(0),move,k);}
  chooseMove(0,0,1);waitAction();CHECK(monget(player(0),ABI_DATA_PP1)==4);
  unsigned damage=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
  if(!scenario){CHECK(damage>0);CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_PARALYSIS);}else{CHECK(!damage);CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)==0);}
  printf("PASS: actual Rain Zap Cannon turn scenario%u damage%u, native animation/onePP/paralysis or Ground/Protect safeguard\n",scenario,damage);
 }
 CHECK(core->loadState(core,bootState));
}
