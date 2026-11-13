#!/usr/bin/env python3
"""GYM-002 November 6 owner-approved Koga battle source contract."""
from pathlib import Path
import re
root = Path(__file__).resolve().parents[1]
parties = (root / "src/data/trainer_parties.h").read_text()
trainers = (root / "src/data/trainers.h").read_text()
moves = (root / "include/constants/moves.h").read_text()
items = (root / "include/constants/items.h").read_text()
expected = [
    ("SCYTHER", 37, "SLASH WING_ATTACK SWORDS_DANCE FOCUS_ENERGY", "ITEM_NONE"),
    ("HERACROSS", 39, "BRICK_BREAK TAKE_DOWN COUNTER FURY_ATTACK", "ITEM_NONE"),
    ("ARBOK", 37, "BITE ACID SCREECH GLARE", "ITEM_NONE"),
    ("CROBAT", 43, "AIR_CUTTER WING_ATTACK CONFUSE_RAY BITE", "ITEM_SCOPE_LENS"),
]
match = re.search(r"static const struct TrainerMonItemCustomMoves sParty_LeaderKoga\[\] = \{(.*?)\n\};", parties, re.S)
assert match, "Koga party declaration"
entries = re.findall(r"\{\s*\.iv = (\d+),\s*\.lvl = (\d+),\s*\.species = SPECIES_(\w+),\s*\.heldItem = (ITEM_\w+),\s*\.moves = \{([^}]+)\},\s*\}", match.group(1), re.S)
assert len(entries) == 4, entries
for (iv, level, species, held, raw_moves), (want_species, want_level, want_moves, want_held) in zip(entries, expected):
    assert int(iv) == 0
    assert (species, int(level), held) == (want_species, want_level, want_held)
    actual_moves = [x.strip() for x in raw_moves.split(",")]
    assert actual_moves == ["MOVE_" + x for x in want_moves.split()]
    for move in actual_moves:
        assert re.search(r"\b" + re.escape(move) + r"\b", moves)
assert "ITEM_SCOPE_LENS" in items
trainer = re.search(r"\[TRAINER_LEADER_KOGA\] = \{(.*?)\n\s*\},", trainers, re.S)
assert trainer
body = trainer.group(1)
for literal in (
    ".items = {ITEM_HYPER_POTION, ITEM_HYPER_POTION, ITEM_FULL_HEAL}",
    ".doubleBattle = FALSE",
    ".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY",
    ".party = ITEM_CUSTOM_MOVES(sParty_LeaderKoga)",
):
    assert literal in body, literal
print("GYM-002 Koga: four approved Pokemon, moves, levels, Scope Lens, Singles, bag and native AI verified.")
