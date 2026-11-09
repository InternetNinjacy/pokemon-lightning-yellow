"""Strictly reverse only approved GYM-002 Surge source deltas.

Byte-exact authorized blocks are restored for historical SHA checks. Unapproved
edits fail; already restored blocks pass idempotently in nested check chains.
"""
from pathlib import Path
import re
import subprocess

BASE = "1a84222a66461d150b624ec13aecde23d73e9fcd"

def prior_gym002_surge_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    baseline = subprocess.check_output(["git", "show", BASE + ":" + str(path)], text=True)
    if str(path) == "src/data/trainer_parties.h":
        pattern = r"static const struct TrainerMon\w+\s+sParty_LeaderLtSurge\[\]\s*=\s*\{.*?\n\};"
        before = list(re.finditer(pattern, baseline, re.S))
        after = list(re.finditer(pattern, text, re.S))
        assert len(before) == len(after) == 1, "sParty_LeaderLtSurge: expected one existing block"
        if after[0].group() == "static const struct TrainerMonNoItemCustomMoves sParty_LeaderLtSurge[] = {\n    {\n        .iv = 0,\n        .lvl = 30,\n        .species = SPECIES_RAICHU,\n        .moves = {MOVE_ZAP_CANNON, MOVE_QUICK_ATTACK, MOVE_BRICK_BREAK, MOVE_AGILITY},\n    },\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_VOLTORB,\n        .moves = {MOVE_THUNDER_WAVE, MOVE_SCREECH, MOVE_NONE, MOVE_NONE},\n    },\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_POLIWAG,\n        .moves = {MOVE_RAIN_DANCE, MOVE_SCARY_FACE, MOVE_NONE, MOVE_NONE},\n    },\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_ABRA,\n        .moves = {MOVE_FAKE_TEARS, MOVE_HELPING_HAND, MOVE_NONE, MOVE_NONE},\n    },\n};":
            text = text[:after[0].start()] + before[0].group() + text[after[0].end():]
        else:
            assert after[0].group() == before[0].group(), "sParty_LeaderLtSurge: unauthorized changes"
    if str(path) == "src/data/trainers.h":
        pattern = r"\[TRAINER_LEADER_LT_SURGE\]\s*=\s*\{.*?\n    \},"
        before = list(re.finditer(pattern, baseline, re.S))
        after = list(re.finditer(pattern, text, re.S))
        assert len(before) == len(after) == 1, "TRAINER_LEADER_LT_SURGE: expected one existing block"
        if after[0].group() == "[TRAINER_LEADER_LT_SURGE] = {\n        .trainerClass = TRAINER_CLASS_LEADER,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_MALE,\n        .trainerPic = TRAINER_PIC_LEADER_LT_SURGE,\n        .trainerName = _(\"LT. SURGE\"),\n        .items = {ITEM_SUPER_POTION, ITEM_FULL_HEAL},\n        .doubleBattle = TRUE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY,\n        .party = NO_ITEM_CUSTOM_MOVES(sParty_LeaderLtSurge),\n    },":
            text = text[:after[0].start()] + before[0].group() + text[after[0].end():]
        else:
            assert after[0].group() == before[0].group(), "TRAINER_LEADER_LT_SURGE: unauthorized changes"
    if str(path) == "data/maps/VermilionCity_Gym/scripts.inc":
        assert baseline.count("\ttrainerbattle_single TRAINER_LEADER_LT_SURGE, VermilionCity_Gym_Text_LtSurgeIntro, VermilionCity_Gym_Text_LtSurgeDefeat, VermilionCity_Gym_EventScript_DefeatedLtSurge, NO_MUSIC") == 1, "Surge original event"
        if text.count("\ttrainerbattle_double TRAINER_LEADER_LT_SURGE, VermilionCity_Gym_Text_LtSurgeIntro, VermilionCity_Gym_Text_LtSurgeDefeat, SaffronCity_Dojo_Text_NeedTwo, VermilionCity_Gym_EventScript_DefeatedLtSurge, NO_MUSIC") == 1:
            text = text.replace("\ttrainerbattle_double TRAINER_LEADER_LT_SURGE, VermilionCity_Gym_Text_LtSurgeIntro, VermilionCity_Gym_Text_LtSurgeDefeat, SaffronCity_Dojo_Text_NeedTwo, VermilionCity_Gym_EventScript_DefeatedLtSurge, NO_MUSIC", "\ttrainerbattle_single TRAINER_LEADER_LT_SURGE, VermilionCity_Gym_Text_LtSurgeIntro, VermilionCity_Gym_Text_LtSurgeDefeat, VermilionCity_Gym_EventScript_DefeatedLtSurge, NO_MUSIC", 1)
        else:
            assert text.count("\ttrainerbattle_single TRAINER_LEADER_LT_SURGE, VermilionCity_Gym_Text_LtSurgeIntro, VermilionCity_Gym_Text_LtSurgeDefeat, VermilionCity_Gym_EventScript_DefeatedLtSurge, NO_MUSIC") == 1, "Surge event changed unexpectedly"
    return text
