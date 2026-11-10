// BTL-005 Batch B real-ROM Singles/Doubles acceptance (staging, not passed).
static void batchBNativeTests(void){
 const unsigned moves[]={MOVE_DISCHARGE,MOVE_THUNDER_FANG,MOVE_WILD_CHARGE,
  MOVE_WOOD_HAMMER,MOVE_DRAIN_PUNCH,MOVE_BRAVE_BIRD,MOVE_ZEN_HEADBUTT,
  MOVE_CROSS_POISON,MOVE_POISON_JAB,MOVE_GUNK_SHOT};
 const unsigned pp[]={15,15,15,15,10,15,15,20,20,5};
 for(unsigned m=0;m<10;m++)for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,moves[m]);
  unsigned flags=core->busRead32(core,SYM_gBattleTypeFlags);
  damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
  CHECK(!!(flags&BATTLE_TYPE_DOUBLE)==two);
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,175);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,175);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,100);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
  if(m==4){
   // A damaged attacker must receive healing from real Drain Punch damage.
   core->busWrite16(core,bmon(0)+ABI_BMON_HP,500);
   monput(player(0),ABI_DATA_HP,500);
  }
  unsigned raw=baseDamage(moves[m],0,0,0);
  CHECK(raw>0&&raw<900);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite16(core,SYM_gCurrentMove,moves[m]);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==raw);
  void *snap=malloc(core->stateSize(core));CHECK(snap&&core->saveState(core,snap));
  unsigned hit=0;
  for(unsigned trial=0;trial<16&&!hit;trial++){
   CHECK(core->loadState(core,snap));
   core->busWrite32(core,SYM_gRngValue,0xB7100000U+m*17001U+two*107U+trial*7919U);
   chooseMove(0,0,1);if(two)chooseMove(2,0,1);
   waitActionBudget(4800);
   unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
   CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
   if(dealt){
    hit++;
    unsigned estimated=m==0&&two?raw*3/4:raw;
    CHECK(dealt+2>=estimated*85/100&&dealt<=estimated+2);
    unsigned ours=core->busRead16(core,bmon(0)+ABI_BMON_HP);
    if(m==2)CHECK(ours<1000&&1000-ours>=dealt/4&&1000-ours<=dealt/4+2);
    else if(m==3||m==5)CHECK(ours<1000&&1000-ours>=dealt/3&&1000-ours<=dealt/3+2);
    else if(m==4)CHECK(ours>500&&ours<=500+dealt/2+2);
    else CHECK(ours==1000);
   }
   if(two){
    if(m==0){
     CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)<1000);
     CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)<1000);
    }else{
     CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);
     CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);
    }
   }
  }
  CHECK(hit);
  free(snap);
  printf("PASS: Batch B move%u %s actual ROM animation, target scope, attack damage, HP interaction, AI and one PP\\n",moves[m],two?"Doubles":"Singles");
 }
 CHECK(core->loadState(core,bootState));
}
