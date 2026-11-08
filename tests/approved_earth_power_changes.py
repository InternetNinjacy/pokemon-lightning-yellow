"""Strict #371 reverse layer, preserving #370 and earlier approved manifests."""
from pathlib import Path

HUNKS = {
  "include/constants/moves.h": [
    [
      "#define MOVE_EARTH_POWER 371\n\n#define MOVES_COUNT 372",
      "#define MOVES_COUNT 371"
    ]
  ],
  "src/data/text/move_names.h": [
    [
      "    [MOVE_EARTH_POWER] = _(\"EARTH POWER\"),\n",
      ""
    ]
  ],
  "src/move_descriptions.c": [
    [
      "const u8 gMoveDescription_EARTH_POWER[] = _(\"The ground erupts\\nwith power. May\\nlower Sp. Def.\");\n",
      ""
    ],
    [
      "    [MOVE_EARTH_POWER - 1] = gMoveDescription_EARTH_POWER,\n",
      ""
    ]
  ],
  "src/data/battle_moves.h": [
    [
      "\n    [MOVE_EARTH_POWER] =\n    {\n        .category = MOVE_CATEGORY_SPECIAL,\n        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,\n        .power = 90,\n        .type = TYPE_GROUND,\n        .accuracy = 100,\n        .pp = 10,\n        .secondaryEffectChance = 10,\n        .target = MOVE_TARGET_SELECTED,\n        .priority = 0,\n        .flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,\n    },\n",
      ""
    ]
  ],
  "data/battle_anim_scripts.s": [
    [
      "\t.4byte Move_EARTH_POWER\n",
      ""
    ],
    [
      "\n@ Owner-approved Earthquake artwork reuse for Earth Power #371; no actual weather/field change.\nMove_EARTH_POWER:\n\tgoto Move_EARTHQUAKE\n",
      ""
    ]
  ]
}

def prior_earth_power_source(path, content=None):
    if content is None:
        content = Path(path).read_text()
    from approved_ice_fang_changes import prior_ice_fang_source
    content = prior_ice_fang_source(path, content)
    for after, before in HUNKS.get(str(path), []):
        assert content.count(after) == 1, "Unreviewed Earth Power #371 delta: " + str(path)
        content = content.replace(after, before, 1)
    return content
