"""Project only two exact owner-locked learnsets for unchanged historical guards.

The independent current-source test checks whole-file before/after SHA256.
Composed older projections may have changed unrelated rows already; their own
frozen hashes remain mandatory. Mixed or mutated pair bodies are never accepted.
"""
from pathlib import Path
import json,hashlib,re
PATH='src/data/pokemon/level_up_learnsets.h'
def contract():
 return json.loads(Path('tests/specifications/pkm009_jolteon_umbreon_locked.json').read_text())
def prior_locked_pair_source(path,text=None):
 if text is None:text=Path(path).read_text()
 if str(path)!=PATH:return text
 spec=contract();states=[]
 for row in spec['rows']:
  match=re.search(r'static const u16 '+re.escape(row['pointer'])+r'\[\] = \{(.*?)(?=\n\};)',text,re.S)
  assert match,'Missing owner-locked learnset '+row['species']
  body=match.group(1)
  assert body in (row['before_body'],row['after_body']),'Unapproved locked-pair source '+row['species']
  states.append(body==row['after_body'])
 assert all(states) or not any(states),'Partially projected locked-pair source'
 if not any(states):return text
 original_digest=hashlib.sha256(text.encode()).hexdigest()
 for row in spec['rows']:
  assert text.count(row['after_body'])==1,'Ambiguous locked-pair source'
  text=text.replace(row['after_body'],row['before_body'],1)
 if original_digest==spec['after_sha256']:
  assert hashlib.sha256(text.encode()).hexdigest()==spec['before_sha256'],'Locked-pair source does not reverse exactly'
 return text
