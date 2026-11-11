// WRL-010: actual ARM FireRed ROM state and 252+form resolver acceptance.
// Runs under libmGBA; no host stub replacements for the tested functions.
// World movement/gift/cutscene scenarios still need separate real map playback.
static void followerNativeTests(void)
{
    CHECK(core->loadState(core, bootState));
    initSave();
    create(SPECIES_PIKACHU, 5);
    core->busWrite8(core, SYM_gPlayerPartyCount, 1);
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CHECK(CALL(FollowerState_SelectPartySlot, 0) == 1);
    CHECK(CALL(FollowerState_ResolvePartySlot) == 0);
    CHECK(CALL(FollowerState_CanDisplay) == 1);

    // HP reaches zero: visual suppression only, same identity survives.
    core->busWrite16(core, scratch, 0);
    CALL(SetMonData, SYM_gPlayerParty, MON_DATA_HP, scratch);
    CHECK(CALL(FollowerState_CanDisplay) == 0);
    CHECK(CALL(FollowerState_ResolvePartySlot) == 0);
    core->busWrite16(core, scratch, 5);
    CALL(SetMonData, SYM_gPlayerParty, MON_DATA_HP, scratch);
    CHECK(CALL(FollowerState_CanDisplay) == 1);

    // Same PID+OT clone is ambiguous even if both nominally valid.
    for (unsigned i = 0; i < ABI_POKEMON_SIZE; ++i)
        core->busWrite8(core, SYM_gPlayerParty + ABI_POKEMON_SIZE + i,
                         core->busRead8(core, SYM_gPlayerParty + i));
    core->busWrite8(core, SYM_gPlayerPartyCount, 2);
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CHECK(CALL(FollowerState_CanDisplay) == 0);
    for (unsigned i = 0; i < ABI_POKEMON_SIZE; ++i)
        core->busWrite8(core, SYM_gPlayerParty + ABI_POKEMON_SIZE + i, 0);
    core->busWrite8(core, SYM_gPlayerPartyCount, 1);
    CHECK(CALL(FollowerState_SelectPartySlot, 0) == 1);
    CALL(FollowerState_BeforePartyRemoval, 0);
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CHECK(CALL(FollowerState_SelectPartySlot, 0) == 1);
    CALL(FollowerState_Clear);
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);

    // Registry should contain only the approved roster and three Castform
    // alternate sheets; the temporary CI build supplies source art locally.
    unsigned count = CALL(FollowerAssets_Count);
    CHECK(count == 0 || count == 255);
    if (getenv("FOLLOWER_NATIVE_ALL252") &&
        strcmp(getenv("FOLLOWER_NATIVE_ALL252"), "1") == 0)
        CHECK(count == 255);
    if (count == 255) {
        for (unsigned number = 1; number <= 252; ++number) {
            unsigned species = CALL(CustomPokedexNumToSpecies, number);
            CHECK(species != 0);
            for (unsigned shiny = 0; shiny < 2; ++shiny) {
                CHECK(CALL(FollowerAssets_Resolve, species, 0, shiny, SYM_gStringVar4) == 1);
                unsigned size = core->busRead8(core, SYM_gStringVar4 + 28);
                CHECK(size == 32 || size == 64);
                CHECK(core->busRead32(core, SYM_gStringVar4 + 0) >= 0x08000000U);
                CHECK(core->busRead32(core, SYM_gStringVar4 + 24) >= 0x08000000U);
            }
        }
        for (unsigned form=0; form<=3; ++form)
            for (unsigned shiny=0; shiny<2; ++shiny)
                CHECK(CALL(FollowerAssets_Resolve, SPECIES_CASTFORM, form, shiny, SYM_gStringVar4) == 1);
        CHECK(CALL(FollowerAssets_Resolve, SPECIES_PIKACHU, 1, 0, SYM_gStringVar4) == 0);
        CHECK(CALL(FollowerAssets_Resolve, 0, 0, 0, SYM_gStringVar4) == 0);
        CHECK(CALL(FollowerAssets_Resolve, 0xFFFF, 0, 0, SYM_gStringVar4) == 0);
        CHECK(CALL(FollowerAssets_Resolve, SPECIES_CASTFORM, 4, 0, SYM_gStringVar4) == 0);
    }
    printf("PASS: native follower state identity/faint/remove and %u ROM-linked art descriptors\n",count);
}

