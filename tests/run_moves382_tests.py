#!/usr/bin/env python3
"""Exact source contract for approved382–384, not an emulator substitute."""
from pathlib import Path
import json,hashlib,re
from approved_moves382_changes import prior_moves382_source
from approved_moves387_changes import prior_moves387_source
from approved_btl_weather_integration import without_weather
m=json.loads(Path('tests/specifications/moves382_delta.json').read_text())
for p,r in m['files'].items():
 assert hashlib.sha256(without_weather(p,prior_moves387_source(p)).encode()).hexdigest()==r['after_sha256'],p
 assert hashlib.sha256(prior_moves382_source(p).encode()).hexdigest()==r['before_sha256'],p
rows=[('FOUL_PLAY',382,'DARK',95,100,15,0,'HIT',0,True),('AQUA_JET',383,'WATER',40,100,20,1,'HIT',0,True),('LIQUIDATION',384,'WATER',85,100,10,0,'DEFENSE_DOWN_HIT',20,False)]
s=Path('src/data/battle_moves.h').read_text();c=Path('include/constants/moves.h').read_text();a=Path('data/battle_anim_scripts.s').read_text()
for key,idx,typ,power,acc,pp,priority,effect,chance,king in rows:
 b=re.search(r'\[MOVE_'+key+r'\]\s*=\s*\{(.*?)\n    \},',s,re.S)[1]
 for f,v in dict(category='MOVE_CATEGORY_PHYSICAL',effect='EFFECT_'+effect,type='TYPE_'+typ,power=power,accuracy=acc,pp=pp,secondaryEffectChance=chance,target='MOVE_TARGET_SELECTED',priority=priority).items():assert f'.{f} = {v},' in b,(key,f)
 assert 'FLAG_MAKES_CONTACT' in b and ('FLAG_KINGS_ROCK_AFFECTED' in b)==king
 assert '#define MOVE_'+key+' '+str(idx) in c and a.count('.4byte Move_'+key+'\n')==1
 for p in ['src/data/pokemon/level_up_learnsets.h','src/data/pokemon/tmhm_learnsets.h','src/data/trainer_parties.h']:assert 'MOVE_'+key not in Path(p).read_text(),p
assert '#define MOVES_COUNT 387' in prior_moves387_source('include/constants/moves.h',c)  # frozen #382-386 ancestor count
assert 'Move_FOUL_PLAY:\n\tgoto Move_THIEF\n' in a and 'Move_LIQUIDATION:\n\tgoto Move_WATERFALL\n' in a
jet=a.split('Move_AQUA_JET:\n')[1].split('\tend\n',1)[0];assert 'AnimTask_TraceMonBlended' in jet and 'gWaterGunProjectileSpriteTemplate' in jet and 'call Move_' not in jet and 'ANIM_DEF_PARTNER' not in jet
p=Path('src/pokemon.c').read_text();assert 'attackStatSource = (move == MOVE_FOUL_PLAY) ? defender : attacker;' in p and 'attack = attackStatSource->attack;' in p
assert p.count('APPLY_STAT_MOD(damage, attackStatSource, attack, STAT_ATK)')==2
print('PASS: exact382–384 data/flags, target Attack/stage source, prior source reversal, selected-target approved existing art and no distribution (static).')
