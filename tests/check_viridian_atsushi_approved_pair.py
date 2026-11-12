#!/usr/bin/env python3
"""GYM-001 approved Viridian Atsushi native-party data delta only."""
from pathlib import Path
import re
from approved_viridian_atsushi_changes import (
    AFTER_PARTY, BEFORE_PARTY, AFTER_METADATA,
    prior_viridian_atsushi_source,
)

parties = Path("src/data/trainer_parties.h").read_text()
trainers = Path("src/data/trainers.h").read_text()
scripts = Path("data/maps/ViridianCity_Gym/scripts.inc").read_text()
assert parties.count(AFTER_PARTY) == 1
assert parties.count(BEFORE_PARTY) == 0
assert prior_viridian_atsushi_source("src/data/trainer_parties.h", parties).count(BEFORE_PARTY) == 1
m = re.search(r"\[TRAINER_BLACK_BELT_ATSUSHI\] = \{(.*?)\n    \},", trainers, re.S)
assert m
for x in (".items = {}", ".doubleBattle = FALSE",
          ".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE", AFTER_METADATA):
    assert x in m.group(1), x
assert "trainerbattle_single TRAINER_BLACK_BELT_ATSUSHI" in scripts
print("PASS: Atsushi Houndour/Arbok Lv40, original IV100, no held items; original Singles and AI. Trainer Revive/Full Heal remains a separate blocker.")
