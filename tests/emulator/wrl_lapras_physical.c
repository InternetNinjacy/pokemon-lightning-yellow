// WRL-009: physically talk to the existing Silph Co. 7F Lapras employee.
// Staged map-local interaction, not natural Silph infiltration or story access.
// Existing compiled-script tests separately cover full-storage retry paths.
static void wrlLaprasPhysicalTests(void) {
    const unsigned group = MAP_GROUP(MAP_SILPH_CO_7F);
    const unsigned map = MAP_NUM(MAP_SILPH_CO_7F);
    for (unsigned pc = 0; pc < 2; pc++) {
        vendorFixture();
        // Authored worker is at (0,7); stand east of him, not on his tile.
        fossilWarp(group, map, 1, 7);
        if (pc) {
            vendorFillCapacity();
            CALL(ZeroBoxMonAt, 0, 0);
        }
        CHECK(!CALL(FlagGet, FLAG_GOT_LAPRAS_FROM_SILPH));
        unsigned worker = CALL(GetObjectEventIdByLocalIdAndMap, 2, map, group);
        CHECK(worker < ABI_OBJECT_COUNT);
        CHECK(CALL(GetObjectEventIdByXY, 7, 14) == worker);
        CALL(PlayerGetDestCoords, scratch, scratch + 2);
        CHECK(core->busRead16(core, scratch) == 8);
        CHECK(core->busRead16(core, scratch + 2) == 14);
        core->busWrite8(core, SYM_gSelectedObjectEvent, 0xFF);
        frames(6, 32); // Real LEFT D-pad, face the worker.
        frames(6, 0);
        press(1);      // Real A-button, never direct script invocation.
        fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
        CHECK(core->busRead8(core, SYM_gSelectedObjectEvent) == worker);
        press(2);      // Decline the nickname through the original Yes/No UI.
        fossilClose(1);
        CHECK(CALL(FlagGet, FLAG_GOT_LAPRAS_FROM_SILPH));
        unsigned mon = pc ? enemy(0) : player(3);
        if (pc) CALL(BoxMonToMon, CALL(GetBoxedMonPtr, 0, 0), mon);
        CHECK(monget(mon, ABI_DATA_SPECIES) == SPECIES_LAPRAS);
        CHECK(monget(mon, ABI_DATA_LEVEL) == 25);
        unsigned personality = monget(mon, ABI_DATA_PERSONALITY);
        unsigned count = core->busRead8(core, SYM_gPlayerPartyCount);
        CHECK(count == (pc ? 6 : 4));
        flashRoundTrip();
        if (pc) CALL(BoxMonToMon, CALL(GetBoxedMonPtr, 0, 0), mon);
        CHECK(monget(mon, ABI_DATA_PERSONALITY) == personality);
        CHECK(CALL(FlagGet, FLAG_GOT_LAPRAS_FROM_SILPH));
        // Flash restores save data, not live field state: re-enter the real map.
        fossilWarp(group, map, 1, 7);
        worker = CALL(GetObjectEventIdByLocalIdAndMap, 2, map, group);
        CHECK(worker < ABI_OBJECT_COUNT);
        core->busWrite8(core, SYM_gSelectedObjectEvent, 0xFF);
        frames(6, 32);
        frames(6, 0);
        press(1);
        CHECK(core->busRead8(core, SYM_gSelectedObjectEvent) == worker);
        fossilClose(1);
        CHECK(CALL(FlagGet, FLAG_GOT_LAPRAS_FROM_SILPH));
        CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == count);
        if (pc) CALL(BoxMonToMon, CALL(GetBoxedMonPtr, 0, 0), mon);
        CHECK(monget(mon, ABI_DATA_PERSONALITY) == personality);
        printf("PASS: WRL Silph Lapras physical LEFT/A employee talk, %s delivery, flash and one-time repeat\\n",
               pc ? "PC" : "party");
    }
    CHECK(core->loadState(core, bootState));
}
