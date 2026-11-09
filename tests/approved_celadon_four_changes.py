"""Byte-exact reversals for approved Celadon ordinary-Trainer species pairs."""
from pathlib import Path
PAIRS = [
  {
    "name": "LassKay",
    "before": "static const struct TrainerMonNoItemDefaultMoves sParty_LassKay[] = {\n    {\n        .iv = 0,\n        .lvl = 23,\n        .species = SPECIES_BELLSPROUT,\n    },\n    {\n        .iv = 0,\n        .lvl = 23,\n        .species = SPECIES_WEEPINBELL,\n    },\n};",
    "after": "static const struct TrainerMonNoItemDefaultMoves sParty_LassKay[] = {\n    {\n        .iv = 0,\n        .lvl = 23,\n        .species = SPECIES_BELLSPROUT,\n    },\n    {\n        .iv = 0,\n        .lvl = 23,\n        .species = SPECIES_SLUGMA,\n    },\n};"
  },
  {
    "name": "LassLisa",
    "before": "static const struct TrainerMonNoItemDefaultMoves sParty_LassLisa[] = {\n    {\n        .iv = 0,\n        .lvl = 23,\n        .species = SPECIES_ODDISH,\n    },\n    {\n        .iv = 0,\n        .lvl = 23,\n        .species = SPECIES_GLOOM,\n    },\n};",
    "after": "static const struct TrainerMonNoItemDefaultMoves sParty_LassLisa[] = {\n    {\n        .iv = 0,\n        .lvl = 23,\n        .species = SPECIES_ODDISH,\n    },\n    {\n        .iv = 0,\n        .lvl = 23,\n        .species = SPECIES_VULPIX,\n    },\n};"
  },
  {
    "name": "PicnickerTina",
    "before": "static const struct TrainerMonNoItemDefaultMoves sParty_PicnickerTina[] = {\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_BULBASAUR,\n    },\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_IVYSAUR,\n    },\n};",
    "after": "static const struct TrainerMonNoItemDefaultMoves sParty_PicnickerTina[] = {\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_EXEGGCUTE,\n    },\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_PONYTA,\n    },\n};"
  },
  {
    "name": "BeautyTamia",
    "before": "static const struct TrainerMonNoItemDefaultMoves sParty_BeautyTamia[] = {\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_BELLSPROUT,\n    },\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_BELLSPROUT,\n    },\n};",
    "after": "static const struct TrainerMonNoItemDefaultMoves sParty_BeautyTamia[] = {\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_GLOOM,\n    },\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_GROWLITHE,\n    },\n};"
  }
]
def prior_celadon_four_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    if str(path) != "src/data/trainer_parties.h":
        return text
    for pair in PAIRS:
        before,after=pair["before"],pair["after"]
        if text.count(after)==1:
            text=text.replace(after,before,1)
        else:
            assert text.count(before)==1, "Unreviewed Celadon pair "+pair["name"]
    return text
