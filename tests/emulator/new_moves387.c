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


// Volt Switch cannot open a replacement screen when every party member is
// already active, and electric immunity must not trigger pivot behavior.
static void voltSwitch388NoBenchTests(void){
 for(unsigned two=0;two<2;two++)for(unsigned immune=0;immune<2;immune++){
  effectBattleFixture(two,MOVE_VOLT_SWITCH);
  unsigned flags=core->busRead32(core,SYM_gBattleTypeFlags);
  damageStats();core->busWrite32(core,SYM_gBattleTypeFlags,flags);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,200);
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,1);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,100);
  if(immune){
   core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_GROUND);
   core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_GROUND);
  }
  unsigned beforeIndex=core->busRead8(core,SYM_gBattlerPartyIndexes);
  chooseMove(0,0,1);if(two)chooseMove(2,0,1);
  waitActionBudget(4800);
  CHECK(monget(player(0),ABI_DATA_PP1)==19);
  CHECK((core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000)==!immune);
  CHECK(core->busRead8(core,SYM_gBattlerPartyIndexes)==beforeIndex);
 }
 CHECK(core->loadState(core,bootState));
 printf("PASS: native Volt Switch #388 Single/Double dealt damage or hit Ground immunity, one PP, no illegal switch with no bench\n");
}

// Real Volt Switch pivot acceptance: create an unused, healthy party member,
// use actual battle move input, then select it through the player's native
// party screen with A presses. No battler indexes or switch buffers are forged.
static void voltSwitch388RealPivotTests(void){
 for(unsigned two=0;two<2;two++){
  effectBattleFixture(two,MOVE_VOLT_SWITCH);
  unsigned reserve=two?2:1;
  CALL(CreateMon,player(reserve),SPECIES_SNORLAX,50,31,1,12348,0,1234);
  CHECK(monget(player(reserve),ABI_DATA_SPECIES)==SPECIES_SNORLAX);
  core->busWrite8(core,SYM_gPlayerPartyCount,reserve+1);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,200);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,100);
  core->busWrite16(core,bmon(0)+ABI_BMON_SPEED,600);
  core->busWrite16(core,bmon(1)+ABI_BMON_SPEED,1);
  unsigned initialCB=core->busRead32(core,SYM_gMain+ABI_MAIN_CB2);
  CHECK(core->busRead16(core,SYM_gBattlerPartyIndexes)==0);
  // Production declares gBattlerPartyIndexes as u16[4], not u8[4].
  // Snapshot the partner's actual 16-bit slot and live identity before pivot.
  unsigned partnerSlotBefore=two?core->busRead16(core,SYM_gBattlerPartyIndexes+2*sizeof(u16)):0;
  unsigned partnerSpeciesBefore=two?core->busRead16(core,bmon(2)+ABI_BMON_SPECIES):0;
  if(two){
   CHECK(partnerSlotBefore==1);
   CHECK(partnerSpeciesBefore==monget(player(1),ABI_DATA_SPECIES));
  }
  chooseMove(0,0,1);if(two)chooseMove(2,0,1);
  unsigned frame,seenPartyScreen=0,switched=0,selectionSent=0,partyFrames=0;
  for(frame=0;frame<7200;frame++){
   unsigned cb=core->busRead32(core,SYM_gMain+ABI_MAIN_CB2);
   if(cb!=initialCB){seenPartyScreen=1;partyFrames++;}
   if(seenPartyScreen&&!selectionSent&&partyFrames>=120){
    // Singles DOWN goes 0 -> 1; Doubles RIGHT goes 0 -> 2.
    // Both select the actual unused reserve through native menu input.
    // Confirm the Pokemon, then confirm its SHIFT action.
    press(two?16:128);
    press(1);
    press(1);
    selectionSent=1;
   }
   if(core->busRead8(core,SYM_gBattlerPartyIndexes)==reserve)switched=1;
   if(switched&&controller(0)==SYM_HandleInputChooseAction)break;
   frames(1,selectionSent?(frame%12)<3?1:0:0);
  }
  if(frame==7200)fprintf(stderr,"VOLT_PIVOT_TIMEOUT two=%u party=%u switched=%u cb=%08x ctrl=%08x script=%08x exec=%08x\\n",two,seenPartyScreen,switched,core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),controller(0),core->busRead32(core,SYM_gBattlescriptCurrInstr),core->busRead32(core,SYM_gBattleControllerExecFlags));
  CHECK(frame<7200);
  CHECK(seenPartyScreen);
  CHECK(switched);
  CHECK(core->busRead8(core,SYM_gBattlerPartyIndexes)==reserve);
  CHECK(core->busRead16(core,bmon(0)+ABI_BMON_SPECIES)==SPECIES_SNORLAX);
  CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)<1000);
  CHECK(monget(player(0),ABI_DATA_PP1)==19);
  if(two){
   unsigned partnerIndex=core->busRead16(core,SYM_gBattlerPartyIndexes+2*sizeof(u16));
   unsigned partnerSpecies=core->busRead16(core,bmon(2)+ABI_BMON_SPECIES);
   unsigned partySpecies=monget(player(1),ABI_DATA_SPECIES);
   fprintf(stderr,"VOLT_PIVOT_PARTNER_DIAGNOSTIC initialIndex=%u partnerIndex=%u initialSpecies=%u partnerSpecies=%u party1Species=%u reserve=%u playerCount=%u battlerCount=%u flags=%08x\\n",
    partnerSlotBefore,partnerIndex,partnerSpeciesBefore,partnerSpecies,partySpecies,reserve,
    core->busRead8(core,SYM_gPlayerPartyCount),core->busRead8(core,SYM_gBattlersCount),core->busRead32(core,SYM_gBattleTypeFlags));
   CHECK(partnerIndex==partnerSlotBefore);
   CHECK(partnerIndex==1);
   CHECK(partnerSpecies==partnerSpeciesBefore);
   CHECK(partnerSpecies==partySpecies);
  }
  printf("PASS: native Volt Switch #388 %s actual damage, genuine player party menu and selected bench replacement; PP and partner identity\\n",two?"Doubles":"Singles");
 }
 CHECK(core->loadState(core,bootState));
}

