#!/usr/bin/env python3
"""GYM-002: exact owner-approved Sabrina Double Battle party and metadata."""
from pathlib import Path
import re
root = Path(__file__).resolve().parents[1]
parties = (root / "src/data/trainer_parties.h").read_text()
trainers = (root / "src/data/trainers.h").read_text()
moves = (root / "include/constants/moves.h").read_text()
expected = [
    ("GENGAR", 43, "HYPNOSIS DREAM_EATER SHADOW_PUNCH DESTINY_BOND"),
    ("MACHAMP", 38, "VITAL_THROW REVENGE FORESIGHT FOCUS_ENERGY"),
    ("DUSCLOPS", 38, "SHADOW_PUNCH CONFUSE_RAY CURSE NIGHT_SHADE"),
    ("HITMONTOP", 37, "TRIPLE_KICK COUNTER QUICK_ATTACK AGILITY"),
]
match = re.search(r"static const struct TrainerMonNoItemCustomMoves sParty_LeaderSabrina\[\] = \{(.*?)\n\};", parties, re.S)
assert match, "Sabrina party declaration"
entries = re.findall(r"\{\s*\.iv = (\d+),\s*\.lvl = (\d+),\s*\.species = SPECIES_(\w+),\s*\.moves = \{([^}]+)\},\s*\}", match.group(1), re.S)
assert len(entries) == 4, entries
for (iv, level, species, raw_moves), (want_species, want_level, want_moves) in zip(entries, expected):
    assert int(iv) == 0
    assert (species, int(level)) == (want_species, want_level)
    actual = [x.strip() for x in raw_moves.split(",")]
    assert actual == ["MOVE_" + x for x in want_moves.split()]
    for move in actual:
        assert re.search(r"\b" + re.escape(move) + r"\b", moves)
trainer = re.search(r"\[TRAINER_LEADER_SABRINA\] = \{(.*?)\n\s*\},", trainers, re.S)
assert trainer
body = trainer.group(1)
for literal in (
    ".items = {ITEM_HYPER_POTION, ITEM_HYPER_POTION, ITEM_FULL_HEAL}",
    ".doubleBattle = TRUE",
    ".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY",
    ".party = NO_ITEM_CUSTOM_MOVES(sParty_LeaderSabrina)",
):
    assert literal in body, literal
script = (root / "data/maps/SaffronCity_Gym/scripts.inc").read_text()
assert (
    "trainerbattle_double TRAINER_LEADER_SABRINA, "
    "SaffronCity_Gym_Text_SabrinaIntro, "
    "SaffronCity_Gym_Text_SabrinaDefeat, "
    "SaffronCity_Dojo_Text_NeedTwo, "
    "SaffronCity_Gym_EventScript_DefeatedSabrina, NO_MUSIC"
) in script, "Leader script must enter the existing Double battle path"
assert "trainerbattle_single TRAINER_LEADER_SABRINA" not in script
print("GYM-002 Sabrina: mandated Gengar/Machamp opening, reserves, 16 moves, Double and AI verified.")
