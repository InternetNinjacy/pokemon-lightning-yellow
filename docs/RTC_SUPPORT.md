# Read-only cartridge RTC dependency

Separate dependency for the evolution overhaul, based on PR #2 without modifying
its gameplay. Uses the GBA cartridge S-3511 GPIO protocol (ports 080000C4/C6/C8,
status read command 63h, date/time read command 65h). Implementation is project-written;
no source from unrelated games or proprietary libraries is imported.

`RtcGetHour` reads raw cartridge/emulator clock time, supports 12h and 24h BCD,
validates calendar and hour/minute/second and returns FALSE for unusable readings. It does not
set/reset the clock, add save offsets, set a timezone, change palettes, or change
save structures. GPIO transactions briefly mask interrupts and restore REG_IME.
The SIIRTC_V001 signature advertises RTC hardware to compatible emulators. Plain
retail FireRed cartridges have no RTC; use RTC-capable hardware or an emulator
with cartridge RTC support enabled. Raw RTC time depends on that device's setup.

Run `python3 tests/run_rtc_tests.py`. Executed host tests cover all 24 hours and
60 minutes in both clock formats, invalid BCD, out-of-range time and common absent
hardware responses. Host serial tests also execute the production RtcRead loop against stubbed pin
I/O, checking LSB-first command bytes and response bytes. This validates logical
serialization, NOT physical GPIO timing or an actual emulator transaction. No
emulator is installed here; physical RTC and emulator transaction testing remain
outstanding. Review and validate those before accepting this dependency. No clock
reset fallback or fabricated time is used.

The evolution consumer separately preserves the disabled upstream code's intended
boundaries: day 12:00–23:59, night 00:00–11:59, friendship >=220. Failed reads must
not select a timed evolution. Stones remain an independent fallback.

Protocol review consulted mGBA's public GBA GPIO implementation as a behavioral
reference (https://github.com/mgba-emu/mgba/blob/master/src/gba/cart/gpio.c);
no mGBA source was copied. Commands/data are LSB first. The 12-hour register is
00–11 with bit 7 indicating PM. The calendar read distinguishes a valid midnight
from an absent all-zero device. Host tests cover invalid dates and leap years.
