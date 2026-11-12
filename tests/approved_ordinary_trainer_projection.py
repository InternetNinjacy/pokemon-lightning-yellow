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
        gym_pattern = r"static const struct TrainerMon\w+\s+sParty_LeaderKoga\[\]\s*=\s*\{.*?\n\};"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, result, re.S))
        assert len(gym_before) == len(gym_after) == 1, "LeaderKoga: protected original/new block count"
        assert gym_after[0].group() == "static const struct TrainerMonItemCustomMoves sParty_LeaderKoga[] = {\n    {\n        .iv = 0,\n        .lvl = 37,\n        .species = SPECIES_SCYTHER,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_SLASH, MOVE_WING_ATTACK, MOVE_SWORDS_DANCE, MOVE_FOCUS_ENERGY},\n    },\n    {\n        .iv = 0,\n        .lvl = 39,\n        .species = SPECIES_HERACROSS,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_BRICK_BREAK, MOVE_TAKE_DOWN, MOVE_COUNTER, MOVE_FURY_ATTACK},\n    },\n    {\n        .iv = 0,\n        .lvl = 37,\n        .species = SPECIES_ARBOK,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_BITE, MOVE_ACID, MOVE_SCREECH, MOVE_GLARE},\n    },\n    {\n        .iv = 0,\n        .lvl = 43,\n        .species = SPECIES_CROBAT,\n        .heldItem = ITEM_SCOPE_LENS,\n        .moves = {MOVE_AIR_CUTTER, MOVE_WING_ATTACK, MOVE_CONFUSE_RAY, MOVE_BITE},\n    },\n};", "LeaderKoga: source changed outside approved Gym contract"
        result = result[:gym_after[0].start()] + gym_before[0].group() + result[gym_after[0].end():]
    if str(path) == "src/data/trainers.h":
        gym_pattern = r"\[TRAINER_LEADER_KOGA\]\s*=\s*\{.*?\n    \},"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, result, re.S))
        assert len(gym_before) == len(gym_after) == 1, "TRAINER_LEADER_KOGA: protected original/new block count"
        assert gym_after[0].group() == "[TRAINER_LEADER_KOGA] = {\n        .trainerClass = TRAINER_CLASS_LEADER,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_MALE,\n        .trainerPic = TRAINER_PIC_LEADER_KOGA,\n        .trainerName = _(\"KOGA\"),\n        .items = {ITEM_HYPER_POTION, ITEM_HYPER_POTION, ITEM_FULL_HEAL},\n        .doubleBattle = FALSE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY,\n        .party = ITEM_CUSTOM_MOVES(sParty_LeaderKoga),\n    },", "TRAINER_LEADER_KOGA: source changed outside approved Gym contract"
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
