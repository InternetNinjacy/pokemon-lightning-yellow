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
  if(misty){CALL(CreateMon,player(1),SPECIES_MEWTWO,100,31,1,124,0,0);
   // All four legal moves are Splash: whichever native cursor slot is selected,
   // the turn cannot prematurely defeat either of Misty's starting Pokemon.
   for(unsigned k=0;k<4;k++){CALL(SetMonMoveSlot,player(0),MOVE_SPLASH,k);CALL(SetMonMoveSlot,player(1),MOVE_SPLASH,k);}
  }else for(unsigned k=1;k<4;k++)CALL(SetMonMoveSlot,player(0),MOVE_SPLASH,k);
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
   // Both Misty Pokemon lead the approved Double: there is no reserve.
   // Dispatch native input only for the active player controller, as the
   // existing gym_castform.c Double driver does. Waiting unconditionally
   // for controller 2 stalls if controller 0 still owns an input callback.
   unsigned pp0=core->busRead8(core,bmon(0)+ABI_BMON_PP);
   unsigned pp2=core->busRead8(core,bmon(2)+ABI_BMON_PP);
   CHECK(pp0>0);CHECK(pp2>0);
   unsigned seen0=0,seen2=0,turnFrames;
   for(turnFrames=0;turnFrames<24000;turnFrames++){
    if(core->busRead8(core,bmon(0)+ABI_BMON_PP)<pp0
       &&core->busRead8(core,bmon(2)+ABI_BMON_PP)<pp2
       &&controller(0)==SYM_HandleInputChooseAction)break;
    unsigned key=0;
    if(turnFrames%8==0){
     key=2;
     unsigned exec=core->busRead32(core,SYM_gBattleControllerExecFlags);
     for(unsigned b=0;b<4;b+=2)if(exec&(1U<<b)){
      unsigned fn=controller(b),cursor;
      if(fn==SYM_HandleInputChooseAction){
       cursor=core->busRead8(core,SYM_gActionSelectionCursor+b);
       CHECK(cursor<4);
       key=cursor&2?64:cursor&1?32:1;break;
      }
      if(fn==SYM_HandleInputChooseMove){
       cursor=core->busRead8(core,SYM_gMoveSelectionCursor+b);
       CHECK(cursor<4);
       CHECK(core->busRead16(core,bmon(b)+ABI_BMON_MOVES+2*cursor)==MOVE_SPLASH);
       if(b==0)seen0=1;else seen2=1;
       key=1;break;
      }
      if(fn==SYM_HandleInputChooseTarget){key=1;break;}
     }
    }
    frames(1,key);
   }
   if(turnFrames==24000)fprintf(stderr,"Misty turn timeout: exec=%08x ctrl0=%08x ctrl2=%08x action0=%u action2=%u move0=%u move2=%u pp0=%u/%u pp2=%u/%u outcome=%u cb2=%08x seen=%u/%u\\n",core->busRead32(core,SYM_gBattleControllerExecFlags),controller(0),controller(2),core->busRead8(core,SYM_gActionSelectionCursor),core->busRead8(core,SYM_gActionSelectionCursor+2),core->busRead8(core,SYM_gMoveSelectionCursor),core->busRead8(core,SYM_gMoveSelectionCursor+2),core->busRead8(core,bmon(0)+ABI_BMON_PP),pp0,core->busRead8(core,bmon(2)+ABI_BMON_PP),pp2,core->busRead8(core,SYM_gBattleOutcome),core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),seen0,seen2);
   CHECK(turnFrames<24000);
   CHECK(seen0&&seen2);
   CHECK(core->busRead8(core,bmon(0)+ABI_BMON_PP)<pp0);
   CHECK(core->busRead8(core,bmon(2)+ABI_BMON_PP)<pp2);
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