// Native field callback acceptance on real Route 5/Four Island Day Care maps.
// Covers map reinitialization, visual suppression and same-instance retention.
// This fixture is not a full controller-input door/ledge/cutscene playback.
static void followerFieldNativeTests(void)
{
    if (CALL(FollowerAssets_Count) != 255)
        return; // source-only local build has no copied artwork by design
    daycareFixture(1);
    CHECK(CALL(FollowerState_SelectPartySlot, 0) == 1);
    frames(100, 0);
    unsigned objectId = CALL(FollowerGraphics_GetObjectEventId);
    unsigned spriteId = CALL(FollowerGraphics_GetSpriteId);
    CHECK(objectId < ABI_OBJECT_COUNT);
    CHECK(spriteId < 64);
    CHECK(CALL(FollowerState_ResolvePartySlot) == 0);

    // Controller-input acceptance, not direct coordinate mutation: the live
    // overworld must advance the real player and the follower must remain
    // within one tile of the player after each completed cardinal step.
    unsigned playerObjectId = core->busRead8(core, SYM_gPlayerAvatar + ABI_AVATAR_EVENT_ID);
    CHECK(playerObjectId < ABI_OBJECT_COUNT);
    unsigned playerCoords = SYM_gObjectEvents + playerObjectId * ABI_OBJECT_EVENT_SIZE + ABI_OBJECT_EVENT_COORDS;
    unsigned followerCoords = SYM_gObjectEvents + objectId * ABI_OBJECT_EVENT_SIZE + ABI_OBJECT_EVENT_COORDS;
    int playerX = (int16_t)core->busRead16(core, playerCoords);
    int playerY = (int16_t)core->busRead16(core, playerCoords + 2);
    unsigned moved = 0;
    const unsigned cardinalKeys[] = {64, 32, 16, 128}; // native UP, LEFT, RIGHT, DOWN
    for (unsigned direction = 0; direction < 4; ++direction) {
        frames(9, cardinalKeys[direction]);
        frames(30, 0);
        int x = (int16_t)core->busRead16(core, playerCoords);
        int y = (int16_t)core->busRead16(core, playerCoords + 2);
        if (x != playerX || y != playerY) {
            ++moved;
            // These are distinct real D-pad movements including two turns
            // and one reversal, not four calls to the controller routine.
            // Ensure each held input completes exactly one cardinal tile:
            // a multiple-tile jump can otherwise satisfy the old trail test.
            const int expectedDx[] = {0, -1, 1, 0};
            const int expectedDy[] = {-1, 0, 0, 1};
            CHECK(x - playerX == expectedDx[direction]);
            CHECK(y - playerY == expectedDy[direction]);
            int followerX = (int16_t)core->busRead16(core, followerCoords);
            int followerY = (int16_t)core->busRead16(core, followerCoords + 2);
            CHECK(abs(followerX - x) <= 1);
            CHECK(abs(followerY - y) <= 1);
            // Exact one-tile trail through corners and the reversal:
            // after each completed step, the follower occupies the
            // previous PLAYER tile, never merely an adjacent stale tile.
            CHECK(followerX == playerX);
            CHECK(followerY == playerY);
            CHECK(CALL(FollowerState_ResolvePartySlot) == 0);
            CHECK(CALL(FollowerGraphics_GetObjectEventId) == objectId);
            // Leave the controller at rest. The follower may animate, but
            // it must not drift into the player or lose its owned object.
            frames(20, 0);
            CHECK((int16_t)core->busRead16(core, followerCoords) == followerX);
            CHECK((int16_t)core->busRead16(core, followerCoords + 2) == followerY);
            CHECK((int16_t)core->busRead16(core, playerCoords) == x);
            CHECK((int16_t)core->busRead16(core, playerCoords + 2) == y);
            CHECK(CALL(FollowerGraphics_GetObjectEventId) == objectId);
        }
        playerX = x;
        playerY = y;
    }
    CHECK(moved == 4);
    printf("PASS: native cardinal controller input produced %u moved directions with adjacent follower tiles\\n", moved);

    // Fainting must not replace the selected Pokémon or drop identity.
    core->busWrite16(core, scratch, 0);
    CALL(SetMonData, SYM_gPlayerParty, MON_DATA_HP, scratch);
    frames(14, 0);
    CHECK(CALL(FollowerState_CanDisplay) == 0);
    CHECK(CALL(FollowerState_ResolvePartySlot) == 0);
    CHECK(CALL(FollowerGraphics_GetObjectEventId) == objectId);

    // Revive/heal the same instance, without selecting slot one again.
    core->busWrite16(core, scratch, 17);
    CALL(SetMonData, SYM_gPlayerParty, MON_DATA_HP, scratch);
    frames(60, 0);
    CHECK(CALL(FollowerState_CanDisplay) == 1);
    CHECK(CALL(FollowerGraphics_GetSpriteId) < 64);
    CALL(FollowerSpecial_PlayCry); // actual compiled cry code; no friendship writes
    CHECK(CALL(FollowerState_ResolvePartySlot) == 0);

    // Cross actual FireRed map reload callbacks with an assigned follower.
    CALL(SetWarpDestination, MAP_GROUP(MAP_FOUR_ISLAND_POKEMON_DAY_CARE),
         MAP_NUM(MAP_FOUR_ISLAND_POKEMON_DAY_CARE), 0xFFFFFFFFU, 3, 6);
    CALL(WarpIntoMap);
    CALL(ResetInitialPlayerAvatarState);
    CALL(CB2_LoadMap);
    frames(360, 0);
    CHECK((core->busRead32(core, SYM_gMain + ABI_MAIN_CB2) & ~1U) == SYM_CB2_Overworld);
    CHECK(CALL(FollowerState_ResolvePartySlot) == 0);
    CHECK(CALL(FollowerGraphics_GetSpriteId) < 64);
    CHECK(CALL(FollowerGraphics_GetObjectEventId) < ABI_OBJECT_COUNT);

    // Clearing selection tears down the single owned sprite/object.
    CALL(FollowerState_Clear);
    frames(20, 0);
    CHECK(CALL(FollowerGraphics_GetObjectEventId) == ABI_OBJECT_COUNT);
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    printf("PASS: native follower map warp, faint/revive, cry and teardown on two real Day Care maps\n");
    // Exercise the real sprite allocator and object-event owner through both
    // tile geometries: Pikachu uses 16 tiles, Steelix requires 64 tiles.
    // An old-size ObjectEventSetGraphicsId fallback would corrupt this switch.
    daycareFixture(1);
    CALL(CreateMon, player(1), SPECIES_STEELIX, 20, 31, 1, 111, 0, 0);
    CHECK(CALL(FollowerState_SelectPartySlot, 1) == 1);
    frames(100, 0);
    CHECK(CALL(FollowerGraphics_GetObjectEventId) < ABI_OBJECT_COUNT);
    CHECK(CALL(FollowerGraphics_GetSpriteId) < 64);
    CHECK(CALL(FollowerGraphics_GetSize) == 64);
    CHECK(CALL(FollowerState_ResolvePartySlot) == 1);

    CHECK(CALL(FollowerState_SelectPartySlot, 0) == 1);
    CALL(FollowerController_RequestRefresh);
    frames(100, 0);
    CHECK(CALL(FollowerGraphics_GetObjectEventId) < ABI_OBJECT_COUNT);
    CHECK(CALL(FollowerGraphics_GetSpriteId) < 64);
    CHECK(CALL(FollowerGraphics_GetSize) == 32);
    CHECK(CALL(FollowerState_ResolvePartySlot) == 0);
    CHECK(CALL(FollowerState_SelectPartySlot, 1) == 1);
    CALL(FollowerController_RequestRefresh);
    frames(100, 0);
    CHECK(CALL(FollowerGraphics_GetObjectEventId) < ABI_OBJECT_COUNT);
    CHECK(CALL(FollowerGraphics_GetSpriteId) < 64);
    CHECK(CALL(FollowerGraphics_GetSize) == 64);
    printf("PASS: native 64x64 Steelix -> 32x32 Pikachu -> 64x64 Steelix resource transitions\\n");

}

