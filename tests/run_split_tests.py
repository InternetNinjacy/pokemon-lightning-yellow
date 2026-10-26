#!/usr/bin/env python3
"""Execute production C routines on the host, then run separate static audits.

This is not a battle-engine or emulator test. Only hardware-facing dependencies
are stubbed; the damage/AI/label functions and move table come from current source.
Run from the repository root. Requires Python 3 and host GCC, no Python packages.
"""
from pathlib import Path
import re
import subprocess
import tempfile


def extract_function(source, name):
    match = re.search(r'^(?:static )?[^\n;]*\b' + name + r'\([^;]*?\)\n\{', source, re.M)
    if not match:
        raise AssertionError('Production function not found: ' + name)
    start = match.start()
    end = source.index('{', start) + 1
    depth = 1
    while depth:
        if source[end] == '{':
            depth += 1
        if source[end] == '}':
            depth -= 1
        end += 1
    return source[start:end] + '\n'


def run_numeric_tests():
    pokemon = Path('src/pokemon.c').read_text()
    parts = [Path('tests/host_support.h').read_text()]
    for name in ['gStatStageRatios', 'sHoldEffectToType']:
        parts.append(re.search(r'^(?:static )?const u8 ' + name + r'\[.*?\n\};', pokemon, re.M | re.S)[0])
    parts.append(pokemon[pokemon.index('#define APPLY_STAT_MOD'):pokemon.index('s32 CalculateBaseDamage(')])
    parts.append(extract_function(pokemon, 'CalculateBaseDamage'))
    commands = Path('src/battle_script_commands.c').read_text()
    for name in ['AI_CalcDmg', 'Cmd_dmgtolevel', 'Cmd_damagetohalftargethp', 'Cmd_psywavedamageeffect',
                 'Cmd_counterdamagecalculator', 'Cmd_mirrorcoatdamagecalculator', 'Cmd_datahpupdate']:
        parts.append(extract_function(commands, name))
    ai = Path('src/battle_ai_script_commands.c').read_text()
    for name in ['Cmd_get_move_category_from_result', 'Cmd_has_move_category']:
        parts.append(extract_function(ai, name))
    parts.append(Path('src/move_categories.c').read_text())
    parts.append(Path('tests/damage_cases.c').read_text())
    with tempfile.TemporaryDirectory(prefix='lightning-split-') as temp:
        path = Path(temp) / 'damage.c'
        path.write_text('\n'.join(parts))
        binary = Path(temp) / 'damage'
        subprocess.run(['gcc', '-std=gnu11', '-D__INTELLISENSE__', '-iquote', '.', '-iquote', 'include',
                        '-Wno-pointer-to-int-cast', '-Wno-int-to-pointer-cast', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    run_numeric_tests()
    from audit_split import run_audits
    run_audits()
