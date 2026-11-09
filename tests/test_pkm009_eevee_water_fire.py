#!/usr/bin/env python3
"""Exact owner-locked Eevee/Vaporeon/Flareon PKM-009 source contracts."""
from pathlib import Path
import re,unittest
ROOT=Path(__file__).resolve().parents[1]
INFO=(ROOT/'src/data/pokemon/species_info.h').read_text()
LEARN=(ROOT/'src/data/pokemon/level_up_learnsets.h').read_text()
MOVES=(ROOT/'include/constants/moves.h').read_text()
APPROVED={
'Eevee':{'stats':(95,115,85,50,85,95),'typing':('NORMAL','NORMAL'),'ability':('RUN_AWAY','NONE'),'moves':[(1,'TACKLE'),(1,'TAIL_WHIP'),(6,'QUICK_ATTACK'),(12,'COVET'),(18,'FOCUS_ENERGY'),(24,'HEADBUTT'),(30,'CHARM'),(36,'BODY_SLAM'),(42,'HOWL'),(48,'DOUBLE_EDGE'),(54,'BATON_PASS')]},
'Vaporeon':{'stats':(130,65,60,110,95,65),'typing':('WATER','WATER'),'ability':('WATER_ABSORB','NONE'),'moves':[(1,'BUBBLE'),(1,'HELPING_HAND'),(8,'WATER_GUN'),(14,'YAWN'),(18,'WATER_PULSE'),(25,'AURORA_BEAM'),(30,'WISH'),(36,'SURF'),(42,'ACID_ARMOR'),(48,'ICE_BEAM'),(54,'HYDRO_PUMP')]},
'Flareon':{'stats':(85,130,85,50,110,65),'typing':('FIRE','FIRE'),'ability':('FLASH_FIRE','NONE'),'moves':[(1,'EMBER'),(1,'GROWL'),(8,'FLASHFIRE'),(14,'FLAME_WHEEL'),(18,'WILL_O_WISP'),(24,'TAKE_DOWN'),(30,'SUNNY_DAY'),(36,'FLAME_FANG'),(42,'ENDURE'),(48,'REVERSAL'),(54,'BLAZE_RUSH')]}
}
FIELDS=('baseHP','baseAttack','baseDefense','baseSpAttack','baseSpDefense','baseSpeed')
class LockedEeveeProfiles(unittest.TestCase):
 def test_exact_species_rows(self):
  for name,approval in APPROVED.items():
   with self.subTest(species=name):
    m=re.search(r'\[SPECIES_'+name.upper()+r'\]\s*=\s*\{(.*?)\n    \},',INFO,re.S)
    self.assertIsNotNone(m)
    b=m.group(1)
    self.assertEqual(tuple(int(re.search(r'\.'+x+r'\s*=\s*(\d+)',b).group(1)) for x in FIELDS),approval['stats'])
    self.assertEqual(re.search(r'\.types = \{TYPE_(\w+), TYPE_(\w+)\}',b).groups(),approval['typing'])
    self.assertEqual(re.search(r'\.abilities = \{ABILITY_(\w+), ABILITY_(\w+)\}',b).groups(),approval['ability'])
    self.assertEqual(sum(approval['stats']),525)
 def test_exact_complete_learnsets(self):
  for name,approval in APPROVED.items():
   with self.subTest(species=name):
    m=re.search(r'static const u16 s'+name+r'LevelUpLearnset\[\] = \{(.*?)\};',LEARN,re.S)
    self.assertIsNotNone(m)
    body=m.group(1)
    entries=[(int(l),n) for l,n in re.findall(r'LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)',body)]
    self.assertEqual(entries,approval['moves'])
    self.assertEqual(body.count('LEVEL_UP_END'),1)
    self.assertEqual([i for i,_ in entries],sorted(i for i,_ in entries))
 def test_all_attacks_exist_in_current_source(self):
  for name,approval in APPROVED.items():
   for _,move in approval['moves']:
    with self.subTest(species=name,move=move):
     self.assertRegex(MOVES,rf'(?m)^#define MOVE_{move}\s+\d+$')
if __name__=='__main__':
 unittest.main()
