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
# Private screen layout is read from the same source compiled into this ROM.
screen_text=Path('src/pokedex_screen.c').read_text()
screen_struct=screen_text[screen_text.index('struct PokedexScreenData\n'):]
screen_struct=screen_struct[:screen_struct.index('\n};')+3]
for key,expr in {
 'SCREEN_ORDERED_MENU':'offsetof(struct PokedexScreenData,orderedListMenuTaskId)',
 'SCREEN_LIST':'offsetof(struct PokedexScreenData,listItems)',
 'SCREEN_COUNT':'offsetof(struct PokedexScreenData,orderedDexCount)',
 'SCREEN_STATE':'offsetof(struct PokedexScreenData,state)',
 'SCREEN_TASK':'offsetof(struct PokedexScreenData,taskId)',
 'SCREEN_DEX_SPECIES':'offsetof(struct PokedexScreenData,dexSpecies)',
 'LIST_SIZE':'sizeof(struct ListMenuItem)',
 'LIST_INDEX':'offsetof(struct ListMenuItem,index)',
 'BMON_SIZE':'sizeof(struct BattlePokemon)',
 'BMON_SPECIES':'offsetof(struct BattlePokemon,species)',
 'BMON_ABILITY':'offsetof(struct BattlePokemon,ability)',
 'BMON_HP':'offsetof(struct BattlePokemon,hp)',
 'BMON_TYPE1':'offsetof(struct BattlePokemon,type1)',
 'BMON_TYPE2':'offsetof(struct BattlePokemon,type2)',
 'BUFFER_STRIDE':'sizeof(gBattleBufferA[0])',
 'MOVEINFO_MOVES':'offsetof(struct ChooseMoveStruct,moves)',
 'META_ALL':'METATILE_ATTRIBUTES_ALL',
 'META_BEHAVIOR':'METATILE_ATTRIBUTE_BEHAVIOR',
 'META_ENCOUNTER':'METATILE_ATTRIBUTE_ENCOUNTER_TYPE',
 'TILE_LAND':'TILE_ENCOUNTER_LAND',
 'SAVE2_PLAYER_NAME':'offsetof(struct SaveBlock2,playerName)',
 'BAG_POCKET':'offsetof(struct BagStruct,pocket)',
 'SAVE1_LOCATION':'offsetof(struct SaveBlock1,location)',
 'WILD_HEADER_SIZE':'sizeof(struct WildPokemonHeader)',
 'WILD_HEADER_LAND':'offsetof(struct WildPokemonHeader,landMonsInfo)',
 'BMON_SPEED':'offsetof(struct BattlePokemon,speed)',
 'BMON_LEVEL':'offsetof(struct BattlePokemon,level)',
 'BMON_STATUS1':'offsetof(struct BattlePokemon,status1)',
 'BMON_STATUS2':'offsetof(struct BattlePokemon,status2)',
 'BMON_ITEM':'offsetof(struct BattlePokemon,item)',
 'BMON_MOVES':'offsetof(struct BattlePokemon,moves)',
 'BMON_PP':'offsetof(struct BattlePokemon,pp)',
 'BATTLESTRUCT_MOVETARGET':'offsetof(struct BattleStruct,moveTarget)',
 'RESULT_CAUGHT':'offsetof(struct BattleResults,caughtMonSpecies)',
}.items(): expressions[key]=expr
for name in ['SPECIES','HELD_ITEM','LEVEL','FRIENDSHIP','IS_EGG','ATK','DEF','HP','MAX_HP','MOVE1','MOVE2','MOVE3','MOVE4','PP1','PP2','PP3','PP4']:
    expressions['DATA_'+name] = 'MON_DATA_'+name
