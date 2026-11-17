"""Exact reversible owner-approved Viridian Kiyo source-only delta."""
from pathlib import Path
BEFORE_PARTY = "static const struct TrainerMonItemDefaultMoves sParty_BlackBeltKiyo[] = {\n    {\n        .iv = 100,\n        .lvl = 43,\n        .species = SPECIES_MACHOKE,\n        .heldItem = ITEM_BLACK_BELT,\n    },\n};"
AFTER_PARTY = "static const struct TrainerMonNoItemDefaultMoves sParty_BlackBeltKiyo[] = {\n    {\n        .iv = 100,\n        .lvl = 43,\n        .species = SPECIES_NUZLEAF,\n    },\n    {\n        .iv = 100,\n        .lvl = 43,\n        .species = SPECIES_CARVANHA,\n    },\n};"
BEFORE_META = ".party = ITEM_DEFAULT_MOVES(sParty_BlackBeltKiyo)"
AFTER_META = ".party = NO_ITEM_DEFAULT_MOVES(sParty_BlackBeltKiyo)"
def prior_viridian_kiyo_source(path, data=None):
    if data is None: data = Path(path).read_text()
    if str(path) == "src/data/trainer_parties.h":
        old, new = BEFORE_PARTY, AFTER_PARTY
    elif str(path) == "src/data/trainers.h":
        old, new = BEFORE_META, AFTER_META
    else:
        return data
    assert data.count(new) == 1 and data.count(old) == 0, "Unapproved Kiyo edit"
    return data.replace(new, old, 1)
