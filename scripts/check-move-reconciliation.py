#!/usr/bin/env python3
"""Audit inventory/provenance/approved exceptions; NOT a battle implementation test."""
from pathlib import Path
import collections,json,re,subprocess,sys,hashlib
R=Path(__file__).resolve().parents[1]
# Batch A staging is checked against the accepted #385-386 blob history and
# its own exact owner-approved source contract before any catalog projection.
sys.path.insert(0,str(R/'tests'))
from approved_batch_a_changes import HUNKS as BATCH_A_HUNKS, prior_batch_a_source
for batch_path in BATCH_A_HUNKS:
    prior_batch_a_source(batch_path)
if (R/'tests/approved_batch_b_changes.py').exists():
    from approved_batch_b_changes import HUNKS as BATCH_B_HUNKS, prior_batch_b_source
    for path in BATCH_B_HUNKS:
        prior_batch_b_source(path)
    subprocess.run([sys.executable,str(R/'tests/run_btl_batch_b_staging_tests.py')],cwd=R,check=True)
else:
    subprocess.run([sys.executable,str(R/'tests/run_btl_batch_a_staging_tests.py')],cwd=R,check=True)
subprocess.run([sys.executable,str(R/'scripts/reconcile-move-specification.py'),'--check'],cwd=R,check=True)
d=json.loads((R/'docs/moves/move-specification.json').read_text());rows=d['moves'];keys={r['key']:r for r in rows}
assert len(rows)==410 and len(keys)==410
# BTL-005 #385-386 remains a separately reviewable, unaccepted implementation.
# Preserve the last accepted catalog hash, after STRICTLY reversing only the
# approved append; the actual production source is independently audited by
# tests/run_water_moves385_tests.py and is never silently normalized.
sys.path.insert(0,str(R/'tests'))
from approved_water_moves385_changes import prior_water_moves385_source
catalog_source=prior_water_moves385_source('src/data/battle_moves.h',(R/'src/data/battle_moves.h').read_text())
assert d['current_battle_moves_sha256']==hashlib.sha256(catalog_source.encode()).hexdigest()
old=[r for r in rows if r['identity'] in ['ORIGINAL','REWORKED_ORIGINAL']];new=[r for r in rows if r['identity'].startswith('NEW_')]
assert len(old)==354 and {r['id'] for r in old}==set(range(1,355))
assert len(new)==56 and {r['proposed_id'] for r in new}==set(range(355,411))
assert all(r['approved']['category'] in ['PHYSICAL','SPECIAL','STATUS'] for r in rows)
assert keys['CURSE']['approved']['type']==keys['CURSE']['current']['type']=='GHOST'
assert keys['CURSE']['approved']['category']=='STATUS'
assert all(r['key']=='CURSE' or r['approved']['type'] in ['NORMAL','FIRE','WATER','ELECTRIC','GRASS','ICE','FIGHTING','POISON','GROUND','FLYING','PSYCHIC','BUG','ROCK','GHOST','DRAGON','DARK','STEEL'] for r in rows)
assert sum(r['identity']=='REWORKED_ORIGINAL' for r in old)==21
for key,field,value in [('SHADOW_BALL','category','SPECIAL'),('ASTONISH','category','SPECIAL'),('SPITE','category','SPECIAL'),('GRUDGE','category','SPECIAL'),('LEECH_LIFE','category','SPECIAL'),('LEECH_LIFE','power',80),('PIN_MISSILE','power',55),('TWINEEDLE','power',40),('ROAR','power',70),('ROAR','priority',0),('SANDSTORM','type','GROUND'),('MEAN_LOOK','type','GHOST'),('ROCK_TOMB','accuracy',90),('HEART_STAMP','power',55),('STONE_EDGE','pp',10),('DRAGON_PULSE','power',85),('FLAME_FANG','power',90),('BUG_BITE','power',65)]:
 assert keys[key]['approved'][field]==value,(key,field)
assert keys['HURRICANE']['approved']['secondaryEffectChance']==30 and keys['DRAGON_RUSH']['approved']['secondaryEffectChance']==20
assert keys['HURRICANE']['effect_rules']['sun_accuracy_percent']==50
assert keys['TIDAL_CRASH']['effect_rules']['defense_curl_bonus'] is False  # 2026-11-10 owner supersession: only Rollout receives Curl
assert keys['BLAZE_RUSH']['effect_rules']['crash_on_miss'] is False
assert keys['THUNDERBOLT']['approved']['power']==95 and keys['THUNDER']['approved']['power']==120
assert keys['THUNDER_WAVE']['approved']['target']=='ALL_OTHERS_IN_DOUBLES'
assert keys['DISCHARGE']['approved']['target']==keys['PETAL_BLIZZARD']['approved']['target']=='ALL_OTHERS'
assert keys['SNARL']['approved']['target']=='BOTH_OPPONENTS'
assert all(k not in keys for k in ['NIGHT_TERROR','PHANTOM_GRASP','PSYCHIC_DREAD','DREAD_GLARE','SEISMIC_SURGE','CLOSE_COMBAT','VACUUM_WAVE','PSYCHO_CUT','PSYCHIC_FANGS','AQUA_TAIL','HYPERSPACE_FURY'])
a=json.loads((R/'docs/moves/animation-reuse.json').read_text())['entries'];asm=(R/'data/battle_anim_scripts.s').read_text();labels=set(re.findall(r'^(Move_\w+)::?',asm,re.M))
assert len(a)==67 and len({r['key'] for r in a})==67
assert sum(keys[r['key']]['identity'].startswith('NEW_') for r in a)==56
for entry in a:
 assert not entry['runtime_verified'] and not entry['composition_verified']
 assert all(s['label'] in labels for s in entry['scripts'])
assert sum('historical_approval_label' in r and 'Owner approved' in r['approval_label'] for r in a)==50
assert all(not r['animation']['runtime_verified'] for r in rows)
for r in rows:
 for e in r['evidence']:assert e['source'] in d['sources'] and 1<=e['line']<=d['sources'][e['source']]['lines']
long=[r['name'] for r in new if len(r['name'])>12]
assert set(long)=={'Skitter Slash','Scorching Sands','Petal Blizzard','Dual Wingbeat'}
print('PASS:354 original +56 approved-new inventory,21 original redesign/type policies, all categories, named custom values, excluded proposals and67 source-present animation candidates.')
print('PASS:50 former animation proposals are owner approved for implementation, not runtime verified; no runtime/new-move implementation claim. Four long names and engine translation fields remain explicitly recorded.')
