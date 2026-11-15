"""Reverse only the approved Vermilion Engineer Baily party for frozen historical checks."""
from pathlib import Path
from approved_historical_reconstruction import reverse_pinned_records

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
    # The old exact-text guards remain. The shared adapter additionally verifies
    # the complete party record against the pinned, previously approved main.
    return reverse_pinned_records(
        text,
        path=PARTY_PATH,
        approved_commit="02240690581eb70d70219024fb7f55da2882308b",
        historical_commit="21a3630b3d51481ab10a1af9637fa1ced73722cc",
        identities=("sParty_EngineerBaily",),
        kind="trainer_party",
    )
