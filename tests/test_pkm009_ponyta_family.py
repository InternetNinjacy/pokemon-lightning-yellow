#!/usr/bin/env python3
"""PKM-009 Ponyta/Rapidash owner-locked species contract, 2026-10-31."""
import re
import unittest
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
STATS = (ROOT / "src/data/pokemon/species_info.h").read_text()
LEARN = (ROOT / "src/data/pokemon/level_up_learnsets.h").read_text()
MOVES = (ROOT / "include/constants/moves.h").read_text()
EXPECT = {
    "Ponyta": {
        "stats": (50, 100, 55, 50, 65, 90),
        "learn": [(1, "QUICK_ATTACK"), (5, "GROWL"), (9, "TAIL_WHIP"),
                  (14, "EMBER"), (16, "FLASHFIRE"), (19, "STOMP"),
                  (25, "FIRE_SPIN"), (28, "FLAME_WHEEL"),
                  (31, "TAKE_DOWN"), (34, "FLAME_FANG"),
                  (38, "AGILITY"), (45, "BLAZE_RUSH")],
    },
    "Rapidash": {
        "stats": (65, 120, 70, 60, 80, 105),
        "learn": [(1, "QUICK_ATTACK"), (1, "GROWL"), (1, "TAIL_WHIP"),
                  (1, "EMBER"), (5, "GROWL"), (9, "TAIL_WHIP"),
                  (14, "EMBER"), (16, "FLASHFIRE"), (19, "STOMP"),
                  (25, "FIRE_SPIN"), (28, "FLAME_WHEEL"),
                  (31, "TAKE_DOWN"), (36, "FLAME_FANG"),
                  (38, "AGILITY"), (48, "BLAZE_RUSH")],
    },
}
FIELDS = ("baseHP", "baseAttack", "baseDefense", "baseSpAttack", "baseSpDefense", "baseSpeed")
class PonytaFamilyContract(unittest.TestCase):
    def test_exact_stats_types_abilities(self):
        for species, data in EXPECT.items():
            with self.subTest(species=species):
                m = re.search(r"\[SPECIES_" + species.upper() + r"\]\s*=\s*\{(.*?)\n    \},", STATS, re.S)
                self.assertIsNotNone(m)
                body=m.group(1)
                self.assertEqual(tuple(int(re.search(r"\." + f + r"\s*=\s*(\d+)", body).group(1)) for f in FIELDS), data["stats"])
                self.assertIn(".types = {TYPE_FIRE, TYPE_FIRE}",body)
                self.assertIn(".abilities = {ABILITY_RUN_AWAY, ABILITY_FLASH_FIRE}", body)
    def test_exact_natural_learnsets(self):
        for species, data in EXPECT.items():
            with self.subTest(species=species):
                m=re.search(r"static const u16 s" + species + r"LevelUpLearnset\[\] = \{(.*?)\};",LEARN,re.S)
                self.assertIsNotNone(m)
                body=m.group(1)
                got=[(int(l),move) for l,move in re.findall(r"LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)",body)]
                self.assertEqual(got,data["learn"])
                self.assertEqual([l for l,_ in got],sorted(l for l,_ in got))
                self.assertEqual(body.count("LEVEL_UP_END"),1)
    def test_custom_moves_exist_in_current_source(self):
        for move in ("FLASHFIRE","FLAME_FANG","BLAZE_RUSH"):
            with self.subTest(move=move):
                self.assertRegex(MOVES,rf"(?m)^#define MOVE_{move}\s+\d+$")
if __name__ == "__main__":
    unittest.main()
