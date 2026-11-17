"""Exact reversible owner-approved Viridian Takashi source-only delta."""
from pathlib import Path
BEFORE_PARTY = "static const struct TrainerMonItemDefaultMoves sParty_BlackBeltTakashi[] = {\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_MACHOKE,\n        .heldItem = ITEM_BLACK_BELT,\n    },\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_MACHOP,\n        .heldItem = ITEM_BLACK_BELT,\n    },\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_MACHOKE,\n        .heldItem = ITEM_BLACK_BELT,\n    },\n};"
AFTER_PARTY = "static const struct TrainerMonNoItemDefaultMoves sParty_BlackBeltTakashi[] = {\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_CACTURNE,\n    },\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_UMBREON,\n    },\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_ABSOL,\n    },\n};"
BEFORE_META = ".party = ITEM_DEFAULT_MOVES(sParty_BlackBeltTakashi)"
AFTER_META = ".party = NO_ITEM_DEFAULT_MOVES(sParty_BlackBeltTakashi)"
def prior_viridian_takashi_source(path, data=None):
    if data is None: data = Path(path).read_text()
    if str(path) == "src/data/trainer_parties.h":
        old, new = BEFORE_PARTY, AFTER_PARTY
    elif str(path) == "src/data/trainers.h":
        old, new = BEFORE_META, AFTER_META
    else:
        return data
    assert data.count(new) == 1 and data.count(old) == 0, "Unapproved Takashi edit"
    return data.replace(new, old, 1)
