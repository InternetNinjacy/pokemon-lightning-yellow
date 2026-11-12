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
    from approved_viridian_cole_changes import prior_viridian_cole_source
    from approved_viridian_atsushi_changes import prior_viridian_atsushi_source
    from approved_viridian_takashi_changes import prior_viridian_takashi_source
    from approved_viridian_kiyo_changes import prior_viridian_kiyo_source
    from approved_viridian_jason_changes import prior_viridian_jason_source
    from approved_viridian_samuel_changes import prior_viridian_samuel_source
    from approved_viridian_yuji_changes import prior_viridian_yuji_source
    result = prior_viridian_cole_source(path, prior_viridian_atsushi_source(path, prior_viridian_takashi_source(path, prior_viridian_yuji_source(path, prior_viridian_samuel_source(path, prior_viridian_jason_source(path, prior_viridian_kiyo_source(path, prior_brock_weather_source(path))))))))
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
