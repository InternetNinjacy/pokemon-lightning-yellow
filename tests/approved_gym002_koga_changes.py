"""Strictly reverse approved GYM-002 delta blocks before historical SHA audits.

Each modified block is compared byte-for-byte to its reviewed approved value;
unrelated text is never normalized. Baseline SHA guards remain unchanged.
"""
import re
import subprocess
from pathlib import Path

BASE = "21a3630b3d51481ab10a1af9637fa1ced73722cc"

def prior_koga_gym002_source(path, source=None):
    if source is None:
        source = Path(path).read_text()
    original = subprocess.check_output(["git", "show", BASE + ":" + str(path)], text=True)
    # Exact owner-approved GYM-002 deltas are reversed ONLY for frozen historical baselines.
    # The approved source-contract test checks each slot independently; this projection
    # additionally requires byte-for-byte agreement with the reviewed Gym block.
    if str(path) == "src/data/trainer_parties.h":
        gym_pattern = r"static const struct TrainerMon\w+\s+sParty_LeaderKoga\[\]\s*=\s*\{.*?\n\};"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, source, re.S))
        assert len(gym_before) == len(gym_after) == 1, "LeaderKoga: protected original/new block count"
        assert gym_after[0].group() == "static const struct TrainerMonItemCustomMoves sParty_LeaderKoga[] = {\n    {\n        .iv = 0,\n        .lvl = 37,\n        .species = SPECIES_SCYTHER,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_SLASH, MOVE_WING_ATTACK, MOVE_SWORDS_DANCE, MOVE_FOCUS_ENERGY},\n    },\n    {\n        .iv = 0,\n        .lvl = 39,\n        .species = SPECIES_HERACROSS,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_BRICK_BREAK, MOVE_TAKE_DOWN, MOVE_COUNTER, MOVE_FURY_ATTACK},\n    },\n    {\n        .iv = 0,\n        .lvl = 37,\n        .species = SPECIES_ARBOK,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_BITE, MOVE_ACID, MOVE_SCREECH, MOVE_GLARE},\n    },\n    {\n        .iv = 0,\n        .lvl = 43,\n        .species = SPECIES_CROBAT,\n        .heldItem = ITEM_SCOPE_LENS,\n        .moves = {MOVE_AIR_CUTTER, MOVE_WING_ATTACK, MOVE_CONFUSE_RAY, MOVE_BITE},\n    },\n};", "LeaderKoga: source changed outside approved Gym contract"
        source = source[:gym_after[0].start()] + gym_before[0].group() + source[gym_after[0].end():]
    if str(path) == "src/data/trainers.h":
        gym_pattern = r"\[TRAINER_LEADER_KOGA\]\s*=\s*\{.*?\n    \},"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, source, re.S))
        assert len(gym_before) == len(gym_after) == 1, "TRAINER_LEADER_KOGA: protected original/new block count"
        assert gym_after[0].group() == "[TRAINER_LEADER_KOGA] = {\n        .trainerClass = TRAINER_CLASS_LEADER,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_MALE,\n        .trainerPic = TRAINER_PIC_LEADER_KOGA,\n        .trainerName = _(\"KOGA\"),\n        .items = {ITEM_HYPER_POTION, ITEM_HYPER_POTION, ITEM_FULL_HEAL},\n        .doubleBattle = FALSE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY,\n        .party = ITEM_CUSTOM_MOVES(sParty_LeaderKoga),\n    },", "TRAINER_LEADER_KOGA: source changed outside approved Gym contract"
        source = source[:gym_after[0].start()] + gym_before[0].group() + source[gym_after[0].end():]
    return source
