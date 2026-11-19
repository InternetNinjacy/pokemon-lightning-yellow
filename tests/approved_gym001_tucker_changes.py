"""Byte-exact approved Vermilion Tucker source projection for frozen historical tests."""
from pathlib import Path
from approved_historical_reconstruction import reverse_pinned_records
REVIEWED = [
  {
    "path": "src/data/trainer_parties.h",
    "before": "static const struct TrainerMonNoItemDefaultMoves sParty_GentlemanTucker[] = {\n    {\n        .iv = 0,\n        .lvl = 23,\n        .species = SPECIES_PIKACHU,\n    },\n};",
    "after": "static const struct TrainerMonNoItemCustomMoves sParty_GentlemanTucker[] = {\n    {\n        .iv = 0,\n        .lvl = 22,\n        .species = SPECIES_VOLTORB,\n        .moves = {MOVE_CHARGE, MOVE_SPARK, MOVE_NONE, MOVE_NONE},\n    },\n};"
  },
  {
    "path": "src/data/trainers.h",
    "before": "[TRAINER_GENTLEMAN_TUCKER] = {\n        .trainerClass = TRAINER_CLASS_GENTLEMAN,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_RICH,\n        .trainerPic = TRAINER_PIC_GENTLEMAN,\n        .trainerName = _(\"TUCKER\"),\n        .items = {},\n        .doubleBattle = FALSE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE,\n        .party = NO_ITEM_DEFAULT_MOVES(sParty_GentlemanTucker),\n    },",
    "after": "[TRAINER_GENTLEMAN_TUCKER] = {\n        .trainerClass = TRAINER_CLASS_GENTLEMAN,\n        .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_RICH,\n        .trainerPic = TRAINER_PIC_GENTLEMAN,\n        .trainerName = _(\"TUCKER\"),\n        .items = {},\n        .doubleBattle = FALSE,\n        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE,\n        .party = NO_ITEM_CUSTOM_MOVES(sParty_GentlemanTucker),\n    },"
  }
]
def prior_gym001_tucker_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    for record in REVIEWED:
        if record["path"] != str(path):
            continue
        before, after = record["before"], record["after"]
        if text.count(after) == 1:
            # Preserve the original accepted/current-or-historical contract,
            # while enforcing pinned, entire-record equivalence for new data.
            identity, kind = (
                ("sParty_GentlemanTucker", "trainer_party")
                if str(path) == "src/data/trainer_parties.h"
                else ("GENTLEMAN_TUCKER", "trainer_metadata")
            )
            text = reverse_pinned_records(
                text,
                path=str(path),
                approved_commit="02240690581eb70d70219024fb7f55da2882308b",
                historical_commit="21a3630b3d51481ab10a1af9637fa1ced73722cc",
                identities=(identity,),
                kind=kind,
            )
        else:
            assert text.count(before) == 1, "Unreviewed Tucker source modification: " + str(path)
    return text
