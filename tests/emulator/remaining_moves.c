// Risk-tiered acceptance for approved remaining moves; actual compiled ARM code.
static void remainingFixture(unsigned two,unsigned move){
 effectBattleFixture(two,move);unsigned flags=core->busRead32(core,SYM_gBattleTypeFlags);damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
 CHECK(!!(flags&BATTLE_TYPE_DOUBLE)==two);CHECK(core->busRead8(core,SYM_gBattlersCount)==(two?4:2));
 core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,100);core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,200);core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,600);
 for(unsigned b=1;b<(two?4:2);b++){core->busWrite16(core,bmon(b)+ABI_BMON_DEFENSE,100);core->busWrite16(core,bmon(b)+ABI_BMON_SPDEFENSE,100);core->busWrite8(core,bmon(b)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);}
}
static void waterDischargeTests(void){
 const unsigned moves[]={MOVE_VORTEX_SPIN,MOVE_TIDAL_CRASH,MOVE_DISCHARGE},pp[]={25,20,15};
 for(unsigned m=0;m<3;m++)for(unsigned two=0;two<2;two++){
  remainingFixture(two,moves[m]);void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned hit=0;
  for(unsigned trial=0;trial<8&&!hit;trial++){
   CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0x38500000+m*163+two*17+trial*7919);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   if(m==1){
    unsigned f;for(f=0;f<4800;f++){frames(1,(f%12)<3?1:0);if(core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000){hit=1;break;}if(f>30&&controller(0)==SYM_HandleInputChooseAction)break;}
    CHECK(f<4800);if(hit)CHECK(core->busRead16(core,SYM_gDynamicBasePower)==30);
   }else{waitAction();hit=core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000;}
   CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
   if(m==0)CHECK(!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_WRAPPED));
   if(two){if(m==2){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)<1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)<1000);}else CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
   CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
  }
  CHECK(hit);free(state);printf("PASS: actual %s move%u damage/target scope/onePP/existing art; no Vortex trap and retained Tidal rollout power\n",two?"Doubles":"Singles",moves[m]);
 }
 // The unmodified native Rollout command supplies all five powers and Curl.
 remainingFixture(0,MOVE_TIDAL_CRASH);unsigned script=CALL(AllocZeroed,16);CHECK(script);
 for(unsigned curl=0;curl<2;curl++){
  core->busWrite8(core,SYM_gBattlerAttacker,0);core->busWrite16(core,SYM_gCurrentMove,MOVE_TIDAL_CRASH);core->busWrite32(core,bmon(0)+ABI_BMON_STATUS2,curl?STATUS2_DEFENSE_CURL:0);core->busWrite8(core,SYM_gMoveResultFlags,0);
  for(unsigned turn=0;turn<5;turn++){core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_rolloutdamagecalculation);CHECK(core->busRead16(core,SYM_gDynamicBasePower)==(30U<<turn)*(curl?2:1));CHECK(!!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS)==(turn<4));CHECK(core->busRead16(core,SYM_gLockedMoves)==MOVE_TIDAL_CRASH);}
 }
 core->busWrite8(core,SYM_gMoveResultFlags,MOVE_RESULT_MISSED);core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_rolloutdamagecalculation);CHECK(!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS));CALL(Free,script);
 // Ground immunity, Electric paralysis immunity and per-recipient Volt Absorb.
 remainingFixture(1,MOVE_DISCHARGE);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_GROUND);core->busWrite8(core,bmon(2)+ABI_BMON_TYPE1,TYPE_ELECTRIC);core->busWrite8(core,bmon(3)+ABI_BMON_ABILITY,ABILITY_VOLT_ABSORB);
 chooseMove(0,0,1);chooseMove(2,0,1);waitAction();CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)<1000);CHECK(core->busRead32(core,bmon(2)+ABI_BMON_STATUS1)==0);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);CHECK(monget(player(0),ABI_DATA_PP1)==14);
 CHECK(core->loadState(core,bootState));printf("PASS: native Tidal five-power/Curl/lock/miss mechanics and actual Discharge independent Ground/Electric/VoltAbsorb controls\n");
}
static void voltSwitchTests(void){
 // Real native turns: no reserve, valid switch, Substitute, Ground, absorption,
 // Protect and genuine Doubles. Only reserve selection is driven by input.
 for(unsigned scenario=0;scenario<7;scenario++){
  unsigned two=scenario==6,reserve=two?2:1,canSwitch=scenario==1||scenario==2||scenario==6;
  remainingFixture(two,MOVE_VOLT_SWITCH);core->busWrite8(core,bmon(0)+ABI_BMON_STAGES+STAT_ATK,12);
  if(scenario){CALL(CreateMon,player(reserve),SPECIES_SNORLAX,50,31,1,54321,0,1234);for(unsigned k=0;k<4;k++)CALL(SetMonMoveSlot,player(reserve),MOVE_SPLASH,k);core->busWrite8(core,SYM_gPlayerPartyCount,reserve+1);}
  if(scenario==2){core->busWrite32(core,bmon(1)+ABI_BMON_STATUS2,STATUS2_SUBSTITUTE);core->busWrite8(core,SYM_gDisableStructs+ABI_DISABLE_SIZE+ABI_DISABLE_SUB_HP,200);}
  if(scenario==3)core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_GROUND);
  if(scenario==4)core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_VOLT_ABSORB);
  if(scenario==5){core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,650);for(unsigned k=0;k<4;k++){CALL(SetMonMoveSlot,enemy(0),MOVE_PROTECT,k);core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*k,MOVE_PROTECT);core->busWrite8(core,bmon(1)+ABI_BMON_PP+k,10);}}
  chooseMove(0,0,1);if(two)chooseMove(2,0,1);unsigned opened=0,selected=0,f;
  for(f=0;f<6000;f++){
   if(CALL(FuncIsActiveTask,SYM_Task_HandleChooseMonInput|1U)){
    opened=1;CHECK(canSwitch);frames(24,0);
    unsigned n;for(n=0;n<8&&core->busRead8(core,SYM_gPartyMenu+ABI_PARTY_MENU_SLOT)!=reserve;n++)press(16);
    CHECK(n<8);press(1);frames(24,0);press(1);selected=1;
   }
   if(f>10&&controller(0)==SYM_HandleInputChooseAction)break;
   frames(1,(f%12)<3?1:0);
  }
  if(f==6000)fprintf(stderr,"Volt switch timeout scenario%u cb%08x ctrl%08x script%08x\n",scenario,core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),controller(0),core->busRead32(core,SYM_gBattlescriptCurrInstr));
  CHECK(f<6000);CHECK(opened==canSwitch);CHECK(selected==canSwitch);CHECK(monget(player(0),ABI_DATA_PP1)==19);
  CHECK(core->busRead16(core,SYM_gBattlerPartyIndexes)==(canSwitch?reserve:0));
  if(canSwitch)CHECK(core->busRead8(core,bmon(0)+ABI_BMON_STAGES+STAT_ATK)==6); // no Baton Pass stages
  unsigned hp=core->busRead16(core,bmon(1)+ABI_BMON_HP);
  if(scenario==2){CHECK(hp==1000);CHECK(core->busRead8(core,SYM_gDisableStructs+ABI_DISABLE_SIZE+ABI_DISABLE_SUB_HP)<200);}
  else if(scenario>=3&&scenario<=5)CHECK(hp==1000);else CHECK(hp<1000);
  if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
  CHECK(core->busRead8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_SWITCH_PENDING)==0);
  printf("PASS: actual Volt Switch scenario%u %s: damage/guard, party%s, onePP and no inherited stages/repeated switch\n",scenario,two?"Doubles":"Singles",canSwitch?" selected reserve":" not opened");
 }
 CHECK(core->loadState(core,bootState));
}

