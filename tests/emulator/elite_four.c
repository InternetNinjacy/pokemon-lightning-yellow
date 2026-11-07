#include "elite_four_expected.h"
static void eliteFourDataTests(void){
 for(unsigned e=0;e<sizeof(eliteIds)/sizeof(*eliteIds);e++){
  daycareFixture(1);core->busWrite32(core,SYM_gBattleTypeFlags,BATTLE_TYPE_TRAINER);
  CHECK(CALL(CreateNPCTrainerParty,SYM_gEnemyParty,eliteIds[e])==eliteSizes[e]);
  for(unsigned i=0;i<eliteSizes[e];i++){
   const struct EliteExpected *m=&eliteMons[e][i];
   CHECK(monget(enemy(i),ABI_DATA_SPECIES)==m->species);CHECK(monget(enemy(i),ABI_DATA_LEVEL)==m->level);CHECK(monget(enemy(i),ABI_DATA_HELD_ITEM)==m->item);CHECK(CALL(GetMonAbility,enemy(i))==m->ability);
   for(unsigned k=0;k<6;k++)CHECK(monget(enemy(i),ABI_DATA_HP_IV+k)==m->iv*31/255);
   for(unsigned k=0;k<4;k++){CHECK(monget(enemy(i),ABI_DATA_MOVE1+k)==m->moves[k]);CHECK(monget(enemy(i),ABI_DATA_PP1+k)==core->busRead8(core,SYM_gBattleMoves+ABI_BMOVE_SIZE*m->moves[k]+ABI_BMOVE_PP));}
  }
 }
 printf("PASS: native eight Elite Four party constructors, exact42 members/order/level/items/abilities including Agatha Inner Focus, all IVs250/255 policy and168 move slots/PP\n");
 CHECK(core->loadState(core,bootState));
}
static void eliteFourBattleTests(void){
 for(unsigned e=getenv("LIGHTNING_ELITE_LANCE_ONLY")?6:0;e<sizeof(eliteIds)/sizeof(*eliteIds);e++){
  daycareFixture(1);CALL(ZeroPlayerPartyMons);CALL(CreateMon,player(0),SPECIES_MEWTWO,100,31,1,0,0,0);core->busWrite8(core,SYM_gPlayerPartyCount,1);monput(player(0),ABI_DATA_MAX_HP,2000);monput(player(0),ABI_DATA_HP,2000);
  const unsigned moves[]={MOVE_PSYCHIC,MOVE_SURF,MOVE_ICE_BEAM,MOVE_THUNDERBOLT};
  for(unsigned k=0;k<4;k++){CALL(SetMonMoveSlot,player(0),moves[k],k);monput(player(0),ABI_DATA_PP1+k,99);}
  core->busWrite8(core,careSave1()+ABI_SAVE1_RIVAL_NAME,0xCC);core->busWrite8(core,careSave1()+ABI_SAVE1_RIVAL_NAME+1,0xFF);
  unsigned rematch=e&1;if(rematch){CALL(FlagSet,FLAG_SYS_CAN_LINK_WITH_RS);CALL(FlagSet,FLAG_SYS_GAME_CLEAR);}
  unsigned stage=e<2?1:e<4?2:e<6?3:4;CALL(VarSet,VAR_MAP_SCENE_POKEMON_LEAGUE,stage);
  // All League rooms share map group1; derive group/number from packed map constant.
  fossilWarp(eliteMaps[e]>>8,eliteMaps[e]&255,4,6);
  core->busWrite8(core,SYM_gSelectedObjectEvent,CALL(GetObjectEventIdByLocalIdAndMap,1,eliteMaps[e]&255,eliteMaps[e]>>8));
  fossilStart(eliteEntries[e]);unsigned started=0,seen=0,won=0,f;
  for(f=0;f<60000;f++){
   if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_BattleMainCB2){
    started=1;CHECK(core->busRead16(core,SYM_gTrainerBattleOpponent_A)==eliteIds[e]);CHECK(!(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_DOUBLE));
    // Controlled strong player ensures battle progression tests are not balance claims.
    core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,2000);core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,2000);
    unsigned species=core->busRead16(core,bmon(1)+ABI_BMON_SPECIES);
    if(core->busRead16(core,bmon(1)+ABI_BMON_HP))for(unsigned i=0;i<eliteSizes[e];i++)if(species==eliteMons[e][i].species)seen|=1U<<i;
   }
   if(started&&CALL(FlagGet,eliteFlags[e])&&fieldScriptDone()){won=1;break;}
   unsigned key=0;
   if(f%8==0){key=2;
    if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_BattleMainCB2&&controller(0)==SYM_HandleInputChooseAction){unsigned c=core->busRead8(core,SYM_gActionSelectionCursor);key=c&2?64:c&1?32:1;}
    if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_BattleMainCB2&&controller(0)==SYM_HandleInputChooseMove){unsigned c=core->busRead8(core,SYM_gMoveSelectionCursor),species=core->busRead16(core,bmon(1)+ABI_BMON_SPECIES),desired=species==SPECIES_TYRANITAR?1:(species==SPECIES_CROBAT||species==SPECIES_AERODACTYL||species==SPECIES_ARTICUNO||species==SPECIES_ZAPDOS||species==SPECIES_MOLTRES||species==SPECIES_DRAGONITE)?2:0;key=(c>>1)!=(desired>>1)?(desired&2?128:64):(c&1)!=(desired&1)?(desired&1?16:32):1;}
   }frames(1,key);
  }
  if(!won){screenshot("/tmp/elite-timeout.ppm");fprintf(stderr,"Elite timeout e%u started%u outcome%u seen%x expected%x script%08x cb%08x ctrl%08x HP%u/%u flag%u\n",e,started,core->busRead8(core,SYM_gBattleOutcome),seen,(1U<<eliteSizes[e])-1,core->busRead32(core,SYM_sGlobalScriptContext+ABI_SCRIPT_PTR),core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),controller(0),monget(player(0),ABI_DATA_HP),core->busRead16(core,bmon(0)+ABI_BMON_HP),CALL(FlagGet,eliteFlags[e]));}
  CHECK(won&&seen==((1U<<eliteSizes[e])-1));CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_WON);flashRoundTrip();CHECK(CALL(FlagGet,eliteFlags[e]));
  printf("PASS: actual League room trainer%u %s native victory, all%u members sent, field/door defeat flag and flash persistence in%u frames; controlled strong player\n",eliteIds[e],rematch?"rematch":"initial",eliteSizes[e],f);
 }
 CHECK(core->loadState(core,bootState));
}
static void eliteFourTests(void){eliteFourDataTests();eliteFourBattleTests();}
