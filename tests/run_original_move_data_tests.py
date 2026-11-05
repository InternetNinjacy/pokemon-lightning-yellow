#!/usr/bin/env python3
"""M2a exact approved fields; original scripts and other move bytes stay intact."""
from pathlib import Path
import hashlib,json,re
from approved_original_move_data_changes import prior_original_move_data_source
m=json.loads(Path('tests/specifications/approved_original_move_data_delta.json').read_text())
p='src/data/battle_moves.h';current=Path(p).read_text();prior=prior_original_move_data_source(p)
assert hashlib.sha256(current.encode()).hexdigest()==m['files'][p]['after_sha256']
assert hashlib.sha256(prior.encode()).hexdigest()==m['files'][p]['before_sha256']
def entries(s):
 return {k:dict(re.findall(r'\.(\w+) = (.*?),',b)) for k,b in re.findall(r'\[MOVE_(\w+)\] =\s*\{(.*?)\n    \}',s,re.S)}
before=entries(prior);after=entries(current);assert len(before)==len(after)==355
approved=json.loads(Path('docs/moves/approved-design-overlays.json').read_text())['originals']
actual={k:{f:v for f,v in after[k].items() if v!=before[k][f]} for k in after if after[k]!=before[k]}
expected={k:{f:str(v) for f,v in fields.items()} for k,fields in m['changes'].items()}
assert actual==expected and len(actual)==8
for k,fields in expected.items():
 for field,value in fields.items():
  design=approved[k]['fields'][field]
  assert value.removeprefix('TYPE_').removeprefix('MOVE_CATEGORY_')==str(design),(k,field)
  assert approved[k]['field_basis'][field]=='OWNER_EXPLICIT'
 # Effects/contact/secondary chances/targeting are preserved, including native
 # Twineedle poison, Icicle Spear hit count, Outrage lock and Astonish Minimize.
 for f in ['effect','flags','target','priority','secondaryEffectChance']:assert after[k][f]==before[k][f]
assert set(actual)=={'TWINEEDLE','LICK','OUTRAGE','MEAN_LOOK','ASTONISH','ROCK_TOMB','SILVER_WIND','ICICLE_SPEAR'}
# Retain the reviewed GenIV category baseline, allowing only this approved exception.
p=Path('tests/move_categories.json');cats=json.loads(p.read_text());old=dict(cats);old['MOVE_ASTONISH']='PHYSICAL'
assert hashlib.sha256((json.dumps(old,indent=2)+'\n').encode()).hexdigest()==m['category_manifest_before_sha256']
assert cats['MOVE_ASTONISH']=='SPECIAL'
rows=json.loads(Path('docs/moves/move-specification.json').read_text())['moves']
for k in expected:
 row=next(r for r in rows if r['key']==k)
 assert row['identity']=='REWORKED_ORIGINAL' and row['implementation_status']=='IMPLEMENTED_APPROVED_DATA' and not row['delta']
print('PASS: eight approved original moves/ten exact field revisions, all347 other rows (including NONE) and specialized effects/flags/targets preserved; Astonish owner-category exception, live reconciliation and historical category baseline checked (static).')
