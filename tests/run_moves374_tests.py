#!/usr/bin/env python3
"""Tier1/2 static contracts, no runtime claims from table presence."""
from pathlib import Path
import json,hashlib,re
from approved_moves374_changes import prior_moves374_source
m=json.loads(Path('tests/specifications/moves374_delta.json').read_text())
for p,r in m['files'].items():
 assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==r['after_sha256'],p
 assert hashlib.sha256(prior_moves374_source(p).encode()).hexdigest()==r['before_sha256'],p
rows=[('ICICLE_CRASH',374,'ICE','PHYSICAL',85,90,10,'FLINCH_HIT',30,'SELECTED',False,False),('DRAGON_PULSE',375,'DRAGON','SPECIAL',85,100,10,'HIT',0,'SELECTED',False,True),('DRAGON_RUSH',376,'DRAGON','PHYSICAL',100,75,10,'FLINCH_HIT',20,'SELECTED',True,False),('SNARL',377,'DARK','SPECIAL',55,95,15,'SPECIAL_ATTACK_DOWN_HIT',100,'BOTH',False,False),('DARK_PULSE',378,'DARK','SPECIAL',80,100,15,'FLINCH_HIT',20,'SELECTED',False,False),('NIGHT_DAZE',379,'DARK','SPECIAL',85,95,10,'ACCURACY_DOWN_HIT',40,'SELECTED',False,False),('NIGHT_SLASH',380,'DARK','PHYSICAL',70,100,15,'HIGH_CRITICAL',0,'SELECTED',True,True)]
s=Path('src/data/battle_moves.h').read_text();constants=Path('include/constants/moves.h').read_text();a=Path('data/battle_anim_scripts.s').read_text()
for key,idx,typ,cat,power,acc,pp,effect,chance,target,contact,king in rows:
 block=re.search(r'\[MOVE_'+key+r'\]\s*=\s*\{(.*?)\n    \},',s,re.S)[1]
 for f,v in dict(category='MOVE_CATEGORY_'+cat,effect='EFFECT_'+effect,type='TYPE_'+typ,power=power,accuracy=acc,pp=pp,secondaryEffectChance=chance,target='MOVE_TARGET_'+target,priority=0).items():assert f'.{f} = {v},' in block,(key,f)
 assert ('FLAG_MAKES_CONTACT' in block)==contact and ('FLAG_KINGS_ROCK_AFFECTED' in block)==king,key
 assert '#define MOVE_'+key+' '+str(idx) in constants
 assert a.count('.4byte Move_'+key+'\n')==1 and a.count('Move_'+key+':')==1
 for p in ['src/data/pokemon/level_up_learnsets.h','src/data/pokemon/tmhm_learnsets.h','src/data/trainer_parties.h']:assert 'MOVE_'+key not in Path(p).read_text(),p
assert '#define MOVES_COUNT 381' in constants
for key,alias in [('ICICLE_CRASH','ICICLE_SPEAR'),('DRAGON_PULSE','DRAGON_BREATH'),('SNARL','UPROAR'),('DARK_PULSE','NIGHT_SHADE'),('NIGHT_DAZE','FAINT_ATTACK'),('NIGHT_SLASH','SLASH')]:assert 'Move_'+key+':\n\tgoto Move_'+alias+'\n' in a
rush=a.split('Move_DRAGON_RUSH:\n')[1].split('\tend\n',1)[0]
assert 'call Move_' not in rush and 'ANIM_DEF_PARTNER' not in rush and 'DragonClawFireSpiral' in rush and 'AnimTask_WindUpLunge' in rush
assert 'MOVE_SNARL, SOUND_MOVES_END' in Path('src/battle_util.c').read_text()
assert '\tif_move MOVE_SNARL, Score_Minus10' in Path('data/battle_ai_scripts.s').read_text()
print('PASS: #374–380 exact approved data, engine flags, existing-art dispatch, Snarl sound/AI routing, no distribution and strict historical reversals (static).')
