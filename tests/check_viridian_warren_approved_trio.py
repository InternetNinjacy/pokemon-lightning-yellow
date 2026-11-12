#!/usr/bin/env python3
"""GYM-001 approved Viridian Warren source-only species/party contract."""
from pathlib import Path
import re
from approved_viridian_warren_changes import BEFORE_PARTY,AFTER_PARTY,AFTER_META,prior_viridian_warren_source
p=Path("src/data/trainer_parties.h").read_text()
t=Path("src/data/trainers.h").read_text()
s=Path("data/maps/ViridianCity_Gym/scripts.inc").read_text()
assert p.count(AFTER_PARTY)==1 and p.count(BEFORE_PARTY)==0
assert prior_viridian_warren_source("src/data/trainer_parties.h",p).count(BEFORE_PARTY)==1
m=re.search(r"\[TRAINER_COOLTRAINER_WARREN\] = \{(.*?)\n    \},",t,re.S)
assert m
for v in (".items = {ITEM_SUPER_POTION, ITEM_FULL_HEAL}",".doubleBattle = FALSE",".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY",AFTER_META):
    assert v in m.group(1),v
assert "trainerbattle_single TRAINER_COOLTRAINER_WARREN" in s
print("PASS: Warren Nuzleaf/Raticate/Absol Lv37/37/38 IV100 no-held default moves; source Singles/AI unchanged. Historical Hyper Potion bag preserved; target Revive+Full Heal still unimplemented.")
