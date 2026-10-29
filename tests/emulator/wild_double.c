// Wild-double integration fixtures. Included by runner.c to share guarded ARM calls.
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/vars.h"
#include "constants/flags.h"
#include "constants/weather.h"

static uint32_t player(unsigned slot) {return SYM_gPlayerParty+slot*ABI_MON_SIZE;}
static uint32_t enemy(unsigned slot) {return SYM_gEnemyParty+slot*ABI_MON_SIZE;}
static uint32_t bmon(unsigned slot) {return SYM_gBattleMons+slot*ABI_BMON_SIZE;}
static uint32_t monget(uint32_t mon,unsigned field){return CALL(GetMonData3,mon,field,0);}
static void monput(uint32_t mon,unsigned field,unsigned value){core->busWrite32(core,scratch,value);CALL(SetMonData,mon,field,scratch);}
static unsigned controller(unsigned b){return core->busRead32(core,SYM_gBattlerControllerFuncs+b*4)&~1U;}
static void screenshot(const char *path){
 if(!path)return;
 const void *video;size_t stride;core->getPixels(core,&video,&stride);
 FILE *file=fopen(path,"wb");CHECK(file);fprintf(file,"P6\n240 160\n255\n");
 for(unsigned y=0;y<160;y++)for(unsigned x=0;x<240;x++){color_t p=((const color_t*)video)[y*stride+x];fputc(p&255,file);fputc((p>>8)&255,file);fputc((p>>16)&255,file);}
 fclose(file);
}
static void wildFixture(unsigned mapGroup,unsigned mapNum,unsigned two){
 CHECK(core->loadState(core,bootState));initSave();
 core->busWrite8(core,SYM_gSaveBlock2+ABI_SAVE2_PLAYER_NAME,0xCA);
 core->busWrite8(core,SYM_gSaveBlock2+ABI_SAVE2_PLAYER_NAME+1,0xFF);
 CALL(ResetTasks);CALL(ZeroPlayerPartyMons);
 CALL(FlagSet,FLAG_SYS_POKEMON_GET);CALL(FlagSet,FLAG_SYS_POKEDEX_GET);
 CALL(CreateMon,player(0),SPECIES_PIKACHU,50,31,1,12345,0,1234);
 if(two)CALL(CreateMon,player(1),SPECIES_BLASTOISE,50,31,1,12346,0,1234);
 core->busWrite8(core,SYM_gPlayerPartyCount,two?2:1);
 for(unsigned i=0;i<two+1 && i<2;i++){
  monput(player(i),ABI_DATA_MOVE1,MOVE_DRAGON_RAGE);monput(player(i),ABI_DATA_PP1,35);
  monput(player(i),ABI_DATA_MOVE2,MOVE_GROWL);monput(player(i),ABI_DATA_PP2,40);
  monput(player(i),ABI_DATA_MOVE3,0);monput(player(i),ABI_DATA_MOVE4,0);
 }
 CALL(SetWarpDestination,mapGroup,mapNum,0xFFFFFFFF,mapGroup==MAP_GROUP(MAP_MT_MOON_1F)?18:9,mapGroup==MAP_GROUP(MAP_MT_MOON_1F)?36:30);
 CALL(WarpIntoMap);CALL(ResetInitialPlayerAvatarState);
 core->busWrite32(core,SYM_gMain,0);CALL(CB2_LoadMap);
 frames(360,0);
 if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)!=SYM_CB2_Overworld){fprintf(stderr,"map-load callback=%08x\n",core->busRead32(core,SYM_gMain+ABI_MAIN_CB2));screenshot("/tmp/wild-map-timeout.ppm");}
 CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
 printf("PASS: actual overworld map loaded %u.%u with %u usable Pokemon\n",mapGroup,mapNum,two?2:1);
}
static uint32_t landInfo(void){
 unsigned header=CALL(GetCurrentMapWildMonHeaderId);CHECK(header!=65535);
 uint32_t info=core->busRead32(core,SYM_gWildMonHeaders+header*ABI_WILD_HEADER_SIZE+ABI_WILD_HEADER_LAND);CHECK(info>=0x08000000);return info;
}
static void waitAction(void){
 unsigned i;
 for(i=0;i<2400;i++){
  if(controller(0)==SYM_HandleInputChooseAction)break;
  frames(1,(i%12)<3?1:0);
 }
 if(i==2400)fprintf(stderr,"Battle action timeout cb=%08x battleflags=%08x ctrl=%08x/%08x/%08x/%08x battlers=%u outcome=%u\n",core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),core->busRead32(core,SYM_gBattleTypeFlags),controller(0),controller(1),controller(2),controller(3),core->busRead8(core,SYM_gBattlersCount),core->busRead8(core,SYM_gBattleOutcome));
 CHECK(i<2400);frames(3,0);
}

