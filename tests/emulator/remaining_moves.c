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

static void reuse389Tests(void){
 const unsigned moves[]={MOVE_THUNDER_FANG,MOVE_WILD_CHARGE,MOVE_SEED_BOMB,MOVE_PETAL_BLIZZARD,MOVE_WOOD_HAMMER,MOVE_AURA_SPHERE,MOVE_FOCUS_BLAST,MOVE_DRAIN_PUNCH,MOVE_FLASH_CANNON,MOVE_MIRROR_SHOT,MOVE_IRON_HEAD,MOVE_BULLET_PUNCH,MOVE_AIR_SLASH};
 const unsigned powers[]={65,90,80,90,120,80,120,75,80,65,80,40,75},pp[]={15,15,15,15,15,20,5,10,10,10,15,30,15},types[]={TYPE_ELECTRIC,TYPE_ELECTRIC,TYPE_GRASS,TYPE_GRASS,TYPE_GRASS,TYPE_FIGHTING,TYPE_FIGHTING,TYPE_FIGHTING,TYPE_STEEL,TYPE_STEEL,TYPE_STEEL,TYPE_STEEL,TYPE_FLYING};
 for(unsigned m=0;m<13;m++){
  remainingFixture(0,moves[m]);unsigned special=m==5||m==6||m==8||m==9||m==12;
  unsigned reference=baseDamage(special?MOVE_FLAMETHROWER:MOVE_ICE_PUNCH,0,powers[m],types[m]);CHECK(baseDamage(moves[m],0,0,0)==reference);core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);core->busWrite16(core,SYM_gCurrentMove,moves[m]);CALL(AI_CalcDmg,0,1);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==reference);
  if(m==7){core->busWrite16(core,bmon(0)+ABI_BMON_HP,500);monput(player(0),ABI_DATA_HP,500);}
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned hit=0;
  for(unsigned trial=0;trial<8&&!hit;trial++){
   CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0x38900000+m*1237+trial*7919);chooseMove(0,0,1);
   unsigned previous=1000,changes=0,f;for(f=0;f<6000;f++){if(controller(0)==SYM_HandleInputChooseAction)break;frames(1,(f%12)<3?1:0);unsigned hp=core->busRead16(core,bmon(1)+ABI_BMON_HP);if(hp<previous){changes++;previous=hp;}}
   CHECK(f<6000);unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);hit=dealt>0;CHECK(changes==(hit?1:0));CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
   unsigned user=core->busRead16(core,bmon(0)+ABI_BMON_HP);
   if(hit&&m==1)CHECK(1000-user==(dealt/4?dealt/4:1));else if(hit&&m==4)CHECK(1000-user==(dealt/3?dealt/3:1));else if(hit&&m==7)CHECK(user-500==(dealt/2?dealt/2:1));else CHECK(user==(m==7?500:1000));
   CHECK(!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&(STATUS2_RECHARGE|STATUS2_MULTIPLETURNS|STATUS2_CONFUSION)));
  }
  CHECK(hit);free(state);printf("PASS: actual move%u category/AI/one-hit/onePP and approved animation; correct native recoil/drain, no borrowed charge/recharge/lock\n",moves[m]);
 }
 // Genuine all-other Petal Blizzard: two foes and ally, never user; no dance lock.
 remainingFixture(1,MOVE_PETAL_BLIZZARD);chooseMove(0,0,1);chooseMove(2,0,1);waitAction();for(unsigned b=1;b<4;b++)CHECK(core->busRead16(core,bmon(b)+ABI_BMON_HP)<1000);CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);CHECK(!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&STATUS2_MULTIPLETURNS));CHECK(monget(player(0),ABI_DATA_PP1)==14);
 // Native guaranteed-hit path still obeys Protect/Ghost/semi-invulnerability.
 for(unsigned guard=0;guard<4;guard++){
  remainingFixture(0,MOVE_AURA_SPHERE);core->busWrite8(core,bmon(0)+ABI_BMON_STAGES+STAT_ACC,0);core->busWrite8(core,bmon(1)+ABI_BMON_STAGES+STAT_EVASION,12);
  if(guard==1)core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_GHOST);
  if(guard==2){core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,650);for(unsigned k=0;k<4;k++){CALL(SetMonMoveSlot,enemy(0),MOVE_PROTECT,k);core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*k,MOVE_PROTECT);core->busWrite8(core,bmon(1)+ABI_BMON_PP+k,10);}}
  if(guard==3)core->busWrite32(core,SYM_gStatuses3+4,STATUS3_UNDERGROUND);
  chooseMove(0,0,1);waitAction();CHECK((core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000)==(guard==0));CHECK(monget(player(0),ABI_DATA_PP1)==19);
 }
 CHECK(core->loadState(core,bootState));printf("PASS: actual Petal Blizzard both foes+ally, Aura Sphere accuracy bypass/Protect/Ghost/underground negatives; no new distribution\n");
}

