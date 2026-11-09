#!/usr/bin/env python3
"""PKM-009 owner-locked Vulpix/Ninetales profile contract (2026-10-31)."""
from pathlib import Path
import re
import unittest

ROOT=Path(__file__).resolve().parents[1]
INFO=(ROOT/"src/data/pokemon/species_info.h").read_text()
LEARN=(ROOT/"src/data/pokemon/level_up_learnsets.h").read_text()
MOVES=(ROOT/"include/constants/moves.h").read_text()

APPROVED={
"Vulpix":{
"stats":(48,31,40,75,65,65),
"moves":[(1,"EMBER"),(5,"TAIL_WHIP"),(9,"LICK"),(13,"QUICK_ATTACK"),
(15,"FIRE_SPIN"),(17,"WILL_O_WISP"),(21,"CONFUSE_RAY"),
(25,"ASTONISH"),(29,"FLAMETHROWER"),(33,"ROAR"),
(37,"DREAM_HAUNT"),(41,"SHADOW_BALL"),(45,"GRUDGE")]},
"Ninetales":{
"stats":(83,51,75,121,100,100),
"moves":[(1,"EMBER"),(5,"TAIL_WHIP"),(9,"LICK"),(13,"QUICK_ATTACK"),
(16,"FIRE_SPIN"),(19,"WILL_O_WISP"),(24,"CONFUSE_RAY"),
(29,"ASTONISH"),(34,"FLAMETHROWER"),(39,"ROAR"),
(44,"DREAM_HAUNT"),(49,"SHADOW_BALL"),(54,"GRUDGE")]}}
FIELDS=("baseHP","baseAttack","baseDefense","baseSpAttack","baseSpDefense","baseSpeed")

class ApprovedVulpixFamily(unittest.TestCase):
    def test_exact_species_fields(self):
        for species,spec in APPROVED.items():
            with self.subTest(species=species):
                m=re.search(r"\[SPECIES_"+species.upper()+r"\]\s*=\s*\{(.*?)\n    \},",INFO,re.S)
                self.assertIsNotNone(m)
                b=m.group(1)
                vals=tuple(int(re.search(r"\."+field+r"\s*=\s*(\d+)",b).group(1)) for field in FIELDS)
                self.assertEqual(vals,spec["stats"])
                self.assertIn(".types = {TYPE_FIRE, TYPE_GHOST}",b)
                self.assertIn(".abilities = {ABILITY_FLASH_FIRE, ABILITY_SHADOW_TAG}",b)
    def test_exact_full_natural_learnsets(self):
        for species,spec in APPROVED.items():
            with self.subTest(species=species):
                m=re.search(r"static const u16 s"+species+r"LevelUpLearnset\[\] = \{(.*?)\};",LEARN,re.S)
                self.assertIsNotNone(m)
                b=m.group(1)
                vals=[(int(l),mv) for l,mv in re.findall(r"LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)",b)]
                self.assertEqual(vals,spec["moves"])
                self.assertEqual([l for l,_ in vals],sorted(l for l,_ in vals))
                self.assertEqual(b.count("LEVEL_UP_END"),1)
    def test_custom_move_definitions_exist(self):
        for move in ("DREAM_HAUNT","ROAR","GRUDGE","SHADOW_BALL"):
            with self.subTest(move=move):
                self.assertRegex(MOVES,rf"(?m)^#define MOVE_{move}\s+\d+$")

if __name__=="__main__":
    unittest.main()
