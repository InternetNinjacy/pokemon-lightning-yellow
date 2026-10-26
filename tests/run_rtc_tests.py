#!/usr/bin/env python3
"""Exercise production BCD/12h/24h decoding. Does not emulate physical GPIO."""
from pathlib import Path
import subprocess
import tempfile
from run_split_tests import extract_function
source = Path('src/rtc.c').read_text()
program = '#include <assert.h>\n#include <stdio.h>\n#include "global.h"\n'
program += extract_function(source, 'RtcDecodeBcd') + extract_function(source, 'RtcDecodeHour')
program += r'''
int main(void) {
    unsigned h, m, checks = 0;
    u8 hour, time[3];
    for (h=0; h<24; h++) for (m=0; m<60; m++) {
        time[0]=(h/10)*16+h%10; time[1]=(m/10)*16+m%10; time[2]=0x59;
        assert(RtcDecodeHour(0x40,time,&hour) && hour==h); checks++;
        { unsigned twelve=h%12; if (!twelve) twelve=12;
          time[0]=(twelve/10)*16+twelve%10; if (h>=12) time[0]|=0x80;
          assert(RtcDecodeHour(0,time,&hour) && hour==h); checks++; }
    }
    time[0]=time[1]=time[2]=0; assert(!RtcDecodeHour(0,time,&hour)); checks++;
    time[0]=time[1]=time[2]=255; assert(!RtcDecodeHour(255,time,&hour)); checks++;
    time[0]=0x24;time[1]=time[2]=0;assert(!RtcDecodeHour(0x40,time,&hour));checks++;
    time[0]=0x1a;assert(!RtcDecodeHour(0x40,time,&hour));checks++;
    time[0]=0x12;time[1]=0x60;assert(!RtcDecodeHour(0x40,time,&hour));checks++;
    printf("PASS: %u RTC decode assertions; GPIO/emulator validation outstanding\n",checks);
    return 0;
}
'''
with tempfile.TemporaryDirectory() as temp:
    path=Path(temp)/'rtc.c'; path.write_text(program)
    subprocess.run(['gcc','-std=gnu11','-D__INTELLISENSE__','-iquote','include',str(path),'-o',temp+'/rtc'],check=True)
    subprocess.run([temp+'/rtc'],check=True)
