"""Exact reverse of six delegated natural-learnset changes for prior hashes."""
import re
def prior_pkm009_birds_rodents_source(path,text):
 if path!='src/data/pokemon/level_up_learnsets.h': return text
 from test_pkm009_birds_rodents import ORIGINAL,TARGET
 for name,old in ORIGINAL.items():
  if old==TARGET[name]:continue
  rx=r'(static const u16 s'+name+r'LevelUpLearnset\[\] = \{)(.*?)(\n\};)'
  m=re.search(rx,text,re.S)
  assert m,'Missing learnset '+name
  actual=[[int(l),move] for l,move in re.findall(r'LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)',m.group(2))]
  assert actual==TARGET[name],'Unexpected learnset '+name
  assert m.group(2).count('LEVEL_UP_END')==1
  body='\n'+'\n'.join('    LEVEL_UP_MOVE(%d, MOVE_%s),'%(l,mv) for l,mv in old)+'\n    LEVEL_UP_END'
  text=text[:m.start(2)]+body+text[m.end(2):]
 return text
