#!/usr/bin/env python3
"""Approved appended Fire data and exact preservation; native tests are separate."""
from pathlib import Path
import hashlib
import json
import re
from approved_new_fire_move_changes import prior_new_fire_move_source

manifest = json.loads(Path('tests/specifications/approved_new_fire_move_delta.json').read_text())
for path, row in manifest['files'].items():
    assert hashlib.sha256(Path(path).read_bytes()).hexdigest() == row['after_sha256'], path
    assert hashlib.sha256(prior_new_fire_move_source(path).encode()).hexdigest() == row['before_sha256'], path

spec = json.loads(Path('docs/moves/move-specification.json').read_text())['moves']
expected = [('FLAME_FANG', 355, 90, 15, 0, 'EFFECT_FLINCH_HIT', 20),
            ('FLASHFIRE', 356, 40, 30, 1, 'EFFECT_HIT', 0),
            ('BLAZE_RUSH', 357, 100, 10, 0, 'EFFECT_RECOIL', 0)]
for key, move_id, power, pp, priority, effect, chance in expected:
    row = next(r for r in spec if r['key'] == key)
    assert row['id'] == move_id and row['delta'] == {}, key
    cur = row['current']
    assert (cur['type'], cur['category'], cur['power'], cur['accuracy'], cur['pp'],
            cur['priority'], cur['effect'], cur['secondaryEffectChance'], cur['target']) == (
            'FIRE', 'PHYSICAL', power, 100, pp, priority, effect, chance, 'MOVE_TARGET_SELECTED'), key
    assert 'FLAG_MAKES_CONTACT' in cur['flags'] and 'FLAG_PROTECT_AFFECTED' in cur['flags']
assert sum(r['id'] is not None for r in spec) == 357
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
    assert not any('MOVE_' + key in Path(path).read_text() for key, *_ in expected), path
print('PASS: three exact appended owner designs, complete catalog and frozen historical reversal; original animation bodies/art reused; no distribution added (source audit, not runtime proof).')
