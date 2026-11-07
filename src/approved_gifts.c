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

// WRL-001: fixed nickname for the approved Pikachu starter; gift is still made
// by FireRed's original givemon script, so its party/ownership behavior stays intact.
void ScriptNameSocketStarterPikachu(void)
{
    static const u8 nickname[] = _("SOCKET");

    if (gPlayerPartyCount != 0 && GetMonData(&gPlayerParty[0], MON_DATA_SPECIES) == SPECIES_PIKACHU)
        SetMonData(&gPlayerParty[0], MON_DATA_NICKNAME, nickname);
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
    CreateMon(mon, SPECIES_EEVEE, 5, USE_RANDOM_IVS, FALSE, 0, OT_ID_PLAYER_ID, 0);
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
