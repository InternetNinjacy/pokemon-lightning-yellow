#!/usr/bin/env python3
"""Exact source contracts only; no battle, save, emulator or RNG simulation."""
from pathlib import Path
import hashlib,json,re,unittest
from approved_jolteon_umbreon_changes import contract,prior_locked_pair_source,PATH
ROOT=Path(__file__).resolve().parents[1]
SPEC=contract()
SOURCE=(ROOT/PATH).read_text()
class LockedPairSource(unittest.TestCase):
 def test_exact_approved_sequences_and_ids(self):
  ids={k:int(v) for k,v in re.findall(r'^#define MOVE_(\w+)\s+(\d+)\s*$',(ROOT/'include/constants/moves.h').read_text(),re.M)}
  self.assertEqual(len(SPEC['rows']),2)
  for row in SPEC['rows']:
   m=re.search(r'static const u16 '+row['pointer']+r'\[\] = \{(.*?)\n\};',SOURCE,re.S)
   self.assertIsNotNone(m)
   self.assertEqual([[int(l),key] for l,key in re.findall(r'LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)',m[1])],row['after'])
   self.assertEqual(m[1].count('LEVEL_UP_END'),1)
   for level,key in row['after']:
    self.assertIn(key,ids);self.assertLess(ids[key],512);self.assertGreaterEqual(level,1);self.assertLessEqual(level,100)
 def test_exact_whole_file_and_inverse(self):
  self.assertEqual(hashlib.sha256(SOURCE.encode()).hexdigest(),SPEC['after_sha256'])
  old=prior_locked_pair_source(PATH,SOURCE)
  self.assertEqual(hashlib.sha256(old.encode()).hexdigest(),SPEC['before_sha256'])
  self.assertEqual(prior_locked_pair_source(PATH,old),old)
 def test_unrelated_source_and_saved_layout_preserved(self):
  for path,digest in SPEC['unchanged_sha256'].items():
   self.assertEqual(hashlib.sha256((ROOT/path).read_bytes()).hexdigest(),digest,path)
 def test_stats_abilities_types_unchanged(self):
  source=(ROOT/'src/data/pokemon/species_info.h').read_text()
  fields=('baseHP','baseAttack','baseDefense','baseSpAttack','baseSpDefense','baseSpeed')
  for row in SPEC['rows']:
   body=re.search(r'\['+row['species']+r'\] =\s*\{(.*?)\n    \},',source,re.S)[1]
   self.assertEqual([int(re.search(r'\.'+field+r' = (\d+)',body)[1]) for field in fields],row['stats'])
   self.assertEqual(re.findall(r'ABILITY_(\w+)',re.search(r'\.abilities = \{(.*?)\}',body)[1]),row['abilities'])
 def test_all_six_locked_family_pools_stay_distinct(self):
  pools=[]
  for name in ('Eevee','Vaporeon','Jolteon','Flareon','Espeon','Umbreon'):
   body=re.search(r'static const u16 s'+name+r'LevelUpLearnset\[\] = \{(.*?)\};',SOURCE,re.S)[1]
   pool=re.findall(r'LEVEL_UP_MOVE\(\d+, MOVE_(\w+)\)',body)
   self.assertEqual(len(pool),11,name);self.assertEqual(len(pool),len(set(pool)),name);pools+=pool
  self.assertEqual(len(pools),len(set(pools)))
 def test_mutated_and_partial_pair_rejected(self):
  row=SPEC['rows'][0]
  partial=SOURCE.replace(row['after_body'],row['before_body'],1)
  with self.assertRaises(AssertionError):prior_locked_pair_source(PATH,partial)
  mutated=SOURCE.replace(row['after_body'],row['after_body'].replace('MOVE_CHARGE','MOVE_SPLASH'),1)
  with self.assertRaises(AssertionError):prior_locked_pair_source(PATH,mutated)
  # Unrelated mutations must fail whole-file preservation after projection.
  mutated=SOURCE+'\n/* unapproved unrelated edit */\n'
  self.assertNotEqual(hashlib.sha256(prior_locked_pair_source(PATH,mutated).encode()).hexdigest(),SPEC['before_sha256'])
if __name__=='__main__':unittest.main()
