#include "global.h"
#include "pokemon.h"
#include "custom_pokedex.h"
#include "follower_state.h"
#include "constants/species.h"

// Versioned record uses only 16 of SaveBlock1's existing 64-byte unused
// region; serialized Pokemon, boxes, day care and all save offsets stay intact.
// Audited against current main: unused_3A94 has no other source claimant.
// The remaining48 bytes are untouched; this does not relocate any save field.
#define FOLLOWER_STATE_MAGIC 0x46574C59 // 'FWLY'
#define FOLLOWER_STATE_VERSION 1

struct FollowerSavedSelection
{
    u32 magic;
    u32 personality;
    u32 otId;
    u16 integrity;
    u8 version;
    u8 selected;
};

STATIC_ASSERT(sizeof(struct FollowerSavedSelection) == 16, FollowerRecordSize);
STATIC_ASSERT(sizeof(((struct SaveBlock1 *)0)->unused_3A94) >= sizeof(struct FollowerSavedSelection), FollowerRecordFits);

static struct FollowerSavedSelection *FollowerState_Record(void)
{
    if (gSaveBlock1Ptr == NULL)
        return NULL;
    return (struct FollowerSavedSelection *)gSaveBlock1Ptr->unused_3A94;
}

static u16 FollowerState_Integrity(const struct FollowerSavedSelection *record)
{
    u32 value = record->magic ^ record->personality ^ record->otId
              ^ ((u32)record->version << 24) ^ ((u32)record->selected << 16);
    return (u16)(value ^ (value >> 16) ^ 0xA59C);
}

static bool8 FollowerState_IsValid(const struct FollowerSavedSelection *record)
{
    return record != NULL
        && record->magic == FOLLOWER_STATE_MAGIC
        && record->version == FOLLOWER_STATE_VERSION
        && record->selected == 1
        && record->integrity == FollowerState_Integrity(record);
}

void FollowerState_Clear(void)
{
    struct FollowerSavedSelection *record = FollowerState_Record();
    // A zeroed/unknown record is unassigned; never convert it to lead.
    if (record != NULL)
        memset(record, 0, sizeof(*record));
}

static bool8 FollowerState_IsEligibleMon(struct Pokemon *mon)
{
    return !GetMonData(mon, MON_DATA_SANITY_IS_BAD_EGG)
        && !GetMonData(mon, MON_DATA_IS_EGG)
        && IsSpeciesInCustomPokedex(GetMonData(mon, MON_DATA_SPECIES));
}

bool8 FollowerState_CanSelectPartySlot(u8 partySlot)
{
    struct Pokemon *mon;
    u32 personality, otId;
    u8 i, matches = 0;

    if (FollowerState_Record() == NULL
     || partySlot >= PARTY_SIZE || partySlot >= gPlayerPartyCount)
        return FALSE;
    mon = &gPlayerParty[partySlot];
    if (!FollowerState_IsEligibleMon(mon))
        return FALSE;

    personality = GetMonData(mon, MON_DATA_PERSONALITY);
    otId = GetMonData(mon, MON_DATA_OT_ID);
    // PID/OT pairs distinguish native instances and survive party reordering,
    // nicknames and evolution. Exact cloned pairs are deliberately ambiguous.
    for (i = 0; i < gPlayerPartyCount && i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) != SPECIES_NONE
         && GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY) == personality
         && GetMonData(&gPlayerParty[i], MON_DATA_OT_ID) == otId)
            matches++;
    }
    return matches == 1;
}

bool8 FollowerState_SelectPartySlot(u8 partySlot)
{
    struct FollowerSavedSelection *record;
    struct Pokemon *mon;
    u32 personality, otId;

    if (!FollowerState_CanSelectPartySlot(partySlot))
        return FALSE;
    mon = &gPlayerParty[partySlot];
    personality = GetMonData(mon, MON_DATA_PERSONALITY);
    otId = GetMonData(mon, MON_DATA_OT_ID);

    record = FollowerState_Record();
    record->magic = FOLLOWER_STATE_MAGIC;
    record->personality = personality;
    record->otId = otId;
    record->version = FOLLOWER_STATE_VERSION;
    record->selected = 1;
    record->integrity = FollowerState_Integrity(record);
    return TRUE;
}

s8 FollowerState_ResolvePartySlot(void)
{
    struct FollowerSavedSelection *record = FollowerState_Record();
    s8 result = -1;
    u8 i;
    if (!FollowerState_IsValid(record))
        return -1;

    for (i = 0; i < gPlayerPartyCount && i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gPlayerParty[i];
        if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE
         && GetMonData(mon, MON_DATA_PERSONALITY) == record->personality
         && GetMonData(mon, MON_DATA_OT_ID) == record->otId)
        {
            if (result != -1)
            {
                // The match is now ambiguous. Do not silently transfer to a clone.
                FollowerState_Clear();
                return -1;
            }
            result = i;
        }
    }
    // Missing means deposited/traded/released, not temporarily fainted.
    if (result == -1 || !FollowerState_IsEligibleMon(&gPlayerParty[result]))
    {
        FollowerState_Clear();
        return -1;
    }
    return result;
}

bool8 FollowerState_IsSelectedPartySlot(u8 partySlot)
{
    return partySlot < PARTY_SIZE && FollowerState_ResolvePartySlot() == partySlot;
}

bool8 FollowerState_CanDisplay(void)
{
    s8 slot = FollowerState_ResolvePartySlot();
    return slot >= 0 && GetMonData(&gPlayerParty[slot], MON_DATA_HP) != 0;
}

void FollowerState_BeforeBoxDeposit(struct Pokemon *mon)
{
    struct FollowerSavedSelection *record = FollowerState_Record();
    if (mon != NULL && FollowerState_IsValid(record)
     && GetMonData(mon, MON_DATA_PERSONALITY) == record->personality
     && GetMonData(mon, MON_DATA_OT_ID) == record->otId)
        FollowerState_Clear();
}

void FollowerState_BeforePartyRemoval(u8 partySlot)
{
    if (FollowerState_IsSelectedPartySlot(partySlot))
        FollowerState_Clear();
}
