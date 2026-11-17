#!/usr/bin/env python3
"""PKM-009 locked Oddish-family source-contract regression.

Authority: owner-approved October 31, 2026 individual profiles #047-050.
Run: python3 tests/test_pkm009_oddish_family.py
"""
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[1]
LEARNSETS = (ROOT / "src/data/pokemon/level_up_learnsets.h").read_text()
SPECIES = (ROOT / "src/data/pokemon/species_info.h").read_text()

EXPECTED = {
    "Oddish": [(1, "ABSORB"), (7, "SWEET_SCENT"), (12, "MEGA_DRAIN"),
               (14, "POISON_POWDER"), (16, "STUN_SPORE"), (18, "SLEEP_POWDER"),
               (23, "ACID"), (32, "MOONLIGHT"), (39, "PETAL_DANCE")],
    "Gloom": [(1, "ABSORB"), (1, "SWEET_SCENT"), (1, "POISON_POWDER"),
              (7, "SWEET_SCENT"), (14, "POISON_POWDER"), (15, "MEGA_DRAIN"),
              (16, "STUN_SPORE"), (18, "SLEEP_POWDER"), (24, "ACID"),
              (35, "MOONLIGHT"), (44, "PETAL_DANCE")],
    "Vileplume": [(1, "ABSORB"), (1, "AROMATHERAPY"), (1, "STUN_SPORE"),
                  (1, "MEGA_DRAIN"), (24, "ACID"), (30, "SLUDGE"),
                  (38, "MOONLIGHT"), (44, "PETAL_DANCE"), (50, "SLUDGE_BOMB")],
    "Bellossom": [(1, "ABSORB"), (1, "SWEET_SCENT"), (1, "STUN_SPORE"),
                   (1, "MAGICAL_LEAF"), (24, "MEGA_DRAIN"),
                   (30, "SLEEP_POWDER"), (36, "SYNTHESIS"),
                   (44, "PETAL_DANCE"), (55, "SOLAR_BEAM")],
}
STATS = {
    "Oddish": (45, 50, 55, 75, 65, 30, "GRASS", "POISON"),
    "Gloom": (60, 65, 70, 85, 75, 40, "GRASS", "POISON"),
    "Vileplume": (75, 80, 85, 100, 90, 50, "GRASS", "POISON"),
    "Bellossom": (75, 80, 85, 90, 100, 50, "GRASS", "GRASS"),
}

class OddishFamilyContract(unittest.TestCase):
    def test_exact_locked_natural_learnsets(self):
        for name, expected in EXPECTED.items():
            with self.subTest(species=name):
                match = re.search(
                    rf"static const u16 s{name}LevelUpLearnset\[\] = \{{(.*?)\}};",
                    LEARNSETS, re.S)
                self.assertIsNotNone(match)
                body = match.group(1)
                actual = [(int(lvl), move) for lvl, move in re.findall(
                    r"LEVEL_UP_MOVE\((\d+), MOVE_([A-Z0-9_]+)\)", body)]
                self.assertEqual(actual, expected)
                self.assertEqual(body.count("LEVEL_UP_END"), 1)
                self.assertEqual([level for level, _ in actual],
                                 sorted(level for level, _ in actual))

    def test_retained_stats_types_abilities(self):
        keys = ("baseHP", "baseAttack", "baseDefense", "baseSpAttack",
                "baseSpDefense", "baseSpeed")
        for name, expected in STATS.items():
            with self.subTest(species=name):
                body = re.search(
                    rf"\[SPECIES_{name.upper()}\]\s*=\s*\{{(.*?)\n    \}},",
                    SPECIES, re.S)
                self.assertIsNotNone(body)
                src = body.group(1)
                actual = tuple(int(re.search(rf"\.{key}\s*=\s*(\d+)", src).group(1))
                               for key in keys)
                self.assertEqual(actual, expected[:6])
                self.assertEqual(
                    re.search(r"\.types\s*=\s*\{TYPE_(\w+), TYPE_(\w+)\}", src).groups(),
                    expected[6:])
                self.assertRegex(src, r"\.abilities\s*=\s*\{ABILITY_CHLOROPHYLL, ABILITY_NONE\}")

if __name__ == "__main__":
    unittest.main()
