// Exercise actual native all-recipient Thunder Wave, not a replacement status model.
static void waitThunderWaveTurn(void){
 unsigned i,seenActive=0;
 for(i=0;i<2400;i++){
  unsigned active=core->busRead8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_TW_ACTIVE);
  seenActive|=active;
  if((seenActive&&!active)||controller(0)==SYM_HandleInputChooseAction)break;
  // Hold the real A button during long multi-recipient text, rather than
  // pulsing it for only 3 of every 12 frames. No controller/PP/script writes.
  frames(1,1);
 }
 CHECK(i<2400);
 // Retain the original independent 2400-frame remainder watchdog and
 // the actual action-selection controller requirement.
 unsigned j;
 for(j=0;j<2400;j++){
  if(controller(0)==SYM_HandleInputChooseAction)break;
  frames(1,1);
 }
 if(j==2400)fprintf(stderr,"TW_REMAINDER_TIMEOUT ctrl0=%08x script=%08x exec=%08x twActive=%u\\n",
  controller(0),core->busRead32(core,SYM_gBattlescriptCurrInstr),
  core->busRead32(core,SYM_gBattleControllerExecFlags),
  core->busRead8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_TW_ACTIVE));
 CHECK(j<2400);
 frames(3,0);
}
static void thunderWaveTests(void){
 CHECK(ABI_BATTLESTRUCT_SIZE==0x200);CHECK(ABI_BATTLESTRUCT_TW_USER==0x1E4);CHECK(ABI_BATTLESTRUCT_TW_ACTIVE==0x1E7);
 effectBattleFixture(1,MOVE_THUNDER_WAVE);
 unsigned context=core->busRead32(core,SYM_gBattleStruct);
 for(unsigned user=0;user<4;user++){
  core->busWrite8(core,context+ABI_BATTLESTRUCT_TW_TARGETS,15U&~(1U<<user));unsigned seen=0;
  for(unsigned i=0;i<3;i++){unsigned b=CALL(NextThunderWaveTarget);CHECK(b<4&&b!=user&&!(seen&(1U<<b)));seen|=1U<<b;}
  CHECK(seen==(15U&~(1U<<user)));CHECK(CALL(NextThunderWaveTarget)==4);
 }
 core->busWrite16(core,bmon(3)+ABI_BMON_HP,0);core->busWrite8(core,SYM_gAbsentBattlerFlags,4);core->busWrite8(core,context+ABI_BATTLESTRUCT_TW_TARGETS,14);
 CHECK(CALL(NextThunderWaveTarget)==1);CHECK(CALL(NextThunderWaveTarget)==4);
 printf("PASS: target-ABI BattleStruct remains512 bytes, private context only in old padding; native iterator all4 actor masks/no self/duplicates and absent/fainted skips\n");
 for(unsigned scenario=0;scenario<12;scenario++){
  unsigned two=scenario!=0,user=scenario>=10?(scenario==10?1:3):scenario>=8?2:0;
  effectBattleFixture(two,MOVE_THUNDER_WAVE);
  if(user==2){CALL(SetMonMoveSlot,player(0),scenario==9?MOVE_MAGIC_COAT:MOVE_SPLASH,0);CALL(SetMonMoveSlot,player(1),MOVE_THUNDER_WAVE,0);core->busWrite16(core,bmon(0)+ABI_BMON_MOVES,scenario==9?MOVE_MAGIC_COAT:MOVE_SPLASH);core->busWrite8(core,bmon(0)+ABI_BMON_PP,scenario==9?15:40);core->busWrite16(core,bmon(2)+ABI_BMON_MOVES,MOVE_THUNDER_WAVE);core->busWrite8(core,bmon(2)+ABI_BMON_PP,20);}
  if(user&1){
   CALL(SetMonMoveSlot,player(0),MOVE_SPLASH,0);core->busWrite16(core,bmon(0)+ABI_BMON_MOVES,MOVE_SPLASH);core->busWrite8(core,bmon(0)+ABI_BMON_PP,40);
   for(unsigned m=0;m<4;m++){CALL(SetMonMoveSlot,enemy(user/2),MOVE_THUNDER_WAVE,m);core->busWrite16(core,bmon(user)+ABI_BMON_MOVES+2*m,MOVE_THUNDER_WAVE);core->busWrite8(core,bmon(user)+ABI_BMON_PP+m,20);}
  }
  core->busWrite16(core,bmon(user)+ABI_BMON_SPEED,300);
  if(scenario==2)core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_LIMBER);
  if(scenario==3)core->busWrite8(core,bmon(2)+ABI_BMON_TYPE2,TYPE_ELECTRIC);
  if(scenario==4)core->busWrite8(core,bmon(3)+ABI_BMON_TYPE1,TYPE_GROUND);
  if(scenario==5||scenario==6||scenario==7){unsigned move=scenario==5?MOVE_PROTECT:scenario==6?MOVE_MAGIC_COAT:MOVE_FOLLOW_ME;for(unsigned m=0;m<4;m++){CALL(SetMonMoveSlot,enemy(0),move,m);core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*m,move);core->busWrite8(core,bmon(1)+ABI_BMON_PP+m,core->busRead8(core,SYM_gBattleMoves+ABI_BMOVE_SIZE*move+ABI_BMOVE_PP));}}
  if(scenario==1)core->busWrite8(core,bmon(3)+ABI_BMON_ABILITY,ABILITY_LIGHTNING_ROD);
  chooseMove(0,0,1);if(two)chooseMove(2,0,1);
  // Read-only diagnostic for full-suite Thunder Wave Doubles scenario stalls.
  // Preserve the original native controller inputs and finite watchdogs.
  fprintf(stderr,"TW_SCENARIO_BEGIN scenario=%u two=%u user=%u ctrl0=%08x ctrl2=%08x script=%08x exec=%08x\\n",
   scenario,two,user,controller(0),controller(2),
   core->busRead32(core,SYM_gBattlescriptCurrInstr),core->busRead32(core,SYM_gBattleControllerExecFlags));
  waitThunderWaveTurn();
  fprintf(stderr,"TW_SCENARIO_RETURN scenario=%u ctrl0=%08x ctrl2=%08x script=%08x exec=%08x\\n",
   scenario,controller(0),controller(2),
   core->busRead32(core,SYM_gBattlescriptCurrInstr),core->busRead32(core,SYM_gBattleControllerExecFlags));
  for(unsigned b=0;b<(two?4:2);b++){
   unsigned expected=b!=user;
   if((scenario==2||scenario==5||scenario==6)&&b==1)expected=0;
   if(scenario==3&&b==2)expected=0;
   if(scenario==4&&b==3)expected=0;
   if(scenario==6&&b==user)expected=1;
   if(scenario==9){if(b==0)expected=0;if(b==user)expected=1;}
   unsigned status=core->busRead32(core,bmon(b)+ABI_BMON_STATUS1);
   if(status!=(expected?STATUS1_PARALYSIS:0))fprintf(stderr,"Thunder Wave scenario%u user%u recipient%u status%u expected%u\n",scenario,user,b,status,expected);
   CHECK(status==(expected?STATUS1_PARALYSIS:0));CHECK(core->busRead16(core,bmon(b)+ABI_BMON_HP)==1000);
  }
  if(user&1){unsigned pp=0;for(unsigned m=0;m<4;m++)pp+=core->busRead8(core,bmon(user)+ABI_BMON_PP+m);CHECK(pp==79);}else CHECK(monget(player(user/2),ABI_DATA_PP1)==19);
  CHECK(core->busRead8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_TW_ACTIVE)==0);
  printf("PASS: actual Thunder Wave scenario%u user%u independent recipients/guard/reflection, onePP19 and no direct damage\n",scenario,user);
 }
 // Recipient evasion can miss independently without carrying its flags to allies/other foes.
 effectBattleFixture(1,MOVE_THUNDER_WAVE);core->busWrite8(core,bmon(1)+ABI_BMON_STAGES+STAT_EVASION,12);
 void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned hit=0,miss=0;
 for(unsigned trial=0;trial<30&&(!hit||!miss);trial++){
  CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0x7A110000+trial*7919);chooseMove(0,0,1);chooseMove(2,0,1);waitThunderWaveTurn();
  unsigned status=core->busRead32(core,bmon(1)+ABI_BMON_STATUS1);CHECK(status==0||status==STATUS1_PARALYSIS);hit+=!!status;miss+=!status;
  CHECK(core->busRead32(core,bmon(2)+ABI_BMON_STATUS1)==STATUS1_PARALYSIS);CHECK(core->busRead32(core,bmon(3)+ABI_BMON_STATUS1)==STATUS1_PARALYSIS);CHECK(monget(player(0),ABI_DATA_PP1)==19);
 }
 CHECK(hit&&miss);free(state);
 effectBattleFixture(1,MOVE_THUNDER_WAVE);core->busWrite32(core,bmon(0)+ABI_BMON_STATUS1,STATUS1_PARALYSIS);monput(player(0),ABI_DATA_STATUS,STATUS1_PARALYSIS);
 state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned able=0,unable=0;
 for(unsigned trial=0;trial<30&&(!able||!unable);trial++){
  CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0xA8E10000+trial*7919);chooseMove(0,0,1);chooseMove(2,0,1);waitThunderWaveTurn();
  unsigned pp=monget(player(0),ABI_DATA_PP1);CHECK(pp==19||pp==20);able+=pp==19;unable+=pp==20;
  for(unsigned b=1;b<4;b++)CHECK(core->busRead32(core,bmon(b)+ABI_BMON_STATUS1)==(pp==19?STATUS1_PARALYSIS:0));
 }
 CHECK(able&&unable);free(state);
 printf("PASS: actual independent evasion hit/miss plus once-per-move attacker paralysis cancellation; all recipients or none and one/noPP respectively\n");
 // Spending the last PP still reaches everyone; native Pressure is charged once.
 for(unsigned pressure=0;pressure<2;pressure++){
  effectBattleFixture(1,MOVE_THUNDER_WAVE);
  unsigned initial=pressure?20:1;core->busWrite8(core,bmon(0)+ABI_BMON_PP,initial);monput(player(0),ABI_DATA_PP1,initial);
  if(pressure)for(unsigned b=1;b<4;b++)core->busWrite8(core,bmon(b)+ABI_BMON_ABILITY,ABILITY_PRESSURE);
  chooseMove(0,0,1);chooseMove(2,0,1);waitThunderWaveTurn();
  for(unsigned b=1;b<4;b++)CHECK(core->busRead32(core,bmon(b)+ABI_BMON_STATUS1)==STATUS1_PARALYSIS);
  CHECK(monget(player(0),ABI_DATA_PP1)==(pressure?16:0));
 }
 // Other Electric moves keep original GenIII Lightning Rod redirection.
 effectBattleFixture(1,MOVE_THUNDERBOLT);core->busWrite8(core,bmon(3)+ABI_BMON_ABILITY,ABILITY_LIGHTNING_ROD);
 core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,100);core->busWrite16(core,bmon(3)+ABI_BMON_SPDEFENSE,200);
 chooseMove(0,0,1);chooseMove(2,0,1);waitAction();
 CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)<1000);CHECK(monget(player(0),ABI_DATA_PP1)==14);
 printf("PASS: actual lastPP Thunder Wave reaches all3; native three-recipient Pressure costs4PP once; ordinary Thunderbolt still redirects to Lightning Rod\n");
 // A called move lazily starts the same iterator without spending its own PP.
 effectBattleFixture(1,MOVE_SLEEP_TALK);
 for(unsigned m=1;m<4;m++){unsigned move=m==1?MOVE_THUNDER_WAVE:MOVE_NONE;CALL(SetMonMoveSlot,player(0),move,m);core->busWrite16(core,bmon(0)+ABI_BMON_MOVES+2*m,move);core->busWrite8(core,bmon(0)+ABI_BMON_PP+m,m==1?20:0);}
 core->busWrite32(core,bmon(0)+ABI_BMON_STATUS1,3);monput(player(0),ABI_DATA_STATUS,3);
 chooseMove(0,0,1);chooseMove(2,0,1);waitThunderWaveTurn();
 for(unsigned b=1;b<4;b++)CHECK(core->busRead32(core,bmon(b)+ABI_BMON_STATUS1)==STATUS1_PARALYSIS);
 CHECK(core->busRead32(core,bmon(0)+ABI_BMON_STATUS1)&STATUS1_SLEEP);CHECK(monget(player(0),ABI_DATA_PP1)==9);CHECK(monget(player(0),ABI_DATA_PP2)==20);
 printf("PASS: actual sleeping user calls only eligible Thunder Wave through Sleep Talk, all3 recipients, Sleep Talk9PP/TW20PP and sleep retained\n");
 // A faster real Spite can exhaust chosen Thunder Wave PP before execution.
 effectBattleFixture(1,MOVE_THUNDER_WAVE);
 core->busWrite8(core,bmon(0)+ABI_BMON_PP,2);monput(player(0),ABI_DATA_PP1,2);
 core->busWrite16(core,SYM_gLastMoves,MOVE_THUNDER_WAVE);
 core->busWrite8(core,bmon(0)+ABI_BMON_TYPE1,TYPE_WATER);core->busWrite8(core,bmon(0)+ABI_BMON_TYPE2,TYPE_WATER);
 core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,500);core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,300);
 for(unsigned m=0;m<4;m++){CALL(SetMonMoveSlot,enemy(0),MOVE_SPITE,m);core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*m,MOVE_SPITE);core->busWrite8(core,bmon(1)+ABI_BMON_PP+m,10);}
 state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned exhausted=0;
 for(unsigned trial=0;trial<20&&!exhausted;trial++){
  CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0xE0A50000+trial*7919);chooseMove(0,0,1);chooseMove(2,0,1);waitThunderWaveTurn();
  unsigned pp=monget(player(0),ABI_DATA_PP1);CHECK(pp==0||pp==1);
  for(unsigned b=1;b<4;b++)CHECK(core->busRead32(core,bmon(b)+ABI_BMON_STATUS1)==(pp?STATUS1_PARALYSIS:0));
  exhausted=pp==0;
 }
 CHECK(exhausted);free(state);
 printf("PASS: actual faster Spite exhausts chosen Thunder Wave PP; no recipient is hit for free after native noPP cancellation\n");
 CHECK(core->loadState(core,bootState));
}
