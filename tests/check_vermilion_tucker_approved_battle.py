#!/usr/bin/env python3
"""GYM-001 approved Vermilion third ordinary Gym Trainer contract."""
from pathlib import Path
import re
root=Path(__file__).resolve().parents[1]
p=(root/"src/data/trainer_parties.h").read_text()
t=(root/"src/data/trainers.h").read_text()
s=(root/"data/maps/VermilionCity_Gym/scripts.inc").read_text()
m=(root/"include/constants/moves.h").read_text()
block=re.search(r"static const struct TrainerMonNoItemCustomMoves sParty_GentlemanTucker\[\] = \{(.*?)\n\};",p,re.S)
assert block
slots=re.findall(r"\{\s*\.iv = (\d+),\s*\.lvl = (\d+),\s*\.species = SPECIES_(\w+),\s*\.moves = \{([^}]+)\},\s*\}",block[1],re.S)
assert slots==[("0","22","VOLTORB","MOVE_CHARGE, MOVE_SPARK, MOVE_NONE, MOVE_NONE")]
assert all(re.search(r"\b"+x+r"\b",m) for x in ("MOVE_CHARGE","MOVE_SPARK","MOVE_NONE"))
trainer=re.search(r"\[TRAINER_GENTLEMAN_TUCKER\] = \{(.*?)\n    \},",t,re.S)
assert trainer
for line in ('.trainerName = _("TUCKER")','.items = {}','.doubleBattle = FALSE',
             '.aiFlags = AI_SCRIPT_CHECK_BAD_MOVE',
             '.party = NO_ITEM_CUSTOM_MOVES(sParty_GentlemanTucker)'):
    assert line in trainer[1],line
assert 'trainerbattle_single TRAINER_GENTLEMAN_TUCKER' in s
print("GYM-001 Tucker PASS: exact Voltorb22 Charge/Spark/empty/empty, no item, original Singles/AI/Gym NPC.")
