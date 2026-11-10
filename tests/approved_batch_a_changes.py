"""Strict reversible source staging for BTL-005 Batch A.

These exact hunks restore the verified #385–386 acceptance commit before
historical source guards run. They do not approve or distribute Batch A.
"""
from pathlib import Path
import hashlib
HUNKS = {
  "include/constants/moves.h": [
    [
      "// BTL-005 Batch A staging: approved fixed IDs; intervening IDs remain reserved for B/C.\n#define MOVE_SEED_BOMB 391\n#define MOVE_PETAL_BLIZZARD 392\n#define MOVE_AURA_SPHERE 394\n#define MOVE_FOCUS_BLAST 395\n#define MOVE_FLASH_CANNON 397\n#define MOVE_MIRROR_SHOT 398\n#define MOVE_IRON_HEAD 399\n#define MOVE_BULLET_PUNCH 400\n#define MOVE_AIR_SLASH 401\n#define MOVE_HEART_STAMP 406\n#define MOVES_COUNT 40",
      "#define MOVES_COUNT 38"
    ]
  ],
  "src/data/battle_moves.h": [
    [
      "\n    // BTL-005 reserved #387: never distributed; not an implemented move.\n    [387] =\n    {\n        .category = MOVE_CATEGORY_STATUS,\n        .effect = EFFECT_SPLASH,\n        .power = 0,\n        .type = TYPE_NORMAL,\n        .accuracy = 0,\n        .pp = 0,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_USER,\n        .priority = 0,\n        .flags = 0,\n    },\n\n\n    // BTL-005 reserved #388: never distributed; not an implemented move.\n    [388] =\n    {\n        .category = MOVE_CATEGORY_STATUS,\n        .effect = EFFECT_SPLASH,\n        .power = 0,\n        .type = TYPE_NORMAL,\n        .accuracy = 0,\n        .pp = 0,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_USER,\n        .priority = 0,\n        .flags = 0,\n    },\n\n\n    // BTL-005 reserved #389: never distributed; not an implemented move.\n    [389] =\n    {\n        .category = MOVE_CATEGORY_STATUS,\n        .effect = EFFECT_SPLASH,\n        .power = 0,\n        .type = TYPE_NORMAL,\n        .accuracy = 0,\n        .pp = 0,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_USER,\n        .priority = 0,\n        .flags = 0,\n    },\n\n\n    // BTL-005 reserved #390: never distributed; not an implemented move.\n    [390] =\n    {\n        .category = MOVE_CATEGORY_STATUS,\n        .effect = EFFECT_SPLASH,\n        .power = 0,\n        .type = TYPE_NORMAL,\n        .accuracy = 0,\n        .pp = 0,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_USER,\n        .priority = 0,\n        .flags = 0,\n    },\n\n\n    // BTL-005 reserved #393: never distributed; not an implemented move.\n    [393] =\n    {\n        .category = MOVE_CATEGORY_STATUS,\n        .effect = EFFECT_SPLASH,\n        .power = 0,\n        .type = TYPE_NORMAL,\n        .accuracy = 0,\n        .pp = 0,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_USER,\n        .priority = 0,\n        .flags = 0,\n    },\n\n\n    // BTL-005 reserved #396: never distributed; not an implemented move.\n    [396] =\n    {\n        .category = MOVE_CATEGORY_STATUS,\n        .effect = EFFECT_SPLASH,\n        .power = 0,\n        .type = TYPE_NORMAL,\n        .accuracy = 0,\n        .pp = 0,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_USER,\n        .priority = 0,\n        .flags = 0,\n    },\n\n\n    // BTL-005 reserved #402: never distributed; not an implemented move.\n    [402] =\n    {\n        .category = MOVE_CATEGORY_STATUS,\n        .effect = EFFECT_SPLASH,\n        .power = 0,\n        .type = TYPE_NORMAL,\n        .accuracy = 0,\n        .pp = 0,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_USER,\n        .priority = 0,\n        .flags = 0,\n    },\n\n\n    // BTL-005 reserved #403: never distributed; not an implemented move.\n    [403] =\n    {\n        .category = MOVE_CATEGORY_STATUS,\n        .effect = EFFECT_SPLASH,\n        .power = 0,\n        .type = TYPE_NORMAL,\n        .accuracy = 0,\n        .pp = 0,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_USER,\n        .priority = 0,\n        .flags = 0,\n    },\n\n\n    // BTL-005 reserved #404: never distributed; not an implemented move.\n    [404] =\n    {\n        .category = MOVE_CATEGORY_STATUS,\n        .effect = EFFECT_SPLASH,\n        .power = 0,\n        .type = TYPE_NORMAL,\n        .accuracy = 0,\n        .pp = 0,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_USER,\n        .priority = 0,\n        .flags = 0,\n    },\n\n\n    // BTL-005 reserved #405: never distributed; not an implemented move.\n    [405] =\n    {\n        .category = MOVE_CATEGORY_STATUS,\n        .effect = EFFECT_SPLASH,\n        .power = 0,\n        .type = TYPE_NORMAL,\n        .accuracy = 0,\n        .pp = 0,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_USER,\n        .priority = 0,\n        .flags = 0,\n    },\n",
      ""
    ],
    [
      "\n    // BTL-005 Batch A approved move #391; reserved intervening IDs are not yet implemented.\n    [MOVE_SEED_BOMB] =\n    {\n        .category = MOVE_CATEGORY_PHYSICAL,\n        .effect = EFFECT_HIT,\n        .power = 80,\n        .type = TYPE_GRASS,\n        .accuracy = 100,\n        .pp = 15,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_SELECTED,\n        .priority = 0,\n        .flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,\n    },\n\n\n    // BTL-005 Batch A approved move #392; reserved intervening IDs are not yet implemented.\n    [MOVE_PETAL_BLIZZARD] =\n    {\n        .category = MOVE_CATEGORY_PHYSICAL,\n        .effect = EFFECT_HIT,\n        .power = 90,\n        .type = TYPE_GRASS,\n        .accuracy = 100,\n        .pp = 15,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_FOES_AND_ALLY,\n        .priority = 0,\n        .flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,\n    },\n\n\n    // BTL-005 Batch A approved move #394; reserved intervening IDs are not yet implemented.\n    [MOVE_AURA_SPHERE] =\n    {\n        .category = MOVE_CATEGORY_SPECIAL,\n        .effect = EFFECT_ALWAYS_HIT,\n        .power = 80,\n        .type = TYPE_FIGHTING,\n        .accuracy = 0,\n        .pp = 20,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_SELECTED,\n        .priority = 0,\n        .flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,\n    },\n\n\n    // BTL-005 Batch A approved move #395; reserved intervening IDs are not yet implemented.\n    [MOVE_FOCUS_BLAST] =\n    {\n        .category = MOVE_CATEGORY_SPECIAL,\n        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,\n        .power = 120,\n        .type = TYPE_FIGHTING,\n        .accuracy = 70,\n        .pp = 5,\n        .secondaryEffectChance = 10,\n        .target = MOVE_TARGET_SELECTED,\n        .priority = 0,\n        .flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,\n    },\n\n\n    // BTL-005 Batch A approved move #397; reserved intervening IDs are not yet implemented.\n    [MOVE_FLASH_CANNON] =\n    {\n        .category = MOVE_CATEGORY_SPECIAL,\n        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,\n        .power = 80,\n        .type = TYPE_STEEL,\n        .accuracy = 100,\n        .pp = 10,\n        .secondaryEffectChance = 10,\n        .target = MOVE_TARGET_SELECTED,\n        .priority = 0,\n        .flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,\n    },\n\n\n    // BTL-005 Batch A approved move #398; reserved intervening IDs are not yet implemented.\n    [MOVE_MIRROR_SHOT] =\n    {\n        .category = MOVE_CATEGORY_SPECIAL,\n        .effect = EFFECT_ACCURACY_DOWN_HIT,\n        .power = 65,\n        .type = TYPE_STEEL,\n        .accuracy = 85,\n        .pp = 10,\n        .secondaryEffectChance = 30,\n        .target = MOVE_TARGET_SELECTED,\n        .priority = 0,\n        .flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,\n    },\n\n\n    // BTL-005 Batch A approved move #399; reserved intervening IDs are not yet implemented.\n    [MOVE_IRON_HEAD] =\n    {\n        .category = MOVE_CATEGORY_PHYSICAL,\n        .effect = EFFECT_FLINCH_HIT,\n        .power = 80,\n        .type = TYPE_STEEL,\n        .accuracy = 100,\n        .pp = 15,\n        .secondaryEffectChance = 30,\n        .target = MOVE_TARGET_SELECTED,\n        .priority = 0,\n        .flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,\n    },\n\n\n    // BTL-005 Batch A approved move #400; reserved intervening IDs are not yet implemented.\n    [MOVE_BULLET_PUNCH] =\n    {\n        .category = MOVE_CATEGORY_PHYSICAL,\n        .effect = EFFECT_HIT,\n        .power = 40,\n        .type = TYPE_STEEL,\n        .accuracy = 100,\n        .pp = 30,\n        .secondaryEffectChance = 0,\n        .target = MOVE_TARGET_SELECTED,\n        .priority = 1,\n        .flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,\n    },\n\n\n    // BTL-005 Batch A approved move #401; reserved intervening IDs are not yet implemented.\n    [MOVE_AIR_SLASH] =\n    {\n        .category = MOVE_CATEGORY_SPECIAL,\n        .effect = EFFECT_FLINCH_HIT,\n        .power = 75,\n        .type = TYPE_FLYING,\n        .accuracy = 95,\n        .pp = 15,\n        .secondaryEffectChance = 30,\n        .target = MOVE_TARGET_SELECTED,\n        .priority = 0,\n        .flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,\n    },\n\n\n    // BTL-005 Batch A approved move #406; reserved intervening IDs are not yet implemented.\n    [MOVE_HEART_STAMP] =\n    {\n        .category = MOVE_CATEGORY_PHYSICAL,\n        .effect = EFFECT_FLINCH_HIT,\n        .power = 55,\n        .type = TYPE_PSYCHIC,\n        .accuracy = 100,\n        .pp = 25,\n        .secondaryEffectChance = 30,\n        .target = MOVE_TARGET_SELECTED,\n        .priority = 0,\n        .flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,\n    },\n\n",
      ""
    ]
  ],
  "src/data/text/move_names.h": [
    [
      "    [MOVE_SEED_BOMB] = _(\"SEED BOMB\"),\n    [MOVE_PETAL_BLIZZARD] = _(\"PETAL BLIZZARD\"),\n    [MOVE_AURA_SPHERE] = _(\"AURA SPHERE\"),\n    [MOVE_FOCUS_BLAST] = _(\"FOCUS BLAST\"),\n    [MOVE_FLASH_CANNON] = _(\"FLASH CANNON\"),\n    [MOVE_MIRROR_SHOT] = _(\"MIRROR SHOT\"),\n    [MOVE_IRON_HEAD] = _(\"IRON HEAD\"),\n    [MOVE_BULLET_PUNCH] = _(\"BULLET PUNCH\"),\n    [MOVE_AIR_SLASH] = _(\"AIR SLASH\"),\n    [MOVE_HEART_STAMP] = _(\"HEART STAMP\"),\n",
      ""
    ]
  ],
  "src/move_descriptions.c": [
    [
      "const u8 gMoveDescription_SEED_BOMB[] = _(\"Strikes the foe\\nwith a hard seed.\");\nconst u8 gMoveDescription_PETAL_BLIZZARD[] = _(\"A storm of petals\\nstrikes everyone\\naround the user.\");\nconst u8 gMoveDescription_AURA_SPHERE[] = _(\"An aura attack\\nthat never misses.\");\nconst u8 gMoveDescription_FOCUS_BLAST[] = _(\"May lower the\\nfoe's SP. DEF.\");\nconst u8 gMoveDescription_FLASH_CANNON[] = _(\"May lower the\\nfoe's SP. DEF.\");\nconst u8 gMoveDescription_MIRROR_SHOT[] = _(\"May lower the\\nfoe's accuracy.\");\nconst u8 gMoveDescription_IRON_HEAD[] = _(\"A head strike that\\nmay cause flinching.\");\nconst u8 gMoveDescription_BULLET_PUNCH[] = _(\"A fast punch that\\nstrikes first.\");\nconst u8 gMoveDescription_AIR_SLASH[] = _(\"Sharp wind blades\\nmay cause flinching.\");\nconst u8 gMoveDescription_HEART_STAMP[] = _(\"A heart-stopping\\nstrike that may\\ncause flinching.\");\n\n",
      ""
    ],
    [
      "    [MOVE_SEED_BOMB - 1] = gMoveDescription_SEED_BOMB,\n    [MOVE_PETAL_BLIZZARD - 1] = gMoveDescription_PETAL_BLIZZARD,\n    [MOVE_AURA_SPHERE - 1] = gMoveDescription_AURA_SPHERE,\n    [MOVE_FOCUS_BLAST - 1] = gMoveDescription_FOCUS_BLAST,\n    [MOVE_FLASH_CANNON - 1] = gMoveDescription_FLASH_CANNON,\n    [MOVE_MIRROR_SHOT - 1] = gMoveDescription_MIRROR_SHOT,\n    [MOVE_IRON_HEAD - 1] = gMoveDescription_IRON_HEAD,\n    [MOVE_BULLET_PUNCH - 1] = gMoveDescription_BULLET_PUNCH,\n    [MOVE_AIR_SLASH - 1] = gMoveDescription_AIR_SLASH,\n    [MOVE_HEART_STAMP - 1] = gMoveDescription_HEART_STAMP,\n",
      ""
    ]
  ],
  "data/battle_anim_scripts.s": [
    [
      "\t.4byte Move_NONE\n\t.4byte Move_NONE\n\t.4byte Move_NONE\n\t.4byte Move_NONE\n\t.4byte Move_SEED_BOMB\n\t.4byte Move_PETAL_BLIZZARD\n\t.4byte Move_NONE\n\t.4byte Move_AURA_SPHERE\n\t.4byte Move_FOCUS_BLAST\n\t.4byte Move_NONE\n\t.4byte Move_FLASH_CANNON\n\t.4byte Move_MIRROR_SHOT\n\t.4byte Move_IRON_HEAD\n\t.4byte Move_BULLET_PUNCH\n\t.4byte Move_AIR_SLASH\n\t.4byte Move_NONE\n\t.4byte Move_NONE\n\t.4byte Move_NONE\n\t.4byte Move_NONE\n\t.4byte Move_HEART_STAMP\n",
      ""
    ],
    [
      "\n\n@ BTL-005 Batch A: approved Gen III animation reuse. Unsupported\n@ intervening moves are reserved and never distributed from this branch.\nMove_SEED_BOMB:\n\tgoto Move_BULLET_SEED\n\nMove_PETAL_BLIZZARD:\n\tgoto Move_PETAL_DANCE\n\nMove_AURA_SPHERE:\n\tgoto Move_PSYCHIC\n\nMove_FOCUS_BLAST:\n\tgoto Move_HYPER_BEAM\n\n@ Flash Cannon: Hyper Beam energy followed by Metal Claw's metallic shimmer.\nMove_FLASH_CANNON:\n\tloadspritegfx ANIM_TAG_ORBS\n\tcreatevisualtask AnimTask_MetallicShine, 5, 0, 0, 0\n\twaitforvisualfinish\n\tplaysewithpan SE_M_HYPER_BEAM, SOUND_PAN_ATTACKER\n\tcreatevisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 1, 0, 8, 1\n\twaitforvisualfinish\n\tend\n\nMove_MIRROR_SHOT:\n\tgoto Move_SWIFT\n\nMove_IRON_HEAD:\n\tgoto Move_HEADBUTT\n\n@ Bullet Punch: Mach Punch's fast fist impact with Metal Claw shine.\nMove_BULLET_PUNCH:\n\tloadspritegfx ANIM_TAG_IMPACT\n\tloadspritegfx ANIM_TAG_HANDS_AND_FEET\n\tcreatevisualtask AnimTask_MetallicShine, 5, 0, 0, 0\n\twaitforvisualfinish\n\tcreatesprite gFistFootSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, 8, 1, 0\n\tplaysewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET\n\tcreatevisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 5, 1\n\twaitforvisualfinish\n\tend\n\nMove_AIR_SLASH:\n\tgoto Move_AIR_CUTTER\n\n@ Heart Stamp: Stomp foot impact followed by Confusion-style psychic pulse.\nMove_HEART_STAMP:\n\tloadspritegfx ANIM_TAG_HANDS_AND_FEET\n\tloadspritegfx ANIM_TAG_IMPACT\n\tmonbg ANIM_TARGET\n\tsetalpha 12, 8\n\tcreatesprite gStompFootSpriteTemplate, ANIM_ATTACKER, 3, 0, -32, 15\n\tdelay 19\n\tcreatesprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, -8, ANIM_TARGET, 1\n\tplaysewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET\n\twaitforvisualfinish\n\tcreatevisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 15, 1\n\tplaysewithpan SE_M_SUPERSONIC, SOUND_PAN_TARGET\n\twaitforvisualfinish\n\tclearmonbg ANIM_TARGET\n\tblendoff\n\tend\n",
      ""
    ]
  ]
}
BASE_BLOBS = {
  "include/constants/moves.h": "2eee2995edf119f08ce36e01eeac337c6ecac197",
  "src/data/battle_moves.h": "921de16e0c92df54d16ddb1e8de94790e5acc93f",
  "src/data/text/move_names.h": "00df9bd1b0ea8329a9d31761d9ba556df76c65a9",
  "src/move_descriptions.c": "3bacc83c2ab6a4d3238e6a7eff4b3f6feb5cae22",
  "data/battle_anim_scripts.s": "b28afe8369f3719be6d64cbcd768cad294a33e9c"
}

def prior_batch_a_source(path, text=None):
    path=str(path)
    if text is None:
        text=Path(path).read_text()
    # A later Batch B layer must first reverse exactly to its Batch A parent.
    if Path(__file__).with_name('approved_batch_b_changes.py').exists():
        from approved_batch_b_changes import prior_batch_b_source
        text=prior_batch_b_source(path,text)
    if path not in HUNKS:
        return text
    def digest(t):
        b=t.encode()
        return hashlib.sha1(b"blob "+str(len(b)).encode()+b"\0"+b).hexdigest()
    if digest(text)==BASE_BLOBS[path]:
        return text
    found=[text.count(after) for after,before in HUNKS[path]]
    if all(x==0 for x in found):
        return text  # Fully pre-projected historical view; downstream frozen hashes apply.
    assert all(x==1 for x in found), "Mixed/unreviewed Batch A source delta: "+path
    for after,before in HUNKS[path]:
        assert text.count(after)==1, "Unreviewed Batch A source delta: "+path
        text=text.replace(after,before,1)
    assert digest(text)==BASE_BLOBS[path], "Batch A does not strictly reverse to verified base: "+path
    return text
