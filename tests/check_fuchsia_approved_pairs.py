#!/usr/bin/env python3
"""GYM-001 exact owner-approved Fuchsia species; original level/IV defaults."""
from pathlib import Path
import re
root=Path(__file__).resolve().parents[1]
p=(root/"src/data/trainer_parties.h").read_text()
t=(root/"src/data/trainers.h").read_text()
s=(root/"data/maps/FuchsiaCity_Gym/scripts.inc").read_text()
for party,trainer,expected,ivs in (
    ("TamerPhil","TRAINER_TAMER_PHIL",("VENONAT","EKANS"),(40,40)),
    ("JugglerShawn","TRAINER_JUGGLER_SHAWN",("ARIADOS","GRIMER"),(0,0)),
    ("JugglerNate","TRAINER_JUGGLER_NATE",("VENOMOTH","WEEZING"),(0,0)),
):
    m=re.search(r"static const struct TrainerMonNoItemDefaultMoves sParty_"+party+r"\[\] = \{(.*?)\n\};",p,re.S)
    assert m,party
    slots=re.findall(r"\.iv = (\d+),\s*\.lvl = (\d+),\s*\.species = SPECIES_(\w+),",m[1])
    assert slots==[(str(ivs[0]),"34",expected[0]),(str(ivs[1]),"34",expected[1])],(party,slots)
    b=re.search(r"\["+trainer+r"\] = \{(.*?)\n    \},",t,re.S)
    assert b,trainer
    for requirement in (
        ".items = {}",".doubleBattle = FALSE",
        ".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE",
        ".party = NO_ITEM_DEFAULT_MOVES(sParty_"+party+")",
    ):
        assert requirement in b[1],(trainer,requirement)
    assert "trainerbattle_single "+trainer in s,trainer
    assert "trainerbattle_double "+trainer not in s
print("GYM-001 Fuchsia PASS: three original two-member parties, exact approved species, Lv34, original IVs, no held/bag items, native moves/abilities/AI and Singles.")
