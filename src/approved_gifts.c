#include "global.h"
#include "gflib.h"
#include "daycare.h"
#include "pokedex.h"
#include "pokemon.h"
#include "constants/species.h"

// Only the approved Water Labyrinth gift; never deliver an Egg to the PC.
u16 ScriptGiveMagbyEgg(void)
{
    struct Pokemon *mon;
    u16 result;
    if (gPlayerPartyCount >= PARTY_SIZE)
        return MON_CANT_GIVE;
    mon = AllocZeroed(sizeof(*mon));
    if (mon == NULL)
        return MON_CANT_GIVE;
    CreateHalfShinyGiftEgg(mon, SPECIES_MAGBY);
    result = GiveMonToPlayer(mon);
    Free(mon);
    return result;
}

u16 ScriptGiveTangoTangela(void)
{
    static const u8 nickname[] = _("TANGO");
    struct Pokemon *mon = AllocZeroed(sizeof(*mon));
    u16 result;
    if (mon == NULL)
        return MON_CANT_GIVE;
    CreateMon(mon, SPECIES_TANGELA, 25, USE_RANDOM_IVS, FALSE, 0, OT_ID_PLAYER_ID, 0);
    SetMonData(mon, MON_DATA_NICKNAME, nickname);
    result = GiveMonToPlayer(mon);
    if (result != MON_CANT_GIVE)
    {
        u16 dex = SpeciesToNationalPokedexNum(SPECIES_TANGELA);
        GetSetPokedexFlag(dex, FLAG_SET_SEEN);
        GetSetPokedexFlag(dex, FLAG_SET_CAUGHT);
    }
    Free(mon);
    return result;
}

// One-time Oak parcel gift, sharing the proven Tangela party/PC path.
// Eevee has Run Away as its only current source ability.
u16 ScriptGiveSueEevee(void)
{
    static const u8 nickname[] = _("SUE");
    struct Pokemon *mon = AllocZeroed(sizeof(*mon));
    u16 result;
    if (mon == NULL)
        return MON_CANT_GIVE;
    // Oak's gift must be foreign-OT for native traded EXP and badge obedience.
    // No canonical Oak numeric trainer ID exists in this gift contract: derive
    // a stable, guaranteed non-player ID from the save's player trainer ID.
    // The bitwise complement also handles a player whose name is OAK.
    u32 playerOtId = gSaveBlock2Ptr->playerTrainerId[0]
                   | (gSaveBlock2Ptr->playerTrainerId[1] << 8)
                   | (gSaveBlock2Ptr->playerTrainerId[2] << 16)
                   | (gSaveBlock2Ptr->playerTrainerId[3] << 24);
    u32 oakOtId = ~playerOtId;
    static const u8 oakName[] = _("OAK");

    CreateMon(mon, SPECIES_EEVEE, 5, USE_RANDOM_IVS, FALSE, 0, OT_ID_PRESET, oakOtId);
    SetMonData(mon, MON_DATA_OT_NAME, oakName);
    SetMonData(mon, MON_DATA_NICKNAME, nickname);
    result = GiveMonToPlayer(mon);
    if (result != MON_CANT_GIVE)
    {
        u16 dex = SpeciesToNationalPokedexNum(SPECIES_EEVEE);
        GetSetPokedexFlag(dex, FLAG_SET_SEEN);
        GetSetPokedexFlag(dex, FLAG_SET_CAUGHT);
    }
    Free(mon);
    return result;
}
