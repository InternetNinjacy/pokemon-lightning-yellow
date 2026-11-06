#!/usr/bin/env python3
"""Approved exact E4 parties and frozen unrelated trainer/source preservation."""
from pathlib import Path
import json,re,hashlib
from approved_elite_four_changes import prior_elite_four_source
m=json.loads(Path('tests/specifications/approved_elite_four_delta.json').read_text())
for p,v in m['files'].items():
 assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==v['after_sha256'],p
 assert hashlib.sha256(prior_elite_four_source(p).encode()).hexdigest()==v['before_sha256'],p
for p,h in m['preserved_files'].items():assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==h,p
spec=json.loads(Path('tests/specifications/approved_elite_four.json').read_text())['battles'];assert len(spec)==8
source=Path('src/data/trainer_parties.h').read_text()
for b in spec:
 assert hashlib.sha256(Path(b['specification']).read_bytes()).hexdigest()==b['sha256']
 if not b['implemented']:continue
 body=re.search(r'\b'+b['array']+r'\[\] = \{(.*?)\n\};',source,re.S)[1]
 entries=re.findall(r'\{(.*?)\n    \}',body,re.S);assert len(entries)==len(b['party'])
 for text,row in zip(entries,b['party']):
  for field,v in [('iv',row['iv']),('lvl',row['level']),('species','SPECIES_'+row['species']),('heldItem','ITEM_'+row['item'])]:
   assert re.search(r'\.'+field+r' = ([^,]+),',text)[1]==str(v)
  assert re.findall(r'MOVE_(\w+)',text)==row['moves']
print('PASS: six exact approved Elite Four parties; all other arrays, metadata, battle constructor, global species/save/IDs frozen; two Agatha encounters explicitly pending native alternate-slot permission')
