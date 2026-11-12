#!/usr/bin/env python3
"""GYM-001 two existing 2-Pokemon Saffron ordinary trainers use native Double script."""
from pathlib import Path
import re

scripts = Path("data/maps/SaffronCity_Gym/scripts.inc").read_text()
metadata = Path("src/data/trainers.h").read_text()
parties = Path("src/data/trainer_parties.h").read_text()
for trainer, name in (("TRAINER_PSYCHIC_TYRON", "PsychicTyron"),
                      ("TRAINER_CHANNELER_AMANDA", "ChannelerAmanda")):
    party = re.search(r"sParty_" + name + r"\[\] = \{(.*?)\n\};", parties, re.S)
    assert party and len(re.findall(r"\.species = SPECIES_", party.group(1))) == 2, name
    meta = re.search(r"\[" + trainer + r"\] = \{(.*?)\n    \},", metadata, re.S)
    assert meta
    for token in (".doubleBattle = TRUE", ".items = {}", ".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE"):
        assert token in meta.group(1), (trainer, token)
    assert re.search(r"trainerbattle_double " + trainer + r", [^\n]*, SaffronCity_Dojo_Text_NeedTwo", scripts), trainer
    assert "trainerbattle_single " + trainer not in scripts, trainer
print("PASS: Saffron Tyron/Amanda reuse original Double script, 2 slots, original AI and no items; party species remain pending.")
