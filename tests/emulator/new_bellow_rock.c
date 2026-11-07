// Native GBA battle-turn acceptance for approved Bellow/Power Gem/Stone Edge #367-369.
static void bellowRockTests(void){
 const unsigned moves[]={MOVE_BELLOW,MOVE_POWER_GEM,MOVE_STONE_EDGE};
 const unsigned pp[]={30,20,10};
 for(unsigned m=0;m<3;m++)for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,moves[m]);
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,200);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,200);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,100);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_NONE);
  damageStats();
  unsigned expected=baseDamage(moves[m],0,0,0);
  CHECK(expected>0);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
  core->busWrite16(core,SYM_gCurrentMove,moves[m]);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
  unsigned hit=0;
  for(unsigned trial=0;trial<24&&!hit;trial++){
   CHECK(core->loadState(core,state));
   core->busWrite32(core,SYM_gRngValue,0xB3670000+m*173+two*31+trial*7919);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   waitAction();
   hit=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
   CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
   CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)==0);
   CHECK(core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_ATK)==6);
   CHECK(core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_SPATK)==6);
   CHECK(core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_DEF)==6);
   CHECK(core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_SPDEF)==6);
   CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
   if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
   CHECK(core->busRead8(core,SYM_gBattleOutcome)==0);
  }
  CHECK(hit>0);
  free(state);
 }
 // Soundproof must block Bellow damage (and AI should avoid the immune target).
 effectBattleFixture(0,MOVE_BELLOW);
 core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_SOUNDPROOF);
 chooseMove(0,0,1);waitAction();
 CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==1000);
 CHECK(monget(player(0),ABI_DATA_PP1)==29);
 effectBattleFixture(0,MOVE_SPLASH);
 core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,ABILITY_SOUNDPROOF);
 for(unsigned m=0;m<4;m++){core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*m,MOVE_BELLOW);core->busWrite8(core,bmon(1)+ABI_BMON_PP+m,30);}
 core->busWrite8(core,SYM_gActiveBattler,1);CALL(BattleAI_SetupAIData);
 unsigned ai=core->busRead32(core,core->busRead32(core,SYM_gBattleResources)+ABI_BRES_AI);
 core->busWrite32(core,ai+ABI_AI_FLAGS,1);CALL(BattleAI_ChooseMoveOrAction);
 for(unsigned m=0;m<4;m++)CHECK(core->busRead8(core,ai+ABI_AI_SCORE+m)==90);
 CHECK(core->loadState(core,bootState));
 printf("PASS: native #367-369 Singles/Doubles selected-target moves, category/AI damage, PP, no imported status, animation return and Bellow Soundproof/AI\n");
}
