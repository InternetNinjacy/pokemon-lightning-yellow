// Integration acceptance for the separately owner-merged Blaine change.
#include "constants/heal_locations.h"
static void blaineFixture(unsigned count) {
 daycareFixture(1);core->busWrite8(core,SYM_gPlayerPartyCount,count);
 for(unsigned i=count;i<6;i++)clearBytes(player(i),ABI_MON_SIZE);
 fossilWarp(MAP_GROUP(MAP_CINNABAR_ISLAND_GYM),MAP_NUM(MAP_CINNABAR_ISLAND_GYM),3,3);
 unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,8,MAP_NUM(MAP_CINNABAR_ISLAND_GYM),MAP_GROUP(MAP_CINNABAR_ISLAND_GYM));
 CHECK(object<ABI_OBJECT_COUNT);core->busWrite8(core,SYM_gSelectedObjectEvent,object);
}
static void blaineVictoryTests(void) {
 blaineFixture(2);CALL(SetLastHealLocationWarp,HEAL_LOCATION_CINNABAR_ISLAND);
 for(unsigned i=0;i<2;i++){CALL(CreateMon,player(i),SPECIES_MEWTWO,100,31,1,i,0,0);monput(player(i),ABI_DATA_MOVE1,MOVE_SWIFT);monput(player(i),ABI_DATA_PP1,99);}
 CHECK(CALL(BagGetQuantityByItemId,ITEM_TM38)==0);fossilStart(SYM_CinnabarIsland_Gym_EventScript_Blaine);waitAction();
 unsigned f,quiet=0,seen=0;const unsigned species[]={SPECIES_DONPHAN,SPECIES_PIDGEOT,SPECIES_SANDSLASH,SPECIES_AERODACTYL,SPECIES_FLYGON};
 for(f=0;f<40000;f++) {
  if(fieldScriptDone()){if(++quiet==30)break;frames(1,0);continue;}quiet=0;
  unsigned key=0;
  if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_BattleMainCB2){
   for(unsigned b=1;b<4;b+=2)if(core->busRead16(core,bmon(b)+ABI_BMON_HP))for(unsigned i=0;i<5;i++)if(core->busRead16(core,bmon(b)+ABI_BMON_SPECIES)==species[i])seen|=1U<<i;
   if(f%8==0){key=2;unsigned exec=core->busRead32(core,SYM_gBattleControllerExecFlags);
    for(unsigned b=0;b<4;b+=2)if(exec&(1U<<b)){
     unsigned fn=controller(b),c;
     if(fn==SYM_HandleInputChooseAction){c=core->busRead8(core,SYM_gActionSelectionCursor+b);key=c&2?64:c&1?32:1;break;}
     if(fn==SYM_HandleInputChooseMove){c=core->busRead8(core,SYM_gMoveSelectionCursor+b);key=c&2?64:c&1?32:1;break;}
     if(fn==SYM_HandleInputChooseTarget){key=1;break;}
    }
   }
  }else if(f%8==0)key=2;
  frames(1,key);
 }
 if(f==40000)fprintf(stderr,"Blaine win timeout cb%08x outcome%u flags%u/%u/%u\n",core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),core->busRead8(core,SYM_gBattleOutcome),CALL(FlagGet,FLAG_DEFEATED_BLAINE),CALL(FlagGet,FLAG_BADGE07_GET),CALL(FlagGet,FLAG_GOT_TM38_FROM_BLAINE));
 CHECK(f<40000);CHECK(seen==31);CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_WON);
 CHECK(CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_LEADER_BLAINE));CHECK(CALL(FlagGet,FLAG_DEFEATED_BLAINE));CHECK(CALL(FlagGet,FLAG_BADGE07_GET));CHECK(CALL(FlagGet,FLAG_GOT_TM38_FROM_BLAINE));CHECK(CALL(BagGetQuantityByItemId,ITEM_TM38)==1);
 flashRoundTrip();CHECK(CALL(FlagGet,FLAG_BADGE07_GET));CHECK(CALL(FlagGet,FLAG_GOT_TM38_FROM_BLAINE));CHECK(CALL(BagGetQuantityByItemId,ITEM_TM38)==1);
 CALL(CB2_ContinueSavedGame);frames(480,0);CHECK(fieldScriptDone());
 unsigned obj=CALL(GetObjectEventIdByLocalIdAndMap,8,MAP_NUM(MAP_CINNABAR_ISLAND_GYM),MAP_GROUP(MAP_CINNABAR_ISLAND_GYM));CHECK(obj<ABI_OBJECT_COUNT);core->busWrite8(core,SYM_gSelectedObjectEvent,obj);
 fossilStart(SYM_CinnabarIsland_Gym_EventScript_Blaine);fossilClose(1);CHECK(CALL(BagGetQuantityByItemId,ITEM_TM38)==1);CHECK(CALL(FlagGet,TRAINER_FLAGS_START+TRAINER_LEADER_BLAINE));
 printf("PASS: actual Blaine Double victory/all five opponents seen, original badge/TM38/defeat script, flash Continue and nonduplicating revisit in%u frames; controlled players, unchanged opponent party\n",f);
 CHECK(core->loadState(core,bootState));
}
static void blaineTests(void) {
 blaineFixture(1);fossilStart(SYM_CinnabarIsland_Gym_EventScript_Blaine);fossilClose(1);
 CHECK(!(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_TRAINER));
 CHECK(!CALL(FlagGet,FLAG_BADGE07_GET));CHECK(!CALL(FlagGet,FLAG_DEFEATED_BLAINE));
 blaineFixture(2);fossilStart(SYM_CinnabarIsland_Gym_EventScript_Blaine);waitAction();
 CHECK(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_TRAINER);
 CHECK(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_DOUBLE);
 CHECK(core->busRead8(core,SYM_gBattlersCount)==4);
 const unsigned species[]={SPECIES_DONPHAN,SPECIES_PIDGEOT,SPECIES_SANDSLASH,SPECIES_AERODACTYL,SPECIES_FLYGON};
 const unsigned levels[]={42,42,43,44,47};
 for(unsigned i=0;i<5;i++){
  unsigned actualSpecies=monget(enemy(i),ABI_DATA_SPECIES),actualLevel=monget(enemy(i),ABI_DATA_LEVEL);
  if(actualSpecies!=species[i]||actualLevel!=levels[i]){
   // 412 is SPECIES_EGG: discriminate corrupt encrypted party records from
   // incorrect trainer selection without changing canonical expectations.
   fprintf(stderr,"BLAINE_NATIVE_DIAGNOSTIC index=%u species=%u expected_species=%u level=%u expected_level=%u enemy_count=%u battler_count=%u flags=%08x bad_egg=%u is_egg=%u trainer=%u battle_left=%u battle_right=%u\\n",
    i,actualSpecies,species[i],actualLevel,levels[i],core->busRead8(core,SYM_gEnemyPartyCount),
    core->busRead8(core,SYM_gBattlersCount),core->busRead32(core,SYM_gBattleTypeFlags),
    monget(enemy(i),ABI_DATA_SANITY_IS_BAD_EGG),monget(enemy(i),ABI_DATA_IS_EGG),
    core->busRead16(core,SYM_gTrainerBattleOpponent_A),
    core->busRead16(core,bmon(1)+ABI_BMON_SPECIES),core->busRead16(core,bmon(3)+ABI_BMON_SPECIES));
   // Isolate a corrupted encrypted party record from the correctly loaded
   // battle battler; never rewrite/rekey Pokémon or bypass the assertion.
   fprintf(stderr,"BLAINE_ENCRYPTED_RECORD index=%u personality=%08x ot_id=%08x stored_checksum=%04x stored_flags=%02x party_index_left=%u party_index_right=%u\\n",
    i,monget(enemy(i),ABI_DATA_PERSONALITY),monget(enemy(i),ABI_DATA_OT_ID),
    core->busRead16(core,enemy(i)+0x1c),core->busRead8(core,enemy(i)+0x13),
    core->busRead8(core,SYM_gBattlerPartyIndexes+1),
    core->busRead8(core,SYM_gBattlerPartyIndexes+3));
  }
  CHECK(actualSpecies==species[i]);CHECK(actualLevel==levels[i]);
 }
 CHECK(core->busRead16(core,bmon(1)+ABI_BMON_SPECIES)==SPECIES_DONPHAN);
 CHECK(core->busRead16(core,bmon(3)+ABI_BMON_SPECIES)==SPECIES_PIDGEOT);
 CHECK(core->busRead8(core,bmon(3)+ABI_BMON_TYPE1)==TYPE_FLYING);
 CHECK(core->busRead8(core,bmon(3)+ABI_BMON_TYPE2)==TYPE_FLYING);
 core->busWrite32(core,SYM_gBattleMoveDamage,100);CALL(TypeCalc,MOVE_EARTHQUAKE,1,3);
 CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==0);
 printf("PASS: actual Blaine NPC refuses one usable Pokemon, starts trainer Double with correct five-level party/Donphan-Pidgeot leads and native Earthquake ally immunity\n");
 // Discard the modified type-check fixture; victory uses a fresh native battle.
 CHECK(core->loadState(core,bootState));
 blaineVictoryTests();
}
