#include "global.h"
#include "follower_state.h"
#include "follower_controller.h"
#include "gflib.h"
#include "daycare.h"
#include "event_data.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
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

// Keep the experimental opening follower selection independent of nicknaming.
// Native givemon and the naming editor retain the player's ownership/name.
void ScriptSelectStarterPikachuFollower(void)
{
    if (gPlayerPartyCount != 0 && GetMonData(&gPlayerParty[0], MON_DATA_SPECIES) == SPECIES_PIKACHU)
    {
        if (FollowerState_SelectPartySlot(0))
            FollowerController_RequestRefresh();
    }
}

// One-time Oak parcel gift, sharing the proven Tangela party/PC path.
// Eevee has Run Away as its only current source ability.
u16 ScriptGiveSueEevee(void)
{
    static const u8 nickname[] = _("SUE");
    static const u8 oakName[] = _("OAK");
    struct Pokemon *mon = AllocZeroed(sizeof(*mon));
    struct BoxPokemon *delivered;
    u32 playerOtId, oakOtId, i;
    u8 gender = MALE;
    u16 result;
    if (mon == NULL)
        return MON_CANT_GIVE;
    // Oak's gift must be foreign-OT for native traded EXP and badge obedience.
    // No canonical Oak numeric trainer ID exists in this gift contract: derive
    // a stable, guaranteed non-player ID from the save's player trainer ID.
    // The bitwise complement also handles a player whose name is OAK.
    playerOtId = (u32)gSaveBlock2Ptr->playerTrainerId[0]
               | ((u32)gSaveBlock2Ptr->playerTrainerId[1] << 8)
               | ((u32)gSaveBlock2Ptr->playerTrainerId[2] << 16)
               | ((u32)gSaveBlock2Ptr->playerTrainerId[3] << 24);
    oakOtId = ~playerOtId;

    // As with Mystery Vendor and Rocket rescue gifts, native delivery must
    // receive player-keyed data before the actual party/PC record is rekeyed.
    CreateMon(mon, SPECIES_EEVEE, 5, USE_RANDOM_IVS, FALSE, 0, OT_ID_PLAYER_ID, 0);
    SetMonData(mon, MON_DATA_NICKNAME, nickname);
    result = GiveMonToPlayer(mon);
    if (result == MON_GIVEN_TO_PARTY || result == MON_GIVEN_TO_PC)
    {
        u16 dex = SpeciesToNationalPokedexNum(SPECIES_EEVEE);
        if (result == MON_GIVEN_TO_PARTY)
            delivered = &gPlayerParty[gPlayerPartyCount - 1].box;
        else
            delivered = GetBoxedMonPtr(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos);
        // OT_ID is a header field; its setter does not re-encrypt secure data.
        for (i = 0; i < ARRAY_COUNT(delivered->secure.raw); i++)
            delivered->secure.raw[i] ^= delivered->otId ^ oakOtId;
        SetBoxMonData(delivered, MON_DATA_OT_ID, &oakOtId);
        SetBoxMonData(delivered, MON_DATA_OT_NAME, oakName);
        SetBoxMonData(delivered, MON_DATA_OT_GENDER, &gender);
        GetSetPokedexFlag(dex, FLAG_SET_SEEN);
        GetSetPokedexFlag(dex, FLAG_SET_CAUGHT);
    }
    Free(mon);
    return result;
}
