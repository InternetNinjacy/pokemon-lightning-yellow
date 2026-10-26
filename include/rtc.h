#ifndef GUARD_RTC_H
#define GUARD_RTC_H

#include "global.h"
// Raw cartridge clock, no save offset. FALSE means no usable clock reading.
bool8 RtcGetHour(u8 *hour);
#endif
