#!/usr/bin/env python3
"""Owner-approved Surge Double leader source contract; runtime acceptance is separate."""
from pathlib import Path
import re
root=Path(__file__).resolve().parents[1]
parties=(root/"src/data/trainer_parties.h").read_text()
trainers=(root/"src/data/trainers.h").read_text()
script=(root/"data/maps/VermilionCity_Gym/scripts.inc").read_text()
move_ids=(root/"include/constants/moves.h").read_text()
party=re.search(r"static const struct TrainerMonNoItemCustomMoves sParty_LeaderLtSurge\[\] = \{(.*?)\n\};",parties,re.S)
assert party
entries=re.findall(r"\{\s*\.iv = (\d+),\s*\.lvl = (\d+),\s*\.species = SPECIES_(\w+),\s*\.moves = \{([^}]+)\},\s*\}",party[1],re.S)
expected=[
("RAICHU",30,"ZAP_CANNON QUICK_ATTACK BRICK_BREAK AGILITY"),
("VOLTORB",24,"THUNDER_WAVE SCREECH NONE NONE"),
("POLIWAG",24,"RAIN_DANCE SCARY_FACE NONE NONE"),
("ABRA",24,"FAKE_TEARS HELPING_HAND NONE NONE"),
]
assert len(entries)==4
for (iv,level,species,moves),(want_sp,want_level,want_moves) in zip(entries,expected):
    assert int(iv)==0 and (species,int(level))==(want_sp,want_level)
    actual=[x.strip() for x in moves.split(",")]
    assert actual==["MOVE_"+x for x in want_moves.split()]
    assert all(re.search(r"\b"+re.escape(x)+r"\b",move_ids) for x in actual)
trainer=re.search(r"\[TRAINER_LEADER_LT_SURGE\] = \{(.*?)\n    \},",trainers,re.S)
assert trainer
for expected in (
    ".items = {ITEM_SUPER_POTION, ITEM_FULL_HEAL}",
    ".doubleBattle = TRUE",
    ".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY",
    ".party = NO_ITEM_CUSTOM_MOVES(sParty_LeaderLtSurge)",
):
    assert expected in trainer[1],expected
assert ("trainerbattle_double TRAINER_LEADER_LT_SURGE, VermilionCity_Gym_Text_LtSurgeIntro, "
        "VermilionCity_Gym_Text_LtSurgeDefeat, SaffronCity_Dojo_Text_NeedTwo, "
        "VermilionCity_Gym_EventScript_DefeatedLtSurge, NO_MUSIC") in script
assert "trainerbattle_single TRAINER_LEADER_LT_SURGE" not in script
assert "setflag FLAG_BADGE03_GET" in script
# Separate TM51 reward implementation is not part of this leader-party PR.
assert "ITEM_TM34" in script
print("GYM-002 Surge leader source PASS: exact four Pokémon and ordered moves, no held items, approved healing bag, Double event with two-mon refusal, existing AI and victory path preserved.")
