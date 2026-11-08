// Actual ROM damage, AI, saved PP and Single/Double move-script execution.
static void roarTests(void){
 wildFixture(MAP_GROUP(MAP_MT_MOON_1F),MAP_NUM(MAP_MT_MOON_1F),1);startSpecial();waitAction();damageStats();
 CHECK(baseDamage(MOVE_ROAR,0,0,0)==9);
 core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,400);CHECK(baseDamage(MOVE_ROAR,0,0,0)==9);
 core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,100);CHECK(baseDamage(MOVE_ROAR,0,0,0)==17);
 damageStats();CHECK(baseDamage(MOVE_ROAR,SIDE_STATUS_REFLECT,0,0)==9);CHECK(baseDamage(MOVE_ROAR,SIDE_STATUS_LIGHTSCREEN,0,0)==5);
 core->busWrite32(core,bmon(0)+ABI_BMON_STATUS1,STATUS1_BURN);CHECK(baseDamage(MOVE_ROAR,0,0,0)==9);
 damageStats();core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
 core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
 core->busWrite16(core,SYM_gCurrentMove,MOVE_ROAR);CALL(AI_CalcDmg,0,1);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==9);
 for(unsigned bonus=0;bonus<4;bonus++){
  CALL(SetMonMoveSlot,player(0),MOVE_ROAR,0);monput(player(0),ABI_DATA_PP_BONUSES,bonus);monput(player(0),ABI_DATA_PP1,20+4*bonus);
  CHECK(CALL(ClampApprovedMovePP,player(0)));CHECK(monget(player(0),ABI_DATA_PP1)==15+3*bonus);
 }
 daycareFixture(0);CALL(SetMonMoveSlot,player(0),MOVE_ROAR,0);monput(player(0),ABI_DATA_PP_BONUSES,3);monput(player(0),ABI_DATA_PP1,32);
 flashRoundTrip();CHECK(monget(player(0),ABI_DATA_PP1)==24);flashRoundTrip();CHECK(monget(player(0),ABI_DATA_PP1)==24);
 monput(player(0),ABI_DATA_PP1,2);flashRoundTrip();CHECK(monget(player(0),ABI_DATA_PP1)==2);
 printf("PASS: actual flash load caps legacy32PP Roar to24 with three PP Ups, preserves spent2PP and second-load identity\n");
 for(unsigned two=0;two<2;two++){
  wildFixture(two ? MAP_GROUP(MAP_MT_MOON_1F) : MAP_GROUP(MAP_ROUTE1),
               two ? MAP_NUM(MAP_MT_MOON_1F) : MAP_NUM(MAP_ROUTE1), two);
  for(unsigned p=0;p<=two;p++)for(unsigned m=0;m<4;m++)CALL(SetMonMoveSlot,player(p),p==0&&m==0?MOVE_ROAR:MOVE_SPLASH,m);
  CHECK(CALL(TryStartLandEncounter,landInfo(),0));waitAction();
  core->busWrite16(core,SYM_gBattleWeather,0);
  for(unsigned b=0;b<(two?4:2);b++){
   core->busWrite8(core,bmon(b)+ABI_BMON_ABILITY,ABILITY_NONE);
   core->busWrite8(core,bmon(b)+ABI_BMON_TYPE1,TYPE_NORMAL);core->busWrite8(core,bmon(b)+ABI_BMON_TYPE2,TYPE_NORMAL);
   for(unsigned s=0;s<ABI_BMON_STAGES_COUNT;s++)core->busWrite8(core,bmon(b)+ABI_BMON_STAGES+s,6);
   core->busWrite16(core,bmon(b)+ABI_BMON_HP,1000);core->busWrite16(core,bmon(b)+ABI_BMON_MAX_HP,1000);
   uint32_t mon=(b&1)?enemy(b>>1):player(b>>1);monput(mon,ABI_DATA_HP,1000);monput(mon,ABI_DATA_MAX_HP,1000);
   if(b&1){core->busWrite32(core,bmon(b)+ABI_BMON_STATUS1,7);monput(mon,ABI_DATA_STATUS,7);}
  }
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
  unsigned lowered=0,unchanged=0,turns=0;
  for(unsigned trial=0;trial<32&&(!lowered||!unchanged);trial++){
   CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0x6A090000+trial*7919);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);waitAction();
   CHECK(core->busRead8(core,SYM_gBattleOutcome)==0);CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000);
   unsigned stage=core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_ATK);CHECK(stage==5||stage==6);lowered+=stage==5;unchanged+=stage==6;turns++;
   CHECK(monget(player(0),ABI_DATA_PP1)==14);
   if(two){CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);CHECK(core->busRead8(core,bmon(3)+ABI_BMON_STAGES+STAT_ATK)==6);}
  }
  CHECK(lowered&&unchanged);
  for(unsigned immune=0;immune<2;immune++){
   CHECK(core->loadState(core,state));
   if(immune)core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_SOUNDPROOF);
   else{core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_GHOST);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_GHOST);}
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);waitAction();
   CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==1000);CHECK(core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_ATK)==6);
   CHECK(core->busRead8(core,SYM_gBattleOutcome)==0);CHECK(monget(player(0),ABI_DATA_PP1)==14);
  }
  CHECK(core->loadState(core,state));core->busWrite32(core,bmon(1)+ABI_BMON_STATUS1,0);monput(enemy(0),ABI_DATA_STATUS,0);
  for(unsigned m=0;m<4;m++){core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*m,MOVE_PROTECT);core->busWrite8(core,bmon(1)+ABI_BMON_PP+m,10);CALL(SetMonMoveSlot,enemy(0),MOVE_PROTECT,m);}
  chooseMove(0,0,1);if(two)chooseMove(2,0,1);waitAction();CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==1000);CHECK(monget(player(0),ABI_DATA_PP1)==14);
  free(state);
  printf("PASS: %s Roar actual animation/turn damage, %u trials include Attack-drop and no-drop; one PP, no switching/escape, Ghost/Soundproof immunity and selected target\n",two?"Double":"Single",turns);
 }
 CHECK(core->loadState(core,bootState));
}
