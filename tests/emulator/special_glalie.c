// Opt-in saved individual ability. Fixtures do not install an encounter or its level.
static void specialGlalieTests(void)
{
 daycareFixture(1);CALL(ResetPokemonStorageSystem);
 for(unsigned pid=0;pid<24;pid++){
  CALL(CreateMon,player(0),SPECIES_GLALIE,30,31,1,pid,1,0x12345678);
  unsigned char original[ABI_MON_SIZE];readBytes(player(0),original,sizeof(original));
  CHECK(CALL(GetMonAbility,player(0))!=ABILITY_SNOW_WARNING);sameBytes(player(0),original,sizeof(original));
  CHECK(!CALL(SetSpecialEncounterAbility,player(0),ABILITY_DRIZZLE));sameBytes(player(0),original,sizeof(original));
  CHECK(CALL(SetSpecialEncounterAbility,player(0),ABILITY_SNOW_WARNING));
  CHECK(CALL(GetMonAbility,player(0))==ABILITY_SNOW_WARNING);
  unsigned char marked[ABI_MON_SIZE];readBytes(player(0),marked,sizeof(marked));unsigned changed=0;
  for(unsigned b=0;b<sizeof(marked);b++)changed+=marked[b]!=original[b];
  CHECK(changed>0&&changed<=4);CHECK(monget(player(0),MON_DATA_IVS)==0x3FFFFFFF);
  CHECK(monget(player(0),ABI_DATA_PERSONALITY)==pid);CHECK(monget(player(0),ABI_DATA_OT_ID)==0x12345678);
  CHECK(!monget(player(0),ABI_DATA_SANITY_IS_BAD_EGG));CHECK(monget(player(0),ABI_DATA_LEVEL)==30);
  CALL(SetBoxMonAt,0,pid,player(0));CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,pid),enemy(0));
  CHECK(CALL(GetMonAbility,enemy(0))==ABILITY_SNOW_WARNING);CHECK(monget(enemy(0),ABI_DATA_PERSONALITY)==pid);
 }
 flashRoundTrip();
 for(unsigned pid=0;pid<24;pid++){
  CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,pid),player(0));
  CHECK(CALL(GetMonAbility,player(0))==ABILITY_SNOW_WARNING);CHECK(monget(player(0),ABI_DATA_PERSONALITY)==pid);
  CHECK(monget(player(0),ABI_DATA_OT_ID)==0x12345678);CHECK(!monget(player(0),ABI_DATA_SANITY_IS_BAD_EGG));
  CALL(CopyPlayerPartyMonToBattleData,0,0);CHECK(core->busRead8(core,bmon(0)+ABI_BMON_ABILITY)==ABILITY_SNOW_WARNING);
 }
 CHECK(CALL(SetSpecialEncounterAbility,player(0),ABILITY_NONE));CHECK(CALL(GetMonAbility,player(0))!=ABILITY_SNOW_WARNING);
 CALL(CreateMon,player(0),SPECIES_CORPHISH,30,31,1,1,0,0);unsigned char prior[ABI_MON_SIZE];readBytes(player(0),prior,sizeof(prior));
 CHECK(!CALL(SetSpecialEncounterAbility,player(0),ABILITY_SNOW_WARNING));sameBytes(player(0),prior,sizeof(prior));
 CHECK(CALL(SetSpecialEncounterAbility,player(0),ABILITY_DRIZZLE));CHECK(CALL(GetMonAbility,player(0))==ABILITY_DRIZZLE);
 CALL(SetBoxMonAt,0,26,player(0));flashRoundTrip();CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,26),player(0));
 CHECK(CALL(GetMonAbility,player(0))==ABILITY_DRIZZLE);
 monput(player(0),ABI_DATA_SPECIES,SPECIES_CRAWDAUNT);
 CHECK(CALL(GetMonAbility,player(0))==ABILITY_DRIZZLE);
 CALL(SetBoxMonAt,0,26,player(0));flashRoundTrip();CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,26),player(0));
 CHECK(CALL(GetMonAbility,player(0))==ABILITY_DRIZZLE);
 CALL(CreateMon,player(0),SPECIES_CRAWDAUNT,30,31,1,2,0,0);
 CHECK(CALL(GetMonAbility,player(0))!=ABILITY_DRIZZLE);
 CHECK(!CALL(SetSpecialEncounterAbility,player(0),ABILITY_DRIZZLE));
 CALL(CreateMon,player(0),SPECIES_GLALIE,30,31,1,0,0,0);monput(player(0),ABI_DATA_IS_EGG,1);CHECK(!CALL(SetSpecialEncounterAbility,player(0),ABILITY_SNOW_WARNING));
 printf("PASS: native Glalie-only opt-in tag across all24 encrypted substructure orders, unchanged legacy/invalid inputs, exact PID/OT, PC copy/flash/withdrawal and player battle data; no Corphish inheritance invented\n");
 // Real capture into party and full-party PC, using the actual Bag/Master Ball path.
 for(unsigned kind=0;kind<3;kind++)for(unsigned pc=0;pc<2;pc++){
  unsigned species=kind==0?SPECIES_TYRANITAR:kind==1?SPECIES_CORPHISH:SPECIES_GLALIE;
  unsigned ability=kind==0?ABILITY_SAND_STREAM:kind==1?ABILITY_DRIZZLE:ABILITY_SNOW_WARNING;
  wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),0);CALL(ResetPokemonStorageSystem);
  if(pc){for(unsigned i=1;i<6;i++)CALL(CreateMon,player(i),SPECIES_PIKACHU,20,31,1,i,0,0);core->busWrite8(core,SYM_gPlayerPartyCount,6);}
  CALL(CreateMon,enemy(0),species,30,31,1,1234,0,0);
  if(kind)CHECK(CALL(SetSpecialEncounterAbility,enemy(0),ability));
  else CHECK(CALL(GetMonAbility,enemy(0))==ABILITY_SAND_STREAM);CHECK(CALL(AddBagItem,ITEM_MASTER_BALL,1));
  unsigned field=CALL(GetSav1Weather);CALL(StartWildBattle);waitAction();frames(30,0);
  CHECK(core->busRead8(core,bmon(1)+ABI_BMON_ABILITY)==ability);CHECK(core->busRead16(core,SYM_gBattleWeather)&(kind==0?B_WEATHER_SANDSTORM_PERMANENT:kind==1?B_WEATHER_RAIN_PERMANENT:B_WEATHER_HAIL_PERMANENT));
  openBagUse();waitOverworld(2);CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_CAUGHT);
  unsigned caught=pc?enemy(0):player(1);if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),caught);
  if(monget(caught,ABI_DATA_SPECIES)!=species){fprintf(stderr,"Glalie capture pc%u partyCount%u species:",pc,core->busRead8(core,SYM_gPlayerPartyCount));for(unsigned i=0;i<6;i++)fprintf(stderr," %u",monget(player(i),ABI_DATA_SPECIES));fprintf(stderr," box%u enemy%u\n",CALL(GetBoxMonDataAt,0,0,ABI_DATA_SPECIES),monget(enemy(0),ABI_DATA_SPECIES));}
  CHECK(monget(caught,ABI_DATA_SPECIES)==species);CHECK(CALL(GetMonAbility,caught)==ability);
  unsigned pid=monget(caught,ABI_DATA_PERSONALITY),ot=monget(caught,ABI_DATA_OT_ID);
  CHECK(CALL(GetSav1Weather)==field);flashRoundTrip();
  if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),caught);
  CHECK(CALL(GetMonAbility,caught)==ability);CHECK(monget(caught,ABI_DATA_PERSONALITY)==pid);CHECK(monget(caught,ABI_DATA_OT_ID)==ot);
  printf("PASS: actual species%u ability%u entry/animation, Master Ball capture to %s, field restoration, native flash reload and retained ability/identity; test-only level/location\n",species,ability,pc?"PC":"party");
 }
 CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),player(0));
 unsigned field=CALL(GetSav1Weather);CALL(CreateMon,enemy(0),SPECIES_RATTATA,5,31,1,99,0,0);
 CALL(StartWildBattle);waitAction();frames(30,0);
 CHECK(core->busRead8(core,bmon(0)+ABI_BMON_ABILITY)==ABILITY_SNOW_WARNING);CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_HAIL_PERMANENT);
 CHECK(CALL(GetSav1Weather)==field);weatherRun();waitOverworld(2);CHECK(CALL(GetMonAbility,player(0))==ABILITY_SNOW_WARNING);CHECK(CALL(GetSav1Weather)==field);
 printf("PASS: captured PC Glalie withdrawn to player party starts Snow Warning in a subsequent real battle; Run restores field without changing its saved ability\n");
 CHECK(core->loadState(core,bootState));
}
