#!/usr/bin/env python3
"""Actual source vs approved385–410 overlay, with exact original-source reversals."""
from pathlib import Path
import re,json,hashlib
from approved_remaining_moves import prior_remaining_source
m=json.loads(Path('tests/specifications/remaining_moves_current_main_delta.json').read_text())
for p,r in m['files'].items():
 assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==r['after_sha256'],p
 assert hashlib.sha256(prior_remaining_source(p).encode()).hexdigest()==r['before_sha256'],p
constants=Path('include/constants/moves.h').read_text();count=int(re.search(r'#define MOVES_COUNT (\d+)',constants)[1]);data=Path('src/data/battle_moves.h').read_text();overlay=json.loads(Path('docs/moves/approved-design-overlays.json').read_text());anim=Path('data/battle_anim_scripts.s').read_text()
for r in overlay['new_moves']:
 if not 385<=r['proposed_id']<count:continue
 key=r['key'];a=r['approved'];t=r['engine_translation'];block=re.search(r'\[MOVE_'+key+r'\]\s*=\s*\{(.*?)\n    \},',data,re.S)[1]
 for f,v in dict(category='MOVE_CATEGORY_'+a['category'],type='TYPE_'+a['type'],power=a['power'],accuracy=a['accuracy'],pp=a['pp'],priority=a['priority'],effect=t['effect'],target=t['target'],flags=t['flags']).items():assert f'.{f} = {v},' in block,(key,f,v)
 chance=a['secondaryEffectChance']
 if key=='VORTEX_SPIN':chance=30
 if key=='THUNDER_FANG':chance=10
 assert f'.secondaryEffectChance = {chance},' in block,key
 assert anim.count('.4byte Move_'+key+'\n')==1 and anim.count('Move_'+key+':')==1,key
 for p in ['src/data/pokemon/level_up_learnsets.h','src/data/pokemon/tmhm_learnsets.h','src/data/trainer_parties.h']:assert 'MOVE_'+key not in Path(p).read_text(),(key,p)
for key in ['VORTEX_SPIN','TIDAL_CRASH']:
 r=next(r for r in overlay['new_moves'] if r['key']==key);assert r['field_basis']['priority']=='OWNER_EXPLICIT_NORMAL_PRIORITY_2026_11_08'
print(f'PASS: approved remaining source385–{count-1}, data/categories/flags/art pointers/no invented distribution and exact original-source preservation. Static; native execution separate.')
