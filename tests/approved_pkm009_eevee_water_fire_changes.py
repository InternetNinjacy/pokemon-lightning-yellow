"""Reversible exact PKM-009 Eevee/Vaporeon/Flareon owner-locked profile deltas.

Used only for earlier cross-workstream whole-file SHA preservation checks.
Forward exact profiles must also pass test_pkm009_eevee_water_fire.py.
"""
import re
_ORIGINAL={
'Eevee':[(1,'TACKLE'),(1,'TAIL_WHIP'),(1,'HELPING_HAND'),(8,'SAND_ATTACK'),(16,'GROWL'),(23,'QUICK_ATTACK'),(30,'BITE'),(36,'BATON_PASS'),(42,'TAKE_DOWN')],
'Vaporeon':[(1,'TACKLE'),(1,'TAIL_WHIP'),(1,'HELPING_HAND'),(8,'SAND_ATTACK'),(16,'WATER_GUN'),(23,'QUICK_ATTACK'),(30,'BITE'),(36,'AURORA_BEAM'),(42,'HAZE'),(47,'ACID_ARMOR'),(52,'HYDRO_PUMP')],
'Flareon':[(1,'TACKLE'),(1,'TAIL_WHIP'),(1,'HELPING_HAND'),(8,'SAND_ATTACK'),(16,'EMBER'),(23,'QUICK_ATTACK'),(30,'BITE'),(36,'FIRE_SPIN'),(42,'SMOG'),(47,'LEER'),(52,'FLAMETHROWER')]}
_STATS={'EEVEE':((95,115,85,50,85,95),(55,55,50,45,65,55)),
        'FLAREON':((85,130,85,50,110,65),(65,130,60,95,110,65))}
_FIELDS=('baseHP','baseAttack','baseDefense','baseSpAttack','baseSpDefense','baseSpeed')
def prior_pkm009_eevee_water_fire_source(path,text):
 if path=='src/data/pokemon/species_info.h':
  from test_pkm009_eevee_water_fire import APPROVED
  for name,(new_stats,old_stats) in _STATS.items():
   assert new_stats==APPROVED[name.title()]['stats']
   pattern=r'(\[SPECIES_'+name+r'\]\s*=\s*\{)(.*?)(\n    \},)'
   m=re.search(pattern,text,re.S)
   assert m,'Missing species '+name
   body=m.group(2)
   for field,new,old in zip(_FIELDS,new_stats,old_stats):
    if new==old: continue
    token='.'+field+' = '+str(new)+','
    assert body.count(token)==1,'Unexpected '+name+' '+field
    body=body.replace(token,'.'+field+' = '+str(old)+',',1)
   text=text[:m.start(2)]+body+text[m.end(2):]
 elif path=='src/data/pokemon/level_up_learnsets.h':
  from test_pkm009_eevee_water_fire import APPROVED
  for name,old in _ORIGINAL.items():
   pattern=r'(static const u16 s'+name+r'LevelUpLearnset\[\] = \{)(.*?)(\n\};)'
   m=re.search(pattern,text,re.S)
   assert m,'Missing learnset '+name
   body=m.group(2)
   actual=[(int(l),move) for l,move in re.findall(r'LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)',body)]
   assert actual==APPROVED[name]['moves'],'Unexpected '+name+' learnset'
   assert body.count('LEVEL_UP_END')==1
   oldbody='\n'+'\n'.join('    LEVEL_UP_MOVE(%d, MOVE_%s),'%a for a in old)+'\n    LEVEL_UP_END'
   text=text[:m.start(2)]+oldbody+text[m.end(2):]
 return text
