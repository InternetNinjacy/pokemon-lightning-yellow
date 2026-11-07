#include "global.h"
#include "gflib.h"
#include "event_data.h"
#include "item.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "pokedex.h"
#include "random.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/vars.h"

// Reserved save-backed variables contain the issued Pokémon's actual personality.
// Together with the fixed foreign OT ID, OT name and evolution-family check,
// this survives trade, evolution, renaming, boxes and ordinary save/load.
static const u16 sCollectedFlags[] = {FLAG_ROCKET_RALTS_COLLECTED, FLAG_ROCKET_SNORUNT_COLLECTED};
static const u16 sReturnedFlags[] = {FLAG_ROCKET_RALTS_RETURNED, FLAG_ROCKET_SNORUNT_RETURNED};
static const u16 sPidLowVars[] = {VAR_ROCKET_RALTS_PID_LOW, VAR_ROCKET_SNORUNT_PID_LOW};
static const u16 sPidHighVars[] = {VAR_ROCKET_RALTS_PID_HIGH, VAR_ROCKET_SNORUNT_PID_HIGH};
static const u16 sSpecies[] = {SPECIES_RALTS, SPECIES_SNORUNT};
static const u16 sRewards[] = {ITEM_RARE_CANDY, ITEM_NEVER_MELT_ICE};
static const u32 sOtIds[] = {0x7AC10459, 0x4C3908B7};
static const u8 sPaige[] = _("PAIGE");
static const u8 sJulia[] = _("JULIA");
static const u8 *const sOtNames[] = {sPaige, sJulia};

// Script result codes: availability 0=not acquired, 1=missing,
// 2=found, 3=already returned; gift 0=party, 1=PC, 2=unavailable,
// return 0=success, 1=missing, 2=bag full, 3=only party Pokémon.
static bool8 IsFamily(u16 quest, u16 species)
{
    if (quest == 0)
        return species == SPECIES_RALTS || species == SPECIES_KIRLIA || species == SPECIES_GARDEVOIR;
    return species == SPECIES_SNORUNT || species == SPECIES_GLALIE;
}

static u32 GetIssuedPersonality(u16 quest)
{
    return (u32)VarGet(sPidLowVars[quest]) | ((u32)VarGet(sPidHighVars[quest]) << 16);
}

static bool8 MatchesBox(u16 quest, struct BoxPokemon *mon)
{
    u8 name[PLAYER_NAME_LENGTH + 1];
    if (!IsFamily(quest, GetBoxMonData(mon, MON_DATA_SPECIES, NULL)))
        return FALSE;
    if (GetBoxMonData(mon, MON_DATA_OT_ID, NULL) != sOtIds[quest]
     || GetBoxMonData(mon, MON_DATA_PERSONALITY, NULL) != GetIssuedPersonality(quest))
        return FALSE;
    GetBoxMonData(mon, MON_DATA_OT_NAME, name);
    return StringCompare(name, sOtNames[quest]) == 0;
}

static bool8 FindIssuedMon(u16 quest, bool8 *inParty, u8 *box, u8 *slot)
{
    u32 i, j;
    for (i = 0; i < PARTY_SIZE; i++)
        if (MatchesBox(quest, &gPlayerParty[i].box))
        {
            *inParty = TRUE;
            *slot = i;
            return TRUE;
        }
    for (i = 0; i < TOTAL_BOXES_COUNT; i++)
        for (j = 0; j < IN_BOX_COUNT; j++)
            if (MatchesBox(quest, GetBoxedMonPtr(i, j)))
            {
                *inParty = FALSE;
                *box = i;
                *slot = j;
                return TRUE;
            }
    return FALSE;
}

u16 RocketRescueStatus(void)
{
    u16 quest = gSpecialVar_0x8009;
    bool8 inParty;
    u8 box, slot;
    if (quest >= ARRAY_COUNT(sSpecies))
        return 1;
    if (FlagGet(sReturnedFlags[quest]))
        return 3;
    if (!FlagGet(sCollectedFlags[quest]))
        return 0;
    return FindIssuedMon(quest, &inParty, &box, &slot) ? 2 : 1;
}

