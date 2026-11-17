// WRL-009: real overworld D-pad/A purchase from the existing Game Corner
// Pokemon prize clerk. Staged coins/party/map, not natural coin acquisition.
// Existing compiled-script tests cover all-storage refusal and other prizes.
static void wrlPorygonPhysicalOpen(unsigned clerk) {
    // Never preselect the clerk or inject the original event script.
    core->busWrite8(core, SYM_gSelectedObjectEvent, 0xFF);
    frames(6, 64); // UP: face the authored counter worker.
    frames(6, 0);
    press(1);      // A: select the actual map NPC.
    fossilWaitMenu(SYM_Task_MultichoiceMenu_HandleInput);
    frames(10, 0);
    CHECK(core->busRead8(core, SYM_gSelectedObjectEvent) == clerk);
    for (unsigned i = 0; i < 4; i++) press(128); // Porygon is prize 5.
    CHECK(CALL(Menu_GetCursorPos) == 4);
    press(1);
    fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
    frames(10, 0);
}

static void wrlPorygonPhysicalTests(void) {
    const unsigned group = MAP_GROUP(MAP_CELADON_CITY_GAME_CORNER_PRIZE_ROOM);
    const unsigned map = MAP_NUM(MAP_CELADON_CITY_GAME_CORNER_PRIZE_ROOM);
    for (unsigned pc = 0; pc < 2; pc++) {
        vendorFixture();
        fossilWarp(group, map, 4, 3); // Clerk (4,2), player one tile south.
        CALL(FlagSet, FLAG_GOT_COIN_CASE);
        CALL(SetCoins, 9999); // Set after live map load, which can reload save.
        if (pc) {
            vendorFillCapacity();
            CALL(ZeroBoxMonAt, 0, 0);
        }
        unsigned clerk = CALL(GetObjectEventIdByLocalIdAndMap, 3, map, group);
        CHECK(clerk < ABI_OBJECT_COUNT);
        CHECK(CALL(GetObjectEventIdByXY, 11, 9) == clerk);
        CALL(PlayerGetDestCoords, scratch, scratch + 2);
        CHECK(core->busRead16(core, scratch) == 11);
        CHECK(core->busRead16(core, scratch + 2) == 10);
        CHECK(CALL(GetCoins) == 9999);
        wrlPorygonPhysicalOpen(clerk);
        CHECK(CALL(GetCoins) == 9999); // No premature charge.
        press(1); // Yes, exchange 9999 coins for Porygon.
        fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
        frames(10, 0);
        press(2); // Decline nickname, avoiding the editor.
        fossilClose(1);
        CHECK(CALL(GetCoins) == 0);
        unsigned mon = pc ? enemy(0) : player(3);
        if (pc) CALL(BoxMonToMon, CALL(GetBoxedMonPtr, 0, 0), mon);
        CHECK(monget(mon, ABI_DATA_SPECIES) == SPECIES_PORYGON);
        CHECK(monget(mon, ABI_DATA_LEVEL) == 26);
        unsigned personality = monget(mon, ABI_DATA_PERSONALITY);
        unsigned count = core->busRead8(core, SYM_gPlayerPartyCount);
        CHECK(count == (pc ? 6 : 4));
        flashRoundTrip();
        CHECK(CALL(GetCoins) == 0);
        if (pc) CALL(BoxMonToMon, CALL(GetBoxedMonPtr, 0, 0), mon);
        CHECK(monget(mon, ABI_DATA_PERSONALITY) == personality);
        // Flash round-trip restores save blocks but not live field state.
        fossilWarp(group, map, 4, 3);
        clerk = CALL(GetObjectEventIdByLocalIdAndMap, 3, map, group);
        CHECK(clerk < ABI_OBJECT_COUNT);
        wrlPorygonPhysicalOpen(clerk);
        press(1); // A second attempt with zero coins must be refused.
        fossilClose(1);
        CHECK(CALL(GetCoins) == 0);
        CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == count);
        if (pc) CALL(BoxMonToMon, CALL(GetBoxedMonPtr, 0, 0), mon);
        CHECK(monget(mon, ABI_DATA_PERSONALITY) == personality);
        printf("PASS: WRL Game Corner physical UP/A Porygon clerk, %s delivery, 9999 coins, flash and zero-coin refusal\n",
               pc ? "PC" : "party");
    }
    CHECK(core->loadState(core, bootState));
}