static void reuse389ExtraTests(void){
 remainingFixture(1,MOVE_THUNDER_FANG);unsigned script=CALL(AllocZeroed,16);CHECK(script);void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
 const unsigned rolls[]={9,10,19,20};
 for(unsigned serene=0;serene<2;serene++)for(unsigned electric=0;electric<2;electric++)for(unsigned r=0;r<4;r++){
  CHECK(core->loadState(core,state));core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,serene?ABILITY_SERENE_GRACE:ABILITY_NONE);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,electric?TYPE_ELECTRIC:TYPE_NORMAL);
  core->busWrite8(core,SYM_gBattlerAttacker,0);core->busWrite8(core,SYM_gBattlerTarget,1);core->busWrite16(core,SYM_gCurrentMove,MOVE_THUNDER_FANG);core->busWrite8(core,SYM_gMoveResultFlags,0);core->busWrite32(core,SYM_gHitMarker,0);core->busWrite8(core,SYM_gBattleCommunication+MOVE_EFFECT_BYTE,MOVE_EFFECT_PARALYSIS);core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(rolls[r]));core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);CALL(Cmd_seteffectwithchance);
  CHECK(!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_PARALYSIS)==(!electric&&rolls[r]<(serene?20:10)));
 }
 CHECK(core->loadState(core,state));CALL(Free,script);free(state);
 remainingFixture(1,MOVE_THUNDER_FANG);core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_ELECTRIC);chooseMove(0,0,1);chooseMove(2,0,1);unsigned paraMarker=0,f;
 for(f=0;f<6000;f++){if(controller(0)==SYM_HandleInputChooseAction)break;frames(1,(f%12)<3?1:0);if(core->busRead16(core,SYM_gCurrentMove)==MOVE_THUNDER_FANG&&core->busRead8(core,SYM_gBattleCommunication+MOVE_EFFECT_BYTE)==MOVE_EFFECT_PARALYSIS)paraMarker=1;}
 CHECK(f<6000&&paraMarker);CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000);CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)==0);CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);CHECK(monget(player(0),ABI_DATA_PP1)==14);
 // Opponent-side composite branch + actual Steel priority ahead of fast Swift.
 remainingFixture(0,MOVE_SWIFT);core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,650);core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,1);
 for(unsigned k=0;k<4;k++){CALL(SetMonMoveSlot,enemy(0),MOVE_BULLET_PUNCH,k);core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*k,MOVE_BULLET_PUNCH);core->busWrite8(core,bmon(1)+ABI_BMON_PP+k,30);}
 chooseMove(0,0,1);unsigned first=0;
 for(f=0;f<6000;f++){if(controller(0)==SYM_HandleInputChooseAction)break;frames(1,(f%12)<3?1:0);if(!first&&core->busRead16(core,bmon(0)+ABI_BMON_HP)<1000){CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==1000);first=1;}}
 CHECK(f<6000&&first);CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000);unsigned pp=0;for(unsigned k=0;k<4;k++)pp+=monget(enemy(0),ABI_DATA_PP1+k);CHECK(pp==119);
 CHECK(core->loadState(core,bootState));printf("PASS: native Thunder Fang10/20%% chance boundaries/Electric immunity, actual Doubles paralysis dispatch, and opponent Bullet Punch composite/priority before fast Swift\n");
}

