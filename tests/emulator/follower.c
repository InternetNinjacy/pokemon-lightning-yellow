// WRL-010: actual ARM FireRed ROM state and 252+form resolver acceptance.
// Runs under libmGBA; no host stub replacements for the tested functions.
// World movement/gift/cutscene scenarios still need separate real map playback.
static void followerNativeTests(void)
{
    CHECK(core->loadState(core, bootState));
    initSave();
    create(SPECIES_PIKACHU, 5);
    core->busWrite8(core, SYM_gPlayerPartyCount, 1);
    CHECK(CALL(FollowerState_ResolvePartySlot) == 0xFFFFFFFFU);
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
    CHECK((int32_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CHECK(CALL(FollowerState_CanDisplay) == 0);
    for (unsigned i = 0; i < ABI_POKEMON_SIZE; ++i)
        core->busWrite8(core, SYM_gPlayerParty + ABI_POKEMON_SIZE + i, 0);
    core->busWrite8(core, SYM_gPlayerPartyCount, 1);
    CHECK(CALL(FollowerState_SelectPartySlot, 0) == 1);
    CALL(FollowerState_BeforePartyRemoval, 0);
    CHECK((int32_t)CALL(FollowerState_ResolvePartySlot) == -1);
    CHECK(CALL(FollowerState_SelectPartySlot, 0) == 1);
    CALL(FollowerState_Clear);
    CHECK((int32_t)CALL(FollowerState_ResolvePartySlot) == -1);

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
    CHECK((int32_t)CALL(FollowerState_ResolvePartySlot) == -1);
    printf("PASS: native follower map warp, faint/revive, cry and teardown on two real Day Care maps\n");
}
