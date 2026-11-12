"""Exact reversible 75-row delegated PKM-009 natural level-up source manifest.

This helper retains existing whole-file source checks without weakening them.
The rows are November 3 globally delegated *working* recommendations, not
individual owner-locked species designs.
"""
import json,re
from pathlib import Path
_SPEC='tests/specifications/pkm009_delegated_001_126.json'
def contract_rows():
 return json.loads(Path(_SPEC).read_text())['rows']
def prior_pkm009_delegated_001_126_source(path,text):
 if path!='src/data/pokemon/level_up_learnsets.h':
  return text
 for row in contract_rows():
  name=row['pointer']
  rx=r'(static const u16 '+re.escape(name)+r'\[\] = \{)(.*?)(\n\};)'
  match=re.search(rx,text,re.S)
  assert match,'Missing species learnset '+str(row['species'])
  body=match.group(2)
  actual=[[int(l),mv] for l,mv in re.findall(r'LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)',body)]
  assert actual==row['after'],'Unexpected learnset change '+row['species']
  assert body.count('LEVEL_UP_END')==1,row['species']
  old='\n'+'\n'.join('    LEVEL_UP_MOVE(%d, MOVE_%s),'%(level,move) for level,move in row['before'])+'\n    LEVEL_UP_END'
  text=text[:match.start(2)]+old+text[match.end(2):]
 return text
