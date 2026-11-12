#!/usr/bin/env python3
"""GYM-001 approved Viridian Black Belt Takashi source delta."""
from pathlib import Path
import re
from approved_viridian_takashi_changes import BEFORE_PARTY, AFTER_PARTY, AFTER_META, prior_viridian_takashi_source

p = Path("src/data/trainer_parties.h").read_text()
t = Path("src/data/trainers.h").read_text()
script = Path("data/maps/ViridianCity_Gym/scripts.inc").read_text()
assert p.count(AFTER_PARTY) == 1 and p.count(BEFORE_PARTY) == 0
assert prior_viridian_takashi_source("src/data/trainer_parties.h", p).count(BEFORE_PARTY) == 1
m = re.search(r"\[TRAINER_BLACK_BELT_TAKASHI\] = \{(.*?)\n    \},", t, re.S)
assert m
for v in (".items = {}", ".doubleBattle = FALSE", ".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE", AFTER_META):
    assert v in m.group(1), v
assert "trainerbattle_single TRAINER_BLACK_BELT_TAKASHI" in script
print("PASS: Takashi Cacturne/Umbreon/Absol Lv38 IV100; original Singles/AI, no held items. Revive/Full Heal separately pending.")
