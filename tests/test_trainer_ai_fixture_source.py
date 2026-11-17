"""Source premise/provenance checks; never executes the native AI harness."""
import re
import subprocess
import unittest
from pathlib import Path

BASE = 'c96184c6058966cbd9789e53eb0eb8462ce4f754'
PREPARED = 'b8390e1aa47ae84951c7263696dc0b1ac9093fe6'
PATH = 'tests/emulator/battle_categories.c'


def historical(ref):
    return subprocess.check_output(['git', 'show', ref + ':' + PATH], text=True)


class TrainerAiFixtureSourceTests(unittest.TestCase):
    def test_exact_previously_validated_fixture(self):
        self.assertEqual(Path(PATH).read_text(), historical(PREPARED))

    def test_immunity_species_are_genuine_single_ability_species(self):
        data = Path('src/data/pokemon/species_info.h').read_text()
        for name, expected in [('FLAREON', ('FLASH_FIRE', 'NONE')),
                               ('GENGAR', ('LEVITATE', 'NONE')),
                               ('VULPIX', ('FLASH_FIRE', 'SHADOW_TAG'))]:
            found = re.findall(r'\[SPECIES_' + name + r'\]\s*=\s*\{.*?\.abilities = \{ABILITY_(\w+), ABILITY_(\w+)\}', data, re.S)
            self.assertEqual(found, [expected], name)

    def test_existing_assertions_and_actions_remain_byte_exact(self):
        old = historical(BASE)
        actual = Path(PATH).read_text()
        old_checks = re.findall(r'CHECK\((.*?)\);', old, re.S)
        new_checks = re.findall(r'CHECK\((.*?)\);', actual, re.S)
        additional = 'monget(player(0),ABI_DATA_ABILITY_NUM)==0'
        self.assertEqual(new_checks.count(additional), 1)
        new_checks.remove(additional)
        self.assertEqual(new_checks, old_checks)
        # Every executed AI command/action selection is preserved in order.
        self.assertEqual(re.findall(r'CALL\((.*?)\);', actual, re.S),
                         re.findall(r'CALL\((.*?)\);', old, re.S))
        self.assertIn('monput(player(0),ABI_DATA_ABILITY_NUM,0);', actual)
        self.assertIn('const unsigned abilitySpecies[]={SPECIES_FLAREON,SPECIES_GENGAR};', actual)


if __name__ == '__main__':
    unittest.main()
