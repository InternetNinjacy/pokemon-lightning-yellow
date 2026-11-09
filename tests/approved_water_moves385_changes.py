"""Strict reversal of approved move #385-386 additions for frozen prior-source audits.
Preserves every earlier manifest/hash. Unexpected source changes still fail.
"""
from pathlib import Path

HUNKS = {
 "include/constants/moves.h": [
  ("#define MOVE_VORTEX_SPIN 385\\n#define MOVE_TIDAL_CRASH 386\\n\\n#define MOVES_COUNT 387", "#define MOVES_COUNT 385"),
 ],
 "src/data/battle_moves.h": [
  ("""    // BTL-005 #385: Whirlpool visuals, but confusion rather than trapping.
    [MOVE_VORTEX_SPIN] =
    {
        .category = MOVE_CATEGORY_PHYSICAL,
        .effect = EFFECT_CONFUSE_HIT,
        .power = 55,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 25,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .priority = 0,
        .flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
    },

    // BTL-005 #386: use the native Rollout/Defense Curl battle path.
    [MOVE_TIDAL_CRASH] =
    {
        .category = MOVE_CATEGORY_PHYSICAL,
        .effect = EFFECT_ROLLOUT,
        .power = 30,
        .type = TYPE_WATER,
        .accuracy = 90,
        .pp = 20,
        .secondaryEffectChance = 0,
        .target = MOVE_TARGET_SELECTED,
        .priority = 0,
        .flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
    },
""", ""),
 ],
 "src/data/text/move_names.h": [
  ('    [MOVE_VORTEX_SPIN] = _("VORTEX SPIN"),\\n    [MOVE_TIDAL_CRASH] = _("TIDAL CRASH"),\\n', ""),
 ],
 "src/move_descriptions.c": [
  ('const u8 gMoveDescription_VORTEX_SPIN[] = _("A swirling water\\\\nstrike that may\\\\nconfuse the foe.");\\nconst u8 gMoveDescription_TIDAL_CRASH[] = _("A 5-turn water\\\\nattack that grows\\\\nstronger each hit.");\\n\\n', ""),
  ('    [MOVE_VORTEX_SPIN - 1] = gMoveDescription_VORTEX_SPIN,\\n    [MOVE_TIDAL_CRASH - 1] = gMoveDescription_TIDAL_CRASH,\\n', ""),
 ],
 "data/battle_anim_scripts.s": [
  ('\\t.4byte Move_VORTEX_SPIN\\n\\t.4byte Move_TIDAL_CRASH\\n', ""),
  ('\\n@ BTL-005 owner-approved Generation III art reuse; no trapping effect attached.\\nMove_VORTEX_SPIN:\\n\\tgoto Move_WHIRLPOOL\\n\\n@ BTL-005 single-target Rollout mechanics with approved Surf animation.\\nMove_TIDAL_CRASH:\\n\\tgoto Move_SURF\\n', ""),
 ],
}

def prior_water_moves385_source(path, text=None):
    if text is None:
        text=Path(path).read_text()
    for after,before in HUNKS.get(str(path),[]):
        assert text.count(after)==1, "Unreviewed #385-386 source delta: "+str(path)
        text=text.replace(after,before,1)
    return text
