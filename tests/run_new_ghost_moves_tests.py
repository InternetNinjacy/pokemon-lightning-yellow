#!/usr/bin/env python3
"""Exact approved Ghost source/effect/animation coverage; native execution separate."""
from pathlib import Path
import hashlib,json,re
from approved_new_ghost_move_changes import prior_new_ghost_move_source
from approved_skitter_slash_changes import prior_skitter_slash_source

m=json.loads(Path('tests/specifications/approved_new_ghost_move_delta.json').read_text())
for path,r in m['files'].items():
 assert hashlib.sha256(prior_skitter_slash_source(path).encode()).hexdigest()==r['after_sha256'],path
 assert hashlib.sha256(prior_new_ghost_move_source(path).encode()).hexdigest()==r['before_sha256'],path
rows=json.loads(Path('docs/moves/move-specification.json').read_text())['moves']
expected=[('SOUL_DRAIN',358,'PHYSICAL',50,'EFFECT_ABSORB',0),('WEARY_TOUCH',359,'PHYSICAL',75,'EFFECT_SLEEP_HIT',22),('DREAM_HAUNT',360,'SPECIAL',55,'EFFECT_HIT',0)]
for key,id,category,power,effect,chance in expected:
 r=next(r for r in rows if r['key']==key);c=r['current'];assert r['id']==id and not r['delta']
 assert (c['type'],c['category'],c['power'],c['accuracy'],c['pp'],c['priority'],c['effect'],c['secondaryEffectChance'],c['target'])==('GHOST',category,power,100,15,0,effect,chance,'MOVE_TARGET_SELECTED')
 assert ('FLAG_MAKES_CONTACT' in c['flags'])==(key!='DREAM_HAUNT')
 assert r['effect_rules']
assert sum(r['id'] is not None and r['id'] <= 360 for r in rows)==360 and len(rows)==410
scripts=Path('data/battle_scripts_1.s').read_text()
table=scripts.split('gBattleScriptsForMoveEffects::',1)[1].split('\nBattleScript_EffectHit::',1)[0]
assert len(re.findall(r'\.4byte BattleScript_',table))==216
assert re.findall(r'\.4byte (BattleScript_\w+)',table)[215]=='BattleScript_EffectSwarmCloud'
assert re.findall(r'\.4byte (BattleScript_\w+)',table)[214]=='BattleScript_EffectSleepHit'
assert 'BattleScript_EffectSleepHit::\n\tsetmoveeffect MOVE_EFFECT_SLEEP\n\tgoto BattleScript_EffectHit\n' in scripts
asm=Path('data/battle_anim_scripts.s').read_text()
for key,parts in zip(['SOUL_DRAIN','WEARY_TOUCH','DREAM_HAUNT'],[['LICK','MEGA_DRAIN'],['LICK','HYPNOSIS'],['NIGHT_SHADE','DREAM_EATER']]):
 expected_body=''.join(re.search(r'Move_'+p+r':\n(.*?)\n\tend',asm,re.S)[1]+'\n\twaitforvisualfinish\n' for p in parts)
 actual=re.search(r'Move_'+key+r':\n(.*?)\tend',asm,re.S)[1]
 assert actual==expected_body,key
for path in ['src/data/pokemon/level_up_learnsets.h','src/data/pokemon/tmhm_learnsets.h','src/data/trainer_parties.h']:
 assert not any('MOVE_'+k in Path(path).read_text() for k,*_ in expected),path
print('PASS: exact three approved Ghost designs, complete appended sleep-hit dispatch, original bodies composed inline, exact old-source reversal and no distribution (static, not runtime proof).')