static void voltSwitchAdditionalTests(void){
 remainingFixture(0,MOVE_VOLT_SWITCH);CHECK(ABI_BATTLESTRUCT_SIZE==512);CHECK(ABI_BATTLESTRUCT_SWITCH_PENDING==0x1ED);
 unsigned script=CALL(AllocZeroed,16);CHECK(script);void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
 for(unsigned pending=0;pending<2;pending++)for(unsigned alive=0;alive<2;alive++)for(unsigned foe=0;foe<2;foe++){
  CHECK(core->loadState(core,state));core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_SWITCH_PENDING,pending);core->busWrite16(core,bmon(0)+ABI_BMON_HP,alive?1000:0);monput(enemy(0),ABI_DATA_HP,foe?1000:0);
  core->busWrite8(core,SYM_gBattlerAttacker,0);core->busWrite8(core,script+1,BS_ATTACKER);core->busWrite8(core,script+2,ABI_PREPARE_HIT_SWITCH);core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_various);
  CHECK(core->busRead8(core,SYM_gBattleCommunication)==(pending&&alive&&foe));CHECK(core->busRead8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_SWITCH_PENDING)==0);
 }
 CHECK(core->loadState(core,state));CALL(Free,script);free(state);
 remainingFixture(0,MOVE_SPLASH);CALL(CreateMon,enemy(1),SPECIES_SNORLAX,50,31,1,54321,0,1234);for(unsigned k=0;k<4;k++)CALL(SetMonMoveSlot,enemy(1),MOVE_SPLASH,k);core->busWrite8(core,SYM_gEnemyPartyCount,2);core->busWrite32(core,SYM_gBattleTypeFlags,core->busRead32(core,SYM_gBattleTypeFlags)|BATTLE_TYPE_TRAINER);
 for(unsigned k=0;k<4;k++){CALL(SetMonMoveSlot,enemy(0),MOVE_VOLT_SWITCH,k);core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*k,MOVE_VOLT_SWITCH);core->busWrite8(core,bmon(1)+ABI_BMON_PP+k,20);}
 chooseMove(0,0,1);waitAction();CHECK(core->busRead16(core,SYM_gBattlerPartyIndexes+2)==1);CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)<1000);unsigned pp=0;for(unsigned k=0;k<4;k++)pp+=monget(enemy(0),ABI_DATA_PP1+k);CHECK(pp==79);
 CHECK(core->loadState(core,bootState));printf("PASS: native Volt Switch pending/alive/opponent gate8cases and actual opponent AI reserve selection/return, onePP, no party UI for player\n");
}

static void remainingMoveTests(void){if(getenv("LIGHTNING_VOLT_SWITCH_AI_ONLY")){voltSwitchAdditionalTests();return;}if(!getenv("LIGHTNING_VOLT_SWITCH_ONLY"))waterDischargeTests();voltSwitchTests();voltSwitchAdditionalTests();}
