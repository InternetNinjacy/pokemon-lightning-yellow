// Native mGBA acceptance: approved Surge four-member Double Gym encounter.
static void surgeFixture(unsigned count) {
    daycareFixture(1);
    core->busWrite8(core, SYM_gPlayerPartyCount, count);
    for (unsigned i=count; i<6; i++) clearBytes(player(i), ABI_MON_SIZE);
    fossilWarp(MAP_GROUP(MAP_VERMILION_CITY_GYM), MAP_NUM(MAP_VERMILION_CITY_GYM), 5, 5);
    unsigned object = CALL(GetObjectEventIdByLocalIdAndMap,1,
        MAP_NUM(MAP_VERMILION_CITY_GYM), MAP_GROUP(MAP_VERMILION_CITY_GYM));
    CHECK(object < ABI_OBJECT_COUNT);
    core->busWrite8(core, SYM_gSelectedObjectEvent, object);
}
static void surgeTests(void) {
    surgeFixture(1);
    fossilStart(SYM_VermilionCity_Gym_EventScript_LtSurge);
    fossilClose(1);
    CHECK(!(core->busRead32(core, SYM_gBattleTypeFlags) & BATTLE_TYPE_TRAINER));
    CHECK(!CALL(FlagGet, FLAG_BADGE03_GET));
    CHECK(core->loadState(core, bootState));

    surgeFixture(2);
    fossilStart(SYM_VermilionCity_Gym_EventScript_LtSurge);
    waitAction();
    CHECK(core->busRead32(core, SYM_gBattleTypeFlags) & BATTLE_TYPE_TRAINER);
    CHECK(core->busRead32(core, SYM_gBattleTypeFlags) & BATTLE_TYPE_DOUBLE);
    CHECK(core->busRead8(core, SYM_gBattlersCount) == 4);
    const unsigned species[] = {SPECIES_RAICHU, SPECIES_VOLTORB,
        SPECIES_POLIWAG, SPECIES_ABRA};
    const unsigned levels[] = {30,24,24,24};
    for (unsigned i=0;i<4;i++) {
        CHECK(monget(enemy(i), ABI_DATA_SPECIES)==species[i]);
        CHECK(monget(enemy(i), ABI_DATA_LEVEL)==levels[i]);
    }
    CHECK(core->busRead16(core,bmon(1)+ABI_BMON_SPECIES)==SPECIES_RAICHU);
    CHECK(core->busRead16(core,bmon(3)+ABI_BMON_SPECIES)==SPECIES_VOLTORB);
    CHECK(monget(enemy(0), ABI_DATA_MOVE1)==MOVE_ZAP_CANNON);
    CHECK(monget(enemy(1), ABI_DATA_MOVE1)==MOVE_THUNDER_WAVE);
    CHECK(monget(enemy(1), ABI_DATA_MOVE1+2)==MOVE_NONE);
    CHECK(monget(enemy(2), ABI_DATA_MOVE1)==MOVE_RAIN_DANCE);
    CHECK(monget(enemy(3), ABI_DATA_MOVE1)==MOVE_FAKE_TEARS);
    printf("PASS: native Surge NPC rejects single Pokemon, opens four-member Double with approved Raichu/Voltorb and reserve moves\n");
    CHECK(core->loadState(core, bootState));
}
