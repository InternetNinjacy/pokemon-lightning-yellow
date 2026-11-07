#!/usr/bin/env python3
"""Chirp #365: exact sound attack, Soundproof gameplay and AI source contract."""
from pathlib import Path
import json
from approved_chirp_changes import prior_chirp_source

root = Path(__file__).resolve().parents[1]
row = next(r for r in json.loads((root/"docs/moves/move-specification.json").read_text())["moves"] if r["key"] == "CHIRP")
assert row["id"] == 365 and row["delta"] == {}
cur = row["current"]
assert (cur["type"],cur["category"],cur["effect"],cur["power"],cur["accuracy"],
        cur["pp"],cur["secondaryEffectChance"],cur["priority"],cur["target"]) == (
        "BUG","SPECIAL","EFFECT_HIT",40,100,25,0,0,"MOVE_TARGET_SELECTED")
assert "FLAG_MAKES_CONTACT" not in cur["flags"]
battle = (root/"src/battle_util.c").read_text()
start = battle.index("static const u16 sSoundMovesTable[]")
end = battle.index("};",start)
assert "MOVE_CHIRP" in battle[start:end] and battle[start:end].count("MOVE_CHIRP")==1
ai = (root/"data/battle_ai_scripts.s").read_text()
start=ai.index("AI_CheckBadMove_CheckSoundproof::")
end=ai.index("AI_CheckBadMove_CheckEffect::",start)
assert "if_move MOVE_CHIRP, Score_Minus10" in ai[start:end]
asm = (root/"data/battle_anim_scripts.s").read_text()
assert "Move_CHIRP:\n\tgoto Move_GROWL" in asm
for path in json.loads((root/"tests/specifications/approved_chirp_delta.json").read_text())["files"]:
    assert prior_chirp_source(path) != (root/path).read_text()
for path in ["src/data/pokemon/level_up_learnsets.h","src/data/pokemon/tmhm_learnsets.h","src/data/trainer_parties.h"]:
    assert "MOVE_CHIRP" not in (root/path).read_text()
print("PASS: exact sound Chirp #365, Soundproof runtime table, AI avoidance, retained animation, no distribution, source delta")
