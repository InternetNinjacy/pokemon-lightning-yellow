#!/usr/bin/env python3
"""BTL-005 owner-approved water moves #385-386 source contract, not native acceptance."""
from pathlib import Path
import re,json
from approved_water_moves385_changes import prior_water_moves385_source

root=Path('.')
spec=json.loads((root/'docs/moves/approved-design-overlays.json').read_text())
moves={m['key']:m for m in spec['new_moves']}
ids=(root/'include/constants/moves.h').read_text()
data=(root/'src/data/battle_moves.h').read_text()
names=(root/'src/data/text/move_names.h').read_text()
descs=(root/'src/move_descriptions.c').read_text()
anim=(root/'data/battle_anim_scripts.s').read_text()
expected={
 'VORTEX_SPIN':(385,55,100,25,30,'EFFECT_CONFUSE_HIT','Move_WHIRLPOOL',False),
 'TIDAL_CRASH':(386,30,90,20,0,'EFFECT_ROLLOUT','Move_SURF',True),
}
assert re.search(r'^#define MOVES_COUNT 387$',ids,re.M)
for key,(idx,power,accuracy,pp,chance,effect,art,contact) in expected.items():
 design=moves[key]
 assert design['proposed_id']==idx
 assert design['approved']['priority']==0
 assert design['approved']['power']==power
 assert design['approved']['accuracy']==accuracy
 assert design['approved']['pp']==pp
 assert re.search(r'^#define MOVE_'+key+r' '+str(idx)+r'$',ids,re.M)
 record=re.search(r'\[MOVE_'+key+r'\]\s*=\s*\{(.*?)\n    \},',data,re.S)
 assert record,key
 body=record[1]
 fields={
  'category':'MOVE_CATEGORY_PHYSICAL','type':'TYPE_WATER','power':str(power),
  'accuracy':str(accuracy),'pp':str(pp),'priority':'0',
  'secondaryEffectChance':str(chance),'target':'MOVE_TARGET_SELECTED','effect':effect,
 }
 for name,value in fields.items():
  assert re.search(r'\.'+name+r'\s*=\s*'+re.escape(value)+r'\s*,',body), (key,name)
 assert ('FLAG_MAKES_CONTACT' in body)==contact,key
 assert re.search(r'\[MOVE_'+key+r'\]\s*=\s*_\("'+key.replace('_',' ')+r'"\)',names),key
 assert 'gMoveDescription_'+key+'[]' in descs
 assert '[MOVE_'+key+' - 1] = gMoveDescription_'+key in descs
 assert re.search(r'^Move_'+key+r':\s*\n\s*goto '+art+r'\s*$',anim,re.M),key
 assert anim.count('\t.4byte Move_'+key+'\n')==1,key
 assert 'STATUS' not in design['approved']['category'],key
# Vortex Spin is NOT the Whirlpool wrapping/trap mechanic.
assert 'EFFECT_CONFUSE_HIT' in data
assert 'EFFECT_ROLLOUT' in data
assert 'gDynamicBasePower *= 2' in (root/'src/battle_script_commands.c').read_text()
rollout_source=(root/'src/battle_script_commands.c').read_text()
rollout_func=rollout_source.split('static void Cmd_rolloutdamagecalculation(void)\n{',1)[1].split('\nstatic void Cmd_jumpifconfusedandstatmaxed',1)[0]
assert 'gCurrentMove != MOVE_GRUDGE\n            && gBattleMons[gBattlerAttacker].status2 & STATUS2_DEFENSE_CURL' in rollout_func
assert spec['new_moves'] and moves['TIDAL_CRASH']['effect_rules']['defense_curl_bonus'] is True
assert 'gDynamicBasePower *= 2' in rollout_func
assert 'gLockedMoves[gBattlerAttacker] = gCurrentMove;' in rollout_func
assert 'CancelMultiTurnMoves(gBattlerAttacker);' in rollout_func
assert prior_water_moves385_source('src/battle_script_commands.c',rollout_source) == rollout_source  # Exact unchanged ancestor engine semantics
for path in ['include/constants/moves.h','src/data/battle_moves.h','src/data/text/move_names.h','src/move_descriptions.c','data/battle_anim_scripts.s']:
 old=prior_water_moves385_source(path)
 assert 'MOVE_VORTEX_SPIN' not in old and 'MOVE_TIDAL_CRASH' not in old,path
print('PASS: approved 385-386 source values, IDs, owner priority, animation, no trap, native Rollout and strict prior-source reversals')