u16 RocketRescueGive(void)
{
    u16 quest = gSpecialVar_0x8009, result, dex;
    struct Pokemon *mon;
    struct BoxPokemon *delivered;
    u8 gender = FEMALE;
    u32 i, pid, ot, oldOt;
    if (quest >= ARRAY_COUNT(sSpecies) || FlagGet(sCollectedFlags[quest]))
        return 2;

    // Avoid attempting ordinary delivery when every possible destination is full.
    if (gPlayerPartyCount == PARTY_SIZE)
    {
        bool8 freeSpace = FALSE;
        u32 box, slot;
        for (box = 0; box < TOTAL_BOXES_COUNT && !freeSpace; box++)
            for (slot = 0; slot < IN_BOX_COUNT; slot++)
                if (GetBoxMonDataAt(box, slot, MON_DATA_SPECIES) == SPECIES_NONE)
                {
                    freeSpace = TRUE;
                    break;
                }
        if (!freeSpace)
            return 2;
    }

    mon = AllocZeroed(sizeof(*mon));
    if (mon == NULL)
        return 2;
    ot = sOtIds[quest];
    do
    {
        pid = (u32)Random() | ((u32)Random() << 16);
    } while (GET_SHINY_VALUE(ot, pid) < SHINY_ODDS);

    // Use the battle-tested Mystery Vendor delivery/rekey path, not a direct
    // foreign-OT CreateMon followed by an OT-overwriting GiveMonToPlayer.
    CreateMon(mon, sSpecies[quest], 27, 32, TRUE, pid, OT_ID_PLAYER_ID, 0);
    result = GiveMonToPlayer(mon);
    Free(mon);
    if (result != MON_GIVEN_TO_PARTY && result != MON_GIVEN_TO_PC)
        return 2;

    if (result == MON_GIVEN_TO_PARTY)
        delivered = &gPlayerParty[gPlayerPartyCount - 1].box;
    else
        delivered = GetBoxedMonPtr(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos);
    oldOt = delivered->otId;
    for (i = 0; i < ARRAY_COUNT(delivered->secure.raw); i++)
        delivered->secure.raw[i] ^= oldOt ^ ot;
    SetBoxMonData(delivered, MON_DATA_OT_ID, &ot);
    SetBoxMonData(delivered, MON_DATA_OT_NAME, sOtNames[quest]);
    SetBoxMonData(delivered, MON_DATA_OT_GENDER, &gender);

    // Store identity and commit collection only after insertion succeeded.
    VarSet(sPidLowVars[quest], pid & 0xFFFF);
    VarSet(sPidHighVars[quest], pid >> 16);
    FlagSet(sCollectedFlags[quest]);
    dex = SpeciesToNationalPokedexNum(sSpecies[quest]);
    GetSetPokedexFlag(dex, FLAG_SET_SEEN);
    GetSetPokedexFlag(dex, FLAG_SET_CAUGHT);
    return result;
}

u16 RocketRescueReturn(void)
{
    u16 quest = gSpecialVar_0x8009;
    bool8 inParty;
    u8 box = 0, slot = 0, i;
    if (quest >= ARRAY_COUNT(sSpecies) || !FlagGet(sCollectedFlags[quest])
     || FlagGet(sReturnedFlags[quest])
     || !FindIssuedMon(quest, &inParty, &box, &slot))
        return 1;
    if (inParty && gPlayerPartyCount <= 1)
        return 3;
    if (!CheckBagHasSpace(sRewards[quest], 1))
        return 2;

    // No script yield or save can occur within this transaction. Award only
    // after all guards, then remove precisely the saved-identity Pokémon.
    if (!AddBagItem(sRewards[quest], 1))
        return 2;
    if (inParty)
    {
        for (i = slot; i + 1 < gPlayerPartyCount; i++)
            gPlayerParty[i] = gPlayerParty[i + 1];
        ZeroMonData(&gPlayerParty[gPlayerPartyCount - 1]);
        CalculatePlayerPartyCount();
    }
    else
        ZeroBoxMonData(GetBoxedMonPtr(box, slot));
    FlagSet(sReturnedFlags[quest]);
    return 0;
}
