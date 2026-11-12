#!/usr/bin/env python3
"""Approved GYM-001 Viridian Cooltrainer Yuji source contract only."""
from pathlib import Path
import re
from approved_viridian_yuji_changes import BEFORE_PARTY, AFTER_PARTY, AFTER_META, prior_viridian_yuji_source
p=Path("src/data/trainer_parties.h").read_text()
t=Path("src/data/trainers.h").read_text()
scripts=Path("data/maps/ViridianCity_Gym/scripts.inc").read_text()
assert p.count(AFTER_PARTY)==1 and p.count(BEFORE_PARTY)==0
assert prior_viridian_yuji_source("src/data/trainer_parties.h",p).count(BEFORE_PARTY)==1
m=re.search(r"\[TRAINER_COOLTRAINER_YUJI\] = \{(.*?)\n    \},",t,re.S)
assert m
for item in (".items = {ITEM_SUPER_POTION, ITEM_FULL_HEAL}",".doubleBattle = FALSE",".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY",AFTER_META):
    assert item in m.group(1),item
assert "trainerbattle_single TRAINER_COOLTRAINER_YUJI" in scripts
print("PASS: Yuji Umbreon/Absol Lv38 IV100 default moves; original Single/AI and historical Hyper Potion bag unchanged. Required Revive+Full Heal not implemented.")