// Story bridge: actual WRL-001 gift functions on ARM under libmGBA.
// The separate WRL-001 suite covers Parcel flag delivery/PC overflow.
// This verifies WRL-010 assigns only the opening Pikachu and never Eevee.

/*
 * Native field-party menu acceptance.  Do not replace these button presses
 * with direct FollowerState_SelectPartySlot / FollowerState_Clear calls:
 * that would skip the real action-list, cursor and callback integration.
 */
static void followerMenuTap(unsigned key)
{
    frames(2, key);
    frames(12, 0);
}

static void followerMenuChooseAction(void)
{
    // Two non-Egg party members: Summary, Switch, Item, follower, Cancel.
    // The follower item is fourth (three DOWN presses from initial cursor).
    followerMenuTap(1); // A opens the actual selected Pokémon action menu
    frames(36, 0);
    for (unsigned i = 0; i < 3; ++i)
        followerMenuTap(128); // DOWN
    followerMenuTap(1); // A dispatches Set as Follower or Stop Following
    frames(36, 0);
}


/*
 * Cross-build legacy save acceptance. The input must be the genuine 128 KiB
 * flash file emitted by the pinned pre-follower main ROM's TrySavingData.
 * It is deliberately NOT constructed by editing the current save record.
 */
static void followerHistoricalFlashNativeTests(void)
{
    const char *path = getenv("LIGHTNING_IMPORT_OLD_SAVE");
    CHECK(path != NULL && *path != '\0');
    FILE *file = fopen(path, "rb");
    CHECK(file != NULL);
    unsigned char *bytes = malloc(0x20000);
    CHECK(bytes != NULL);
    CHECK(fread(bytes, 1, 0x20000, file) == 0x20000);
    CHECK(fgetc(file) == EOF);
    CHECK(fclose(file) == 0);
    CHECK(core->loadState(core, bootState));
    initSave();
    GBASavedataForceType(&((struct GBA *)core->board)->memory.savedata, SAVEDATA_FLASH1M);
    CHECK(core->savedataRestore(core, bytes, 0x20000, true));
    free(bytes);
    CHECK(CALL(LoadGameSave, 0) == 1);
    CALL(SetBagPocketsPointers);
    CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == 2);
    CHECK(monget(player(0), ABI_DATA_SPECIES) == SPECIES_PIKACHU);
    CHECK(monget(player(1), ABI_DATA_SPECIES) == SPECIES_EEVEE);
    CHECK(monget(player(0), ABI_DATA_PERSONALITY) == 34567);
    CHECK(monget(player(1), ABI_DATA_PERSONALITY) == 45678);
    CHECK(CALL(BagGetQuantityByItemId, ITEM_POTION) == 3);
    CHECK(CALL(FlagGet, FLAG_SYS_POKEMON_GET));
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CHECK(!CALL(FollowerState_CanDisplay));
    CHECK(CALL(FollowerState_SelectPartySlot, 1));
    CHECK(CALL(FollowerState_ResolvePartySlot) == 1);
    flashRoundTrip();
    CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == 2);
    CHECK(monget(player(0), ABI_DATA_PERSONALITY) == 34567);
    CHECK(monget(player(1), ABI_DATA_PERSONALITY) == 45678);
    CHECK(CALL(BagGetQuantityByItemId, ITEM_POTION) == 3);
    CHECK(CALL(FlagGet, FLAG_SYS_POKEMON_GET));
    CHECK(CALL(FollowerState_ResolvePartySlot) == 1);
    CALL(FollowerState_Clear);
    flashRoundTrip();
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CHECK(CALL(BagGetQuantityByItemId, ITEM_POTION) == 3);
    CHECK(CALL(FlagGet, FLAG_SYS_POKEMON_GET));
    printf("PASS: genuine pinned pre-feature ROM flash save loaded, PID/party/bag/flags preserved, follower selected/cleared across real flash reloads\\n");
}

