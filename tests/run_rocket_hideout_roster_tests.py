#!/usr/bin/env python3
"""Static source-contract checks for the eleven Celadon Rocket Hideout battles.

Run from repository root: python3 tests/run_rocket_hideout_roster_tests.py
This is not a compiled ROM or emulator acceptance test.
"""
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parents[1]
PARTIES = (ROOT / "src/data/trainer_parties.h").read_text()
TRAINERS = (ROOT / "src/data/trainers.h").read_text()
CLASSES = (ROOT / "include/constants/trainers.h").read_text()
NAMES = (ROOT / "src/data/text/trainer_class_names.h").read_text()

EXPECTED = {
    8: [("MEOWTH", 24), ("EKANS", 24)],
    9: [("RATICATE", 25), ("MURKROW", 25)],
    10: [("HOUNDOUR", 24), ("SEEDOT", 24), ("CARVANHA", 25)],
    11: [("RATTATA", 24), ("NUZLEAF", 25), ("PERSIAN", 28), ("RATICATE", 26)],
    12: [("ARBOK", 27), ("SNEASEL", 27)],
    13: [("ARBOK", 30), ("PERSIAN", 30), ("SHARPEDO", 31), ("HOUNDOOM", 31)],
    14: [("RATICATE", 27), ("NUZLEAF", 28), ("MURKROW", 28), ("RALTS", 27)],
    15: [("HOUNDOUR", 28), ("SNEASEL", 29), ("SNORUNT", 27)],
    16: [("ARBOK", 31), ("CRAWDAUNT", 32), ("HOUNDOOM", 33)],
    17: [("SHIFTRY", 31), ("UMBREON", 32), ("ABSOL", 34)],
    18: [("PERSIAN", 29), ("CARVANHA", 29)],
}

assert len(EXPECTED) == 11
assert sum(map(len, EXPECTED.values())) == 32

for trainer_id, expected in EXPECTED.items():
    party = re.search(
        rf"static const struct (\w+) sParty_TeamRocketGrunt{trainer_id}\[\] = \{{(.*?)\n\}};",
        PARTIES, re.S,
    )
    assert party, f"Missing party {trainer_id}"
    assert party.group(1) == "TrainerMonNoItemDefaultMoves", trainer_id
    body = party.group(2)
    found = re.findall(r"\.lvl = (\d+),\s*\.species = SPECIES_(\w+)", body)
    assert [(species, int(level)) for level, species in found] == expected, trainer_id
    assert ".moves" not in body and ".heldItem" not in body
    trainer = re.search(
        rf"\[TRAINER_TEAM_ROCKET_GRUNT_{trainer_id}\] = \{{(.*?)\n    \}},",
        TRAINERS, re.S,
    )
    assert trainer, trainer_id
    record = trainer.group(1)
    assert f"NO_ITEM_DEFAULT_MOVES(sParty_TeamRocketGrunt{trainer_id})" in record
    assert ".items = {}" in record, trainer_id
    expected_class = "ROCKET_EXECUTIVE" if trainer_id in (16, 17) else "TEAM_ROCKET"
    assert f".trainerClass = TRAINER_CLASS_{expected_class}" in record, trainer_id
    if trainer_id in (16, 17):
        assert '.trainerName = _("EXECUTIVE")' in record

assert "#define TRAINER_CLASS_ROCKET_EXECUTIVE" in CLASSES
assert '[TRAINER_CLASS_ROCKET_EXECUTIVE]' in NAMES
assert '= _("ROCKET")' in NAMES
print("PASS: 11 Rocket Hideout teams; 32 ordered slots; default moves, no items; two Executive classes")
