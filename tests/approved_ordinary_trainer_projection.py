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
    # Undo only byte-exact owner-approved Erika blocks before frozen-source audits.
    if str(path) == "src/data/trainer_parties.h":
        gym_pattern = r"static const struct TrainerMon\w+\s+sParty_LeaderErika\[\]\s*=\s*\{.*?\n\};"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, result, re.S))
        assert len(gym_before) == len(gym_after) == 1, "LeaderErika: protected block count"
        assert gym_after[0].group() == "static const struct TrainerMonItemCustomMoves sParty_LeaderErika[] = {\n    {\n        .iv = 0,\n        .lvl = 29,\n        .species = SPECIES_BELLOSSOM,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_SOLAR_BEAM, MOVE_SYNTHESIS, MOVE_SLEEP_POWDER, MOVE_MAGICAL_LEAF},\n    },\n    {\n        .iv = 0,\n        .lvl = 30,\n        .species = SPECIES_RAPIDASH,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_FLAME_WHEEL, MOVE_FLASHFIRE, MOVE_STOMP, MOVE_TAKE_DOWN},\n    },\n    {\n        .iv = 0,\n        .lvl = 31,\n        .species = SPECIES_NINETALES,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_FIRE_SPIN, MOVE_ASTONISH, MOVE_CONFUSE_RAY, MOVE_WILL_O_WISP},\n    },\n    {\n        .iv = 0,\n        .lvl = 33,\n        .species = SPECIES_VILEPLUME,\n        .heldItem = ITEM_SITRUS_BERRY,\n        .moves = {MOVE_SLUDGE_BOMB, MOVE_MEGA_DRAIN, MOVE_SOLAR_BEAM, MOVE_MOONLIGHT},\n    },\n};", "LeaderErika: unauthorized changes"
        result = result[:gym_after[0].start()] + gym_before[0].group() + result[gym_after[0].end():]
    if str(path) == "src/data/trainers.h":
        gym_pattern = r"\[TRAINER_LEADER_ERIKA\]\s*=\s*\{.*?\n    \},"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, result, re.S))
        assert len(gym_before) == len(gym_after) == 1, "TRAINER_LEADER_ERIKA: protected block count"
        assert gym_after[0].group() == "[TRAINER_LEADER_ERIKA] = {\n        .trainerClass = TRAINER_CLASS_LEADER,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_FEMALE | F_TRAINER_FEMALE,\n        .trainerPic = TRAINER_PIC_LEADER_ERIKA,\n        .trainerName = _(\"ERIKA\"),\n        .items = {ITEM_SUPER_POTION, ITEM_SUPER_POTION},\n        .doubleBattle = FALSE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY,\n        .party = ITEM_CUSTOM_MOVES(sParty_LeaderErika),\n    },", "TRAINER_LEADER_ERIKA: unauthorized changes"
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
