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
    from approved_gym002_changes import prior_gym002_source
    from approved_gym001_luis_changes import prior_luis_source
    from approved_gym002_koga_changes import prior_koga_gym002_source
    from approved_gym002_erika_changes import prior_erika_gym002_source
    from approved_gym001_tucker_changes import prior_gym001_tucker_source
    from approved_gym001_baily_changes import prior_gym001_baily_source
    from approved_fuchsia_three_changes import prior_fuchsia_three_source
    from approved_celadon_four_changes import prior_celadon_four_source
    from approved_viridian_cole_changes import prior_viridian_cole_source
    from approved_viridian_atsushi_changes import prior_viridian_atsushi_source
    from approved_viridian_takashi_changes import prior_viridian_takashi_source
    from approved_viridian_kiyo_changes import prior_viridian_kiyo_source
    from approved_viridian_jason_changes import prior_viridian_jason_source
    from approved_viridian_samuel_changes import prior_viridian_samuel_source
    from approved_viridian_yuji_changes import prior_viridian_yuji_source
    from approved_viridian_warren_changes import prior_viridian_warren_source
    from approved_viridian_bags_changes import prior_viridian_bags_source
    result = prior_erika_gym002_source(path, prior_koga_gym002_source(path, prior_celadon_four_source(path, prior_fuchsia_three_source(path, prior_gym001_baily_source(path, prior_gym001_tucker_source(path, prior_viridian_bags_source(path, prior_viridian_cole_source(path, prior_viridian_atsushi_source(path, prior_viridian_takashi_source(path, prior_viridian_warren_source(path, prior_viridian_yuji_source(path, prior_viridian_samuel_source(path, prior_viridian_jason_source(path, prior_viridian_kiyo_source(path, prior_brock_weather_source(path, prior_gym002_source(path, prior_luis_source(path))))))))))))))))))
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
    # Strictly reverse two already-merged Gym Leader party changes omitted
    # from earlier historical projections. Exact approved source snapshots are
    # required; any later party drift fails rather than being silently erased.
    if str(path) == "src/data/trainer_parties.h":
        approved_leaders = {
            "sParty_LeaderLtSurge": ("static const struct TrainerMonNoItemCustomMoves sParty_LeaderLtSurge[] = {\n    {\n        .iv = 0,\n        .lvl = 30,\n        .species = SPECIES_RAICHU,\n        .moves = {MOVE_ZAP_CANNON, MOVE_QUICK_ATTACK, MOVE_BRICK_BREAK, MOVE_AGILITY},\n    },\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_VOLTORB,\n        .moves = {MOVE_THUNDER_WAVE, MOVE_SCREECH, MOVE_NONE, MOVE_NONE},\n    },\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_POLIWAG,\n        .moves = {MOVE_RAIN_DANCE, MOVE_SCARY_FACE, MOVE_NONE, MOVE_NONE},\n    },\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_ABRA,\n        .moves = {MOVE_FAKE_TEARS, MOVE_HELPING_HAND, MOVE_NONE, MOVE_NONE},\n    },\n};", "static const struct TrainerMonNoItemCustomMoves sParty_LeaderLtSurge[] = {\n    {\n        .iv = 0,\n        .lvl = 21,\n        .species = SPECIES_VOLTORB,\n        .moves = {MOVE_SONIC_BOOM, MOVE_TACKLE, MOVE_SCREECH, MOVE_SHOCK_WAVE},\n    },\n    {\n        .iv = 0,\n        .lvl = 18,\n        .species = SPECIES_PIKACHU,\n        .moves = {MOVE_QUICK_ATTACK, MOVE_THUNDER_WAVE, MOVE_DOUBLE_TEAM, MOVE_SHOCK_WAVE},\n    },\n    {\n        .iv = 0,\n        .lvl = 24,\n        .species = SPECIES_RAICHU,\n        .moves = {MOVE_QUICK_ATTACK, MOVE_THUNDER_WAVE, MOVE_DOUBLE_TEAM, MOVE_SHOCK_WAVE},\n    },\n};"),
            "sParty_LeaderSabrina": ("static const struct TrainerMonNoItemCustomMoves sParty_LeaderSabrina[] = {\n    {\n        .iv = 0,\n        .lvl = 43,\n        .species = SPECIES_GENGAR,\n        .moves = {MOVE_HYPNOSIS, MOVE_DREAM_EATER, MOVE_SHADOW_PUNCH, MOVE_DESTINY_BOND},\n    },\n    {\n        .iv = 0,\n        .lvl = 38,\n        .species = SPECIES_MACHAMP,\n        .moves = {MOVE_VITAL_THROW, MOVE_REVENGE, MOVE_FORESIGHT, MOVE_FOCUS_ENERGY},\n    },\n    {\n        .iv = 0,\n        .lvl = 38,\n        .species = SPECIES_DUSCLOPS,\n        .moves = {MOVE_SHADOW_PUNCH, MOVE_CONFUSE_RAY, MOVE_CURSE, MOVE_NIGHT_SHADE},\n    },\n    {\n        .iv = 0,\n        .lvl = 37,\n        .species = SPECIES_HITMONTOP,\n        .moves = {MOVE_TRIPLE_KICK, MOVE_COUNTER, MOVE_QUICK_ATTACK, MOVE_AGILITY},\n    },\n};", "static const struct TrainerMonNoItemCustomMoves sParty_LeaderSabrina[] = {\n    {\n        .iv = 0,\n        .lvl = 38,\n        .species = SPECIES_KADABRA,\n        .moves = {MOVE_PSYBEAM, MOVE_REFLECT, MOVE_FUTURE_SIGHT, MOVE_CALM_MIND},\n    },\n    {\n        .iv = 0,\n        .lvl = 37,\n        .species = SPECIES_MR_MIME,\n        .moves = {MOVE_BARRIER, MOVE_PSYBEAM, MOVE_BATON_PASS, MOVE_CALM_MIND},\n    },\n    {\n        .iv = 0,\n        .lvl = 38,\n        .species = SPECIES_VENOMOTH,\n        .moves = {MOVE_PSYBEAM, MOVE_GUST, MOVE_LEECH_LIFE, MOVE_SUPERSONIC},\n    },\n    {\n        .iv = 0,\n        .lvl = 43,\n        .species = SPECIES_ALAKAZAM,\n        .moves = {MOVE_PSYCHIC, MOVE_RECOVER, MOVE_FUTURE_SIGHT, MOVE_CALM_MIND},\n    },\n};"),
        }
        for symbol, (approved, historical) in approved_leaders.items():
            assert result.count(approved) == 1, symbol + ": approved party drift"
            assert historical not in result, symbol + ": historical party duplicated"
            result = result.replace(approved, historical, 1)
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
