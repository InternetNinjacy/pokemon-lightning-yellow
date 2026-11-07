"""Strict #370 reverse layer, applied *before* immutable older approval manifests.

Each exact source insertion is reversed only when its reviewed text appears once.
Never use broad regex deletion, weaken historical assertions, or change original manifests.
"""
from pathlib import Path


def prior_scorching_sands_source(path, content=None):
    if content is None:
        content = Path(path).read_text()
    path = str(path)
    from approved_earth_power_changes import prior_earth_power_source
    content = prior_earth_power_source(path, content)
    changes = {
        'include/constants/moves.h': [
            ('#define MOVE_SCORCHING_SANDS 370\n\n#define MOVES_COUNT 371',
             '#define MOVES_COUNT 370'),
        ],
        'src/data/text/move_names.h': [
            ('    [MOVE_SCORCHING_SANDS] = _("SCORCHING SANDS"),\n', ''),
        ],
        'src/move_descriptions.c': [
            ('const u8 gMoveDescription_SCORCHING_SANDS[] = _("Scorching sand\\nmay leave the foe\\nwith a burn.");\n', ''),
            ('    [MOVE_SCORCHING_SANDS - 1] = gMoveDescription_SCORCHING_SANDS,\n', ''),
        ],
        'src/data/battle_moves.h': [
            ('''\n    [MOVE_SCORCHING_SANDS] =
    {
        .category = MOVE_CATEGORY_SPECIAL,
        .effect = EFFECT_BURN_HIT,
        .power = 70,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .priority = 0,
        .flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
    },
''', ''),
        ],
        'data/battle_anim_scripts.s': [
            ('\t.4byte Move_SCORCHING_SANDS\n', ''),
            ('''\n\n@ Owner-approved Sandstorm + Ember composite; all assets and sounds are native.
@ No weather state mutation, imported sprites, or persistent background.
Move_SCORCHING_SANDS:
\tloadspritegfx ANIM_TAG_FLYING_DIRT
\tloadspritegfx ANIM_TAG_SMALL_EMBER
\tplaysewithpan SE_M_SANDSTORM, SOUND_PAN_ATTACKER
\tcreatesprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 10, 2304, 96, 0
\tdelay 10
\tcreatesprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 50, 2560, 96, 0
\tdelay 10
\tcreatesprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 70, 1984, 96, 0
\tdelay 10
\twaitforvisualfinish
\tloopsewithpan SE_M_EMBER, SOUND_PAN_ATTACKER, 5, 2
\tcreatesprite gEmberSpriteTemplate, ANIM_TARGET, 2, 20, 0, -16, 24, 20, 1
\tdelay 4
\tcreatesprite gEmberSpriteTemplate, ANIM_TARGET, 2, 20, 0, 0, 24, 20, 1
\tdelay 4
\tcreatesprite gEmberSpriteTemplate, ANIM_TARGET, 2, 20, 0, 16, 24, 20, 1
\tdelay 16
\tplaysewithpan SE_M_FLAME_WHEEL, SOUND_PAN_TARGET
\tcall EmberFireHit
\tcall EmberFireHit
\tcall EmberFireHit
\twaitforvisualfinish
\tend
''', ''),
        ],
    }
    for after, before in changes.get(path, []):
        assert content.count(after) == 1, 'Unreviewed Scorching Sands #370 delta: ' + path
        content = content.replace(after, before, 1)
    return content
