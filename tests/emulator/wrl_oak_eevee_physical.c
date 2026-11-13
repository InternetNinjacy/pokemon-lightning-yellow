// WRL-009: actual D-pad/A interaction with Oak's authored Lab NPC to retry
// the existing parcel Eevee. The pending flag and Lab location are staged:
// this is NOT proof of uninterrupted early-game story progression.
static void wrlOakEeveePhysicalTests(void) {
    const unsigned group = MAP_GROUP(MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB);
    const unsigned map = MAP_NUM(MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB);
    for (unsigned pc = 0; pc < 2; pc++) {
        vendorFixture();
        // Oak is the fourth authored object at (6,3). Do not move or inject him.
        CALL(FlagClear, FLAG_HIDE_OAK_IN_HIS_LAB);
        fossilWarp(group, map, 6, 4); // One tile south of Oak.
        if (pc) {
            vendorFillCapacity();
            CALL(ZeroBoxMonAt, 0, 0);
        }
        CALL(FlagSet, FLAG_OAK_EEVEE_PENDING);
        CHECK(!CALL(FlagGet, FLAG_GOT_OAK_PARCEL_EEVEE));
        unsigned oak = CALL(GetObjectEventIdByLocalIdAndMap, 4, map, group);
        CHECK(oak < ABI_OBJECT_COUNT);
        CHECK(CALL(GetObjectEventIdByXY, 13, 10) == oak);
        CALL(PlayerGetDestCoords, scratch, scratch + 2);
        CHECK(core->busRead16(core, scratch) == 13);
        CHECK(core->busRead16(core, scratch + 2) == 11);
        core->busWrite8(core, SYM_gSelectedObjectEvent, 0xFF);
        frames(6, 64); // UP: face the real Oak object.
        frames(6, 0);
        press(1); // A: enter Oak's actual map script, not direct script setup.
        CHECK(core->busRead8(core, SYM_gSelectedObjectEvent) == oak);
        fossilClose(1);
        CHECK(CALL(FlagGet, FLAG_GOT_OAK_PARCEL_EEVEE));
        CHECK(!CALL(FlagGet, FLAG_OAK_EEVEE_PENDING));
        unsigned mon = pc ? enemy(0) : player(3);
        if (pc) CALL(BoxMonToMon, CALL(GetBoxedMonPtr, 0, 0), mon);
        CHECK(monget(mon, ABI_DATA_SPECIES) == SPECIES_EEVEE);
        unsigned personality = monget(mon, ABI_DATA_PERSONALITY);
        unsigned count = core->busRead8(core, SYM_gPlayerPartyCount);
        CHECK(count == (pc ? 6 : 4));
        flashRoundTrip();
        CHECK(CALL(FlagGet, FLAG_GOT_OAK_PARCEL_EEVEE));
        CHECK(!CALL(FlagGet, FLAG_OAK_EEVEE_PENDING));
        if (pc) CALL(BoxMonToMon, CALL(GetBoxedMonPtr, 0, 0), mon);
        CHECK(monget(mon, ABI_DATA_SPECIES) == SPECIES_EEVEE);
        CHECK(monget(mon, ABI_DATA_PERSONALITY) == personality);
        // Flash reloads persistent save data, not the live map and avatar.
        fossilWarp(group, map, 6, 4);
        oak = CALL(GetObjectEventIdByLocalIdAndMap, 4, map, group);
        CHECK(oak < ABI_OBJECT_COUNT);
        CHECK(CALL(GetObjectEventIdByXY, 13, 10) == oak);
        core->busWrite8(core, SYM_gSelectedObjectEvent, 0xFF);
        frames(6, 64);
        frames(6, 0);
        press(1);
        CHECK(core->busRead8(core, SYM_gSelectedObjectEvent) == oak);
        fossilClose(1); // Ordinary repeat talk must not create a second Eevee.
        CHECK(CALL(FlagGet, FLAG_GOT_OAK_PARCEL_EEVEE));
        CHECK(!CALL(FlagGet, FLAG_OAK_EEVEE_PENDING));
        CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == count);
        if (pc) CALL(BoxMonToMon, CALL(GetBoxedMonPtr, 0, 0), mon);
        CHECK(monget(mon, ABI_DATA_PERSONALITY) == personality);
        printf("PASS: WRL Oak physical UP/A parcel Eevee, %s delivery, flash and no duplicate\n",
               pc ? "PC" : "party");
    }
    CHECK(core->loadState(core, bootState));
}
