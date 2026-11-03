#include "global.h"
#include "gflib.h"
#include "event_data.h"
#include "money.h"
#include "mystery_vendor.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "pokedex.h"
#include "random.h"
#include "constants/flags.h"

static const u16 sVendorFlags[MYSTERY_VENDOR_COUNT] = {
    FLAG_MYSTERY_VENDOR_01, FLAG_MYSTERY_VENDOR_02, FLAG_MYSTERY_VENDOR_03,
    FLAG_MYSTERY_VENDOR_04, FLAG_MYSTERY_VENDOR_05, FLAG_MYSTERY_VENDOR_06,
    FLAG_MYSTERY_VENDOR_07, FLAG_MYSTERY_VENDOR_08, FLAG_MYSTERY_VENDOR_09,
    FLAG_MYSTERY_VENDOR_10,
};
static const u16 sSpecies[MYSTERY_VENDOR_COUNT] = {
    SPECIES_BULBASAUR, SPECIES_SQUIRTLE, SPECIES_CHARMANDER,
    SPECIES_KANGASKHAN, SPECIES_PINSIR, SPECIES_FEEBAS, SPECIES_CHANSEY,
    SPECIES_DRATINI, SPECIES_ABSOL, SPECIES_MAGIKARP,
};
static const u8 sMysteryName[] = _("Mystery");

// Rejection of the incomplete top interval makes each result equally likely.
u16 MysteryRandomBelow(u16 count)
{
    u32 limit;
    u16 value;
    if (count == 0)
        return 0;
    limit = 65536 / count * count;
    do { value = Random(); } while (value >= limit);
    return value % count;
}

static bool8 MysteryStateValid(void)
{
    u32 i, sellers = 0, species = 0;
    for (i = 0; i < MYSTERY_VENDOR_COUNT; i++)
    {
        sellers += FlagGet(sVendorFlags[i]);
        species += FlagGet(FLAG_MYSTERY_SPECIES_00 + i);
    }
    // Legacy Route4 bought exactly Magikarp, not a new random network species.
    if (sellers == 1 && species == 0 && FlagGet(FLAG_BOUGHT_MAGIKARP))
    {
        FlagSet(FLAG_MYSTERY_SPECIES_09);
        species = 1;
    }
    return sellers == species;
}

u16 MysteryVendorStatus(void)
{
    u16 vendor = gSpecialVar_0x8009;
    if (vendor >= MYSTERY_VENDOR_COUNT || !MysteryStateValid())
        return MYSTERY_VENDOR_INVALID;
    return FlagGet(sVendorFlags[vendor]) ? MYSTERY_VENDOR_SOLD : 0;
}

static bool8 MysteryHasSpace(void)
{
    u32 box, slot;
    for (slot = 0; slot < PARTY_SIZE; slot++)
        if (GetMonData(&gPlayerParty[slot], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
            return TRUE;
    for (box = 0; box < TOTAL_BOXES_COUNT; box++)
        for (slot = 0; slot < IN_BOX_COUNT; slot++)
            if (GetBoxMonDataAt(box, slot, MON_DATA_SPECIES) == SPECIES_NONE)
                return TRUE;
    return FALSE;
}

u16 PurchaseMysteryPokemon(void)
{
    struct Pokemon *mon;
    struct BoxPokemon *delivered;
    u16 remaining[MYSTERY_VENDOR_COUNT], count = 0, choice, species, dex;
    u16 vendor = gSpecialVar_0x8009;
    u16 result = MysteryVendorStatus();
    u32 i, pid, ot;
    u8 gender = MALE;
    bool8 shiny;
    if (result != 0)
        return result;
    if (!IsEnoughMoney(&gSaveBlock1Ptr->money, MYSTERY_VENDOR_PRICE))
        return MYSTERY_VENDOR_POOR;
    if (!MysteryHasSpace())
        return MON_CANT_GIVE;
    mon = AllocZeroed(sizeof(*mon));
    if (mon == NULL)
        return MON_CANT_GIVE;
    for (i = 0; i < MYSTERY_VENDOR_COUNT; i++)
        if (!FlagGet(FLAG_MYSTERY_SPECIES_00 + i))
            remaining[count++] = i;
    if (count == 0)
    {
        Free(mon);
        return MYSTERY_VENDOR_INVALID;
    }
    choice = remaining[MysteryRandomBelow(count)];
    species = sSpecies[choice];
    ot = ~((u32)gSaveBlock2Ptr->playerTrainerId[0]
        | ((u32)gSaveBlock2Ptr->playerTrainerId[1] << 8)
        | ((u32)gSaveBlock2Ptr->playerTrainerId[2] << 16)
        | ((u32)gSaveBlock2Ptr->playerTrainerId[3] << 24));
    shiny = MysteryRandomBelow(10) < 9;
    if (shiny)
    {
        u32 low = Random();
        pid = low | (((ot >> 16) ^ (ot & 0xFFFF) ^ low ^ MysteryRandomBelow(8)) << 16);
    }
    else
    {
        do { pid = Random(); pid |= (u32)Random() << 16; }
        while (GET_SHINY_VALUE(ot, pid) < SHINY_ODDS);
    }
    // Native delivery assumes player-keyed secure data. Keep it valid until
    // delivery, then rekey the actual record to the final network identity.
    CreateMon(mon, species, 5, 32, TRUE, pid, OT_ID_PLAYER_ID, 0);
    result = GiveMonToPlayer(mon);
    Free(mon);
    if (result != MON_GIVEN_TO_PARTY && result != MON_GIVEN_TO_PC)
        return MON_CANT_GIVE;
    // Native delivery replaces OT with player data. Restore the foreign identity
    // on the actual delivered record before committing or exposing any UI.
    if (result == MON_GIVEN_TO_PARTY)
        delivered = &gPlayerParty[gPlayerPartyCount - 1].box;
    else
        delivered = GetBoxedMonPtr(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos);
    // OT_ID is a header field: SetBoxMonData does NOT re-encrypt it. XOR
    // old/new OT keys over the secure words; plaintext/checksum/PID stay intact.
    for (i = 0; i < ARRAY_COUNT(delivered->secure.raw); i++)
        delivered->secure.raw[i] ^= delivered->otId ^ ot;
    SetBoxMonData(delivered, MON_DATA_OT_ID, &ot);
    SetBoxMonData(delivered, MON_DATA_OT_NAME, sMysteryName);
    SetBoxMonData(delivered, MON_DATA_OT_GENDER, &gender);
    RemoveMoney(&gSaveBlock1Ptr->money, MYSTERY_VENDOR_PRICE);
    FlagSet(FLAG_MYSTERY_SPECIES_00 + choice);
    FlagSet(sVendorFlags[vendor]);
    dex = SpeciesToNationalPokedexNum(species);
    GetSetPokedexFlag(dex, FLAG_SET_SEEN);
    GetSetPokedexFlag(dex, FLAG_SET_CAUGHT);
    VarSet(VAR_TEMP_1, species);
    return result;
}
