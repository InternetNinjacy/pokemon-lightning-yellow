#!/usr/bin/env python3
"""Differential negative coverage for Phase 1 strict historical adapters.

Run explicitly with:
    python3 -m unittest discover -s tests -p test_historical_reconstruction_equivalence.py
This test is NOT a replacement for any registered host/native/CI requirement.
"""
import hashlib
import unittest
from unittest.mock import patch

from approved_historical_reconstruction import (
    reverse_exact_hunks,
    reverse_pinned_records,
    require_frozen_digest,
)
from approved_gym001_baily_changes import (
    AFTER as BAILY_AFTER, BEFORE as BAILY_BEFORE,
    PARTY_PATH as PARTY, prior_gym001_baily_source,
)
from approved_gym001_tucker_changes import (
    REVIEWED as TUCKER, prior_gym001_tucker_source,
)

APPROVED = "02240690581eb70d70219024fb7f55da2882308b"
HISTORICAL = "21a3630b3d51481ab10a1af9637fa1ced73722cc"
TRAINERS = "src/data/trainers.h"


def old_baily(path, data):
    if path != PARTY:
        return data
    assert data.count(BAILY_AFTER) == 1
    assert BAILY_BEFORE not in data
    return data.replace(BAILY_AFTER, BAILY_BEFORE, 1)


def old_tucker(path, data):
    for item in TUCKER:
        if item["path"] != path:
            continue
        if data.count(item["after"]) == 1:
            data = data.replace(item["after"], item["before"], 1)
        else:
            assert data.count(item["before"]) == 1
    return data


def source_for(path, approved):
    baily = BAILY_AFTER if approved else BAILY_BEFORE
    tucker = TUCKER[0]["after" if approved else "before"]
    if path == PARTY:
        return "// pinned party header\n" + baily + "\n" + tucker + "\n// other protected parties\n"
    if path == TRAINERS:
        return "// pinned trainers header\n" + TUCKER[1]["after" if approved else "before"] + "\n"
    raise AssertionError("unapproved source path: " + path)


def pinned_git(args, *, text):
    assert text is True
    assert len(args) == 3 and args[:2] == ["git", "show"], args
    commit, sep, path = args[2].partition(":")
    assert sep and commit in (APPROVED, HISTORICAL), args
    return source_for(path, commit == APPROVED)


class HistoricalReconstructionDifferential(unittest.TestCase):
    def setUp(self):
        self.patch = patch("approved_historical_reconstruction.subprocess.check_output",
                           side_effect=pinned_git)
        self.patch.start()
        self.addCleanup(self.patch.stop)

    def compare(self, name, old, new, path, source, should_accept):
        def attempt(fn):
            try:
                return (True, fn(path, source))
            except AssertionError:
                return (False, None)
        old_accept, old_result = attempt(old)
        new_accept, new_result = attempt(new)
        self.assertEqual((old_accept, new_accept), (should_accept, should_accept), name)
        if should_accept:
            self.assertEqual(old_result, new_result, name)
        return new_result

    def test_baily_exact_output_and_negative_mutations(self):
        source = source_for(PARTY, True)
        old, new = old_baily, prior_gym001_baily_source
        output = self.compare("accepted current Baily", old, new, PARTY, source, True)
        self.assertIn(BAILY_BEFORE, output)
        for name, changed in (
            ("species", source.replace("SPECIES_PICHU", "SPECIES_MEWTWO")),
            ("level", source.replace(".lvl = 8,", ".lvl = 9,")),
            ("duplicate new party", source + BAILY_AFTER),
            ("duplicate old party", source + BAILY_BEFORE),
            ("already historical Baily", source.replace(BAILY_AFTER, BAILY_BEFORE)),
        ):
            with self.subTest(name=name):
                self.compare(name, old, new, PARTY, changed, False)
        self.compare("other path pass-through", old, new, TRAINERS, "unrelated", True)

    def test_tucker_parties_metadata_and_old_fallback(self):
        for entry in TUCKER:
            path = entry["path"]
            source = source_for(path, True)
            with self.subTest(path=path):
                self.compare("approved", old_tucker, prior_gym001_tucker_source,
                             path, source, True)
                historical = source.replace(entry["after"], entry["before"])
                self.compare("already historical", old_tucker, prior_gym001_tucker_source,
                             path, historical, True)
                self.compare("duplicate current", old_tucker, prior_gym001_tucker_source,
                             path, source + entry["after"], False)
                if path == PARTY:
                    changed = source.replace("SPECIES_VOLTORB", "SPECIES_MEWTWO")
                else:
                    changed = source.replace("NO_ITEM_CUSTOM_MOVES", "ITEM_CUSTOM_MOVES")
                self.compare("unreviewed record", old_tucker, prior_gym001_tucker_source,
                             path, changed, False)
        self.compare("unrelated path", old_tucker, prior_gym001_tucker_source,
                     "include/global.h", "pass-through", True)

    def test_unrelated_changes_remain_rejected_by_unchanged_final_digest(self):
        for path, fn in ((PARTY, prior_gym001_baily_source),
                         (TRAINERS, prior_gym001_tucker_source)):
            original = source_for(path, True)
            output = fn(path, original)
            expected = hashlib.sha256(output.encode()).hexdigest()
            self.assertEqual(require_frozen_digest(output, expected_sha256=expected,
                                                  path=path), output)
            modified = original + "// unauthorized unrelated change\n"
            changed_output = fn(path, modified)
            with self.assertRaises(AssertionError):
                require_frozen_digest(changed_output, expected_sha256=expected,
                                      path=path)
            # The literal fingerprint is computed solely for this isolated
            # differential fixture, NOT substituted for project frozen hashes.

    def test_shared_helper_fails_closed_on_unpinned_or_unreviewed_data(self):
        source = source_for(PARTY, True)
        settings = dict(path=PARTY, approved_commit=APPROVED,
                        historical_commit=HISTORICAL,
                        identities=("sParty_EngineerBaily",), kind="trainer_party")
        with self.assertRaises(AssertionError):
            reverse_pinned_records(source, **{**settings, "approved_commit": "main"})
        with self.assertRaises(AssertionError):
            reverse_pinned_records(source, **{**settings, "identities": ("sParty_Unknown",)})
        with self.assertRaises(AssertionError):
            reverse_pinned_records(source, **{**settings, "identities":
                                             ("sParty_EngineerBaily", "sParty_EngineerBaily")})
        with self.assertRaises(AssertionError):
            reverse_pinned_records(source, **{**settings, "kind": "unexpected"})
        with self.assertRaises(AssertionError):
            reverse_exact_hunks("A", hunks=[{"before": "B", "after": "C"}],
                                label="unapproved")
        self.assertEqual(reverse_exact_hunks("A", hunks=[{"before": "B", "after": "A"}],
                                             label="approved"), "B")


if __name__ == "__main__":
    unittest.main()
