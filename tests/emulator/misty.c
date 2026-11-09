// Native mGBA acceptance: approved Misty encounter and battler-only abilities.
static void mistyFixture(unsigned count) {
    daycareFixture(1);
    core->busWrite8(core, SYM_gPlayerPartyCount, count);
    for (unsigned i = count; i < 6; i++) clearBytes(player(i), ABI_MON_SIZE);
    fossilWarp(MAP_GROUP(MAP_CERULEAN_CITY_GYM), MAP_NUM(MAP_CERULEAN_CITY_GYM), 8, 9);
    unsigned object = CALL(GetObjectEventIdByLocalIdAndMap, 3,
        MAP_NUM(MAP_CERULEAN_CITY_GYM), MAP_GROUP(MAP_CERULEAN_CITY_GYM));
    CHECK(object < ABI_OBJECT_COUNT);
    core->busWrite8(core, SYM_gSelectedObjectEvent, object);
}
static void mistyTests(void) {
    mistyFixture(1);
    fossilStart(SYM_CeruleanCity_Gym_EventScript_Misty);
    fossilClose(1);
    CHECK(!(core->busRead32(core, SYM_gBattleTypeFlags) & BATTLE_TYPE_TRAINER));
    CHECK(!CALL(FlagGet, FLAG_BADGE02_GET));
    CHECK(core->loadState(core, bootState));

    mistyFixture(2);
    fossilStart(SYM_CeruleanCity_Gym_EventScript_Misty);
    waitAction();
    CHECK(core->busRead32(core, SYM_gBattleTypeFlags) & BATTLE_TYPE_TRAINER);
    CHECK(core->busRead32(core, SYM_gBattleTypeFlags) & BATTLE_TYPE_DOUBLE);
    CHECK(core->busRead8(core, SYM_gBattlersCount) == 4);
    const unsigned species[] = {SPECIES_STARYU, SPECIES_PSYDUCK,
        SPECIES_GOLDUCK, SPECIES_STARMIE};
    const unsigned level[] = {22, 22, 24, 25};
    for (unsigned i=0; i<4; i++) {
        CHECK(monget(enemy(i), ABI_DATA_SPECIES) == species[i]);
        CHECK(monget(enemy(i), ABI_DATA_LEVEL) == level[i]);
    }
    CHECK(core->busRead16(core, bmon(1) + ABI_BMON_SPECIES) == SPECIES_STARYU);
    CHECK(core->busRead16(core, bmon(3) + ABI_BMON_SPECIES) == SPECIES_PSYDUCK);
    CHECK(core->busRead8(core, bmon(1) + ABI_BMON_ABILITY) == ABILITY_NATURAL_CURE);
    CHECK(core->busRead8(core, bmon(3) + ABI_BMON_ABILITY) == ABILITY_DAMP);
    // Exercise the actual production ability getter for both reserve entries;
    // does not mutate species slots, ability tables, or permanent party data.
    unsigned oldSpecies=core->busRead16(core,bmon(1)+ABI_BMON_SPECIES);
    core->busWrite16(core,bmon(1)+ABI_BMON_SPECIES,SPECIES_GOLDUCK);
    CHECK(CALL(GetBattlerInitialAbility,1) == ABILITY_SYNCHRONIZE);
    core->busWrite16(core,bmon(1)+ABI_BMON_SPECIES,SPECIES_STARMIE);
    CHECK(CALL(GetBattlerInitialAbility,1) == ABILITY_DRIZZLE);
    core->busWrite16(core,bmon(1)+ABI_BMON_SPECIES,oldSpecies);
    printf("PASS: actual Misty Gym refusal with one Pokemon, four-Pokemon Double entry, first two battler abilities and reserve native ability getter\n");
    CHECK(core->loadState(core,bootState));
}
