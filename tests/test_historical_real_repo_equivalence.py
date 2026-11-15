#!/usr/bin/env python3
"""Opt-in full-checkout proof for the migrated Baily and Tucker adapters.

Run from repository root with both exact pinned commits present:
    PYTHONPATH=tests python3 -m unittest -v tests/test_historical_real_repo_equivalence.py
Not registered in CI. No gameplay, fixture or frozen digest mutation.
"""
import hashlib
import json
import re
import subprocess
import unittest
from pathlib import Path

from approved_gym001_baily_changes import (
    AFTER as BAILY_AFTER, BEFORE as BAILY_BEFORE,
    PARTY_PATH, prior_gym001_baily_source,
)
from approved_gym001_tucker_changes import (
    REVIEWED as TUCKER, prior_gym001_tucker_source,
)
from approved_historical_reconstruction import (
    _one_record, require_frozen_digest,
)

APPROVED_MAIN = "02240690581eb70d70219024fb7f55da2882308b"
HISTORICAL_BASE = "21a3630b3d51481ab10a1af9637fa1ced73722cc"
TRAINER_METADATA = "src/data/trainers.h"
FROZEN_SPECS = (
    "tests/specifications/brock_weather_party_delta.json",
    "tests/specifications/approved_elite_four_delta.json",
    "tests/specifications/approved_thunder_wave_delta.json",
)


def git_source(commit, path):
    return subprocess.check_output(
        ["git", "show", commit + ":" + path], text=True
    )


def old_baily(path, source):
    if path != PARTY_PATH:
        return source
    assert source.count(BAILY_AFTER) == 1
    assert BAILY_BEFORE not in source
    return source.replace(BAILY_AFTER, BAILY_BEFORE, 1)


def old_tucker(path, source):
    for row in TUCKER:
        if row["path"] != path:
            continue
        if source.count(row["after"]) == 1:
            source = source.replace(row["after"], row["before"], 1)
        else:
            assert source.count(row["before"]) == 1
    return source


class FullRepositoryHistoricalEquivalence(unittest.TestCase):
    """Must fail if either trusted commit is absent: never use fake Git history."""

    def test_real_pinned_records_and_exact_adapter_output(self):
        cases = (
            (PARTY_PATH, "sParty_EngineerBaily", "trainer_party",
             old_baily, prior_gym001_baily_source),
            (PARTY_PATH, "sParty_GentlemanTucker", "trainer_party",
             old_tucker, prior_gym001_tucker_source),
            (TRAINER_METADATA, "GENTLEMAN_TUCKER", "trainer_metadata",
             old_tucker, prior_gym001_tucker_source),
        )
        for path, identity, kind, old, migrated in cases:
            with self.subTest(path=path, identity=identity):
                historical = git_source(HISTORICAL_BASE, path)
                approved = git_source(APPROVED_MAIN, path)
                current = Path(path).read_text()
                previous = _one_record(historical, identity, kind).group()
                reviewed = _one_record(approved, identity, kind).group()
                self.assertNotEqual(previous, reviewed)
                self.assertEqual(_one_record(current, identity, kind).group(), reviewed)
                expected = old(path, current)
                actual = migrated(path, current)
                self.assertEqual(actual, expected)  # entire source, not just one record
                self.assertEqual(_one_record(actual, identity, kind).group(), previous)
                # An unreviewed edit to the approved record fails closed.
                index = current.index(reviewed)
                changed = current[:index] + reviewed.replace(
                    "    },", "        // unauthorized change\n    },", 1
                ) + current[index + len(reviewed):]
                with self.assertRaises(AssertionError):
                    migrated(path, changed)
                # Even an unrelated edit that survives local record reversal
                # cannot meet the unchanged whole-source expected fingerprint.
                frozen_expected = hashlib.sha256(actual.encode()).hexdigest()
                unrelated = current + "\n// unrelated unauthorized byte\n"
                changed_source = migrated(path, unrelated)
                with self.assertRaises(AssertionError):
                    require_frozen_digest(
                        changed_source, expected_sha256=frozen_expected, path=path
                    )

    def test_original_frozen_manifests_are_byte_identical_to_approved_main(self):
        for path in FROZEN_SPECS:
            with self.subTest(path=path):
                preserved = git_source(APPROVED_MAIN, path)
                current = Path(path).read_text()
                self.assertEqual(current, preserved)
                manifest = json.loads(current)
                checked = 0
                for row in manifest.get("files", {}).values():
                    for key in ("before_sha256", "after_sha256"):
                        if key in row:
                            self.assertRegex(row[key], r"^[0-9a-f]{64}$")
                            checked += 1
                for value in manifest.get("preserved_files", {}).values():
                    self.assertRegex(value, r"^[0-9a-f]{64}$")
                    checked += 1
                self.assertGreater(checked, 0)

    def test_preserved_frozen_brock_sha_rejects_anomalous_source(self):
        manifest = json.loads(
            Path("tests/specifications/brock_weather_party_delta.json").read_text()
        )
        path = PARTY_PATH
        current = Path(path).read_text()
        # The frozen Brock after-digest is kept intact. This proves that
        # unnormalized, post-approval source cannot be silently treated as
        # an accepted old snapshot. Full chain reconstruction is a separate
        # acceptance prerequisite, not guessed here.
        expected = manifest["files"][path]["after_sha256"]
        with self.assertRaises(AssertionError):
            require_frozen_digest(
                current + "\n// unauthorized diff\n",
                expected_sha256=expected, path=path
            )


if __name__ == "__main__":
    unittest.main()
