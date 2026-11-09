// BTL-005 #385-386: exercise the real compiled ROM in Singles and Doubles.
// This is an acceptance smoke suite, not a substitute for full Rollout
// five-turn, Defense Curl, and save/PP persistence regression coverage.
static void waterMoves385Tests(void){
 const unsigned moves[]={MOVE_VORTEX_SPIN,MOVE_TIDAL_CRASH};
 const unsigned pp[]={25,20};
 for(unsigned m=0;m<2;m++)for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,moves[m]);
  unsigned flags=core->busRead32(core,SYM_gBattleTypeFlags);damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,180);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,1);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,999);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
  unsigned expected=baseDamage(moves[m],0,0,0);
  CHECK(expected>0&&expected<1000);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite16(core,SYM_gCurrentMove,moves[m]);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  void *snap=malloc(core->stateSize(core));CHECK(snap&&core->saveState(core,snap));
  unsigned hits=0;
  for(unsigned trial=0;trial<40&&!hits;trial++){
   CHECK(core->loadState(core,snap));
   core->busWrite32(core,SYM_gRngValue,0x38538600U+trial*7919U+two*17U+m*31U);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   waitActionBudget(4800);
   unsigned hp=core->busRead16(core,bmon(1)+ABI_BMON_HP);
   CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
   CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
   if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
   if(hp<1000){CHECK(hp>=1000-expected);hits++;}
   if(m==0)CHECK(!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_WRAPPED));
  }
  CHECK(hits);
  free(snap);
  printf("PASS: native water move %u %s physical AI damage, single-target actual animation, PP, no trap\\n",moves[m],two?"Doubles":"Singles");
 }
 CHECK(core->loadState(core,bootState));
}
