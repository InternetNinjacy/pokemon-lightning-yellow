// Execute the compiled field scripts and native menus, inventory, flags and saves.
#include "constants/vars.h"
#include "constants/global.h"
static const unsigned reviveState=VAR_MAP_SCENE_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM_REVIVE_STATE;
static const unsigned fossilKind=VAR_MAP_SCENE_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM_WHICH_FOSSIL;
static void fossilWarp(unsigned group,unsigned map,unsigned x,unsigned y) {
 CALL(SetWarpDestination,group,map,0xFFFFFFFF,x,y);CALL(WarpIntoMap);
 CALL(ResetInitialPlayerAvatarState);CALL(CB2_LoadMap);frames(360,0);
 CHECK(fieldScriptDone());
}
static void fossilFixture(unsigned aide) {
 daycareFixture(1); // independent blank flash and initialized encrypted save state
 if(aide)fossilWarp(MAP_GROUP(MAP_ROUTE10_POKEMON_CENTER_1F),MAP_NUM(MAP_ROUTE10_POKEMON_CENTER_1F),12,6);
 else fossilWarp(MAP_GROUP(MAP_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM),MAP_NUM(MAP_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM),11,5);
 CALL(ClearBag);
 // Existing objects/scripts are retained; inject the same interaction entry
 // point to avoid a wandering NPC's random position changing test timing.
 core->busWrite8(core,SYM_gSelectedObjectEvent,CALL(GetObjectEventIdByLocalIdAndMap,aide?5:2,
  aide?MAP_NUM(MAP_ROUTE10_POKEMON_CENTER_1F):MAP_NUM(MAP_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM),
  aide?MAP_GROUP(MAP_ROUTE10_POKEMON_CENTER_1F):MAP_GROUP(MAP_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM)));
}
static void fossilStart(uint32_t script) {CALL(ScriptContext_SetupScript,script);}
static unsigned fossilTextPaused(void) {
 for(unsigned i=0;i<ABI_TEXT_COUNT;i++) {
  unsigned addr=SYM_sTextPrinters+i*ABI_TEXT_SIZE;
  unsigned state=core->busRead8(core,addr+ABI_TEXT_STATE);
  if(core->busRead8(core,addr+ABI_TEXT_ACTIVE) && (state==ABI_TEXT_CLEAR || state==ABI_TEXT_SCROLL_START || state==ABI_TEXT_WAIT))return 1;
 }
 // Script mode2 is the engine native-wait mode, not stale nativePtr data.
 if(core->busRead8(core,SYM_sGlobalScriptContext+ABI_SCRIPT_MODE)==2
  && (core->busRead32(core,SYM_sGlobalScriptContext+ABI_SCRIPT_NATIVE)&~1U)==SYM_WaitForAorBPress)return 1;
 return 0;
}
static void fossilWaitMenu(uint32_t task) {
 unsigned f;
 for(f=0;f<6000;f++) {
  // Task functions are Thumb pointers in the target task table.
  if(CALL(FuncIsActiveTask,task|1U))break;
  frames(1,(f%12)==0 && fossilTextPaused()?1:0);
 }
 if(f==6000)fprintf(stderr,"Fossil menu timeout task%08x script%08x status%u\n",task,core->busRead32(core,SYM_sGlobalScriptContext+ABI_SCRIPT_PTR),core->busRead8(core,SYM_sGlobalScriptContextStatus));
 CHECK(f<6000);frames(10,0);
}
static void fossilClose(unsigned key) {
 unsigned f;
 for(f=0;f<6000;f++) {
  frames(1,(f%12)<3?key:0);
  if(f>20 && fieldScriptDone())break;
 }
 if(f==6000){fprintf(stderr,"Fossil close timeout script%08x status%u claw%u flag%u\n",core->busRead32(core,SYM_sGlobalScriptContext+ABI_SCRIPT_PTR),core->busRead8(core,SYM_sGlobalScriptContextStatus),CALL(BagGetQuantityByItemId,ITEM_CLAW_FOSSIL),CALL(FlagGet,FLAG_GOT_CLAW_FOSSIL_FROM_OAKS_AIDE));screenshot("/tmp/fossil-timeout.ppm");}
 CHECK(f<6000);frames(6,0);
}
static void fossilCancelMenu(void) {
 fossilWaitMenu(SYM_Task_MultichoiceMenu_HandleInput);press(2);fossilClose(1);
}
static void markCaught(unsigned count) {
 for(unsigned i=1;i<=count;i++) {
  unsigned dex=CALL(SpeciesToNationalPokedexNum,CALL(CustomPokedexNumToSpecies,i));
  CALL(GetSetPokedexFlag,dex,ABI_FLAG_SET_SEEN);CALL(GetSetPokedexFlag,dex,ABI_FLAG_SET_CAUGHT);
 }
 CHECK(CALL(GetCustomPokedexCount,ABI_FLAG_GET_CAUGHT)==count);
}
static void fillFossilPocket(void) {
 for(unsigned i=1;i<ITEMS_COUNT && CALL(CheckBagHasSpace,ITEM_CLAW_FOSSIL,1);i++)
  if(i!=ITEM_CLAW_FOSSIL && i!=ITEM_HELIX_FOSSIL && i!=ITEM_DOME_FOSSIL && CALL(ItemId_GetPocket,i)==CALL(ItemId_GetPocket,ITEM_CLAW_FOSSIL))CHECK(CALL(AddBagItem,i,1));
 CHECK(!CALL(CheckBagHasSpace,ITEM_CLAW_FOSSIL,1));
 CHECK(!CALL(CheckBagHasSpace,ITEM_HELIX_FOSSIL,1));
}
static void aideFossilTests(void) {
 fossilFixture(1);markCaught(19);
 fossilStart(SYM_Route10_PokemonCenter_1F_EventScript_Aide);fossilClose(1);
 CHECK(!CALL(BagGetQuantityByItemId,ITEM_CLAW_FOSSIL));CHECK(!CALL(FlagGet,FLAG_GOT_CLAW_FOSSIL_FROM_OAKS_AIDE));
 markCaught(20);fillFossilPocket();
 fossilStart(SYM_Route10_PokemonCenter_1F_EventScript_Aide);fossilClose(1);
 CHECK(!CALL(FlagGet,FLAG_GOT_CLAW_FOSSIL_FROM_OAKS_AIDE));
 CALL(ClearBag);fossilStart(SYM_Route10_PokemonCenter_1F_EventScript_Aide);fossilClose(1);
 CHECK(CALL(BagGetQuantityByItemId,ITEM_CLAW_FOSSIL)==1);CHECK(CALL(FlagGet,FLAG_GOT_CLAW_FOSSIL_FROM_OAKS_AIDE));
 CHECK(!CALL(BagGetQuantityByItemId,ITEM_EVERSTONE));
 flashRoundTrip();fossilStart(SYM_Route10_PokemonCenter_1F_EventScript_Aide);fossilClose(1);
 CHECK(CALL(BagGetQuantityByItemId,ITEM_CLAW_FOSSIL)==1);
 fossilFixture(1);markCaught(20);CALL(FlagSet,FLAG_GOT_EVERSTONE_FROM_OAKS_AIDE);
 fossilStart(SYM_Route10_PokemonCenter_1F_EventScript_Aide);fossilClose(1);
 CHECK(!CALL(BagGetQuantityByItemId,ITEM_CLAW_FOSSIL));
 printf("PASS: actual aide scripts reject19/full bag, award one Claw at20, preserve saved one-time bit and old-save claimed reward, never award Everstone\n");
}
static void unchosenFossilTests(void) {
 fossilFixture(0);fossilStart(SYM_CinnabarIsland_PokemonLab_ExperimentRoom_EventScript_FossilScientist);fossilCancelMenu();
 CHECK(!CALL(FlagGet,FLAG_GOT_UNCHOSEN_MT_MOON_FOSSIL));
 for(unsigned helix=0;helix<2;helix++) {
  fossilFixture(0);CALL(FlagSet,FLAG_GOT_FOSSIL_FROM_MT_MOON);CALL(FlagSet,helix?FLAG_GOT_HELIX_FOSSIL:FLAG_GOT_DOME_FOSSIL);
  unsigned gift=helix?ITEM_DOME_FOSSIL:ITEM_HELIX_FOSSIL,original=helix?ITEM_HELIX_FOSSIL:ITEM_DOME_FOSSIL;
  fossilStart(SYM_CinnabarIsland_PokemonLab_ExperimentRoom_EventScript_FossilScientist);
  fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);press(2);fossilClose(1);
  CHECK(!CALL(FlagGet,FLAG_GOT_UNCHOSEN_MT_MOON_FOSSIL));CHECK(!CALL(BagGetQuantityByItemId,gift));
  fillFossilPocket();fossilStart(SYM_CinnabarIsland_PokemonLab_ExperimentRoom_EventScript_FossilScientist);fossilClose(1);
  CHECK(!CALL(FlagGet,FLAG_GOT_UNCHOSEN_MT_MOON_FOSSIL));CALL(ClearBag);
  flashRoundTrip();fossilStart(SYM_CinnabarIsland_PokemonLab_ExperimentRoom_EventScript_FossilScientist);fossilClose(1);
  CHECK(CALL(BagGetQuantityByItemId,gift)==1);CHECK(!CALL(BagGetQuantityByItemId,original));
  CHECK(CALL(FlagGet,FLAG_GOT_UNCHOSEN_MT_MOON_FOSSIL));
  CHECK(CALL(FlagGet,FLAG_GOT_HELIX_FOSSIL)==helix);CHECK(CALL(FlagGet,FLAG_GOT_DOME_FOSSIL)==!helix);
  flashRoundTrip();fossilStart(SYM_CinnabarIsland_PokemonLab_ExperimentRoom_EventScript_FossilScientist);fossilCancelMenu();
  CHECK(CALL(BagGetQuantityByItemId,gift)==1);CHECK(!CALL(BagGetQuantityByItemId,original));
 }
 // Inconsistent imported choice flags do not guess a fossil or publish reward.
 fossilFixture(0);CALL(FlagSet,FLAG_GOT_FOSSIL_FROM_MT_MOON);CALL(FlagSet,FLAG_GOT_HELIX_FOSSIL);CALL(FlagSet,FLAG_GOT_DOME_FOSSIL);
 fossilStart(SYM_CinnabarIsland_PokemonLab_ExperimentRoom_EventScript_FossilScientist);fossilCancelMenu();
 CHECK(!CALL(FlagGet,FLAG_GOT_UNCHOSEN_MT_MOON_FOSSIL));
 printf("PASS: actual scientist gives only opposite MtMoon choice; no pre-choice/repeated/full-bag/refused gift, original choice flags immutable, flash persistence and corrupt-choice fail-closed\n");
}
static void fossilChoose(unsigned index) {
 fossilWaitMenu(SYM_Task_MultichoiceMenu_HandleInput);
 for(unsigned i=0;i<index;i++)press(128);
 CHECK(CALL(Menu_GetCursorPos)==index);press(1);
}
static void revivalTests(void) {
 const unsigned items[]={ITEM_HELIX_FOSSIL,ITEM_DOME_FOSSIL,ITEM_OLD_AMBER,ITEM_CLAW_FOSSIL};
 const unsigned species[]={SPECIES_OMANYTE,SPECIES_KABUTO,SPECIES_AERODACTYL,SPECIES_ANORITH};
 const unsigned flags[]={FLAG_REVIVED_HELIX,FLAG_REVIVED_DOME,FLAG_REVIVED_AMBER,FLAG_REVIVED_CLAW};
 fossilFixture(0);CALL(FlagSet,FLAG_GOT_UNCHOSEN_MT_MOON_FOSSIL);
 for(unsigned slot=1;slot<6;slot++)clearBytes(player(slot),ABI_MON_SIZE);
 core->busWrite8(core,SYM_gPlayerPartyCount,1);
 for(unsigned i=0;i<4;i++) {
  fossilStart(SYM_CinnabarIsland_PokemonLab_ExperimentRoom_EventScript_FossilScientist);fossilChoose(i);fossilClose(1);
  CHECK(CALL(VarGet,reviveState)==0);CHECK(!CALL(FlagGet,flags[i]));
  CHECK(CALL(AddBagItem,items[i],1));
  fossilStart(SYM_CinnabarIsland_PokemonLab_ExperimentRoom_EventScript_FossilScientist);fossilChoose(i);
  fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);press(2);fossilClose(1);
  CHECK(CALL(BagGetQuantityByItemId,items[i])==1);CHECK(CALL(VarGet,reviveState)==0);
  fossilStart(SYM_CinnabarIsland_PokemonLab_ExperimentRoom_EventScript_FossilScientist);fossilChoose(i);fossilClose(1);
  CHECK(!CALL(BagGetQuantityByItemId,items[i]));CHECK(CALL(VarGet,reviveState)==1);CHECK(CALL(VarGet,fossilKind)==i+1);
  CHECK(!CALL(FlagGet,flags[i]));flashRoundTrip();
  // Still-working message preserves state and does not consume another item.
  fossilStart(SYM_CinnabarIsland_PokemonLab_ExperimentRoom_EventScript_FossilScientist);fossilClose(1);CHECK(CALL(VarGet,reviveState)==1);
  fossilWarp(MAP_GROUP(MAP_CINNABAR_ISLAND_POKEMON_LAB_ENTRANCE),MAP_NUM(MAP_CINNABAR_ISLAND_POKEMON_LAB_ENTRANCE),7,6);
  CHECK(CALL(VarGet,reviveState)==2);flashRoundTrip();
  fossilWarp(MAP_GROUP(MAP_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM),MAP_NUM(MAP_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM),11,5);
  fossilStart(SYM_CinnabarIsland_PokemonLab_ExperimentRoom_EventScript_FossilScientist);
  fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);press(2);fossilClose(1);
  CHECK(CALL(IsSpeciesInCustomPokedex,species[i]));CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==i+2);CHECK(monget(player(i+1),ABI_DATA_SPECIES)==species[i]);CHECK(monget(player(i+1),ABI_DATA_LEVEL)==5);
  CHECK(CALL(FlagGet,flags[i]));CHECK(CALL(VarGet,reviveState)==0);
  CHECK(CALL(GetSetPokedexFlag,CALL(SpeciesToNationalPokedexNum,species[i]),ABI_FLAG_GET_CAUGHT));
  flashRoundTrip();CHECK(monget(player(i+1),ABI_DATA_SPECIES)==species[i]);
  CHECK(CALL(AddBagItem,items[i],1));fossilStart(SYM_CinnabarIsland_PokemonLab_ExperimentRoom_EventScript_FossilScientist);fossilChoose(i);fossilClose(1);
  CHECK(CALL(BagGetQuantityByItemId,items[i])==1);CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==i+2);
 }
 printf("PASS: all four fossils in ONE SAVE: actual menus cancel/refuse safely, consume once, preserve pending save, become ready on original entrance transition, award registered level5 species once and survive flash\n");
}
static void fossilTests(void) {aideFossilTests();unchosenFossilTests();revivalTests();}
