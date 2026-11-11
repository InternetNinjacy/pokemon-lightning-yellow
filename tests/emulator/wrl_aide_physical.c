// WRL-009: physical A-button interaction with Route 10's existing Claw Fossil aide.
// Staged map-local 19/20 caught fixtures; this is not natural story traversal.
// Never invoke the aide script directly or set the selected object to the aide.
static void wrlAidePhysicalTests(void) {
    const unsigned group = MAP_GROUP(MAP_ROUTE10_POKEMON_CENTER_1F);
    const unsigned map = MAP_NUM(MAP_ROUTE10_POKEMON_CENTER_1F);
    for (unsigned caught = 19; caught <= 20; caught++) {
        vendorFixture();
        fossilWarp(group, map, 12, 6); // one tile south of the authored aide
        CALL(ClearBag);
        markCaught(caught);
        unsigned aide = CALL(GetObjectEventIdByLocalIdAndMap, 5, map, group);
        CHECK(aide < ABI_OBJECT_COUNT);
        CHECK(CALL(GetObjectEventIdByXY, 12 + 7, 5 + 7) == aide);
        CALL(PlayerGetDestCoords, scratch, scratch + 2);
        CHECK(core->busRead16(core, scratch) == 12 + 7);
        CHECK(core->busRead16(core, scratch + 2) == 6 + 7);
        core->busWrite8(core, SYM_gSelectedObjectEvent, 0xFF);
        frames(6, 64); // physical UP, blocked by the actual aide object
        frames(6, 0);
        press(1);      // physical A, not ScriptContext_SetupScript
        fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
        CHECK(core->busRead8(core, SYM_gSelectedObjectEvent) == aide);
        CHECK(CALL(GetCustomPokedexCount, ABI_FLAG_GET_CAUGHT) == caught);
        press(1);      // accept the real aide's 20-caught check
        fossilClose(1);
        unsigned awarded = caught == 20;
        CHECK(CALL(BagGetQuantityByItemId, ITEM_CLAW_FOSSIL) == awarded);
        CHECK(!!CALL(FlagGet, FLAG_GOT_CLAW_FOSSIL_FROM_OAKS_AIDE) == awarded);
        if (awarded) {
            flashRoundTrip();
            CHECK(CALL(BagGetQuantityByItemId, ITEM_CLAW_FOSSIL) == 1);
            CHECK(CALL(FlagGet, FLAG_GOT_CLAW_FOSSIL_FROM_OAKS_AIDE));
            // Flash loading restores save blocks but not the live field/avatar
            // state. Re-enter the authored map before testing a second real
            // overworld conversation, as Continue would do.
            fossilWarp(group, map, 12, 6);
            aide = CALL(GetObjectEventIdByLocalIdAndMap, 5, map, group);
            CHECK(aide < ABI_OBJECT_COUNT);
            CHECK(CALL(GetObjectEventIdByXY, 12 + 7, 5 + 7) == aide);
            // Repeat through real A input: the one-time aide cannot award twice.
            core->busWrite8(core, SYM_gSelectedObjectEvent, 0xFF);
            frames(6, 64);
            frames(6, 0);
            press(1);
            fossilClose(1);
            CHECK(core->busRead8(core, SYM_gSelectedObjectEvent) == aide);
            CHECK(CALL(BagGetQuantityByItemId, ITEM_CLAW_FOSSIL) == 1);
        }
        printf("PASS: WRL Route10 aide physical UP/A with %u caught, Claw award=%u and one-time flag\n",
               caught, awarded);
    }
    CHECK(core->loadState(core, bootState));
}
