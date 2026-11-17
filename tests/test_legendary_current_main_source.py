"""Pinned source/integration contracts; never emulator or natural-access acceptance."""
import hashlib
import json
import re
import subprocess
import unittest
from pathlib import Path
from approved_stationary_changes import prior_stationary_source

BASE = '28dd82b5d9a684455a36f6458ac88a3e7e45d4ed'
SOURCE = 'cefd2a46d9b97b39a30b2f1b1903ec845979a258'
WILD_FIXTURE = '71f25df94c38ebf4d5c46ad8f7978e76b42944a0'
MAPS = ['Route10', 'MtEmber_SummitPath_1F', 'SeafoamIslands_B4F', 'SevenIsland_TanobyRuins']
SPECIES = ['RAIKOU', 'ENTEI', 'SUICUNE', 'MEW']


def git_source(commit, path):
    return subprocess.check_output(['git', 'show', commit + ':' + path], text=True)


class LegendaryCurrentMainSource(unittest.TestCase):
    def test_nine_production_files_are_exact_prepared235(self):
        paths = ['src/roamer.c']
        paths += [f'data/maps/{name}/{leaf}' for name in MAPS for leaf in ('map.json', 'scripts.inc')]
        for path in paths:
            with self.subTest(path=path):
                self.assertEqual(Path(path).read_text(), git_source(SOURCE, path))

    def test_only_eight_unused_flag_names_change(self):
        source = Path('include/constants/flags.h').read_text()
        restored = source
        for index, species in enumerate(SPECIES):
            for kind, value in [('FOUGHT', 0x325 + index), ('HIDE', 0x329 + index)]:
                name = f'FLAG_{kind}_LIGHTNING_{species}'
                rows = re.findall(r'^#define ' + name + r'\s+(0x[0-9A-Fa-f]+)$', source, re.M)
                self.assertEqual([int(x, 16) for x in rows], [value])
                restored = restored.replace(name, f'FLAG_0x{value:X}')
                numeric = re.findall(r'^#define \w+\s+0x' + format(value, 'X') + r'\s*$', source, re.M)
                self.assertEqual(len(numeric), 1)
        # Preserve alignment exactly: the reviewed rename inverse reconstructs
        # the complete original header, including later Heracross/Route6 claims.
        self.assertEqual(prior_stationary_source('include/constants/flags.h', source), git_source(BASE, 'include/constants/flags.h'))
        for flag in ('FLAG_LIGHTNING_HERACROSS_RESOLVED', 'FLAG_HIDE_LIGHTNING_HERACROSS', 'FLAG_ROUTE6_TM34_CLAIMED'):
            original = [x for x in git_source(BASE, 'include/constants/flags.h').splitlines() if x.startswith('#define ' + flag + ' ')]
            for line in original:
                self.assertIn(line, source)

    def test_all_twelve_protected_deltas_reverse_exactly(self):
        spec = json.loads(Path('tests/specifications/legendary_current_main_delta.json').read_text())
        self.assertEqual(spec['base_main'], BASE)
        self.assertEqual(len(spec['files']), 12)
        for path, row in spec['files'].items():
            with self.subTest(path=path):
                source = Path(path).read_text()
                self.assertEqual(hashlib.sha256(source.encode()).hexdigest(), row['after_sha256'])
                self.assertEqual(prior_stationary_source(path, source), git_source(BASE, path))
                self.assertEqual(prior_stationary_source(path, git_source(BASE, path)), git_source(BASE, path))

    def test_projection_never_discards_unapproved_drift(self):
        for path in ('src/roamer.c', 'data/maps/Route10/scripts.inc', 'include/constants/flags.h'):
            source = Path(path).read_text()
            for changed in (source + '\n/* unexpected drift */\n', source.replace('LIGHTNING', 'UNAPPROVED', 1)):
                if changed == source:
                    continue
                try:
                    restored = prior_stationary_source(path, changed)
                except AssertionError:
                    continue
                self.assertNotEqual(restored, git_source(BASE, path))

    def test_existing_objects_warps_and_layouts_are_preserved(self):
        for name, species in zip(MAPS, SPECIES):
            path = f'data/maps/{name}/map.json'
            current = json.loads(Path(path).read_text())
            original = json.loads(git_source(BASE, path))
            additions = [row for row in current['object_events'] if row['flag'] == f'FLAG_HIDE_LIGHTNING_{species}']
            self.assertEqual(len(additions), 1)
            current['object_events'].remove(additions[0])
            self.assertEqual(current, original)
            self.assertLess(len(original['object_events']) + 3, 16)  # new NPC + player + follower
            self.assertEqual(additions[0]['movement_type'], 'MOVEMENT_TYPE_FACE_DOWN')
        self.assertEqual(Path('data/layouts/layouts.json').read_text(), git_source(BASE, 'data/layouts/layouts.json'))

    def test_native_fixtures_and_all_default_groups_are_retained(self):
        self.assertEqual(Path('tests/emulator/legendary_statics.c').read_text(), git_source(SOURCE, 'tests/emulator/legendary_statics.c'))
        fixture = json.loads(Path('tests/specifications/legendary_current_main_delta.json').read_text())['wild_double_fixture']
        self.assertEqual(fixture['source_commit'], WILD_FIXTURE)
        self.assertEqual(hashlib.sha256(Path(fixture['path']).read_bytes()).hexdigest(), fixture['after_sha256'])
        runner = Path('tests/emulator/runner.c').read_text()
        self.assertEqual(runner.count('    legendaryStationaryNativeTests();'), 1)
        self.assertIn('#include "legendary_statics.c"', runner)
        self.assertEqual(prior_stationary_source('tests/emulator/runner.c', runner), git_source(BASE, 'tests/emulator/runner.c'))
        old_workflow = git_source(BASE, '.github/workflows/build.yml')
        workflow = Path('.github/workflows/build.yml').read_text()
        for command in ('check_legendary_statics.py', 'test_legendary_current_main_source.py'):
            line = '          python3 tests/' + command + '\n'
            self.assertEqual(workflow.count(line), 1)
            workflow = workflow.replace(line, '', 1)
        self.assertEqual(workflow, old_workflow)

    def test_no_unrelated_gameplay_or_experimental243_npcs_imported(self):
        paths = ['src/follower_state.c', 'src/follower_controller.c', 'src/follower_graphics.c',
                 'src/follower_starter.c', 'src/approved_gifts.c', 'include/global.h',
                 'src/data/pokemon/species_info.h', 'src/data/pokemon/level_up_learnsets.h',
                 'src/data/pokemon/tmhm_learnsets.h', 'src/data/trainer_parties.h',
                 'src/dynamic_weather.c', 'src/field_weather_effects.c', 'src/data/wild_encounters.json']
        paths += ['data/maps/' + name for name in [
            'FuchsiaCity/scripts.inc', 'FuchsiaCity/text.inc', 'FuchsiaCity_House2/scripts.inc',
            'Route11_EastEntrance_1F/scripts.inc', 'Route11_EastEntrance_1F/text.inc',
            'Route12_FishingHouse/scripts.inc', 'VermilionCity_House1/scripts.inc',
            'ViridianCity_House/scripts.inc', 'ViridianCity_House/text.inc']]
        for path in paths:
            with self.subTest(path=path):
                self.assertEqual(Path(path).read_text(), git_source(BASE, path))


if __name__ == '__main__':
    unittest.main()
