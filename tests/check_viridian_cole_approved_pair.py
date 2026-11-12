#!/usr/bin/env python3
"""Approved Viridian Tamer Cole species-only source delta."""
from pathlib import Path
import re
from approved_viridian_cole_changes import BEFORE, AFTER, prior_viridian_cole_source

parties = Path("src/data/trainer_parties.h").read_text()
metadata = Path("src/data/trainers.h").read_text()
script = Path("data/maps/ViridianCity_Gym/scripts.inc").read_text()
assert parties.count(AFTER) == 1
assert parties.count(BEFORE) == 0
assert prior_viridian_cole_source("src/data/trainer_parties.h", parties).count(BEFORE) == 1
block = re.search(r"\[TRAINER_TAMER_COLE\] = \{(.*?)\n    \},", metadata, re.S)
assert block
for v in (".items = {}", ".doubleBattle = FALSE",
          ".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE",
          ".party = NO_ITEM_DEFAULT_MOVES(sParty_TamerCole)"):
    assert v in block.group(1), v
assert "trainerbattle_single TRAINER_TAMER_COLE" in script
print("PASS: Viridian Tamer Cole has Raticate/Houndour at original Lv39 and IV40; native Singles, AI and other historical source preserved. Trainer Revive/Full Heal pending separate implementation.")
