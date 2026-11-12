#!/usr/bin/env python3
"""Delegated November 3 #016–022 family source profile exact contract."""
from pathlib import Path
import re,unittest
BASE=Path(__file__).resolve().parents[1]
LEARN=(BASE/'src/data/pokemon/level_up_learnsets.h').read_text()
MOVES=(BASE/'include/constants/moves.h').read_text()
TARGET={"Pidgey":[[1,"TACKLE"],[5,"SAND_ATTACK"],[9,"GUST"],[13,"QUICK_ATTACK"],[13,"WING_ATTACK"],[19,"WHIRLWIND"],[31,"FEATHER_DANCE"],[39,"AGILITY"],[47,"MIRROR_MOVE"]],"Pidgeotto":[[1,"TACKLE"],[1,"SAND_ATTACK"],[1,"GUST"],[5,"SAND_ATTACK"],[9,"GUST"],[13,"QUICK_ATTACK"],[20,"WHIRLWIND"],[20,"WING_ATTACK"],[32,"AERIAL_ACE"],[34,"FEATHER_DANCE"],[43,"AGILITY"],[52,"MIRROR_MOVE"]],"Pidgeot":[[1,"TACKLE"],[1,"SAND_ATTACK"],[1,"GUST"],[1,"QUICK_ATTACK"],[5,"SAND_ATTACK"],[9,"GUST"],[13,"QUICK_ATTACK"],[20,"WHIRLWIND"],[27,"WING_ATTACK"],[34,"FEATHER_DANCE"],[38,"AERIAL_ACE"],[48,"AGILITY"],[49,"AIR_CUTTER"],[62,"MIRROR_MOVE"]],"Rattata":[[1,"TACKLE"],[1,"TAIL_WHIP"],[7,"QUICK_ATTACK"],[12,"BITE"],[13,"HYPER_FANG"],[20,"FOCUS_ENERGY"],[27,"PURSUIT"],[34,"SUPER_FANG"],[41,"ENDEAVOR"]],"Raticate":[[1,"TACKLE"],[1,"TAIL_WHIP"],[1,"QUICK_ATTACK"],[7,"QUICK_ATTACK"],[13,"HYPER_FANG"],[20,"SCARY_FACE"],[25,"CRUNCH"],[30,"PURSUIT"],[40,"SUPER_FANG"],[50,"ENDEAVOR"]],"Spearow":[[1,"PECK"],[1,"GROWL"],[7,"LEER"],[13,"FURY_ATTACK"],[19,"PURSUIT"],[25,"AERIAL_ACE"],[31,"MIRROR_MOVE"],[37,"DRILL_PECK"],[43,"AGILITY"]],"Fearow":[[1,"PECK"],[1,"GROWL"],[1,"LEER"],[1,"FURY_ATTACK"],[7,"LEER"],[13,"FURY_ATTACK"],[26,"PURSUIT"],[27,"AERIAL_ACE"],[32,"MIRROR_MOVE"],[39,"DRILL_PECK"],[47,"AGILITY"]]}
ORIGINAL={"Pidgey":[[1,"TACKLE"],[5,"SAND_ATTACK"],[9,"GUST"],[13,"QUICK_ATTACK"],[19,"WHIRLWIND"],[25,"WING_ATTACK"],[31,"FEATHER_DANCE"],[39,"AGILITY"],[47,"MIRROR_MOVE"]],"Pidgeotto":[[1,"TACKLE"],[1,"SAND_ATTACK"],[1,"GUST"],[5,"SAND_ATTACK"],[9,"GUST"],[13,"QUICK_ATTACK"],[20,"WHIRLWIND"],[27,"WING_ATTACK"],[34,"FEATHER_DANCE"],[43,"AGILITY"],[52,"MIRROR_MOVE"]],"Pidgeot":[[1,"TACKLE"],[1,"SAND_ATTACK"],[1,"GUST"],[1,"QUICK_ATTACK"],[5,"SAND_ATTACK"],[9,"GUST"],[13,"QUICK_ATTACK"],[20,"WHIRLWIND"],[27,"WING_ATTACK"],[34,"FEATHER_DANCE"],[48,"AGILITY"],[62,"MIRROR_MOVE"]],"Rattata":[[1,"TACKLE"],[1,"TAIL_WHIP"],[7,"QUICK_ATTACK"],[13,"HYPER_FANG"],[20,"FOCUS_ENERGY"],[27,"PURSUIT"],[34,"SUPER_FANG"],[41,"ENDEAVOR"]],"Raticate":[[1,"TACKLE"],[1,"TAIL_WHIP"],[1,"QUICK_ATTACK"],[7,"QUICK_ATTACK"],[13,"HYPER_FANG"],[20,"SCARY_FACE"],[30,"PURSUIT"],[40,"SUPER_FANG"],[50,"ENDEAVOR"]],"Spearow":[[1,"PECK"],[1,"GROWL"],[7,"LEER"],[13,"FURY_ATTACK"],[19,"PURSUIT"],[25,"AERIAL_ACE"],[31,"MIRROR_MOVE"],[37,"DRILL_PECK"],[43,"AGILITY"]],"Fearow":[[1,"PECK"],[1,"GROWL"],[1,"LEER"],[1,"FURY_ATTACK"],[7,"LEER"],[13,"FURY_ATTACK"],[26,"PURSUIT"],[32,"MIRROR_MOVE"],[40,"DRILL_PECK"],[47,"AGILITY"]]}
class DelegatedBirdRodentSource(unittest.TestCase):
 def test_full_learnsets_including_source_retained_spearow(self):
  for name,rows in TARGET.items():
   with self.subTest(name=name):
    pattern=r'static const u16 s'+name+r'LevelUpLearnset\[\] = \{(.*?)\};'
    match=re.search(pattern,LEARN,re.S)
    self.assertIsNotNone(match)
    body=match.group(1)
    got=[[int(l),mv] for l,mv in re.findall(r'LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)',body)]
    self.assertEqual(got,rows)
    self.assertEqual(body.count('LEVEL_UP_END'),1)
    self.assertEqual([x[0] for x in got],sorted(x[0] for x in got))
    for _,move in rows:
     self.assertRegex(MOVES,rf'(?m)^#define MOVE_{move}\s+\d+$')
if __name__=='__main__':unittest.main()
