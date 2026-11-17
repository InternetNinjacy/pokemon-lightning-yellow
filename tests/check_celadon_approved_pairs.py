#!/usr/bin/env python3
"""GYM-001 four exact owner-approved Celadon Grass/Fire species pairs."""
from pathlib import Path
import re
root=Path(__file__).resolve().parents[1]
p=(root/"src/data/trainer_parties.h").read_text()
t=(root/"src/data/trainers.h").read_text()
s=(root/"data/maps/CeladonCity_Gym/scripts.inc").read_text()
for party,trainer,lvl,expected in (
    ("LassKay","TRAINER_LASS_KAY",23,("BELLSPROUT","SLUGMA")),
    ("LassLisa","TRAINER_LASS_LISA",23,("ODDISH","VULPIX")),
    ("PicnickerTina","TRAINER_PICNICKER_TINA",24,("EXEGGCUTE","PONYTA")),
    ("BeautyTamia","TRAINER_BEAUTY_TAMIA",24,("GLOOM","GROWLITHE")),
):
    m=re.search(r"static const struct TrainerMonNoItemDefaultMoves sParty_"+party+r"\[\] = \{(.*?)\n\};",p,re.S)
    assert m,party
    slots=re.findall(r"\.iv = (\d+),\s*\.lvl = (\d+),\s*\.species = SPECIES_(\w+),",m[1])
    assert slots==[("0",str(lvl),expected[0]),("0",str(lvl),expected[1])],(party,slots)
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
assert "trainerbattle_single TRAINER_COOLTRAINER_MARY" in s
assert "sParty_CooltrainerMary[] = {" in p
print("GYM-001 Celadon PASS: four exact species pairs at unchanged FireRed levels/IVs, no items, standard Singles/AI, and Mary preserved.")
