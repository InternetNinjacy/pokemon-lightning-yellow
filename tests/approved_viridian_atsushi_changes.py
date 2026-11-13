"""Strict reversal of the approved Viridian Atsushi pair and party item-format delta."""
from pathlib import Path
BEFORE_PARTY = "static const struct TrainerMonItemDefaultMoves sParty_BlackBeltAtsushi[] = {\n    {\n        .iv = 100,\n        .lvl = 40,\n        .species = SPECIES_MACHOP,\n        .heldItem = ITEM_BLACK_BELT,\n    },\n    {\n        .iv = 100,\n        .lvl = 40,\n        .species = SPECIES_MACHOKE,\n        .heldItem = ITEM_BLACK_BELT,\n    },\n};"
AFTER_PARTY = "static const struct TrainerMonNoItemDefaultMoves sParty_BlackBeltAtsushi[] = {\n    {\n        .iv = 100,\n        .lvl = 40,\n        .species = SPECIES_HOUNDOUR,\n    },\n    {\n        .iv = 100,\n        .lvl = 40,\n        .species = SPECIES_ARBOK,\n    },\n};"
BEFORE_METADATA = ".party = ITEM_DEFAULT_MOVES(sParty_BlackBeltAtsushi)"
AFTER_METADATA = ".party = NO_ITEM_DEFAULT_MOVES(sParty_BlackBeltAtsushi)"

def prior_viridian_atsushi_source(path, data=None):
    if data is None:
        data = Path(path).read_text()
    path = str(path)
    if path == "src/data/trainer_parties.h":
        old, new = BEFORE_PARTY, AFTER_PARTY
    elif path == "src/data/trainers.h":
        old, new = BEFORE_METADATA, AFTER_METADATA
    else:
        return data
    assert data.count(new) == 1, "Atsushi approved delta missing/altered"
    assert data.count(old) == 0, "Duplicate legacy Atsushi data"
    return data.replace(new, old, 1)
