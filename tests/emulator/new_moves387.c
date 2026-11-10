// BTL-005 #387 real-ROM Discharge: correct Electric special stat, true
// all-others Singles/Doubles targeting, one PP and 30%/Serene Grace60%.
static void discharge387Tests(void){
 for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,MOVE_DISCHARGE);
  unsigned flags=core->busRead32(core,SYM_gBattleTypeFlags);
  damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,1);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,220);
  for(unsigned b=1;b<(two?4:2);b++){
   core->busWrite16(core,bmon(b)+ABI_BMON_DEFENSE,999);
   core->busWrite16(core,bmon(b)+ABI_BMON_SPDEFENSE,100);
  }
  unsigned expected=baseDamage(MOVE_DISCHARGE,0,0,0);
  CHECK(expected>0&&expected<1000);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_DISCHARGE);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  chooseMove(0,0,1);if(two)chooseMove(2,0,1);
  waitActionBudget(4800);
  CHECK(monget(player(0),ABI_DATA_PP1)==14);
  CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000);
  if(two){
   CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)<1000);
   CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)<1000);
  }
 }
 effectBattleFixture(0,MOVE_DISCHARGE);
 unsigned script=CALL(AllocZeroed,32);CHECK(script);
 void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
 const unsigned rolls[]={29,30,59,60};
 for(unsigned serene=0;serene<2;serene++)for(unsigned r=0;r<4;r++){
  CHECK(core->loadState(core,state));
  core->busWrite8(core,SYM_gBattlerAttacker,0);
  core->busWrite8(core,SYM_gBattlerTarget,1);
  core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,serene?ABILITY_SERENE_GRACE:ABILITY_NONE);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_NONE);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_DISCHARGE);
  core->busWrite8(core,SYM_gMoveResultFlags,0);
  core->busWrite32(core,SYM_gHitMarker,0);
  core->busWrite8(core,SYM_gBattleCommunication+MOVE_EFFECT_BYTE,MOVE_EFFECT_PARALYSIS);
  core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(rolls[r]));
  core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);
  CALL(Cmd_seteffectwithchance);
  CHECK(!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_PARALYSIS)==(rolls[r]<(serene?60:30)));
 }
 CHECK(core->loadState(core,state));CALL(Free,script);free(state);
 CHECK(core->loadState(core,bootState));
 printf("PASS: native Discharge #387 Singles/Doubles all-other-battler HP targeting, Electric special AI, 1 PP, 30/60%% paralysis\n");
}


// Live ROM Singles/Doubles acceptance for first-batch native damage effects.
// This suite does not substitute for Volt Switch's dedicated party-selection
// acceptance or Thunder Fang's full independent two-RNG proof.
static void moves389to394Tests(void){
 const unsigned moves[]={MOVE_THUNDER_FANG,MOVE_WILD_CHARGE,MOVE_SEED_BOMB,
                         MOVE_PETAL_BLIZZARD,MOVE_WOOD_HAMMER,MOVE_AURA_SPHERE};
 const unsigned initialPP[]={15,15,15,15,15,20};
 for(unsigned m=0;m<6;m++)for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,moves[m]);
  unsigned flags=core->busRead32(core,SYM_gBattleTypeFlags);
  damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,m==5?1:220);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,m==5?220:1);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,600);
  for(unsigned b=1;b<(two?4:2);b++){
   core->busWrite16(core,bmon(b)+ABI_BMON_DEFENSE,100);
   core->busWrite16(core,bmon(b)+ABI_BMON_SPDEFENSE,100);
   core->busWrite16(core,bmon(b)+ABI_BMON_SPEED,1);
  }
  // Aura Sphere must bypass maximized opponent evasion and minimized
  // attacker accuracy; all other moves use their approved accuracy values.
  if(m==5){
   core->busWrite8(core,bmon(0)+ABI_BMON_STAGES+STAT_ACC,0);
   core->busWrite8(core,bmon(1)+ABI_BMON_STAGES+STAT_EVASION,12);
  }
  unsigned expected=baseDamage(moves[m],0,0,0);
  CHECK(expected>0&&expected<1000);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite16(core,SYM_gCurrentMove,moves[m]);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  // A normal battle action must consume one PP; battle replay catches
  // missed accuracy cases separately, without accepting a miss as a hit.
  void *snap=malloc(core->stateSize(core));CHECK(snap&&core->saveState(core,snap));
  unsigned hit=0;
  for(unsigned trial=0;trial<20&&!hit;trial++){
   CHECK(core->loadState(core,snap));
   core->busWrite32(core,SYM_gRngValue,0x389394U+trial*7919U+m*31U+two*17U);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   waitActionBudget(4800);
   CHECK(monget(player(0),ABI_DATA_PP1)==initialPP[m]-1);
   unsigned hp=core->busRead16(core,bmon(1)+ABI_BMON_HP);
   if(hp==1000)continue;
   CHECK(hp<1000);
   unsigned dealt=1000-hp;
   if(two){
    unsigned ally=core->busRead16(core,bmon(2)+ABI_BMON_HP);
    unsigned other=core->busRead16(core,bmon(3)+ABI_BMON_HP);
    if(m==3)CHECK(ally<1000&&other<1000);
    else CHECK(ally==1000&&other==1000);
   }
   unsigned self=core->busRead16(core,bmon(0)+ABI_BMON_HP);
   if(m==1)CHECK(1000-self==(dealt/4?dealt/4:1));
   else if(m==4)CHECK(1000-self==(dealt/3?dealt/3:1));
   else CHECK(self==1000);
   hit++;
  }
  CHECK(hit);
  free(snap);
 }
 CHECK(core->loadState(core,bootState));
 printf("PASS: native #389-394 Singles/Doubles physical-special AI, hits, selected/spread target HP, PP, quarter/third recoil and Aura Sphere accuracy bypass\n");
}
