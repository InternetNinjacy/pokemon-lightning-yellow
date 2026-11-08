#!/usr/bin/env python3
"""BTL-005 #367-369 strict source contract; native GBA execution is separate."""
import json
from pathlib import Path
from approved_bellow_rock_changes import prior_bellow_rock_source
ROOT=Path(__file__).resolve().parents[1]
const=(ROOT/"include/constants/moves.h").read_text()
data=(ROOT/"src/data/battle_moves.h").read_text()
names=(ROOT/"src/data/text/move_names.h").read_text()
desc=(ROOT/"src/move_descriptions.c").read_text()
anim=(ROOT/"data/battle_anim_scripts.s").read_text()
rows={r["key"]:r for r in json.loads((ROOT/"docs/moves/move-specification.json").read_text())["moves"]}
approved=[
 ("BELLOW",367,"NORMAL","SPECIAL",40,100,30,"EFFECT_HIT","UPROAR"),
 ("POWER_GEM",368,"ROCK","SPECIAL",80,100,20,"EFFECT_HIT","ANCIENT_POWER"),
 ("STONE_EDGE",369,"ROCK","PHYSICAL",100,80,10,"EFFECT_HIGH_CRITICAL","ROCK_SLIDE")]
for key,number,kind,category,power,accuracy,pp,effect,visual in approved:
 assert f"#define MOVE_{key} {number}" in const
 assert f"[MOVE_{key}] = _(" in names
 assert f"gMoveDescription_{key}[]" in desc and f"[MOVE_{key} - 1] = gMoveDescription_{key}" in desc
 assert f"Move_{key}:\n\tgoto Move_{visual}" in anim
 start=data.index(f"[MOVE_{key}] =")
 block=data[start:data.index("    },",start)]
 for field,val in [("category",f"MOVE_CATEGORY_{category}"),("type",f"TYPE_{kind}"),("power",power),("accuracy",accuracy),("pp",pp),("effect",effect),("target","MOVE_TARGET_SELECTED"),("priority",0),("secondaryEffectChance",0)]:
  assert f".{field} = {val}," in block,(key,field)
 assert "FLAG_MAKES_CONTACT" not in block,key
 r=rows[key]
 # Catalog must reflect the compiled ID and mechanics; don't silently stamp an unverified runtime state.
 assert r["proposed_id"]==number and r["approved"]["type"]==kind
 for f in ["src/data/pokemon/level_up_learnsets.h","src/data/pokemon/tmhm_learnsets.h","src/data/trainer_parties.h"]:
  assert f"MOVE_{key}" not in (ROOT/f).read_text(),(key,f)
sound=(ROOT/"src/battle_util.c").read_text()
table=sound[sound.index("static const u16 sSoundMovesTable[]"):sound.index("};",sound.index("static const u16 sSoundMovesTable[]"))]
assert table.count("MOVE_BELLOW")==1
ai=(ROOT/"data/battle_ai_scripts.s").read_text()
block=ai[ai.index("AI_CheckBadMove_CheckSoundproof::"):ai.index("AI_CheckBadMove_CheckEffect::")]
assert "if_move MOVE_BELLOW, Score_Minus10" in block
assert "#define MOVES_COUNT 373" in const  # Superseded by new approved move #370; #367-369 unchanged
manifest=json.loads((ROOT/"tests/specifications/approved_bellow_rock_delta.json").read_text())
for path in manifest["files"]:
 current=(ROOT/path).read_text()
 assert prior_bellow_rock_source(path,current)!=current,path
print("PASS: Bellow/Power Gem/Stone Edge #367-369 exact data, sound/AI, reuse-only animations, no distributions and strict reverse source manifest")
