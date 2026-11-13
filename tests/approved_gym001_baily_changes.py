"""Reverse only the approved Vermilion Engineer Baily party for frozen historical checks."""
from pathlib import Path

PARTY_PATH = "src/data/trainer_parties.h"
BEFORE = """static const struct TrainerMonNoItemDefaultMoves sParty_EngineerBaily[] = {
    {
        .iv = 0,
        .lvl = 21,
        .species = SPECIES_VOLTORB,
    },
    {
        .iv = 0,
        .lvl = 21,
        .species = SPECIES_MAGNEMITE,
    },
};"""
AFTER = """static const struct TrainerMonNoItemDefaultMoves sParty_EngineerBaily[] = {
    {
        .iv = 0,
        .lvl = 8,
        .species = SPECIES_PICHU,
    },
    {
        .iv = 0,
        .lvl = 22,
        .species = SPECIES_FLAAFFY,
    },
};"""

def prior_gym001_baily_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    if str(path) != PARTY_PATH:
        return text
    assert text.count(AFTER) == 1, "Baily party drift or duplicate"
    assert BEFORE not in text, "Original Baily party unexpectedly duplicated"
    return text.replace(AFTER, BEFORE, 1)