static void final410Tests(void){
 if(!getenv("LIGHTNING_FINAL410_ITEM_ONLY")){
 const unsigned moves[]={MOVE_HURRICANE,MOVE_BRAVE_BIRD,MOVE_ACROBATICS,MOVE_DUAL_WINGBEAT,MOVE_HEART_STAMP,MOVE_ZEN_HEADBUTT,MOVE_CROSS_POISON,MOVE_POISON_JAB,MOVE_GUNK_SHOT};
 const unsigned pp[]={10,15,15,10,25,15,20,20,5};
 for(unsigned m=0;m<9;m++){
  remainingFixture(0,moves[m]);if(!m)core->busWrite16(core,SYM_gBattleWeather,B_WEATHER_RAIN_PERMANENT);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));unsigned hit=0;
  for(unsigned trial=0;trial<8&&!hit;trial++){
   CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0x40200000+m*1237+trial*7919);chooseMove(0,0,1);unsigned prev=1000,hits=0,f;
   for(f=0;f<6000;f++){if(controller(0)==SYM_HandleInputChooseAction)break;frames(1,(f%12)<3?1:0);unsigned hp=core->busRead16(core,bmon(1)+ABI_BMON_HP);if(hp<prev){prev=hp;hits++;}}
   CHECK(f<6000);unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);hit=dealt>0;CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
   if(m==3&&hit)CHECK(hits==2);else if(m<6&&hit)CHECK(hits==1);
   if(m==1&&hit)CHECK(1000-core->busRead16(core,bmon(0)+ABI_BMON_HP)==(dealt/3?dealt/3:1));else CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
   CHECK(!(core->busRead32(core,bmon(0)+ABI_BMON_STATUS2)&(STATUS2_RECHARGE|STATUS2_MULTIPLETURNS)));
  }
  CHECK(hit);free(state);printf("PASS: actual move%u damage/onePP/approved art, no borrowed charging/recharge; native two-hit or recoil where specified\n",moves[m]);
 }
 // Exact native Hurricane accuracy across all100 RNG values and suppression.
 remainingFixture(1,MOVE_HURRICANE);unsigned script=CALL(AllocZeroed,16);CHECK(script);void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
 const unsigned expected[]={70,100,50,70,70,70,100};
 for(unsigned mode=0;mode<7;mode++){
  unsigned hits=0;
  for(unsigned roll=0;roll<100;roll++){
   CHECK(core->loadState(core,state));core->busWrite8(core,SYM_gBattlerAttacker,0);core->busWrite8(core,SYM_gBattlerTarget,1);core->busWrite16(core,SYM_gCurrentMove,MOVE_HURRICANE);core->busWrite8(core,SYM_gMoveResultFlags,0);core->busWrite32(core,SYM_gHitMarker,0);
   core->busWrite16(core,SYM_gBattleWeather,mode==0?0:(mode==2||mode==4)?B_WEATHER_SUN_PERMANENT:B_WEATHER_RAIN_PERMANENT);
   if(mode==3)core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_CLOUD_NINE);if(mode==4)core->busWrite8(core,bmon(2)+ABI_BMON_ABILITY,ABILITY_AIR_LOCK);if(mode==5)core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,ABILITY_AIR_LOCK);
   if(mode==6){core->busWrite8(core,bmon(0)+ABI_BMON_STAGES+STAT_ACC,0);core->busWrite8(core,bmon(1)+ABI_BMON_STAGES+STAT_EVASION,12);}
   for(unsigned b=0;b<4;b++){core->busWrite8(core,script+1+b,(script+12)>>(8*b));}core->busWrite8(core,script+5,MOVE_HURRICANE&255);core->busWrite8(core,script+6,MOVE_HURRICANE>>8);core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(roll));CALL(Cmd_accuracycheck);
   unsigned hit=core->busRead32(core,SYM_gBattlescriptCurrInstr)==script+7;CHECK(hit==(roll<expected[mode]));hits+=hit;
  }
  CHECK(hits==expected[mode]);
 }
 CHECK(core->loadState(core,state));CALL(Free,script);free(state);
 // Full move script: Flying target is reachable, underground target is not;
 // Protect wins even in Rain. Doubles still damages only the selected foe.
 for(unsigned guard=0;guard<4;guard++){
  remainingFixture(1,MOVE_HURRICANE);core->busWrite16(core,SYM_gBattleWeather,B_WEATHER_RAIN_PERMANENT);
  if(guard==1)core->busWrite32(core,SYM_gStatuses3+4,STATUS3_ON_AIR);if(guard==2)core->busWrite32(core,SYM_gStatuses3+4,STATUS3_UNDERGROUND);
  if(guard==3){core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,650);for(unsigned k=0;k<4;k++){CALL(SetMonMoveSlot,enemy(0),MOVE_PROTECT,k);core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*k,MOVE_PROTECT);core->busWrite8(core,bmon(1)+ABI_BMON_PP+k,10);}}
  chooseMove(0,0,1);chooseMove(2,0,1);waitAction();CHECK((core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000)==(guard<2));CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);CHECK(monget(player(0),ABI_DATA_PP1)==9);CHECK(core->busRead16(core,SYM_gBattleWeather)==B_WEATHER_RAIN_PERMANENT);
 }
 }
 // Acrobatics uses an actual held-item presence, shared by live and AI damage.
 remainingFixture(0,MOVE_ACROBATICS);const unsigned items[]={ITEM_NONE,ITEM_NUGGET,ITEM_ORAN_BERRY,ITEM_SHARP_BEAK,ITEM_CHOICE_BAND};
 for(unsigned i=0;i<5;i++){core->busWrite16(core,bmon(0)+ABI_BMON_ITEM,items[i]);unsigned reference=baseDamage(MOVE_ICE_PUNCH,0,i?55:110,TYPE_FLYING);CHECK(baseDamage(MOVE_ACROBATICS,0,0,0)==reference);core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);core->busWrite16(core,SYM_gCurrentMove,MOVE_ACROBATICS);CALL(AI_CalcDmg,0,1);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==reference);}
 for(unsigned mode=0;mode<3;mode++){
  remainingFixture(1,MOVE_ACROBATICS);core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,1);core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,650);core->busWrite16(core,bmon(0)+ABI_BMON_ITEM,mode==2?ITEM_SITRUS_BERRY:ITEM_NUGGET);
  monput(player(0),ABI_DATA_HELD_ITEM,mode==2?ITEM_SITRUS_BERRY:ITEM_NUGGET);
  if(mode==2){core->busWrite16(core,bmon(0)+ABI_BMON_HP,500);monput(player(0),ABI_DATA_HP,500);}
  unsigned foe=mode==1?MOVE_KNOCK_OFF:mode==2?MOVE_SWIFT:MOVE_SPLASH;
  for(unsigned k=0;k<4;k++){CALL(SetMonMoveSlot,enemy(0),foe,k);core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*k,foe);core->busWrite8(core,bmon(1)+ABI_BMON_PP+k,20);}
  chooseMove(0,0,1);chooseMove(2,0,1);unsigned seen=0,f;
  for(f=0;f<6000;f++){if(controller(0)==SYM_HandleInputChooseAction)break;frames(1,(f%12)<3?1:0);if(!seen&&core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000){CHECK(core->busRead16(core,SYM_gBattleMovePower)==(mode==1?110:55));CHECK(core->busRead16(core,bmon(0)+ABI_BMON_ITEM)==(mode==1?ITEM_NONE:mode==2?ITEM_SITRUS_BERRY:ITEM_NUGGET));seen=1;}}
  CHECK(f<6000&&seen);CHECK(monget(player(0),ABI_DATA_PP1)==14);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);
  if(mode==2){
   // Preserve native GenIII end-of-turn Berry timing. Boost the NEXT attack.
   CHECK(core->busRead16(core,bmon(0)+ABI_BMON_ITEM)==ITEM_NONE);unsigned previous=core->busRead16(core,bmon(1)+ABI_BMON_HP);seen=0;
   frames(3,0);chooseMove(0,0,1);chooseMove(2,0,1);
   for(f=0;f<6000;f++){if(controller(0)==SYM_HandleInputChooseAction)break;frames(1,(f%12)<3?1:0);if(!seen&&core->busRead16(core,bmon(1)+ABI_BMON_HP)<previous){CHECK(core->busRead16(core,SYM_gBattleMovePower)==110);seen=1;}}
   CHECK(f<6000&&seen);CHECK(monget(player(0),ABI_DATA_PP1)==13);
  }
 }
 // Native two-hit loop continues after breaking Substitute, with only one PP.
 remainingFixture(1,MOVE_DUAL_WINGBEAT);core->busWrite32(core,bmon(1)+ABI_BMON_STATUS2,STATUS2_SUBSTITUTE);core->busWrite8(core,SYM_gDisableStructs+ABI_DISABLE_SIZE+ABI_DISABLE_SUB_HP,1);
 void *snap=malloc(core->stateSize(core));CHECK(snap&&core->saveState(core,snap));unsigned hit=0;
 for(unsigned trial=0;trial<8&&!hit;trial++){CHECK(core->loadState(core,snap));core->busWrite32(core,SYM_gRngValue,0x40500000+trial*7919);chooseMove(0,0,1);chooseMove(2,0,1);waitAction();hit=core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000;CHECK(monget(player(0),ABI_DATA_PP1)==9);CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
 CHECK(hit);free(snap);CHECK(core->loadState(core,bootState));printf("PASS: native Hurricane exact70/100/50/suppressed accuracy and actual weather/Protect/air/underground; Acrobatics held/KnockOff/consumed-berry state; Dual Wingbeat Sub break in Doubles\n");
}

static void remainingMoveTests(void){if(getenv("LIGHTNING_FINAL410_ONLY")){final410Tests();return;}if(getenv("LIGHTNING_REUSE389_EXTRA_ONLY")){reuse389ExtraTests();return;}if(getenv("LIGHTNING_REUSE389_ONLY")){reuse389Tests();return;}if(getenv("LIGHTNING_VOLT_SWITCH_AI_ONLY")){voltSwitchAdditionalTests();return;}if(!getenv("LIGHTNING_VOLT_SWITCH_ONLY"))waterDischargeTests();voltSwitchTests();voltSwitchAdditionalTests();reuse389Tests();reuse389ExtraTests();final410Tests();}
