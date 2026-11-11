// Issue #191 — native production-routine contract tests.
// These run in the actual ROM via mGBA, not a simulated learnset.
// Physical NPC entry/cancel is exercised separately in all 19 Center maps.
static unsigned centerHasMove(unsigned count, unsigned move) {
    for (unsigned i = 0; i < count; i++)
        if (core->busRead16(core, SYM_gStringVar4 + 2*i) == move)
            return 1;
    return 0;
}
static unsigned centerEligible(unsigned mon) {
    return CALL(GetMoveRelearnerMoves, mon, SYM_gStringVar4);
}
static void centerMoveReminderLearningTests(void) {
    vendorFixture();
    fossilWarp(MAP_GROUP(MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F),
               MAP_NUM(MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F), 10, 5);
    // A Pokémon at level 15 cannot remember a level-16 evolved-species move.
    CALL(CreateMon, player(0), SPECIES_VAPOREON, 15, 31, 1, 7, 0, 0);
    for (unsigned i = 0; i < 4; i++)
        monput(player(0), ABI_DATA_MOVE1 + i, MOVE_SPLASH);
    unsigned n = centerEligible(player(0));
    CHECK(n > 0);
    CHECK(centerHasMove(n, MOVE_TACKLE));
    CHECK(!centerHasMove(n, MOVE_WATER_GUN));
    CHECK(!centerHasMove(n, MOVE_GROWL)); // unevolved Eevee-only
    CHECK(CALL(GetNumberOfRelearnableMoves, player(0)) == n);

    // At level 16 Water Gun is eligible for Vaporeon, never Eevee's Growl.
    CALL(CreateMon, player(0), SPECIES_VAPOREON, 16, 31, 1, 7, 0, 0);
    for (unsigned i = 0; i < 4; i++)
        monput(player(0), ABI_DATA_MOVE1 + i, MOVE_SPLASH);
    n = centerEligible(player(0));
    CHECK(centerHasMove(n, MOVE_WATER_GUN));
    CHECK(!centerHasMove(n, MOVE_GROWL));
    // Confirm known moves are excluded and full four-move slots do not
    // automatically overwrite or consume any original move.
    monput(player(0), ABI_DATA_MOVE1, MOVE_WATER_GUN);
    n = centerEligible(player(0));
    CHECK(!centerHasMove(n, MOVE_WATER_GUN));
    CHECK(monget(player(0), ABI_DATA_MOVE1) == MOVE_WATER_GUN);
    CHECK(monget(player(0), ABI_DATA_MOVE2) == MOVE_SPLASH);

    // Espeon uses the same current-species rule, not Eevee's Growl.
    CALL(CreateMon, player(0), SPECIES_ESPEON, 16, 31, 1, 7, 0, 0);
    for (unsigned i = 0; i < 4; i++)
        monput(player(0), ABI_DATA_MOVE1 + i, MOVE_SPLASH);
    n = centerEligible(player(0));
    CHECK(centerHasMove(n, MOVE_CONFUSION));
    CHECK(!centerHasMove(n, MOVE_WATER_GUN));
    CHECK(!centerHasMove(n, MOVE_GROWL));

    // No available move when the Pokémon knows every eligible level-1 move.
    CALL(CreateMon, player(0), SPECIES_EEVEE, 1, 31, 1, 7, 0, 0);
    monput(player(0), ABI_DATA_MOVE1, MOVE_TACKLE);
    monput(player(0), ABI_DATA_MOVE2, MOVE_TAIL_WHIP);
    monput(player(0), ABI_DATA_MOVE3, MOVE_HELPING_HAND);
    monput(player(0), ABI_DATA_MOVE4, MOVE_NONE);
    CHECK(centerEligible(player(0)) == 0);
    CHECK(CALL(GetNumberOfRelearnableMoves, player(0)) == 0);

    // The production learn-move routine actually modifies a slot, and flash
    // persistence protects the new move and leaves the original balance.
    CALL(CreateMon, player(0), SPECIES_VAPOREON, 16, 31, 1, 7, 0, 0);
    for (unsigned i = 0; i < 4; i++)
        monput(player(0), ABI_DATA_MOVE1 + i, MOVE_NONE);
    unsigned money = CALL(GetMoney, careSave1() + ABI_SAVE1_MONEY);
    CHECK(CALL(GiveMoveToMon, player(0), MOVE_WATER_GUN) != 0xFFFF);
    CHECK(monget(player(0), ABI_DATA_MOVE1) == MOVE_WATER_GUN);
    CHECK(CALL(GetMoney, careSave1() + ABI_SAVE1_MONEY) == money);
    flashRoundTrip();
    CHECK(monget(player(0), ABI_DATA_MOVE1) == MOVE_WATER_GUN);
    CHECK(CALL(GetMoney, careSave1() + ABI_SAVE1_MONEY) == money);
    CHECK(!centerHasMove(centerEligible(player(0)), MOVE_WATER_GUN));
    printf("PASS: production move eligibility, evolved species, level16, full slots, known/no moves and learned-move flash persistence\n");

    // Complete real-NPC dialog -> party picker -> move picker -> Yes -> learned
    // transaction. D-pad/A/B are the only UI inputs; do not set script flags,
    // force an NPC script, or write a learned move during this transaction.
    vendorFixture();
    fossilWarp(MAP_GROUP(MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F),
               MAP_NUM(MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F), 10, 5);
    CALL(CreateMon, player(0), SPECIES_VAPOREON, 16, 31, 1, 7, 0, 0);
    monput(player(0), ABI_DATA_MOVE1, MOVE_TACKLE);
    monput(player(0), ABI_DATA_MOVE2, MOVE_TAIL_WHIP);
    monput(player(0), ABI_DATA_MOVE3, MOVE_HELPING_HAND);
    monput(player(0), ABI_DATA_MOVE4, MOVE_NONE);
    CHECK(centerEligible(player(0)) == 2);
    CHECK(centerHasMove(2, MOVE_WATER_GUN));
    unsigned originalMoney = CALL(GetMoney, careSave1() + ABI_SAVE1_MONEY);
    unsigned npc = CALL(GetObjectEventIdByXY, 10 + 7, 4 + 7);
    CHECK(npc < ABI_OBJECT_COUNT);
    core->busWrite8(core, SYM_gSelectedObjectEvent, 0xFF);
    frames(6, 64); frames(6, 0); press(1); // physical UP, A
    fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
    CHECK(core->busRead8(core, SYM_gSelectedObjectEvent) == npc);
    press(1); // accept free reminder
    unsigned f;
    for (f = 0; f < 6000; f++) {
        if (CALL(FuncIsActiveTask, SYM_Task_HandleChooseMonInput | 1U)) break;
        frames(1, (f % 12) == 0 && fossilTextPaused() ? 1 : 0);
    }
    CHECK(f < 6000);
    frames(16, 0); press(1); // select party member 0 through the real party menu
    for (f = 0; f < 6000; f++) {
        if ((core->busRead32(core, SYM_gMain + ABI_MAIN_CB2) & ~1U)
            == SYM_CB2_MoveRelearner) break;
        frames(1, (f % 12) == 0 && fossilTextPaused() ? 1 : 0);
    }
    CHECK(f < 6000);
    for (f = 0; f < 6000; f++) {
        unsigned p = core->busRead32(core, SYM_sMoveRelearner);
        if (p && core->busRead8(core, p) == 4) break; // list ready
        frames(1, 0);
    }
    CHECK(f < 6000);
    unsigned ui = core->busRead32(core, SYM_sMoveRelearner);
    CHECK(core->busRead8(core, ui + ABI_RELEARNER_INDEX) == 0);
    press(128); frames(16, 0); // DOWN to Water Gun
    CHECK(core->busRead8(core, ui + ABI_RELEARNER_INDEX) == 1);
    press(1); // select Water Gun
    for (f = 0; f < 6000; f++) {
        if (core->busRead8(core, ui) == 9) break; // confirmation Yes/No
        frames(1, 0);
    }
    CHECK(f < 6000); frames(16, 0);
    press(1); // confirm teaching into the empty fourth slot
    for (f = 0; f < 6000; f++) {
        if (core->busRead8(core, ui) == 33) break; // fanfare complete
        frames(1, 0);
    }
    CHECK(f < 6000);
    CHECK(monget(player(0), ABI_DATA_MOVE4) == MOVE_WATER_GUN);
    press(1); // dismiss learned-move fanfare
    fossilClose(2); // B exits repeated party picker after return to field
    CHECK(CALL(GetMoney, careSave1() + ABI_SAVE1_MONEY) == originalMoney);
    flashRoundTrip();
    CHECK(monget(player(0), ABI_DATA_MOVE4) == MOVE_WATER_GUN);
    CHECK(CALL(GetMoney, careSave1() + ABI_SAVE1_MONEY) == originalMoney);
    printf("PASS: real Center NPC -> party -> relearn UI -> Water Gun; free, save/reload stable\n");

    CHECK(core->loadState(core, bootState));
}
