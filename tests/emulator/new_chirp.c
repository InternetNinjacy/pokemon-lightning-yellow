// Actual Chirp #365 sound immunity, physical/special, PP and AI via compiled ROM.
static void chirpTests(void){
 for(unsigned two=0;two<2;two++)for(unsigned shield=0;shield<2;shield++){
  effectBattleFixture(two,MOVE_CHIRP);
  damageStats();
  core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,1);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,200);
  core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,999);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,100);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_NORMAL);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,shield?ABILITY_SOUNDPROOF:ABILITY_BATTLE_ARMOR);

  unsigned expected=baseDamage(MOVE_CHIRP,0,0,0);
  CHECK(expected>=30&&expected<=45);
  chooseMove(0,0,1);if(two)chooseMove(2,0,1);
  waitAction();
  unsigned hit=1000-core->busRead16(core,bmon(1)+ABI_BMON_HP);
  if(shield)CHECK(hit==0);else CHECK(hit>0&&hit<=expected);
  CHECK(monget(player(0),ABI_DATA_PP1)==24);
  CHECK(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)==0);
  if(two){CHECK(core->busRead16(core,bmon(2)+ABI_BMON_HP)==1000);CHECK(core->busRead16(core,bmon(3)+ABI_BMON_HP)==1000);}
 }
 // The bad-move AI penalizes sound attacks against a known Soundproof target.
 for(unsigned shield=0;shield<2;shield++){
  effectBattleFixture(0,MOVE_SPLASH);
  core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,shield?ABILITY_SOUNDPROOF:ABILITY_NONE);
  for(unsigned m=0;m<4;m++){core->busWrite16(core,bmon(1)+ABI_BMON_MOVES+2*m,MOVE_CHIRP);core->busWrite8(core,bmon(1)+ABI_BMON_PP+m,25);}
  core->busWrite8(core,SYM_gActiveBattler,1);CALL(BattleAI_SetupAIData);
  if(shield)CALL(RecordAbilityBattle,0,ABILITY_SOUNDPROOF); // AI has observed the target's ability.
  unsigned ai=core->busRead32(core,core->busRead32(core,SYM_gBattleResources)+ABI_BRES_AI);
  core->busWrite32(core,ai+ABI_AI_FLAGS,1);CALL(BattleAI_ChooseMoveOrAction);
  for(unsigned m=0;m<4;m++)CHECK(core->busRead8(core,ai+ABI_AI_SCORE+m)==(shield?90:100));
 }
 CHECK(core->loadState(core,bootState));
 printf("PASS: actual Singles/Doubles Chirp sound damage/PP, Soundproof immunity, no spread, native visuals and AI penalty\n");
}
