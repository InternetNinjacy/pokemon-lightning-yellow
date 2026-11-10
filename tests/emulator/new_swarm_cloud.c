// Real compiled ROM: independent 50% confusion and poison rolls for Swarm Cloud.
static void swarmCloudTests(void){
 const unsigned moves[]={MOVE_SWARM_CLOUD};
 for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,moves[0]);
  damageStats();
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,1);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,200);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,999);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,100);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);

  unsigned expected=baseDamage(MOVE_SWARM_CLOUD,0,0,0);
  CHECK(expected>60&&expected<95);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_SWARM_CLOUD);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  void *snapshot=malloc(core->stateSize(core));CHECK(snapshot&&core->saveState(core,snapshot));
  unsigned combinations=0,misses=0;
  for(unsigned trial=0;trial<100&&combinations!=15;trial++){
   CHECK(core->loadState(core,snapshot));
   core->busWrite32(core,SYM_gRngValue,0x2ABA0000+trial*7919);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   // Double-status turns include two end-of-turn messages/animations; retain the same completion gate.
   waitActionBudget(4800);
   unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
   unsigned confused=!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_CONFUSION);
   unsigned poisoned=!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_POISON);
   CHECK(monget(player(0),ABI_DATA_PP1)==14);
   CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
   if(dealt>0)combinations|=1U<<(confused+2*poisoned);
   else{misses++;CHECK(!confused&&!poisoned);}
   if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
  }
  CHECK(combinations==15);CHECK(misses>0);
  free(snapshot);
 }
 CHECK(core->loadState(core,bootState));
 printf("PASS: native Singles/Doubles Swarm Cloud #366 AI/Special damage and all four independent confusion/poison outcome combinations; onePP, original visuals, selected foe\n");
}
