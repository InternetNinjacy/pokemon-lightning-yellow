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
    // Command and response bytes are transferred LSB first.
    for (i = 0; i < 8; i++)
    {
        u16 data = command & (1 << i) ? RTC_SIO : 0;
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
        if (!RtcDecodeBcd(time[0] & 0x7F, 12, &rawHour) || rawHour == 0)
            return FALSE;
        *hour = rawHour % 12 + ((time[0] & 0x80) ? 12 : 0);
    }
    return TRUE;
}

bool8 RtcGetHour(u8 *hour)
{
    u8 status, time[3];
    u16 ime = REG_IME;
    // Bound the bit-banged transaction; preserve the caller's interrupt state.
    REG_IME = 0;
    RTC_CONTROL = 1;
    RtcRead(0x63, &status, 1);
    RtcRead(0x67, time, 3);
    RTC_CONTROL = 0;
    REG_IME = ime;
    return RtcDecodeHour(status, time, hour);
}
