#!/usr/bin/env python3
"""Exercise production BCD/12h/24h decoding. Does not emulate physical GPIO."""
from pathlib import Path
import subprocess
import tempfile
from run_split_tests import extract_function
source = Path('src/rtc.c').read_text()
program = '#include <assert.h>\n#include <stdio.h>\n#include "global.h"\n'
program += r'''
static u16 direction;
static u8 output[3], captured;
static int responseBit, commandBit;
static u16 lastPins;
#define RTC_CLK 1
#define RTC_SIO 2
#define RTC_CS 4
#define RTC_DIRECTION direction
static u16 TestReadPins(void) { return ((output[responseBit/8] >> (responseBit%8)) & 1) * RTC_SIO; }
#define RTC_DATA TestReadPins()
static void RtcWritePins(u16 pins) {
 if (!(pins & RTC_CS)) {commandBit=0;responseBit=-1;}
 else if (direction==7 && !(lastPins & RTC_CLK) && (pins & RTC_CLK)) {
  if (pins & RTC_SIO) captured |= 1 << commandBit;
  commandBit++;
 } else if (direction==5 && (lastPins & RTC_CLK) && !(pins & RTC_CLK)) responseBit++;
 lastPins=pins;
}
'''
program += extract_function(source, 'RtcRead')
program += extract_function(source, 'RtcDecodeBcd') + extract_function(source, 'RtcDecodeHour') + extract_function(source, 'RtcDecodeDateTime')
program += r'''
static u8 testHour;
static bool8 testValid;
static unsigned reads;
bool8 RtcGetHour(u8 *hour) {reads++; *hour=testHour; return testValid;}
'''
program += Path('src/time_of_day.c').read_text()
program += r'''
int main(void) {
    unsigned h, m, checks = 0;
    u8 hour, time[3];
    for (h=0; h<24; h++) for (m=0; m<60; m++) {
        time[0]=(h/10)*16+h%10; time[1]=(m/10)*16+m%10; time[2]=0x59;
        assert(RtcDecodeHour(0x40,time,&hour) && hour==h); checks++;
        { unsigned twelve=h%12;
          time[0]=(twelve/10)*16+twelve%10; if (h>=12) time[0]|=0x80;
          assert(RtcDecodeHour(0,time,&hour) && hour==h); checks++; }
    }
    time[0]=time[1]=time[2]=0; assert(RtcDecodeHour(0,time,&hour) && hour==0); checks++;
    time[0]=time[1]=time[2]=255; assert(!RtcDecodeHour(255,time,&hour)); checks++;
    time[0]=0x24;time[1]=time[2]=0;assert(!RtcDecodeHour(0x40,time,&hour));checks++;
    time[0]=0x1a;assert(!RtcDecodeHour(0x40,time,&hour));checks++;
    time[0]=0x12;time[1]=0x60;assert(!RtcDecodeHour(0x40,time,&hour));checks++;
    { u8 dateTime[7]={0};
      assert(!RtcDecodeDateTime(0,dateTime,&hour));checks++;
      dateTime[0]=0x24;dateTime[1]=0x02;dateTime[2]=0x29;
      assert(RtcDecodeDateTime(0,dateTime,&hour) && hour==0);checks++;
      dateTime[0]=0x23;assert(!RtcDecodeDateTime(0,dateTime,&hour));checks++;
    }
    { u8 bytes[3];
      captured=0;output[0]=0x40;RtcRead(0x63,bytes,1);assert(captured==0xC6 && bytes[0]==0x40);checks++;
      captured=0;output[0]=0x23;output[1]=0x59;output[2]=0x58;RtcRead(0x67,bytes,3);
      assert(captured==0xE6 && bytes[0]==0x23 && bytes[1]==0x59 && bytes[2]==0x58);checks++;
    }
    // Execute the shared production policy with a controlled clock source.
    for(h=0;h<24;h++) {
      u8 period=255;testHour=h;testValid=TRUE;reads=0;
      assert(TryGetTimeOfDay(&period) && reads==1);checks++;
      assert(period==(h>=6 && h<18 ? TIME_OF_DAY_DAY:TIME_OF_DAY_NIGHT));checks++;
      assert(GetTimeOfDay()==period);checks++;
      testValid=FALSE;period=255;
      assert(!TryGetTimeOfDay(&period) && period==255);checks++;
      assert(GetTimeOfDay()==TIME_OF_DAY_DAY);checks++;
    }
    testValid=TRUE;testHour=24;
    assert(GetTimeOfDay()==TIME_OF_DAY_DAY);checks++;
    printf("PASS: %u RTC decoder/serial/time-policy assertions (stubbed GPIO/clock; physical hardware unverified)\n",checks);
    return 0;
}
'''
with tempfile.TemporaryDirectory() as temp:
    path=Path(temp)/'rtc.c'; path.write_text(program)
    subprocess.run(['gcc','-std=gnu11','-D__INTELLISENSE__','-iquote','include',str(path),'-o',temp+'/rtc'],check=True)
    subprocess.run([temp+'/rtc'],check=True)
