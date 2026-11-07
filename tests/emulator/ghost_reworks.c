// Project-written guarded native tests; no replacement game routines.
static void ghostReworkTests(void){
 effectBattleFixture(0,MOVE_SPITE);damageStats();
 core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_WATER);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_WATER);
 CHECK(baseDamage(MOVE_GRUDGE,0,0,0)==5);
 CHECK(baseDamage(MOVE_SPITE,0,0,0)==5);
 core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);CALL(AI_CalcDmg,0,1);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==5);
 for(unsigned scenario=0;scenario<7;scenario++){
  effectBattleFixture(0,MOVE_SPITE);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,scenario==6?TYPE_NORMAL:TYPE_WATER);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,scenario==6?TYPE_NORMAL:TYPE_WATER);
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,200);core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,100);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,200);core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,1);
  // Sleep isolates PP depletion from the opponent spending PP on its own turn.
  core->busWrite32(core,bmon(1)+ABI_BMON_STATUS1,3);
  unsigned last=scenario==1?MOVE_NONE:scenario==2?0xFFFF:scenario==3?MOVE_TACKLE:MOVE_SPLASH;
  core->busWrite16(core,SYM_gLastMoves+2,last);
  unsigned initial=scenario==4?1:scenario==5?0:40;
  core->busWrite8(core,bmon(1)+ABI_BMON_PP,initial);monput(enemy(0),ABI_DATA_PP1,initial);
  chooseMove(0,0,1);waitAction();unsigned total=0;for(unsigned m=0;m<4;m++)total+=core->busRead8(core,bmon(1)+ABI_BMON_PP+m);
  unsigned drain=initial+120-total;
  if(scenario==0)CHECK(drain>=2&&drain<=5);else CHECK(drain==0);
  unsigned damage=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
  if(scenario==6)CHECK(damage==0);else CHECK(damage>=23&&damage<=28);
  CHECK(monget(player(0),ABI_DATA_PP1)==9);
  printf("PASS: actual Spite scenario%u Ghost Special damage%u, optional PP drain%u, onePP; no eligible move does not block damage\n",scenario,damage,drain);
 }
 effectBattleFixture(0,MOVE_GRUDGE);unsigned script=CALL(AllocZeroed,16);CHECK(script);
 for(unsigned move=0;move<2;move++)for(unsigned curl=0;curl<2;curl++){
  core->busWrite8(core,SYM_gBattlerAttacker,0);core->busWrite16(core,SYM_gCurrentMove,move?MOVE_ROLLOUT:MOVE_GRUDGE);
  core->busWrite32(core,bmon(0)+ABI_BMON_STATUS2,curl?STATUS2_DEFENSE_CURL:0);core->busWrite8(core,SYM_gMoveResultFlags,0);
  for(unsigned turn=0;turn<5;turn++){
   core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_rolloutdamagecalculation);
   CHECK(core->busRead16(core,SYM_gDynamicBasePower)==(30U<<turn)*(move&&curl?2:1));
   CHECK(!!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS)==(turn<4));
   CHECK(core->busRead16(core,SYM_gLockedMoves)==(move?MOVE_ROLLOUT:MOVE_GRUDGE));
  }
 }
 core->busWrite16(core,SYM_gCurrentMove,MOVE_GRUDGE);core->busWrite32(core,bmon(0)+ABI_BMON_STATUS2,0);core->busWrite8(core,SYM_gMoveResultFlags,0);core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_rolloutdamagecalculation);
 core->busWrite8(core,SYM_gMoveResultFlags,MOVE_RESULT_MISSED);core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_rolloutdamagecalculation);CHECK(!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS));
 CALL(Free,script);
 printf("PASS: native Grudge30/60/120/240/480 with/without Defense Curl, five-turn lock/end/miss cancellation; original Rollout curl bonus preserved\n");
 effectBattleFixture(0,MOVE_GRUDGE);
 core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_WATER);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_WATER);
 core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,100);core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,100);
 core->busWrite32(core,bmon(0)+ABI_BMON_STATUS2,STATUS2_DEFENSE_CURL);
 void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned complete=0;
 for(unsigned trial=0;trial<20&&!complete;trial++){
  CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0x6D760000+trial*7919);chooseMove(0,0,1);
  unsigned previous=1000,hits=0,n;
  for(n=0;n<20000;n++){
   frames(1,(n%12)<3?1:0);unsigned hp=core->busRead16(core,bmon(1)+ABI_BMON_HP);
   if(hp<previous){CHECK(hits<5);CHECK(core->busRead16(core,SYM_gDynamicBasePower)==(30U<<hits));hits++;previous=hp;if(hits==5)break;}
   if(n>30&&controller(0)==SYM_HandleInputChooseAction&&!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS))break;
  }
  CHECK(n<20000);waitAction();CHECK(monget(player(0),ABI_DATA_PP1)==9);
  CHECK(!(core->busRead32(core,SYM_gStatuses3)&STATUS3_GRUDGE));
  if(hits==5){complete=1;CHECK(!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS));}
 }
 CHECK(complete);free(state);
 printf("PASS: actual Grudge five successive native turns/animations powers30/60/120/240/480 despite Defense Curl, onePP9, lock cleared, no old KO-trigger status\n");
 CHECK(core->loadState(core,bootState));
}
