#!/usr/bin/env python3
"""PKM-009 delegated 75-species natural learnset and full-source preservation contract."""
from pathlib import Path
import sys,unittest,re,json,hashlib
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from approved_pkm009_delegated_001_126_changes import (
 contract_rows,prior_pkm009_delegated_001_126_source)
INFO=(ROOT/'src/data/pokemon/level_up_learnsets.h').read_text()
MOVES=(ROOT/'include/constants/moves.h').read_text()
SPEC=json.loads((ROOT/'tests/specifications/pkm009_delegated_001_126.json').read_text())
class DelegatedHalfDex(unittest.TestCase):
 def test_75_exact_profile_sequences(self):
  data=contract_rows()
  self.assertEqual(len(data),75)
  self.assertEqual(len({x['species'] for x in data}),75)
  self.assertEqual(len({x['dex'] for x in data}),75)
  for row in data:
   with self.subTest(dex=row['dex'],species=row['species']):
    match=re.search(r'static const u16 '+re.escape(row['pointer'])+r'\[\] = \{(.*?)\};',INFO,re.S)
    self.assertIsNotNone(match)
    body=match.group(1)
    actual=[[int(l),m] for l,m in re.findall(r'LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)',body)]
    self.assertEqual(actual,row['after'])
    self.assertEqual(body.count('LEVEL_UP_END'),1)
    self.assertEqual([x[0] for x in actual],sorted(x[0] for x in actual))
    self.assertEqual(row['authority'],'NOV03_DELEGATED_WORKING_NOT_OWNER_EXPLICIT')
    for _,mv in row['after']:
     self.assertRegex(MOVES,rf'(?m)^#define MOVE_{mv}\s+\d+$')
 def test_byte_exact_other_species_preserved(self):
  original=prior_pkm009_delegated_001_126_source('src/data/pokemon/level_up_learnsets.h',INFO)
  blob=b'blob '+str(len(original.encode())).encode()+b'\0'+original.encode()
  self.assertEqual(hashlib.sha1(blob).hexdigest(),SPEC['expected_original_level_up_source_git_blob_sha1'])
if __name__=='__main__': unittest.main()
