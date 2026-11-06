// Execute native Gym scripts, constructor, AI, weather/form animations and return.
#include "gym_castform_expected.h"
static const unsigned gymMaps[]={MAP_PEWTER_CITY_GYM,MAP_CERULEAN_CITY_GYM,MAP_CELADON_CITY_GYM};
static const unsigned gymTrainers[]={TRAINER_CAMPER_LIAM,TRAINER_PICNICKER_DIANA,TRAINER_COOLTRAINER_MARY};
static const unsigned gymLocal[]={2,2,8};
static const unsigned gymEntry[]={SYM_PewterCity_Gym_EventScript_Liam,SYM_CeruleanCity_Gym_EventScript_Diana,SYM_CeladonCity_Gym_EventScript_Mary};
static const unsigned gymWeather[]={WEATHER_SNOW,WEATHER_NONE,WEATHER_DROUGHT};
static const unsigned gymType[]={TYPE_ICE,TYPE_WATER,TYPE_FIRE};
static void gymFixture(unsigned t,unsigned count){
 daycareFixture(1);clockAt(23);CALL(SetLastHealLocationWarp,HEAL_LOCATION_CERULEAN_CITY);
 for(unsigned i=0;i<count;i++){CALL(CreateMon,player(i),SPECIES_MEWTWO,100,31,1,i,0,0);monput(player(i),ABI_DATA_MOVE1,t==1?MOVE_SPLASH:MOVE_SWIFT);monput(player(i),ABI_DATA_PP1,40);monput(player(i),ABI_DATA_MOVE1+1,MOVE_SWIFT);monput(player(i),ABI_DATA_PP1+1,20);}
 for(unsigned i=count;i<6;i++)clearBytes(player(i),ABI_MON_SIZE);
 core->busWrite8(core,SYM_gPlayerPartyCount,count);
 fprintf(stderr,"INFO Gym fixture%u count%u warp\n",t,count);
 fossilWarp(MAP_GROUP(gymMaps[t]),MAP_NUM(gymMaps[t]),t==0?4:t==1?5:6,t==0?10:t==1?9:12);
 unsigned obj=CALL(GetObjectEventIdByLocalIdAndMap,gymLocal[t],MAP_NUM(gymMaps[t]),MAP_GROUP(gymMaps[t]));CHECK(obj<ABI_OBJECT_COUNT);core->busWrite8(core,SYM_gSelectedObjectEvent,obj);
 CHECK(CALL(GetSav1Weather)==gymWeather[t]);CHECK(CALL(DynamicWeatherForBattle)==gymWeather[t]);
}
static void gymDataTests(void){
 for(unsigned t=0;t<3;t++){
  gymFixture(t,2);core->busWrite32(core,SYM_gBattleTypeFlags,BATTLE_TYPE_TRAINER|(t==1?BATTLE_TYPE_DOUBLE:0));
  CHECK(CALL(CreateNPCTrainerParty,SYM_gEnemyParty,gymTrainers[t])==2);
  for(unsigned i=0;i<2;i++){
   CHECK(monget(enemy(i),ABI_DATA_SPECIES)==gymExpectedSpecies[t][i]);CHECK(monget(enemy(i),ABI_DATA_LEVEL)==gymExpectedLevels[t][i]);CHECK(monget(enemy(i),ABI_DATA_HELD_ITEM)==ITEM_NONE);
   for(unsigned k=0;k<4;k++)CHECK(monget(enemy(i),ABI_DATA_MOVE1+k)==gymExpectedMoves[t][i][k]);
   if(monget(enemy(i),ABI_DATA_SPECIES)==SPECIES_CASTFORM)CHECK(CALL(GetMonAbility,enemy(i))==ABILITY_FORECAST);
  }
 }
 printf("PASS: real ROM constructs all six exact Gym Pokemon, levels, move slots, no held items and original Forecast\n");
}
static void gymLegacyWeatherTests(void){
 for(unsigned t=0;t<3;t+=2){
  gymFixture(t,2);clockAt(12);weatherWarp(MAP_CINNABAR_ISLAND,10,10);weatherSetAndWait(WEATHER_RAIN);
  weatherWarp(gymMaps[t],t==0?4:6,t==0?10:12);CHECK(core->busRead8(core,weatherContext()+4)==WEATHER_RAIN);CHECK(core->busRead8(core,weatherContext()+5)==1);
  unsigned char context[8];for(unsigned i=0;i<8;i++)context[i]=core->busRead8(core,weatherContext()+i);
  CALL(SetSavedWeather,WEATHER_NONE);flashRoundTrip();CHECK(CALL(GetSav1Weather)==WEATHER_NONE);CALL(CB2_ContinueSavedGame);frames(480,0);CHECK(fieldScriptDone());
  CHECK(CALL(GetSav1Weather)==gymWeather[t]);CHECK(CALL(GetCurrentWeather)==gymWeather[t]);for(unsigned i=0;i<8;i++)CHECK(core->busRead8(core,weatherContext()+i)==context[i]);
  clockAt(23);frames(32,0);CHECK(CALL(DynamicWeatherForBattle)==gymWeather[t]);
 }
 printf("PASS: real flash/Continue from legacy clear saves in both fixed-weather Gyms adopts header snow/Sun while preserving outdoor Rain context; nighttime does not clear Gym Sun\n");
}
static void gymBallType(unsigned type,unsigned doubled){
 uint32_t bs=core->busRead32(core,SYM_gBattleStruct),instr=core->busRead32(core,SYM_gBattlescriptCurrInstr);
 unsigned oldType=core->busRead8(core,bs+ABI_BATTLESTRUCT_DYNAMIC_TYPE),oldMult=core->busRead8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER);
 core->busWrite8(core,bs+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);core->busWrite32(core,SYM_gBattlescriptCurrInstr,scratch);
 CALL(Cmd_setweatherballtype);CHECK((core->busRead8(core,bs+ABI_BATTLESTRUCT_DYNAMIC_TYPE)&0x3f)==type);CHECK(core->busRead8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER)==(doubled?2:1));
 core->busWrite8(core,bs+ABI_BATTLESTRUCT_DYNAMIC_TYPE,oldType);core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,oldMult);core->busWrite32(core,SYM_gBattlescriptCurrInstr,instr);
}
static void gymFinish(unsigned t){
 unsigned f,quiet=0,seen=0;
 for(f=0;f<30000;f++){
  if(fieldScriptDone()){if(++quiet==30)break;frames(1,0);continue;}quiet=0;
  unsigned key=0;
  if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_BattleMainCB2){
   if(core->busRead16(core,bmon(1)+ABI_BMON_SPECIES)==SPECIES_CASTFORM && core->busRead16(core,bmon(1)+ABI_BMON_HP) && core->busRead8(core,bmon(1)+ABI_BMON_TYPE1)==gymType[t] && (t!=2 || core->busRead8(core,SYM_gBattleMonForms+1)==1))seen=1;
   if(f%8==0){
    key=2;unsigned exec=core->busRead32(core,SYM_gBattleControllerExecFlags);
    for(unsigned b=0;b<4;b+=2)if(exec&(1U<<b)){
     unsigned fn=controller(b),cursor;
     if(fn==SYM_HandleInputChooseAction){cursor=core->busRead8(core,SYM_gActionSelectionCursor+b);key=cursor&2?64:cursor&1?32:1;break;}
     if(fn==SYM_HandleInputChooseMove){cursor=core->busRead8(core,SYM_gMoveSelectionCursor+b);key=cursor&2?64:((cursor&1)!=(t==1))?16:1;break;}
     if(fn==SYM_HandleInputChooseTarget){key=1;break;}
    }
   }
  }else if(f%8==0)key=2;
  frames(1,key);
 }
 if(f==30000)fprintf(stderr,"Gym%u timeout outcome%u callback%08x\n",t,core->busRead8(core,SYM_gBattleOutcome),core->busRead32(core,SYM_gMain+ABI_MAIN_CB2));
 CHECK(f<30000);CHECK(seen);CHECK(CALL(FlagGet,TRAINER_FLAGS_START+gymTrainers[t]));CHECK(CALL(GetSav1Weather)==gymWeather[t]);
 flashRoundTrip();CHECK(CALL(FlagGet,TRAINER_FLAGS_START+gymTrainers[t]));
 printf("PASS: native Gym%u victory, correct Castform weather type, field return and saved defeat flag in%u frames\n",t,f);
}
static void gymCastformTests(void){
 gymDataTests();gymLegacyWeatherTests();
 // Native Double script must reject one usable mon, including a fainted second.
 for(unsigned n=0;n<2;n++){
  gymFixture(1,n?2:1);if(n)monput(player(1),ABI_DATA_HP,0);
  fossilStart(gymEntry[1]);fossilClose(1);CHECK(!(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_TRAINER));CHECK(!CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_PICNICKER_DIANA));vendorTextMatches(SYM_SaffronCity_Dojo_Text_NeedTwo);
 }
 printf("PASS: Diana native mandatory Double refuses one mon and one usable plus fainted second without setting defeat\n");
 for(unsigned t=0;t<3;t++){
  gymFixture(t,2);frames(180,0);CHECK(CALL(GetCurrentWeather)==gymWeather[t]);
  // Noon and night do not normalize fixed indoor Sun; outside context stays separate.
  clockAt(12);CHECK(CALL(DynamicWeatherForBattle)==gymWeather[t]);clockAt(23);frames(32,0);CHECK(CALL(DynamicWeatherForBattle)==gymWeather[t]);
  fossilStart(gymEntry[t]);waitAction();CHECK(core->busRead16(core,SYM_gTrainerBattleOpponent_A)==gymTrainers[t]);CHECK(core->busRead8(core,SYM_gBattlersCount)==(t==1?4:2));
  CHECK(!!(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_DOUBLE)==(t==1));
  uint32_t hist=core->busRead32(core,core->busRead32(core,SYM_gBattleResources)+ABI_BRES_HISTORY);CHECK(core->busRead8(core,hist+ABI_HISTORY_COUNT)==0);
  if(t==0){CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_HAIL_PERMANENT);CHECK(core->busRead8(core,bmon(1)+ABI_BMON_TYPE1)==TYPE_ICE);CHECK(core->busRead8(core,SYM_gBattleMonForms+1)==3);gymBallType(TYPE_ICE,1);}
  if(t==2){CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_SUN_PERMANENT);gymBallType(TYPE_FIRE,1);}
  if(t==1){
   CHECK(core->busRead16(core,SYM_gBattleWeather)==0);CHECK(core->busRead16(core,bmon(1)+ABI_BMON_SPECIES)==SPECIES_CASTFORM);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_SPECIES)==SPECIES_SLOWPOKE);CHECK(core->busRead8(core,bmon(1)+ABI_BMON_TYPE1)==TYPE_NORMAL);gymBallType(TYPE_NORMAL,0);
   // Player-only non-damaging fixtures allow the unmodified enemy AI to use its
   // actual approved Rain Dance. Opponent species/moves/stats are never patched.
   unsigned elapsed;
   for(elapsed=0;elapsed<24000;elapsed++){
    if((core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_RAIN) && controller(0)==SYM_HandleInputChooseAction)break;
    unsigned key=0;
    if(elapsed%8==0){
     key=2;unsigned exec=core->busRead32(core,SYM_gBattleControllerExecFlags);
     for(unsigned b=0;b<4;b+=2)if(exec&(1U<<b)){
      unsigned fn=controller(b),cursor;
      if(fn==SYM_HandleInputChooseAction){cursor=core->busRead8(core,SYM_gActionSelectionCursor+b);key=cursor&2?64:cursor&1?32:1;break;}
      if(fn==SYM_HandleInputChooseMove){cursor=core->busRead8(core,SYM_gMoveSelectionCursor+b);key=cursor&2?64:cursor&1?32:1;break;}
      if(fn==SYM_HandleInputChooseTarget){key=1;break;}
     }
    }
    frames(1,key);
   }
   CHECK(elapsed<24000);CHECK(core->busRead8(core,bmon(1)+ABI_BMON_PP)<5);CHECK(core->busRead8(core,bmon(1)+ABI_BMON_TYPE1)==TYPE_WATER);CHECK(core->busRead8(core,SYM_gBattleMonForms+1)==2);gymBallType(TYPE_WATER,1);CHECK(CALL(GetSav1Weather)==WEATHER_NONE);
   printf("PASS: actual unmodified Diana AI uses Rain Dance in%u frames; Rainy Form animation and Water Weather Ball opcode; field stays clear\n",elapsed);
   // Remaining fight has rainy Castform; finishing monitor uses its current type.
  }
  gymFinish(t);
  // Post-defeat interaction runs the original dialogue without a new battle.
  fossilStart(gymEntry[t]);fossilClose(1);CHECK(CALL(FlagGet,TRAINER_FLAGS_START+gymTrainers[t]));
 }
 CHECK(core->loadState(core,bootState));
}
