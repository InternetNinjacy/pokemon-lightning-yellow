// Native constructor and actual battle acceptance, independent of static audits.
#include "rival_expected.h"
static void rivalDataTests(void) {
 for(unsigned e=0;e<8;e++)for(unsigned variant=0;variant<3;variant++) {
  daycareFixture(1);core->busWrite32(core,SYM_gBattleTypeFlags,BATTLE_TYPE_TRAINER);
  CHECK(CALL(CreateNPCTrainerParty,SYM_gEnemyParty,rivalIds[e][variant])==rivalSizes[e]);
  for(unsigned i=0;i<rivalSizes[e];i++) {
   const struct RivalExpected *m=&rivalMons[e][i];
   CHECK(monget(enemy(i),ABI_DATA_SPECIES)==m->species);CHECK(monget(enemy(i),ABI_DATA_LEVEL)==m->level);
   CHECK(monget(enemy(i),ABI_DATA_HELD_ITEM)==m->item);CHECK(monget(enemy(i),ABI_DATA_HP_IV)==m->iv*31/255);
   CHECK(CALL(GetMonAbility,enemy(i))!=ABILITY_NONE);
   for(unsigned k=0;k<4;k++){CHECK(monget(enemy(i),ABI_DATA_MOVE1+k)==m->moves[k]);CHECK(monget(enemy(i),ABI_DATA_PP1+k)==core->busRead8(core,SYM_gBattleMoves+ABI_BMOVE_SIZE*m->moves[k]+ABI_BMOVE_PP));}
  }
 }
 const unsigned family[]={SPECIES_DRATINI,SPECIES_DRAGONAIR,SPECIES_DRAGONITE};
 for(unsigned i=0;i<3;i++) {
  uint32_t moves=core->busRead32(core,SYM_gLevelUpLearnsets+family[i]*4);unsigned found=0;
  for(unsigned k=0;k<30;k++){unsigned word=core->busRead16(core,moves+k*2);if(word==0xffff)break;if(word==((1U<<9)|MOVE_TACKLE))found|=1;if(word==((12U<<9)|MOVE_TWISTER))found|=2;if(word==((22U<<9)|MOVE_DRAGON_BREATH))found|=4;if(word==((1U<<9)|MOVE_TWISTER))found|=8;CHECK((word&511)!=MOVE_DRAGON_RAGE);}
  CHECK((found&7)==7);if(i)CHECK(found&8);
  CALL(CreateMon,player(0),family[i],22,31,1,0,0,0);unsigned hasBreath=0;
  for(unsigned k=0;k<4;k++){unsigned m=monget(player(0),ABI_DATA_MOVE1+k);hasBreath|=m==MOVE_DRAGON_BREATH;CHECK(m!=MOVE_DRAGON_RAGE);}CHECK(hasBreath);
 }
 CHECK(core->loadState(core,bootState));initSave();create(SPECIES_DRATINI,29);CHECK(target(EVO_MODE_NORMAL,0)==0);put(ABI_DATA_LEVEL,30);CHECK(target(EVO_MODE_NORMAL,0)==SPECIES_DRAGONAIR);
 create(SPECIES_DRAGONAIR,54);CHECK(target(EVO_MODE_NORMAL,0)==0);put(ABI_DATA_LEVEL,55);CHECK(target(EVO_MODE_NORMAL,0)==SPECIES_DRAGONITE);
 printf("PASS: actual native constructors for all24 rival IDs/99 Pokemon, exact levels/IVs/items/moves/PP/abilities; native family learnsets and unchanged30/55 evolution thresholds\n");
 CHECK(core->loadState(core,bootState));
}
static const uint32_t rivalEntries[]={SYM_PalletTown_ProfessorOaksLab_EventScript_RivalBattleSquirtle,SYM_Route22_EventScript_EarlyRivalSquirtle,SYM_CeruleanCity_EventScript_RivalSquirtle,SYM_SSAnne_2F_Corridor_EventScript_RivalSquirtle,SYM_PokemonTower_2F_EventScript_RivalSquirtle,SYM_SilphCo_7F_EventScript_RivalSquirtle,SYM_Route22_EventScript_LateRivalSquirtle,SYM_PokemonLeague_ChampionsRoom_EventScript_BattleSquirtle};
static void rivalBattleTests(void) {
 for(unsigned e=getenv("LIGHTNING_RIVAL_CHAMP_ONLY")?7:0;e<8;e++) {
  // Controlled arena avoids unrelated story walking. Execute the existing native
  // rival battle command including Oak tutorial; source event scripts unchanged.
  rangerFixture(1);CALL(CreateMon,player(0),SPECIES_MEWTWO,100,31,1,0,0,0);monput(player(0),ABI_DATA_MOVE1,MOVE_PSYCHIC);monput(player(0),ABI_DATA_PP1,99);monput(player(0),ABI_DATA_MOVE2,MOVE_SURF);monput(player(0),ABI_DATA_PP2,99);
  core->busWrite8(core,careSave1()+ABI_SAVE1_RIVAL_NAME,0xCC);core->busWrite8(core,careSave1()+ABI_SAVE1_RIVAL_NAME+1,0xFF);
  CALL(VarSet,VAR_STARTER_MON,2);fossilStart(rivalEntries[e]);
  unsigned f,started=0,won=0,seen=0;
  for(f=0;f<40000;f++) {
   if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_BattleMainCB2) {
    started=1;CHECK(core->busRead16(core,SYM_gTrainerBattleOpponent_A)==rivalIds[e][0]);
    unsigned species=core->busRead16(core,bmon(1)+ABI_BMON_SPECIES);
    if(core->busRead16(core,bmon(1)+ABI_BMON_HP))for(unsigned i=0;i<rivalSizes[e];i++)if(species==rivalMons[e][i].species)seen|=1U<<i;
   }
   if(started&&core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_WON){won=1;break;}
   unsigned key=0;if(f%8==0){key=e==0?1:2;
    if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_BattleMainCB2 && (core->busRead32(core,SYM_gBattleControllerExecFlags)&1U)) {
     unsigned fn=controller(0);
     if(fn==SYM_HandleInputChooseAction){unsigned c=core->busRead8(core,SYM_gActionSelectionCursor);key=c&2?64:c&1?32:1;}
     if(fn==SYM_HandleInputChooseMove || fn==SYM_OakOldManHandleInputChooseMove){unsigned c=core->busRead8(core,SYM_gMoveSelectionCursor),species=core->busRead16(core,bmon(1)+ABI_BMON_SPECIES),desired=(species==SPECIES_TYRANITAR||species==SPECIES_PUPITAR);key=c&2?64:(c&1)!=desired?(desired?16:32):1;}
    }
   }frames(1,key);
  }
  if(!won)screenshot("/tmp/rival-timeout.ppm");
  if(!won)fprintf(stderr,"Rival timeout e%u started%u outcome%u controller%08x callback%08x\n",e,started,core->busRead8(core,SYM_gBattleOutcome),controller(0),core->busRead32(core,SYM_gMain+ABI_MAIN_CB2));
  CHECK(started&&won);CHECK(seen==((1U<<rivalSizes[e])-1));
  printf("PASS: actual approved rival encounter%u native%s victory, every replacement seen, unmodified opponent stats; %u frames (controlled arena, not full story progression)\n",e+1,e==0?" Oak-tutorial":"",f);
 }
 CHECK(core->loadState(core,bootState));
}
static void rivalTests(void){rivalDataTests();rivalBattleTests();}
