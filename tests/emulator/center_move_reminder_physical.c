// Issue #191: staged physical D-pad / A / B interaction, no direct script call.
// Representative Kanto, Sevii and Indigo locations; not natural travel proof.
// The shared legacy relearner engine is additionally checked by existing suites.
static void centerMoveReminderPhysicalTests(void) {
    static const struct {
        unsigned group, map, x, y;
    } sites[] = {
        { MAP_GROUP(MAP_ROUTE10_POKEMON_CENTER_1F), MAP_NUM(MAP_ROUTE10_POKEMON_CENTER_1F), 10, 4 },
        { MAP_GROUP(MAP_TWO_ISLAND_POKEMON_CENTER_1F), MAP_NUM(MAP_TWO_ISLAND_POKEMON_CENTER_1F), 10, 4 },
        { MAP_GROUP(MAP_INDIGO_PLATEAU_POKEMON_CENTER_1F), MAP_NUM(MAP_INDIGO_PLATEAU_POKEMON_CENTER_1F), 18, 13 },
    };
    for (unsigned i = 0; i < sizeof(sites) / sizeof(sites[0]); i++) {
        const unsigned group = sites[i].group, map = sites[i].map;
        const unsigned x = sites[i].x, y = sites[i].y;
        vendorFixture();
        fossilWarp(group, map, x, y + 1);
        CALL(PlayerGetDestCoords, scratch, scratch + 2);
        CHECK(core->busRead16(core, scratch) == x + 7);
        CHECK(core->busRead16(core, scratch + 2) == y + 8);
        unsigned reminder = CALL(GetObjectEventIdByXY, x + 7, y + 7);
        CHECK(reminder < ABI_OBJECT_COUNT);
        CALL(SetMoney, careSave1() + ABI_SAVE1_MONEY, 70000);
        unsigned before = CALL(GetMoney, careSave1() + ABI_SAVE1_MONEY);
        CHECK(before == 70000);
        unsigned count = core->busRead8(core, SYM_gPlayerPartyCount);
        core->busWrite8(core, SYM_gSelectedObjectEvent, 0xFF);
        frames(6, 64); // Actual UP; object should block walking.
        frames(6, 0);
        press(1);      // Actual A-button; let engine select NPC.
        fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
        CHECK(core->busRead8(core, SYM_gSelectedObjectEvent) == reminder);
        press(2);      // Real B: decline the free service.
        fossilClose(2);
        CHECK(CALL(GetMoney, careSave1() + ABI_SAVE1_MONEY) == before);
        CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == count);
        printf("PASS: Center reminder physical UP/A/B, no fee, site=%u\n", i + 1);
    }
    CHECK(core->loadState(core, bootState));
}
