#!/usr/bin/env python3
"""BTL-005 Batch A focused *staging* contract: no runtime acceptance claim."""
from pathlib import Path
import json,re

R=Path(__file__).resolve().parents[1]
spec={m["proposed_id"]:m for m in json.loads((R/"docs/moves/approved-design-overlays.json").read_text())["new_moves"]}
anim={m["key"]:m for m in json.loads((R/"docs/moves/animation-reuse.json").read_text())["entries"]}
# Batch B invokes this contract against the exact reversible Batch A source
# view, while its own ten production entries are checked separately.
def staged_a_source(path):
 text=(R/path).read_text()
 if (R/"tests/approved_batch_b_changes.py").exists():
  from approved_batch_b_changes import prior_batch_b_source
  return prior_batch_b_source(str(path),text)
 return text
ids=staged_a_source("include/constants/moves.h")
data=staged_a_source("src/data/battle_moves.h")
names=staged_a_source("src/data/text/move_names.h")
desc=staged_a_source("src/move_descriptions.c")
art=staged_a_source("data/battle_anim_scripts.s")
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
  # The array must have a valid category for every allocated slot. These
  # numeric-only, zero-PP Splash records cannot masquerade as actual moves.
  reserved=re.search(r"\["+str(number)+r"\]\s*=\s*\{(.*?)\n    \},",data,re.S)
  assert reserved,number
  raw=reserved.group(1)
  for exact in (".category = MOVE_CATEGORY_STATUS,",".effect = EFFECT_SPLASH,",
                ".power = 0,", ".accuracy = 0,", ".pp = 0,", ".flags = 0,"):
   assert exact in raw,(number,exact)
assert "gCurrentMove == MOVE_ROLLOUT" in (R/"src/battle_script_commands.c").read_text()
assert spec[386]["effect_rules"]["defense_curl_bonus"] is False
print("PASS: Batch A ten approved indexed data/name/description/animation candidates; original Rollout-only Curl preserved. STAGING ONLY: reserved gaps, effects and composites require native acceptance.")
