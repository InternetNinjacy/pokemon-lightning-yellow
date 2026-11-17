// WRL-009: physically talk to the existing wandering Cinnabar Lab scientist.
// The Mt. Moon choice and map are staged; this does NOT prove Surf/Cinnabar
// story access. Existing fossils.c covers compiled revival transactions.
static unsigned wrlScientistPhysicalTalk(unsigned group, unsigned map) {
    unsigned scientist = CALL(GetObjectEventIdByLocalIdAndMap, 2, map, group);
    CHECK(scientist < ABI_OBJECT_COUNT);
    CALL(PlayerGetDestCoords, scratch, scratch + 2);
    CHECK(core->busRead16(core, scratch) == 19); // authored (12,4) +7
    CHECK(core->busRead16(core, scratch + 2) == 11);
    // The unmodified scientist wanders. Wait until he is directly north;
    // never teleport him, change his movement, or preselect his script.
    unsigned stable = 0, wait;
    for (wait = 0; wait < 6000 && stable < 24; wait++) {
        if (CALL(GetObjectEventIdByXY, 19, 10) == scientist)
            stable++;
        else
            stable = 0;
        frames(1, 0);
    }
    if (stable != 24)
        fprintf(stderr, "WRL Cinnabar scientist did not return to (12,3): object=%u waited=%u\\n",
                scientist, wait);
    CHECK(stable == 24);
    core->busWrite8(core, SYM_gSelectedObjectEvent, 0xFF);
    frames(6, 64); // Actual D-pad UP.
    frames(6, 0);
    press(1); // Actual A, never ScriptContext_SetupScript.
    return scientist;
}

static void wrlScientistPhysicalTests(void) {
    const unsigned group = MAP_GROUP(MAP_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM);
    const unsigned map = MAP_NUM(MAP_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM);
    for (unsigned helix = 0; helix < 2; helix++) {
        vendorFixture();
        fossilWarp(group, map, 12, 4);
        CALL(ClearBag);
        CALL(FlagSet, FLAG_GOT_FOSSIL_FROM_MT_MOON);
        CALL(FlagSet, helix ? FLAG_GOT_HELIX_FOSSIL : FLAG_GOT_DOME_FOSSIL);
        const unsigned gift = helix ? ITEM_DOME_FOSSIL : ITEM_HELIX_FOSSIL;
        const unsigned original = helix ? ITEM_HELIX_FOSSIL : ITEM_DOME_FOSSIL;
        CHECK(!CALL(FlagGet, FLAG_GOT_UNCHOSEN_MT_MOON_FOSSIL));

        unsigned scientist = wrlScientistPhysicalTalk(group, map);
        fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
        CHECK(core->busRead8(core, SYM_gSelectedObjectEvent) == scientist);
        press(2); // Decline the real Yes/No gift dialogue.
        // Verify the B decision before any text-dismissal input: an A during
        // close can also accept a re-opened menu on a wandering NPC.
        CHECK(!CALL(BagGetQuantityByItemId, gift));
        CHECK(!CALL(FlagGet, FLAG_GOT_UNCHOSEN_MT_MOON_FOSSIL));
        fossilClose(2); // Keep B through the decline/close path, never A.
        CHECK(!CALL(BagGetQuantityByItemId, gift));
        CHECK(!CALL(FlagGet, FLAG_GOT_UNCHOSEN_MT_MOON_FOSSIL));

        scientist = wrlScientistPhysicalTalk(group, map);
        fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
        CHECK(core->busRead8(core, SYM_gSelectedObjectEvent) == scientist);
        press(1); // Accept the opposite fossil through the original menu.
        fossilClose(1);
        CHECK(CALL(BagGetQuantityByItemId, gift) == 1);
        CHECK(!CALL(BagGetQuantityByItemId, original));
        CHECK(CALL(FlagGet, FLAG_GOT_UNCHOSEN_MT_MOON_FOSSIL));
        CHECK(CALL(FlagGet, FLAG_GOT_HELIX_FOSSIL) == helix);
        CHECK(CALL(FlagGet, FLAG_GOT_DOME_FOSSIL) == !helix);

        flashRoundTrip();
        CHECK(CALL(BagGetQuantityByItemId, gift) == 1);
        CHECK(CALL(FlagGet, FLAG_GOT_UNCHOSEN_MT_MOON_FOSSIL));
        fossilWarp(group, map, 12, 4);
        scientist = wrlScientistPhysicalTalk(group, map);
        fossilWaitMenu(SYM_Task_MultichoiceMenu_HandleInput);
        CHECK(core->busRead8(core, SYM_gSelectedObjectEvent) == scientist);
        press(2); // Existing revival service, not another fossil gift.
        fossilClose(1);
        CHECK(CALL(BagGetQuantityByItemId, gift) == 1);
        CHECK(!CALL(BagGetQuantityByItemId, original));
        CHECK(CALL(FlagGet, FLAG_GOT_UNCHOSEN_MT_MOON_FOSSIL));
        printf("PASS: WRL Cinnabar wandering scientist physical UP/A, opposite %s gift, decline, flash and no duplicate\\n",
               helix ? "Dome" : "Helix");
    }
    CHECK(core->loadState(core, bootState));
}
