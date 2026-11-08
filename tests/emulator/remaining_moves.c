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
static void remainingMoveTests(void){waterDischargeTests();}
