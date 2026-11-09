"""Strictly reverse approved GYM-002 delta blocks before historical SHA audits.

Each modified block is compared byte-for-byte to its reviewed approved value;
unrelated text is never normalized. Baseline SHA guards remain unchanged.
"""
import re
import subprocess
from pathlib import Path

BASE = "21a3630b3d51481ab10a1af9637fa1ced73722cc"

def prior_gym002_source(path, source=None):
    if source is None:
        source = Path(path).read_text()
    original = subprocess.check_output(["git", "show", BASE + ":" + str(path)], text=True)
    # Exact owner-approved GYM-002 deltas are reversed ONLY for frozen historical baselines.
    # The approved source-contract test checks each slot independently; this projection
    # additionally requires byte-for-byte agreement with the reviewed Gym block.
    if str(path) == "src/data/trainer_parties.h":
        gym_pattern = r"static const struct TrainerMon\w+\s+sParty_LeaderSabrina\[\]\s*=\s*\{.*?\n\};"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, source, re.S))
        assert len(gym_before) == len(gym_after) == 1, "LeaderSabrina: protected original/new block count"
        assert gym_after[0].group() == "static const struct TrainerMonNoItemCustomMoves sParty_LeaderSabrina[] = {\n    {\n        .iv = 0,\n        .lvl = 43,\n        .species = SPECIES_GENGAR,\n        .moves = {MOVE_HYPNOSIS, MOVE_DREAM_EATER, MOVE_SHADOW_PUNCH, MOVE_DESTINY_BOND},\n    },\n    {\n        .iv = 0,\n        .lvl = 38,\n        .species = SPECIES_MACHAMP,\n        .moves = {MOVE_VITAL_THROW, MOVE_REVENGE, MOVE_FORESIGHT, MOVE_FOCUS_ENERGY},\n    },\n    {\n        .iv = 0,\n        .lvl = 38,\n        .species = SPECIES_DUSCLOPS,\n        .moves = {MOVE_SHADOW_PUNCH, MOVE_CONFUSE_RAY, MOVE_CURSE, MOVE_NIGHT_SHADE},\n    },\n    {\n        .iv = 0,\n        .lvl = 37,\n        .species = SPECIES_HITMONTOP,\n        .moves = {MOVE_TRIPLE_KICK, MOVE_COUNTER, MOVE_QUICK_ATTACK, MOVE_AGILITY},\n    },\n};", "LeaderSabrina: source changed outside approved Gym contract"
        source = source[:gym_after[0].start()] + gym_before[0].group() + source[gym_after[0].end():]
    if str(path) == "src/data/trainers.h":
        gym_pattern = r"\[TRAINER_LEADER_SABRINA\]\s*=\s*\{.*?\n    \},"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, source, re.S))
        assert len(gym_before) == len(gym_after) == 1, "TRAINER_LEADER_SABRINA: protected original/new block count"
        assert gym_after[0].group() == "[TRAINER_LEADER_SABRINA] = {\n        .trainerClass = TRAINER_CLASS_LEADER,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_FEMALE | F_TRAINER_FEMALE,\n        .trainerPic = TRAINER_PIC_LEADER_SABRINA,\n        .trainerName = _(\"SABRINA\"),\n        .items = {ITEM_HYPER_POTION, ITEM_HYPER_POTION, ITEM_FULL_HEAL},\n        .doubleBattle = TRUE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY,\n        .party = NO_ITEM_CUSTOM_MOVES(sParty_LeaderSabrina),\n    },", "TRAINER_LEADER_SABRINA: source changed outside approved Gym contract"
        source = source[:gym_after[0].start()] + gym_before[0].group() + source[gym_after[0].end():]
    return source
