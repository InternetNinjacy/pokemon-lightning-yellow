#ifndef GUARD_RANGER_BASE_CAMP_H
#define GUARD_RANGER_BASE_CAMP_H

struct Pokemon;

void ApplyRangerTrainerData(struct Pokemon *mon, u16 trainerId, u8 slot);
u16 RangerGiveGift(void);
bool8 RangerNeedsMapRefresh(void);

#endif
