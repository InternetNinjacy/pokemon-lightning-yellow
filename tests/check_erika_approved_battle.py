#!/usr/bin/env python3
"""GYM-002 owner-approved Erika source contract (current approved leader register)."""
from pathlib import Path
import re
root = Path(__file__).resolve().parents[1]
parties = (root / "src/data/trainer_parties.h").read_text()
trainers = (root / "src/data/trainers.h").read_text()
moves = (root / "include/constants/moves.h").read_text()
items = (root / "include/constants/items.h").read_text()
expected = [
    ("BELLOSSOM", 29, "SOLAR_BEAM SYNTHESIS SLEEP_POWDER MAGICAL_LEAF", "ITEM_NONE"),
    ("RAPIDASH", 30, "FLAME_WHEEL FLASHFIRE STOMP TAKE_DOWN", "ITEM_NONE"),
    ("NINETALES", 31, "FIRE_SPIN ASTONISH CONFUSE_RAY WILL_O_WISP", "ITEM_NONE"),
    ("VILEPLUME", 33, "SLUDGE_BOMB MEGA_DRAIN SOLAR_BEAM MOONLIGHT", "ITEM_SITRUS_BERRY"),
]
party = re.search(r"static const struct TrainerMonItemCustomMoves sParty_LeaderErika\[\] = \{(.*?)\n\};", parties, re.S)
assert party, "Erika party"
entries = re.findall(r"\{\s*\.iv = (\d+),\s*\.lvl = (\d+),\s*\.species = SPECIES_(\w+),\s*\.heldItem = (ITEM_\w+),\s*\.moves = \{([^}]+)\},\s*\}", party.group(1), re.S)
assert len(entries) == 4, len(entries)
for (iv, level, species, held, raw_moves), (want_species, want_level, want_moves, want_held) in zip(entries, expected):
    assert int(iv) == 0
    assert (species, int(level), held) == (want_species, want_level, want_held)
    actual = [x.strip() for x in raw_moves.split(",")]
    assert actual == ["MOVE_" + name for name in want_moves.split()]
    for move in actual:
        assert re.search(r"\b" + re.escape(move) + r"\b", moves), move
assert "ITEM_SITRUS_BERRY" in items
trainer = re.search(r"\[TRAINER_LEADER_ERIKA\] = \{(.*?)\n\s*\},", trainers, re.S)
assert trainer, "Erika metadata"
body = trainer.group(1)
for literal in (
    ".items = {ITEM_SUPER_POTION, ITEM_SUPER_POTION}",
    ".doubleBattle = FALSE",
    ".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY",
    ".party = ITEM_CUSTOM_MOVES(sParty_LeaderErika)",
):
    assert literal in body, literal
print("GYM-002 Erika source PASS: exact four, sixteen moves, berry, two Super Potions, Singles and existing AI.")
