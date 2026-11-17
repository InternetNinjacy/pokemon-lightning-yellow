#include "constants/battle_script_commands.h"
// Ice Fang #372: actual compiled ROM Single/Double trials, including transient
// flinch status before the engine legitimately clears it at end of turn.
struct IceFangObserved {unsigned freeze;unsigned flinch;};
static struct IceFangObserved iceFangObservedTurn(void){
 struct IceFangObserved v={0,0};unsigned frame;
 for(frame=0;frame<4800;frame++){
  if(controller(0)==SYM_HandleInputChooseAction)break;
  frames(1,(frame%12)<3?1:0);
  v.freeze |= !!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_FREEZE);
  v.flinch |= !!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_FLINCHED);
 }
 if(frame==4800)fprintf(stderr,"Ice Fang timeout script=%08x ctrl=%08x\n",core->busRead32(core,SYM_gBattlescriptCurrInstr),controller(0));
 CHECK(frame<4800);frames(3,0);return v;
}
// New Tier2 acceptance only: native effects are forced through their real RNG
// command; whole turns exercise the animation/damage/guard combinations.
static unsigned iceFangChanceSeed(unsigned roll){
 unsigned seed=0;while((((seed*1103515245U+24691U)>>16)%100)!=roll)seed++;return seed;
}
static void iceFangAcceptanceGaps(void){
 effectBattleFixture(0,MOVE_ICE_FANG);
 unsigned script=CALL(AllocZeroed,16);CHECK(script);
 void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
 for(unsigned guard=0;guard<8;guard++)for(unsigned effect=0;effect<2;effect++){
  CHECK(core->loadState(core,state));
  core->busWrite8(core,SYM_gBattlerAttacker,0);core->busWrite8(core,SYM_gBattlerTarget,1);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_ICE_FANG);
  core->busWrite8(core,SYM_gMoveResultFlags,0);core->busWrite32(core,SYM_gHitMarker,0);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_NONE);
  core->busWrite8(core,SYM_gBattlerByTurnOrder,guard==3?1:0);core->busWrite8(core,SYM_gBattlerByTurnOrder+1,guard==3?0:1);
  core->busWrite8(core,SYM_gCurrentTurnActionNumber,guard==3?1:0);
  if(guard==1){core->busWrite32(core,bmon(1)+ABI_BMON_STATUS2,STATUS2_SUBSTITUTE);core->busWrite8(core,SYM_gDisableStructs+ABI_DISABLE_SIZE+ABI_DISABLE_SUB_HP,200);}
  if(guard==2)core->busWrite16(core,SYM_gSideStatuses+2,SIDE_STATUS_SAFEGUARD);
  if(guard==4){core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_ICE);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_ICE);}
  if(guard>=5)core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,guard==5?ABILITY_MAGMA_ARMOR:guard==6?ABILITY_INNER_FOCUS:ABILITY_SHIELD_DUST);
  core->busWrite8(core,SYM_gBattleCommunication+MOVE_EFFECT_BYTE,effect?MOVE_EFFECT_FLINCH:MOVE_EFFECT_FREEZE);
  core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(0));
  core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_seteffectwithchance);
  unsigned applied=effect?!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_FLINCHED):!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_FREEZE);
  CHECK(applied==(effect?(guard==0||guard==2||guard==4||guard==5):(guard==0||guard==3||guard==6)));
 }
 const unsigned rolls[]={9,10,19,20};
 for(unsigned serene=0;serene<2;serene++)for(unsigned r=0;r<4;r++){
  CHECK(core->loadState(core,state));core->busWrite8(core,SYM_gBattlerAttacker,0);core->busWrite8(core,SYM_gBattlerTarget,1);
  core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,serene?ABILITY_SERENE_GRACE:ABILITY_NONE);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_NONE);core->busWrite16(core,SYM_gCurrentMove,MOVE_ICE_FANG);
  core->busWrite8(core,SYM_gMoveResultFlags,0);core->busWrite32(core,SYM_gHitMarker,0);
  core->busWrite8(core,SYM_gBattleCommunication+MOVE_EFFECT_BYTE,MOVE_EFFECT_FREEZE);
  core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(rolls[r]));core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_seteffectwithchance);
  CHECK(!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_FREEZE)==(rolls[r]<(serene?20:10)));
 }
 unsigned accurate=0;
 for(unsigned roll=0;roll<100;roll++){
  CHECK(core->loadState(core,state));core->busWrite8(core,SYM_gBattlerAttacker,0);core->busWrite8(core,SYM_gBattlerTarget,1);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_ICE_FANG);core->busWrite8(core,SYM_gMoveResultFlags,0);core->busWrite32(core,SYM_gHitMarker,0);
  for(unsigned b=0;b<4;b++)core->busWrite8(core,script+1+b,(script+12)>>(8*b));
  core->busWrite8(core,script+5,MOVE_ICE_FANG&255);core->busWrite8(core,script+6,MOVE_ICE_FANG>>8);core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);
  core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(roll));CALL(Cmd_accuracycheck);
  unsigned hit=core->busRead32(core,SYM_gBattlescriptCurrInstr)==script+7;CHECK(hit==(roll<95));accurate+=hit;
 }
 CHECK(accurate==95);
 CHECK(core->loadState(core,state));CALL(Free,script);free(state);
 for(unsigned two=0;two<2;two++)for(unsigned guard=0;guard<4;guard++){
  effectBattleFixture(two,MOVE_ICE_FANG);CHECK(core->busRead8(core,SYM_gBattlersCount)==(two?4:2));CHECK(!!(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_DOUBLE)==two);unsigned battleFlags=core->busRead32(core,SYM_gBattleTypeFlags);damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,battleFlags);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,guard==3?1:300);core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,guard==3?300:1);
  if(guard<2){core->busWrite32(core,bmon(1)+ABI_BMON_STATUS2,STATUS2_SUBSTITUTE);core->busWrite8(core,SYM_gDisableStructs+ABI_DISABLE_SIZE+ABI_DISABLE_SUB_HP,guard?1:200);}
  if(guard==2){core->busWrite16(core,SYM_gSideStatuses+2,SIDE_STATUS_SAFEGUARD);core->busWrite8(core,SYM_gSideTimers+ABI_SIDE_TIMER_SIZE+ABI_SIDE_SAFEGUARD_TIMER,5);}
  if(guard==3)for(unsigned k=0;k<4;k++){core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*k,MOVE_SWIFT);core->busWrite8(core,bmon(1)+ABI_BMON_PP+k,20);CALL(SetMonMoveSlot,enemy(0),MOVE_SWIFT,k);}
  void *initial=malloc(core->stateSize(core));CHECK(initial&&core->saveState(core,initial));unsigned hit=0;
  for(unsigned trial=0;trial<8&&!hit;trial++){
   CHECK(core->loadState(core,initial));core->busWrite32(core,SYM_gRngValue,0x1CE50000+trial*7919+guard*31+two*17);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);struct IceFangObserved v=iceFangObservedTurn();
   CHECK(monget(player(0),ABI_DATA_PP1)==14);
   if(guard<2){hit=core->busRead8(core,SYM_gDisableStructs+ABI_DISABLE_SIZE+ABI_DISABLE_SUB_HP)<(guard?1:200);CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==1000);CHECK(!v.freeze&&!v.flinch);}
   else {hit=core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000;if(guard==2)CHECK(!v.freeze);else {CHECK(!v.flinch);CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)<1000);unsigned used=0;for(unsigned k=0;k<4;k++)used+=20-monget(enemy(0),ABI_DATA_PP1+k);CHECK(used==1);}}
   if(two){CHECK(guard==3?core->busRead16(core,bmon(2)+ABI_BMON_HP)<1000:core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
  }
  CHECK(hit);free(initial);
  printf("PASS: Ice Fang actual %s guard%u: surviving/broken Substitute or Safeguard or already-acted foe, damage/onePP/no spread and legitimate flinch timing\n",two?"Doubles":"Singles",guard);
 }
 CHECK(core->loadState(core,bootState));
 printf("PASS: native forced Ice Fang freeze/flinch guard matrix, late-target negative and exact10/20%% boundary controls; no gameplay code change\n");
}
// Recorded from exact-head59dd4324 native discovery, including genuine Doubles.
// Replay each actual outcome and miss directly instead of repeating RNG searches.
static const unsigned iceFangReplaySeeds[2][2][5]={
 {{0x37200000,0x37201EEF,0x37209AAB,0x3726866A,0x37220DDF},
  {0x37211EF0,0x37215CCE,0x37217BBD,0x37210001,0x37236AAD}},
 {{0x37200011,0x3720B9AB,0x37203DEF,0x3730EAC5,0x37215456},
  {0x37211F01,0x37210012,0x3721B9AC,0x3725789D,0x37215CDF}}
};
static void iceFangTests(void){
 if(!getenv("LIGHTNING_ICE_FANG_DOUBLES_ONLY"))iceFangAcceptanceGaps();
 if(getenv("LIGHTNING_ICE_FANG_GAPS_ONLY"))return;
 for(unsigned two=getenv("LIGHTNING_ICE_FANG_DOUBLES_ONLY")?1:0;two<2;two++){
  effectBattleFixture(two,MOVE_ICE_FANG);CHECK(core->busRead8(core,SYM_gBattlersCount)==(two?4:2));CHECK(!!(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_DOUBLE)==two);unsigned battleFlags=core->busRead32(core,SYM_gBattleTypeFlags);damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,battleFlags);
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,200);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,1);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,999);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
  unsigned expected=baseDamage(MOVE_ICE_FANG,0,0,0);
  CHECK(expected>25&&expected<140);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_ICE_FANG);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  void *initial=malloc(core->stateSize(core));CHECK(initial&&core->saveState(core,initial));
  for(unsigned serene=0;serene<2;serene++){
   CHECK(core->loadState(core,initial));
   core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,serene?ABILITY_SERENE_GRACE:ABILITY_NONE);
   void *snap=malloc(core->stateSize(core));CHECK(snap&&core->saveState(core,snap));
   unsigned combinations=0,misses=0;
   // The original five replay seeds were recorded on a prior source head.
   // Doubles action ordering can consume RNG differently: replay each known
   // seed first, then search a bounded deterministic sequence for EACH exact
   // independent status combination and one real miss. Do not accept fewer
   // outcomes, weaken combat/PP checks, or rely on a particular old seed.
   for(unsigned trial=0;trial<5;trial++){
    unsigned found=0;
    for(unsigned attempt=0;attempt<512&&!found;attempt++){
     unsigned seed=attempt<5?iceFangReplaySeeds[two][serene][attempt]:0x37200000U+attempt*7919U+two*17U+serene*29U;
     CHECK(core->loadState(core,snap));
     core->busWrite32(core,SYM_gRngValue,seed);
     chooseMove(0,0,1);if(two)chooseMove(2,0,1);
     struct IceFangObserved observed=iceFangObservedTurn();
     unsigned hp=core->busRead16(core,bmon(1)+ABI_BMON_HP);
     CHECK(monget(player(0),ABI_DATA_PP1)==14);
     CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
     if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
     if(hp==1000){
      CHECK(!observed.freeze&&!observed.flinch);
      if(trial!=4)continue;
      misses++;found=1;
      printf("ICEFANG_SEED two%u serene%u miss seed%08x\\n",two,serene,seed);
     }else{
      CHECK(hp<1000&&hp>=1000-expected);
      unsigned combination=observed.freeze+2*observed.flinch;
      if(trial==4||combination!=trial)continue;
      CHECK(combination==trial);
      combinations|=1U<<combination;found=1;
      printf("ICEFANG_SEED two%u serene%u outcome%u seed%08x\\n",two,serene,combination,seed);
     }
    }
    if(!found)fprintf(stderr,"ICEFANG_OUTCOME_UNREACHABLE two=%u serene=%u outcome=%u within512 trials\\n",two,serene,trial);
    CHECK(found);
   }
   CHECK(combinations==15&&misses>0);
   free(snap);
   printf("PASS: Ice Fang native %s %s all four independent freeze/flinch outcomes, misses, PP, Physical AI damage and existing-art termination\n",two?"Doubles":"Singles",serene?"Serene Grace":"base chance");
  }
  free(initial);
 }
 if(getenv("LIGHTNING_ICE_FANG_DOUBLES_ONLY")){CHECK(core->loadState(core,bootState));return;}
 // Native immunity/ability controls. Fresh independent battle for each variant.
 // Keep HP=1000; discover only the first required positive control, not120
 // duplicate animated trials. Exact guard/rate assertions execute natively above.
 for(unsigned variant=0;variant<3;variant++){
  effectBattleFixture(0,MOVE_ICE_FANG);unsigned battleFlags=core->busRead32(core,SYM_gBattleTypeFlags);damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,battleFlags);
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,160);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,variant==0?TYPE_ICE:TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,variant==0?TYPE_ICE:TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,variant==1?ABILITY_INNER_FOCUS:(variant==2?ABILITY_SHIELD_DUST:ABILITY_BATTLE_ARMOR));
  void *snapshot=malloc(core->stateSize(core));CHECK(snapshot&&core->saveState(core,snapshot));
  unsigned hit=0,freeze=0,flinch=0;
  for(unsigned trial=0;trial<64&&(variant==2?!hit:variant==0?!flinch:!freeze);trial++){
   CHECK(core->loadState(core,snapshot));
   core->busWrite32(core,SYM_gRngValue,0x1CE00000+trial*7919+variant*71);
   chooseMove(0,0,1);
   struct IceFangObserved v=iceFangObservedTurn();
   CHECK(monget(player(0),ABI_DATA_PP1)==14);
   if(core->busRead16(core,bmon(1)+ABI_BMON_HP)==1000){
    CHECK(!v.freeze&&!v.flinch);
   }else{hit++;freeze+=v.freeze;flinch+=v.flinch;}
  }
  CHECK(hit>0); // Exact95/100 native accuracy is independently exhaustive above.
  if(variant==0){CHECK(freeze==0);CHECK(flinch>0);} // Ice type can flinch but cannot freeze
  if(variant==1){CHECK(flinch==0);CHECK(freeze>0);} // Inner Focus prevents flinch only
  if(variant==2){CHECK(freeze==0);CHECK(flinch==0);} // Shield Dust prevents secondary statuses
  free(snapshot);
  printf("PASS: Ice Fang native ability/type control %u: %u hits, %u frozen, %u flinched\\n",variant,hit,freeze,flinch);
 }
 CHECK(core->loadState(core,bootState));
}
