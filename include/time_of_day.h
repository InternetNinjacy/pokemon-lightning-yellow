#ifndef GUARD_TIME_OF_DAY_H
#define GUARD_TIME_OF_DAY_H

#include "global.h"
#include "constants/time_of_day.h"

// Functional periods, independent of any future visual dawn/dusk transitions.
// FALSE leaves the output untouched: timed evolution must fail closed.
bool8 TryGetTimeOfDay(u8 *timeOfDay);
// Environmental callers may use the approved DAY fallback on unavailable RTC.
u8 GetTimeOfDay(void);

#endif
