#!/usr/bin/env python3
"""Frozen approved Roar source delta; executed mechanics are in the native suite."""
import hashlib,json,re
from pathlib import Path
from approved_roar_changes import prior_roar_source
manifest=json.loads(Path('tests/specifications/approved_roar_delta.json').read_text())
for path,entry in manifest['files'].items():
    assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==entry['after_sha256'],path
    assert hashlib.sha256(prior_roar_source(path).encode()).hexdigest()==entry['before_sha256'],path
source=Path('src/data/battle_moves.h').read_text()
row=re.search(r'\[MOVE_ROAR\] =\s*\{(.*?)\n    \}',source,re.S)[1]
fields=dict(re.findall(r'\.(\w+) = (.*?),',row))
assert fields==dict(category='MOVE_CATEGORY_SPECIAL',effect='EFFECT_ATTACK_DOWN_HIT',power='70',type='TYPE_NORMAL',accuracy='100',pp='15',secondaryEffectChance='20',target='MOVE_TARGET_SELECTED',priority='0',flags='FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED')
old=prior_roar_source('src/data/battle_moves.h')
assert re.sub(r'\[MOVE_ROAR\] =\s*\{.*?\n    \}', '',source,flags=re.S)==re.sub(r'\[MOVE_ROAR\] =\s*\{.*?\n    \}', '',old,flags=re.S)
a=json.loads(Path('docs/moves/approved-design-overlays.json').read_text())['originals']['ROAR']
for key in ['category','power','pp','priority','secondaryEffectChance']:
    assert fields[key].removeprefix('MOVE_CATEGORY_')==str(a['fields'][key])
    assert a['field_basis'][key]=='OWNER_EXPLICIT'
assert a['engine_translation']['effect']==fields['effect']
assert a['fields']['effect']=='DAMAGE_ATTACK_MINUS_1_20_PERCENT'
rows=json.loads(Path('docs/moves/move-specification.json').read_text())['moves']
r=next(r for r in rows if r['key']=='ROAR');assert not r['delta'] and r['implementation_status']=='IMPLEMENTED_APPROVED_DATA'
assert 'MOVE_ROAR' in Path('src/battle_util.c').read_text() # sound-based immunity retained
print('PASS: exact approved Special70/100/15/priority0/20% Attack-1 Roar; every other move row preserved; semantic/engine translation explicit (static)')
