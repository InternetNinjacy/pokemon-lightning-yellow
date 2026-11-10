"""Strict owner-approved #385–386 source reversal for earlier frozen guards.
Expected historical SHA values and original approved manifests remain unchanged.
"""
from pathlib import Path

HUNKS = {
  # New production delta is strictly reversible before the frozen ancestor
  # source-history guards run; never rewrite their approved snapshots.
  "src/battle_script_commands.c": [
    [
      "if (gCurrentMove == MOVE_ROLLOUT\n            && gBattleMons[gBattlerAttacker].status2 & STATUS2_DEFENSE_CURL)",
      "if (gCurrentMove != MOVE_GRUDGE\n            && gBattleMons[gBattlerAttacker].status2 & STATUS2_DEFENSE_CURL)"
    ]
  ],
  "include/constants/moves.h": [
    [
      "#define MOVE_VORTEX_SPIN 385\n#define MOVE_TIDAL_CRASH 386\n\n#define MOVES_COUNT 387\n",
      "#define MOVES_COUNT 385\n"
    ]
  ],
  "src/data/battle_moves.h": [
    [
      "\n    // BTL-005 #385: Whirlpool visuals, but confusion rather than trapping.\n    [MOVE_VORTEX_SPIN] =\n    {\n        .category = MOVE_CATEGORY_PHYSICAL,\n        .effect = EFFECT_CONFUSE_HIT,\n        .power = 55,\n        .type = TYPE_WATER,\n        .accuracy = 100,\n        .pp = 25,\n        .secondaryEffectChance = 30,\n        .target = MOVE_TARGET_SELECTED,\n        .priority = 0,\n        .flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,\n    },\n\n    // BTL-005 #386: use the native Rollout/Defense Curl battle path.\n    [MOVE_TIDAL_CRASH] =\n    {\n        .category = MOVE_CATEGORY_PHYSICAL,\n        .effect = EFFECT_ROLLOUT,\n        .power = 30,\n        .type = TYPE_WATER,\n        .accuracy = 90,\n        .pp = 20,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_SELECTED,\n        .priority = 0,\n        .flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,\n    },\n",
      ""
    ]
  ],
  "src/data/text/move_names.h": [
    [
      "    [MOVE_VORTEX_SPIN] = _(\"VORTEX SPIN\"),\n    [MOVE_TIDAL_CRASH] = _(\"TIDAL CRASH\"),\n",
      ""
    ]
  ],
  "src/move_descriptions.c": [
    [
      "const u8 gMoveDescription_VORTEX_SPIN[] = _(\"A swirling water\\nstrike that may\\nconfuse the foe.\");\nconst u8 gMoveDescription_TIDAL_CRASH[] = _(\"A 5-turn water\\nattack that grows\\nstronger each hit.\");\n\n",
      ""
    ],
    [
      "    [MOVE_VORTEX_SPIN - 1] = gMoveDescription_VORTEX_SPIN,\n    [MOVE_TIDAL_CRASH - 1] = gMoveDescription_TIDAL_CRASH,\n",
      ""
    ]
  ],
  "data/battle_anim_scripts.s": [
    [
      "\t.4byte Move_VORTEX_SPIN\n\t.4byte Move_TIDAL_CRASH\n",
      ""
    ],
    [
      "\n@ BTL-005 owner-approved Generation III art reuse; no trapping effect attached.\nMove_VORTEX_SPIN:\n\tgoto Move_WHIRLPOOL\n\n@ BTL-005 single-target Rollout mechanics with approved Surf animation.\nMove_TIDAL_CRASH:\n\tgoto Move_SURF\n",
      ""
    ]
  ]
}

def prior_water_moves385_source(path, text=None):
    if text is None:
        text=Path(path).read_text()
    for after,before in HUNKS.get(str(path),[]):
        assert text.count(after)==1, "Unreviewed #385-386 source delta: "+str(path)
        text=text.replace(after,before,1)
    return text
