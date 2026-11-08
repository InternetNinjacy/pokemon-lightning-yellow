#include "constants/opponents.h"
// Approved individual Trainer abilities, without altering any party or species data.
static void weatherGymAbilityTests(void)
{
 const unsigned trainers[]={TRAINER_LEADER_BROCK,TRAINER_LEADER_BROCK,TRAINER_LEADER_MISTY};
 const unsigned species[]={SPECIES_SNORUNT,SPECIES_ONIX,SPECIES_STARMIE};
 const unsigned expected[]={ABILITY_ICE_BODY,ABILITY_OVERCOAT,ABILITY_DRIZZLE};
 for(unsigned i=0;i<3;i++){
  wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),0);CHECK(CALL(TryStartLandEncounter,landInfo(),0));waitAction();
  core->busWrite16(core,SYM_gTrainerBattleOpponent_A,trainers[i]);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPECIES,species[i]);
  core->busWrite32(core,SYM_gBattleTypeFlags,BATTLE_TYPE_TRAINER);
  CHECK(CALL(GetBattlerInitialAbility,1)==expected[i]);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPECIES,species[i]);CHECK(CALL(GetBattlerInitialAbility,0)!=expected[i]);
  const unsigned excluded[]={BATTLE_TYPE_LINK,BATTLE_TYPE_EREADER_TRAINER,BATTLE_TYPE_TRAINER_TOWER};
  for(unsigned k=0;k<3;k++){core->busWrite32(core,SYM_gBattleTypeFlags,BATTLE_TYPE_TRAINER|excluded[k]);CHECK(CALL(GetBattlerInitialAbility,1)!=expected[i]);}
  core->busWrite32(core,SYM_gBattleTypeFlags,0);CHECK(CALL(GetBattlerInitialAbility,1)!=expected[i]);
  core->busWrite32(core,SYM_gBattleTypeFlags,BATTLE_TYPE_TRAINER);core->busWrite16(core,SYM_gTrainerBattleOpponent_A,TRAINER_CAMPER_LIAM);CHECK(CALL(GetBattlerInitialAbility,1)!=expected[i]);
 }
 // Genuine current leader scripts and reserve switch-in; no fixture Trainer teams.
 for(unsigned misty=0;misty<2;misty++){
  daycareFixture(1);clockAt(12);
  for(unsigned k=1;k<6;k++)clearBytes(player(k),ABI_MON_SIZE);
  core->busWrite8(core,SYM_gPlayerPartyCount,1);
  CALL(CreateMon,player(0),SPECIES_MEWTWO,100,31,1,123,0,0);CALL(SetMonMoveSlot,player(0),MOVE_SWIFT,0);
  for(unsigned k=1;k<4;k++)CALL(SetMonMoveSlot,player(0),MOVE_SPLASH,k);
  unsigned map=misty?MAP_CERULEAN_CITY_GYM:MAP_PEWTER_CITY_GYM;
  fossilWarp(MAP_GROUP(map),MAP_NUM(map),misty?5:4,misty?9:10);
  unsigned obj=CALL(GetObjectEventIdByLocalIdAndMap,misty?3:1,MAP_NUM(map),MAP_GROUP(map));CHECK(obj<ABI_OBJECT_COUNT);core->busWrite8(core,SYM_gSelectedObjectEvent,obj);
  unsigned field=CALL(GetSav1Weather);
  fossilStart(misty?SYM_CeruleanCity_Gym_EventScript_Misty:SYM_PewterCity_Gym_EventScript_Brock);waitAction();
  CHECK(core->busRead16(core,SYM_gTrainerBattleOpponent_A)==(misty?TRAINER_LEADER_MISTY:TRAINER_LEADER_BROCK));
  CHECK(core->busRead16(core,bmon(1)+ABI_BMON_SPECIES)==(misty?SPECIES_STARYU:SPECIES_GEODUDE));
  if(misty)CHECK(!(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_RAIN));
  fprintf(stderr,"INFO leader%u defeat lead\n",misty);frames(30,0);chooseMove(0,0,1);waitAction();
  CHECK(core->busRead16(core,bmon(1)+ABI_BMON_SPECIES)==(misty?SPECIES_STARMIE:SPECIES_ONIX));
  CHECK(core->busRead8(core,bmon(1)+ABI_BMON_ABILITY)==(misty?ABILITY_DRIZZLE:ABILITY_OVERCOAT));
  CHECK(CALL(GetSav1Weather)==field);
  if(misty)CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_RAIN_PERMANENT);
  else CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_HAIL_PERMANENT);
  unsigned reserveHp=core->busRead16(core,bmon(1)+ABI_BMON_HP);
  fprintf(stderr,"INFO leader%u reserve turn\n",misty);frames(30,0);chooseMove(0,1,1);waitAction();CHECK(CALL(GetSav1Weather)==field);
  if(misty)CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_RAIN_PERMANENT);
  else CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==reserveHp);
  printf("PASS: actual %s leader script/native original party and reserve entry assign %s; no field-weather overwrite or blanket Cerulean Rain\n",misty?"Misty":"Brock",misty?"Starmie Drizzle":"Onix Overcoat");
 }
 CHECK(core->loadState(core,bootState));
}
