"""Source-only safeguards for the prepared Oak identity/naming integration."""
import hashlib
import json
import subprocess
import unittest
from pathlib import Path
from approved_wrl001_identity_changes import prior_wrl001_identity_source
from approved_follower_changes import prior_follower_source

BASE = '21d597b178327e706b015c61643f4489e0d8c4cb'
PREPARED = '0f693dd7bef4219e161b299f08a9a225ecdac9d6'


def source(ref, path):
    return subprocess.check_output(['git', 'show', ref + ':' + path], text=True)


class Wrl001IdentityProjectionTests(unittest.TestCase):
    def test_gameplay_is_exact_prepared_implementation_and_reverses(self):
        spec = json.loads(Path('tests/specifications/wrl001_identity_delta.json').read_text())
        for path, row in spec['files'].items():
            actual = prior_follower_source(path, Path(path).read_text())
            with self.subTest(path=path):
                self.assertEqual(actual, source(PREPARED, path))
                self.assertEqual(hashlib.sha256(actual.encode()).hexdigest(), row['after_sha256'])
                old = prior_wrl001_identity_source(path, actual)
                self.assertEqual(old, source(BASE, path))
                self.assertEqual(prior_wrl001_identity_source(path, old), old)

    def test_mutated_and_partially_projected_changes_rejected(self):
        spec = json.loads(Path('tests/specifications/wrl001_identity_delta.json').read_text())
        partial_checked = False
        for path, row in spec['files'].items():
            actual = prior_follower_source(path, Path(path).read_text())
            for hunk in row['hunks']:
                with self.subTest(path=path), self.assertRaises(AssertionError):
                    prior_wrl001_identity_source(path, actual.replace(hunk['after'], '', 1))
            if len(row['hunks']) > 1:
                partial_checked = True
                hunk = row['hunks'][0]
                with self.assertRaises(AssertionError):
                    prior_wrl001_identity_source(path, actual.replace(hunk['after'], hunk['before'], 1))
        self.assertTrue(partial_checked)

    def test_native_fixture_is_unchanged_from_historical_evidence(self):
        path = 'tests/emulator/wrl001_identity.c'
        self.assertEqual(Path(path).read_text(), source(PREPARED, path))

    def test_existing_native_registration_and_guards_preserved(self):
        path = 'tests/emulator/run.py'
        new = prior_follower_source(path, Path(path).read_text())
        line = "required += ['ScriptGiveSueEevee', 'ScriptGiveMon', 'EventScript_ChangePokemonNickname']\n"
        self.assertEqual(new.count(line), 1)
        self.assertEqual(new.replace(line, '', 1), source(BASE, path))
        path = 'tests/emulator/runner.c'
        new = prior_follower_source(path, Path(path).read_text())
        additions = ['#include "wrl001_identity.c"\n\n', '    wrl001Tests();\n']
        additions += [line + '\n' for line in new.splitlines()
                      if 'if(getenv("LIGHTNING_WRL001_' in line]
        self.assertEqual(len(additions), 4)
        for addition in additions:
            self.assertEqual(new.count(addition), 1)
            new = new.replace(addition, '', 1)
        self.assertEqual(new, source(BASE, path))

    def test_old_gift_manifests_remain_frozen(self):
        for path in ['tests/specifications/approved_gift_delta.json',
                     'tests/specifications/wrl001_opening_delta.json']:
            self.assertEqual(Path(path).read_text(), source(BASE, path))


if __name__ == '__main__':
    unittest.main()