// Native compiled-ROM boundary checks for Thunder Fang's two distinct
// secondary-effect commands. Each rolls independently through the original
// battle command, with effects isolated so neither can mask the other.
static void thunderFang389IndependentEffects(void){
 effectBattleFixture(0,MOVE_THUNDER_FANG);
 unsigned script=CALL(AllocZeroed,32);CHECK(script);
 void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
 const unsigned rolls[]={9,10,19,20};
 for(unsigned serene=0;serene<2;serene++)for(unsigned effect=0;effect<2;effect++)for(unsigned r=0;r<4;r++){
  CHECK(core->loadState(core,state));
  core->busWrite8(core,SYM_gBattlerAttacker,0);
  core->busWrite8(core,SYM_gBattlerTarget,1);
  core->busWrite8(core,SYM_gBattlerByTurnOrder,0);
  core->busWrite8(core,SYM_gBattlerByTurnOrder+1,1);
  core->busWrite8(core,SYM_gCurrentTurnActionNumber,0);
  core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,serene?ABILITY_SERENE_GRACE:ABILITY_NONE);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_NONE);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_THUNDER_FANG);
  core->busWrite8(core,SYM_gMoveResultFlags,0);
  core->busWrite32(core,SYM_gHitMarker,0);
  core->busWrite8(core,SYM_gBattleCommunication+MOVE_EFFECT_BYTE,effect?MOVE_EFFECT_FLINCH:MOVE_EFFECT_PARALYSIS);
  core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(rolls[r]));
  core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);
  CALL(Cmd_seteffectwithchance);
  unsigned para=!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_PARALYSIS);
  unsigned flinch=!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_FLINCHED);
  CHECK((effect?flinch:para)==(rolls[r]<(serene?20:10)));
  CHECK(effect?!para:!flinch);
 }
 CHECK(core->loadState(core,state));CALL(Free,script);free(state);
 CHECK(core->loadState(core,bootState));
 printf("PASS: native Thunder Fang #389 independent paralysis/flinch exact 10/20%% RNG boundaries and non-cross-effect guards\\n");
}

// Exercise the real secondary-effect command with relevant battle guards.
// Protect the distinction between primary paralysis, flinch, and abilities.
static void thunderFang389GuardMatrix(void){
 effectBattleFixture(0,MOVE_THUNDER_FANG);
 unsigned script=CALL(AllocZeroed,32);CHECK(script);
 void *state=malloc(core->stateSize(core));CHECK(state&&core->saveState(core,state));
 for(unsigned guard=0;guard<8;guard++)for(unsigned effect=0;effect<2;effect++){
  CHECK(core->loadState(core,state));
  core->busWrite8(core,SYM_gBattlerAttacker,0);
  core->busWrite8(core,SYM_gBattlerTarget,1);
  core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,ABILITY_NONE);
  core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_NONE);
  core->busWrite8(core,SYM_gBattlerByTurnOrder,guard==3?1:0);
  core->busWrite8(core,SYM_gBattlerByTurnOrder+1,guard==3?0:1);
  core->busWrite8(core,SYM_gCurrentTurnActionNumber,guard==3?1:0);
  core->busWrite16(core,SYM_gCurrentMove,MOVE_THUNDER_FANG);
  core->busWrite8(core,SYM_gMoveResultFlags,0);
  core->busWrite32(core,SYM_gHitMarker,0);
  if(guard==1){
   core->busWrite32(core,bmon(1)+ABI_BMON_STATUS2,STATUS2_SUBSTITUTE);
   core->busWrite8(core,SYM_gDisableStructs+ABI_DISABLE_SIZE+ABI_DISABLE_SUB_HP,200);
  }
  if(guard==2)core->busWrite16(core,SYM_gSideStatuses+2,SIDE_STATUS_SAFEGUARD);
  if(guard==4){
   core->busWrite8(core,bmon(1)+ABI_BMON_TYPE1,TYPE_ELECTRIC);
   core->busWrite8(core,bmon(1)+ABI_BMON_TYPE2,TYPE_ELECTRIC);
  }
  if(guard==5)core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_INNER_FOCUS);
  if(guard==6)core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_SHIELD_DUST);
  if(guard==7)core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_LIMBER);
  core->busWrite8(core,SYM_gBattleCommunication+MOVE_EFFECT_BYTE,effect?MOVE_EFFECT_FLINCH:MOVE_EFFECT_PARALYSIS);
  core->busWrite32(core,SYM_gRngValue,iceFangChanceSeed(0));
  core->busWrite32(core,SYM_gBattlescriptCurrInstr,script);
  CALL(Cmd_seteffectwithchance);
  unsigned para=!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS1)&STATUS1_PARALYSIS);
  unsigned flinch=!!(core->busRead32(core,bmon(1)+ABI_BMON_STATUS2)&STATUS2_FLINCHED);
  unsigned allowed=effect?(guard==0||guard==2||guard==4||guard==7):
                          (guard==0||guard==3||guard==5);
  CHECK((effect?flinch:para)==allowed);
  CHECK(effect?!para:!flinch);
 }
 CHECK(core->loadState(core,state));CALL(Free,script);free(state);
 CHECK(core->loadState(core,bootState));
 printf("PASS: native Thunder Fang #389 independent paralysis/flinch guard matrix: substitute, safeguard, turn order, Electric, Inner Focus, Shield Dust, Limber\\n");
}

