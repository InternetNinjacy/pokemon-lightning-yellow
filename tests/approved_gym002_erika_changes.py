"""Exact reversible Erika source projection for historical frozen SHA audits."""
import re
import subprocess
from pathlib import Path
BASE = "21a3630b3d51481ab10a1af9637fa1ced73722cc"
def prior_erika_gym002_source(path, source=None):
    if source is None:
        source = Path(path).read_text()
    original = subprocess.check_output(["git", "show", BASE + ":" + str(path)], text=True)
    # Undo only byte-exact owner-approved Erika blocks before frozen-source audits.
    if str(path) == "src/data/trainer_parties.h":
        gym_pattern = r"static const struct TrainerMon\w+\s+sParty_LeaderErika\[\]\s*=\s*\{.*?\n\};"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, source, re.S))
        assert len(gym_before) == len(gym_after) == 1, "LeaderErika: protected block count"
        assert gym_after[0].group() == "static const struct TrainerMonItemCustomMoves sParty_LeaderErika[] = {\n    {\n        .iv = 0,\n        .lvl = 29,\n        .species = SPECIES_BELLOSSOM,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_SOLAR_BEAM, MOVE_SYNTHESIS, MOVE_SLEEP_POWDER, MOVE_MAGICAL_LEAF},\n    },\n    {\n        .iv = 0,\n        .lvl = 30,\n        .species = SPECIES_RAPIDASH,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_FLAME_WHEEL, MOVE_FLASHFIRE, MOVE_STOMP, MOVE_TAKE_DOWN},\n    },\n    {\n        .iv = 0,\n        .lvl = 31,\n        .species = SPECIES_NINETALES,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_FIRE_SPIN, MOVE_ASTONISH, MOVE_CONFUSE_RAY, MOVE_WILL_O_WISP},\n    },\n    {\n        .iv = 0,\n        .lvl = 33,\n        .species = SPECIES_VILEPLUME,\n        .heldItem = ITEM_SITRUS_BERRY,\n        .moves = {MOVE_SLUDGE_BOMB, MOVE_MEGA_DRAIN, MOVE_SOLAR_BEAM, MOVE_MOONLIGHT},\n    },\n};", "LeaderErika: unauthorized changes"
        source = source[:gym_after[0].start()] + gym_before[0].group() + source[gym_after[0].end():]
    if str(path) == "src/data/trainers.h":
        gym_pattern = r"\[TRAINER_LEADER_ERIKA\]\s*=\s*\{.*?\n    \},"
        gym_before = list(re.finditer(gym_pattern, original, re.S))
        gym_after = list(re.finditer(gym_pattern, source, re.S))
        assert len(gym_before) == len(gym_after) == 1, "TRAINER_LEADER_ERIKA: protected block count"
        assert gym_after[0].group() == "[TRAINER_LEADER_ERIKA] = {\n        .trainerClass = TRAINER_CLASS_LEADER,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_FEMALE | F_TRAINER_FEMALE,\n        .trainerPic = TRAINER_PIC_LEADER_ERIKA,\n        .trainerName = _(\"ERIKA\"),\n        .items = {ITEM_SUPER_POTION, ITEM_SUPER_POTION},\n        .doubleBattle = FALSE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY,\n        .party = ITEM_CUSTOM_MOVES(sParty_LeaderErika),\n    },", "TRAINER_LEADER_ERIKA: unauthorized changes"
        source = source[:gym_after[0].start()] + gym_before[0].group() + source[gym_after[0].end():]
    return source
