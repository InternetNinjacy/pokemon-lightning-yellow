"""Source-only regression for later locked records in the weather slot audit."""
import contextlib
import io
import runpy
import subprocess
import unittest
from pathlib import Path
from unittest.mock import patch


class WeatherSpeciesHistoryTests(unittest.TestCase):
    def test_original_frozen_digest_unchanged_and_audit_passes(self):
        path = 'tests/specifications/special_glalie_delta.json'
        self.assertEqual(Path(path).read_text(), subprocess.check_output(
            ['git', 'show', 'b8ec987f582196c25a0422d6bca64fb648e6a3e9:' + path], text=True))
        with contextlib.redirect_stdout(io.StringIO()):
            runpy.run_path('tests/run_weather_species_tests.py')

    def test_unapproved_ability_or_stat_drift_rejected(self):
        original_read = Path.read_text
        for before, after in (('ABILITY_SWIFT_SWIM, ABILITY_DRIZZLE', 'ABILITY_SWIFT_SWIM, ABILITY_DAMP'),
                              ('.baseHP = 45,', '.baseHP = 44,')):
            def altered(path, *args, **kwargs):
                source = original_read(path, *args, **kwargs)
                if str(path) == 'src/data/pokemon/species_info.h':
                    self.assertIn(before, source)
                    return source.replace(before, after, 1)
                return source
            with patch.object(Path, 'read_text', altered), self.assertRaises(AssertionError):
                runpy.run_path('tests/run_weather_species_tests.py')

if __name__ == '__main__':
    unittest.main()
