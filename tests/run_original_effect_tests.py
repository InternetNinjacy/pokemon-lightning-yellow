#!/usr/bin/env python3
"""Exact approved Pin Missile/Dragon Rage source and evidence audit."""
from pathlib import Path
import json,re,hashlib
from approved_original_effect_changes import prior_original_effect_source
m=json.loads(Path('tests/specifications/approved_original_effect_delta.json').read_text())
for p,v in m['files'].items():
 from approved_spread_status_changes import prior_spread_status_source
 assert hashlib.sha256(prior_spread_status_source(p).encode()).hexdigest()==v['after_sha256'],p
 assert hashlib.sha256(prior_original_effect_source(p).encode()).hexdigest()==v['before_sha256'],p
source=prior_spread_status_source('src/data/battle_moves.h');old=prior_original_effect_source('src/data/battle_moves.h')
def rows(s):return {k:dict(re.findall(r'\.(\w+) = (.*?),',b)) for k,b in re.findall(r'\[MOVE_(\w+)\] =\s*\{(.*?)\n    \}',s,re.S)}
a,b=rows(old),rows(source);assert len(a)==len(b)==355
changes={k:{f:v for f,v in b[k].items() if v!=a[k][f]} for k in b if a[k]!=b[k]}
assert changes=={'PIN_MISSILE':{'effect':'EFFECT_POISON_HIT','power':'55','secondaryEffectChance':'30'},'DRAGON_RAGE':{'effect':'EFFECT_LEVEL_DAMAGE'}}
assert b['PIN_MISSILE']['accuracy']=='85' and b['PIN_MISSILE']['pp']=='20' and b['PIN_MISSILE']['category']=='MOVE_CATEGORY_PHYSICAL'
assert b['DRAGON_RAGE']['power']=='1' and b['DRAGON_RAGE']['pp']=='10' and b['DRAGON_RAGE']['category']=='MOVE_CATEGORY_SPECIAL'
approved=json.loads(Path('docs/moves/approved-design-overlays.json').read_text())['originals']
for k in changes:
 assert approved[k]['implementation_status']=='IMPLEMENTED_APPROVED_DATA'
 assert approved[k]['engine_translation']['effect']==b[k]['effect']
 assert b[k]['flags']==a[k]['flags'] and b[k]['target']==a[k]['target']
print('PASS: exact owner-approved single55/85/20/30% poison Pin Missile and user-level Dragon Rage; all other fields/353 rows, IDs, PP maxima and existing flags/effect scripts retained (static)')

from approved_electric_immunity_changes import prior_electric_immunity_source
immunity=json.loads(Path('tests/specifications/approved_electric_immunity_delta.json').read_text())
for p,v in immunity['files'].items():
 from approved_spread_status_changes import prior_spread_status_source
 assert hashlib.sha256(prior_spread_status_source(p).encode()).hexdigest()==v['after_sha256'],p
 assert hashlib.sha256(prior_electric_immunity_source(p).encode()).hexdigest()==v['before_sha256'],p
print('PASS: exact approved global Electric paralysis immunity delta; primary message and effective-recipient secondary/contact guard (static)')

spread=json.loads(Path('tests/specifications/approved_spread_status_delta.json').read_text())
for p,v in spread['files'].items():
 from approved_ghost_rework_changes import prior_ghost_rework_source
 assert hashlib.sha256(prior_ghost_rework_source(p).encode()).hexdigest()==v['after_sha256'],p
 assert hashlib.sha256(prior_spread_status_source(p).encode()).hexdigest()==v['before_sha256'],p
print('PASS: exact approved Scary Face Ghost15PP and three both-opponent status targets; historical hashes preserved (static)')

ghost=json.loads(Path('tests/specifications/approved_ghost_rework_delta.json').read_text())
for p,v in ghost['files'].items():
 from approved_thunder_wave_changes import prior_thunder_wave_source
 assert hashlib.sha256(prior_thunder_wave_source(p).encode()).hexdigest()==v['after_sha256'],p
 assert hashlib.sha256(prior_ghost_rework_source(p).encode()).hexdigest()==v['before_sha256'],p
print('PASS: exact approved Spite damage/optional PP drain and Grudge native Rollout/no Defense Curl; no other production changes (static)')

tw=json.loads(Path('tests/specifications/approved_thunder_wave_delta.json').read_text())
for p,v in tw['files'].items():
 text=Path(p).read_text()
 if p=='src/battle_main.c':
  from approved_elite_four_changes import prior_elite_four_source
  text=prior_elite_four_source(p,text)
 assert hashlib.sha256(text.encode()).hexdigest()==v['after_sha256'],p
 assert hashlib.sha256(prior_thunder_wave_source(p,text).encode()).hexdigest()==v['before_sha256'],p
print('PASS: exact approved all-other-active Thunder Wave with private padding context, native per-recipient checks; saved/link fields unchanged (static)')