required = [
    'AddBagItem', 'BagGetQuantityByItemId', 'BeginItemEvolution', 'CB2_LoadMap', 'CB2_OpenPokedexFromStartMenu',
    'CB2_Overworld', 'CastformDataTypeChange', 'ClearBag', 'CreateMon', 'CustomPokedexNumToSpecies',
    'DexScreen_CountMonsInOrderedList', 'DisableNationalPokedex', 'EnableNationalPokedex', 'EvolutionScene', 'FlagSet',
    'GetBoxMonDataAt', 'GetBoxNamePtr', 'GetCurrentMapWildMonHeaderId', 'GetCustomPokedexCount', 'GetEvolutionTargetSpecies',
    'GetMonAbility', 'GetMonData3', 'GetSav1Weather', 'GetSetPokedexFlag', 'GetWildCatchTarget',
    'GetWildOpponentCount', 'HandleInputChooseAction', 'HandleInputChooseMove', 'HandleInputChooseTarget', 'HasAllCustomPokedexSpecies',
    'HasAllMons', 'InitRoamer', 'IsRunningFromBattleImpossible', 'ListMenuGetScrollAndRow', 'LoadGameSave',
    'MapGridGetMetatileAttributeAt', 'Menu_GetCursorPos', 'Menu_MoveCursorNoWrapAround', 'OpponentHandleChooseMove', 'PlayerGetDestCoords',
    'ResetInitialPlayerAvatarState', 'ResetTasks', 'RtcGetHour', 'Save_ResetSaveCounters', 'SetBagPocketsPointers',
    'SetControllerToOpponent', 'SetControllerToPlayer', 'SetMonData', 'SetWarpDestination', 'SetWeather',
    'SpeciesToCustomPokedexNum', 'SpeciesToNationalPokedexNum', 'SpeciesToPokedexNum', 'StandardWildEncounter', 'SweetScentWildEncounter',
    'TryDoDoubleWildBattle', 'TrySavingData', 'TryStartLandEncounter', 'VarSet', 'WarpIntoMap',
    'ZeroPlayerPartyMons', 'gActionSelectionCursor', 'gActiveBattler', 'gBagMenuState', 'gBattleBufferA',
    'gBattleBufferB', 'gBattleMons', 'gBattleOutcome', 'gBattleResults', 'gBattleTypeFlags',
    'gBattleWeather', 'gBattlerControllerFuncs', 'gBattlersCount', 'gBattlescriptCurrInstr', 'gCB2_AfterEvolution',
    'gEnemyParty', 'gFlashMemoryPresent', 'gMain', 'gMoveSelectionCursor', 'gMultiUsePlayerCursor',
    'gPaletteFade', 'gPlayerParty', 'gPlayerPartyCount', 'gPokemonStorage', 'gPokemonStoragePtr',
    'gRngValue', 'gSaveBlock1', 'gSaveBlock1Ptr', 'gSaveBlock2', 'gSaveBlock2Ptr',
    'gTasks', 'gWildMonHeaders', 'sEvoStructPtr', 'sPlttBufferTransferPending', 'sPokedexScreenData',
    'sRoamerLocation',
]
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
        symbols.setdefault(fields[2],int(fields[0],16)) # first matching local symbol (player controller precedes tutorial controllers)
assert set(required)<=symbols.keys(), set(required)-symbols.keys()
# Private input function names also exist in tutorial controllers. Verify the
# selected local symbols belong to the actual player controller's address range.
for name in ['HandleInputChooseAction','HandleInputChooseMove','HandleInputChooseTarget']:
    assert symbols['SetControllerToPlayer'] < symbols[name] < symbols['SetControllerToOpponent'], name
with tempfile.TemporaryDirectory(prefix='lightning-emulator-') as temp:
    temp=Path(temp)
    source='#include <stddef.h>\n#include "global.h"\n#include "pokemon.h"\n#include "main.h"\n#include "task.h"\n'
    source+='#include "battle_controllers.h"\n#include "item_menu.h"\n#include "wild_encounter.h"\n#include "battle.h"\n#include "list_menu.h"\n'+screen_struct+'\n'
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
