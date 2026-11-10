#!/usr/bin/env python3
"""BTL-005 Batch A focused *staging* contract: no runtime acceptance claim."""
from pathlib import Path
import json,re

R=Path(__file__).resolve().parents[1]
spec={m["proposed_id"]:m for m in json.loads((R/"docs/moves/approved-design-overlays.json").read_text())["new_moves"]}
anim={m["key"]:m for m in json.loads((R/"docs/moves/animation-reuse.json").read_text())["entries"]}
ids=(R/"include/constants/moves.h").read_text()
data=(R/"src/data/battle_moves.h").read_text()
names=(R/"src/data/text/move_names.h").read_text()
desc=(R/"src/move_descriptions.c").read_text()
art=(R/"data/battle_anim_scripts.s").read_text()
cases={
391:("EFFECT_HIT","MOVE_TARGET_SELECTED",False),
392:("EFFECT_HIT","MOVE_TARGET_FOES_AND_ALLY",False),
394:("EFFECT_ALWAYS_HIT","MOVE_TARGET_SELECTED",False),
395:("EFFECT_SPECIAL_DEFENSE_DOWN_HIT","MOVE_TARGET_SELECTED",False),
397:("EFFECT_SPECIAL_DEFENSE_DOWN_HIT","MOVE_TARGET_SELECTED",False),
398:("EFFECT_ACCURACY_DOWN_HIT","MOVE_TARGET_SELECTED",False),
399:("EFFECT_FLINCH_HIT","MOVE_TARGET_SELECTED",True),
400:("EFFECT_HIT","MOVE_TARGET_SELECTED",True),
401:("EFFECT_FLINCH_HIT","MOVE_TARGET_SELECTED",False),
406:("EFFECT_FLINCH_HIT","MOVE_TARGET_SELECTED",True),
}
assert re.search(r"^#define MOVES_COUNT 407$",ids,re.M)
table=art.split("gBattleAnims_Moves::",1)[1].split(".4byte Move_COUNT @",1)[0]
entries=re.findall(r"^\s*\.4byte Move_(\w+)[^\n]*$",table,re.M)
assert len(entries)==407, len(entries)
for number,(effect,target,contact) in cases.items():
 row=spec[number];key=row["key"];approved=row["approved"]
 assert re.search(r"^#define MOVE_"+key+r" "+str(number)+r"$",ids,re.M)
 assert entries[number]==key,(number,entries[number],key)
 assert "Move_"+key+":" in art
 assert "[MOVE_"+key+"]" in names
 assert "gMoveDescription_"+key+"[]" in desc
 assert "[MOVE_"+key+" - 1] = gMoveDescription_"+key in desc
 block=re.search(r"\[MOVE_"+key+r"\]\s*=\s*\{(.*?)\n    \},",data,re.S)
 assert block,key
 t=block.group(1)
 expectations={
 "effect":effect,"target":target,"power":str(approved["power"]),
 "accuracy":str(approved["accuracy"]),"pp":str(approved["pp"]),
 "priority":str(approved["priority"]),
 "secondaryEffectChance":str(approved["secondaryEffectChance"]),
 "category":"MOVE_CATEGORY_"+approved["category"],"type":"TYPE_"+approved["type"],
 }
 for field,value in expectations.items():
  assert re.search(r"\."+field+r"\s*=\s*"+re.escape(value)+r"\s*,",t),(key,field,value)
 assert ("FLAG_MAKES_CONTACT" in t)==contact,key
 assert anim[key]["approval_label"].startswith("Owner approved"),key
# The gaps belong to future approved batches; never represent them as accepted.
for number in range(387,407):
 if number not in cases:
  assert entries[number]=="NONE",number
  assert "[MOVE_"+spec[number]["key"]+"]" not in data,number
assert "gCurrentMove == MOVE_ROLLOUT" in (R/"src/battle_script_commands.c").read_text()
assert spec[386]["effect_rules"]["defense_curl_bonus"] is False
print("PASS: Batch A ten approved indexed data/name/description/animation candidates; original Rollout-only Curl preserved. STAGING ONLY: reserved gaps, effects and composites require native acceptance.")
