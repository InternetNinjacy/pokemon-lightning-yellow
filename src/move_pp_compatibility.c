#include "global.h"
#include "move_pp_compatibility.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "route5_daycare.h"
#include "constants/moves.h"

void ClampApprovedDaycarePP(struct DayCare *daycare)
{
    u8 slot;
    if (daycare == NULL)
        return;
    for (slot = 0; slot < DAYCARE_MON_COUNT; slot++)
        ClampApprovedMovePP(&daycare->mons[slot].mon);
}

void NormalizeLoadedMovePP(void)
{
    u16 box, slot;
    struct Route5DaycareSave *route5 = &gSaveBlock1Ptr->route5Daycare;

    for (slot = 0; slot < PARTY_SIZE; slot++)
        if (ClampApprovedMovePP(&gPlayerParty[slot].box))
            gSaveBlock1Ptr->playerParty[slot].box = gPlayerParty[slot].box;
    for (box = 0; box < TOTAL_BOXES_COUNT; box++)
        for (slot = 0; slot < IN_BOX_COUNT; slot++)
            ClampApprovedMovePP(&gPokemonStoragePtr->boxes[box][slot]);
    ClampApprovedDaycarePP(&gSaveBlock1Ptr->daycare);
    // Do not eagerly import legacy storage. Its existing lazy migration performs
    // the same clamp on the active copy, never on the forensic recovery record.
    if (route5->magic == ROUTE5_DAYCARE_MAGIC && route5->version == ROUTE5_DAYCARE_VERSION)
        ClampApprovedDaycarePP(GetRoute5Daycare());
}
