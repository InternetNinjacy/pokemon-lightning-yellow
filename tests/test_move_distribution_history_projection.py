"""Pure-source negative controls for approved later learnsets in old move guards."""
import ast
import contextlib
import io
import json
import runpy
import subprocess
import unittest
from pathlib import Path
from unittest.mock import patch
from approved_locked_learnset_distribution import prior_locked_distribution_source

BASE = '777b2f655365cecd484236f5fbe83b298d9b8f69'
LEARNSETS = 'src/data/pokemon/level_up_learnsets.h'


class MoveDistributionHistoryTests(unittest.TestCase):
    def test_exact_approved_profiles_reverse_without_new_distribution(self):
        source = Path(LEARNSETS).read_text()
        self.assertIn('MOVE_DREAM_HAUNT', source)
        self.assertIn('MOVE_SNARL', source)
        historical = prior_locked_distribution_source(LEARNSETS)
        self.assertNotIn('MOVE_DREAM_HAUNT', historical)
        self.assertNotIn('MOVE_SNARL', historical)
        for path in ('src/data/pokemon/tmhm_learnsets.h', 'src/data/trainer_parties.h'):
            self.assertEqual(prior_locked_distribution_source(path), Path(path).read_text())

    def test_frozen_move_and_weather_manifests_unchanged(self):
        for name in ('moves374_delta.json', 'approved_new_ghost_move_delta.json',
                     'special_glalie_delta.json', 'weather_ability_delta.json'):
            path = 'tests/specifications/' + name
            self.assertEqual(Path(path).read_text(), subprocess.check_output(
                ['git', 'show', BASE + ':' + path], text=True))

    def test_added_distribution_outside_locked_profiles_rejected(self):
        original_read = Path.read_text
        cases = [('tests/run_moves374_tests.py', 'MOVE_SNARL'),
                 ('tests/run_new_ghost_moves_tests.py', 'MOVE_SOUL_DRAIN')]
        for runner, move in cases:
            for target in (LEARNSETS, 'src/data/trainer_parties.h'):
                def altered(path, *args, **kwargs):
                    source = original_read(path, *args, **kwargs)
                    if str(path) == target:
                        self.assertIn('MOVE_TACKLE', source)
                        return source.replace('MOVE_TACKLE', move, 1)
                    return source
                with self.subTest(runner=runner, target=target):
                    with patch.object(Path, 'read_text', altered), self.assertRaises(AssertionError):
                        with contextlib.redirect_stdout(io.StringIO()):
                            runpy.run_path(runner)

    def test_locked_profile_edit_rejected(self):
        original_read = Path.read_text
        def altered(path, *args, **kwargs):
            source = original_read(path, *args, **kwargs)
            return source.replace('MOVE_DREAM_HAUNT', 'MOVE_SHADOW_BALL', 1) if str(path) == LEARNSETS else source
        with patch.object(Path, 'read_text', altered), self.assertRaises(AssertionError):
            prior_locked_distribution_source(LEARNSETS)

    def test_new_fire_source_contract_without_chance_execution(self):
        # Execute only the existing structural prefix. The controlled-RNG C
        # harness stays unchanged and unexecuted under owner playtest policy.
        path = 'tests/run_new_fire_moves_tests.py'
        tree = ast.parse(Path(path).read_text(), filename=path)
        boundary = next(i for i, node in enumerate(tree.body)
                        if isinstance(node, ast.Assign)
                        and any(isinstance(t, ast.Name) and t.id == 'body' for t in node.targets))
        self.assertIn('new_fire_chance_cases.c', ast.unparse(tree.body[boundary]))
        prefix = ast.Module(body=tree.body[:boundary], type_ignores=[])
        self.assertFalse(any(isinstance(n, ast.Call) and isinstance(n.func, ast.Attribute)
                             and isinstance(n.func.value, ast.Name) and n.func.value.id == 'subprocess'
                             for n in ast.walk(prefix)))
        with contextlib.redirect_stdout(io.StringIO()):
            exec(compile(prefix, path, 'exec'), {'__name__': 'fire_source_prefix'})

    def test_remaining_weather_guards_pass_with_frozen_records(self):
        for runner in ('tests/run_special_glalie_tests.py', 'tests/run_weather_ability_tests.py'):
            with self.subTest(runner=runner), contextlib.redirect_stdout(io.StringIO()):
                runpy.run_path(runner)


if __name__ == '__main__':
    unittest.main()
