#ifndef GUARD_FOLLOWER_STATE_H
#define GUARD_FOLLOWER_STATE_H

#include "global.h"

// Save-backed assignment only. Visual suppression (faint, bike, Surf,
// battle and resource pressure) must never mutate this selection.
bool8 FollowerState_SelectPartySlot(u8 partySlot);
void FollowerState_Clear(void);
s8 FollowerState_ResolvePartySlot(void);
bool8 FollowerState_IsSelectedPartySlot(u8 partySlot);
bool8 FollowerState_CanDisplay(void);
// Call immediately before removing a selected party instance from the party.
// Never call on unrelated deposits or simple party reordering.
void FollowerState_BeforePartyRemoval(u8 partySlot);

#endif // GUARD_FOLLOWER_STATE_H
