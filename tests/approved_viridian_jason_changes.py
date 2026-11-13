"""Strict reversible source adapter for the approved Viridian Tamer Jason party delta."""
from pathlib import Path
BEFORE_PARTY = "static const struct TrainerMonNoItemDefaultMoves sParty_TamerJason[] = {\n    {\n        .iv = 40,\n        .lvl = 43,\n        .species = SPECIES_RHYHORN,\n    },\n};"
AFTER_PARTY = "static const struct TrainerMonNoItemDefaultMoves sParty_TamerJason[] = {\n    {\n        .iv = 40,\n        .lvl = 43,\n        .species = SPECIES_CARVANHA,\n    },\n    {\n        .iv = 40,\n        .lvl = 43,\n        .species = SPECIES_UMBREON,\n    },\n};"
def prior_viridian_jason_source(path, data=None):
    if data is None:
        data = Path(path).read_text()
    if str(path) != "src/data/trainer_parties.h":
        return data
    assert data.count(AFTER_PARTY) == 1 and data.count(BEFORE_PARTY) == 0, "Unapproved Jason party delta"
    return data.replace(AFTER_PARTY, BEFORE_PARTY, 1)
