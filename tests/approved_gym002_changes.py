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
        gym_pattern = r"static const struct TrainerMon\w+\s+sParty_BossGiovanni\[\]\s*=\s*\{.*?\n\};"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, source, re.S))
        assert len(gym_before) == len(gym_after) == 1, "BossGiovanni: protected original/new block count"
        assert gym_after[0].group() == "static const struct TrainerMonNoItemCustomMoves sParty_BossGiovanni[] = {\n    {\n        .iv = 250,\n        .lvl = 25,\n        .species = SPECIES_HOUNDOOM,\n        .moves = {MOVE_EMBER, MOVE_BITE, MOVE_SMOG, MOVE_HOWL},\n    },\n    {\n        .iv = 250,\n        .lvl = 26,\n        .species = SPECIES_SHARPEDO,\n        .moves = {MOVE_CRUNCH, MOVE_BITE, MOVE_FOCUS_ENERGY, MOVE_SCARY_FACE},\n    },\n    {\n        .iv = 250,\n        .lvl = 27,\n        .species = SPECIES_SHIFTRY,\n        .moves = {MOVE_NATURE_POWER, MOVE_FAKE_OUT, MOVE_GROWTH, MOVE_TORMENT},\n    },\n    {\n        .iv = 250,\n        .lvl = 29,\n        .species = SPECIES_TYRANITAR,\n        .moves = {MOVE_ROCK_SLIDE, MOVE_THRASH, MOVE_BITE, MOVE_DIG},\n    },\n};", "BossGiovanni: source changed outside approved Gym contract"
        source = source[:gym_after[0].start()] + gym_before[0].group() + source[gym_after[0].end():]
    if str(path) == "src/data/trainer_parties.h":
        gym_pattern = r"static const struct TrainerMon\w+\s+sParty_BossGiovanni2\[\]\s*=\s*\{.*?\n\};"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, source, re.S))
        assert len(gym_before) == len(gym_after) == 1, "BossGiovanni2: protected original/new block count"
        assert gym_after[0].group() == "static const struct TrainerMonNoItemCustomMoves sParty_BossGiovanni2[] = {\n    {\n        .iv = 250,\n        .lvl = 35,\n        .species = SPECIES_HOUNDOOM,\n        .moves = {MOVE_EMBER, MOVE_BITE, MOVE_HOWL, MOVE_ROAR},\n    },\n    {\n        .iv = 250,\n        .lvl = 37,\n        .species = SPECIES_SHARPEDO,\n        .moves = {MOVE_CRUNCH, MOVE_SLASH, MOVE_SCREECH, MOVE_FOCUS_ENERGY},\n    },\n    {\n        .iv = 250,\n        .lvl = 38,\n        .species = SPECIES_SHIFTRY,\n        .moves = {MOVE_NATURE_POWER, MOVE_FAKE_OUT, MOVE_FAINT_ATTACK, MOVE_RAZOR_WIND},\n    },\n    {\n        .iv = 250,\n        .lvl = 36,\n        .species = SPECIES_SNEASEL,\n        .moves = {MOVE_FAINT_ATTACK, MOVE_FURY_SWIPES, MOVE_SCREECH, MOVE_AGILITY},\n    },\n    {\n        .iv = 250,\n        .lvl = 41,\n        .species = SPECIES_TYRANITAR,\n        .moves = {MOVE_ROCK_SLIDE, MOVE_THRASH, MOVE_DIG, MOVE_SCARY_FACE},\n    },\n};", "BossGiovanni2: source changed outside approved Gym contract"
        source = source[:gym_after[0].start()] + gym_before[0].group() + source[gym_after[0].end():]
    if str(path) == "src/data/trainer_parties.h":
        gym_pattern = r"static const struct TrainerMon\w+\s+sParty_LeaderGiovanni\[\]\s*=\s*\{.*?\n\};"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, source, re.S))
        assert len(gym_before) == len(gym_after) == 1, "LeaderGiovanni: protected original/new block count"
        assert gym_after[0].group() == "static const struct TrainerMonItemCustomMoves sParty_LeaderGiovanni[] = {\n    {\n        .iv = 0,\n        .lvl = 44,\n        .species = SPECIES_HOUNDOOM,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_FLAMETHROWER, MOVE_FAINT_ATTACK, MOVE_BITE, MOVE_HOWL},\n    },\n    {\n        .iv = 0,\n        .lvl = 45,\n        .species = SPECIES_SHARPEDO,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_CRUNCH, MOVE_SLASH, MOVE_SCREECH, MOVE_SWAGGER},\n    },\n    {\n        .iv = 0,\n        .lvl = 46,\n        .species = SPECIES_SHIFTRY,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_FAINT_ATTACK, MOVE_RAZOR_WIND, MOVE_FAKE_OUT, MOVE_NATURE_POWER},\n    },\n    {\n        .iv = 0,\n        .lvl = 43,\n        .species = SPECIES_SNEASEL,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_ICY_WIND, MOVE_FAINT_ATTACK, MOVE_AGILITY, MOVE_SCREECH},\n    },\n    {\n        .iv = 0,\n        .lvl = 42,\n        .species = SPECIES_MURKROW,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_FAINT_ATTACK, MOVE_NIGHT_SHADE, MOVE_TAUNT, MOVE_PURSUIT},\n    },\n    {\n        .iv = 0,\n        .lvl = 50,\n        .species = SPECIES_TYRANITAR,\n        .heldItem = ITEM_HARD_STONE,\n        .moves = {MOVE_ROCK_SLIDE, MOVE_CRUNCH, MOVE_DIG, MOVE_THRASH},\n    },\n};", "LeaderGiovanni: source changed outside approved Gym contract"
        source = source[:gym_after[0].start()] + gym_before[0].group() + source[gym_after[0].end():]
    if str(path) == "src/data/trainers.h":
        gym_pattern = r"\[TRAINER_BOSS_GIOVANNI\]\s*=\s*\{.*?\n    \},"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, source, re.S))
        assert len(gym_before) == len(gym_after) == 1, "TRAINER_BOSS_GIOVANNI: protected original/new block count"
        assert gym_after[0].group() == "[TRAINER_BOSS_GIOVANNI] = {\n        .trainerClass = TRAINER_CLASS_BOSS,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_AQUA,\n        .trainerPic = TRAINER_PIC_LEADER_GIOVANNI,\n        .trainerName = _(\"GIOVANNI\"),\n        .items = {},\n        .doubleBattle = FALSE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE,\n        .party = NO_ITEM_CUSTOM_MOVES(sParty_BossGiovanni),\n    },", "TRAINER_BOSS_GIOVANNI: source changed outside approved Gym contract"
        source = source[:gym_after[0].start()] + gym_before[0].group() + source[gym_after[0].end():]
    if str(path) == "src/data/trainers.h":
        gym_pattern = r"\[TRAINER_BOSS_GIOVANNI_2\]\s*=\s*\{.*?\n    \},"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, source, re.S))
        assert len(gym_before) == len(gym_after) == 1, "TRAINER_BOSS_GIOVANNI_2: protected original/new block count"
        assert gym_after[0].group() == "[TRAINER_BOSS_GIOVANNI_2] = {\n        .trainerClass = TRAINER_CLASS_BOSS,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_AQUA,\n        .trainerPic = TRAINER_PIC_LEADER_GIOVANNI,\n        .trainerName = _(\"GIOVANNI\"),\n        .items = {},\n        .doubleBattle = FALSE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE,\n        .party = NO_ITEM_CUSTOM_MOVES(sParty_BossGiovanni2),\n    },", "TRAINER_BOSS_GIOVANNI_2: source changed outside approved Gym contract"
        source = source[:gym_after[0].start()] + gym_before[0].group() + source[gym_after[0].end():]
    if str(path) == "src/data/trainers.h":
        gym_pattern = r"\[TRAINER_LEADER_GIOVANNI\]\s*=\s*\{.*?\n    \},"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, source, re.S))
        assert len(gym_before) == len(gym_after) == 1, "TRAINER_LEADER_GIOVANNI: protected original/new block count"
        assert gym_after[0].group() == "[TRAINER_LEADER_GIOVANNI] = {\n        .trainerClass = TRAINER_CLASS_LEADER,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_AQUA,\n        .trainerPic = TRAINER_PIC_LEADER_GIOVANNI,\n        .trainerName = _(\"GIOVANNI\"),\n        .items = {ITEM_MAX_POTION, ITEM_SUPER_POTION, ITEM_FULL_RESTORE},\n        .doubleBattle = FALSE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY,\n        .party = ITEM_CUSTOM_MOVES(sParty_LeaderGiovanni),\n    },", "TRAINER_LEADER_GIOVANNI: source changed outside approved Gym contract"
        source = source[:gym_after[0].start()] + gym_before[0].group() + source[gym_after[0].end():]
    return source
