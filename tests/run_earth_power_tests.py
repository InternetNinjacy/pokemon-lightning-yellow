#!/usr/bin/env python3
"""Earth Power #371 source contract; native Singles/Doubles verified separately."""
import json
from pathlib import Path
from approved_avalanche_changes import prior_avalanche_source
from approved_earth_power_changes import prior_earth_power_source
ROOT=Path(__file__).resolve().parents[1]
read=lambda p:(ROOT/p).read_text()
const=prior_avalanche_source("include/constants/moves.h",read("include/constants/moves.h"))
data=read("src/data/battle_moves.h")
names=read("src/data/text/move_names.h")
desc=read("src/move_descriptions.c")
anim=read("data/battle_anim_scripts.s")
assert "#define MOVE_EARTH_POWER 371" in const and "#define MOVES_COUNT 373" in const
assert '[MOVE_EARTH_POWER] = _("EARTH POWER")' in names
assert "gMoveDescription_EARTH_POWER[]" in desc and "[MOVE_EARTH_POWER - 1] = gMoveDescription_EARTH_POWER" in desc
start=data.index("[MOVE_EARTH_POWER] =")
block=data[start:data.index("    },",start)]
for field,val in [("category","MOVE_CATEGORY_SPECIAL"),("effect","EFFECT_SPECIAL_DEFENSE_DOWN_HIT"),
 ("type","TYPE_GROUND"),("power",90),("accuracy",100),("pp",10),
 ("secondaryEffectChance",10),("target","MOVE_TARGET_SELECTED"),("priority",0)]:
 assert f".{field} = {val}," in block,(field,val)
assert "FLAG_MAKES_CONTACT" not in block
assert "FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED" in block
assert anim.count(".4byte Move_EARTH_POWER")==1
assert anim.count("Move_EARTH_POWER:\n\tgoto Move_EARTHQUAKE")==1
for p in ["src/data/pokemon/level_up_learnsets.h","src/data/pokemon/tmhm_learnsets.h","src/data/trainer_parties.h"]:
 assert "MOVE_EARTH_POWER" not in read(p),p
for p in ["include/constants/moves.h","src/data/text/move_names.h","src/move_descriptions.c","src/data/battle_moves.h","data/battle_anim_scripts.s"]:
 assert "EARTH_POWER" not in prior_earth_power_source(p,read(p)),p
catalog=json.loads(read("docs/moves/move-specification.json"))
row=next(r for r in catalog["moves"] if r["key"]=="EARTH_POWER")
assert row["id"]==371 and row["proposed_id"]==371 and row["approved"]["power"]==90
assert row["approved"]["effect"]=="SP_DEFENSE_MINUS_1_10"
assert row["implementation_status"]=="SOURCE_DATA_PRESENT_RUNTIME_UNVERIFIED"
print("PASS: Earth Power #371 approved data, exact strict reverse source, native artwork, and no distribution; mGBA separately gated")
