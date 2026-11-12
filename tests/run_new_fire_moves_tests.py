#!/usr/bin/env python3
"""Approved appended Fire data and exact preservation; native tests are separate."""
from pathlib import Path
import hashlib
import json
import re
import subprocess
import tempfile
from run_split_tests import extract_function
from approved_new_ghost_move_changes import prior_new_ghost_move_source
from approved_new_fire_move_changes import prior_new_fire_move_source

manifest = json.loads(Path('tests/specifications/approved_new_fire_move_delta.json').read_text())
for path, row in manifest['files'].items():
    assert hashlib.sha256(prior_new_ghost_move_source(path).encode()).hexdigest() == row['after_sha256'], path
    assert hashlib.sha256(prior_new_fire_move_source(path).encode()).hexdigest() == row['before_sha256'], path

spec = json.loads(Path('docs/moves/move-specification.json').read_text())['moves']
expected = [('FLAME_FANG', 355, 90, 15, 0, 'EFFECT_FLINCH_HIT', 20),
            ('FLASHFIRE', 356, 40, 30, 1, 'EFFECT_QUICK_ATTACK', 0),
            ('BLAZE_RUSH', 357, 100, 10, 0, 'EFFECT_RECOIL', 0)]
for key, move_id, power, pp, priority, effect, chance in expected:
    row = next(r for r in spec if r['key'] == key)
    assert row['id'] == move_id and row['delta'] == {}, key
    cur = row['current']
    assert (cur['type'], cur['category'], cur['power'], cur['accuracy'], cur['pp'],
            cur['priority'], cur['effect'], cur['secondaryEffectChance'], cur['target']) == (
            'FIRE', 'PHYSICAL', power, 100, pp, priority, effect, chance, 'MOVE_TARGET_SELECTED'), key
    assert 'FLAG_MAKES_CONTACT' in cur['flags'] and 'FLAG_PROTECT_AFFECTED' in cur['flags']
assert sum(r['id'] is not None and r['id'] <= 360 for r in spec) == 360
assert len(spec) == 410

asm = Path('data/battle_anim_scripts.s').read_text()
for key, original in zip(['FLAME_FANG', 'FLASHFIRE', 'BLAZE_RUSH'],
                         ['CRUNCH', 'QUICK_ATTACK', 'HI_JUMP_KICK']):
    old = re.search(r'Move_' + original + r':\n(.*?)\n\tend', asm, re.S)[1]
    new = re.search(r'Move_' + key + r':\n(.*?)\n\tend', asm, re.S)[1]
    assert new == old + '\n\twaitforvisualfinish\n\tcall NewFireMove_Ember', key
ember = re.search(r'Move_EMBER:\n(.*?)\n\tend', asm, re.S)[1]
shared = re.search(r'NewFireMove_Ember:\n(.*?)\n\treturn', asm, re.S)[1]
hit = re.search(r'EmberFireHit:\n(.*?)\n\treturn', asm, re.S)[1]
assert shared == ember.replace('\tcall EmberFireHit', hit) + '\n\twaitforvisualfinish'
assert '\tcall ' not in shared  # FireRed animation interpreter has ONE return slot.
# No production distribution has been added as a side effect of engine support.
for path in ['src/data/pokemon/level_up_learnsets.h', 'src/data/pokemon/tmhm_learnsets.h',
             'src/data/trainer_parties.h']:
    source=Path(path).read_text()
    if path=='src/data/pokemon/level_up_learnsets.h':
        from approved_pkm009_delegated_001_126_changes import prior_pkm009_delegated_001_126_source
        from approved_pkm009_delegated_127_252_changes import prior_pkm009_delegated_127_252_source
        source=prior_pkm009_delegated_001_126_source(path,source)
        source=prior_pkm009_delegated_127_252_source(path,source)
    assert not any('MOVE_' + key in source for key, *_ in expected), path
print('PASS: three exact appended owner designs, complete catalog and frozen historical reversal; original animation bodies/art reused; no distribution added (source audit, not runtime proof).')

# Execute the actual production chance routine with controlled rolls; only its
# effect callback/RNG are stubbed, separately from the real battle acceptance.
body = Path('tests/new_fire_chance_cases.c').read_text()
body += extract_function(Path('src/battle_script_commands.c').read_text(), 'Cmd_seteffectwithchance')
body += r"""
int main(void){
 for(unsigned newer=0;newer<2;newer++)for(unsigned grace=0;grace<2;grace++){
  unsigned chances[]={0,20,22,30,50,100};
  for(unsigned c=0;c<6;c++){
   unsigned total=0;gCurrentMove=newer?355:1;gBattleMons[0].ability=grace?ABILITY_SERENE_GRACE:0;
   gBattleMoves[gCurrentMove].secondaryEffectChance=chances[c];
   for(roll=0;roll<100;roll++){
    calls=0;gBattleCommunication[0]=1;gBattlescriptCurrInstr=script;gMoveResultFlags=0;
    Cmd_seteffectwithchance();total+=calls;assert(!gBattleCommunication[0]);
   }
   unsigned expected=chances[c]*(grace?2:1)+(newer?0:1);if(expected>100)expected=100;
   assert(total==expected);
  }
 }
 gCurrentMove=355;gBattleMoves[355].secondaryEffectChance=20;
 for(roll=0;roll<100;roll++){
  calls=0;gMoveResultFlags=MOVE_RESULT_NO_EFFECT;gBattleCommunication[0]=1;gBattlescriptCurrInstr=script;
  Cmd_seteffectwithchance();assert(!calls);
 }
 return 0;
}
"""
with tempfile.TemporaryDirectory(prefix='lightning-new-fire-chance-') as temp:
    source = Path(temp) / 'chance.c'; source.write_text(body)
    binary = Path(temp) / 'chance'
    subprocess.run(['gcc', '-std=gnu11', '-Wall', '-Wextra', str(source), '-o', str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
print('PASS: executed production secondary-chance routine over all100 rolls for0/20/22/30/50/100, Serene Grace, immunity and unchanged inclusive legacy behavior; callback/RNG stubs, not an emulator battle.')
