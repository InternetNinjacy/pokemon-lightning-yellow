#!/usr/bin/env python3
"""BTL-005 owner-approved water moves #385-386 source contract, not native acceptance."""
from pathlib import Path
import re,json
from approved_water_moves385_changes import prior_water_moves385_source
from approved_moves387_changes import prior_moves387_source

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
assert re.search(r'^#define MOVES_COUNT 387$',prior_moves387_source('include/constants/moves.h',ids),re.M)
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
assert 'gCurrentMove == MOVE_ROLLOUT\n            && gBattleMons[gBattlerAttacker].status2 & STATUS2_DEFENSE_CURL' in rollout_func
assert spec['new_moves'] and moves['TIDAL_CRASH']['effect_rules']['defense_curl_bonus'] is False
assert 'gDynamicBasePower *= 2' in rollout_func
assert 'gLockedMoves[gBattlerAttacker] = gCurrentMove;' in rollout_func
assert 'CancelMultiTurnMoves(gBattlerAttacker);' in rollout_func
assert 'gCurrentMove != MOVE_GRUDGE' in prior_water_moves385_source('src/battle_script_commands.c',rollout_source)  # strict historical projection
for path in ['include/constants/moves.h','src/data/battle_moves.h','src/data/text/move_names.h','src/move_descriptions.c','data/battle_anim_scripts.s']:
 old=prior_water_moves385_source(path)
 assert 'MOVE_VORTEX_SPIN' not in old and 'MOVE_TIDAL_CRASH' not in old,path
print('PASS: approved 385-386 source values, IDs, owner priority, animation, no trap, native Rollout and strict prior-source reversals')

# Approved first batch #387-394. Strictly project all eight production
# additions away before historical checks; do not alter frozen source hashes.
approved387={
 'DISCHARGE':(387,'ELECTRIC','SPECIAL',80,100,15,30,'PARALYZE_HIT','FOES_AND_ALLY','THUNDERBOLT'),
 'VOLT_SWITCH':(388,'ELECTRIC','SPECIAL',70,100,20,0,'HIT','SELECTED',None),
 'THUNDER_FANG':(389,'ELECTRIC','PHYSICAL',65,95,15,10,'THUNDER_FANG','SELECTED',None),
 'WILD_CHARGE':(390,'ELECTRIC','PHYSICAL',90,100,15,0,'RECOIL','SELECTED','VOLT_TACKLE'),
 'SEED_BOMB':(391,'GRASS','PHYSICAL',80,100,15,0,'HIT','SELECTED','BULLET_SEED'),
 'PETAL_BLIZZARD':(392,'GRASS','PHYSICAL',90,100,15,0,'HIT','FOES_AND_ALLY','PETAL_DANCE'),
 'WOOD_HAMMER':(393,'GRASS','PHYSICAL',120,100,15,0,'DOUBLE_EDGE','SELECTED',None),
 'AURA_SPHERE':(394,'FIGHTING','SPECIAL',80,0,20,0,'ALWAYS_HIT','SELECTED','PSYCHIC'),
}
assert re.search(r'^#define MOVES_COUNT 395$',ids,re.M)
for key,(idx,kind,category,power,accuracy,pp,chance,effect,target,art) in approved387.items():
 design=moves[key]
 assert design['proposed_id']==idx
 assert design['approved']['power']==power and design['approved']['accuracy']==accuracy
 assert design['approved']['pp']==pp and design['approved']['priority']==0
 assert re.search(r'^#define MOVE_'+key+r' '+str(idx)+r'$',ids,re.M),key
 record=re.search(r'\[MOVE_'+key+r'\]\s*=\s*\{(.*?)\n    \},',data,re.S)
 assert record,key
 for field,value in {'category':'MOVE_CATEGORY_'+category,'type':'TYPE_'+kind,
   'power':str(power),'accuracy':str(accuracy),'pp':str(pp),
   'secondaryEffectChance':str(chance),'priority':'0',
   'effect':'EFFECT_'+effect,'target':'MOVE_TARGET_'+target}.items():
  assert re.search(r'\.'+field+r'\s*=\s*'+value+r'\s*,',record[1]),(key,field)
 assert ('[MOVE_'+key+'] = _("'+key.replace('_',' ')+'")') in names,key
 assert 'gMoveDescription_'+key+'[]' in descs,key
 assert '[MOVE_'+key+' - 1] = gMoveDescription_'+key in descs,key
 assert anim.count('\t.4byte Move_'+key+'\n')==1,key
 if art:assert re.search(r'^Move_'+key+r':\s*\n\s*goto Move_'+art+r'\s*$',anim,re.M),key
 else:assert 'Move_'+key+':' in anim,key
assert 'BattleScript_VoltSwitchPivot::' in (root/'data/battle_scripts_1.s').read_text()
scripts=(root/'data/battle_scripts_1.s').read_text()
assert 'jumpifmove MOVE_VOLT_SWITCH, BattleScript_VoltSwitchPivot' in scripts
assert 'jumpifmovehadnoeffect BattleScript_MoveEnd' in scripts
assert 'jumpifcantswitch SWITCH_IGNORE_ESCAPE_PREVENTION | BS_ATTACKER, BattleScript_MoveEnd' in scripts
assert 'BattleScript_EffectThunderFang::\n\tsetmoveeffect MOVE_EFFECT_PARALYSIS' in scripts
assert 'jumpifmove MOVE_THUNDER_FANG, BattleScript_IceFangFlinch' in scripts
assert '#define EFFECT_THUNDER_FANG 217' in (root/'include/constants/battle_move_effects.h').read_text()
for path in ['include/constants/moves.h','src/data/battle_moves.h',
 'src/data/text/move_names.h','src/move_descriptions.c','data/battle_anim_scripts.s',
 'include/constants/battle_move_effects.h','data/battle_scripts_1.s']:
 before=prior_moves387_source(path)
 assert 'MOVE_DISCHARGE' not in before and 'MOVE_VOLT_SWITCH' not in before,path
 assert 'MOVE_THUNDER_FANG' not in before,path
 if path in ('include/constants/moves.h','src/data/battle_moves.h',
             'src/data/text/move_names.h','src/move_descriptions.c','data/battle_anim_scripts.s'):
  assert 'MOVE_DISCHARGE' not in prior_water_moves385_source(path),path
print('PASS: staged #387-394 exact owner data, native effect bindings, approved animation scripts and strict historical reversal; runtime acceptance independently gated')
