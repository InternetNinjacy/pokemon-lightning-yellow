"""Exact current-source authority/provenance checks; no Pokémon or game execution."""
import hashlib
import json
import re
import subprocess
import unittest
from pathlib import Path
from approved_146_learnset_changes import PATH, contract, prior_146_learnset_source


def historical(ref, path):
    return subprocess.check_output(['git', 'show', ref + ':' + path], text=True)


def moves(block):
    return [[int(level), move] for level, move in re.findall(r'LEVEL_UP_MOVE\(\s*(\d+), MOVE_(\w+)\)', block)]


class Approved146SourceTests(unittest.TestCase):
    def test_every_approved_row_matches_both_pinned_source_manifests(self):
        spec = contract()
        native = Path(PATH).read_text()
        constants = Path('include/constants/moves.h').read_text()
        ids = {move: int(value) for move, value in re.findall(r'^#define MOVE_(\w+)\s+(\d+)\s*$', constants, re.M)}
        capacity_source = Path('include/constants/pokemon.h').read_text()
        capacity = int(re.search(r'^#define MAX_LEVEL_UP_MOVES\s+(\d+)', capacity_source, re.M)[1])
        old_rows = []
        for source in spec['source_manifests']:
            text = historical(spec['source_commit'], source['path'])
            self.assertEqual(hashlib.sha256(text.encode()).hexdigest(), source['sha256'])
            self.assertEqual(subprocess.check_output(['git', 'rev-parse', spec['source_commit'] + ':' + source['path']], text=True).strip(), source['git_blob_sha1'])
            source_rows = json.loads(text)['rows']
            self.assertEqual(len(source_rows), source['count'])
            old_rows.extend(source_rows)
        self.assertEqual([s['count'] for s in spec['source_manifests']], [74, 72])
        self.assertEqual(len(old_rows), 146)
        for original, row in zip(old_rows, spec['rows']):
            with self.subTest(species=row['species']):
                self.assertEqual([row['dex'], row['species'], row['pointer']], [original['dex'], original['species'], original['pointer']])
                self.assertEqual(moves(row['before_body']), original['before'])
                self.assertEqual(moves(row['after_body']), original['after'])
                self.assertEqual(native.count(row['after_body']), 1)
                self.assertNotIn(row['before_body'], native)
                self.assertLessEqual(len(original['after']), capacity, 'native level-up/egg/Reminder buffer bound')
                for level, move in original['after']:
                    self.assertTrue(1 <= level <= 100)
                    self.assertTrue(1 <= ids[move] <= 410)

    def test_whole_file_reverses_to_exact_current_main_with_no_unrelated_changes(self):
        spec = contract()
        current = Path(PATH).read_text()
        self.assertEqual(hashlib.sha256(current.encode()).hexdigest(), spec['after_sha256'])
        old = historical(spec['base_main'], PATH)
        self.assertEqual(hashlib.sha256(old.encode()).hexdigest(), spec['before_sha256'])
        self.assertEqual(prior_146_learnset_source(PATH, current), old)
        self.assertEqual(prior_146_learnset_source(PATH, old), old)

    def test_fourteen_locked_profiles_and_ten_excluded_proposals_preserved(self):
        spec = contract()
        locked = {'VULPIX','NINETALES','ODDISH','GLOOM','VILEPLUME','BELLOSSOM','PONYTA','RAPIDASH','EEVEE','VAPOREON','JOLTEON','FLAREON','ESPEON','UMBREON'}
        separate = {'BULBASAUR','IVYSAUR','VENUSAUR','ARBOK','NIDOKING','ARIADOS','NUZLEAF','SHIFTRY','BRELOOM','CACTURNE'}
        self.assertFalse((locked | separate) & {r['species'] for r in spec['rows']})
        # Exact whole-file reversal proves every other array, including Dugtrio's
        # FireRed/LeafGreen conditional definitions, stayed byte-for-byte intact.
        for path in ['src/data/pokemon/species_info.h', 'src/data/pokemon/tmhm_learnsets.h', 'src/data/pokemon/evolution.h']:
            self.assertEqual(Path(path).read_text(), historical(spec['base_main'], path))

    def test_mutated_missing_duplicate_and_partial_protected_arrays_rejected(self):
        spec = contract()
        current = Path(PATH).read_text()
        for index in (0, 73, 74, 145):
            row = spec['rows'][index]
            mutations = [current.replace(row['after_body'], '', 1),
                         current + '\n' + row['after_body'],
                         current.replace(row['after_body'], row['before_body'], 1),
                         current.replace(row['after_body'], row['after_body'].replace('MOVE_', 'MOVE_UNAPPROVED_', 1), 1)]
            for changed in mutations:
                with self.subTest(species=row['species']), self.assertRaises(AssertionError):
                    prior_146_learnset_source(PATH, changed)

    def test_current_source_whole_file_contract_rejects_unrelated_drift(self):
        spec = contract()
        current = Path(PATH).read_text()
        changed = current + '\n/* outside approved146 change */\n'
        # The composable inverse leaves outside bytes visible; current-source
        # and downstream frozen whole-file checks must still reject them.
        self.assertNotEqual(hashlib.sha256(changed.encode()).hexdigest(), spec['after_sha256'])
        self.assertNotEqual(prior_146_learnset_source(PATH, changed), historical(spec['base_main'], PATH))


if __name__ == '__main__':
    unittest.main()
