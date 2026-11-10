// Execute actual ARM helpers and native encrypted save/party data.
static void captureWaitTask(uint32_t function) {
 unsigned f;for(f=0;f<2400;f++) {if(CALL(FuncIsActiveTask,function|1))break;frames(1,0);}
 if(f==2400)fprintf(stderr,"capture shop task timeout %08x\n",function);
 CHECK(f<2400);frames(60,0);
}
static void captureShopTests(void) {
 vendorFixture();fossilWarp(MAP_GROUP(MAP_VERMILION_CITY_MART),MAP_NUM(MAP_VERMILION_CITY_MART),5,4);
 CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,20000);
 unsigned obj=CALL(GetObjectEventIdByLocalIdAndMap,1,MAP_NUM(MAP_VERMILION_CITY_MART),MAP_GROUP(MAP_VERMILION_CITY_MART));
 CHECK(obj<ABI_OBJECT_COUNT);core->busWrite8(core,SYM_gSelectedObjectEvent,obj);
 fossilStart(SYM_VermilionCity_Mart_EventScript_Clerk);captureWaitTask(SYM_Task_ShopMenu);press(1);captureWaitTask(SYM_Task_BuyMenu);
 press(1);captureWaitTask(SYM_Task_BuyHowManyDialogueHandleInput);
 for(unsigned i=0;i<9;i++)press(64);
 unsigned task=CALL(FindTaskIdByFunc,SYM_Task_BuyHowManyDialogueHandleInput|1);CHECK(task<16);
 CHECK(core->busRead16(core,SYM_gTasks+task*ABI_TASK_SIZE+ABI_TASK_DATA+2)==10);
 press(1);captureWaitTask(SYM_Task_CallYesOrNoCallback);press(1);frames(600,0);press(1);captureWaitTask(SYM_Task_BuyMenu);
 CHECK(CALL(BagGetQuantityByItemId,ITEM_POKE_BALL)==10);CHECK(CALL(BagGetQuantityByItemId,ITEM_PREMIER_BALL)==1);
 CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==18000);press(2);captureWaitTask(SYM_Task_ShopMenu);press(2);fossilClose(1);
 flashRoundTrip();CHECK(CALL(BagGetQuantityByItemId,ITEM_PREMIER_BALL)==1);CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==18000);
 printf("PASS: actual Mart buy/quantity10/confirm/return UI gives ten Poke plus one Premier, charges2000 once and survives flash\n");
}
static void captureWeatherTests(void) {
 wildFixture(MAP_GROUP(MAP_MT_MOON_1F),MAP_NUM(MAP_MT_MOON_1F),1);CALL(SetWeather,WEATHER_SNOW);frames(60,0);startSpecial();waitAction();
 CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_HAIL_PERMANENT);
 if(getenv("LIGHTNING_HAIL_GROWL")) {
  for(unsigned foe=1;foe<=3;foe+=2)for(unsigned m=0;m<4;m++) {
   core->busWrite16(core,bmon(foe)+ABI_BMON_MOVES+2*m,m?0:MOVE_GROWL);core->busWrite8(core,bmon(foe)+ABI_BMON_PP+m,m?0:40);
  }
  chooseMove(0,1,1);chooseMove(2,1,1);
  unsigned f;for(f=0;f<12000;f++){if(controller(0)==SYM_HandleInputChooseAction)break;frames(1,(f%12)<3?1:0);}
  fprintf(stderr,"Four Growl/Hail stress frames=%u script=%08x exec=%08x\n",f,core->busRead32(core,SYM_gBattlescriptCurrInstr),core->busRead32(core,SYM_gBattleControllerExecFlags));CHECK(f<12000);return;
 }
 for(unsigned foe=1;foe<=3;foe+=2)for(unsigned m=0;m<4;m++) {
  core->busWrite16(core,bmon(foe)+ABI_BMON_MOVES+2*m,m?0:MOVE_SPLASH);core->busWrite8(core,bmon(foe)+ABI_BMON_PP+m,m?0:40);
 }
 for(unsigned b=0;b<=2;b+=2) {
  monput(player(b/2),ABI_DATA_MOVE2,MOVE_SPLASH);monput(player(b/2),ABI_DATA_PP2,40);
  core->busWrite16(core,bmon(b)+ABI_BMON_MOVES+2,MOVE_SPLASH);core->busWrite8(core,bmon(b)+ABI_BMON_PP+1,40);
 }
 for(unsigned turn=0;turn<6;turn++){chooseMove(0,1,1);chooseMove(2,1,1);waitAction();CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_HAIL_PERMANENT);}
 // Native opcode replaces field Hail; current catch rule sees the changed flag.
 unsigned savedScript=core->busRead32(core,SYM_gBattlescriptCurrInstr);
 core->busWrite32(core,SYM_gBattlescriptCurrInstr,SYM_gSpeciesNames);CALL(Cmd_setsunny);
 CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_SUN);CHECK(!(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_HAIL));
 CALL(Cmd_sethail);CHECK(core->busRead16(core,SYM_gBattleWeather)==B_WEATHER_HAIL_TEMPORARY);
 CHECK(core->busRead8(core,SYM_gWishFutureKnock+ABI_WEATHER_DURATION)==5);
 core->busWrite32(core,SYM_gBattlescriptCurrInstr,savedScript);
 for(unsigned turn=0;turn<5;turn++){chooseMove(0,1,1);chooseMove(2,1,1);waitAction();}
 CHECK(!(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_HAIL));
 const unsigned abilities[]={ABILITY_DRIZZLE,ABILITY_DROUGHT,ABILITY_SAND_STREAM};
 const unsigned masks[]={B_WEATHER_RAIN_PERMANENT,B_WEATHER_SUN_PERMANENT,B_WEATHER_SANDSTORM_PERMANENT};
 void *weatherState=malloc(core->stateSize(core));CHECK(weatherState && core->saveState(core,weatherState));
 for(unsigned i=0;i<3;i++) {
  CHECK(core->loadState(core,weatherState));core->busWrite16(core,SYM_gBattleWeather,0);
  CHECK(CALL(AbilityBattleEffects,0,0,0,abilities[i],0));CHECK(core->busRead16(core,SYM_gBattleWeather)&masks[i]);
 }
 CHECK(core->loadState(core,weatherState));free(weatherState);
 core->busWrite16(core,SYM_gBattleWeather,0);CALL(Cmd_setrain);CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_RAIN_TEMPORARY);
 CALL(Cmd_setsandstorm);CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_SANDSTORM_TEMPORARY);
 CALL(CreateMon,player(0),SPECIES_CASTFORM,30,31,1,123,0,0);CALL(CopyPlayerPartyMonToBattleData,0,0);
 core->busWrite16(core,SYM_gBattleWeather,B_WEATHER_HAIL_PERMANENT);CALL(CastformDataTypeChange,0);
 CHECK(core->busRead8(core,bmon(0)+ABI_BMON_TYPE1)==TYPE_ICE);CHECK(core->busRead8(core,bmon(0)+ABI_BMON_TYPE2)==TYPE_ICE);
 printf("PASS: native Drizzle/Drought/Sand Stream, Rain Dance/Sandstorm opcodes and Forecast Ice typing under natural Hail (controlled calls, not all animation scenes)\n");
 printf("PASS: natural field Snow enters actual permanent Hail and survives six full turns; native Sunny Day replacement and temporary Hail expiry after five actual turns\n");
}
static void captureTests(void) {
 vendorFixture();
 const unsigned weather[]={0,B_WEATHER_RAIN_TEMPORARY,B_WEATHER_RAIN_PERMANENT,B_WEATHER_SUN,B_WEATHER_SANDSTORM,B_WEATHER_HAIL_TEMPORARY,B_WEATHER_HAIL_PERMANENT};
 const unsigned items[]={ITEM_POKE_BALL,ITEM_GREAT_BALL,ITEM_ULTRA_BALL,ITEM_MASTER_BALL,ITEM_SAFARI_BALL,ITEM_DIVE_BALL,ITEM_NET_BALL,ITEM_NEST_BALL,ITEM_REPEAT_BALL,ITEM_TIMER_BALL,ITEM_LUXURY_BALL,ITEM_PREMIER_BALL,ITEM_SNOW_BALL};
 for(unsigned i=0;i<13;i++)CHECK(CALL(IsPokeBallItem,items[i]));
 CHECK(!CALL(IsPokeBallItem,ITEM_POTION));CHECK(!CALL(IsPokeBallItem,0));
 for(unsigned w=0;w<7;w++)for(unsigned water=0;water<2;water++)for(unsigned cave=0;cave<2;cave++) {
  unsigned wet=water || (weather[w]&(B_WEATHER_RAIN|B_WEATHER_HAIL));
  unsigned dust=weather[w]&(B_WEATHER_SUN|B_WEATHER_SANDSTORM);
  unsigned snow=cave || (weather[w]&B_WEATHER_HAIL);
  CHECK(CALL(GetBallCatchMultiplier,ITEM_DIVE_BALL,SPECIES_PIKACHU,5,0,0,weather[w],water,cave)==(wet?30:10));
  CHECK(CALL(GetBallCatchMultiplier,ITEM_NET_BALL,SPECIES_PIKACHU,5,0,0,weather[w],water,cave)==(dust?30:10));
  CHECK(CALL(GetBallCatchMultiplier,ITEM_SNOW_BALL,SPECIES_PIKACHU,5,0,0,weather[w],water,cave)==(snow?30:10));
 }
 const unsigned legends[]={SPECIES_ARTICUNO,SPECIES_ZAPDOS,SPECIES_MOLTRES,SPECIES_RAIKOU,SPECIES_ENTEI,SPECIES_SUICUNE,SPECIES_MEWTWO,SPECIES_MEW};
 for(unsigned i=0;i<8;i++)CHECK(CALL(GetBallCatchMultiplier,ITEM_PREMIER_BALL,legends[i],70,0,0,0,0,0)==35);
 CHECK(CALL(GetBallCatchMultiplier,ITEM_PREMIER_BALL,SPECIES_DRAGONITE,70,0,0,0,0,0)==10);
 for(unsigned n=1;n<=100;n++)CHECK(CALL(GetBallCatchMultiplier,ITEM_NEST_BALL,SPECIES_PIKACHU,n,0,0,0,0,0)==(n<30?40-n:10));
 for(unsigned n=0;n<100;n++)CHECK(CALL(GetBallCatchMultiplier,ITEM_TIMER_BALL,SPECIES_PIKACHU,5,n,0,0,0,0)==(n<30?n+10:40));
 for(unsigned i=0;i<13;i++) {
  unsigned code=CALL(BallItemToCaptureCode,items[i]);CHECK(code==(items[i]==ITEM_SNOW_BALL?13:items[i]));CHECK(CALL(CaptureCodeToBallItem,code)==items[i]);
  core->busWrite32(core,scratch,code);CALL(SetMonData,player(0),ABI_DATA_POKEBALL,scratch);CHECK(monget(player(0),ABI_DATA_POKEBALL)==code);
 }
 CHECK(CALL(ItemIdToBallId,ITEM_SNOW_BALL)==CALL(ItemIdToBallId,ITEM_DIVE_BALL));
 flashRoundTrip();CHECK(monget(player(0),ABI_DATA_POKEBALL)==13);
 CHECK(CALL(AddBagItem,ITEM_POTION,7));CHECK(CALL(AddBagItem,ITEM_SNOW_BALL,3));flashRoundTrip();
 CHECK(CALL(CheckBagHasItem,ITEM_POTION,7));CHECK(CALL(CheckBagHasItem,ITEM_SNOW_BALL,3));
 // Each purchase type has the same independent quantity rule.
 for(unsigned i=0;i<13;i++) {
  if(items[i]==ITEM_MASTER_BALL||items[i]==ITEM_SAFARI_BALL||items[i]==ITEM_PREMIER_BALL)continue;
  const unsigned quantity[]={1,9,10,19,20,30,99};
  for(unsigned q=0;q<7;q++) {
   vendorFixture();CHECK(CALL(TryBuyBallsWithPremierBonus,items[i],quantity[q]));
   CHECK(CALL(CheckBagHasItem,items[i],quantity[q]));
   CHECK(CALL(CheckBagHasItem,ITEM_PREMIER_BALL,quantity[q]/10+1)==0);
   if(quantity[q]/10)CHECK(CALL(CheckBagHasItem,ITEM_PREMIER_BALL,quantity[q]/10));
  }
 }
 vendorFixture();CHECK(CALL(TryBuyBallsWithPremierBonus,ITEM_POKE_BALL,5));CHECK(CALL(TryBuyBallsWithPremierBonus,ITEM_GREAT_BALL,5));CHECK(!CALL(CheckBagHasItem,ITEM_PREMIER_BALL,1));
 CHECK(CALL(AddBagItem,ITEM_PREMIER_BALL,999));CHECK(!CALL(TryBuyBallsWithPremierBonus,ITEM_SNOW_BALL,10));CHECK(!CALL(CheckBagHasItem,ITEM_SNOW_BALL,1));
 printf("PASS: native ball weather/origin/cave matrix, whitelist, original Nest/Timer, all13 capture codes, Snow/Potion flash persistence, purchase bonuses/overflow and no mixed pooling\n");
 // Real Snow bag guard, throw/capture and four-bit identity on both survivor sides.
 for(unsigned variant=0;variant<3;variant++) {
  unsigned killed=variant==1?3:1;
  wildFixture(MAP_GROUP(MAP_MT_MOON_1F),MAP_NUM(MAP_MT_MOON_1F),1);
  if(variant==2) {
   CALL(ResetPokemonStorageSystem);
   for(unsigned slot=2;slot<6;slot++)CALL(CreateMon,player(slot),SPECIES_PIKACHU,20,31,1,slot,0,0);
   core->busWrite8(core,SYM_gPlayerPartyCount,6);
  }
  CHECK(CALL(AddBagItem,ITEM_SNOW_BALL,2));startSpecial();waitAction();openBagUse();
  CHECK(CALL(BagGetQuantityByItemId,ITEM_SNOW_BALL)==2);
  for(unsigned n=0;n<80 && controller(0)!=SYM_HandleInputChooseAction;n++){press(2);frames(6,0);}
  waitController(0,SYM_HandleInputChooseAction);if(core->busRead8(core,SYM_gActionSelectionCursor)&1)press(32);
  // Cloyster knows priority Protect at this level. Control the opponents'
  // move choices so this capture-path fixture does not depend on Protect RNG.
  for(unsigned foe=1;foe<=3;foe+=2)for(unsigned m=0;m<4;m++) {
   core->busWrite16(core,bmon(foe)+ABI_BMON_MOVES+2*m,m?0:MOVE_GROWL);
   core->busWrite8(core,bmon(foe)+ABI_BMON_PP+m,m?0:40);
  }
  killOne(killed);unsigned target=killed^2;
  core->busWrite16(core,bmon(target)+ABI_BMON_HP,1);core->busWrite32(core,bmon(target)+ABI_BMON_STATUS1,1);
  openBagUse();
  if(killed==1) {
   waitController(2,SYM_HandleInputChooseAction);press(2);waitController(0,SYM_HandleInputChooseAction);
   CHECK(CALL(BagGetQuantityByItemId,ITEM_SNOW_BALL)==2);
   if(core->busRead8(core,SYM_gActionSelectionCursor)&1)press(32);
   openBagUse();
  }
  chooseMove(2,1,target);waitOverworld(2);
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_CAUGHT);
  if(variant<2)CHECK(monget(player(2),ABI_DATA_POKEBALL)==13);
  else {CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_POKEBALL)==13);CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_SPECIES)==SPECIES_CLOYSTER);}
  CHECK(CALL(BagGetQuantityByItemId,ITEM_SNOW_BALL)==1);flashRoundTrip();
  if(variant<2)CHECK(monget(player(2),ABI_DATA_POKEBALL)==13);else CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_POKEBALL)==13);
 }
 printf("PASS: real Snow guard/throw/shared animation, both survivor captures and full-party PC delivery, exact consumption, stored code13 and flash reload\n");
 captureShopTests();
 captureWeatherTests();
 // Exercise real Oak gift helper with a full Nest stack, then one-time retry.
 vendorFixture();fossilWarp(MAP_GROUP(MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB),MAP_NUM(MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB),6,5);
 CALL(AddBagItem,ITEM_NEST_BALL,999);CALL(FlagSet,FLAG_OAK_NEST_BALLS_PENDING);
 fossilStart(SYM_PalletTown_ProfessorOaksLab_EventScript_ProfOak);fossilClose(1);CHECK(CALL(FlagGet,FLAG_OAK_NEST_BALLS_PENDING));flashRoundTrip();CHECK(CALL(FlagGet,FLAG_OAK_NEST_BALLS_PENDING));
 CALL(RemoveBagItem,ITEM_NEST_BALL,999);fossilStart(SYM_PalletTown_ProfessorOaksLab_EventScript_ProfOak);fossilClose(1);
 CHECK(!CALL(FlagGet,FLAG_OAK_NEST_BALLS_PENDING));CHECK(CALL(CheckBagHasItem,ITEM_NEST_BALL,5));CHECK(!CALL(CheckBagHasItem,ITEM_NEST_BALL,6));flashRoundTrip();CHECK(!CALL(FlagGet,FLAG_OAK_NEST_BALLS_PENDING));
 printf("PASS: actual Oak NPC full-bag pending gift and saved retry of exactly five Nest Balls\n");
}
