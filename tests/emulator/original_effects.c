// Real native turns for approved single-hit Pin Missile and level-based Dragon Rage.
static void effectBattleFixture(unsigned two,unsigned move){
 // Route 1 is ordinary Singles after WRL-002; use the retained Mt. Moon double fixture.
 wildFixture(two ? MAP_GROUP(MAP_MT_MOON_1F) : MAP_GROUP(MAP_ROUTE1),
             two ? MAP_NUM(MAP_MT_MOON_1F) : MAP_NUM(MAP_ROUTE1), two);
 for(unsigned p=0;p<=two;p++)for(unsigned m=0;m<4;m++)CALL(SetMonMoveSlot,player(p),!p&&!m?move:MOVE_SPLASH,m);
 CHECK(CALL(TryStartLandEncounter,landInfo(),0));waitAction();core->busWrite16(core,SYM_gBattleWeather,0);
 for(unsigned b=0;b<(two?4:2);b++){
  core->busWrite8(core,bmon(b)+ABI_BMON_TYPE1,TYPE_NORMAL);core->busWrite8(core,bmon(b)+ABI_BMON_TYPE2,TYPE_NORMAL);
  core->busWrite8(core,bmon(b)+ABI_BMON_ABILITY,b&1?ABILITY_BATTLE_ARMOR:ABILITY_NONE);core->busWrite16(core,bmon(b)+ABI_BMON_ITEM,ITEM_NONE);
  core->busWrite16(core,bmon(b)+ABI_BMON_HP,1000);core->busWrite16(core,bmon(b)+ABI_BMON_MAX_HP,1000);monput(b&1?enemy(b>>1):player(b>>1),ABI_DATA_HP,1000);monput(b&1?enemy(b>>1):player(b>>1),ABI_DATA_MAX_HP,1000);
  for(unsigned s=0;s<ABI_BMON_STAGES_COUNT;s++)core->busWrite8(core,bmon(b)+ABI_BMON_STAGES+s,6);
  if(b&1)for(unsigned m=0;m<4;m++){core->busWrite16(core,bmon(b)+ABI_BMON_MOVES+2*m,MOVE_SPLASH);core->busWrite8(core,bmon(b)+ABI_BMON_PP+m,40);CALL(SetMonMoveSlot,enemy(b>>1),MOVE_SPLASH,m);}
 }
}
static void electricImmunityTests(void){
 for(unsigned electric=0;electric<3;electric++){
  effectBattleFixture(0,MOVE_SPLASH);
  core->busWrite8(core,bmon(0)+ABI_BMON_TYPE1,electric==1?TYPE_ELECTRIC:TYPE_NORMAL);
  core->busWrite8(core,bmon(0)+ABI_BMON_TYPE2,electric==2?TYPE_ELECTRIC:TYPE_NORMAL);
  for(unsigned m=0;m<4;m++){
   core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*m,MOVE_THUNDER_WAVE);
   core->busWrite8(core,bmon(1)+ABI_BMON_PP+m,20);
  }
  core->busWrite8(core,SYM_gActiveBattler,1);CALL(BattleAI_SetupAIData);
  unsigned ai=core->busRead32(core,core->busRead32(core,SYM_gBattleResources)+ABI_BRES_AI);
  core->busWrite32(core,ai+ABI_AI_FLAGS,1);CALL(BattleAI_ChooseMoveOrAction);
  for(unsigned m=0;m<4;m++)CHECK(core->busRead8(core,ai+ABI_AI_SCORE+m)==(electric?90:100));
 }
 printf("PASS: native bad-move AI evaluates all4 Thunder Wave slots100 normally/90 against either Electric type slot\n");
 const unsigned moves[]={MOVE_THUNDER_WAVE,MOVE_STUN_SPORE,MOVE_ZAP_CANNON};
 for(unsigned electric=0;electric<2;electric++)for(unsigned m=0;m<3;m++){
  effectBattleFixture(0,moves[m]);
  // Immunity applies in either type slot; the second slot is deliberately used.
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,electric?TYPE_ELECTRIC:TYPE_NORMAL);
  core->busWrite16(core,SYM_gBattleWeather,m==2?B_WEATHER_RAIN_PERMANENT:0);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned successes=0;
  for(unsigned trial=0;trial<12;trial++){
   CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0xE1EC0000+trial*7919);
   chooseMove(0,0,1);waitAction();unsigned status=core->busRead32(core,bmon(1)+ABI_BMON_STATUS1);
   if(electric)CHECK(status==0);else successes+=!!(status&STATUS1_PARALYSIS);
   CHECK(monget(player(0),ABI_DATA_PP1)==(m==2?4:!m?19:29));
   if(m==2)CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000);
   if(!electric&&successes)break;
  }
  if(!electric)CHECK(successes);
  free(state);
  printf("PASS: actual move%u Electric%u primary/secondary paralysis immunity, retained damage/onePP and non-Electric control\n",moves[m],electric);
 }
 for(unsigned electric=0;electric<2;electric++){
  effectBattleFixture(0,MOVE_TACKLE);core->busWrite8(core,bmon(0)+ABI_BMON_TYPE1,electric?TYPE_ELECTRIC:TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_STATIC);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned procs=0;
  for(unsigned trial=0;trial<20;trial++){
   CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0x57A71000+trial*7919);
   chooseMove(0,0,1);waitAction();unsigned status=core->busRead32(core,bmon(0)+ABI_BMON_STATUS1);
   if(electric)CHECK(status==0);else procs+=!!(status&STATUS1_PARALYSIS);
   CHECK(monget(player(0),ABI_DATA_PP1)==34);
  }
  if(!electric)CHECK(procs);
  free(state);
  printf("PASS: actual contact Static20 seeded turns Electric%u, correct effective recipient; control procs%u\n",electric,procs);
 }
 // Existing paralysis is not silently cured by the new acquisition immunity.
 effectBattleFixture(0,MOVE_THUNDER_WAVE);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_ELECTRIC);
 core->busWrite32(core,bmon(1)+ABI_BMON_STATUS1,STATUS1_PARALYSIS);
 chooseMove(0,0,1);waitAction();CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)==STATUS1_PARALYSIS);
 CHECK(core->loadState(core,bootState));
}
static void spreadStatusTests(void){
 const unsigned moves[]={MOVE_SCARY_FACE,MOVE_SCREECH,MOVE_FAKE_TEARS};
 const unsigned stats[]={STAT_SPEED,STAT_DEF,STAT_SPDEF};
 const unsigned pp[]={15,40,20};
 for(unsigned m=0;m<3;m++)for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,moves[m]);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned both=0,split=0;
  for(unsigned trial=0;trial<50;trial++){
   CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0x5A7A0000+m*97+trial*7919);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);waitAction();
   unsigned a=core->busRead8(core,bmon(1)+ABI_BMON_STAGES+stats[m]);unsigned b=two?core->busRead8(core,bmon(3)+ABI_BMON_STAGES+stats[m]):a;
   CHECK((a==4||a==6)&&(b==4||b==6));CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
   CHECK(core->busRead8(core,bmon(0)+ABI_BMON_STAGES+stats[m])==6);
   if(two)CHECK(core->busRead8(core,bmon(2)+ABI_BMON_STAGES+stats[m])==6);
   for(unsigned bat=0;bat<(two?4:2);bat++)CHECK(core->busRead16(core,bmon(bat)+ABI_BMON_HP)==1000);
   both+=a==4&&b==4;split+=a!=b;
   if(both&&(!two||m==2||split))break;
  }
  CHECK(both);if(two&&m!=2)CHECK(split);free(state);
  printf("PASS: actual %s move%u both foes stat-2 independently, no ally/user/damage, onePP; both%u split%u\n",two?"Double":"Single",moves[m],both,split);
 }
 // Per-opponent prevention must not abort the other opponent's stat drop.
 for(unsigned guard=0;guard<3;guard++){
  effectBattleFixture(1,MOVE_FAKE_TEARS);
  if(guard==0)core->busWrite8(core,bmon(3)+ABI_BMON_ABILITY,ABILITY_CLEAR_BODY);
  if(guard==1)for(unsigned m=0;m<4;m++){
   core->busWrite16(core,bmon(3)+ABI_BMON_MOVES+2*m,MOVE_PROTECT);
   core->busWrite8(core,bmon(3)+ABI_BMON_PP+m,10);CALL(SetMonMoveSlot,enemy(1),MOVE_PROTECT,m);
  }
  if(guard==2)core->busWrite8(core,bmon(3)+ABI_BMON_STAGES+STAT_SPDEF,0);
  chooseMove(0,0,1);chooseMove(2,0,1);waitAction();
  CHECK(core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_SPDEF)==4);
  CHECK(core->busRead8(core,bmon(3)+ABI_BMON_STAGES+STAT_SPDEF)==(guard==2?0:6));
  CHECK(core->busRead8(core,bmon(0)+ABI_BMON_STAGES+STAT_SPDEF)==6);
  CHECK(core->busRead8(core,bmon(2)+ABI_BMON_STAGES+STAT_SPDEF)==6);
  CHECK(monget(player(0),ABI_DATA_PP1)==19);
 }
 printf("PASS: actual Double Fake Tears preserves separate Clear Body/Protect/stat-floor prevention without suppressing the other foe or charging extraPP\n");
 CHECK(core->loadState(core,bootState));
}
#include "ghost_reworks.c"
#include "thunder_wave.c"
#include "new_fire_moves.c"
#include "new_ghost_moves.c"
#include "new_skitter_slash.c"
#include "new_venom_web.c"
#include "new_bug_statdrops.c"
#include "new_chirp.c"
#include "new_swarm_cloud.c"
#include "new_bellow_rock.c"
#include "new_scorching_sands.c"
#include "new_earth_power.c"
#include "new_ice_fang.c"
#include "new_avalanche.c"
#include "new_moves374.c"
#include "new_payback.c"
#include "new_moves382.c"
#include "new_water_moves385.c"
#include "btl_batch_a.c"
#include "btl_batch_b.c"
static void originalEffectTests(void){
 if(getenv("LIGHTNING_BTL_BATCH_A_ONLY")){batchAExactMechanics();batchANativeTests();batchAFlashPersistence();return;}
 if(getenv("LIGHTNING_BTL_BATCH_B_ONLY")){batchBExactMechanics();batchBNativeTests();batchBFlashPersistence();return;}
 if(getenv("LIGHTNING_SKITTER_SLASH_ONLY")){skitterSlashTests();return;}
 if(getenv("LIGHTNING_VENOM_WEB_ONLY")){venomWebTests();return;}
 if(getenv("LIGHTNING_BUG_STATDROPS_ONLY")){bugStatdropTests();return;}
 if(getenv("LIGHTNING_CHIRP_ONLY")){chirpTests();return;}
 if(getenv("LIGHTNING_SWARM_CLOUD_ONLY")){swarmCloudTests();return;}
 if(getenv("LIGHTNING_BELLOW_ROCK_ONLY")){bellowRockTests();return;}
 if(getenv("LIGHTNING_SCORCHING_SANDS_ONLY")){scorchingSandsTests();return;}
 if(getenv("LIGHTNING_EARTH_POWER_ONLY")){earthPowerTests();return;}
 if(getenv("LIGHTNING_ICE_FANG_ONLY")){iceFangTests();return;}
 if(getenv("LIGHTNING_NEW_GHOST_MOVES_ONLY")){newGhostMoveTests();return;}
 if(getenv("LIGHTNING_NEW_FIRE_MOVES_ONLY")){newFireMoveTests();return;}
 if(getenv("LIGHTNING_THUNDER_WAVE_ONLY")){thunderWaveTests();return;}
 if(getenv("LIGHTNING_GHOST_REWORKS_ONLY")){ghostReworkTests();return;}
 newGhostMoveTests();
 newFireMoveTests();
 skitterSlashTests();
 venomWebTests();
 bugStatdropTests();
 chirpTests();
 swarmCloudTests();
 bellowRockTests();
 scorchingSandsTests();
 earthPowerTests();
 iceFangTests();
 avalancheTests();
 moves374Tests();
 paybackTests();
 moves382Tests();
 waterMoves385Tests();
 waterMoves385Mechanics();
 waterMoves385SavePersistence();
 batchAExactMechanics();
 batchANativeTests();
 batchAFlashPersistence();
 batchBExactMechanics();
 batchBNativeTests();
 batchBFlashPersistence();
 thunderWaveTests();
 ghostReworkTests();
 spreadStatusTests();
 electricImmunityTests();
 effectBattleFixture(1,MOVE_PIN_MISSILE);damageStats();CHECK(baseDamage(MOVE_PIN_MISSILE,0,0,0)==50);
 core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);core->busWrite16(core,SYM_gCurrentMove,MOVE_PIN_MISSILE);CALL(AI_CalcDmg,0,1);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==50);
 for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,MOVE_PIN_MISSILE);core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,200);core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned poison=0,plain=0,miss=0;
  for(unsigned trial=0;trial<40&&(!poison||!plain||!miss);trial++){
   CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0x55140000+trial*7919);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);waitAction();
   unsigned poisoned=!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_POISON);unsigned total=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);CHECK(monget(player(0),ABI_DATA_PP1)==19);
   if(!total){miss++;CHECK(!poisoned);}else{unsigned direct=total-(poisoned?125:0);CHECK(direct>=42&&direct<=50);poison+=poisoned;plain+=!poisoned;}
   if(two){CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);CHECK(core->busRead32(core,bmon(3)+ABI_BMON_STATUS1)==0);}
  }
  CHECK(poison&&plain&&miss);free(state);
  printf("PASS: actual %s Pin Missile native animation/one-hit42..50 direct damage; poison+125 residual, nonpoison and85%%-accuracy miss; onePP, selected target only\n",two?"Double":"Single");
 }
 const unsigned levels[]={1,27,50,100};
 for(unsigned two=0;two<2;two++)for(unsigned i=0;i<4;i++){
  effectBattleFixture(two,MOVE_DRAGON_RAGE);core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,levels[i]);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,1);core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,999);core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,999);core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,999);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_STEEL);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_STEEL);
  chooseMove(0,0,1);if(two)chooseMove(2,0,1);waitAction();
  CHECK(1000-core->busRead16(core,bmon(1)+ABI_BMON_HP)==levels[i]);CHECK(monget(player(0),ABI_DATA_PP1)==9);
  if(two)CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==0);
 }
 printf("PASS: actual Single/Double Dragon Rage native animation/eight turns exactly1/27/50/100 damage despite skewed stats/Steel resistance, no spread or forced exit, onePP\n");
 CHECK(core->loadState(core,bootState));
}
