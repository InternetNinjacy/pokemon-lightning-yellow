#!/usr/bin/env python3
"""Exact Swarm Cloud native-script contract; actual independent rolls are native-tested."""
from pathlib import Path
import json
import re
from approved_swarm_cloud_changes import prior_swarm_cloud_source

root = Path(__file__).resolve().parents[1]
rows = json.loads((root/"docs/moves/move-specification.json").read_text())["moves"]
row = next(r for r in rows if r["key"] == "SWARM_CLOUD")
assert row["id"] == 366 and row["delta"] == {}
c = row["current"]
assert (c["type"],c["category"],c["power"],c["accuracy"],c["pp"],
        c["priority"],c["secondaryEffectChance"],c["target"],c["effect"]) == (
        "BUG","SPECIAL",90,90,15,0,50,"MOVE_TARGET_SELECTED","EFFECT_SWARM_CLOUD")
assert "FLAG_MAKES_CONTACT" not in c["flags"]
effects = (root/"include/constants/battle_move_effects.h").read_text()
assert "#define EFFECT_SWARM_CLOUD 215" in effects
battle = (root/"data/battle_scripts_1.s").read_text()
table = battle.split("gBattleScriptsForMoveEffects::",1)[1].split("\nBattleScript_EffectHit::",1)[0]
pointers = re.findall(r'\.4byte (BattleScript_\w+)',table)
assert len(pointers)==216 and pointers[214]=="BattleScript_EffectSleepHit" and pointers[215]=="BattleScript_EffectSwarmCloud"
assert re.search(r'BattleScript_EffectSwarmCloud::\n\tsetmoveeffect MOVE_EFFECT_CONFUSION\n\tgoto BattleScript_EffectHit',battle)
assert re.search(r'seteffectwithchance\n\tjumpifnotmove MOVE_SWARM_CLOUD, BattleScript_AfterSwarmCloud\n\tsetmoveeffect MOVE_EFFECT_POISON\n\tseteffectwithchance\nBattleScript_AfterSwarmCloud::\n\ttryfaintmon BS_TARGET',battle)
# Both independent rolls inherit the same 50% source chance; statuses have
# distinct native immunity checks (Own Tempo, Safeguard, Poison/Steel, etc.).
asm = (root/"data/battle_anim_scripts.s").read_text()
assert "Move_SWARM_CLOUD:" in asm and "SwarmCloudPoisonVisual:" in asm
assert "\tjumprettrue SwarmCloudOnPlayer" in asm
assert "\tgoto SwarmCloudPoisonVisual" in asm
assert "gPoisonPowderParticleSpriteTemplate" in asm.split("SwarmCloudPoisonVisual:",1)[1]
for path in json.loads((root/"tests/specifications/approved_swarm_cloud_delta.json").read_text())["files"]:
    assert prior_swarm_cloud_source(path) != (root/path).read_text()
for path in ["src/data/pokemon/level_up_learnsets.h","src/data/pokemon/tmhm_learnsets.h","src/data/trainer_parties.h"]:
    assert "MOVE_SWARM_CLOUD" not in (root/path).read_text()
print("PASS: Swarm Cloud #366 source effect215, exact two separately gated native50% checks, approved existing-art composition and strict preservation")
