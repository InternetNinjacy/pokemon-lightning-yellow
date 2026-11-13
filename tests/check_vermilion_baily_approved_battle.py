#!/usr/bin/env python3
"""GYM-001 owner-approved Vermilion Engineer Baily paralysis Singles contract."""
from pathlib import Path
import re

root = Path(__file__).resolve().parents[1]
party = (root / "src/data/trainer_parties.h").read_text()
trainers = (root / "src/data/trainers.h").read_text()
script = (root / "data/maps/VermilionCity_Gym/scripts.inc").read_text()
dialogue = (root / "data/maps/VermilionCity_Gym/text.inc").read_text()
learn = (root / "src/data/pokemon/level_up_learnsets.h").read_text()
species = (root / "src/data/pokemon/species_info.h").read_text()

p = re.search(r"static const struct TrainerMonNoItemDefaultMoves sParty_EngineerBaily\[\] = \{(.*?)\n\};", party, re.S)
assert p, "Baily must use ordinary level-up moves, no special moves or items"
entries = re.findall(r"\{\s*\.iv = (\d+),\s*\.lvl = (\d+),\s*\.species = SPECIES_(\w+),\s*\}", p[1], re.S)
assert entries == [("0", "8", "PICHU"), ("0", "22", "FLAAFFY")], entries
t = re.search(r"\[TRAINER_ENGINEER_BAILY\] = \{(.*?)\n    \},", trainers, re.S)
assert t
for clause in ('.trainerClass = TRAINER_CLASS_ENGINEER', '.trainerName = _("BAILY")',
               '.items = {}', '.doubleBattle = FALSE',
               '.aiFlags = AI_SCRIPT_CHECK_BAD_MOVE',
               '.party = NO_ITEM_DEFAULT_MOVES(sParty_EngineerBaily)'):
    assert clause in t[1], clause
assert "trainerbattle_single TRAINER_ENGINEER_BAILY" in script
assert 'Better bring a PARLYZ HEAL!' in dialogue
assert 'You might need it!$' in dialogue
assert "You weren't shocked at all!$" in dialogue
for mon, level in (("Pichu", 8), ("Flaaffy", 22)):
    moves = re.search(r"static const u16 s" + mon + r"LevelUpLearnset\[\] = \{(.*?)\n\};", learn, re.S)
    assert moves
    assert re.search(r"LEVEL_UP_MOVE\(" + str(level if mon == "Pichu" else 18) +
                     r", MOVE_THUNDER_WAVE\)", moves[1])
    info = re.search(r"\[SPECIES_" + mon.upper() + r"\] =\s*\{(.*?)\n    \},", species, re.S)
    assert info and ".abilities = {ABILITY_STATIC, ABILITY_NONE}" in info[1]
print("PASS: Baily Pichu8/Flaaffy22, natural Thunder Wave, Static, Singles, no items, approved text and unchanged trainer/map identity")
