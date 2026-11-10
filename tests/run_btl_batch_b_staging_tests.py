#!/usr/bin/env python3
"""Batch B ten-move source contract. Staging checks are not native acceptance."""
import json,re
from pathlib import Path
R=Path(__file__).resolve().parents[1]
spec={m["proposed_id"]:m for m in json.loads((R/"docs/moves/approved-design-overlays.json").read_text())["new_moves"]}
approved_anim={m["key"]:m for m in json.loads((R/"docs/moves/animation-reuse.json").read_text())["entries"]}
src=lambda path:(R/path).read_text()
ids=src("include/constants/moves.h")
data=src("src/data/battle_moves.h")
names=src("src/data/text/move_names.h")
descs=src("src/move_descriptions.c")
art=src("data/battle_anim_scripts.s")
scripts=src("data/battle_scripts_1.s")
cases={
387:("EFFECT_PARALYZE_HIT","MOVE_TARGET_FOES_AND_ALLY",False),
389:("EFFECT_ICE_FANG","MOVE_TARGET_SELECTED",True),
390:("EFFECT_RECOIL","MOVE_TARGET_SELECTED",True),
393:("EFFECT_DOUBLE_EDGE","MOVE_TARGET_SELECTED",True),
396:("EFFECT_ABSORB","MOVE_TARGET_SELECTED",True),
403:("EFFECT_DOUBLE_EDGE","MOVE_TARGET_SELECTED",True),
407:("EFFECT_FLINCH_HIT","MOVE_TARGET_SELECTED",True),
408:("EFFECT_POISON_TAIL","MOVE_TARGET_SELECTED",True),
409:("EFFECT_POISON_HIT","MOVE_TARGET_SELECTED",True),
410:("EFFECT_POISON_HIT","MOVE_TARGET_SELECTED",False),
}
assert re.search(r"^#define MOVES_COUNT 411$",ids,re.M)
registry=art.split("gBattleAnims_Moves::",1)[1].split(".4byte Move_COUNT @",1)[0]
positions=re.findall(r"^\s*\.4byte Move_(\w+)[^\n]*$",registry,re.M)
assert len(positions)==411,len(positions)
for n,(effect,target,contact) in cases.items():
 m=spec[n];k=m["key"];p=m["approved"]
 assert re.search(r"^#define MOVE_"+k+" "+str(n)+r"$",ids,re.M)
 assert positions[n]==k,(n,positions[n],k)
 assert "Move_"+k+":" in art
 assert "[MOVE_"+k+"]" in names
 assert "gMoveDescription_"+k+"[]" in descs
 assert "[MOVE_"+k+" - 1] = gMoveDescription_"+k in descs
 record=re.search(r"\[MOVE_"+k+r"\]\s*=\s*\{(.*?)\n    \},",data,re.S)
 assert record,k
 text=record.group(1)
 checks={"effect":effect,"target":target,"category":"MOVE_CATEGORY_"+p["category"],
         "type":"TYPE_"+p["type"],"power":str(p["power"]),"accuracy":str(p["accuracy"]),
         "pp":str(p["pp"]),"priority":str(p["priority"])}
 if p["secondaryEffectChance"] is not None:
  checks["secondaryEffectChance"]=str(p["secondaryEffectChance"])
 for field,value in checks.items():
  assert re.search(r"\."+field+r"\s*=\s*"+re.escape(value)+r"\s*,",text),(k,field,value)
 assert ("FLAG_MAKES_CONTACT" in text)==contact,k
 assert approved_anim[k]["approval_label"].startswith("Owner approved"),k
# Thunder Fang retains both independent ten-percent effect rolls. Its primary
# effect is paralysis rather than inherited Ice Fang freezing.
assert "jumpifmove MOVE_THUNDER_FANG, BattleScript_ThunderFangParalysis" in scripts
assert "setmoveeffect MOVE_EFFECT_PARALYSIS" in scripts.split("BattleScript_ThunderFangParalysis:",1)[1].split("\n\n",1)[0]
assert "jumpifmove MOVE_THUNDER_FANG, BattleScript_IceFangFlinch" in scripts
# The remaining C IDs cannot be mistakenly presented as accepted moves.
for n in (388,402,404,405):
 assert positions[n]=="NONE"
 assert "[MOVE_"+spec[n]["key"]+"]" not in data
assert "gCurrentMove == MOVE_ROLLOUT" in src("src/battle_script_commands.c")
assert spec[386]["effect_rules"]["defense_curl_bonus"] is False
print("PASS: ten Batch B approved indexed production records, names, art and separate Thunder Fang rolls; Batch C remains reserved. STAGING, not native acceptance.")
