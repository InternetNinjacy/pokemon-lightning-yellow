"""Strict reversal of the staged BTL-005 #387 delta; frozen ancestor hashes unchanged."""
from pathlib import Path

HUNKS = {
    "include/constants/moves.h": [
        ("#define MOVE_TIDAL_CRASH 386\n\n// BTL-005 next approved batch (staged pending runtime acceptance).\n#define MOVE_DISCHARGE 387\n\n#define MOVES_COUNT 388\n", "#define MOVE_TIDAL_CRASH 386\n\n#define MOVES_COUNT 387\n"),
    ],
    "src/data/battle_moves.h": [
        ("\n    // BTL-005 #387: all other battlers, independent 30% paralysis per hit.\n    [MOVE_DISCHARGE] =\n    {\n        .category = MOVE_CATEGORY_SPECIAL,\n        .effect = EFFECT_PARALYZE_HIT,\n        .power = 80,\n        .type = TYPE_ELECTRIC,\n        .accuracy = 100,\n        .pp = 15,\n        .secondaryEffectChance = 30,\n        .target = MOVE_TARGET_FOES_AND_ALLY,\n        .priority = 0,\n        .flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,\n    },\n", ""),
    ],
    "src/data/text/move_names.h": [
        ("    [MOVE_DISCHARGE] = _(\"DISCHARGE\"),\n", ""),
    ],
    "src/move_descriptions.c": [
        ("const u8 gMoveDescription_DISCHARGE[] = _(\"Electricity hits\\nall other foes.\\nMay paralyze.\");\n", ""),
        ("    [MOVE_DISCHARGE - 1] = gMoveDescription_DISCHARGE,\n", ""),
    ],
    "data/battle_anim_scripts.s": [
        ("\t.4byte Move_DISCHARGE\n", ""),
        ("\n@ Approved #387 reused Thunderbolt animation; real Doubles targeting remains independent.\nMove_DISCHARGE:\n\tgoto Move_THUNDERBOLT\n", ""),
    ],
}

def prior_moves387_source(path, text=None):
    if text is None:
        text=Path(path).read_text()
    for after,before in HUNKS.get(str(path), []):
        assert text.count(after)==1, "Unreviewed #387 source delta: "+str(path)
        text=text.replace(after,before,1)
    return text
