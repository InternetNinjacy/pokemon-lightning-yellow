// Foul Play borrows target Attack/stages only. Native reference substitutes those
// two inputs into an ordinary Physical attack; the actual damage formula is shared.
static unsigned foulPlayReference(void){
 unsigned raw=core->busRead16(core,bmon(0)+ABI_BMON_ATTACK),stage=core->busRead8(core,bmon(0)+ABI_BMON_STAGES+STAT_ATK);
 core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,core->busRead16(core,bmon(1)+ABI_BMON_ATTACK));core->busWrite8(core,bmon(0)+ABI_BMON_STAGES+STAT_ATK,core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_ATK));
 unsigned expected=baseDamage(MOVE_ICE_PUNCH,0,95,TYPE_DARK);
 core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,raw);core->busWrite8(core,bmon(0)+ABI_BMON_STAGES+STAT_ATK,stage);return expected;
}
static void moves382Tests(void){
 effectBattleFixture(0,MOVE_FOUL_PLAY);damageStats();core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,1);core->busWrite16(core,bmon(1)+ABI_BMON_ATTACK,200);
 for(unsigned crit=1;crit<=2;crit++)for(unsigned stage=0;stage<=12;stage++)for(unsigned own=0;own<3;own++){
  core->busWrite8(core,SYM_gCritMultiplier,crit);core->busWrite8(core,bmon(1)+ABI_BMON_STAGES+STAT_ATK,stage);core->busWrite8(core,bmon(0)+ABI_BMON_STAGES+STAT_ATK,own*6);
  unsigned expected=foulPlayReference();CHECK(baseDamage(MOVE_FOUL_PLAY,0,0,0)==expected);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);core->busWrite16(core,SYM_gCurrentMove,MOVE_FOUL_PLAY);CALL(AI_CalcDmg,0,1);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected*crit);
 }
 // User-owned level, item, ability and burn modifiers are retained. Target-owned
 // Attack item/ability/burn modifiers are not transplanted with its raw stat.
 const unsigned abilities[]={ABILITY_NONE,ABILITY_HUGE_POWER,ABILITY_PURE_POWER,ABILITY_HUSTLE,ABILITY_GUTS};
 const unsigned items[]={ITEM_NONE,ITEM_CHOICE_BAND,ITEM_BLACK_GLASSES,ITEM_THICK_CLUB};
 core->busWrite8(core,SYM_gCritMultiplier,1);core->busWrite8(core,bmon(1)+ABI_BMON_STAGES+STAT_ATK,8);core->busWrite16(core,bmon(0)+ABI_BMON_SPECIES,SPECIES_CUBONE);
 for(unsigned a=0;a<5;a++)for(unsigned item=0;item<4;item++)for(unsigned burn=0;burn<2;burn++){
  core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,abilities[a]);core->busWrite16(core,bmon(0)+ABI_BMON_ITEM,items[item]);core->busWrite32(core,bmon(0)+ABI_BMON_STATUS1,burn?STATUS1_BURN:0);
  unsigned expected=foulPlayReference();CHECK(baseDamage(MOVE_FOUL_PLAY,0,0,0)==expected);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_HUGE_POWER);core->busWrite16(core,bmon(1)+ABI_BMON_ITEM,ITEM_CHOICE_BAND);core->busWrite32(core,bmon(1)+ABI_BMON_STATUS1,STATUS1_BURN);
  CHECK(baseDamage(MOVE_FOUL_PLAY,0,0,0)==expected);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_NONE);core->busWrite16(core,bmon(1)+ABI_BMON_ITEM,ITEM_NONE);core->busWrite32(core,bmon(1)+ABI_BMON_STATUS1,0);
 }
 // Defender stat source is pointer-specific in Doubles, not hardcoded battler1.
 effectBattleFixture(1,MOVE_FOUL_PLAY);unsigned flags=core->busRead32(core,SYM_gBattleTypeFlags);damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
 core->busWrite16(core,bmon(1)+ABI_BMON_ATTACK,60);core->busWrite16(core,bmon(3)+ABI_BMON_ATTACK,240);core->busWrite16(core,bmon(3)+ABI_BMON_DEFENSE,100);
 unsigned d1=CALL(CalculateBaseDamage,bmon(0),bmon(1),MOVE_FOUL_PLAY,0,0,0,0,1),d3=CALL(CalculateBaseDamage,bmon(0),bmon(3),MOVE_FOUL_PLAY,0,0,0,0,3);CHECK(d3>d1*3);
 core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);core->busWrite16(core,SYM_gCurrentMove,MOVE_FOUL_PLAY);CALL(AI_CalcDmg,0,3);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==d3);
 printf("PASS: native Foul Play target raw Attack/all13 stages, critical negatives, user stage independence, user ability/item/burn ownership and selected Doubles defender with shared AI\n");
 const unsigned moves[]={MOVE_FOUL_PLAY,MOVE_AQUA_JET,MOVE_LIQUIDATION},pp[]={15,20,10};
 for(unsigned m=0;m<3;m++)for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,moves[m]);flags=core->busRead32(core,SYM_gBattleTypeFlags);damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
  CHECK(!!(flags&BATTLE_TYPE_DOUBLE)==two);core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,m?200:1);core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,1);core->busWrite16(core,bmon(1)+ABI_BMON_ATTACK,200);core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,m==1?1:600);core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,m==1?600:1);
  if(!m)core->busWrite16(core,bmon(1)+ABI_BMON_ITEM,ITEM_NUGGET);
  if(m==1)for(unsigned k=0;k<4;k++){CALL(SetMonMoveSlot,enemy(0),MOVE_SWIFT,k);core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*k,MOVE_SWIFT);core->busWrite8(core,bmon(1)+ABI_BMON_PP+k,20);}
  unsigned raw=baseDamage(moves[m],0,0,0),seen=0,drop=0,f;
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
  for(unsigned trial=0;trial<(m==2?16:1)&&(!seen||(m==2&&!drop));trial++){
   CHECK(core->loadState(core,state));core->busWrite32(core,SYM_gRngValue,0x38200000+m*1237+two*17+trial*7919);seen=0;
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   for(f=0;f<4800;f++){
    if(controller(0)==SYM_HandleInputChooseAction)break;
    frames(1,(f%12)<3?1:0);
    if(!seen&&core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000){if(m==1)CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);seen=1;}
   }
   CHECK(f<4800&&seen);frames(3,0);unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);CHECK(dealt>=raw*85/100&&dealt<=raw);CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
   if(m==1)CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)<1000);else CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
   if(!m){CHECK(core->busRead16(core,bmon(1)+ABI_BMON_ITEM)==ITEM_NUGGET);CHECK(core->busRead16(core,bmon(0)+ABI_BMON_ITEM)==ITEM_NONE);}
   if(m==2){unsigned stage=core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_DEF);CHECK(stage==5||stage==6);drop|=stage==5;}
   if(two)CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);
  }
  CHECK(seen);if(m==2)CHECK(drop);free(state);
  printf("PASS: actual %s move%u physical damage, selected-target scope, onePP and approved existing-art playback%s\n",two?"Doubles":"Singles",moves[m],m==0?" without item theft":m==1?" with actual +1 priority before fast Swift":" with Defense-drop positive control");
 }
 CHECK(core->loadState(core,bootState));
}
