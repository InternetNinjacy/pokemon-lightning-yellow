"""Strictly reverse only approved Fuchsia ordinary pairs for frozen source audits."""
from pathlib import Path
PAIRS = [
  {
    "name": "TamerPhil",
    "before": "static const struct TrainerMonNoItemDefaultMoves sParty_TamerPhil[] = {\n    {\n        .iv = 40,\n        .lvl = 34,\n        .species = SPECIES_SANDSLASH,\n    },\n    {\n        .iv = 40,\n        .lvl = 34,\n        .species = SPECIES_ARBOK,\n    },\n};",
    "after": "static const struct TrainerMonNoItemDefaultMoves sParty_TamerPhil[] = {\n    {\n        .iv = 40,\n        .lvl = 34,\n        .species = SPECIES_VENONAT,\n    },\n    {\n        .iv = 40,\n        .lvl = 34,\n        .species = SPECIES_EKANS,\n    },\n};"
  },
  {
    "name": "JugglerShawn",
    "before": "static const struct TrainerMonNoItemDefaultMoves sParty_JugglerShawn[] = {\n    {\n        .iv = 0,\n        .lvl = 34,\n        .species = SPECIES_DROWZEE,\n    },\n    {\n        .iv = 0,\n        .lvl = 34,\n        .species = SPECIES_HYPNO,\n    },\n};",
    "after": "static const struct TrainerMonNoItemDefaultMoves sParty_JugglerShawn[] = {\n    {\n        .iv = 0,\n        .lvl = 34,\n        .species = SPECIES_ARIADOS,\n    },\n    {\n        .iv = 0,\n        .lvl = 34,\n        .species = SPECIES_GRIMER,\n    },\n};"
  },
  {
    "name": "JugglerNate",
    "before": "static const struct TrainerMonNoItemDefaultMoves sParty_JugglerNate[] = {\n    {\n        .iv = 0,\n        .lvl = 34,\n        .species = SPECIES_DROWZEE,\n    },\n    {\n        .iv = 0,\n        .lvl = 34,\n        .species = SPECIES_KADABRA,\n    },\n};",
    "after": "static const struct TrainerMonNoItemDefaultMoves sParty_JugglerNate[] = {\n    {\n        .iv = 0,\n        .lvl = 34,\n        .species = SPECIES_VENOMOTH,\n    },\n    {\n        .iv = 0,\n        .lvl = 34,\n        .species = SPECIES_WEEZING,\n    },\n};"
  }
]
def prior_fuchsia_three_source(path, data=None):
    if data is None:
        data = Path(path).read_text()
    if str(path) != "src/data/trainer_parties.h":
        return data
    for pair in PAIRS:
        before,after = pair["before"],pair["after"]
        if data.count(after)==1:
            data = data.replace(after,before,1)
        else:
            assert data.count(before)==1, "Unreviewed Fuchsia party: "+pair["name"]
    return data
