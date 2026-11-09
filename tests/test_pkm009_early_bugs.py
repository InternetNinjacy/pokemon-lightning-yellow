#!/usr/bin/env python3
"""November 3 delegated early Bug-line natural progression only.

Source stats, typing, ability slots, TM/HM eligibility and evolutions must remain
unchanged; the exact 2-species recommendations are not individual owner locks.
"""
from pathlib import Path
import re,unittest
ROOT=Path(__file__).resolve().parents[1]
LEARN=(ROOT/'src/data/pokemon/level_up_learnsets.h').read_text()
INFO=(ROOT/'src/data/pokemon/species_info.h').read_text()
EVOLUTION=(ROOT/'src/data/pokemon/evolution.h').read_text()
MOVES=(ROOT/'include/constants/moves.h').read_text()
EXPECTED={
'Butterfree':[(1,'CONFUSION'),(10,'CONFUSION'),(13,'POISON_POWDER'),(14,'STUN_SPORE'),(15,'SLEEP_POWDER'),(18,'SUPERSONIC'),(23,'WHIRLWIND'),(23,'GUST'),(32,'SILVER_WIND'),(34,'PSYBEAM'),(40,'SAFEGUARD')],
'Beedrill':[(1,'FURY_ATTACK'),(10,'FURY_ATTACK'),(15,'FOCUS_ENERGY'),(20,'TWINEEDLE'),(25,'RAGE'),(30,'PURSUIT'),(34,'AERIAL_ACE'),(35,'PIN_MISSILE'),(40,'AGILITY'),(45,'ENDEAVOR')]}
class EarlyBugsSourceContract(unittest.TestCase):
 def test_exact_learnsets(self):
  for name,expected in EXPECTED.items():
   with self.subTest(species=name):
    m=re.search(r'static const u16 s'+name+r'LevelUpLearnset\[\] = \{(.*?)\};',LEARN,re.S)
    self.assertIsNotNone(m)
    body=m.group(1)
    actual=[(int(l),move) for l,move in re.findall(r'LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)',body)]
    self.assertEqual(actual,expected)
    self.assertEqual(body.count('LEVEL_UP_END'),1)
    for _,move in actual:
     self.assertRegex(MOVES,rf'(?m)^#define MOVE_{move}\s+\d+$')
 def test_evolution_and_ability_unchanged(self):
  for name,typ,ability,stats in [
   ('Butterfree',('BUG','FLYING'),('COMPOUND_EYES','NONE'),(60,45,50,80,80,70)),
   ('Beedrill',('BUG','POISON'),('SWARM','NONE'),(65,80,40,45,80,75))]:
   with self.subTest(species=name):
    match=re.search(r'\[SPECIES_'+name.upper()+r'\]\s*=\s*\{(.*?)\n    \},',INFO,re.S)
    self.assertIsNotNone(match)
    body=match.group(1)
    self.assertIn('.types = {TYPE_'+typ[0]+', TYPE_'+typ[1]+'}',body)
    self.assertIn('.abilities = {ABILITY_'+ability[0]+', ABILITY_'+ability[1]+'}',body)
    fields=['baseHP','baseAttack','baseDefense','baseSpAttack','baseSpDefense','baseSpeed']
    self.assertEqual(tuple(int(re.search(r'\.'+f+r'\s*=\s*(\d+)',body).group(1)) for f in fields),stats)
  self.assertRegex(EVOLUTION,r'\[SPECIES_METAPOD\]\s*=\s*\{\{EVO_LEVEL,\s*10,\s*SPECIES_BUTTERFREE\}\}')
  self.assertRegex(EVOLUTION,r'\[SPECIES_KAKUNA\]\s*=\s*\{\{EVO_LEVEL,\s*10,\s*SPECIES_BEEDRILL\}\}')
if __name__=='__main__': unittest.main()
