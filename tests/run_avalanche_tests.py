#!/usr/bin/env python3
"""Exact #373 contract/reversals. Numeric C runs in split; new engine runs in mGBA."""
from pathlib import Path
import hashlib,json,re
from approved_avalanche_changes import prior_avalanche_source
from approved_moves374_changes import prior_moves374_source
m=json.loads(Path('tests/specifications/avalanche_delta.json').read_text())
for p,r in m['files'].items():
    assert hashlib.sha256(prior_moves374_source(p).encode()).hexdigest()==r['after_sha256'],p
    assert hashlib.sha256(prior_avalanche_source(p).encode()).hexdigest()==r['before_sha256'],p
s=Path('src/data/battle_moves.h').read_text().split('[MOVE_AVALANCHE] =')[1].split('    },',1)[0]
for field,value in {'category':'MOVE_CATEGORY_PHYSICAL','effect':'EFFECT_HIT','power':60,'type':'TYPE_ICE','accuracy':100,'pp':10,'priority':-4,'target':'MOVE_TARGET_SELECTED','secondaryEffectChance':0}.items():
    assert f'.{field} = {value},' in s,(field,value)
assert 'FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED' in s
constants=prior_moves374_source('include/constants/moves.h');assert '#define MOVE_AVALANCHE 373' in constants and '#define MOVES_COUNT 374' in constants
anim=Path('data/battle_anim_scripts.s').read_text();assert anim.count('.4byte Move_AVALANCHE')==1
body=prior_moves374_source('data/battle_anim_scripts.s').split('Move_AVALANCHE:\n')[1];assert 'ANIM_TAG_ROCKS' in body and 'IceBeamCreateCrystals' in body
assert 'ANIM_DEF_PARTNER' not in body and 'call Move_' not in body and body.strip().endswith('end')
for p in ['src/data/pokemon/level_up_learnsets.h','src/data/pokemon/tmhm_learnsets.h','src/data/trainer_parties.h']:
    assert 'MOVE_AVALANCHE' not in Path(p).read_text(),p
assert 'BattlerWasDamagedBy(battlerIdAtk, battlerIdDef)' in Path('src/pokemon.c').read_text()
assert 'damagedByBattlers[MAX_BATTLERS_COUNT]' in Path('include/battle.h').read_text()
print('PASS: exact Avalanche373 approved data/target-specific engine delta, original-ID/source reversals, selected-target existing-art composite and no distribution (static).')
