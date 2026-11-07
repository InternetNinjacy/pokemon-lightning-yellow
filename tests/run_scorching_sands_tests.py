#!/usr/bin/env python3
"""BTL-005 #370 exact source/approval audit; real battle verification runs in mGBA."""
import json
from pathlib import Path
from approved_scorching_sands_changes import prior_scorching_sands_source

root = Path(__file__).resolve().parents[1]
read = lambda path: (root/path).read_text()
constant = read("include/constants/moves.h")
moves = read("src/data/battle_moves.h")
names = read("src/data/text/move_names.h")
descriptions = read("src/move_descriptions.c")
animations = read("data/battle_anim_scripts.s")
catalog = json.loads(read("docs/moves/move-specification.json"))
overlays = json.loads(read("docs/moves/approved-design-overlays.json"))
reuse = json.loads(read("docs/moves/animation-reuse.json"))

assert "#define MOVE_SCORCHING_SANDS 370" in constant
assert "#define MOVES_COUNT 371" in constant
assert '[MOVE_SCORCHING_SANDS] = _("SCORCHING SANDS")' in names
assert "gMoveDescription_SCORCHING_SANDS[]" in descriptions
assert "[MOVE_SCORCHING_SANDS - 1] = gMoveDescription_SCORCHING_SANDS" in descriptions
start = moves.index("[MOVE_SCORCHING_SANDS] =")
block = moves[start:moves.index("    },", start)]
for field, val in [
    ("category", "MOVE_CATEGORY_SPECIAL"),
    ("effect", "EFFECT_BURN_HIT"),
    ("power", 70), ("type", "TYPE_GROUND"),
    ("accuracy", 100), ("pp", 10),
    ("secondaryEffectChance", 30),
    ("target", "MOVE_TARGET_SELECTED"), ("priority", 0),
]:
    assert f".{field} = {val}," in block, field
assert "FLAG_MAKES_CONTACT" not in block
assert "FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED" in block
assert animations.count(".4byte Move_SCORCHING_SANDS") == 1
assert animations.count("Move_SCORCHING_SANDS:") == 1
script = animations[animations.index("Move_SCORCHING_SANDS:"):]
for fragment in ["ANIM_TAG_FLYING_DIRT", "ANIM_TAG_SMALL_EMBER",
                 "gFlyingSandCrescentSpriteTemplate", "gEmberSpriteTemplate",
                 "call EmberFireHit", "waitforvisualfinish", "\tend"]:
    assert fragment in script, fragment
assert "createvisualtask AnimTask_LoadSandstormBackground" not in script
assert "setweather" not in script.lower()
for path in ["src/data/pokemon/level_up_learnsets.h",
             "src/data/pokemon/tmhm_learnsets.h",
             "src/data/trainer_parties.h"]:
    assert "MOVE_SCORCHING_SANDS" not in read(path), path

for path in ["include/constants/moves.h", "src/data/text/move_names.h",
             "src/move_descriptions.c", "src/data/battle_moves.h",
             "data/battle_anim_scripts.s"]:
    content = read(path)
    old = prior_scorching_sands_source(path, content)
    assert old != content and "SCORCHING_SANDS" not in old, path

def find_row(obj):
    rows = obj["moves"] if isinstance(obj, dict) else obj
    return next(r for r in rows if r.get("key") == "SCORCHING_SANDS")

for document in [catalog, overlays]:
    row = find_row(document)
    assert row["proposed_id"] == 370 and row["approved"]["power"] == 70
    assert row["approved"]["secondaryEffectChance"] == 30
visual = find_row(reuse)
assert visual["candidate"] == "SANDSTORM + EMBER"
assert visual["approval_label"].startswith("Owner approved for implementation/playback testing")
print("PASS: Scorching Sands #370 exact design, data/text, source reverse, existing-art composite, no distribution; native runtime separately gated")
