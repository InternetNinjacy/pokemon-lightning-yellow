#!/usr/bin/env python3
"""GYM-001 Cerulean Luis: approved two-Pokemon Double Battle and encounter wiring."""
from pathlib import Path
import re
root = Path(__file__).resolve().parents[1]
parties = (root / "src/data/trainer_parties.h").read_text()
trainers = (root / "src/data/trainers.h").read_text()
scripts = (root / "data/maps/CeruleanCity_Gym/scripts.inc").read_text()
moves = (root / "include/constants/moves.h").read_text()
expected = [
    ("SLOWPOKE", 17, "WATER_GUN CONFUSION GROWL DISABLE"),
    ("HORSEA", 17, "WATER_GUN BUBBLE SMOKESCREEN LEER"),
]
body = re.search(r"static const struct TrainerMonNoItemCustomMoves sParty_SwimmerMaleLuis\[\] = \{(.*?)\n\};", parties, re.S)
assert body, "Luis party"
slots = re.findall(r"\{\s*\.iv = (\d+),\s*\.lvl = (\d+),\s*\.species = SPECIES_(\w+),\s*\.moves = \{([^}]+)\},\s*\}", body.group(1), re.S)
assert len(slots) == 2
for (iv, level, species, raw_moves), (want_species, want_level, want_moves) in zip(slots, expected):
    assert int(iv) == 0
    assert (species, int(level)) == (want_species, want_level)
    actual = [x.strip() for x in raw_moves.split(",")]
    assert actual == ["MOVE_" + x for x in want_moves.split()]
    assert all(re.search(r"\b" + re.escape(x) + r"\b", moves) for x in actual)
meta = re.search(r"\[TRAINER_SWIMMER_MALE_LUIS\] = \{(.*?)\n\s*\},", trainers, re.S)
assert meta
for text in (
    '.trainerName = _("LUIS")',
    '.items = {}',
    '.doubleBattle = TRUE',
    '.aiFlags = AI_SCRIPT_CHECK_BAD_MOVE',
    '.party = NO_ITEM_CUSTOM_MOVES(sParty_SwimmerMaleLuis)',
):
    assert text in meta.group(1), text
assert ('trainerbattle_double TRAINER_SWIMMER_MALE_LUIS, CeruleanCity_Gym_Text_LuisIntro, '
        'CeruleanCity_Gym_Text_LuisDefeat, SaffronCity_Dojo_Text_NeedTwo') in scripts
assert 'trainerbattle_single TRAINER_SWIMMER_MALE_LUIS' not in scripts
assert 'trainerbattle_double TRAINER_PICNICKER_DIANA' in scripts, 'completed Diana battle must remain intact'
print("GYM-001 Cerulean Luis PASS: approved Slowpoke/Horsea two-mon Double, legal move IDs, original AI/identity, refusal dialogue and Diana preserved.")
