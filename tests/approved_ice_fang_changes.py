"""Strict #372 reverse layer, applied ahead of immutable earlier source manifests."""
from pathlib import Path

HUNKS = {
 "include/constants/moves.h": [
  ("#define MOVE_ICE_FANG 372\n\n#define MOVES_COUNT 373", "#define MOVES_COUNT 372"),
 ],
 "src/data/text/move_names.h": [
  ("    [MOVE_ICE_FANG] = _(\"ICE FANG\"),\n", ""),
 ],
 "src/move_descriptions.c": [
  ("const u8 gMoveDescription_ICE_FANG[] = _(\"An icy bite may\\nfreeze the foe or\\nmake it flinch.\");\n", ""),
  ("    [MOVE_ICE_FANG - 1] = gMoveDescription_ICE_FANG,\n", ""),
 ],
 "src/data/battle_moves.h": [
  ("\n    [MOVE_ICE_FANG] =\n    {\n        .category = MOVE_CATEGORY_PHYSICAL,\n        .effect = EFFECT_ICE_FANG,\n        .power = 65,\n        .type = TYPE_ICE,\n        .accuracy = 95,\n        .pp = 15,\n        .secondaryEffectChance = 10,\n        .target = MOVE_TARGET_SELECTED,\n        .priority = 0,\n        .flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,\n    },", ""),
 ],
 "data/battle_anim_scripts.s": [
  ("\t.4byte Move_ICE_FANG\n", ""),
  ("\n@ Ice Fang #372, approved existing Bite + Ice Punch art. Inline two bodies,\n@ preserving native call-slot semantics and ending once after both sequences.\nMove_ICE_FANG:\n\tloadspritegfx ANIM_TAG_SHARP_TEETH\n\tloadspritegfx ANIM_TAG_IMPACT\n\tmonbg ANIM_TARGET\n\tsetalpha 12, 8\n\tplaysewithpan SE_M_BITE, SOUND_PAN_TARGET\n\tcreatesprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, 0, -32, 0, 0, 819, 10\n\tcreatesprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, 0, 32, 4, 0, -819, 10\n\tdelay 10\n\tcreatesprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2\n\tcreatevisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 4, 7, 1\n\twaitforvisualfinish\n\tclearmonbg ANIM_TARGET\n\tblendoff\n\tdelay 1\n\tmonbg ANIM_DEF_PARTNER\n\tsetalpha 12, 8\n\tloadspritegfx ANIM_TAG_ICE_CRYSTALS\n\tloadspritegfx ANIM_TAG_IMPACT\n\tloadspritegfx ANIM_TAG_HANDS_AND_FEET\n\tcreatesprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 0, 7, RGB_BLACK\n\tcreatevisualtask AnimTask_BlendBattleAnimPal, 10, F_PAL_TARGET, 2, 0, 9, RGB(12, 26, 31)\n\tdelay 20\n\tplaysewithpan SE_M_STRING_SHOT, SOUND_PAN_TARGET\n\tcreatesprite gIceCrystalSpiralInwardSmall, ANIM_ATTACKER, 2, 0\n\tcreatesprite gIceCrystalSpiralInwardSmall, ANIM_ATTACKER, 2, 64\n\tcreatesprite gIceCrystalSpiralInwardSmall, ANIM_ATTACKER, 2, 128\n\tcreatesprite gIceCrystalSpiralInwardSmall, ANIM_ATTACKER, 2, 192\n\tdelay 5\n\tcreatesprite gIceCrystalSpiralInwardLarge, ANIM_ATTACKER, 2, 32\n\tcreatesprite gIceCrystalSpiralInwardLarge, ANIM_ATTACKER, 2, 96\n\tcreatesprite gIceCrystalSpiralInwardLarge, ANIM_ATTACKER, 2, 160\n\tcreatesprite gIceCrystalSpiralInwardLarge, ANIM_ATTACKER, 2, 224\n\tdelay 17\n\tcreatesprite gFistFootSpriteTemplate, ANIM_ATTACKER, 4, 0, -10, 8, 1, 0\n\tcreatesprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, -10, ANIM_TARGET, 1\n\tplaysewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET\n\tdelay 2\n\tcreatevisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 5, 3, 1\n\twaitforvisualfinish\n\tdelay 15\n\tcall IceCrystalEffectShort\n\tdelay 5\n\tcreatevisualtask AnimTask_BlendBattleAnimPal, 10, F_PAL_TARGET, 2, 9, 0, RGB(12, 26, 31)\n\twaitforvisualfinish\n\tcreatesprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 0, 7, 0, RGB_BLACK\n\twaitforvisualfinish\n\tclearmonbg ANIM_DEF_PARTNER\n\tblendoff\n\tend", ""),
 ],
}

def prior_ice_fang_source(path,content=None):
 if content is None: content=Path(path).read_text()
 for after,before in HUNKS.get(str(path),[]):
  assert content.count(after)==1, "Unreviewed #372 delta: "+str(path)
  content=content.replace(after,before,1)
 return content
