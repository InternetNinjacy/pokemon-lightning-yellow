"""Source-only controls for pinned trainer-history reconstruction."""
import re
import unittest
from pathlib import Path
from unittest.mock import patch

from approved_ordinary_trainer_projection import restore_ordinary_trainer_blocks
from approved_two_leader_party_projection import prior_two_leader_party_source, _party
from approved_four_trainer_metadata_projection import prior_four_trainer_metadata_source, _record


class TrainerHistoryProjectionTests(unittest.TestCase):
    def test_both_complete_files_reconstruct_frozen_history(self):
        for path in ('src/data/trainer_parties.h', 'src/data/trainers.h'):
            with self.subTest(path=path):
                restore_ordinary_trainer_blocks(path)

    def test_leader_level_drift_rejected(self):
        source = Path('src/data/trainer_parties.h').read_text()
        block = _party(source, 'sParty_LeaderLtSurge').group()
        changed = re.sub(r'(\.lvl\s*=\s*)\d+', r'\g<1>1', block, count=1)
        self.assertNotEqual(block, changed)
        with self.assertRaises(AssertionError):
            prior_two_leader_party_source(source.replace(block, changed, 1))

    def test_missing_or_duplicate_leader_rejected(self):
        source = Path('src/data/trainer_parties.h').read_text()
        block = _party(source, 'sParty_LeaderSabrina').group()
        for changed in (source.replace(block, '', 1), source + '\n' + block):
            with self.assertRaises(AssertionError):
                prior_two_leader_party_source(changed)

    def test_double_flag_or_metadata_drift_rejected(self):
        source = Path('src/data/trainers.h').read_text()
        block = _record(source, 'TRAINER_LEADER_LT_SURGE').group()
        for changed in (block.replace('.doubleBattle = TRUE,', '.doubleBattle = FALSE,'),
                        block.replace('.doubleBattle', '/* unexpected */ .doubleBattle')):
            self.assertNotEqual(block, changed)
            with self.assertRaises(AssertionError):
                prior_four_trainer_metadata_source(source.replace(block, changed, 1))

    def test_unrelated_whole_file_drift_rejected(self):
        original_read = Path.read_text
        for target in ('src/data/trainer_parties.h', 'src/data/trainers.h'):
            def altered(path, *args, **kwargs):
                text = original_read(path, *args, **kwargs)
                return text + '\n/* unapproved outside-record change */\n' if str(path) == target else text
            with self.subTest(path=target), patch.object(Path, 'read_text', altered):
                with self.assertRaises(AssertionError):
                    restore_ordinary_trainer_blocks(target)


if __name__ == '__main__':
    unittest.main()
