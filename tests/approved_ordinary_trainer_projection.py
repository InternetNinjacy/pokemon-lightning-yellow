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
        gym_pattern = r"static const struct TrainerMon\w+\s+sParty_LeaderSabrina\[\]\s*=\s*\{.*?\n\};"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, result, re.S))
        assert len(gym_before) == len(gym_after) == 1, "LeaderSabrina: protected original/new block count"
        assert gym_after[0].group() == "static const struct TrainerMonNoItemCustomMoves sParty_LeaderSabrina[] = {\n    {\n        .iv = 0,\n        .lvl = 43,\n        .species = SPECIES_GENGAR,\n        .moves = {MOVE_HYPNOSIS, MOVE_DREAM_EATER, MOVE_SHADOW_PUNCH, MOVE_DESTINY_BOND},\n    },\n    {\n        .iv = 0,\n        .lvl = 38,\n        .species = SPECIES_MACHAMP,\n        .moves = {MOVE_VITAL_THROW, MOVE_REVENGE, MOVE_FORESIGHT, MOVE_FOCUS_ENERGY},\n    },\n    {\n        .iv = 0,\n        .lvl = 38,\n        .species = SPECIES_DUSCLOPS,\n        .moves = {MOVE_SHADOW_PUNCH, MOVE_CONFUSE_RAY, MOVE_CURSE, MOVE_NIGHT_SHADE},\n    },\n    {\n        .iv = 0,\n        .lvl = 37,\n        .species = SPECIES_HITMONTOP,\n        .moves = {MOVE_TRIPLE_KICK, MOVE_COUNTER, MOVE_QUICK_ATTACK, MOVE_AGILITY},\n    },\n};", "LeaderSabrina: source changed outside approved Gym contract"
        result = result[:gym_after[0].start()] + gym_before[0].group() + result[gym_after[0].end():]
    if str(path) == "src/data/trainers.h":
        gym_pattern = r"\[TRAINER_LEADER_SABRINA\]\s*=\s*\{.*?\n    \},"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, result, re.S))
        assert len(gym_before) == len(gym_after) == 1, "TRAINER_LEADER_SABRINA: protected original/new block count"
        assert gym_after[0].group() == "[TRAINER_LEADER_SABRINA] = {\n        .trainerClass = TRAINER_CLASS_LEADER,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_FEMALE | F_TRAINER_FEMALE,\n        .trainerPic = TRAINER_PIC_LEADER_SABRINA,\n        .trainerName = _(\"SABRINA\"),\n        .items = {ITEM_HYPER_POTION, ITEM_HYPER_POTION, ITEM_FULL_HEAL},\n        .doubleBattle = TRUE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY,\n        .party = NO_ITEM_CUSTOM_MOVES(sParty_LeaderSabrina),\n    },", "TRAINER_LEADER_SABRINA: source changed outside approved Gym contract"
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
