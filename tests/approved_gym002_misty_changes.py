"""Exact approved Misty GYM-002 source reversal for immutable historical audits.

No historical hashes are changed. Each reviewed block is byte-exact;
unexpected changes are rejected and already-reversed paths remain idempotent.
"""
from pathlib import Path

REVIEWED = [
  {
    "path": "src/data/trainer_parties.h",
    "old": "static const struct TrainerMonNoItemCustomMoves sParty_LeaderMisty[] = {\n    {\n        .iv = 0,\n        .lvl = 18,\n        .species = SPECIES_STARYU,\n        .moves = {MOVE_TACKLE, MOVE_HARDEN, MOVE_RECOVER, MOVE_WATER_PULSE},\n    },\n    {\n        .iv = 0,\n        .lvl = 21,\n        .species = SPECIES_STARMIE,\n        .moves = {MOVE_SWIFT, MOVE_RECOVER, MOVE_RAPID_SPIN, MOVE_WATER_PULSE},\n    },\n};",
    "new": "static const struct TrainerMonItemCustomMoves sParty_LeaderMisty[] = {\n    {\n        .iv = 0,\n        .lvl = 22,\n        .species = SPECIES_STARYU,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_WATER_GUN, MOVE_RAPID_SPIN, MOVE_SWIFT, MOVE_RECOVER},\n    },\n    {\n        .iv = 0,\n        .lvl = 22,\n        .species = SPECIES_PSYDUCK,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_WATER_GUN, MOVE_CONFUSION, MOVE_DISABLE, MOVE_TAIL_WHIP},\n    },\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_GOLDUCK,\n        .heldItem = ITEM_NONE,\n        .moves = {MOVE_WATER_PULSE, MOVE_CONFUSION, MOVE_AMNESIA, MOVE_HYPNOSIS},\n    },\n    {\n        .iv = 0,\n        .lvl = 25,\n        .species = SPECIES_STARMIE,\n        .heldItem = ITEM_LUM_BERRY,\n        .moves = {MOVE_PSYBEAM, MOVE_RECOVER, MOVE_BUBBLE_BEAM, MOVE_THUNDER_WAVE},\n    },\n};",
    "label": "Misty party"
  },
  {
    "path": "src/data/trainers.h",
    "old": "[TRAINER_LEADER_MISTY] = {\n        .trainerClass = TRAINER_CLASS_LEADER,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_FEMALE | F_TRAINER_FEMALE,\n        .trainerPic = TRAINER_PIC_LEADER_MISTY,\n        .trainerName = _(\"MISTY\"),\n        .items = {ITEM_SUPER_POTION},\n        .doubleBattle = FALSE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY,\n        .party = NO_ITEM_CUSTOM_MOVES(sParty_LeaderMisty),\n    },",
    "new": "[TRAINER_LEADER_MISTY] = {\n        .trainerClass = TRAINER_CLASS_LEADER,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_FEMALE | F_TRAINER_FEMALE,\n        .trainerPic = TRAINER_PIC_LEADER_MISTY,\n        .trainerName = _(\"MISTY\"),\n        .items = {ITEM_SUPER_POTION},\n        .doubleBattle = TRUE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY,\n        .party = ITEM_CUSTOM_MOVES(sParty_LeaderMisty),\n    },",
    "label": "Misty trainer"
  },
  {
    "path": "data/maps/CeruleanCity_Gym/scripts.inc",
    "old": "\ttrainerbattle_single TRAINER_LEADER_MISTY, CeruleanCity_Gym_Text_MistyIntro, CeruleanCity_Gym_Text_MistyDefeat, CeruleanCity_Gym_EventScript_MistyDefeated, NO_MUSIC",
    "new": "\ttrainerbattle_double TRAINER_LEADER_MISTY, CeruleanCity_Gym_Text_MistyIntro, CeruleanCity_Gym_Text_MistyDefeat, SaffronCity_Dojo_Text_NeedTwo, CeruleanCity_Gym_EventScript_MistyDefeated, NO_MUSIC",
    "label": "Misty encounter"
  },
  {
    "path": "src/battle_util.c",
    "old": "        if (gTrainerBattleOpponent_A == TRAINER_LEADER_MISTY && species == SPECIES_STARMIE)\n",
    "new": "        if (gTrainerBattleOpponent_A == TRAINER_LEADER_MISTY)\n",
    "label": "Misty scoped ability"
  }
]

def prior_misty_gym_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    for item in REVIEWED:
        if item["path"] != str(path):
            continue
        after, before = item["new"], item["old"]
        if text.count(after) == 1:
            text = text.replace(after, before, 1)
        else:
            assert text.count(before) == 1, item["label"] + ": unapproved modification"
    return text
