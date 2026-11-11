#include "constants/opponents.h"
#include "ordinary_trainers_expected.h"

// Native ROM readback of all owner-authorized ordinary parties; no stubs.
static void ordinaryTrainerPartyDataTests(void) {
    // One native initialized fixture suffices: CreateNPCTrainerParty itself
    // calls ZeroEnemyPartyMons before populating each new Trainer array.
    daycareFixture(1);
    unsigned index = 0;
    for (unsigned t = 0; t < ORDINARY_TRAINER_COUNT; t++) {
        core->busWrite32(core, SYM_gBattleTypeFlags, BATTLE_TYPE_TRAINER);
        const unsigned id = ordinaryTrainerIds[t];
        const unsigned size = ordinaryTrainerSizes[t];
        const unsigned result = CALL(CreateNPCTrainerParty, SYM_gEnemyParty, id);
        if (result != size)
            fprintf(stderr, "Trainer %u ID %u: native size %u expected %u\n",
                    t, id, result, size);
        CHECK(result == size);
        for (unsigned i = 0; i < size; i++) {
            const struct OrdinaryExpected *mon = &ordinaryTrainerMons[index++];
            if (monget(enemy(i), ABI_DATA_SPECIES) != mon->species)
                fprintf(stderr, "Trainer %u mon %u: species native %u expected %u\n",
                        id, i, monget(enemy(i), ABI_DATA_SPECIES), mon->species);
            CHECK(monget(enemy(i), ABI_DATA_SPECIES) == mon->species);
            CHECK(monget(enemy(i), ABI_DATA_LEVEL) == mon->level);
            CHECK(monget(enemy(i), ABI_DATA_HELD_ITEM) == mon->item);
            for (unsigned iv = 0; iv < 6; iv++)
                CHECK(monget(enemy(i), ABI_DATA_HP_IV + iv) == mon->iv * 31 / 255);
            for (unsigned slot = 0; slot < 4; slot++) {
                CHECK(monget(enemy(i), ABI_DATA_MOVE1 + slot) == mon->moves[slot]);
                CHECK(monget(enemy(i), ABI_DATA_PP1 + slot)
                      == core->busRead8(core,
                          SYM_gBattleMoves + ABI_BMOVE_SIZE * mon->moves[slot] + ABI_BMOVE_PP));
            }
        }
    }
    CHECK(index == ORDINARY_TRAINER_MON_COUNT);
    // Tucker is separately owner-approved and not part of the locked
    // 500-party ordinary Trainer expected array. Check the actual native
    // constructor, including both explicit and empty custom-move slots.
    core->busWrite32(core, SYM_gBattleTypeFlags, BATTLE_TYPE_TRAINER);
    CHECK(CALL(CreateNPCTrainerParty, SYM_gEnemyParty,
               TRAINER_GENTLEMAN_TUCKER) == 1);
    const unsigned tucker = enemy(0);
    CHECK(monget(tucker, ABI_DATA_SPECIES) == SPECIES_VOLTORB);
    CHECK(monget(tucker, ABI_DATA_LEVEL) == 22);
    CHECK(monget(tucker, ABI_DATA_HELD_ITEM) == ITEM_NONE);
    for (unsigned iv = 0; iv < 6; iv++)
        CHECK(monget(tucker, ABI_DATA_HP_IV + iv) == 0);
    const unsigned tuckerMoves[] = {MOVE_CHARGE, MOVE_SPARK, MOVE_NONE, MOVE_NONE};
    for (unsigned slot = 0; slot < 4; slot++) {
        unsigned move = monget(tucker, ABI_DATA_MOVE1 + slot);
        CHECK(move == tuckerMoves[slot]);
        CHECK(monget(tucker, ABI_DATA_PP1 + slot) ==
              core->busRead8(core,
                  SYM_gBattleMoves + ABI_BMOVE_SIZE * move + ABI_BMOVE_PP));
    }
    printf("PASS: native Vermilion Tucker one Voltorb Lv22, exact Charge/Spark/empty slots, zero IVs, no held item and original PP\n");

    // Baily is a distinct, approved ordinary Vermilion Gym Trainer, stacked
    // after Tucker without modifying Tucker's native acceptance. Both use
    // genuine production CreateNPCTrainerParty and normal level-up moves.
    core->busWrite32(core, SYM_gBattleTypeFlags, BATTLE_TYPE_TRAINER);
    CHECK(CALL(CreateNPCTrainerParty, SYM_gEnemyParty,
               TRAINER_ENGINEER_BAILY) == 2);
    const unsigned bailySpecies[] = {SPECIES_PICHU, SPECIES_FLAAFFY};
    const unsigned bailyLevels[] = {8, 22};
    const unsigned bailyLegalMoves[][4] = {
        {MOVE_THUNDER_SHOCK, MOVE_CHARM, MOVE_TAIL_WHIP, MOVE_THUNDER_WAVE},
        {MOVE_TACKLE, MOVE_GROWL, MOVE_THUNDER_SHOCK, MOVE_THUNDER_WAVE},
    };
    for (unsigned i = 0; i < 2; i++) {
        const unsigned mon = enemy(i);
        CHECK(monget(mon, ABI_DATA_SPECIES) == bailySpecies[i]);
        CHECK(monget(mon, ABI_DATA_LEVEL) == bailyLevels[i]);
        CHECK(monget(mon, ABI_DATA_HELD_ITEM) == ITEM_NONE);
        for (unsigned iv = 0; iv < 6; iv++)
            CHECK(monget(mon, ABI_DATA_HP_IV + iv) == 0);
        for (unsigned slot = 0; slot < 4; slot++) {
            const unsigned move = monget(mon, ABI_DATA_MOVE1 + slot);
            unsigned matches = 0;
            for (unsigned legal = 0; legal < 4; legal++)
                if (move == bailyLegalMoves[i][legal])
                    matches++;
            CHECK(matches == 1);
            CHECK(monget(mon, ABI_DATA_PP1 + slot) ==
                  core->busRead8(core,
                      SYM_gBattleMoves + ABI_BMOVE_SIZE * move + ABI_BMOVE_PP));
        }
        unsigned waveCount = 0;
        for (unsigned slot = 0; slot < 4; slot++)
            if (monget(mon, ABI_DATA_MOVE1 + slot) == MOVE_THUNDER_WAVE)
                waveCount++;
        CHECK(waveCount == 1);
    }
    printf("PASS: native Vermilion Baily Pichu8/Flaaffy22, natural Thunder Wave, legal moves/PP, zero IVs and no held items\n");
    printf("PASS: native ROM constructors for %u ordinary parties, %u Pokémon, "
           "species/level/IV/held-item/moves/PP; ten dummy source records excluded\n",
           ORDINARY_TRAINER_COUNT, index);
    CHECK(core->loadState(core, bootState));
}
