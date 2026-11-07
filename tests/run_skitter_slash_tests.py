#!/usr/bin/env python3
"""Source-level contract for approved BTL-005 move #361.

This deliberately does not certify ROM execution, native animation playback,
damage, or AI choices; those require the full compiled mGBA suite.
"""
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[1]


class SkitterSlashSourceTests(unittest.TestCase):
    def test_move_namespace_and_complete_catalog(self):
        from sys import path
        path.insert(0, str(ROOT / "scripts"))
        from move_catalog import validate
        import json
        overlay = json.loads((ROOT / "docs/moves/approved-design-overlays.json").read_text())
        self.assertEqual(validate(ROOT, overlay["new_moves"]), 366)
        constants = (ROOT / "include/constants/moves.h").read_text()
        self.assertRegex(constants, r"(?m)^#define MOVE_SKITTER_SLASH 361$")

    def test_approved_data_and_no_distribution(self):
        data = (ROOT / "src/data/battle_moves.h").read_text()
        body = re.search(r"\[MOVE_SKITTER_SLASH\]\s*=\s*\{(.*?)\n    \},", data, re.S)
        self.assertIsNotNone(body)
        fields = dict(re.findall(r"\.(\w+)\s*=\s*(.*?),", body.group(1)))
        self.assertEqual(
            {k: fields[k] for k in ("category", "effect", "power", "type", "accuracy", "pp",
                                      "secondaryEffectChance", "target", "priority")},
            dict(category="MOVE_CATEGORY_PHYSICAL", effect="EFFECT_HIT", power="40",
                 type="TYPE_BUG", accuracy="100", pp="15", secondaryEffectChance="0",
                 target="MOVE_TARGET_SELECTED", priority="0"),
        )
        self.assertIn("FLAG_PROTECT_AFFECTED", fields["flags"])
        self.assertIn("FLAG_MAKES_CONTACT", fields["flags"])

    def test_name_description_and_native_visual(self):
        names = (ROOT / "src/data/text/move_names.h").read_text()
        self.assertIn('[MOVE_SKITTER_SLASH] = _("SKITTER SLASH")', names)
        desc = (ROOT / "src/move_descriptions.c").read_text()
        self.assertIn("gMoveDescription_SKITTER_SLASH[]", desc)
        self.assertIn("[MOVE_SKITTER_SLASH - 1] = gMoveDescription_SKITTER_SLASH", desc)
        animations = (ROOT / "data/battle_anim_scripts.s").read_text()
        self.assertIn(".4byte Move_SKITTER_SLASH", animations)
        self.assertRegex(animations, r"Move_SKITTER_SLASH:\s+goto Move_FURY_CUTTER")


if __name__ == "__main__":
    unittest.main()
