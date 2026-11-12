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
    // Erika's separate approved leader data are outside the 500-party
    // ordinary Trainer lock. Verify the real ROM party constructor.
    static const struct {
        unsigned species, level, item, moves[4];
    } erikaApproved[] = {
        {SPECIES_BELLOSSOM,29,ITEM_NONE,{MOVE_SOLAR_BEAM,MOVE_SYNTHESIS,MOVE_SLEEP_POWDER,MOVE_MAGICAL_LEAF}},
        {SPECIES_RAPIDASH,30,ITEM_NONE,{MOVE_FLAME_WHEEL,MOVE_FLASHFIRE,MOVE_STOMP,MOVE_TAKE_DOWN}},
        {SPECIES_NINETALES,31,ITEM_NONE,{MOVE_FIRE_SPIN,MOVE_ASTONISH,MOVE_CONFUSE_RAY,MOVE_WILL_O_WISP}},
        {SPECIES_VILEPLUME,33,ITEM_SITRUS_BERRY,{MOVE_SLUDGE_BOMB,MOVE_MEGA_DRAIN,MOVE_SOLAR_BEAM,MOVE_MOONLIGHT}},
    };
    core->busWrite32(core, SYM_gBattleTypeFlags, BATTLE_TYPE_TRAINER);
    CHECK(CALL(CreateNPCTrainerParty, SYM_gEnemyParty, TRAINER_LEADER_ERIKA) == 4);
    for (unsigned i = 0; i < 4; i++) {
        unsigned mon = enemy(i);
        CHECK(monget(mon, ABI_DATA_SPECIES) == erikaApproved[i].species);
        CHECK(monget(mon, ABI_DATA_LEVEL) == erikaApproved[i].level);
        CHECK(monget(mon, ABI_DATA_HELD_ITEM) == erikaApproved[i].item);
        for (unsigned iv = 0; iv < 6; iv++)
            CHECK(monget(mon, ABI_DATA_HP_IV + iv) == 0);
        for (unsigned slot = 0; slot < 4; slot++) {
            unsigned move = monget(mon, ABI_DATA_MOVE1 + slot);
            CHECK(move == erikaApproved[i].moves[slot]);
            CHECK(monget(mon, ABI_DATA_PP1 + slot) ==
                  core->busRead8(core,
                      SYM_gBattleMoves + ABI_BMOVE_SIZE * move + ABI_BMOVE_PP));
        }
    }
    printf("PASS: native Erika four exact leader Pokemon, Lv29/30/31/33, sixteen moves, original IVs, Sitrus ace and actual PP\n");
    printf("PASS: native ROM constructors for %u ordinary parties, %u Pokémon, "
           "species/level/IV/held-item/moves/PP; ten dummy source records excluded\n",
           ORDINARY_TRAINER_COUNT, index);
    CHECK(core->loadState(core, bootState));
}
