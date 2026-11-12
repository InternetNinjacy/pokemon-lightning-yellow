#!/usr/bin/env python3
"""GYM-001 approved Viridian Tamer Jason party source contract only."""
from pathlib import Path
import re
from approved_viridian_jason_changes import BEFORE_PARTY, AFTER_PARTY, prior_viridian_jason_source
p = Path("src/data/trainer_parties.h").read_text()
t = Path("src/data/trainers.h").read_text()
script = Path("data/maps/ViridianCity_Gym/scripts.inc").read_text()
assert p.count(AFTER_PARTY) == 1 and p.count(BEFORE_PARTY) == 0
assert prior_viridian_jason_source("src/data/trainer_parties.h", p).count(BEFORE_PARTY) == 1
m = re.search(r"\[TRAINER_TAMER_JASON\] = \{(.*?)\n    \},", t, re.S)
assert m
for v in (".items = {}", ".doubleBattle = FALSE", ".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE", ".party = NO_ITEM_DEFAULT_MOVES(sParty_TamerJason)"):
    assert v in m.group(1), v
assert "trainerbattle_single TRAINER_TAMER_JASON" in script
print("PASS: Jason Carvanha/Umbreon Lv43 IV40, two no-item default-moves slots, original Singles/AI. Trainer Revive+Full Heal remains separately unresolved.")
