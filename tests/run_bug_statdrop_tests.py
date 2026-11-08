#!/usr/bin/env python3
"""Approved Bug Bite/Chitin Crush source contracts. Native mechanics tested separately."""
import json
import re
from pathlib import Path
from approved_bug_statdrop_changes import prior_bug_statdrop_source

root = Path(__file__).resolve().parents[1]
spec = json.loads((root / "docs/moves/move-specification.json").read_text())["moves"]
expected = [
    ("BUG_BITE", 363, 65, "EFFECT_SPECIAL_ATTACK_DOWN_HIT"),
    ("CHITIN_CRUSH", 364, 85, "EFFECT_DEFENSE_DOWN_HIT"),
]
for key, move_id, power, effect in expected:
    row = next(x for x in spec if x["key"] == key)
    assert row["id"] == move_id and not row["delta"]
    current = row["current"]
    assert (current["category"], current["type"], current["power"], current["accuracy"],
            current["pp"], current["priority"], current["effect"], current["secondaryEffectChance"],
            current["target"]) == ("PHYSICAL", "BUG", power, 90, 15, 0, effect, 30,
                                     "MOVE_TARGET_SELECTED")
    assert "FLAG_MAKES_CONTACT" in current["flags"]
for path in json.loads((root / "tests/specifications/approved_bug_statdrop_delta.json").read_text())["files"]:
    assert prior_bug_statdrop_source(path) != (root / path).read_text()
asm = (root / "data/battle_anim_scripts.s").read_text()
assert re.search(r'Move_BUG_BITE:\n\tgoto Move_BITE\n', asm)
vice = re.search(r'Move_VICE_GRIP:\n(.*?)\n\tend', asm, re.S)[1]
rock = re.search(r'Move_ROCK_SMASH:\n(.*?)\n\tend', asm, re.S)[1]
chitin = re.search(r'Move_CHITIN_CRUSH:\n(.*?)\n\tend', asm, re.S)[1]
assert chitin == vice + "\n\twaitforvisualfinish\n" + rock
scripts = (root / "data/battle_scripts_1.s").read_text()
assert "BattleScript_EffectSpecialAttackDownHit::" in scripts and "setmoveeffect MOVE_EFFECT_SP_ATK_MINUS_1" in scripts
assert "BattleScript_EffectDefenseDownHit::" in scripts and "setmoveeffect MOVE_EFFECT_DEF_MINUS_1" in scripts
for path in ["src/data/pokemon/level_up_learnsets.h", "src/data/pokemon/tmhm_learnsets.h", "src/data/trainer_parties.h"]:
    assert all("MOVE_" + key not in (root / path).read_text() for key, *_ in expected)
print("PASS: exact two approved Bug statdrop moves, native effects, 30% proc, existing-art visuals, no distribution, exact source reversal")
