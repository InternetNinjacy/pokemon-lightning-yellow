// Ice Fang #372. Actual compiled GBA in Singles/Doubles with fresh RNG trials.
static void iceFangTests(void){
 for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,MOVE_ICE_FANG);damageStats();
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
  void *snap=malloc(core->stateSize(core));CHECK(snap&&core->saveState(core,snap));
  unsigned frozen=0,ordinary=0,misses=0;
  for(unsigned trial=0;trial<220&&(!frozen||!ordinary||!misses);trial++){
   CHECK(core->loadState(core,snap));
   core->busWrite32(core,SYM_gRngValue,0x37200000+trial*7919+two*17);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   waitActionBudget(4800);
   unsigned hp=core->busRead16(core,bmon(1)+ABI_BMON_HP);
   unsigned fr=!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_FREEZE);
   CHECK(monget(player(0),ABI_DATA_PP1)==14);
   CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
   if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
   if(hp==1000){misses++;CHECK(!fr);}
   else{CHECK(hp<1000&&hp>=1000-expected);frozen+=fr;ordinary+=!fr;}
  }
  CHECK(frozen&&ordinary&&misses);free(snap);
  printf("PASS: Ice Fang native %s physical damage/AI, PP, freeze+ordinary+miss, selected foe, composite visual termination\n",two?"Doubles":"Singles");
 }
 CHECK(core->loadState(core,bootState));
}
