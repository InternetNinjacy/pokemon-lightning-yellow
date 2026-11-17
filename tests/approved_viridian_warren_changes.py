"""Exact reversible source projection for owner-approved Viridian Cooltrainer Warren."""
from pathlib import Path
BEFORE_PARTY = "static const struct TrainerMonNoItemCustomMoves sParty_CooltrainerWarren[] = {\n    {\n        .iv = 100,\n        .lvl = 37,\n        .species = SPECIES_MAROWAK,\n        .moves = {MOVE_BONEMERANG, MOVE_HEADBUTT, MOVE_LEER, MOVE_GROWL},\n    },\n    {\n        .iv = 100,\n        .lvl = 37,\n        .species = SPECIES_MAROWAK,\n        .moves = {MOVE_BONEMERANG, MOVE_HEADBUTT, MOVE_LEER, MOVE_GROWL},\n    },\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_RHYHORN,\n        .moves = {MOVE_TAKE_DOWN, MOVE_ROCK_BLAST, MOVE_FURY_ATTACK, MOVE_SCARY_FACE},\n    },\n    {\n        .iv = 100,\n        .lvl = 39,\n        .species = SPECIES_NIDORINA,\n        .moves = {MOVE_FURY_SWIPES, MOVE_BITE, MOVE_POISON_STING, MOVE_DOUBLE_KICK},\n    },\n    {\n        .iv = 100,\n        .lvl = 39,\n        .species = SPECIES_NIDOQUEEN,\n        .moves = {MOVE_BODY_SLAM, MOVE_BITE, MOVE_POISON_STING, MOVE_DOUBLE_KICK},\n    },\n};"
AFTER_PARTY = "static const struct TrainerMonNoItemDefaultMoves sParty_CooltrainerWarren[] = {\n    {\n        .iv = 100,\n        .lvl = 37,\n        .species = SPECIES_NUZLEAF,\n    },\n    {\n        .iv = 100,\n        .lvl = 37,\n        .species = SPECIES_RATICATE,\n    },\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_ABSOL,\n    },\n};"
BEFORE_META = ".party = NO_ITEM_CUSTOM_MOVES(sParty_CooltrainerWarren)"
AFTER_META = ".party = NO_ITEM_DEFAULT_MOVES(sParty_CooltrainerWarren)"
def prior_viridian_warren_source(path, data=None):
    if data is None: data = Path(path).read_text()
    if str(path) == "src/data/trainer_parties.h":
        old,new = BEFORE_PARTY,AFTER_PARTY
    elif str(path) == "src/data/trainers.h":
        old,new = BEFORE_META,AFTER_META
    else:
        return data
    assert data.count(new)==1 and data.count(old)==0, "Unexpected Warren party change"
    return data.replace(new,old,1)
