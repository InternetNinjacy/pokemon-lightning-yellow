// BTL-005 Batch A: native Singles/Doubles acceptance on real ROM.
// The dedicated staging entry point does not certify unimplemented B/C slots.
static void batchANativeTests(void){
 const unsigned moves[]={
  MOVE_SEED_BOMB,MOVE_PETAL_BLIZZARD,MOVE_AURA_SPHERE,MOVE_FOCUS_BLAST,
  MOVE_FLASH_CANNON,MOVE_MIRROR_SHOT,MOVE_IRON_HEAD,MOVE_BULLET_PUNCH,
  MOVE_AIR_SLASH,MOVE_HEART_STAMP
 };
 const unsigned pp[]={15,15,20,5,10,10,15,30,15,25};
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
  core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,m==7?1:500);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,m==7?500:1);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
  if(m==7){
   // Positive priority control versus a faster opponent using Swift.
   for(unsigned slot=0;slot<4;slot++){
    CALL(SetMonMoveSlot,enemy(0),MOVE_SWIFT,slot);
    core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*slot,MOVE_SWIFT);
    core->busWrite8(core,bmon(1)+ABI_BMON_PP+slot,20);
   }
  }
  unsigned raw=baseDamage(moves[m],0,0,0);
  CHECK(raw>0&&raw<900);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite16(core,SYM_gCurrentMove,moves[m]);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==raw);
  void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
  unsigned hit=0,miss=0,proc=0;
  unsigned trials=(m==3||m==4||m==5||m==6||m==8||m==9)?48:10;
  for(unsigned t=0;t<trials && (!hit||((m==3||m==5||m==8)&&!miss) ||
     ((m==3||m==4||m==5||m==6||m==8||m==9)&&!proc));t++){
   CHECK(core->loadState(core,state));
   core->busWrite32(core,SYM_gRngValue,0xBA700000U + m*19001U + two*101U + t*7919U);
   chooseMove(0,0,1);
   if(two)chooseMove(2,0,1);
   unsigned frame=0,seen=0;
   for(;frame<4800;frame++){
    if(controller(0)==SYM_HandleInputChooseAction)break;
    frames(1,(frame%12)<3?1:0);
    if(!seen&&core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000){
     if(m==7)CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
     seen=1;
    }
   }
   CHECK(frame<4800);
   frames(3,0);
   unsigned dealt=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
   CHECK(monget(player(0),ABI_DATA_PP1)==pp[m]-1);
   if(dealt){
    CHECK(dealt>=raw*85/100&&dealt<=raw);
    hit++;
   }else miss++;
   if(two){
    if(m==1){
     CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)<1000);
     CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)<1000);
    }else{
     CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);
     CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);
    }
   }
   if(m==3||m==4)proc|=(core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_SPDEF)<6);
   if(m==5)proc|=(core->busRead8(core,bmon(1)+ABI_BMON_STAGES+STAT_ACC)<6);
   // Flinch must be verified in a later dedicated real turn-order test;
   // do not manufacture a successful proc in this smoke suite.
  }
  CHECK(hit);
  if(m==3||m==5||m==8)CHECK(miss);
  if(m==3||m==4||m==5)CHECK(proc);
  free(state);
  printf("PASS: Batch A move%u %s native targeting, damage, actual animation, PP and partial proc/priority probe (flinch acceptance pending)\\n",moves[m],two?"Doubles":"Singles");
 }
 CHECK(core->loadState(core,bootState));
}
