"""Source-only negative controls for the reused six-record history mapping."""
import unittest
from pathlib import Path
from approved_six_locked_species_source import RECORDS, prior_six_locked_species_source

class LockedSpeciesHistoryTests(unittest.TestCase):
    def test_all_six_exact_records_reverse(self):
        source = Path('src/data/pokemon/species_info.h').read_text()
        # Weather abilities are a separately approved, independently checked delta.
        from approved_weather_species_slots import prior_weather_species_source
        source = prior_weather_species_source(source)
        result = prior_six_locked_species_source(source)
        for pair in RECORDS.values():
            self.assertEqual(source.count(pair['after']), 1)
            self.assertEqual(result.count(pair['before']), 1)

    def test_each_unapproved_record_drift_rejected(self):
        from approved_weather_species_slots import prior_weather_species_source
        source = prior_weather_species_source(Path('src/data/pokemon/species_info.h').read_text())
        for species, pair in RECORDS.items():
            with self.subTest(species=species):
                changed = pair['after'].replace('.baseHP =', '.baseHP /* drift */ =', 1)
                with self.assertRaises(AssertionError):
                    prior_six_locked_species_source(source.replace(pair['after'], changed, 1))

    def test_missing_and_duplicate_records_rejected(self):
        from approved_weather_species_slots import prior_weather_species_source
        source = prior_weather_species_source(Path('src/data/pokemon/species_info.h').read_text())
        record = RECORDS['VULPIX']['after']
        for changed in (source.replace(record, '', 1), source + '\n' + record):
            with self.assertRaises(AssertionError):
                prior_six_locked_species_source(changed)

if __name__ == '__main__':
    unittest.main()
