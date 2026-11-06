#!/usr/bin/env python3
"""Exact M2b source/authority/save-layout audit; crypto and flash execute in mGBA."""
from pathlib import Path
import hashlib,json,re
from approved_move_pp_changes import prior_move_pp_source
from approved_roar_changes import prior_roar_source
m=json.loads(Path('tests/specifications/approved_move_pp_delta.json').read_text())
for p,v in m['files'].items():
 assert hashlib.sha256(prior_roar_source(p).encode()).hexdigest()==v['after_sha256'],p
 assert hashlib.sha256(prior_move_pp_source(p).encode()).hexdigest()==v['before_sha256'],p
for p,sha in m['added_files'].items():assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==sha,p
p='src/data/battle_moves.h';old=prior_move_pp_source(p);new=prior_roar_source(p)
def entries(s):return {k:dict(re.findall(r'\.(\w+) = (.*?),',b)) for k,b in re.findall(r'\[MOVE_(\w+)\] =\s*\{(.*?)\n    \}',s,re.S)}
a,b=entries(old),entries(new);assert len(a)==len(b)==355
changes={k:{f:v for f,v in b[k].items() if v!=a[k][f]} for k in b if a[k]!=b[k]}
assert changes=={'LEECH_LIFE':{'category':'MOVE_CATEGORY_SPECIAL','power':'80','pp':'10'}}
o=json.loads(Path('docs/moves/approved-design-overlays.json').read_text())['originals']['LEECH_LIFE']
assert o['fields']=={'category':'SPECIAL','power':80,'pp':10}
assert all(o['field_basis'][f]=='OWNER_EXPLICIT' for f in changes['LEECH_LIFE'])
assert b['LEECH_LIFE']['effect']=='EFFECT_ABSORB'
from run_split_tests import extract_function
c=extract_function(Path('src/pokemon.c').read_text(),'ClampApprovedMovePP')
sites=Path('src/move_pp_compatibility.c').read_text()
assert set(re.findall(r'\bMOVE_[A-Z_]+\b',c))=={'MOVE_LEECH_LIFE','MOVE_ROAR'}
assert 'if (attacks->pp[slot] > maximum)' in c and 'BoxMonRestorePP(' not in c and 'Random(' not in c
assert 'copy = *mon;' in c and 'DecryptBoxMon(&copy);' in c and 'CalculateBoxMonChecksum(&copy) != copy.checksum' in c
assert 'if (changed)' in c and 'EncryptBoxMon(&copy);\n        *mon = copy;' in c
assert 'route5DayCareMon' not in sites # recovery copy must not be normalized
assert 'route5->magic == ROUTE5_DAYCARE_MAGIC && route5->version == ROUTE5_DAYCARE_VERSION' in sites
assert 'if (result == SAVE_STATUS_OK || result == SAVE_STATUS_ERROR)' in Path('src/save.c').read_text()
# No save structure or ID edits, even though source hooks are added.
for p,sha in m['preserved_files'].items():assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==sha,p
print('PASS: atomic owner-approved Special/80/10 Leech Life, all other move fields/rows and save/ID layouts preserved; frozen PP/load/lazy-migration hooks, no RNG/refill/recovery-copy writes (static, not runtime crypto proof).')