// Verify actual flash serialization for all eight new move slots in two
// separately encrypted boxed Pokémon, not a transient CPU snapshot.
static void moves387to394SavePersistence(void){
 const unsigned ids[]={MOVE_DISCHARGE,MOVE_VOLT_SWITCH,MOVE_THUNDER_FANG,MOVE_WILD_CHARGE,
                       MOVE_SEED_BOMB,MOVE_PETAL_BLIZZARD,MOVE_WOOD_HAMMER,MOVE_AURA_SPHERE};
 const unsigned pp[]={9,11,7,8,6,5,4,3};
 CHECK(core->loadState(core,bootState));
 GBASavedataForceType(&((struct GBA*)core->board)->memory.savedata,SAVEDATA_FLASH1M);
 void *oldFlash=NULL;
 size_t oldSize=core->savedataClone(core,&oldFlash);
 CHECK(oldSize==0x20000&&oldFlash);
 unsigned char *blank=malloc(0x20000);CHECK(blank);
 memset(blank,255,0x20000);
 CHECK(core->savedataRestore(core,blank,0x20000,true));
 free(blank);
 initSave();
 for(unsigned p=0;p<2;p++){
  CALL(CreateMon,player(p),p?SPECIES_PIKACHU:SPECIES_SQUIRTLE,50,31,1,9001+p,0,777);
  for(unsigned s=0;s<4;s++){
   CALL(SetMonMoveSlot,player(p),ids[4*p+s],s);
   monput(player(p),ABI_DATA_PP1+s,pp[4*p+s]);
  }
  monput(player(p),ABI_DATA_PP_BONUSES,p?5:1);
 }
 core->busWrite8(core,SYM_gPlayerPartyCount,2);
 unsigned char saved[2][ABI_BOX_SIZE];
 for(unsigned p=0;p<2;p++)readBytes(player(p),saved[p],ABI_BOX_SIZE);
 CHECK(CALL(TrySavingData,0)==1);
 for(unsigned k=0;k<ABI_SAVE1_SIZE;k+=4)core->busWrite32(core,SYM_gSaveBlock1+k,0);
 for(unsigned k=0;k<ABI_SAVE2_SIZE;k+=4)core->busWrite32(core,SYM_gSaveBlock2+k,0);
 for(unsigned p=0;p<2;p++)for(unsigned s=0;s<4;s++)CALL(SetMonMoveSlot,player(p),MOVE_SPLASH,s);
 CHECK(CALL(LoadGameSave,0)==1);
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==2);
 for(unsigned p=0;p<2;p++){
  sameBytes(player(p),saved[p],ABI_BOX_SIZE);
  for(unsigned s=0;s<4;s++){
   CHECK(monget(player(p),ABI_DATA_MOVE1+s)==ids[4*p+s]);
   CHECK(monget(player(p),ABI_DATA_PP1+s)==pp[4*p+s]);
  }
  CHECK(monget(player(p),ABI_DATA_PP_BONUSES)==(p?5:1));
  CHECK(!monget(player(p),ABI_DATA_SANITY_IS_BAD_EGG));
 }
 CHECK(core->savedataRestore(core,oldFlash,oldSize,true));
 free(oldFlash);
 CHECK(core->loadState(core,bootState));
 printf("PASS: native #387-394 move IDs, spent PP, PP Ups and encrypted two-mon records survive real flash save/load\n");
}
