"""Byte-strict historical reverse projection for the two subsequently updated Gym parties.

Use the already approved source snapshots in pinned history, never change
production game parties or frozen test hashes.
"""
import re
import subprocess

PATH = "src/data/trainer_parties.h"
APPROVED_MAIN = "02240690581eb70d70219024fb7f55da2882308b"
HISTORICAL_BASE = "21a3630b3d51481ab10a1af9637fa1ced73722cc"
SYMBOLS = ("sParty_LeaderLtSurge", "sParty_LeaderSabrina")


def _party(text, name):
    pattern = r"static const struct TrainerMon\w+\s+" + name + r"\[\]\s*=\s*\{.*?\};"
    found = list(re.finditer(pattern, text, re.S))
    assert len(found) == 1, "ambiguous leader party: " + name
    return found[0]


def prior_two_leader_party_source(text):
    current = subprocess.check_output(["git", "show", APPROVED_MAIN + ":" + PATH], text=True)
    old = subprocess.check_output(["git", "show", HISTORICAL_BASE + ":" + PATH], text=True)
    for name in SYMBOLS:
        actual = _party(text, name)
        approved = _party(current, name).group()
        historical = _party(old, name).group()
        assert approved != historical, name + ": historical delta absent"
        assert actual.group() == approved, name + ": changed beyond approved leader source"
        text = text[:actual.start()] + historical + text[actual.end():]
    return text
