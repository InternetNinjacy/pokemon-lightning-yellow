// Actual-ROM mGBA tests for issue #44's foreign-OT rescue identity and returns.
// Included after mystery_vendor.c by runner.c to reuse its established fixtures.
static void rocketFixture(unsigned quest)
{
    vendorFixture();
    // Mystery Vendor fixture starts with three Pikachu. Keep exactly one
    // baseline party member so the issued rescue is deterministically slot 1.
    for (unsigned slot = 1; slot < 6; slot++)
        clearBytes(player(slot), ABI_MON_SIZE);
    core->busWrite8(core, SYM_gPlayerPartyCount, 1);
    for (unsigned flag = 0x316; flag <= 0x31B; flag++)
        CALL(FlagClear, flag);
    core->busWrite16(core, SYM_gSpecialVar_0x8009, quest);
}

static void rocketHideoutNativeTests(void)
{
    // Two independent pickups, foreign OT mechanics, party return, exact rewards.
    for (unsigned q = 0; q < 2; q++)
    {
        rocketFixture(q);
        unsigned species = q ? SPECIES_SNORUNT : SPECIES_RALTS;
        unsigned item = q ? ITEM_NEVER_MELT_ICE : ITEM_RARE_CANDY;
        CHECK(CALL(RocketRescueStatus) == 0);
        CHECK(CALL(RocketRescueGive) == 0);
        CHECK(CALL(RocketRescueStatus) == 2);
        CHECK(monget(player(1), ABI_DATA_SPECIES) == species);
        CHECK(monget(player(1), ABI_DATA_LEVEL) == 27);
        CHECK(CALL(IsTradedMon, player(1)));
        CHECK(CALL(BagGetQuantityByItemId, item) == 0);
        CHECK(CALL(RocketRescueReturn) == 0);
        CHECK(CALL(BagGetQuantityByItemId, item) == 1);
        CHECK(CALL(RocketRescueStatus) == 3);
        CHECK(CALL(RocketRescueReturn) == 1);
        CHECK(CALL(BagGetQuantityByItemId, item) == 1);
        CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == 1);
    }
    printf("PASS: native Ralts/Snorunt party gifts, traded OT, exact returns, and single rewards\n");

    rocketFixture(0);
    vendorFillCapacity();
    CHECK(CALL(RocketRescueStatus) == 0);
    CHECK(CALL(RocketRescueGive) == 2);
    CHECK(CALL(FlagGet, 0x316) == 0);
    CALL(ZeroBoxMonAt, 3, 7);
    CHECK(CALL(RocketRescueGive) == 1);
    CHECK(CALL(RocketRescueStatus) == 2);
    CHECK(core->busRead16(core, SYM_gSpecialVar_MonBoxId) == 3);
    CHECK(core->busRead16(core, SYM_gSpecialVar_MonBoxPos) == 7);
    CHECK(CALL(GetBoxMonDataAt, 3, 7, ABI_DATA_SPECIES) == SPECIES_RALTS);
    CHECK(CALL(GetBoxMonDataAt, 3, 7, ABI_DATA_OT_ID) == 0x7AC10459);
    CHECK(CALL(RocketRescueReturn) == 0);
    CHECK(CALL(GetBoxMonDataAt, 3, 7, ABI_DATA_SPECIES) == SPECIES_NONE);
    CHECK(CALL(BagGetQuantityByItemId, ITEM_RARE_CANDY) == 1);
    CHECK(CALL(RocketRescueReturn) == 1);
    printf("PASS: full party and PC block gift; free one box; deliver, find, remove, and reward exactly once\n");

    // Same-species substitute cannot complete the quest once the actual one is gone.
    rocketFixture(0);
    CHECK(CALL(RocketRescueGive) == 0);
    unsigned originalPid = monget(player(1), ABI_DATA_PERSONALITY);
    clearBytes(player(1), ABI_MON_SIZE);
    core->busWrite8(core, SYM_gPlayerPartyCount, 1);
    CHECK(CALL(RocketRescueStatus) == 1);
    CALL(CreateMon, player(1), SPECIES_RALTS, 27, 31, 1, originalPid ^ 1, 0, 0);
    core->busWrite8(core, SYM_gPlayerPartyCount, 2);
    CHECK(CALL(RocketRescueStatus) == 1);
    CHECK(CALL(RocketRescueReturn) == 1);
    printf("PASS: missing actual gift and same-species replacement rejected\n");

    // Actual individual remains eligible when traded away and back, after a
    // species evolution-field transition, independent of current level.
    rocketFixture(0);
    CHECK(CALL(RocketRescueGive) == 0);
    unsigned char saved[ABI_MON_SIZE];
    readBytes(player(1), saved, sizeof(saved));
    clearBytes(player(1), ABI_MON_SIZE);
    core->busWrite8(core, SYM_gPlayerPartyCount, 1);
    CHECK(CALL(RocketRescueStatus) == 1);
    for (unsigned i = 0; i < sizeof(saved); i++)
        core->busWrite8(core, player(1) + i, saved[i]);
    core->busWrite8(core, SYM_gPlayerPartyCount, 2);
    CHECK(CALL(RocketRescueStatus) == 2);
    core->busWrite16(core, scratch, SPECIES_KIRLIA);
    CALL(SetMonData, player(1), ABI_DATA_SPECIES, scratch);
    CHECK(CALL(RocketRescueStatus) == 2);
    CHECK(CALL(RocketRescueReturn) == 0);
    printf("PASS: native saved identity remains valid through absence/return and evolution-field mutation\n");

    // Never remove the player's only Pokémon.
    rocketFixture(1);
    CHECK(CALL(RocketRescueGive) == 0);
    copyBytes(player(0), player(1), ABI_MON_SIZE);
    clearBytes(player(1), ABI_MON_SIZE);
    core->busWrite8(core, SYM_gPlayerPartyCount, 1);
    CHECK(CALL(RocketRescueStatus) == 2);
    CHECK(CALL(RocketRescueReturn) == 3);
    CHECK(CALL(BagGetQuantityByItemId, ITEM_NEVER_MELT_ICE) == 0);
    CHECK(CALL(FlagGet, 0x319) == 0);
    printf("PASS: only-party-Pokémon return rejected without item or flag commit\n");
}
