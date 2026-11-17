"""Source-only controls: immutable provenance and reject unapproved historical drift."""
import hashlib
import json
import subprocess
import unittest
from pathlib import Path
from approved_gym_castform_changes import prior_gym_castform_source
from approved_later_event_flags_projection import prior_later_event_flags_source
from approved_twelve_learnset_projection import prior_twelve_learnsets_source, _record


def git_source(ref, path):
    return subprocess.check_output(['git', 'show', ref + ':' + path], text=True)


class GymHistoryTests(unittest.TestCase):
    def test_frozen_gym_hashes_unchanged_and_satisfied(self):
        path = 'tests/specifications/approved_gym_castform_delta.json'
        self.assertEqual(Path(path).read_text(), git_source('b8ec987f582196c25a0422d6bca64fb648e6a3e9', path))
        spec = json.loads(Path(path).read_text())
        for file, digest in {**spec['before_sha256'], **spec['unchanged_sha256']}.items():
            with self.subTest(file=file):
                self.assertEqual(hashlib.sha256(prior_gym_castform_source(file).encode()).hexdigest(), digest)

    def test_flag_hunks_match_immutable_sources(self):
        spec = json.loads(Path('tests/specifications/approved_later_event_flags_projection.json').read_text())
        path = 'include/constants/flags.h'
        after = git_source(spec['approved_main'], path)
        before = git_source(spec['historical_weather_dependency'], path)
        for hunk in spec['hunks']:
            self.assertEqual(after.count(hunk['after']), 1)
            self.assertEqual(before.count(hunk['before']), 1)
        prior_later_event_flags_source(after)

    def test_flag_drift_and_duplicate_rejected(self):
        source = Path('include/constants/flags.h').read_text()
        spec = json.loads(Path('tests/specifications/approved_later_event_flags_projection.json').read_text())
        for hunk in spec['hunks']:
            for changed in (source.replace(hunk['after'], hunk['after'].replace('0x', '0X', 1), 1),
                            source + hunk['after'], source.replace(hunk['after'], '', 1)):
                with self.assertRaises(AssertionError):
                    prior_later_event_flags_source(changed)

    def test_brock_script_matches_exact_approved_commit(self):
        spec = json.loads(Path('tests/specifications/approved_brock_reward_projection.json').read_text())
        self.assertEqual(spec['after'], git_source(spec['approved_commit'], spec['path']))
        self.assertEqual(spec['before'], git_source(spec['approved_commit'] + '^', spec['path']))
        for changed in (spec['after'].replace('ITEM_TM07', 'ITEM_TM39', 1), spec['after'] + '\n'):
            with self.assertRaises(AssertionError):
                prior_gym_castform_source(spec['path'], changed)

    def test_learnset_drift_or_duplicate_rejected(self):
        source = Path('src/data/pokemon/level_up_learnsets.h').read_text()
        block = _record(source, 'Vulpix').group()
        for changed in (source.replace(block, block.replace('MOVE_', 'MOVE_BAD_', 1), 1),
                        source + '\n' + block, source.replace(block, '', 1)):
            with self.assertRaises(AssertionError):
                prior_twelve_learnsets_source(changed)

    def test_helper_reused_byte_exact_from_prepared_pr233(self):
        path = 'tests/approved_twelve_learnset_projection.py'
        self.assertEqual(Path(path).read_text(), git_source('dde9c7d245b3ed9cf21ec475c65b2651f4e07921', path))


if __name__ == '__main__':
    unittest.main()
