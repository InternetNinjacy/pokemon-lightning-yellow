"""Byte-exact reversal of the sole approved Viridian Tamer Cole species delta."""
from pathlib import Path

BEFORE = """static const struct TrainerMonNoItemDefaultMoves sParty_TamerCole[] = {
    {
        .iv = 40,
        .lvl = 39,
        .species = SPECIES_ARBOK,
    },
    {
        .iv = 40,
        .lvl = 39,
        .species = SPECIES_TAUROS,
    },
};"""
AFTER = """static const struct TrainerMonNoItemDefaultMoves sParty_TamerCole[] = {
    {
        .iv = 40,
        .lvl = 39,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 40,
        .lvl = 39,
        .species = SPECIES_HOUNDOUR,
    },
};"""

def prior_viridian_cole_source(path, data=None):
    if data is None:
        data = Path(path).read_text()
    if str(path) != "src/data/trainer_parties.h":
        return data
    assert data.count(AFTER) == 1, "Cole approval missing or unexpectedly edited"
    assert data.count(BEFORE) == 0, "Original Cole party unexpectedly duplicated"
    return data.replace(AFTER, BEFORE, 1)
