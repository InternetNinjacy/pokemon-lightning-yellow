#include "global.h"
#include "rtc.h"
#include "time_of_day.h"

bool8 TryGetTimeOfDay(u8 *timeOfDay)
{
    u8 hour;
    if (!RtcGetHour(&hour) || hour >= 24)
        return FALSE;
    *timeOfDay = hour >= DAY_START_HOUR && hour < NIGHT_START_HOUR
              ? TIME_OF_DAY_DAY : TIME_OF_DAY_NIGHT;
    return TRUE;
}

u8 GetTimeOfDay(void)
{
    u8 timeOfDay = TIME_OF_DAY_DAY;
    TryGetTimeOfDay(&timeOfDay);
    return timeOfDay;
}
