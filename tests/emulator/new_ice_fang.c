// Ice Fang #372: actual compiled ROM Single/Double trials, including transient
// flinch status before the engine legitimately clears it at end of turn.
struct IceFangObserved {unsigned freeze;unsigned flinch;};
static struct IceFangObserved iceFangObservedTurn(void){
 struct IceFangObserved v={0,0};unsigned frame;
 for(frame=0;frame<4800;frame++){
  if(controller(0)==SYM_HandleInputChooseAction)break;
  frames(1,(frame%12)<3?1:0);
  v.freeze |= !!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_FREEZE);
  v.flinch |= !!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_FLINCHED);
 }
 if(frame==4800)fprintf(stderr,"Ice Fang timeout script=%08x ctrl=%08x\n",core->busRead32(core,SYM_gBattlescriptCurrInstr),controller(0));
 CHECK(frame<4800);frames(3,0);return v;
}
static void iceFangTests(void){
 for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,MOVE_ICE_FANG);damageStats();
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,200);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,1);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,999);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_BATTLE_ARMOR);
  unsigned expected=baseDamage(MOVE_ICE_FANG,0,0,0);
  CHECK(expected>25&&expected<140);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite8(core,core->busRead32(core,SYM_gBattleStruct)+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_ICE_FANG);
  CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==expected);
  void *initial=malloc(core->stateSize(core));CHECK(initial&&core->saveState(core,initial));
  for(unsigned serene=0;serene<2;serene++){
   CHECK(core->loadState(core,initial));
   core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,serene?ABILITY_SERENE_GRACE:ABILITY_NONE);
   void *snap=malloc(core->stateSize(core));CHECK(snap&&core->saveState(core,snap));
   unsigned combinations=0,misses=0;
   for(unsigned trial=0;trial<(serene?260:700)&&(combinations!=15||!misses);trial++){
    CHECK(core->loadState(core,snap));
    core->busWrite32(core,SYM_gRngValue,0x37200000+trial*7919+two*17+serene*65537);
    chooseMove(0,0,1);if(two)chooseMove(2,0,1);
    struct IceFangObserved observed=iceFangObservedTurn();
    unsigned hp=core->busRead16(core,bmon(1)+ABI_BMON_HP);
    CHECK(monget(player(0),ABI_DATA_PP1)==14);
    CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==1000);
    if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
    if(hp==1000){misses++;CHECK(!observed.freeze&&!observed.flinch);}
    else{
     CHECK(hp<1000&&hp>=1000-expected);
     combinations |= 1U<<(observed.freeze+2*observed.flinch);
    }
   }
   CHECK(combinations==15&&misses>0);
   free(snap);
   printf("PASS: Ice Fang native %s %s all four independent freeze/flinch outcomes, misses, PP, Physical AI damage and existing-art termination\n",two?"Doubles":"Singles",serene?"Serene Grace":"base chance");
  }
  free(initial);
 }
 CHECK(core->loadState(core,bootState));
}
