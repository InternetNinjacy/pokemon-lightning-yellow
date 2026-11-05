#!/usr/bin/env python3
"""M1a exact approved delta/name-capacity audits; runtime tests are separate."""
from pathlib import Path
import hashlib,json,re
from approved_move_foundation_changes import prior_move_foundation_source
manifest=json.loads(Path('tests/specifications/approved_move_foundation_delta.json').read_text())
for p,change in manifest.items():
    assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==change['after_sha256'],p
    assert hashlib.sha256(prior_move_foundation_source(p).encode()).hexdigest()==change['before_sha256'],p
constants=Path('include/constants/global.h').read_text()
assert re.search(r'#define MOVE_NAME_LENGTH 15\b',constants)
for p in ['src/learn_move.c','include/pokemon_special_anim_internal.h']:
    assert 'MOVE_NAME_LENGTH + 1' in Path(p).read_text()
header=Path('include/battle_message.h').read_text()
assert 'textBuffs[3][0x10]' in header and '#define TEXT_BUFF_ARRAY_COUNT   16' in header
assert '#define BATTLE_TEXT_BUFFER_SIZE (MOVE_NAME_LENGTH + 2)' in header
# Existing namespace, complete original data and save definitions stay byte-identical.
for p in ['include/constants/moves.h','include/global.h','src/data/text/move_names.h']:
    expected=json.loads(Path('tests/specifications/move_foundation_unchanged.json').read_text())[p]
    assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==expected,p
rows=json.loads(Path('docs/moves/move-specification.json').read_text())['moves']
assert next(r for r in rows if r['key']=='CURSE')['current']['type']=='GHOST'
assert all(len(r['name'])<=15 for r in rows)
print('PASS: seven exact approved source deltas,15-char capacity,17-byte used-move buffer, original16-byte wire payload, existing move IDs/names/save definitions preserved (static).')
print('LIMIT: battle64px/relearner80px layouts still require adaptation before long-name move entries are enabled; no new move IDs/effects are live.')

owner=Path('docs/moves/OWNER_DECISIONS_2026-11-05.md')
overlay=json.loads(Path('docs/moves/approved-design-overlays.json').read_text())
assert overlay['sources']['owner_20261105']['sha256']==hashlib.sha256(owner.read_bytes()).hexdigest()
for key in ['PAYBACK','DRAGON_RUSH']:
    r=next(r for r in rows if r['key']==key)
    assert r['effect_rules']['reference_generation']==4
print('PASS: effective owner decision provenance and Generation IV reference recorded; imported mechanics are not runtime implemented.')

assert all("u8 ALIGNED(4) gBattleTextBuff"+str(i) in Path("src/battle_main.c").read_text() for i in [1,2,3])
