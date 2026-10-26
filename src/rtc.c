#include "global.h"
#include "rtc.h"

// S-3511-compatible GBA cartridge GPIO. Read-only: never reset/set the clock.
#define RTC_DATA      (*(volatile u16 *)0x080000C4)
#define RTC_DIRECTION (*(volatile u16 *)0x080000C6)
#define RTC_CONTROL   (*(volatile u16 *)0x080000C8)
#define RTC_CLK  1
#define RTC_SIO  2
#define RTC_CS   4

// Conventional signature used by emulator cartridge-hardware autodetection.
const char gRtcSignature[] = "SIIRTC_V001";

static void RtcDelay(void)
{
    volatile u32 i;
    for (i = 0; i < 8; i++)
        ;
}

static void RtcWritePins(u16 pins)
{
    RTC_DATA = pins;
    RtcDelay();
}

static void RtcRead(u8 command, u8 *bytes, u32 count)
{
    u32 i, bit;
    RTC_DIRECTION = RTC_CLK | RTC_SIO | RTC_CS;
    RtcWritePins(RTC_CLK);
    RtcWritePins(RTC_CS | RTC_CLK);
    // Command bytes are MSB first; response bytes are LSB first.
    for (i = 0; i < 8; i++)
    {
        u16 data = command & (0x80 >> i) ? RTC_SIO : 0;
        RtcWritePins(RTC_CS | data);
        RtcWritePins(RTC_CS | data | RTC_CLK);
    }
    RTC_DIRECTION = RTC_CLK | RTC_CS;
    for (i = 0; i < count; i++)
    {
        bytes[i] = 0;
        for (bit = 0; bit < 8; bit++)
        {
            RtcWritePins(RTC_CS);
            RtcWritePins(RTC_CS | RTC_CLK);
            if (RTC_DATA & RTC_SIO)
                bytes[i] |= 1 << bit;
        }
    }
    RtcWritePins(RTC_CLK);
    RTC_DIRECTION = RTC_CLK | RTC_SIO | RTC_CS;
}

static bool8 RtcDecodeBcd(u8 bcd, u8 max, u8 *value)
{
    if ((bcd & 15) > 9 || (bcd >> 4) > 9)
        return FALSE;
    *value = (bcd >> 4) * 10 + (bcd & 15);
    return *value <= max;
}

static bool8 RtcDecodeHour(u8 status, const u8 *time, u8 *hour)
{
    u8 minute, second, rawHour;
    if (!RtcDecodeBcd(time[1], 59, &minute) || !RtcDecodeBcd(time[2], 59, &second))
        return FALSE;
    if (status & 0x40) // 24-hour mode
    {
        if (!RtcDecodeBcd(time[0], 23, &rawHour))
            return FALSE;
        *hour = rawHour;
    }
    else // 12-hour mode, bit 7 = PM
    {
        if (!RtcDecodeBcd(time[0] & 0x7F, 11, &rawHour))
            return FALSE;
        *hour = rawHour + ((time[0] & 0x80) ? 12 : 0);
    }
    return TRUE;
}

static bool8 RtcDecodeDateTime(u8 status, const u8 *dateTime, u8 *hour)
{
    u8 year, month, day, weekday;
    u8 days;
    if (!RtcDecodeBcd(dateTime[0], 99, &year)
     || !RtcDecodeBcd(dateTime[1], 12, &month) || month == 0
     || !RtcDecodeBcd(dateTime[2], 31, &day) || day == 0
     || !RtcDecodeBcd(dateTime[3], 6, &weekday))
        return FALSE;
    days = month == 2 ? (year % 4 == 0 ? 29 : 28)
         : (month == 4 || month == 6 || month == 9 || month == 11) ? 30 : 31;
    if (day > days)
        return FALSE;
    return RtcDecodeHour(status, dateTime + 4, hour);
}

bool8 RtcGetHour(u8 *hour)
{
    u8 status, dateTime[7];
    u16 ime = REG_IME;
    // Bound the bit-banged transaction; preserve the caller's interrupt state.
    REG_IME = 0;
    RTC_CONTROL = 1;
    RtcRead(0x63, &status, 1);
    RtcRead(0x65, dateTime, 7);
    RTC_CONTROL = 0;
    REG_IME = ime;
    // A valid calendar distinguishes midnight from absent all-zero hardware.
    return RtcDecodeDateTime(status, dateTime, hour);
}
