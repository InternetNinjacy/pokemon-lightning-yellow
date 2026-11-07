// Native mGBA acceptance for approved Scorching Sands #370.
// Never patch battle-memory effect or animations: run actual source-built ROM.
static void scorchingSandsTests(void){
 for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,MOVE_SCORCHING_SANDS);
  damageStats();
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,1);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,200);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,999);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,100);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
  unsigned expected=baseDamage(MOVE_SCORCHING_SANDS,0,0,0);
  CHECK(expected>50&&expected<90);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_SCORCHING_SANDS);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
  unsigned burned=0,plain=0;
  for(unsigned trial=0;trial<36&&(!burned||!plain);trial++){
   CHECK(core->loadState(core,state));
   core->busWrite32(core,SYM_gRngValue,0x5CA70000+trial*7919+two*31);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   waitActionBudget(4800);
   unsigned isBurn=!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_BURN);
   unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
   CHECK(dealt>0&&dealt<=expected+125);
   CHECK(monget(player(0),ABI_DATA_PP1)==9);
   CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
   if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
   burned+=isBurn;plain+=!isBurn;
  }
  CHECK(burned&&plain);
  free(state);
  printf("PASS: native Scorching Sands #370 %s Special/AI damage, burn/no-burn, one PP, selected target, Sandstorm+Ember visual termination\n",two?"Doubles":"Singles");
 }
 // Flying-type target: Ground move deals zero damage, no imported Fire effectiveness.
 effectBattleFixture(0,MOVE_SCORCHING_SANDS);
 core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_FLYING);
 core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_FLYING);
 chooseMove(0,0,1);waitActionBudget(4800);
 CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==1000);
 CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)==0);
 CHECK(monget(player(0),ABI_DATA_PP1)==9);
 // Fire targets can be hit by Ground damage but must not burn.
 effectBattleFixture(0,MOVE_SCORCHING_SANDS);
 core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_FIRE);
 core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_FIRE);
 chooseMove(0,0,1);waitActionBudget(4800);
 CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000);
 CHECK(!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_BURN));
 CHECK(monget(player(0),ABI_DATA_PP1)==9);
 CHECK(core->loadState(core,bootState));
 printf("PASS: Scorching Sands #370 Ground immunity, Fire status immunity, one PP, no new assets or distribution\n");
}
