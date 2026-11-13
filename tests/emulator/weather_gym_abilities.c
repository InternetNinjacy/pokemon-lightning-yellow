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
 // Genuine leader scripts: Brock's reserve switch-in and Misty's two active foes.
 // Misty's approved Double must reject a party with fewer than two usable Pokemon.
 for(unsigned fainted=0;fainted<2;fainted++){
  daycareFixture(1);clockAt(12);
  for(unsigned k=1;k<6;k++)clearBytes(player(k),ABI_MON_SIZE);
  CALL(CreateMon,player(0),SPECIES_MEWTWO,100,31,1,123,0,0);
  if(fainted){CALL(CreateMon,player(1),SPECIES_MEWTWO,100,31,1,124,0,0);monput(player(1),ABI_DATA_HP,0);}
  core->busWrite8(core,SYM_gPlayerPartyCount,fainted?2:1);
  fossilWarp(MAP_GROUP(MAP_CERULEAN_CITY_GYM),MAP_NUM(MAP_CERULEAN_CITY_GYM),5,9);
  unsigned obj=CALL(GetObjectEventIdByLocalIdAndMap,3,MAP_NUM(MAP_CERULEAN_CITY_GYM),MAP_GROUP(MAP_CERULEAN_CITY_GYM));CHECK(obj<ABI_OBJECT_COUNT);core->busWrite8(core,SYM_gSelectedObjectEvent,obj);
  fossilStart(SYM_CeruleanCity_Gym_EventScript_Misty);fossilClose(1);
  CHECK(!(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_TRAINER));
  CHECK(core->busRead8(core,SYM_gBattlersCount)==0);
  CHECK(!CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_LEADER_MISTY));
  vendorTextMatches(SYM_SaffronCity_Dojo_Text_NeedTwo);
 }
 printf("PASS: Misty approved Double refuses one usable Pokemon and a fainted second without battle or defeat flag\\n");
 for(unsigned misty=0;misty<2;misty++){
  daycareFixture(1);clockAt(12);
  for(unsigned k=1;k<6;k++)clearBytes(player(k),ABI_MON_SIZE);
  core->busWrite8(core,SYM_gPlayerPartyCount,misty?2:1);
  CALL(CreateMon,player(0),SPECIES_MEWTWO,100,31,1,123,0,0);CALL(SetMonMoveSlot,player(0),MOVE_SWIFT,0);
  if(misty){CALL(CreateMon,player(1),SPECIES_MEWTWO,100,31,1,124,0,0);CALL(SetMonMoveSlot,player(1),MOVE_SWIFT,0);
   for(unsigned k=1;k<4;k++)CALL(SetMonMoveSlot,player(1),MOVE_SPLASH,k);}
  for(unsigned k=1;k<4;k++)CALL(SetMonMoveSlot,player(0),MOVE_SPLASH,k);
  unsigned map=misty?MAP_CERULEAN_CITY_GYM:MAP_PEWTER_CITY_GYM;
  fossilWarp(MAP_GROUP(map),MAP_NUM(map),misty?5:4,misty?9:10);
  unsigned obj=CALL(GetObjectEventIdByLocalIdAndMap,misty?3:1,MAP_NUM(map),MAP_GROUP(map));CHECK(obj<ABI_OBJECT_COUNT);core->busWrite8(core,SYM_gSelectedObjectEvent,obj);
  unsigned field=CALL(GetSav1Weather);
  fossilStart(misty?SYM_CeruleanCity_Gym_EventScript_Misty:SYM_PewterCity_Gym_EventScript_Brock);waitAction();
  CHECK(core->busRead16(core,SYM_gTrainerBattleOpponent_A)==(misty?TRAINER_LEADER_MISTY:TRAINER_LEADER_BROCK));
  CHECK(core->busRead16(core,bmon(1)+ABI_BMON_SPECIES)==(misty?SPECIES_STARYU:SPECIES_SNORUNT));
  if(misty){
   CHECK(core->busRead8(core,SYM_gBattlersCount)==4);
   CHECK((core->busRead32(core,SYM_gBattleTypeFlags)&(BATTLE_TYPE_TRAINER|BATTLE_TYPE_DOUBLE))==(BATTLE_TYPE_TRAINER|BATTLE_TYPE_DOUBLE));
   CHECK(monget(enemy(0),ABI_DATA_SPECIES)==SPECIES_STARYU);
   CHECK(monget(enemy(1),ABI_DATA_SPECIES)==SPECIES_STARMIE);
   CHECK(core->busRead16(core,bmon(3)+ABI_BMON_SPECIES)==SPECIES_STARMIE);
   CHECK(core->busRead8(core,bmon(3)+ABI_BMON_ABILITY)==ABILITY_DRIZZLE);
   CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_RAIN_PERMANENT);
   CHECK(CALL(GetSav1Weather)==field);
   // Both of Misty's Pokemon are already on the field; there is no reserve.
   // Advance an ordinary Double turn through both real player controllers.
   // The previous Brock test may leave the shared native move cursor in row 2.
   // Navigate through the real menu instead of assuming the cursor starts at 0.
   for(unsigned b=0;b<3;b+=2){
    waitController(b,SYM_HandleInputChooseAction);press(1);
    waitController(b,SYM_HandleInputChooseMove);
    unsigned cursor=core->busRead8(core,SYM_gMoveSelectionCursor+b);
    CHECK(cursor<4);
    if(cursor>=2)press(64); // Native UP, second row -> first row.
    cursor=core->busRead8(core,SYM_gMoveSelectionCursor+b);
    if(cursor==0)press(16); // Native RIGHT, first move -> Splash.
    CHECK(core->busRead8(core,SYM_gMoveSelectionCursor+b)==1);
    press(1);
    if(controller(b)==SYM_HandleInputChooseTarget){
     unsigned cursorTarget=core->busRead8(core,SYM_gMultiUsePlayerCursor);
     for(unsigned steps=0;steps<4&&cursorTarget!=(b?3:1);steps++){
      press(16);cursorTarget=core->busRead8(core,SYM_gMultiUsePlayerCursor);
     }
     CHECK(cursorTarget==(b?3:1));press(1);
    }
   }
   waitAction();
   CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_RAIN_PERMANENT);
   CHECK(CALL(GetSav1Weather)==field);
   CHECK(!CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_LEADER_MISTY));
   printf("PASS: actual Misty leader Double starts with Staryu/Starmie, four battlers, Starmie Drizzle, native turn and unchanged field weather\\n");
   continue;
  }
  else {
   unsigned history=core->busRead32(core,core->busRead32(core,SYM_gBattleResources)+ABI_BRES_HISTORY);
   CHECK(core->busRead8(core,history+ABI_HISTORY_COUNT)==1);CHECK(core->busRead16(core,history+ABI_HISTORY_ITEMS)==ITEM_POTION);
   CHECK(monget(enemy(0),ABI_DATA_LEVEL)==14);CHECK(monget(enemy(1),ABI_DATA_LEVEL)==16);
   const unsigned moves[2][4]={{MOVE_POWDER_SNOW,MOVE_HEADBUTT,MOVE_LEER,MOVE_NONE},{MOVE_SCREECH,MOVE_ROCK_THROW,MOVE_HARDEN,MOVE_TACKLE}};
   for(unsigned slot=0;slot<2;slot++)for(unsigned k=0;k<4;k++)CHECK(monget(enemy(slot),ABI_DATA_MOVE1+k)==moves[slot][k]);
   CHECK(monget(enemy(0),ABI_DATA_HELD_ITEM)==ITEM_NONE);CHECK(monget(enemy(1),ABI_DATA_HELD_ITEM)==ITEM_PECHA_BERRY);
   CHECK(core->busRead8(core,bmon(1)+ABI_BMON_ABILITY)==ABILITY_ICE_BODY);
   unsigned max=core->busRead16(core,bmon(1)+ABI_BMON_MAX_HP),hp=max-9;
   core->busWrite16(core,bmon(1)+ABI_BMON_HP,hp);monput(enemy(0),ABI_DATA_HP,hp);
   frames(30,0);chooseMove(0,1,1);waitAction();
   CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==hp+max/16);
  }
  fprintf(stderr,"INFO leader%u defeat lead\n",misty);frames(30,0);
  waitController(0,SYM_HandleInputChooseAction);press(1);waitController(0,SYM_HandleInputChooseMove);CHECK(core->busRead8(core,SYM_gMoveSelectionCursor)==1);press(32);CHECK(core->busRead8(core,SYM_gMoveSelectionCursor)==0);press(1);
  waitAction();
  CHECK(core->busRead16(core,bmon(1)+ABI_BMON_SPECIES)==(misty?SPECIES_STARMIE:SPECIES_ONIX));
  CHECK(core->busRead8(core,bmon(1)+ABI_BMON_ABILITY)==(misty?ABILITY_DRIZZLE:ABILITY_OVERCOAT));
  CHECK(CALL(GetSav1Weather)==field);
  if(misty)CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_RAIN_PERMANENT);
  else CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_HAIL_PERMANENT);
  unsigned reserveHp=core->busRead16(core,bmon(1)+ABI_BMON_HP);
  fprintf(stderr,"INFO leader%u reserve turn\n",misty);frames(30,0);chooseMove(0,1,1);waitAction();CHECK(CALL(GetSav1Weather)==field);
  if(misty)CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_RAIN_PERMANENT);
  else CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==reserveHp);
  printf("PASS: actual %s leader script/native approved party and reserve entry assign %s; no field-weather overwrite or blanket Cerulean Rain\n",misty?"Misty":"Brock",misty?"Starmie Drizzle":"Onix Overcoat");
 }
 CHECK(core->loadState(core,bootState));
}
