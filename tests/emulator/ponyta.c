// Real target lookup, cancel/success scenes and flash data; no evolution simulator.
static void ponytaTests(void) {
 CHECK(core->loadState(core,bootState));initSave();CALL(DisableNationalPokedex);create(SPECIES_PONYTA,29);
 CHECK(target(EVO_MODE_NORMAL,0)==SPECIES_NONE);put(ABI_DATA_LEVEL,30);
 CHECK(target(EVO_MODE_NORMAL,0)==SPECIES_RAPIDASH);put(ABI_DATA_HELD_ITEM,ITEM_EVERSTONE);
 CHECK(target(EVO_MODE_NORMAL,0)==SPECIES_NONE);put(ABI_DATA_HELD_ITEM,ITEM_NONE);
 levelScene(SPECIES_PONYTA,30,SPECIES_RAPIDASH,0,2,100,100);
 levelScene(SPECIES_PONYTA,30,SPECIES_RAPIDASH,0,0,100,100);
 printf("PASS: native Ponyta29/30 threshold, Everstone, animation B cancellation and Rapidash evolution/Dex/flash before National Dex\n");
}
