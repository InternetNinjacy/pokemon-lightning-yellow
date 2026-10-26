#!/usr/bin/env python3
"""Build a headless mGBA runner for the actual ROM/ELF; no copyrighted BIOS."""
from pathlib import Path
import os
import re
import struct
import subprocess
import tempfile

# Generate ABI constants with the game's ARM compiler, never host offsetof.
expressions = {
    'MON_SIZE': 'sizeof(struct Pokemon)',
    'SAVE1_SIZE': 'sizeof(struct SaveBlock1)',
    'SAVE2_SIZE': 'sizeof(struct SaveBlock2)',
    'SAVE2_KEY': 'offsetof(struct SaveBlock2, encryptionKey)',
    'MAIN_HELD': 'offsetof(struct Main, heldKeys)',
    'MAIN_NEW': 'offsetof(struct Main, newKeys)',
    'MAIN_CB2': 'offsetof(struct Main, callback2)',
    'TASK_SIZE': 'sizeof(struct Task)',
    'TASK_DATA': 'offsetof(struct Task, data)',
}
for name in ['SPECIES','HELD_ITEM','LEVEL','FRIENDSHIP','IS_EGG']:
    expressions['DATA_'+name] = 'MON_DATA_'+name
required = ['RtcGetHour','gPlayerParty','CreateMon','SetMonData','GetMonData3',
            'GetEvolutionTargetSpecies','gSaveBlock1Ptr','gSaveBlock2Ptr','gSaveBlock1','gSaveBlock2',
            'SetBagPocketsPointers','ClearBag','EnableNationalPokedex','DisableNationalPokedex','AddBagItem','BagGetQuantityByItemId',
            'BeginItemEvolution','CB2_FinishItemEvolution','gMain','gCB2_AfterEvolution',
            'gEvolutionTable','Task_EvolutionScene','gTasks','sEvoStructPtr','gPlayerPartyCount','gHeap','ResetTasks','gPaletteFade','sPlttBufferTransferPending']
symbols={}
end_ewram=0
for line in subprocess.check_output(['arm-none-eabi-nm','-S','pokefirered.elf'],text=True).splitlines():
    f=line.split()
    if len(f)==4 and f[2] in 'bBdD' and 0x02000000<=int(f[0],16)<0x02040000:
        end_ewram=max(end_ewram,int(f[0],16)+int(f[1],16))
assert end_ewram<0x0203FD00, 'No safe test-stack space at end of EWRAM'
for line in subprocess.check_output(['arm-none-eabi-nm','-n','pokefirered.elf'],text=True).splitlines():
    fields=line.split()
    if len(fields)==3 and fields[2] in required:
        symbols[fields[2]]=int(fields[0],16)
assert set(required)<=symbols.keys(), set(required)-symbols.keys()
with tempfile.TemporaryDirectory(prefix='lightning-emulator-') as temp:
    temp=Path(temp)
    source='#include <stddef.h>\n#include "global.h"\n#include "pokemon.h"\n#include "main.h"\n#include "task.h"\n'
    source+='const unsigned long abi[] = {'+', '.join(expressions.values())+'};\n'
    (temp/'abi.c').write_text(source)
    with (temp/'abi.i').open('w') as out:
        subprocess.run(['gcc','-E','-nostdinc','-undef','-iquote','include','-I','tools/agbcc/include',
                        '-I','tools/agbcc',str(temp/'abi.c')],stdout=out,check=True)
    subprocess.run(['tools/agbcc/bin/agbcc','-O2','-o',str(temp/'abi.s'),str(temp/'abi.i')],check=True)
    subprocess.run(['arm-none-eabi-as','-mcpu=arm7tdmi','-o',str(temp/'abi.o'),str(temp/'abi.s')],check=True)
    subprocess.run(['arm-none-eabi-objcopy','-O','binary','-j','.rodata',str(temp/'abi.o'),str(temp/'abi.bin')],check=True)
    values=struct.unpack('<'+'I'*len(expressions),(temp/'abi.bin').read_bytes())
    header='\n'.join('#define ABI_'+name+' '+str(value) for name,value in zip(expressions,values))+'\n'
    scene_source=Path('src/evolution_scene.c').read_text()
    for group in re.findall(r'enum\s*\{(.*?)\};',scene_source,re.S):
        names=re.findall(r'\b(?:EVOSTATE|MVSTATE)_\w+',group)
        if names:
            assert len(names)==len(set(names))
            header+=''.join('#define E_'+name+' '+str(i)+'\n' for i,name in enumerate(names))
    header+='\n'.join('#define SYM_'+name+' 0x%08xU'%value for name,value in symbols.items())+'\n'
    header+='#define ABI_EWRAM_END 0x%08xU\n'%end_ewram
    (temp/'symbols.h').write_text(header)
    command=['gcc','-std=gnu11','-O2','-Wall','-Wextra','-I',str(temp),'-iquote','include',
             'tests/emulator/runner.c','-o',str(temp/'runner')]
    # Local workspace dependency extraction; Actions installs libmgba-dev normally.
    prefix=os.environ.get('MGBA_PREFIX')
    if prefix:
        command+=['-I',prefix+'/usr/include','-L',prefix+'/usr/lib/x86_64-linux-gnu',
                  '-Wl,-rpath,'+prefix+'/usr/lib/x86_64-linux-gnu']
    command+=['-lmgba']
    subprocess.run(command,check=True)
    subprocess.run([str(temp/'runner'),'pokefirered.gba'],check=True,env={**os.environ,'TZ':'UTC'})
