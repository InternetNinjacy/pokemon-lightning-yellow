"""Strict source-only reversal of twelve later-merged level-up learnset arrays.

No installed move distribution is edited. Every current array must match the
approved merged-main snapshot before projection to pinned historical baseline.
"""
import re
import subprocess

PATH = "src/data/pokemon/level_up_learnsets.h"
APPROVED_MAIN = "02240690581eb70d70219024fb7f55da2882308b"
HISTORICAL_BASE = "21a3630b3d51481ab10a1af9637fa1ced73722cc"
LEARNSETS = ("Vulpix", "Ninetales", "Oddish", "Gloom", "Vileplume",
             "Ponyta", "Rapidash", "Eevee", "Vaporeon", "Flareon",
             "Bellossom", "Espeon")


def _record(text, name):
    symbol = "s" + name + "LevelUpLearnset"
    pattern = r"static const u16 " + symbol + r"\[\]\s*=\s*\{.*?\n\};"
    matches = list(re.finditer(pattern, text, re.S))
    assert len(matches) == 1, symbol + ": unexpected missing/duplicate learnset"
    return matches[0]


def prior_twelve_learnsets_source(text):
    current = subprocess.check_output(["git", "show", APPROVED_MAIN + ":" + PATH], text=True)
    baseline = subprocess.check_output(["git", "show", HISTORICAL_BASE + ":" + PATH], text=True)
    for species in LEARNSETS:
        match = _record(text, species)
        approved = _record(current, species).group()
        old = _record(baseline, species).group()
        assert approved != old, species + ": baseline delta absent"
        assert match.group() == approved, species + ": current learnset drift"
        text = text[:match.start()] + old + text[match.end():]
    return text
