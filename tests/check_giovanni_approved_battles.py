#!/usr/bin/env python3
"""GYM-002: exact owner-approved November 6 Giovanni source contract."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
PARTIES = (ROOT / "src/data/trainer_parties.h").read_text()
TRAINERS = (ROOT / "src/data/trainers.h").read_text()
MOVES = (ROOT / "include/constants/moves.h").read_text()
ITEMS = (ROOT / "include/constants/items.h").read_text()

TEAMS = {
    "BossGiovanni": [
        ("HOUNDOOM", 25, "EMBER BITE SMOG HOWL"),
        ("SHARPEDO", 26, "CRUNCH BITE FOCUS_ENERGY SCARY_FACE"),
        ("SHIFTRY", 27, "NATURE_POWER FAKE_OUT GROWTH TORMENT"),
        ("TYRANITAR", 29, "ROCK_SLIDE THRASH BITE DIG"),
    ],
    "BossGiovanni2": [
        ("HOUNDOOM", 35, "EMBER BITE HOWL ROAR"),
        ("SHARPEDO", 37, "CRUNCH SLASH SCREECH FOCUS_ENERGY"),
        ("SHIFTRY", 38, "NATURE_POWER FAKE_OUT FAINT_ATTACK RAZOR_WIND"),
        ("SNEASEL", 36, "FAINT_ATTACK FURY_SWIPES SCREECH AGILITY"),
        ("TYRANITAR", 41, "ROCK_SLIDE THRASH DIG SCARY_FACE"),
    ],
    "LeaderGiovanni": [
        ("HOUNDOOM", 44, "FLAMETHROWER FAINT_ATTACK BITE HOWL"),
        ("SHARPEDO", 45, "CRUNCH SLASH SCREECH SWAGGER"),
        ("SHIFTRY", 46, "FAINT_ATTACK RAZOR_WIND FAKE_OUT NATURE_POWER"),
        ("SNEASEL", 43, "ICY_WIND FAINT_ATTACK AGILITY SCREECH"),
        ("MURKROW", 42, "FAINT_ATTACK NIGHT_SHADE TAUNT PURSUIT"),
        ("TYRANITAR", 50, "ROCK_SLIDE CRUNCH DIG THRASH"),
    ],
}

def block(source, start):
    match = re.search(re.escape(start) + r"\s*=\s*\{(.*?)\n\};", source, re.S)
    assert match is not None, start
    return match.group(1)

def check_parties():
    for name, expected in TEAMS.items():
        kind = "Item" if name == "LeaderGiovanni" else "NoItem"
        body = block(PARTIES, f"static const struct TrainerMon{kind}CustomMoves sParty_{name}[]")
        mons = re.findall(r"\{\s*\.iv\s*=\s*(\d+),\s*\.lvl\s*=\s*(\d+),\s*\.species\s*=\s*SPECIES_(\w+),\s*(?:\.heldItem\s*=\s*(ITEM_\w+),\s*)?\.moves\s*=\s*\{([^}]+)\},\s*\}", body)
        assert len(mons) == len(expected), (name, len(mons), len(expected))
        for index, (actual, approved) in enumerate(zip(mons, expected)):
            iv, level, species, item, move_string = actual
            approved_species, approved_level, approved_moves = approved
            assert (species, int(level)) == (approved_species, approved_level), (name, index, actual)
            assert int(iv) == (0 if name == "LeaderGiovanni" else 250)
            assert item == ("ITEM_HARD_STONE" if name == "LeaderGiovanni" and index == 5 else "ITEM_NONE" if name == "LeaderGiovanni" else ""), (name, index, item)
            moves = [x.strip() for x in move_string.split(",")]
            assert moves == ["MOVE_" + x for x in approved_moves.split()], (name, index, moves)
            for move in moves:
                assert re.search(r"\b" + re.escape(move) + r"\b", MOVES), move

def check_trainers():
    for key, name, party_mode, items, ai in [
        ("TRAINER_BOSS_GIOVANNI", "BossGiovanni", "NO_ITEM_CUSTOM_MOVES", "", "AI_SCRIPT_CHECK_BAD_MOVE"),
        ("TRAINER_BOSS_GIOVANNI_2", "BossGiovanni2", "NO_ITEM_CUSTOM_MOVES", "", "AI_SCRIPT_CHECK_BAD_MOVE"),
        ("TRAINER_LEADER_GIOVANNI", "LeaderGiovanni", "ITEM_CUSTOM_MOVES", "ITEM_MAX_POTION, ITEM_SUPER_POTION, ITEM_FULL_RESTORE", "AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY"),
    ]:
        match = re.search(r"\[" + key + r"\]\s*=\s*\{(.*?)\n\s*\},", TRAINERS, re.S)
        assert match is not None, key
        body = match.group(1)
        assert ".items = {" + items + "}" in body, (key, "items")
        assert ".doubleBattle = FALSE" in body, (key, "battle format")
        assert ".aiFlags = " + ai + "," in body, (key, "AI")
        assert ".party = " + party_mode + "(sParty_" + name + ")" in body, (key, "party structure")
    for item in ("ITEM_HARD_STONE", "ITEM_MAX_POTION", "ITEM_SUPER_POTION", "ITEM_FULL_RESTORE"):
        assert item in ITEMS, item

if __name__ == "__main__":
    check_parties()
    check_trainers()
    print("GYM-002 Giovanni: 3 encounters, 15 approved slots, held/bag items, formats, and AI verified.")
