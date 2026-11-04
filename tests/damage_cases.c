static unsigned checks;
#define CHECK(expr) do { checks++; if (!(expr)) { fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #expr); return 1; } } while (0)
static void Reset(void)
{
    int i, j;
    gHitMarker = gBattleControllerExecFlags = gMoveResultFlags = 0;
    memset(gTakenDmg, 0, sizeof(gTakenDmg));
    memset(gSpecialStatuses, 0, sizeof(gSpecialStatuses));
    memset(gDisableStructs, 0, sizeof(gDisableStructs));
    memset(gSideTimers, 0, sizeof(gSideTimers));
    memset(gBattleMons, 0, sizeof(gBattleMons));
    memset(&resourceFlags, 0, sizeof(resourceFlags));
    memset(&battleStruct, 0, sizeof(battleStruct));
    memset(gProtectStructs, 0, sizeof(gProtectStructs));
    memset(gSideStatuses, 0, sizeof(gSideStatuses));
    memset(gStatuses3, 0, sizeof(gStatuses3));
    resources.flags = &resourceFlags;
    gBattleScripting.dmgMultiplier = 1;
    for (i = 0; i < 2; i++)
    {
        gBattleMons[i].level = 50;
        gBattleMons[i].hp = gBattleMons[i].maxHP = 200;
        gBattleMons[i].attack = 200;
        gBattleMons[i].defense = 100;
        gBattleMons[i].spAttack = 50;
        gBattleMons[i].spDefense = 200;
        for (j = 0; j < NUM_BATTLE_STATS; j++)
            gBattleMons[i].statStages[j] = DEFAULT_STAT_STAGE;
    }
    gCritMultiplier = 1;
    gBattleTypeFlags = gBattleWeather = gDynamicBasePower = 0;
    gBattlerAttacker = 0; gBattlerTarget = 1;
    fieldAbility = 0; alive = 1;
}
static s32 Damage(u16 move, u16 screens, u16 power, u8 type)
{
    gCurrentMove = move;
    return CalculateBaseDamage(&gBattleMons[0], &gBattleMons[1], move, screens, power, type, 0, 1);
}
int main(void)
{
    s32 physical, special;
    unsigned i;
    const u8 script[8] = {0};
    Reset();
    physical = Damage(MOVE_FIRE_PUNCH, 0, 0, 0);
    special = Damage(MOVE_FLAMETHROWER, 0, 0, 0);
    CHECK(physical == 68); CHECK(special == 12);
    CHECK(Damage(MOVE_SHADOW_PUNCH, 0, 0, 0) == 54);
    CHECK(Damage(MOVE_SHADOW_BALL, 0, 0, 0) == 10);
    // Isolated special Ghost standard-power fixture: same production calculator,
    // no invented move in game data. Night Shade is tested through its own effect.
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 80, TYPE_GHOST) == 10);
    gBattleMons[0].attack = 400;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 134);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == special);
    gBattleMons[0].attack = 200; gBattleMons[1].defense = 200;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 35);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == special);
    Reset(); gBattleMons[0].spAttack = 100;
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == 22);
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == physical);
    gBattleMons[1].spDefense = 100;
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == 43);
    Reset();
    for (i = 0; i < MOVES_COUNT; i++)
    {
        CHECK(gBattleMoves[i].category >= MOVE_CATEGORY_PHYSICAL && gBattleMoves[i].category <= MOVE_CATEGORY_STATUS);
        if (IS_MOVE_STATUS(i)) CHECK(Damage(i, 0, 100, TYPE_FIRE) == 0);
        CHECK(strcmp((const char *)GetMoveCategoryName(i), IS_MOVE_PHYSICAL(i) ? "Physical" : IS_MOVE_SPECIAL(i) ? "Special" : "Status") == 0);
        CHECK(strcmp((const char *)GetMoveCategoryShortName(i), IS_MOVE_PHYSICAL(i) ? "PHYS" : IS_MOVE_SPECIAL(i) ? "SPEC" : "STAT") == 0);
    }
    CHECK(sizeof(struct BattleMove) == 10);
    CHECK(__builtin_offsetof(struct BattleMove, flags) == 8);
    CHECK(__builtin_offsetof(struct BattleMove, category) == 9);
    CHECK(__alignof__(struct BattleMove) == 1);
    CHECK(Damage(MOVE_FIRE_PUNCH, SIDE_STATUS_REFLECT, 0, 0) == 35);
    CHECK(Damage(MOVE_FIRE_PUNCH, SIDE_STATUS_LIGHTSCREEN, 0, 0) == physical);
    CHECK(Damage(MOVE_FLAMETHROWER, SIDE_STATUS_LIGHTSCREEN, 0, 0) == 7);
    CHECK(Damage(MOVE_FLAMETHROWER, SIDE_STATUS_REFLECT, 0, 0) == special);
    gBattleMons[0].status1 = STATUS1_BURN;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 35);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == special);
    gBattleMons[0].ability = ABILITY_GUTS;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 101);
    Reset();
    gBattleMons[0].ability = ABILITY_HUGE_POWER;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 134);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == special);
    gBattleMons[0].ability = ABILITY_PURE_POWER;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 134);
    gBattleMons[0].ability = ABILITY_HUSTLE;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 101);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == special);
    Reset(); gBattleMons[1].ability = ABILITY_THICK_FAT;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 35);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == 7);
    Reset(); gBattleMons[0].item = HOLD_EFFECT_CHOICE_BAND;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 101);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == special);
    Reset(); gBattleMons[0].item = HOLD_EFFECT_FIRE_POWER;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 74);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == 13);
    Reset(); gBattleMons[0].item = HOLD_EFFECT_LIGHT_BALL; gBattleMons[0].species = SPECIES_PIKACHU;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == physical); // Gen III Light Ball boosts Sp.Atk only
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == 22);
    Reset(); gBattleMons[0].item = HOLD_EFFECT_THICK_CLUB; gBattleMons[0].species = SPECIES_MAROWAK;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 134);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == special);
    Reset(); gBattleMons[1].item = HOLD_EFFECT_METAL_POWDER; gBattleMons[1].species = SPECIES_DITTO;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 35);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == special);
    Reset(); gBattleMons[0].item = HOLD_EFFECT_SOUL_DEW; gBattleMons[0].species = SPECIES_LATIAS;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == physical);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == 17);
    Reset(); gBattleMons[0].item = HOLD_EFFECT_DEEP_SEA_TOOTH; gBattleMons[0].species = SPECIES_CLAMPERL;
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == 22);
    Reset(); gBattleMons[1].item = HOLD_EFFECT_DEEP_SEA_SCALE; gBattleMons[1].species = SPECIES_CLAMPERL;
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == 7);
    Reset(); gBattleMons[1].ability = ABILITY_MARVEL_SCALE; gBattleMons[1].status1 = STATUS1_PARALYSIS;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 46);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == special);
    Reset(); gBattleMons[0].ability = ABILITY_PLUS; fieldAbility = ABILITY_MINUS;
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == 17);
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == physical);
    Reset(); gBattleMons[1].ability = ABILITY_THICK_FAT;
    CHECK(Damage(MOVE_ICE_PUNCH, 0, 0, 0) == 35);
    CHECK(Damage(MOVE_ICE_BEAM, 0, 0, 0) == 7);
    Reset();
    gBattleWeather = B_WEATHER_RAIN_TEMPORARY;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 35);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == 7);
    gBattleWeather = B_WEATHER_SUN_TEMPORARY;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 101);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == 17);
    fieldAbility = ABILITY_CLOUD_NINE;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == physical);
    Reset(); resourceFlags.flags[0] = RESOURCE_FLAG_FLASH_FIRE;
    CHECK(Damage(MOVE_FIRE_PUNCH, 0, 0, 0) == 101);
    CHECK(Damage(MOVE_FLAMETHROWER, 0, 0, 0) == 17);
    Reset();
    CHECK(Damage(MOVE_EXPLOSION, 0, 0, 0) == 442);
    gBattleMons[0].statStages[STAT_ATK] = 4; gBattleMons[1].statStages[STAT_DEF] = 8;
    gCritMultiplier = 2;
    CHECK(Damage(MOVE_FIRE_PUNCH, SIDE_STATUS_REFLECT, 0, 0) == physical);
    Reset(); gBattleTypeFlags = BATTLE_TYPE_DOUBLE; alive = 2;
    CHECK(Damage(MOVE_FIRE_PUNCH, SIDE_STATUS_REFLECT, 0, 0) == 46);
    CHECK(Damage(MOVE_SURF, 0, 0, 0) == 7);
    Reset();
    CHECK(Damage(MOVE_HIDDEN_POWER, 0, 60, TYPE_FIGHTING) == 8);
    gBattleMons[0].attack = 600;
    CHECK(Damage(MOVE_HIDDEN_POWER, 0, 60, TYPE_FIGHTING) == 8);
    Reset();
    gCurrentMove = MOVE_FIRE_PUNCH; AI_CalcDmg(0, 1); CHECK(gBattleMoveDamage == physical);
    gCurrentMove = MOVE_FLAMETHROWER; AI_CalcDmg(0, 1); CHECK(gBattleMoveDamage == special);
    gCurrentMove = MOVE_CALM_MIND; AI_CalcDmg(0, 1); CHECK(gBattleMoveDamage == 0);
    // Execute unchanged fixed-damage commands, rather than ordinary base damage.
    gBattlescriptCurrInstr = script; gCurrentMove = MOVE_NIGHT_SHADE; Cmd_dmgtolevel(); CHECK(gBattleMoveDamage == 50);
    gBattlescriptCurrInstr = script; gCurrentMove = MOVE_SEISMIC_TOSS; Cmd_dmgtolevel(); CHECK(gBattleMoveDamage == 50);
    gBattlescriptCurrInstr = script; randomValue = 0; Cmd_psywavedamageeffect(); CHECK(gBattleMoveDamage == 25);
    gBattlescriptCurrInstr = script; randomValue = 10; Cmd_psywavedamageeffect(); CHECK(gBattleMoveDamage == 75);
    gBattlescriptCurrInstr = script; Cmd_damagetohalftargethp(); CHECK(gBattleMoveDamage == 100);
    // Execute the HP update command: real category-sensitive damage history,
    // passive/status exclusions, and the controller-busy synchronization guard.
    {
        const u8 update[] = {0, BS_TARGET};
        Reset(); gCurrentMove = MOVE_FIRE_PUNCH; gBattleMoveDamage = 37;
        gBattlescriptCurrInstr = update; Cmd_datahpupdate();
        CHECK(gProtectStructs[1].physicalDmg == 37);
        CHECK(gProtectStructs[1].specialDmg == 0);
        CHECK(gProtectStructs[1].physicalBattlerId == 0);
        CHECK(gBattleMons[1].hp == 163);
        Reset(); gCurrentMove = MOVE_SHADOW_BALL; gBattleMoveDamage = 23;
        gBattlescriptCurrInstr = update; Cmd_datahpupdate();
        CHECK(gProtectStructs[1].specialDmg == 23);
        CHECK(gProtectStructs[1].physicalDmg == 0);
        Reset(); gCurrentMove = MOVE_FIRE_PUNCH; gBattleMoveDamage = 37; gHitMarker = HITMARKER_PASSIVE_DAMAGE;
        gBattlescriptCurrInstr = update; Cmd_datahpupdate(); CHECK(gProtectStructs[1].physicalDmg == 0);
        Reset(); gCurrentMove = MOVE_PAIN_SPLIT; gBattleMoveDamage = 25;
        gBattlescriptCurrInstr = update; Cmd_datahpupdate();
        CHECK(gBattleMons[1].hp == 175); CHECK(gProtectStructs[1].specialDmg == 0); CHECK(gProtectStructs[1].physicalDmg == 0);
        Reset(); gCurrentMove = MOVE_FIRE_PUNCH; gBattleMoveDamage = 37; gBattleControllerExecFlags = 1;
        gBattlescriptCurrInstr = update; Cmd_datahpupdate(); CHECK(gBattleMons[1].hp == 200); CHECK(gBattlescriptCurrInstr == update);
        gBattleControllerExecFlags = 0;
    }
    // Counter/Mirror Coat consume the category-specific damage history.
    gProtectStructs[0].physicalDmg = 37; gProtectStructs[0].physicalBattlerId = 1;
    gBattlescriptCurrInstr = script; Cmd_counterdamagecalculator(); CHECK(gBattleMoveDamage == 74);
    gProtectStructs[0].specialDmg = 23; gProtectStructs[0].specialBattlerId = 1;
    gBattlescriptCurrInstr = script; Cmd_mirrorcoatdamagecalculator(); CHECK(gBattleMoveDamage == 46);
    gProtectStructs[0].specialDmg = 0;
    gBattlescriptCurrInstr = script; Cmd_mirrorcoatdamagecalculator(); CHECK(gBattlescriptCurrInstr == NULL);
    // New AI opcodes execute with real structs and only observed opponent moves.
    resources.ai = &thinking; resources.battleHistory = &history;
    thinking.funcResult = MOVE_FIRE_PUNCH; sAIScriptPtr = script;
    Cmd_get_move_category_from_result(); CHECK(thinking.funcResult == MOVE_CATEGORY_PHYSICAL);
    CHECK(sAIScriptPtr == script + 1);
    thinking.funcResult = MOVE_SHADOW_BALL; sAIScriptPtr = script;
    Cmd_get_move_category_from_result(); CHECK(thinking.funcResult == MOVE_CATEGORY_SPECIAL);
    thinking.funcResult = MOVE_UNAVAILABLE; sAIScriptPtr = script;
    Cmd_get_move_category_from_result(); CHECK(thinking.funcResult == MOVE_CATEGORY_STATUS);
    {
        const u8 hasPhysical[] = {0x53, AI_TARGET, MOVE_CATEGORY_PHYSICAL};
        const u8 hasSpecial[] = {0x53, AI_TARGET, MOVE_CATEGORY_SPECIAL};
        gBattleMons[1].moves[0] = MOVE_FIRE_PUNCH;
        sAIScriptPtr = hasPhysical; Cmd_has_move_category(); CHECK(!thinking.funcResult); // hidden
        history.usedMoves[0].moves[0] = MOVE_FIRE_PUNCH;
        sAIScriptPtr = hasPhysical; Cmd_has_move_category(); CHECK(thinking.funcResult);
        CHECK(sAIScriptPtr == hasPhysical + 3);
        sAIScriptPtr = hasSpecial; Cmd_has_move_category(); CHECK(!thinking.funcResult);
        history.usedMoves[0].moves[0] = MOVE_SHADOW_BALL;
        sAIScriptPtr = hasSpecial; Cmd_has_move_category(); CHECK(thinking.funcResult);
    }
    printf("PASS: %u numeric/category/UI-helper assertions (production routines, stubbed environment)\n", checks);
    return 0;
}
