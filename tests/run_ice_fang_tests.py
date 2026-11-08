#!/usr/bin/env python3
"""Ice Fang #372 source contract; actual frozen/flinched outcomes verified in native tests."""
import json
from pathlib import Path
from approved_avalanche_changes import prior_avalanche_source
from approved_ice_fang_changes import prior_ice_fang_source
ROOT=Path(__file__).resolve().parents[1]
read=lambda path:(ROOT/path).read_text()
c=prior_avalanche_source("include/constants/moves.h",read("include/constants/moves.h")); d=read("src/data/battle_moves.h"); a=read("data/battle_anim_scripts.s")
assert "#define MOVE_ICE_FANG 372" in c and "#define MOVES_COUNT 373" in c
start=d.index("[MOVE_ICE_FANG] ="); block=d[start:d.index("    },",start)]
for field,value in [("category","MOVE_CATEGORY_PHYSICAL"),("effect","EFFECT_ICE_FANG"),("power",65),("type","TYPE_ICE"),("accuracy",95),("pp",15),("secondaryEffectChance",10),("target","MOVE_TARGET_SELECTED"),("priority",0)]:
 assert f".{field} = {value}," in block,(field,value)
assert "FLAG_MAKES_CONTACT" in block and "FLAG_PROTECT_AFFECTED" in block
assert '    [MOVE_ICE_FANG] = _("ICE FANG")' in read("src/data/text/move_names.h")
assert "gMoveDescription_ICE_FANG[]" in read("src/move_descriptions.c")
assert "[MOVE_ICE_FANG - 1] = gMoveDescription_ICE_FANG" in read("src/move_descriptions.c")
scripts=read("data/battle_scripts_1.s")
assert "#define EFFECT_ICE_FANG 216" in read("include/constants/battle_move_effects.h")
assert "BattleScript_EffectIceFang::\n\tsetmoveeffect MOVE_EFFECT_FREEZE" in scripts
assert "\tgoto BattleScript_AfterSwarmCloud\nBattleScript_IceFangFlinch::\n\tsetmoveeffect MOVE_EFFECT_FLINCH\n\tseteffectwithchance" in scripts
assert scripts.index("BattleScript_IceFangFlinch::") > scripts.index("BattleScript_AfterSwarmCloud::")-200
assert a.count(".4byte Move_ICE_FANG")==1 and a.count("Move_ICE_FANG:")==1
visual=a[a.index("Move_ICE_FANG:"):]
for v in ["ANIM_TAG_SHARP_TEETH","gSharpTeethSpriteTemplate","ANIM_TAG_ICE_CRYSTALS","gIceCrystalSpiralInwardSmall","IceCrystalEffectShort"]:
 assert v in visual,v
assert visual.strip().endswith("end")
for path in ["src/data/pokemon/level_up_learnsets.h","src/data/pokemon/tmhm_learnsets.h","src/data/trainer_parties.h"]:
 assert "MOVE_ICE_FANG" not in read(path)
for path in ["include/constants/moves.h","src/data/text/move_names.h","src/move_descriptions.c","src/data/battle_moves.h","data/battle_anim_scripts.s"]:
 assert "ICE_FANG" not in prior_ice_fang_source(path,read(path))
row=next(x for x in json.loads(read("docs/moves/move-specification.json"))["moves"] if x["key"]=="ICE_FANG")
assert row["id"]==372 and row["proposed_id"]==372 and row["implementation_status"]=="SOURCE_DATA_PRESENT_RUNTIME_UNVERIFIED"
assert row["approved"]["effect"]=="INDEPENDENT_FREEZE_10_FLINCH_10"
print("PASS: Ice Fang #372 independent effect source, native art composition, strict reverse and no distribution")
