// Tier3: real ARM damage recording, shared live/AI calculation and native turns.
static unsigned avalancheLedger(void){return core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DAMAGE_SOURCES;}
static void avalancheFixture(unsigned two){
 effectBattleFixture(two,MOVE_AVALANCHE);unsigned flags=core->busRead32(core,SYM_gBattleTypeFlags);damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
 CHECK(core->busRead8(core,SYM_gBattlersCount)==(two?4:2));CHECK(!!(flags&BATTLE_TYPE_DOUBLE)==two);
 core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
}
static void avalancheHpCommand(unsigned script,unsigned recipient,unsigned source,unsigned move,int amount,unsigned marker,unsigned operand,unsigned result){
 core->busWrite32(core,SYM_gBattleControllerExecFlags,0);core->busWrite8(core,SYM_gMoveResultFlags,result);core->busWrite32(core,SYM_gHitMarker,marker);
 core->busWrite8(core,SYM_gBattlerAttacker,source);core->busWrite8(core,SYM_gBattlerTarget,recipient);core->busWrite16(core,SYM_gCurrentMove,move);
 core->busWrite32(core,SYM_gBattleMoveDamage,amount);core->busWrite8(core,script+1,operand);core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_datahpupdate);
}
static void avalancheTests(void){
 avalancheFixture(1);CHECK(ABI_BATTLESTRUCT_SIZE==512);CHECK(ABI_BATTLESTRUCT_DAMAGE_SOURCES==0x1E8);
 unsigned script=CALL(AllocZeroed,16);CHECK(script);void *snap=malloc(core->stateSize(core));CHECK(snap&&core->saveState(core,snap));
 for(unsigned recipient=0;recipient<4;recipient++)for(unsigned source=0;source<4;source++){
  CHECK(core->loadState(core,snap));avalancheHpCommand(script,recipient,source,MOVE_ICE_PUNCH,10,0,BS_TARGET,0);
  CHECK(core->busRead8(core,avalancheLedger()+recipient)==(recipient==source?0:1U<<source));
  CHECK(CALL(BattlerWasDamagedBy,recipient,source)==(recipient!=source));
 }
 for(unsigned variant=0;variant<7;variant++){
  CHECK(core->loadState(core,snap));
  if(variant==6){core->busWrite32(core,bmon(0)+ABI_BMON_STATUS2,STATUS2_SUBSTITUTE);core->busWrite8(core,SYM_gDisableStructs+ABI_DISABLE_SUB_HP,25);}
  avalancheHpCommand(script,0,1,variant==3?MOVE_LEECH_SEED:MOVE_ICE_PUNCH,variant==0?0:variant==1?-10:10,variant==2?HITMARKER_PASSIVE_DAMAGE:0,variant==4?BS_ATTACKER:BS_TARGET,variant==5?MOVE_RESULT_DOESNT_AFFECT_FOE:0);
  for(unsigned b=0;b<4;b++)CHECK(core->busRead8(core,avalancheLedger()+b)==0);
 }
 CHECK(core->loadState(core,snap));avalancheHpCommand(script,0,1,MOVE_ICE_PUNCH,10,0,BS_TARGET,0);avalancheHpCommand(script,0,3,MOVE_FLAMETHROWER,10,0,BS_TARGET,0);
 CHECK(core->busRead8(core,avalancheLedger())==10);CHECK(CALL(BattlerWasDamagedBy,0,1));CHECK(CALL(BattlerWasDamagedBy,0,3));
 unsigned normal=baseDamage(MOVE_ICE_PUNCH,0,60,0),boosted=baseDamage(MOVE_ICE_PUNCH,0,120,0);CHECK(boosted>normal);
 for(unsigned mask=0;mask<16;mask++){
  core->busWrite8(core,avalancheLedger(),mask);CHECK(baseDamage(MOVE_AVALANCHE,0,0,0)==((mask&2)?boosted:normal));
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);core->busWrite16(core,SYM_gCurrentMove,MOVE_AVALANCHE);CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==((mask&2)?boosted:normal));
 }
 CHECK(!CALL(BattlerWasDamagedBy,4,0));CHECK(!CALL(BattlerWasDamagedBy,0,4));
 core->busWrite8(core,avalancheLedger(),10);core->busWrite8(core,avalancheLedger()+1,15);core->busWrite8(core,avalancheLedger()+2,2);
 core->busWrite8(core,SYM_gActiveBattler,1);CALL(SwitchInClearSetData);
 CHECK(core->busRead8(core,avalancheLedger())==8);CHECK(core->busRead8(core,avalancheLedger()+1)==0);CHECK(core->busRead8(core,avalancheLedger()+2)==0);
 for(unsigned b=0;b<4;b++)core->busWrite8(core,avalancheLedger()+b,15);
 CALL(TurnValuesCleanUp,1);for(unsigned b=0;b<4;b++)CHECK(core->busRead8(core,avalancheLedger()+b)==15);
 CALL(TurnValuesCleanUp,0);for(unsigned b=0;b<4;b++)CHECK(core->busRead8(core,avalancheLedger()+b)==0);
 CHECK(core->loadState(core,snap));CALL(Free,script);free(snap);
 printf("PASS: native Avalanche all16 damage source pairs, seven indirect/non-hit exclusions, shared power/AI across16 masks, replacement identity and true turn reset; unchanged512-byte ABI\n");
 for(unsigned scenario=0;scenario<7;scenario++){
  unsigned two=scenario>=2&&scenario<=5,boost=scenario==1||scenario==3;
  avalancheFixture(two);core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,600);
  for(unsigned b=1;b<(two?4:2);b+=2){
   unsigned move=(scenario==1||scenario==3||(scenario==2&&b==3)||((scenario==4||scenario==5)&&b==1))?MOVE_SWIFT:scenario==6?MOVE_COUNTER:MOVE_SPLASH;
   core->busWrite16(core,bmon(b)+ABI_BMON_SPEED,b==1?300:200);
   for(unsigned k=0;k<4;k++){CALL(SetMonMoveSlot,enemy(b>>1),move,k);core->busWrite16(core,bmon(b)+ABI_BMON_MOVES+2*k,move);core->busWrite8(core,bmon(b)+ABI_BMON_PP+k,20);}
  }
  if(scenario==1)core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,1); // -4 still acts last
  if(scenario==4||scenario==5){core->busWrite32(core,bmon(0)+ABI_BMON_STATUS2,STATUS2_SUBSTITUTE);core->busWrite8(core,SYM_gDisableStructs+ABI_DISABLE_SUB_HP,scenario==4?200:1);}
  unsigned raw=baseDamage(MOVE_ICE_PUNCH,0,boost?120:60,0),seen=0,f;
  chooseMove(0,0,1);if(two)chooseMove(2,0,1);
  for(f=0;f<4800;f++){
   if(controller(0)==SYM_HandleInputChooseAction)break;frames(1,(f%12)<3?1:0);
   if(!seen&&core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000){
    CHECK(core->busRead16(core,SYM_gBattleMovePower)==(boost?120:60));
    if(boost)CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)<1000);
    if(scenario==3)CHECK((core->busRead8(core,avalancheLedger())&10)==10);
    if(scenario==2)CHECK(core->busRead8(core,avalancheLedger())==8);
    seen=1;
   }
  }
  CHECK(f<4800&&seen);frames(3,0);
  unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);CHECK(dealt>=(raw*85/100)&&dealt<=raw);CHECK(monget(player(0),ABI_DATA_PP1)==9);
  if(two)CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);
  if(scenario==4||scenario==5)CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
  for(unsigned b=0;b<4;b++)CHECK(core->busRead8(core,avalancheLedger()+b)==0);
  if(scenario==3){
   for(unsigned b=1;b<4;b+=2)for(unsigned k=0;k<4;k++){CALL(SetMonMoveSlot,enemy(b>>1),MOVE_SPLASH,k);core->busWrite16(core,bmon(b)+ABI_BMON_MOVES+2*k,MOVE_SPLASH);core->busWrite8(core,bmon(b)+ABI_BMON_PP+k,40);}
   unsigned hp=core->busRead16(core,bmon(1)+ABI_BMON_HP);chooseMove(0,0,1);chooseMove(2,0,1);waitAction();unsigned diff=hp-core->busRead16(core,bmon(1)+ABI_BMON_HP),base=baseDamage(MOVE_ICE_PUNCH,0,60,0);
   CHECK(diff>=base*85/100&&diff<=base);CHECK(monget(player(0),ABI_DATA_PP1)==8);
  }
  printf("PASS: actual Avalanche scenario%u %s power%u, selected target damage%u, onePP, approved animation and no stale carry; arena fixture, not move distribution\n",scenario,two?"Doubles":"Singles",boost?120:60,dealt);
 }
 CHECK(core->loadState(core,bootState));
}
