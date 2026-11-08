#!/usr/bin/env python3
"""Exact approved GenIV Payback source contract; native tests prove execution."""
from pathlib import Path
import json,hashlib
from approved_payback_changes import prior_payback_source
m=json.loads(Path('tests/specifications/payback_delta.json').read_text())
for p,r in m['files'].items():
 assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==r['after_sha256'],p
 assert hashlib.sha256(prior_payback_source(p).encode()).hexdigest()==r['before_sha256'],p
s=Path('src/data/battle_moves.h').read_text().split('[MOVE_PAYBACK] =')[1].split('    },',1)[0]
for f,v in dict(category='MOVE_CATEGORY_PHYSICAL',effect='EFFECT_HIT',power=50,type='TYPE_DARK',accuracy=100,pp=10,secondaryEffectChance=0,target='MOVE_TARGET_SELECTED',priority=0).items():assert f'.{f} = {v},' in s,(f,v)
assert 'FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED' in s
c=Path('include/constants/moves.h').read_text();assert '#define MOVE_PAYBACK 381' in c and '#define MOVES_COUNT 382' in c
a=Path('data/battle_anim_scripts.s').read_text();assert a.count('.4byte Move_PAYBACK\n')==1 and 'Move_PAYBACK:\n\tgoto Move_REVENGE\n' in a
p=Path('src/pokemon.c').read_text();assert 'move == MOVE_PAYBACK && BattlerHasActedThisTurn(battlerIdDef)' in p
main=Path('src/battle_main.c').read_text();assert 'gBattleStruct->actedThisTurn = 0;' in main and 'gBattleStruct->actedThisTurn |= 1U << gBattlerByTurnOrder[gBattleStruct->savedTurnActionNumber];' in main
assert 'actedThisTurn' not in main.split('void SwitchInClearSetData(void)')[1].split('void FaintClearSetData')[0] # switching retains GenIV slot history
for p in ['src/data/pokemon/level_up_learnsets.h','src/data/pokemon/tmhm_learnsets.h','src/data/trainer_parties.h']:assert 'MOVE_PAYBACK' not in Path(p).read_text()
print('PASS: exact Payback381 data, original-ID/source reversals, completed selected-target action/switch context, native Revenge artwork and no distribution (static).')