static void followerPartyMenuNativeTests(void)
{
    daycareFixture(1);
    CALL(ZeroPlayerPartyMons);
    CALL(CreateMon, player(0), SPECIES_PIKACHU, 5, 31, 1, 34567, 0, 0);
    core->busWrite8(core, SYM_gPlayerPartyCount, 1);
    CALL(ScriptNameSocketStarterPikachu);
    CHECK(CALL(ScriptGiveSueEevee) == 0);
    CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == 2);
    const unsigned socketPid = monget(player(0), ABI_DATA_PERSONALITY);
    const unsigned suePid = monget(player(1), ABI_DATA_PERSONALITY);
    CHECK(socketPid != suePid);
    CALL(FollowerState_Clear);
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);

    // Initialize the real compiled field party menu, then press actual keys.
    CALL(CB2_PartyMenuFromStartMenu);
    frames(180, 0);
    CHECK(core->busRead8(core, SYM_gPartyMenu + ABI_PARTY_MENU_SLOT) == 0);
    followerMenuChooseAction();
    CHECK(CALL(FollowerState_ResolvePartySlot) == 0);
    CHECK(monget(player(0), ABI_DATA_PERSONALITY) == socketPid);

    followerMenuTap(128); // move highlighted Pokémon from slot 0 to slot 1
    CHECK(core->busRead8(core, SYM_gPartyMenu + ABI_PARTY_MENU_SLOT) == 1);
    followerMenuChooseAction();
    CHECK(CALL(FollowerState_ResolvePartySlot) == 1);
    CHECK(monget(player(1), ABI_DATA_PERSONALITY) == suePid);
    followerMenuTap(2); // B closes party menu, preserving selection
    frames(140, 0);
    flashRoundTrip();
    CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == 2);
    CHECK(monget(player(0), ABI_DATA_PERSONALITY) == socketPid);
    CHECK(monget(player(1), ABI_DATA_PERSONALITY) == suePid);
    CHECK(CALL(FollowerState_ResolvePartySlot) == 1);

    // Reopen and use the menu's Stop Following action for the saved SUE.
    CALL(CB2_PartyMenuFromStartMenu);
    frames(180, 0);
    followerMenuTap(128);
    CHECK(core->busRead8(core, SYM_gPartyMenu + ABI_PARTY_MENU_SLOT) == 1);
    followerMenuChooseAction();
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    followerMenuTap(2);
    frames(140, 0);
    flashRoundTrip();
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CHECK(monget(player(0), ABI_DATA_PERSONALITY) == socketPid);
    CHECK(monget(player(1), ABI_DATA_PERSONALITY) == suePid);
    printf("PASS: actual party-menu A/D-pad Set/Stop actions, SOCKET-to-SUE identity and flash deselection\\n");
    daycareFixture(1);
}

