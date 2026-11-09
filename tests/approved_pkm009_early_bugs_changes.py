"""Strict reversible November 3 delegated early Bug learnset timings.

These two designs have global delegated recommendation authority, not
individual owner-locked approval. Other source history remains protected.
"""
import re
ORIGINAL={
'Butterfree':[(1,'CONFUSION'),(10,'CONFUSION'),(13,'POISON_POWDER'),(14,'STUN_SPORE'),(15,'SLEEP_POWDER'),(18,'SUPERSONIC'),(23,'WHIRLWIND'),(28,'GUST'),(34,'PSYBEAM'),(40,'SAFEGUARD'),(47,'SILVER_WIND')],
'Beedrill':[(1,'FURY_ATTACK'),(10,'FURY_ATTACK'),(15,'FOCUS_ENERGY'),(20,'TWINEEDLE'),(25,'RAGE'),(30,'PURSUIT'),(35,'PIN_MISSILE'),(40,'AGILITY'),(45,'ENDEAVOR')]}
def prior_pkm009_early_bugs_source(path,text):
 if path!='src/data/pokemon/level_up_learnsets.h':
  return text
 from test_pkm009_early_bugs import EXPECTED
 for name,old in ORIGINAL.items():
  rexp=r'(static const u16 s'+name+r'LevelUpLearnset\[\] = \{)(.*?)(\n\};)'
  m=re.search(rexp,text,re.S)
  assert m,'Missing '+name
  body=m.group(2)
  actual=[(int(l),move) for l,move in re.findall(r'LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)',body)]
  assert actual==EXPECTED[name],'Unreviewed delegated learnset drift: '+name
  assert body.count('LEVEL_UP_END')==1
  prior_body='\n'+'\n'.join('    LEVEL_UP_MOVE(%d, MOVE_%s),'%x for x in old)+'\n    LEVEL_UP_END'
  text=text[:m.start(2)]+prior_body+text[m.end(2):]
 return text
