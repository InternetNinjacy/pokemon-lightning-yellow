// BTL-005 #362: native Bind-class trap execution and existing-art composite.
static void venomWebTests(void){
 for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,MOVE_VENOM_WEB);
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,200);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,1);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,999);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_NORMAL);
  damageStats();
  unsigned expected=baseDamage(MOVE_VENOM_WEB,0,0,0);
  CHECK(expected>=35&&expected<=55);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_VENOM_WEB);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
  unsigned landed=0;
  for(unsigned trial=0;trial<40&&!landed;trial++){
   CHECK(core->loadState(core,state));
   core->busWrite32(core,SYM_gRngValue,0x71C50000+trial*7919);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   waitAction(); // Must complete both native visual portions.
   CHECK(monget(player(0),ABI_DATA_PP1)==19);
   CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)==0);
   CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
   if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
   unsigned trapped=core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_WRAPPED;
   unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
   if(trapped){landed=1;CHECK(dealt>=30&&dealt<=expected+125);}
   else CHECK(!dealt);
  }
  CHECK(landed);
  free(state);
 }
 CHECK(core->loadState(core,bootState));
 printf("PASS: actual Singles/Doubles Venom Web #362 damage/AI, selected foe, onePP, native trap status without poison, composite animation termination\n");
}
