"""Static data/layout/route audits, explicitly distinct from executed damage tests."""
from collections import Counter
import hashlib
import json
from pathlib import Path
import re
import unittest


def source(path):
    return Path(path).read_text()


class SplitAudits(unittest.TestCase):
    def test_every_move_matches_reviewed_category_manifest(self):
        expected = json.loads(source('tests/move_categories.json'))
        entries = re.findall(r'\[(MOVE_\w+)\] =\s*\{(.*?)\n    \}', source('src/data/battle_moves.h'), re.S)
        self.assertEqual(len(entries), 355)
        self.assertEqual(len(dict(entries)), len(entries))
        actual = {}
        for name, body in entries:
            categories = re.findall(r'\.category = MOVE_CATEGORY_(\w+)', body)
            self.assertEqual(len(categories), 1, name)
            actual[name] = categories[0]
        self.assertEqual(actual, expected)
        print('Category manifest:', dict(Counter(actual.values())))

    def test_no_unapproved_move_property_changes(self):
        # Exact upstream move data with only the category include/fields removed.
        data = source('src/data/battle_moves.h')
        data = data.replace('#include "constants/move_categories.h"\n\n', '')
        data = re.sub(r'        \.category = MOVE_CATEGORY_\w+,\n', '', data)
        expected = json.loads(source('tests/preserved_paths.json'))['move_data_sha256']
        self.assertEqual(hashlib.sha256(data.encode()).hexdigest(), expected)

    def test_specialized_scripts_and_damage_commands_preserved(self):
        from run_split_tests import extract_function
        fixture = json.loads(source('tests/preserved_paths.json'))
        for path, digest in fixture['files'].items():
            self.assertEqual(hashlib.sha256(Path(path).read_bytes()).hexdigest(), digest, path)
        commands = source('src/battle_script_commands.c')
        for name, digest in fixture['functions'].items():
            self.assertEqual(hashlib.sha256(extract_function(commands, name).encode()).hexdigest(), digest, name)

    def test_category_consumers_use_authoritative_move_data(self):
        battle = source('src/battle_script_commands.c')
        self.assertIn('IS_MOVE_PHYSICAL(move)', battle)  # accuracy's resolved move
        self.assertIn('if (IS_MOVE_PHYSICAL(gCurrentMove)', battle)  # Counter history
        self.assertIn('else if (IS_MOVE_SPECIAL(gCurrentMove)', battle)
        self.assertIn('(gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg)', battle)
        self.assertNotIn('IS_TYPE_PHYSICAL', battle + source('src/pokemon.c') + source('include/battle.h'))
        self.assertIn('GetMoveCategoryShortName(moveInfo->moves[gMoveSelectionCursor[gActiveBattler]])', source('src/battle_controller_player.c'))
        summary = source('src/pokemon_summary_screen.c')
        self.assertIn('GetMoveCategoryName(sMonSummaryScreen->moveIds[sMoveSelectionCursorPos])', summary)
        self.assertIn('GetMoveCategoryShortName(sMonSummaryScreen->moveIds[4])', summary)

    def test_ui_text_fits_existing_windows(self):
        # Use the game's actual FONT_SMALL Latin widths and character encoding.
        widths = re.search(r'sFontSmallLatinGlyphWidths\[\] =\s*\{(.*?)\};', source('src/text.c'), re.S)[1]
        widths = [int(n) for n in re.findall(r'\d+', widths)]
        encoding = {char: int(value, 16) for char, value in re.findall(r"^'(.)'\s*= ([0-9A-F]{2})$", source('charmap.txt'), re.M)}
        def width(text):
            return sum(widths[encoding[c]] for c in text)
        types = re.search(r'const u8 gTypeNames.*?\n\};', source('src/battle_main.c'), re.S)[0]
        names = re.findall(r'_\("([A-Z?]+)"\)', types)
        self.assertEqual(len(names), 18)
        for name in names:
            for category in ('PHYS', 'SPEC', 'STAT'):
                self.assertLessEqual(width(name + '/' + category), 64)
        for category in ('PHYS', 'SPEC', 'STAT'):
            self.assertLessEqual(3 + width(category), 40)  # summary icon column
        # Geometry bounds: category list row fits before next icon; fifth fits pane.
        self.assertLessEqual(18 + 13, 28 + 5)
        self.assertLessEqual(4 * 28 + 18 + 13, 18 * 8)
        self.assertLessEqual(29 + 13, 42)  # detail category vs description

    def test_ai_strategy_no_longer_infers_categories_from_types(self):
        scripts = source('data/battle_ai_scripts.s')
        self.assertNotRegex(scripts, r'PhysicalTypes|PhysicalTypeList|SpecialTypeList')
        self.assertEqual(scripts.count('get_move_category_from_result'), 4)
        self.assertEqual(scripts.count('if_has_move_category AI_TARGET'), 6)
        commands = source('src/battle_ai_script_commands.c')
        self.assertIn('Cmd_get_move_category_from_result,', commands)
        self.assertIn('Cmd_has_move_category,', commands)


def run_audits():
    result = unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(SplitAudits))
    if not result.wasSuccessful():
        raise SystemExit(1)
