"""Strict reversible owner-approved Cooltrainer Yuji original-source delta."""
from pathlib import Path
BEFORE_PARTY = "static const struct TrainerMonNoItemCustomMoves sParty_CooltrainerYuji[] = {\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_SANDSLASH,\n        .moves = {MOVE_SLASH, MOVE_SWIFT, MOVE_SAND_ATTACK, MOVE_POISON_STING},\n    },\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_GRAVELER,\n        .moves = {MOVE_ROCK_BLAST, MOVE_MAGNITUDE, MOVE_MUD_SPORT, MOVE_DEFENSE_CURL},\n    },\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_ONIX,\n        .moves = {MOVE_DRAGON_BREATH, MOVE_SANDSTORM, MOVE_ROCK_THROW, MOVE_BIND},\n    },\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_GRAVELER,\n        .moves = {MOVE_ROCK_BLAST, MOVE_MAGNITUDE, MOVE_ROLLOUT, MOVE_DEFENSE_CURL},\n    },\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_MAROWAK,\n        .moves = {MOVE_BONEMERANG, MOVE_HEADBUTT, MOVE_LEER, MOVE_GROWL},\n    },\n};"
AFTER_PARTY = "static const struct TrainerMonNoItemDefaultMoves sParty_CooltrainerYuji[] = {\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_UMBREON,\n    },\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_ABSOL,\n    },\n};"
BEFORE_META = ".party = NO_ITEM_CUSTOM_MOVES(sParty_CooltrainerYuji)"
AFTER_META = ".party = NO_ITEM_DEFAULT_MOVES(sParty_CooltrainerYuji)"
def prior_viridian_yuji_source(path, data=None):
    if data is None:
        data = Path(path).read_text()
    if str(path) == "src/data/trainer_parties.h":
        old, new = BEFORE_PARTY, AFTER_PARTY
    elif str(path) == "src/data/trainers.h":
        old, new = BEFORE_META, AFTER_META
    else:
        return data
    assert data.count(new)==1 and data.count(old)==0, "Unapproved Yuji change"
    return data.replace(new,old,1)