static void press(unsigned key){frames(3,key);frames(3,0);}
static void waitController(unsigned b,unsigned function){
 unsigned i;
 for(i=0;i<600;i++){if(controller(b)==function)break;frames(1,0);}
 if(i==600)fprintf(stderr,"controller %u waiting for %08x found %08x\n",b,function,controller(b));
 CHECK(i<600);
}
static void chooseMove(unsigned b,unsigned moveIndex,unsigned target){
 waitController(b,SYM_HandleInputChooseAction);press(1);
 waitController(b,SYM_HandleInputChooseMove);
 // Move cursor starts at 0 for these fixtures. Position it through actual D-pad input.
 unsigned cursor=core->busRead8(core,SYM_gMoveSelectionCursor+b);
 if(cursor!=moveIndex)press(16); // RIGHT toggles between first two moves
 CHECK(core->busRead8(core,SYM_gMoveSelectionCursor+b)==moveIndex);
 press(1);
 if(controller(b)==SYM_HandleInputChooseTarget){
  // Full backward cycles exercise the repaired original target-wrap bounds.
  for(unsigned n=0;n<6;n++){press(32);CHECK(core->busRead8(core,SYM_gMultiUsePlayerCursor)<4);}
  for(unsigned n=0;n<6 && core->busRead8(core,SYM_gMultiUsePlayerCursor)!=target;n++)press(16);
  CHECK(core->busRead8(core,SYM_gMultiUsePlayerCursor)==target);
  press(1);
 }
}
static void killOne(unsigned target){
 chooseMove(0,0,target);chooseMove(2,1,target);waitAction();
 CHECK(core->busRead16(core,bmon(target)+ABI_BMON_HP)==0);
 CHECK(CALL(GetWildOpponentCount)==1);
 printf("PASS: selected opposing battler %u, played damage/faint animations; other opponent remains\n",target);
}
static void openBagUse(void){
 waitController(0,SYM_HandleInputChooseAction);
 core->busWrite16(core,SYM_gBagMenuState+ABI_BAG_POCKET,2); // fixture: start on BALLS pocket
 press(16); // Fight -> Bag
 press(1);frames(120,0);
 press(1);frames(30,0); // choose Master Ball, open context
 press(1);frames(30,0); // USE (runs the production item callback)
}
static void waitOverworld(unsigned keys){
 unsigned i;
 for(i=0;i<3600;i++){
  if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld)break;
  frames(1,(i%12)<3?keys:0);
 }
 if(i==3600)fprintf(stderr,"overworld timeout cb=%08x ctrl0=%08x script=%08x outcome=%u\n",core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),controller(0),core->busRead32(core,SYM_gBattlescriptCurrInstr),core->busRead8(core,SYM_gBattleOutcome));
 if(i==3600)screenshot("/tmp/wild-double-timeout.ppm");
 CHECK(i<3600);frames(60,0);
}
static void startSpecial(void){
 // Invoke the natural encounter trigger on packed attributes from an actual
 // ordinary land tile. This exercises its rate/Repel/roamer dispatch, without
 // a scripted event or writing either enemy species.
 uint32_t attrs=0,behavior=0;unsigned found=0;
 for(unsigned y=8;y<45&&!found;y++)for(unsigned x=8;x<24;x++)
  if(CALL(MapGridGetMetatileAttributeAt,x,y,ABI_META_ENCOUNTER)==ABI_TILE_LAND){
   attrs=CALL(MapGridGetMetatileAttributeAt,x,y,ABI_META_ALL);
   behavior=CALL(MapGridGetMetatileAttributeAt,x,y,ABI_META_BEHAVIOR);
   found=1;break;
  }
 CHECK(found);
 for(unsigned i=0;i<5000;i++){
  if(!CALL(StandardWildEncounter,attrs,behavior))continue;
  if(monget(enemy(0),ABI_DATA_SPECIES)==SPECIES_ONIX && monget(enemy(1),ABI_DATA_SPECIES)==SPECIES_CLOYSTER)return;
  CALL(ResetTasks); // discard only the unexecuted trial's transition task
 }
 CHECK(0);
}

