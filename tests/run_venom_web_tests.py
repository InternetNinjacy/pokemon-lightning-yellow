#!/usr/bin/env python3
"""Owner-approved Venom Web source contract; actual trap execution tested in mGBA."""
import json
import re
from pathlib import Path
from approved_venom_web_changes import prior_venom_web_source
from approved_skitter_slash_changes import prior_skitter_slash_source

root = Path(__file__).resolve().parents[1]
data = (root / "src/data/battle_moves.h").read_text()
m = re.search(r'\[MOVE_VENOM_WEB\]\s*=\s*\{(.*?)\n    \},', data, re.S)
assert m
fields = dict(re.findall(r'\.(\w+)\s*=\s*(.*?),', m[1]))
for field, value in dict(category="MOVE_CATEGORY_PHYSICAL", effect="EFFECT_TRAP",
                         power="50", type="TYPE_BUG", accuracy="90", pp="20",
                         secondaryEffectChance="100", target="MOVE_TARGET_SELECTED",
                         priority="0").items():
    assert fields[field] == value, field
assert "FLAG_MAKES_CONTACT" in fields["flags"]
assert "FLAG_PROTECT_AFFECTED" in fields["flags"]

a = (root / "data/battle_anim_scripts.s").read_text()
body = re.search(r'Move_VENOM_WEB:\n(.*?)\n\tend', a, re.S)[1]
assert body.count("\tcall BindWrapSqueezeTarget") == 2
native = re.search(r'Move_POISON_POWDER:\n(.*?)\n\tend', a, re.S)[1]
assert native in body
assert not re.search(r'\tcall Move_(BIND|POISON_POWDER)', body)
assert "Move_VENOM_WEB" in a.split("gBattleAnims_Moves::", 1)[1].split("\n\t.align", 1)[0]

# The native trap declines to refresh existing wrapping status.
engine = (root / "src/battle_script_commands.c").read_text()
assert "case MOVE_EFFECT_WRAP:" in engine
assert "if (gBattleMons[gEffectBattler].status2 & STATUS2_WRAPPED)" in engine
script = (root / "data/battle_scripts_1.s").read_text()
assert "BattleScript_EffectTrap::" in script
assert "setmoveeffect MOVE_EFFECT_WRAP" in script

for path in ("src/data/pokemon/level_up_learnsets.h", "src/data/pokemon/tmhm_learnsets.h", "src/data/trainer_parties.h"):
    assert "MOVE_VENOM_WEB" not in (root / path).read_text()
for path in json.loads((root / "tests/specifications/approved_venom_web_delta.json").read_text())["files"]:
    after = (root / path).read_text()
    assert prior_venom_web_source(path, after) != after
    assert prior_skitter_slash_source(path, after) != after

print("PASS: approved Venom Web #362 native Bind translation, zero poison mechanic, exact existing-art composite, no distribution, strict source reversal (source only)")
