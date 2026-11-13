"""Strict reversible Viridian Cooltrainer Samuel original-party source adapter."""
from pathlib import Path
BEFORE_PARTY = "static const struct TrainerMonNoItemCustomMoves sParty_CooltrainerSamuel[] = {\n    {\n        .iv = 100,\n        .lvl = 37,\n        .species = SPECIES_SANDSLASH,\n        .moves = {MOVE_SLASH, MOVE_SWIFT, MOVE_SAND_ATTACK, MOVE_POISON_STING},\n    },\n    {\n        .iv = 100,\n        .lvl = 37,\n        .species = SPECIES_SANDSLASH,\n        .moves = {MOVE_SLASH, MOVE_SWIFT, MOVE_SAND_ATTACK, MOVE_POISON_STING},\n    },\n    {\n        .iv = 100,\n        .lvl = 38,\n        .species = SPECIES_RHYHORN,\n        .moves = {MOVE_TAKE_DOWN, MOVE_ROCK_BLAST, MOVE_FURY_ATTACK, MOVE_SCARY_FACE},\n    },\n    {\n        .iv = 100,\n        .lvl = 39,\n        .species = SPECIES_NIDORINO,\n        .moves = {MOVE_FURY_ATTACK, MOVE_HORN_ATTACK, MOVE_POISON_STING, MOVE_DOUBLE_KICK},\n    },\n    {\n        .iv = 100,\n        .lvl = 39,\n        .species = SPECIES_NIDOKING,\n        .moves = {MOVE_THRASH, MOVE_DOUBLE_KICK, MOVE_POISON_STING, MOVE_FOCUS_ENERGY},\n    },\n};"
AFTER_PARTY = "static const struct TrainerMonNoItemDefaultMoves sParty_CooltrainerSamuel[] = {\n    {\n        .iv = 100,\n        .lvl = 37,\n        .species = SPECIES_ARBOK,\n    },\n    {\n        .iv = 100,\n        .lvl = 37,\n        .species = SPECIES_CACTURNE,\n    },\n};"
BEFORE_META = ".party = NO_ITEM_CUSTOM_MOVES(sParty_CooltrainerSamuel)"
AFTER_META = ".party = NO_ITEM_DEFAULT_MOVES(sParty_CooltrainerSamuel)"
def prior_viridian_samuel_source(path, data=None):
    if data is None: data = Path(path).read_text()
    if str(path) == "src/data/trainer_parties.h": old,new = BEFORE_PARTY,AFTER_PARTY
    elif str(path) == "src/data/trainers.h": old,new = BEFORE_META,AFTER_META
    else: return data
    assert data.count(new)==1 and data.count(old)==0, "Unapproved Samuel party/metadata change"
    return data.replace(new,old,1)
