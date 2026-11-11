// WRL-009: staged physical NPC interaction for the ten existing Mystery Vendors.
// Unlike vendorUiTests(), this does not call ScriptContext_SetupScript,
// preselect an object, or inject a purchase. It is NOT a new-game route proof.
static void wrlVendorPhysicalTalkTests(void) {
    for (unsigned v = 0; v < MYSTERY_VENDOR_COUNT; v++) {
        const struct VendorSite *site = &vendorSites[v];
        vendorFixture();
        // Saffron's civilians are story-hidden until the relevant event.
        // This staged fixture tests an explicitly visible NPC, not story access.
        if (v == 7)
            CALL(FlagClear, FLAG_HIDE_SAFFRON_CIVILIANS);
        fossilWarp(site->group, site->map, site->x, site->y + 1);
        // CB2_LoadMap can reset the staged encrypted save state. Set the
        // vendor funds after entering the map, before any physical dialogue.
        CALL(SetMoney, careSave1() + ABI_SAVE1_MONEY, 70000);
        CHECK(CALL(GetMoney, careSave1() + ABI_SAVE1_MONEY) == 70000);
        unsigned object = CALL(GetObjectEventIdByLocalIdAndMap,
                               site->local, site->map, site->group);
        CHECK(object < ABI_OBJECT_COUNT);
        CALL(PlayerGetDestCoords, scratch, scratch + 2);
        unsigned px = core->busRead16(core, scratch);
        unsigned py = core->busRead16(core, scratch + 2);
        CHECK(px == site->x + 7 && py == site->y + 8);
        // Deliberately poison the selected-object slot: only the actual
        // player-facing A-button interaction may choose the vendor.
        core->busWrite8(core, SYM_gSelectedObjectEvent, 0xFF);
        frames(6, 64); // Real D-pad UP to face the NPC to the north.
        frames(6, 0);
        press(1);      // Real A-button interaction, no direct script call.
        unsigned f;
        for (f = 0; f < 6000; f++) {
            if (CALL(FuncIsActiveTask, SYM_Task_YesNoMenu_HandleInput | 1U))
                break;
            frames(1, (f % 12) == 0 && fossilTextPaused() ? 1 : 0);
        }
        if (f == 6000) {
            CALL(PlayerGetDestCoords, scratch, scratch + 2);
            fprintf(stderr, "WRL vendor physical talk failed site=%u object=%u selected=%u map=(%u,%u) grid=(%u,%u) script=%08x status=%u\\n",
                    v + 1, object, core->busRead8(core, SYM_gSelectedObjectEvent),
                    site->group, site->map,
                    core->busRead16(core, scratch), core->busRead16(core, scratch + 2),
                    core->busRead32(core, SYM_sGlobalScriptContext + ABI_SCRIPT_PTR),
                    core->busRead8(core, SYM_sGlobalScriptContextStatus));
        }
        CHECK(f < 6000);
        CHECK(core->busRead8(core, SYM_gSelectedObjectEvent) == object);
        CHECK(core->busRead16(core, SYM_gSpecialVar_0x8009) == v);
        vendorTextMatches(vendorIntros[v]);
        CHECK(CALL(GetMoney, careSave1() + ABI_SAVE1_MONEY) == 70000);
        press(2); // Decline purchase via the real Yes/No menu.
        fossilClose(1);
        CHECK(CALL(GetMoney, careSave1() + ABI_SAVE1_MONEY) == 70000);
        CHECK(!CALL(MysteryVendorStatus));
        CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == 3);
        printf("PASS: WRL vendor%u staged physical UP/A NPC interaction, correct seller and no-charge decline\\n", v + 1);
    }
    CHECK(core->loadState(core, bootState));
}
