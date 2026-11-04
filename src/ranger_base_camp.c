#include "global.h"
#include "gflib.h"
#include "event_data.h"
#include "pokemon.h"
#include "pokedex.h"
#include "ranger_base_camp.h"
#include "constants/abilities.h"
#include "constants/flags.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/opponents.h"

// Trainer-specific settings only. Species data and ordinary creation stay native.
static const u8 sRangerAbilities[5][4] = {
    {ABILITY_NONE, ABILITY_INTIMIDATE},
    {ABILITY_KEEN_EYE, ABILITY_INSOMNIA},
    {ABILITY_CLOUD_NINE, ABILITY_STATIC},
    {ABILITY_FLASH_FIRE, ABILITY_INTIMIDATE},
    {ABILITY_INSOMNIA, ABILITY_STATIC, ABILITY_CLOUD_NINE, ABILITY_INTIMIDATE},
};

void ApplyRangerTrainerData(struct Pokemon *mon, u16 trainerId, u8 slot)
{
    u16 species;
    u8 ability, abilityNum, friendship = MAX_FRIENDSHIP;
    if (trainerId < TRAINER_RANGER_CAMP_1 || trainerId > TRAINER_RANGER_PARK || slot >= ARRAY_COUNT(sRangerAbilities[0]))
        return;
    ability = sRangerAbilities[trainerId - TRAINER_RANGER_CAMP_1][slot];
    if (ability != ABILITY_NONE)
    {
        species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        if (gSpeciesInfo[species].abilities[0] == ability)
            abilityNum = 0;
        else if (gSpeciesInfo[species].abilities[1] == ability)
            abilityNum = 1;
        else
            return; // Never invent an ability outside approved species data.
        SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);
    }
    if ((trainerId == TRAINER_RANGER_CAMP_1 || trainerId == TRAINER_RANGER_CAMP_2) && slot == 0)
        SetMonData(mon, MON_DATA_FRIENDSHIP, &friendship);
}

static const u16 sRangerGiftSpecies[] = {SPECIES_ARCANINE, SPECIES_NOCTOWL, SPECIES_GOLDUCK};
static const u16 sRangerGiftMoves[][MAX_MON_MOVES] = {
    {MOVE_BLAZE_KICK, MOVE_BITE, MOVE_TAKE_DOWN, MOVE_ODOR_SLEUTH},
    {MOVE_EXTRASENSORY, MOVE_HYPNOSIS, MOVE_REFLECT, MOVE_AERIAL_ACE},
    {MOVE_MUDDY_WATER, MOVE_CONFUSION, MOVE_CALM_MIND, MOVE_DISABLE},
};
static const u8 sParkName[PLAYER_NAME_LENGTH + 1] = _("PARK");

u16 RangerGiveGift(void)
{
    struct Pokemon *mon;
    u32 ot;
    u16 species, dex;
    u8 slot, move, gender = MALE;
    u16 choice = gSpecialVar_0x8004;
    if (choice >= ARRAY_COUNT(sRangerGiftSpecies)
     || !FlagGet(TRAINER_FLAGS_START + TRAINER_RANGER_PARK)
     || FlagGet(FLAG_RANGER_CAMP_GIFT_RECEIVED))
        return MON_CANT_GIVE;
    for (slot = 0; slot < PARTY_SIZE; slot++)
        if (GetMonData(&gPlayerParty[slot], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
            break;
    if (slot == PARTY_SIZE)
        return MON_CANT_GIVE; // No RNG, allocation, reward flags or PC delivery.
    mon = AllocZeroed(sizeof(*mon));
    if (mon == NULL)
        return MON_CANT_GIVE;
    species = sRangerGiftSpecies[choice];
    // Complement is collision-safe for every32-bit player ID. Native preset-OT
    // creation encrypts directly under the final key, unlike GiveMonToPlayer.
    ot = ~((u32)gSaveBlock2Ptr->playerTrainerId[0]
        | ((u32)gSaveBlock2Ptr->playerTrainerId[1] << 8)
        | ((u32)gSaveBlock2Ptr->playerTrainerId[2] << 16)
        | ((u32)gSaveBlock2Ptr->playerTrainerId[3] << 24));
    CreateMon(mon, species, 30, USE_RANDOM_IVS, FALSE, 0, OT_ID_PRESET, ot);
    SetMonData(mon, MON_DATA_OT_NAME, sParkName);
    SetMonData(mon, MON_DATA_OT_GENDER, &gender);
    for (move = 0; move < MAX_MON_MOVES; move++)
    {
        SetMonData(mon, MON_DATA_MOVE1 + move, &sRangerGiftMoves[choice][move]);
        SetMonData(mon, MON_DATA_PP1 + move, &gBattleMoves[sRangerGiftMoves[choice][move]].pp);
    }
    CopyMon(&gPlayerParty[slot], mon, sizeof(*mon));
    Free(mon);
    CalculatePlayerPartyCount();
    dex = SpeciesToNationalPokedexNum(species);
    GetSetPokedexFlag(dex, FLAG_SET_SEEN);
    GetSetPokedexFlag(dex, FLAG_SET_CAUGHT);
    FlagSet(FLAG_RANGER_CAMP_GIFT_RECEIVED); // Commit only after real delivery.
    return MON_GIVEN_TO_PARTY;
}

bool8 RangerNeedsMapRefresh(void)
{
    u8 i;
    if (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(MAP_SAFFRON_CITY_DOJO)
     || gSaveBlock1Ptr->location.mapNum != MAP_NUM(MAP_SAFFRON_CITY_DOJO))
        return FALSE;
    for (i = 0; i < gMapHeader.events->objectEventCount; i++)
        if (gSaveBlock1Ptr->objectEventTemplates[i].localId != gMapHeader.events->objectEvents[i].localId
         || gSaveBlock1Ptr->objectEventTemplates[i].graphicsId != gMapHeader.events->objectEvents[i].graphicsId
         || gSaveBlock1Ptr->objectEventTemplates[i].flagId != gMapHeader.events->objectEvents[i].flagId)
            return TRUE;
    return FALSE;
}
