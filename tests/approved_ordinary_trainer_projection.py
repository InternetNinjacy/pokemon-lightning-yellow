"""Reconstruct exact pre-ordinary-Trainer source for other-feature regressions.

Only the 500 non-dummy Trainer records explicitly authorized in the current
ordinary-Trainer implementation lock may differ. Unrelated content, including
all six currently implemented Elite Four party arrays, must remain byte-identical
to the pinned approved main source.
"""
import json
import re
import subprocess
from pathlib import Path

BASE = "21a3630b3d51481ab10a1af9637fa1ced73722cc"
LOCK = json.loads(Path("docs/ordinary-trainers-implementation-lock.json").read_text())


def restore_ordinary_trainer_blocks(path):
    original = subprocess.check_output(
        ["git", "show", BASE + ":" + str(path)], text=True)
    from approved_brock_weather_changes import prior_brock_weather_source
    result = prior_brock_weather_source(path)
    # Rocket Hideout #45 modifies only existing Grunt IDs 8–18. Revert
    # those exact approved records before the ordinary-Trainer baseline check;
    # unrelated changes still fail the final byte-for-byte comparison.
    if str(path) in ("src/data/trainer_parties.h", "src/data/trainers.h"):
        for n in range(8, 19):
            if str(path) == "src/data/trainer_parties.h":
                pattern = (
                    r"static const struct TrainerMon\w+\s+"
                    + re.escape("sParty_TeamRocketGrunt" + str(n))
                    + r"\[\]\s*=\s*\{.*?\};"
                )
            else:
                pattern = (
                    r"\[TRAINER_TEAM_ROCKET_GRUNT_" + str(n)
                    + r"\]\s*=\s*\{.*?\n    \},"
                )
            before = list(re.finditer(pattern, original, re.S))
            after = list(re.finditer(pattern, result, re.S))
            assert len(before) == len(after) == 1, (path, n)
            result = result[:after[0].start()] + before[0].group() + result[after[0].end():]
    # Exact owner-approved GYM-002 deltas are reversed ONLY for frozen historical baselines.
    # The approved source-contract test checks each slot independently; this projection
    # additionally requires byte-for-byte agreement with the reviewed Gym block.
    if str(path) == "src/data/trainer_parties.h":
        gym_pattern = r"static const struct TrainerMon\w+\s+sParty_BossGiovanni\[\]\s*=\s*\{.*?\n\};"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, result, re.S))
        assert len(gym_before) == len(gym_after) == 1, "BossGiovanni: protected original/new block count"
        assert gym_after[0].group() == "static const struct TrainerMonNoItemCustomMoves sParty_BossGiovanni[] = {\n    {\n        .iv = 250,\n        .lvl = 25,\n        .species = SPECIES_HOUNDOOM,\n        .moves = {MOVE_EMBER, MOVE_BITE, MOVE_SMOG, MOVE_HOWL},\n    },\n    {\n        .iv = 250,\n        .lvl = 26,\n        .species = SPECIES_SHARPEDO,\n        .moves = {MOVE_CRUNCH, MOVE_BITE, MOVE_FOCUS_ENERGY, MOVE_SCARY_FACE},\n    },\n    {\n        .iv = 250,\n        .lvl = 27,\n        .species = SPECIES_SHIFTRY,\n        .moves = {MOVE_NATURE_POWER, MOVE_FAKE_OUT, MOVE_GROWTH, MOVE_TORMENT},\n    },\n    {\n        .iv = 250,\n        .lvl = 29,\n        .species = SPECIES_TYRANITAR,\n        .moves = {MOVE_ROCK_SLIDE, MOVE_THRASH, MOVE_BITE, MOVE_DIG},\n    },\n};", "BossGiovanni: source changed outside approved Gym contract"
        result = result[:gym_after[0].start()] + gym_before[0].group() + result[gym_after[0].end():]
    if str(path) == "src/data/trainer_parties.h":
        gym_pattern = r"static const struct TrainerMon\w+\s+sParty_BossGiovanni2\[\]\s*=\s*\{.*?\n\};"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, result, re.S))
        assert len(gym_before) == len(gym_after) == 1, "BossGiovanni2: protected original/new block count"
        assert gym_after[0].group() == "static const struct TrainerMonNoItemCustomMoves sParty_BossGiovanni2[] = {\n    {\n        .iv = 250,\n        .lvl = 35,\n        .species = SPECIES_HOUNDOOM,\n        .moves = {MOVE_EMBER, MOVE_BITE, MOVE_HOWL, MOVE_ROAR},\n    },\n    {\n        .iv = 250,\n        .lvl = 37,\n        .species = SPECIES_SHARPEDO,\n        .moves = {MOVE_CRUNCH, MOVE_SLASH, MOVE_SCREECH, MOVE_FOCUS_ENERGY},\n    },\n    {\n        .iv = 250,\n        .lvl = 38,\n        .species = SPECIES_SHIFTRY,\n        .moves = {MOVE_NATURE_POWER, MOVE_FAKE_OUT, MOVE_FAINT_ATTACK, MOVE_RAZOR_WIND},\n    },\n    {\n        .iv = 250,\n        .lvl = 36,\n        .species = SPECIES_SNEASEL,\n        .moves = {MOVE_FAINT_ATTACK, MOVE_FURY_SWIPES, MOVE_SCREECH, MOVE_AGILITY},\n    },\n    {\n        .iv = 250,\n        .lvl = 41,\n        .species = SPECIES_TYRANITAR,\n        .moves = {MOVE_ROCK_SLIDE, MOVE_THRASH, MOVE_DIG, MOVE_SCARY_FACE},\n    },\n};", "BossGiovanni2: source changed outside approved Gym contract"
        result = result[:gym_after[0].start()] + gym_before[0].group() + result[gym_after[0].end():]
    if str(path) == "src/data/trainer_parties.h":
        gym_pattern = r"static const struct TrainerMon\w+\s+sParty_LeaderGiovanni\[\]\s*=\s*\{.*?\n\};"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, result, re.S))
        assert len(gym_before) == len(gym_after) == 1, "LeaderGiovanni: protected original/new block count"
        assert gym_after[0].group() == "static const struct TrainerMonItemCustomMoves sParty_LeaderGiovanni[] = {\n    {\n        .iv = 0,\n        .lvl = 44,\n        .species = SPECIES_HOUNDOOM,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_FLAMETHROWER, MOVE_FAINT_ATTACK, MOVE_BITE, MOVE_HOWL},\n    },\n    {\n        .iv = 0,\n        .lvl = 45,\n        .species = SPECIES_SHARPEDO,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_CRUNCH, MOVE_SLASH, MOVE_SCREECH, MOVE_SWAGGER},\n    },\n    {\n        .iv = 0,\n        .lvl = 46,\n        .species = SPECIES_SHIFTRY,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_FAINT_ATTACK, MOVE_RAZOR_WIND, MOVE_FAKE_OUT, MOVE_NATURE_POWER},\n    },\n    {\n        .iv = 0,\n        .lvl = 43,\n        .species = SPECIES_SNEASEL,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_ICY_WIND, MOVE_FAINT_ATTACK, MOVE_AGILITY, MOVE_SCREECH},\n    },\n    {\n        .iv = 0,\n        .lvl = 42,\n        .species = SPECIES_MURKROW,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_FAINT_ATTACK, MOVE_NIGHT_SHADE, MOVE_TAUNT, MOVE_PURSUIT},\n    },\n    {\n        .iv = 0,\n        .lvl = 50,\n        .species = SPECIES_TYRANITAR,\n        .heldItem = ITEM_HARD_STONE,\n        .moves = {MOVE_ROCK_SLIDE, MOVE_CRUNCH, MOVE_DIG, MOVE_THRASH},\n    },\n};", "LeaderGiovanni: source changed outside approved Gym contract"
        result = result[:gym_after[0].start()] + gym_before[0].group() + result[gym_after[0].end():]
    if str(path) == "src/data/trainers.h":
        gym_pattern = r"\[TRAINER_BOSS_GIOVANNI\]\s*=\s*\{.*?\n    \},"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, result, re.S))
        assert len(gym_before) == len(gym_after) == 1, "TRAINER_BOSS_GIOVANNI: protected original/new block count"
        assert gym_after[0].group() == "[TRAINER_BOSS_GIOVANNI] = {\n        .trainerClass = TRAINER_CLASS_BOSS,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_AQUA,\n        .trainerPic = TRAINER_PIC_LEADER_GIOVANNI,\n        .trainerName = _(\"GIOVANNI\"),\n        .items = {},\n        .doubleBattle = FALSE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE,\n        .party = NO_ITEM_CUSTOM_MOVES(sParty_BossGiovanni),\n    },", "TRAINER_BOSS_GIOVANNI: source changed outside approved Gym contract"
        result = result[:gym_after[0].start()] + gym_before[0].group() + result[gym_after[0].end():]
    if str(path) == "src/data/trainers.h":
        gym_pattern = r"\[TRAINER_BOSS_GIOVANNI_2\]\s*=\s*\{.*?\n    \},"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, result, re.S))
        assert len(gym_before) == len(gym_after) == 1, "TRAINER_BOSS_GIOVANNI_2: protected original/new block count"
        assert gym_after[0].group() == "[TRAINER_BOSS_GIOVANNI_2] = {\n        .trainerClass = TRAINER_CLASS_BOSS,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_AQUA,\n        .trainerPic = TRAINER_PIC_LEADER_GIOVANNI,\n        .trainerName = _(\"GIOVANNI\"),\n        .items = {},\n        .doubleBattle = FALSE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE,\n        .party = NO_ITEM_CUSTOM_MOVES(sParty_BossGiovanni2),\n    },", "TRAINER_BOSS_GIOVANNI_2: source changed outside approved Gym contract"
        result = result[:gym_after[0].start()] + gym_before[0].group() + result[gym_after[0].end():]
    if str(path) == "src/data/trainers.h":
        gym_pattern = r"\[TRAINER_LEADER_GIOVANNI\]\s*=\s*\{.*?\n    \},"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, result, re.S))
        assert len(gym_before) == len(gym_after) == 1, "TRAINER_LEADER_GIOVANNI: protected original/new block count"
        assert gym_after[0].group() == "[TRAINER_LEADER_GIOVANNI] = {\n        .trainerClass = TRAINER_CLASS_LEADER,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_AQUA,\n        .trainerPic = TRAINER_PIC_LEADER_GIOVANNI,\n        .trainerName = _(\"GIOVANNI\"),\n        .items = {ITEM_MAX_POTION, ITEM_SUPER_POTION, ITEM_FULL_RESTORE},\n        .doubleBattle = FALSE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY,\n        .party = ITEM_CUSTOM_MOVES(sParty_LeaderGiovanni),\n    },", "TRAINER_LEADER_GIOVANNI: source changed outside approved Gym contract"
        result = result[:gym_after[0].start()] + gym_before[0].group() + result[gym_after[0].end():]
    for trainer_id, record in LOCK["entries"].items():
        if record["source_dummy"]:
            continue
        if str(path) == "src/data/trainer_parties.h":
            name = re.escape(record["party_symbol"])
            pattern = r"static const struct TrainerMon\w+\s+" + name + r"\[\]\s*=\s*\{.*?\};"
        elif str(path) == "src/data/trainers.h":
            pattern = r"\[" + re.escape(trainer_id) + r"\]\s*=\s*\{.*?\n    \},"
        else:
            raise ValueError("Unknown protected Trainer path " + str(path))
        before = list(re.finditer(pattern, original, re.S))
        after = list(re.finditer(pattern, result, re.S))
        assert len(before) == len(after) == 1, (path, trainer_id)
        result = result[:after[0].start()] + before[0].group() + result[after[0].end():]
    assert result == original, str(path) + ": unexpected change outside approved Trainer records"
    return result