static void warpGrass(void){
 unsigned x,y,found=0;
 for(y=8;y<45&&!found;y++)for(x=8;x<24;x++)
  if(CALL(MapGridGetMetatileAttributeAt,x,y,ABI_META_ENCOUNTER)==ABI_TILE_LAND
   && CALL(MapGridGetMetatileAttributeAt,x,y+1,ABI_META_ENCOUNTER)==ABI_TILE_LAND){found=1;break;}
 CHECK(found);y--;
 CALL(SetWarpDestination,MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),0xFFFFFFFF,x-7,y-7);
 CALL(WarpIntoMap);CALL(CB2_LoadMap);frames(180,0);
 CALL(PlayerGetDestCoords,scratch,scratch+2);
 CHECK(CALL(MapGridGetMetatileAttributeAt,core->busRead16(core,scratch),core->busRead16(core,scratch+2),ABI_META_ENCOUNTER)==ABI_TILE_LAND);
}
static void walkToBattle(void){
 // Walk back and forth between adjacent ordinary grass tiles. No encounter call.
 unsigned f;
 for(f=0;f<3600;f++){
  if(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_DOUBLE)break;
  frames(1,((f/24)%2)?64:128);
 }
 CHECK(f<3600);core->setKeys(core,0);waitAction();
 CHECK(core->busRead8(core,SYM_gBattlersCount)==4);
 printf("PASS: actual directional walking triggered wild double after %u frames on ordinary grass\n",f);
}
static void generationTests(void){
 wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);
 uint32_t info=landInfo();unsigned special=0,duplicates=0,firstP=0,secondP=0;
 core->busWrite32(core,SYM_gRngValue,0x790abcde);
 for(unsigned i=0;i<1000;i++){
  CHECK(CALL(TryStartLandEncounter,info,1));
  unsigned a=monget(enemy(0),ABI_DATA_SPECIES),b=monget(enemy(1),ABI_DATA_SPECIES);
  if(a==SPECIES_ONIX&&b==SPECIES_CLOYSTER)special++;
  else{
   CHECK(a==SPECIES_PIDGEY||a==SPECIES_RATTATA);CHECK(b==SPECIES_PIDGEY||b==SPECIES_RATTATA);
   firstP+=a==SPECIES_PIDGEY;secondP+=b==SPECIES_PIDGEY;duplicates+=a==b;
  }
  for(unsigned j=0;j<2;j++){unsigned level=monget(enemy(j),ABI_DATA_LEVEL);CHECK(level>=2&&level<=5);}
  CHECK(monget(enemy(2),ABI_DATA_SPECIES)==0);
  CALL(ResetTasks);
 }
 CHECK(special>=25&&special<=75);CHECK(duplicates>300);
 CHECK(firstP>400&&firstP<550&&secondP>400&&secondP<550);
 printf("PASS: real encrypted ROM generation: %u/1000 special, %u duplicates, first/second Pidgey=%u/%u\n",special,duplicates,firstP,secondP);
 CALL(VarSet,VAR_REPEL_STEP_COUNT,100);
 for(unsigned i=0;i<30;i++)CHECK(!CALL(TryStartLandEncounter,info,1));
 CHECK(CALL(TryStartLandEncounter,info,0));CALL(ResetTasks);
 printf("PASS: real Repel suppresses low-level pairs; zero-flags Sweet Scent generation bypasses it\n");
 CALL(VarSet,VAR_REPEL_STEP_COUNT,0);
 monput(player(0),ABI_DATA_HP,0);CHECK(!CALL(TryDoDoubleWildBattle));
 CHECK(CALL(TryStartLandEncounter,info,0));CHECK(core->busRead32(core,SYM_gBattleTypeFlags)==0);
 CHECK(monget(enemy(1),ABI_DATA_SPECIES)==0);CALL(ResetTasks);
 printf("PASS: fainted lead plus one usable Pokemon falls back to single, with no stale second opponent\n");
 monput(player(0),ABI_DATA_HP,100);monput(player(1),ABI_DATA_IS_EGG,1);
 CHECK(!CALL(TryDoDoubleWildBattle));
 printf("PASS: egg is excluded from usable double-battle party count\n");
}
static void wildTests(void){
 generationTests();
 wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);
 CHECK(CALL(TryDoDoubleWildBattle));
 warpGrass();walkToBattle();
 unsigned s1=monget(enemy(0),ABI_DATA_SPECIES),s2=monget(enemy(1),ABI_DATA_SPECIES);
 printf("Encounter generated %u + %u\n",s1,s2);
 waitAction();
 CHECK(core->busRead8(core,SYM_gBattlersCount)==4);
 CHECK((core->busRead32(core,SYM_gBattleTypeFlags)&~BATTLE_TYPE_IS_MASTER)==BATTLE_TYPE_DOUBLE);
 CHECK(CALL(GetWildOpponentCount)==2);
 CHECK(core->busRead16(core,bmon(1)+ABI_BMON_SPECIES)==s1);
 CHECK(core->busRead16(core,bmon(3)+ABI_BMON_SPECIES)==s2);
 // Execute the actual wild-opponent controller in a restored battle snapshot.
 void *battleState=malloc(core->stateSize(core));CHECK(battleState&&core->saveState(core,battleState));
 for(unsigned b=1;b<4;b+=2){
  unsigned targets[2]={0};
  for(unsigned n=0;n<200;n++){
   core->busWrite8(core,SYM_gActiveBattler,b);
   uint32_t packet=SYM_gBattleBufferA+b*ABI_BUFFER_STRIDE+4+ABI_MOVEINFO_MOVES;
   for(unsigned m=0;m<4;m++)core->busWrite16(core,packet+2*m,m?MOVE_NONE:MOVE_TACKLE);
   CALL(OpponentHandleChooseMove);
   unsigned t=core->busRead8(core,SYM_gBattleBufferB+b*ABI_BUFFER_STRIDE+3);
   CHECK(t==0||t==2);targets[t/2]++;
  }
  CHECK(targets[0]>60&&targets[1]>60);
  printf("PASS: real wild controller %u targeted player battlers 0/2 %u/%u times\n",b,targets[0],targets[1]);
 }
 CHECK(core->loadState(core,battleState));free(battleState);
 // Execute real trap checks, including a defeated trapping opponent.
 core->busWrite8(core,SYM_gActiveBattler,0);
 unsigned oldAbility=core->busRead8(core,bmon(1)+ABI_BMON_ABILITY);
 core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_SHADOW_TAG);
 CHECK(CALL(IsRunningFromBattleImpossible)!=0);
 unsigned oldHP=core->busRead16(core,bmon(1)+ABI_BMON_HP);
 core->busWrite16(core,bmon(1)+ABI_BMON_HP,0);CHECK(CALL(IsRunningFromBattleImpossible)==0);
 core->busWrite16(core,bmon(1)+ABI_BMON_HP,oldHP);
 core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,oldAbility);
 screenshot(getenv("LIGHTNING_WILD_CAPTURE"));
 printf("PASS: real wild-double transition/intro/send-out reached player action UI with four battlers\n");
 // Run using ordinary command menu, without Smoke Ball/Run Away.
 press(128);press(16);press(1);waitOverworld(1);
 CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);
 printf("PASS: normal Run command escaped double encounter and restored overworld\n");
 for(unsigned killed=1;killed<=3;killed+=2){
  wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);
  CHECK(CALL(AddBagItem,ITEM_MASTER_BALL,2));startSpecial();waitAction();
  CHECK(CALL(GetWildOpponentCount)==2);
  openBagUse();
  CHECK(CALL(BagGetQuantityByItemId,ITEM_MASTER_BALL)==2);
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==0);
  printf("PASS: real Bag USE rejected ball with both opponents alive, item retained\n");
  // Dismiss message and close Bag, preserving the pending turn.
  for(unsigned n=0;n<80 && controller(0)!=SYM_HandleInputChooseAction;n++){press(2);frames(6,0);}
  waitController(0,SYM_HandleInputChooseAction);
  // After cancelling Bag the command cursor remains on Bag. LEFT selects Fight.
  if(core->busRead8(core,SYM_gActionSelectionCursor)&1)press(32);
  killOne(killed);
  unsigned caught=killed==1?SPECIES_CLOYSTER:SPECIES_ONIX;
  CHECK(CALL(GetWildCatchTarget)==(killed^2));
  openBagUse();chooseMove(2,1,killed^2);waitOverworld(2); // B declines nickname and closes new-Dex popup
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_CAUGHT);
  CHECK(CALL(BagGetQuantityByItemId,ITEM_MASTER_BALL)==1);
  CHECK(monget(player(2),ABI_DATA_SPECIES)==caught);
  CHECK(CALL(GetSetPokedexFlag,CALL(SpeciesToNationalPokedexNum,caught),1));
  CHECK(core->busRead16(core,SYM_gBattleResults+ABI_RESULT_CAUGHT)==caught);
  printf("PASS: captured %u from battler %u, correct party/Dex/result, one ball consumed, returned to overworld\n",caught,killed^2);
  CHECK(CALL(TrySavingData,0)==1);
  monput(player(2),ABI_DATA_SPECIES,SPECIES_BULBASAUR);
  for(unsigned i=0;i<ABI_SAVE1_SIZE;i+=4)core->busWrite32(core,SYM_gSaveBlock1+i,0);
  for(unsigned i=0;i<ABI_SAVE2_SIZE;i+=4)core->busWrite32(core,SYM_gSaveBlock2+i,0);
  CHECK(CALL(LoadGameSave,0)==1);
  CHECK(monget(player(2),ABI_DATA_SPECIES)==caught);
  CHECK(CALL(GetSetPokedexFlag,CALL(SpeciesToNationalPokedexNum,caught),1));
  CHECK(CALL(BagGetQuantityByItemId,ITEM_MASTER_BALL)==1);
  printf("PASS: captured species, Dex and ball inventory survived actual flash save/load\n");
 }

 wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);
 startSpecial();waitAction();
 chooseMove(0,0,1);chooseMove(2,0,3);waitOverworld(1);
 CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_WON);
 CHECK(monget(enemy(0),ABI_DATA_HP)==0&&monget(enemy(1),ABI_DATA_HP)==0);
 printf("PASS: independently targeted both wild opponents, won after two faints and restored overworld\n");
 // One-member party at entry, ordinary non-designated map, and cave entry.
 for(unsigned kind=0;kind<3;kind++){
  unsigned group=kind==2?MAP_GROUP(MAP_MT_MOON_1F):MAP_GROUP(MAP_ROUTE1);
  unsigned map=kind==2?MAP_NUM(MAP_MT_MOON_1F):kind==1?MAP_NUM(MAP_ROUTE2):MAP_NUM(MAP_ROUTE1);
  wildFixture(group,map,kind!=0);
  CHECK(CALL(TryStartLandEncounter,landInfo(),0));waitAction();
  CHECK(core->busRead8(core,SYM_gBattlersCount)==(kind==2?4:2));
  press(128);press(16);press(1);waitOverworld(1);
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);
  printf("PASS: %s battle entry/Run/overworld\n",kind==0?"one-member single fallback":kind==1?"ordinary Route 2 single":"Mt. Moon cave double");
 }
 // RTC is present but does not replace existing encounter tables. Test the real
 // Sweet Scent entry at both times, including its original Repel bypass.
 for(unsigned hour=0;hour<=12;hour+=12){
  wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);warpGrass();clockAt(hour);
  CALL(VarSet,VAR_REPEL_STEP_COUNT,100);
  CALL(SetWeather,WEATHER_RAIN);frames(120,0);
  CHECK(CALL(SweetScentWildEncounter));waitAction();
  CHECK(core->busRead8(core,SYM_gBattlersCount)==4);
  CHECK(core->busRead16(core,SYM_gBattleWeather)&B_WEATHER_RAIN);
  press(128);press(16);press(1);waitOverworld(1);
  CHECK(CALL(GetSav1Weather)==WEATHER_RAIN);
  printf("PASS: real Sweet Scent/Repel bypass at RTC hour %u; rain entered battle and persisted on return\n",hour);
 }


 wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);
 for(unsigned i=2;i<6;i++)CALL(CreateMon,player(i),SPECIES_PIKACHU,50,31,1,12345+i,0,1234);
 core->busWrite8(core,SYM_gPlayerPartyCount,6);
 uint32_t name=CALL(GetBoxNamePtr,0);core->busWrite8(core,name,0xBC);core->busWrite8(core,name+1,0xFF);
 CHECK(CALL(AddBagItem,ITEM_MASTER_BALL,1));startSpecial();waitAction();killOne(1);
 openBagUse();chooseMove(2,1,3);waitOverworld(2);
 CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_CAUGHT);
 CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_SPECIES)==SPECIES_CLOYSTER);
 CHECK(CALL(BagGetQuantityByItemId,ITEM_MASTER_BALL)==0);
 printf("PASS: full-party capture sent right-side Cloyster to PC with correct species and one ball consumed\n");
 wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);warpGrass();
 CALL(InitRoamer);
 core->busWrite8(core,SYM_sRoamerLocation,MAP_GROUP(MAP_ROUTE1));
 core->busWrite8(core,SYM_sRoamerLocation+1,MAP_NUM(MAP_ROUTE1));
 unsigned found=0;
 for(unsigned n=0;n<100;n++){
  CHECK(CALL(SweetScentWildEncounter));
  if(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_ROAMER){found=1;break;}
  CALL(ResetTasks);
 }
 CHECK(found);CHECK(!(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_DOUBLE));
 CHECK(monget(enemy(1),ABI_DATA_SPECIES)==0);waitAction();
 CHECK(core->busRead8(core,SYM_gBattlersCount)==2);
 chooseMove(0,1,1);waitOverworld(1);
 CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_MON_FLED);
 printf("PASS: roaming Pokemon preempted double policy, entered single battle and fled normally\n");
}