static void followerGiftNativeTests(void)
{
    daycareFixture(1);
    CALL(ZeroPlayerPartyMons);
    CALL(CreateMon, SYM_gPlayerParty, SPECIES_PIKACHU, 5, 31, 1, 34567, 0, 0);
    core->busWrite8(core, SYM_gPlayerPartyCount, 1);
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CALL(ScriptNameSocketStarterPikachu);
    CHECK(CALL(FollowerState_ResolvePartySlot) == 0);
    CHECK(CALL(FollowerState_CanDisplay) == 1);
    CALL(GetMonData3, SYM_gPlayerParty, ABI_DATA_NICKNAME, scratch);
    textEquals(scratch, "SOCKET");
    unsigned pid = get(ABI_DATA_PERSONALITY);
    unsigned result = CALL(ScriptGiveSueEevee);
    CHECK(result == 0);
    CHECK(core->busRead8(core,SYM_gPlayerPartyCount) == 2);
    CHECK(monget(player(1),ABI_DATA_SPECIES) == SPECIES_EEVEE);
    CHECK(CALL(FollowerState_ResolvePartySlot) == 0);
    CHECK(get(ABI_DATA_PERSONALITY) == pid);
    CHECK(CALL(FollowerState_SelectPartySlot,1) == 1);
    CHECK(CALL(FollowerState_ResolvePartySlot) == 1);
    // Verify real flash persistence of both gift identities and the manually
    // selected individual; do not confuse RAM-only state with saved state.
    unsigned suePid = monget(player(1), ABI_DATA_PERSONALITY);
    flashRoundTrip();
    CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == 2);
    CHECK(monget(player(0), ABI_DATA_SPECIES) == SPECIES_PIKACHU);
    CHECK(monget(player(1), ABI_DATA_SPECIES) == SPECIES_EEVEE);
    CHECK(monget(player(0), ABI_DATA_PERSONALITY) == pid);
    CHECK(monget(player(1), ABI_DATA_PERSONALITY) == suePid);
    CHECK(CALL(FollowerState_ResolvePartySlot) == 1);

    // A byte-identical SUE clone after an actual flash reload must not allow
    // ambiguous PID/OT selection. Persist the fail-closed selection state and
    // verify that deleting the clone does not silently resurrect a follower.
    for (unsigned i = 0; i < ABI_POKEMON_SIZE; ++i)
        core->busWrite8(core, SYM_gPlayerParty + 2 * ABI_POKEMON_SIZE + i,
                        core->busRead8(core, SYM_gPlayerParty + ABI_POKEMON_SIZE + i));
    core->busWrite8(core, SYM_gPlayerPartyCount, 3);
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CHECK(CALL(FollowerState_CanDisplay) == 0);
    flashRoundTrip();
    CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == 3);
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    for (unsigned i = 0; i < ABI_POKEMON_SIZE; ++i)
        core->busWrite8(core, SYM_gPlayerParty + 2 * ABI_POKEMON_SIZE + i, 0);
    core->busWrite8(core, SYM_gPlayerPartyCount, 2);
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CHECK(CALL(FollowerState_SelectPartySlot, 1) == 1);
    flashRoundTrip();
    CHECK(CALL(FollowerState_ResolvePartySlot) == 1);
    CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == 2);
    CALL(FollowerState_Clear);
    flashRoundTrip();
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == 2);
    printf("PASS: native SOCKET auto-assignment, manual SUE selection and cleared state across flash saves\n");
    // Native compatibility of saves made before the 16-byte follower record
    // existed. Exercise the exact SaveBlock1 field from the ARM-derived ABI,
    // without touching Pokemon, storage or unrelated save regions.
    unsigned save1 = core->busRead32(core, SYM_gSaveBlock1Ptr);
    CHECK(save1 >= 0x02000000U && save1 < 0x02040000U);
    unsigned record = save1 + ABI_SAVE1_FOLLOWER_RECORD;
    for (unsigned variant = 0; variant < 2; ++variant) {
        for (unsigned i = 0; i < 16; ++i)
            core->busWrite8(core, record + i, variant ? 0xFF : 0);
        flashRoundTrip();
        CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
        CHECK(CALL(FollowerState_CanDisplay) == 0);
        CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == 2);
        CHECK(monget(player(0), ABI_DATA_PERSONALITY) == pid);
        CHECK(monget(player(1), ABI_DATA_PERSONALITY) == suePid);
    }
    // Incompatible version and damaged checksum are likewise rejected after
    // real flash reload, without accidentally selecting party lead.
    CHECK(CALL(FollowerState_SelectPartySlot, 1) == 1);
    core->busWrite8(core, record + 14, 2); // record.version
    flashRoundTrip();
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CHECK(CALL(FollowerState_SelectPartySlot, 1) == 1);
    core->busWrite8(core, record + 12, core->busRead8(core, record + 12) ^ 0x40); // integrity
    flashRoundTrip();
    CHECK((int8_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CHECK(core->busRead8(core, SYM_gPlayerPartyCount) == 2);
    CHECK(monget(player(0), ABI_DATA_PERSONALITY) == pid);
    CHECK(monget(player(1), ABI_DATA_PERSONALITY) == suePid);
    printf("PASS: native old/zero/erased follower records and invalid version/integrity fail closed across flash saves\\n");

    // loadState restores the emulated CPU, but not the libmGBA flash file.
    // The following Dex suite creates its own Castform save and must not
    // inherit our two-gift sectors or counters. Reset through the existing
    // verified fresh-flash fixture instead of weakening any Dex assertions.
    daycareFixture(1);
}
