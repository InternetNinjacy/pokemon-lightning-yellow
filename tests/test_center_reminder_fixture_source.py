"""Static fixture/data alignment; no native menu or game execution."""
import re
import subprocess
import unittest
from pathlib import Path

BASE = '9bb5cc7eecba911f19d7e8839463edebf25b06d4'
PREPARED = 'd9f90011d83e35868126f7e2a8ea3d67a6705d7e'
PATH = 'tests/emulator/center_move_reminder_learning.c'


def historical(ref):
    return subprocess.check_output(['git', 'show', ref + ':' + PATH], text=True)


def learnset(species):
    text = Path('src/data/pokemon/level_up_learnsets.h').read_text()
    block = re.search(r's' + species + r'LevelUpLearnset\[\] = \{(.*?)\n\};', text, re.S)[1]
    return [(int(level), move) for level, move in re.findall(r'LEVEL_UP_MOVE\(\s*(\d+), MOVE_(\w+)\)', block)]


class CenterReminderFixtureSourceTests(unittest.TestCase):
    def test_exact_prepared_fixture(self):
        self.assertEqual(Path(PATH).read_text(), historical(PREPARED))

    def test_level_boundary_and_species_exclusions_match_approved_data(self):
        vaporeon = learnset('Vaporeon')
        self.assertEqual([m for level, m in vaporeon if level <= 7], ['BUBBLE', 'HELPING_HAND'])
        self.assertEqual([m for level, m in vaporeon if level <= 8], ['BUBBLE', 'HELPING_HAND', 'WATER_GUN'])
        self.assertNotIn('TAIL_WHIP', [m for _, m in vaporeon])
        espeon = [m for level, m in learnset('Espeon') if level <= 8]
        self.assertIn('CONFUSION', espeon)
        self.assertNotIn('WATER_GUN', espeon)
        self.assertNotIn('TAIL_WHIP', espeon)
        self.assertEqual([m for level, m in learnset('Eevee') if level == 1], ['TACKLE', 'TAIL_WHIP'])
        # Physical-menu fixture deliberately knows Helping Hand; Water Gun is
        # second after Bubble, for both the three-slot and full-party scenarios.
        self.assertEqual([m for level, m in vaporeon if level <= 8 and m != 'HELPING_HAND'], ['BUBBLE', 'WATER_GUN'])

    def test_assertions_preserved_with_only_documented_species_data_updates(self):
        before = re.findall(r'CHECK\((.*?)\);', historical(BASE), re.S)
        after = re.findall(r'CHECK\((.*?)\);', Path(PATH).read_text(), re.S)
        expected = [check.replace('MOVE_GROWL', 'MOVE_TAIL_WHIP') for check in before]
        expected = [check.replace('centerHasMove(n, MOVE_TACKLE)', 'centerHasMove(n, MOVE_BUBBLE)') for check in expected]
        self.assertEqual(after, expected)

    def test_native_input_and_noncreation_actions_unchanged(self):
        before = historical(BASE)
        after = Path(PATH).read_text()
        for function in ['press', 'frames']:
            pattern = r'\b' + function + r'\((.*?)\);'
            self.assertEqual(re.findall(pattern, after, re.S), re.findall(pattern, before, re.S))
        calls = lambda text: [call for call in re.findall(r'CALL\((.*?)\);', text, re.S)
                              if not call.startswith('CreateMon,')]
        self.assertEqual(calls(after), calls(before))


if __name__ == '__main__':
    unittest.main()
