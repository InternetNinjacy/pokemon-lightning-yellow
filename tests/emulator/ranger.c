// Compiled ROM acceptance: native trainer creation, scripts, battles, PC and flash.
#include "constants/opponents.h"
#include "constants/heal_locations.h"
#include "ranger_expected.h"
static const uint32_t rangerEntries[]={SYM_SaffronCity_Dojo_EventScript_Ranger1,SYM_SaffronCity_Dojo_EventScript_Ranger2,SYM_SaffronCity_Dojo_EventScript_Ranger3,SYM_SaffronCity_Dojo_EventScript_Ranger4,SYM_SaffronCity_Dojo_EventScript_RangerPark};
static const uint32_t rangerGifts[]={SYM_SaffronCity_Dojo_EventScript_RangerGift0,SYM_SaffronCity_Dojo_EventScript_RangerGift1,SYM_SaffronCity_Dojo_EventScript_RangerGift2};
static const unsigned rangerSizes[]={2,2,2,2,4};
static const unsigned rangerSpecies[5][4]={{SPECIES_PERSIAN,SPECIES_ARCANINE},{SPECIES_FEAROW,SPECIES_NOCTOWL},{SPECIES_GOLDUCK,SPECIES_MANECTRIC},{SPECIES_HOUNDOOM,SPECIES_ARCANINE},{SPECIES_NOCTOWL,SPECIES_MANECTRIC,SPECIES_GOLDUCK,SPECIES_ARCANINE}};
static const unsigned rangerLevels[5][4]={{34,35},{35,36},{36,37},{37,38},{39,39,40,41}};
static const unsigned rangerAbilities[5][4]={{ABILITY_LIMBER,ABILITY_INTIMIDATE},{ABILITY_KEEN_EYE,ABILITY_INSOMNIA},{ABILITY_CLOUD_NINE,ABILITY_STATIC},{ABILITY_FLASH_FIRE,ABILITY_INTIMIDATE},{ABILITY_INSOMNIA,ABILITY_STATIC,ABILITY_CLOUD_NINE,ABILITY_INTIMIDATE}};
static const unsigned rangerGiftSpecies[]={SPECIES_ARCANINE,SPECIES_NOCTOWL,SPECIES_GOLDUCK};
static const unsigned rangerGiftMoves[][4]={{MOVE_BLAZE_KICK,MOVE_BITE,MOVE_TAKE_DOWN,MOVE_ODOR_SLEUTH},{MOVE_EXTRASENSORY,MOVE_HYPNOSIS,MOVE_REFLECT,MOVE_AERIAL_ACE},{MOVE_MUDDY_WATER,MOVE_CONFUSION,MOVE_CALM_MIND,MOVE_DISABLE}};
static void rangerWarp(unsigned x,unsigned y) {fossilWarp(MAP_GROUP(MAP_SAFFRON_CITY_DOJO),MAP_NUM(MAP_SAFFRON_CITY_DOJO),x,y);}
static void rangerFlags(unsigned mask) {for(unsigned i=0;i<4;i++){if(mask&(1U<<i)){CALL(FlagSet,TRAINER_FLAGS_START+TRAINER_RANGER_CAMP_1+i);}else{CALL(FlagClear,TRAINER_FLAGS_START+TRAINER_RANGER_CAMP_1+i);}}}
static void rangerFixture(unsigned count) {
 daycareFixture(1);CALL(ResetPokemonStorageSystem);CALL(SetLastHealLocationWarp,HEAL_LOCATION_SAFFRON_CITY);
 for(unsigned i=0;i<count;i++)CALL(CreateMon,player(i),SPECIES_PIKACHU,20,31,1,i,0,0);
 for(unsigned i=count;i<6;i++)clearBytes(player(i),ABI_MON_SIZE);
 core->busWrite8(core,SYM_gPlayerPartyCount,count);rangerWarp(6,12);
}
static void rangerStart(unsigned local,uint32_t script) {
 unsigned obj=CALL(GetObjectEventIdByLocalIdAndMap,local,MAP_NUM(MAP_SAFFRON_CITY_DOJO),MAP_GROUP(MAP_SAFFRON_CITY_DOJO));
 CHECK(obj<ABI_OBJECT_COUNT);core->busWrite8(core,SYM_gSelectedObjectEvent,obj);fossilStart(script);
}
static void rangerWalk(unsigned x,unsigned y) {
 int from[208],queue[208],head=0,tail=0;for(unsigned i=0;i<208;i++)from[i]=-2;
 CALL(PlayerGetDestCoords,scratch,scratch+2);int sx=core->busRead16(core,scratch)-7,sy=core->busRead16(core,scratch+2)-7;
 CHECK(sx>=0&&sx<13&&sy>=0&&sy<16);int start=sy*13+sx,goal=y*13+x;queue[tail++]=start;from[start]=-1;
 const int dx[]={0,0,-1,1},dy[]={-1,1,0,0};
 while(head<tail&&from[goal]==-2){int p=queue[head++];for(unsigned d=0;d<4;d++){
  int nx=p%13+dx[d],ny=p/13+dy[d];if(nx<1||nx>11||ny<3||ny>13)continue;
  if(((nx==1||nx==11)&&(ny==8||ny==10))||(nx==6&&ny==5)||((nx==5||nx==6||nx==7)&&ny==3))continue;
  int n=ny*13+nx;
  // Keep native one-tile trainer sight active, but avoid it unless it is the goal.
  if(n!=goal&&(nx==2||nx==10)&&(ny==8||ny==10))continue;
  if(from[n]!=-2||CALL(MapGridGetCollisionAt,nx+7,ny+7))continue;
  from[n]=p;queue[tail++]=n;
 }}
 CHECK(from[goal]!=-2);unsigned path[208],n=0;for(int p=goal;p!=start;p=from[p])path[n++]=p;int pos=start;
 while(n){int next=path[--n],diff=next-pos;unsigned key=diff==-13?64:diff==13?128:diff==-1?32:16,t;
  for(t=0;t<80;t++){frames(1,key);CALL(PlayerGetDestCoords,scratch,scratch+2);if(core->busRead16(core,scratch)==(unsigned)(next%13+7)&&core->busRead16(core,scratch+2)==(unsigned)(next/13+7))break;}
  CHECK(t<80);frames(20,0);pos=next;
 }
}
static void rangerDataTests(void) {
 for(unsigned t=0;t<5;t++) {
  rangerFixture(2);core->busWrite32(core,SYM_gBattleTypeFlags,BATTLE_TYPE_TRAINER);
  CHECK(CALL(CreateNPCTrainerParty,SYM_gEnemyParty,TRAINER_RANGER_CAMP_1+t)==rangerSizes[t]);
  for(unsigned m=0;m<rangerSizes[t];m++) {
   CHECK(monget(enemy(m),ABI_DATA_SPECIES)==rangerSpecies[t][m]);CHECK(monget(enemy(m),ABI_DATA_LEVEL)==rangerLevels[t][m]);
   CHECK(CALL(GetMonAbility,enemy(m))==rangerAbilities[t][m]);CHECK(monget(enemy(m),ABI_DATA_HELD_ITEM)==ITEM_NONE);
   for(unsigned k=0;k<4;k++) {
    CHECK(monget(enemy(m),ABI_DATA_MOVE1+k)==rangerExpectedMoves[t][m][k]);
    CHECK(monget(enemy(m),ABI_DATA_PP1+k)==core->busRead8(core,SYM_gBattleMoves+ABI_BMOVE_SIZE*rangerExpectedMoves[t][m][k]+ABI_BMOVE_PP));
   }
  }
  if(t<2) {
   CHECK(monget(enemy(0),ABI_DATA_FRIENDSHIP)==255);
   core->busWrite8(core,bmon(1)+ABI_BMON_FRIENDSHIP,monget(enemy(0),ABI_DATA_FRIENDSHIP));
   core->busWrite8(core,SYM_gBattlerAttacker,1);core->busWrite16(core,SYM_gCurrentMove,MOVE_RETURN);
   core->busWrite32(core,SYM_gBattlescriptCurrInstr,scratch);CALL(Cmd_friendshiptodamagecalculation);
   CHECK(core->busRead16(core,SYM_gDynamicBasePower)==102);
  }
 }
 CHECK(core->loadState(core,bootState));
 printf("PASS: real constructor creates all12 Ranger Pokemon with approved levels/abilities/no held items and native Return power102\n");
}
static void rangerGiftIdentityTests(void) {
 for(unsigned gift=0;gift<3;gift++) {
  rangerFixture(6);rangerWarp(6,6);core->busWrite16(core,SYM_gSpecialVar_0x8004,gift);
  unsigned seed=core->busRead32(core,SYM_gRngValue);CHECK(CALL(RangerGiveGift)==MON_CANT_GIVE);CHECK(core->busRead32(core,SYM_gRngValue)==seed);
  CALL(FlagSet,TRAINER_FLAGS_START+TRAINER_RANGER_PARK);CHECK(CALL(RangerGiveGift)==MON_CANT_GIVE);CHECK(core->busRead32(core,SYM_gRngValue)==seed);CHECK(!CALL(FlagGet,FLAG_RANGER_CAMP_GIFT_RECEIVED));
  clearBytes(player(5),ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,5);
  CHECK(CALL(RangerGiveGift)==MON_GIVEN_TO_PARTY);CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==6);CHECK(CALL(FlagGet,FLAG_RANGER_CAMP_GIFT_RECEIVED));
  CHECK(monget(player(5),ABI_DATA_SPECIES)==rangerGiftSpecies[gift]);CHECK(monget(player(5),ABI_DATA_LEVEL)==30);CHECK(monget(player(5),ABI_DATA_OT_ID)==~0x78563412U);
  CHECK(CALL(IsTradedMon,player(5)));CHECK(!monget(player(5),ABI_DATA_SANITY_IS_BAD_EGG));CHECK(monget(player(5),ABI_DATA_HELD_ITEM)==ITEM_NONE);
  CALL(GetMonData3,player(5),ABI_DATA_OT_NAME,scratch);const unsigned char park[]={0xCA,0xBB,0xCC,0xC5,0xFF};sameBytes(scratch,park,sizeof(park));
  for(unsigned i=0;i<4;i++){CHECK(monget(player(5),ABI_DATA_MOVE1+i)==rangerGiftMoves[gift][i]);CHECK(monget(player(5),ABI_DATA_PP1+i)==core->busRead8(core,SYM_gBattleMoves+ABI_BMOVE_SIZE*rangerGiftMoves[gift][i]+ABI_BMOVE_PP));}
  unsigned pid=monget(player(5),ABI_DATA_PERSONALITY);flashRoundTrip();CHECK(monget(player(5),ABI_DATA_PERSONALITY)==pid);CHECK(CALL(IsTradedMon,player(5)));
  for(unsigned other=0;other<3;other++){core->busWrite16(core,SYM_gSpecialVar_0x8004,other);CHECK(CALL(RangerGiveGift)==MON_CANT_GIVE);}
  CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==6);CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_SPECIES)==SPECIES_NONE);
 }
 // The actual delivered PARK mon, rather than a merely renamed own mon, drives EXP/obedience.
 wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);CALL(FlagSet,TRAINER_FLAGS_START+TRAINER_RANGER_PARK);core->busWrite16(core,SYM_gSpecialVar_0x8004,0);CHECK(CALL(RangerGiveGift)==0);
 startSpecial();waitAction();copyBytes(player(1),player(2),ABI_MON_SIZE);CHECK(vendorExp(0,100)==100);CHECK(vendorExp(1,100)==150);
 CALL(CopyPlayerPartyMonToBattleData,0,1);core->busWrite8(core,SYM_gBattlerPartyIndexes,1);core->busWrite8(core,SYM_gBattlerAttacker,0);core->busWrite8(core,SYM_gBattlerTarget,1);
 core->busWrite16(core,SYM_gCurrentMove,MOVE_BITE);core->busWrite8(core,SYM_gCurrMovePos,1);core->busWrite32(core,SYM_gBattleTypeFlags,0);
 unsigned disobey=0;for(unsigned i=0;i<128;i++)disobey+=CALL(IsMonDisobedient)!=0;CHECK(disobey>0);CALL(FlagSet,FLAG_BADGE02_GET);for(unsigned i=0;i<32;i++)CHECK(CALL(IsMonDisobedient)==0);
 CHECK(core->loadState(core,bootState));
 printf("PASS: all3 exact party-only PARK gifts, full-party/no-RNG guards, unique saved reward, native150%% EXP and badge-dependent obedience\n");
}
static void rangerGateTests(void) {
 rangerFixture(2);rangerWarp(6,6);rangerStart(5,rangerEntries[4]);fossilClose(1);CHECK(CALL(FlagGet,FLAG_RANGER_PARK_INTRO_SEEN));vendorTextMatches(SYM_SaffronCity_Dojo_Text_ParkLocked);
 for(unsigned mask=0;mask<15;mask++) {
  rangerFlags(mask);rangerStart(5,rangerEntries[4]);fossilClose(1);CHECK(!CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_RANGER_PARK));vendorTextMatches(SYM_SaffronCity_Dojo_Text_ParkLocked);
 }
 rangerFlags(15);rangerStart(5,rangerEntries[4]);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);press(2);fossilClose(1);vendorTextMatches(SYM_SaffronCity_Dojo_Text_ParkNo);CHECK(!CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_RANGER_PARK));
 flashRoundTrip();rangerWarp(6,6);for(unsigned i=0;i<4;i++)CHECK(CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_RANGER_CAMP_1+i));
 clearBytes(player(1),ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,1);rangerStart(5,rangerEntries[4]);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);press(1);fossilClose(1);CHECK(!CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_RANGER_PARK));
 // Two party entries must still mean two usable, non-Egg Pokemon.
 CALL(CreateMon,player(1),SPECIES_PIKACHU,20,31,1,2,0,0);core->busWrite8(core,SYM_gPlayerPartyCount,2);
 monput(player(1),ABI_DATA_HP,0);rangerStart(5,rangerEntries[4]);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);press(1);fossilClose(1);CHECK(!CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_RANGER_PARK));
 monput(player(1),ABI_DATA_HP,20);monput(player(1),ABI_DATA_IS_EGG,1);rangerStart(5,rangerEntries[4]);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);press(1);fossilClose(1);CHECK(!CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_RANGER_PARK));
 printf("PASS: actual lore/gate for every0..3-win combination, four-win NO, saved flags and one-usable Double refusal\n");
}
static void rangerRename(void) {
 unsigned f;for(f=0;f<900;f++){frames(1,0);if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_NamingScreen)break;}
 CHECK(f<900);frames(120,0);for(unsigned i=0;i<12;i++)press(2);press(1);press(8);frames(30,0);press(1);fossilClose(1);
}
static void rangerGiftUiTests(void) {
 for(unsigned gift=0;gift<3;gift++) {
  rangerFixture(6);rangerWarp(6,4);unsigned local=gift==2?8:6+gift;
  rangerStart(local,rangerGifts[gift]);fossilClose(1);vendorTextMatches(SYM_SaffronCity_Dojo_Text_GiftLocked);CHECK(!CALL(FlagGet,FLAG_RANGER_CAMP_GIFT_RECEIVED));
  rangerFlags(15);CALL(FlagSet,TRAINER_FLAGS_START+TRAINER_RANGER_PARK);
  rangerStart(local,rangerGifts[gift]);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);press(2);fossilClose(1);CHECK(!CALL(FlagGet,FLAG_RANGER_CAMP_GIFT_RECEIVED));
  rangerStart(local,rangerGifts[gift]);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);press(1);fossilClose(1);vendorTextMatches(SYM_SaffronCity_Dojo_Text_GiftFull);CHECK(!CALL(FlagGet,FLAG_RANGER_CAMP_GIFT_RECEIVED));
  flashRoundTrip();rangerWarp(6,4);clearBytes(player(5),ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,5);
  rangerStart(local,rangerGifts[gift]);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);press(1);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
  if(gift==0){press(1);rangerRename();CALL(GetMonData3,player(5),ABI_DATA_NICKNAME,testStack);CHECK(core->busRead8(core,testStack)==0xBB);}else{press(2);fossilClose(1);}
  CHECK(CALL(FlagGet,FLAG_RANGER_CAMP_GIFT_RECEIVED));CHECK(monget(player(5),ABI_DATA_SPECIES)==rangerGiftSpecies[gift]);CHECK(CALL(IsTradedMon,player(5)));
  for(unsigned i=6;i<=8;i++)CHECK(CALL(GetObjectEventIdByLocalIdAndMap,i,MAP_NUM(MAP_SAFFRON_CITY_DOJO),MAP_GROUP(MAP_SAFFRON_CITY_DOJO))>=ABI_OBJECT_COUNT);
  flashRoundTrip();rangerWarp(6,6);rangerStart(5,rangerEntries[4]);fossilClose(1);vendorTextMatches(SYM_SaffronCity_Dojo_Text_ParkAfterGift);CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==6);
 }
 printf("PASS: actual3 gift previews/NO/full-party/saved retry/nickname acceptance and refusal, unique foreign delivery, all balls removed and saved Park after-gift dialogue\n");
}
static void rangerPcTests(void) {
 rangerFixture(6);rangerWalk(1,13);press(64);press(1);fossilWaitMenu(SYM_Task_MultichoiceMenu_HandleInput);press(1);
 fossilWaitMenu(SYM_Task_PCMainMenu);frames(90,0);press(128);press(1);fossilWaitMenu(SYM_Task_PokeStorageMain);
 press(1);fossilWaitMenu(SYM_Task_OnSelectedMon);frames(60,0);press(1);fossilWaitMenu(SYM_Task_DepositMenu);frames(60,0);press(1);fossilWaitMenu(SYM_Task_PokeStorageMain);
 CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_SPECIES)==SPECIES_PIKACHU);
 press(2);fossilWaitMenu(SYM_Task_OnBPressed);frames(60,0);press(2);fossilWaitMenu(SYM_Task_PCMainMenu);frames(90,0);press(2);fossilWaitMenu(SYM_Task_MultichoiceMenu_HandleInput);press(2);fossilClose(2);
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==5);CHECK(!CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_RANGER_CAMP_1));CHECK(!CALL(FlagGet,FLAG_RANGER_CAMP_GIFT_RECEIVED));
 screenshot(getenv("LIGHTNING_RANGER_PC_CAPTURE"));
 printf("PASS: actual before-battle Ranger PC tile opens standard storage;6->5 deposit/box record and field return work without a badge\n");
}
static void rangerContinueTests(void) {
 const unsigned pos[][2]={{1,12},{6,3}};
 for(unsigned n=0;n<2;n++) {
  rangerFixture(2);rangerWarp(pos[n][0],pos[n][1]);
  uint32_t templates=careSave1()+ABI_SAVE1_OBJECT_TEMPLATES;
  for(unsigned i=0;i<5;i++)core->busWrite8(core,templates+i*ABI_TEMPLATE_SIZE+ABI_TEMPLATE_GRAPHICS,OBJ_EVENT_GFX_BLACK_BELT);
  clearBytes(templates+7*ABI_TEMPLATE_SIZE,ABI_TEMPLATE_SIZE);CALL(FlagSet,FLAG_GOT_HITMON_FROM_DOJO);CALL(FlagSet,FLAG_HIDE_DOJO_HITMONLEE_BALL);CALL(FlagSet,TRAINER_FLAGS_START+TRAINER_BLACK_BELT_KOICHI);
  flashRoundTrip();CHECK(CALL(RangerNeedsMapRefresh));CALL(CB2_ContinueSavedGame);frames(480,0);CHECK(fieldScriptDone());CHECK(!CALL(RangerNeedsMapRefresh));
  CALL(PlayerGetDestCoords,scratch,scratch+2);CHECK(core->busRead16(core,scratch)==pos[n][0]+7);CHECK(core->busRead16(core,scratch+2)==pos[n][1]+8);
  CHECK(CALL(FlagGet,FLAG_GOT_HITMON_FROM_DOJO));CHECK(CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_BLACK_BELT_KOICHI));CHECK(!CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_RANGER_PARK));CHECK(!CALL(FlagGet,FLAG_RANGER_CAMP_GIFT_RECEIVED));
  CHECK(monget(player(0),ABI_DATA_SPECIES)==SPECIES_PIKACHU);flashRoundTrip();CHECK(!CALL(RangerNeedsMapRefresh));
 }
 printf("PASS: actual old inside-dojo Continue refreshes8 templates and PC/ball overlaps, preserving party and legacy gift/trainer flags independently\n");
}
static void rangerFinishBattle(unsigned trainer,unsigned won) {
 unsigned f,quiet=0,seen=0;
 for(f=0;f<30000;f++) {
  if(fieldScriptDone()) {if(++quiet==30)break;frames(1,0);continue;}
  quiet=0;
  if(trainer==TRAINER_RANGER_PARK && (core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_BattleMainCB2) {
   for(unsigned b=1;b<4;b+=2)if(core->busRead16(core,bmon(b)+ABI_BMON_HP)) {
    unsigned species=core->busRead16(core,bmon(b)+ABI_BMON_SPECIES);
    for(unsigned i=0;i<4;i++)if(species==rangerSpecies[4][i])seen|=1U<<i;
   }
  }
  unsigned key=0;
  if((f%8)==0) {
   key=2; // Advance battle text and decline native optional switch prompts.
   if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_BattleMainCB2) {
    unsigned exec=core->busRead32(core,SYM_gBattleControllerExecFlags);
    for(unsigned b=0;b<4;b+=2) {
     if(!(exec&(1U<<b)))continue;
     unsigned fn=controller(b);
     if(fn==SYM_HandleInputChooseAction) {
      unsigned cursor=core->busRead8(core,SYM_gActionSelectionCursor+b);
      key=cursor&2?64:cursor&1?32:1;break;
     }
     if(fn==SYM_HandleInputChooseMove) {
      unsigned cursor=core->busRead8(core,SYM_gMoveSelectionCursor+b);
      key=cursor&2?64:cursor&1?32:1;break;
     }
     if(fn==SYM_HandleInputChooseTarget){key=1;break;}
    }
   }
  }
  frames(1,key);
 }
 if(f==30000){fprintf(stderr,"Ranger win timeout trainer%u callback%08x ctrls%08x/%08x outcome%u\n",trainer,core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),controller(0),controller(2),core->busRead8(core,SYM_gBattleOutcome));screenshot("/tmp/ranger-battle-timeout.ppm");}
 CHECK(f<30000);CHECK(CALL(FlagGet,TRAINER_FLAGS_START+trainer)==won);frames(30,0);
 if(won&&trainer==TRAINER_RANGER_PARK)CHECK(seen==15);
 printf("PASS: actual Ranger trainer%u %s and native field-script return in%u frames\n",trainer,won?"victory":"loss/blackout",f);
}
static void rangerBattleTests(void) {
 rangerFixture(2);rangerWarp(6,9);
 for(unsigned i=0;i<2;i++){CALL(CreateMon,player(i),SPECIES_MEWTWO,100,31,1,i,0,0);monput(player(i),ABI_DATA_MOVE1,MOVE_SWIFT);monput(player(i),ABI_DATA_PP1,20);}
 const unsigned order[]={2,0,3,1};
 for(unsigned step=0;step<4;step++) {
  unsigned which=order[step];rangerWarp(6,9);
  if(step==0){rangerWalk(10,8);waitAction();} // Actual top Ranger LOS before either bottom trainer.
  else {rangerStart(which+1,rangerEntries[which]);waitAction();}
  CHECK(core->busRead16(core,SYM_gTrainerBattleOpponent_A)==TRAINER_RANGER_CAMP_1+which);CHECK(!(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_DOUBLE));
  uint32_t hist=core->busRead32(core,core->busRead32(core,SYM_gBattleResources)+ABI_BRES_HISTORY);
  CHECK(core->busRead8(core,hist+ABI_HISTORY_COUNT)==1);CHECK(core->busRead16(core,hist+ABI_HISTORY_ITEMS)==ITEM_SUPER_POTION);
  rangerFinishBattle(TRAINER_RANGER_CAMP_1+which,1);
  flashRoundTrip();CHECK(CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_RANGER_CAMP_1+which));
  for(unsigned i=0;i<2;i++)monput(player(i),ABI_DATA_PP1,20);
 }
 // Exercise a real loss with intentionally weak player fixtures; opponents stay approved/unmodified.
 for(unsigned i=0;i<2;i++){CALL(CreateMon,player(i),SPECIES_PIKACHU,1,0,1,i,0,0);monput(player(i),ABI_DATA_MOVE1,MOVE_SPLASH);monput(player(i),ABI_DATA_PP1,40);}
 rangerWarp(6,6);rangerStart(5,rangerEntries[4]);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);press(1);waitAction();
 rangerFinishBattle(TRAINER_RANGER_PARK,0);CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_LOST);
 CHECK(!CALL(FlagGet,FLAG_RANGER_CAMP_GIFT_RECEIVED));for(unsigned i=0;i<4;i++)CHECK(CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_RANGER_CAMP_1+i));
 rangerWarp(6,6);rangerStart(5,rangerEntries[4]);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);press(2);fossilClose(1);vendorTextMatches(SYM_SaffronCity_Dojo_Text_ParkNo);
 for(unsigned i=0;i<2;i++){CALL(CreateMon,player(i),SPECIES_MEWTWO,100,31,1,i,0,0);monput(player(i),ABI_DATA_MOVE1,MOVE_SWIFT);monput(player(i),ABI_DATA_PP1,20);}
 rangerStart(5,rangerEntries[4]);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);press(1);waitAction();
 CHECK(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_DOUBLE);CHECK(core->busRead8(core,SYM_gBattlersCount)==4);
 CHECK(core->busRead16(core,bmon(1)+ABI_BMON_SPECIES)==SPECIES_NOCTOWL);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_SPECIES)==SPECIES_MANECTRIC);
 uint32_t hist=core->busRead32(core,core->busRead32(core,SYM_gBattleResources)+ABI_BRES_HISTORY);CHECK(core->busRead8(core,hist+ABI_HISTORY_COUNT)==1);CHECK(core->busRead16(core,hist+ABI_HISTORY_ITEMS)==ITEM_FULL_RESTORE);
 rangerFinishBattle(TRAINER_RANGER_PARK,1);CHECK(!CALL(FlagGet,FLAG_RANGER_CAMP_GIFT_RECEIVED));flashRoundTrip();CHECK(CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_RANGER_PARK));
 rangerWarp(6,6);rangerStart(5,rangerEntries[4]);fossilClose(1);vendorTextMatches(SYM_SaffronCity_Dojo_Text_ParkBeforeGift);
 screenshot(getenv("LIGHTNING_RANGER_CAPTURE"));
 printf("PASS: actual controlled-level100 player defeats unmodified Rangers in3/1/4/2 order, native bag inventories, saved wins, Park actual loss/rechallenge and all four native Double replacements/victory with no rematch\n");
}
static void rangerTests(void) {rangerDataTests();rangerGiftIdentityTests();rangerGateTests();rangerGiftUiTests();rangerPcTests();rangerContinueTests();rangerBattleTests();}
