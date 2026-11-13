#!/usr/bin/env python3
"""PKM-009 original 252-regional-species TM/HM source parity, not TM51 approval.

Independent current production TMHM_LEARNSET extraction checked against a
frozen 252-species manifest extracted from the historical current-source
matrix; never treats unapproved historical proposals as new TM permission.
"""
from pathlib import Path
import json,re,hashlib,unittest
ROOT=Path(__file__).resolve().parents[1]
SPEC=json.loads((ROOT/'tests/specifications/pkm009_tmhm_252_source_contract.json').read_text())
SOURCE=(ROOT/'src/data/pokemon/tmhm_learnsets.h').read_text()
ITEMS=(ROOT/'include/constants/items.h').read_text()
class SpeciesTMHMParity(unittest.TestCase):
 def test_exact_252_source_machines(self):
  rows=SPEC['rows']
  self.assertEqual(len(rows),252)
  self.assertEqual([x['dex'] for x in rows],list(range(1,253)))
  self.assertEqual(len({x['species'] for x in rows}),252)
  self.assertEqual(SPEC['source_species_count'],252)
  count=0
  found=set()
  for row in rows:
   species=row['species']
   with self.subTest(species=species):
    matches=list(re.finditer(r'^\s*\['+re.escape(species)+r'\]\s*=\s*TMHM_LEARNSET\(',SOURCE,re.M))
    self.assertEqual(len(matches),1)
    tail=SOURCE[matches[0].start():]
    nextSpecies=re.search(r'\n\s*\[SPECIES_\w+\]\s*=',tail[1:])
    block=tail[:nextSpecies.start()+1] if nextSpecies else tail.split('\n};',1)[0]
    got=re.findall(r'TMHM\((TM\d\d_[A-Z0-9_]+|HM\d\d_[A-Z0-9_]+)\)',block)
    self.assertEqual(got,row['machines'])
    self.assertEqual(len(got),len(set(got)),species)
    self.assertEqual(sum(n.startswith('TM') for n in got)<=50,True)
    count+=len(got)
    found.update(got)
  self.assertEqual(count,5831)
  self.assertEqual(SPEC['compatibility_edges'],5831)
  self.assertEqual(len(found),58)
  self.assertEqual(SPEC['original_machines'],58)
  self.assertEqual({int(x[2:4]) for x in found if x.startswith('TM')},set(range(1,51)))
  self.assertEqual({int(x[2:4]) for x in found if x.startswith('HM')},set(range(1,9)))
 def test_file_byte_exact_and_no_unauthorized_tm51(self):
  data=SOURCE.encode()
  blob=b'blob '+str(len(data)).encode()+b'\0'+data
  self.assertEqual(hashlib.sha1(blob).hexdigest(),SPEC['source_current_main_tmhm_git_blob_sha1'])
  self.assertNotIn('TM51_',SOURCE)
  self.assertNotIn('ITEM_TM51_',ITEMS)
  self.assertIn('#define ITEM_HM08 346',ITEMS)
if __name__=='__main__':
 unittest.main()
