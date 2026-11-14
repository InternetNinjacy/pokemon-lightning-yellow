"""Strict previous-history metadata projection for four later Trainer Doubles.

Matched whole records must be byte-identical to the pinned approved main source,
then restored to the corresponding historical records. Production source never changes.
"""
import re
import subprocess

PATH = "src/data/trainers.h"
APPROVED_MAIN = "02240690581eb70d70219024fb7f55da2882308b"
HISTORICAL_BASE = "21a3630b3d51481ab10a1af9637fa1ced73722cc"
TRAINERS = ("TRAINER_PSYCHIC_TYRON", "TRAINER_CHANNELER_AMANDA",
            "TRAINER_LEADER_LT_SURGE", "TRAINER_LEADER_SABRINA")


def _record(source, name):
    pat = r"\[" + re.escape(name) + r"\]\s*=\s*\{.*?\n    \},"
    found = list(re.finditer(pat, source, re.S))
    assert len(found) == 1, "ambiguous Trainer record " + name
    return found[0]


def prior_four_trainer_metadata_source(source):
    approved = subprocess.check_output(
        ["git", "show", APPROVED_MAIN + ":" + PATH], text=True)
    historical = subprocess.check_output(
        ["git", "show", HISTORICAL_BASE + ":" + PATH], text=True)
    for name in TRAINERS:
        actual = _record(source, name)
        after = _record(approved, name).group()
        before = _record(historical, name).group()
        assert after != before, name + ": missing documented doubleBattle delta"
        assert after.replace(".doubleBattle = TRUE,", ".doubleBattle = FALSE,", 1) == before, (
            name + ": unrelated historical metadata changed")
        assert actual.group() == after, name + ": unexpected approved Trainer drift"
        source = source[:actual.start()] + before + source[actual.end():]
    return source
