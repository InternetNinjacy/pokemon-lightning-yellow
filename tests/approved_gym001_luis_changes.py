"""Strict source projection for approved GYM-001 Cerulean Luis battle.

Exact approved party and metadata blocks must match byte-for-byte; only
those and the explicitly approved script command are reversed for older audits.
"""
from pathlib import Path
import subprocess
import re
BASE = "37230a9c513ed013fc6b402ec800bd1d13960492"

def prior_luis_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    original = subprocess.check_output(["git", "show", BASE + ":" + str(path)], text=True)
    if str(path) == "src/data/trainer_parties.h":
        pattern = r"static const struct TrainerMon\w+\s+sParty_SwimmerMaleLuis\[\]\s*=\s*\{.*?\n\};"
        before = list(re.finditer(pattern, original, re.S))
        after = list(re.finditer(pattern, text, re.S))
        assert len(before) == len(after) == 1, "SwimmerMaleLuis: protected block count"
        assert after[0].group() == "static const struct TrainerMonNoItemCustomMoves sParty_SwimmerMaleLuis[] = {\n    {\n        .iv = 0,\n        .lvl = 17,\n        .species = SPECIES_SLOWPOKE,\n        .moves = {MOVE_WATER_GUN, MOVE_CONFUSION, MOVE_GROWL, MOVE_DISABLE},\n    },\n    {\n        .iv = 0,\n        .lvl = 17,\n        .species = SPECIES_HORSEA,\n        .moves = {MOVE_WATER_GUN, MOVE_BUBBLE, MOVE_SMOKESCREEN, MOVE_LEER},\n    },\n};", "SwimmerMaleLuis: unexpected modification"
        text = text[:after[0].start()] + before[0].group() + text[after[0].end():]
    if str(path) == "src/data/trainers.h":
        pattern = r"\[TRAINER_SWIMMER_MALE_LUIS\]\s*=\s*\{.*?\n    \},"
        before = list(re.finditer(pattern, original, re.S))
        after = list(re.finditer(pattern, text, re.S))
        assert len(before) == len(after) == 1, "TRAINER_SWIMMER_MALE_LUIS: protected block count"
        assert after[0].group() == "[TRAINER_SWIMMER_MALE_LUIS] = {\n        .trainerClass = TRAINER_CLASS_SWIMMER_M,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_SWIMMER,\n        .trainerPic = TRAINER_PIC_SWIMMER_M,\n        .trainerName = _(\"LUIS\"),\n        .items = {},\n        .doubleBattle = TRUE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE,\n        .party = NO_ITEM_CUSTOM_MOVES(sParty_SwimmerMaleLuis),\n    },", "TRAINER_SWIMMER_MALE_LUIS: unexpected modification"
        text = text[:after[0].start()] + before[0].group() + text[after[0].end():]
    if str(path) == "data/maps/CeruleanCity_Gym/scripts.inc":
        assert text.count("\ttrainerbattle_double TRAINER_SWIMMER_MALE_LUIS, CeruleanCity_Gym_Text_LuisIntro, CeruleanCity_Gym_Text_LuisDefeat, SaffronCity_Dojo_Text_NeedTwo") == 1, "Luis exact Double trigger"
        assert original.count("\ttrainerbattle_single TRAINER_SWIMMER_MALE_LUIS, CeruleanCity_Gym_Text_LuisIntro, CeruleanCity_Gym_Text_LuisDefeat") == 1, "original Luis trigger"
        text = text.replace("\ttrainerbattle_double TRAINER_SWIMMER_MALE_LUIS, CeruleanCity_Gym_Text_LuisIntro, CeruleanCity_Gym_Text_LuisDefeat, SaffronCity_Dojo_Text_NeedTwo", "\ttrainerbattle_single TRAINER_SWIMMER_MALE_LUIS, CeruleanCity_Gym_Text_LuisIntro, CeruleanCity_Gym_Text_LuisDefeat", 1)
    return text
