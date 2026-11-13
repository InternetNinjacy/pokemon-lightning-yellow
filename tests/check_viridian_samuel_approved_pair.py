#!/usr/bin/env python3
"""GYM-001 Viridian Samuel approved-species source contract (not item-mechanics acceptance)."""
from pathlib import Path
import re
from approved_viridian_samuel_changes import BEFORE_PARTY,AFTER_PARTY,AFTER_META,prior_viridian_samuel_source
parties=Path("src/data/trainer_parties.h").read_text()
trainers=Path("src/data/trainers.h").read_text()
scripts=Path("data/maps/ViridianCity_Gym/scripts.inc").read_text()
assert parties.count(AFTER_PARTY)==1 and parties.count(BEFORE_PARTY)==0
assert prior_viridian_samuel_source("src/data/trainer_parties.h",parties).count(BEFORE_PARTY)==1
m=re.search(r"\[TRAINER_COOLTRAINER_SAMUEL\] = \{(.*?)\n    \},",trainers,re.S)
assert m
for item in (".items = {ITEM_SUPER_POTION, ITEM_FULL_HEAL}",".doubleBattle = FALSE",".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY",AFTER_META):
    assert item in m.group(1),item
assert "trainerbattle_single TRAINER_COOLTRAINER_SAMUEL" in scripts
print("PASS: Samuel Arbok/Cacturne Lv37 IV100 default moves, source Singles/AI intact; original Super Potion trainer bag unchanged and NOT compliant with pending Revive+Full Heal design.")
