#!/usr/bin/env python3
"""Temporary source fixtures test tooling only, never playable moves or battles."""
from pathlib import Path
import importlib.util
import json
import re
import shutil
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))
from move_catalog import validate
spec = importlib.util.spec_from_file_location('reconcile', ROOT / 'scripts/reconcile-move-specification.py')
reconcile = importlib.util.module_from_spec(spec)
spec.loader.exec_module(reconcile)


class MoveCatalogTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.paths = ['include/constants/moves.h', 'src/data/battle_moves.h',
                      'src/data/text/move_names.h', 'src/move_descriptions.c',
                      'data/battle_anim_scripts.s', 'docs/moves/approved-design-overlays.json',
                      'docs/moves/animation-reuse.json']
        for path in self.paths:
            dest = self.root / path
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / path, dest)
        self.design = json.loads((self.root / self.paths[-2]).read_text())

    def change(self, path, old, new):
        p = self.root / path
        text = p.read_text()
        self.assertIn(old, text)
        p.write_text(text.replace(old, new, 1))

    def add_fixture(self):
        from approved_new_fire_move_changes import prior_new_fire_move_source
        for path in self.paths[:5]:
            p = self.root / path
            p.write_text(prior_new_fire_move_source(path, p.read_text()))
        animation = json.loads((self.root / self.paths[-1]).read_text())
        for row in animation['entries']:
            for script in row['scripts']:
                script['line'] -= int(re.search(r'#define MOVES_COUNT (\d+)', (ROOT / self.paths[0]).read_text())[1]) - 355
        (self.root / self.paths[-1]).write_text(json.dumps(animation))
        self.change(self.paths[0], '#define MOVES_COUNT 355',
                    '#define MOVE_FLAME_FANG 355\n#define MOVES_COUNT 356')
        body = '''    [MOVE_FLAME_FANG] =
    {
        .category = MOVE_CATEGORY_PHYSICAL,
        .effect = EFFECT_FLINCH_HIT,
        .power = 90,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .priority = 0,
        .flags = FLAG_MAKES_CONTACT,
    },
'''
        self.change(self.paths[1], '\n};', '\n' + body + '};')
        self.change(self.paths[2], '\n};', '\n    [MOVE_FLAME_FANG] = _("Flame Fang"),\n};')
        self.change(self.paths[3], '\n};', '\n    [MOVE_FLAME_FANG - 1] = gMoveDescription_Fixture,\n};\nconst u8 gMoveDescription_Fixture[] = _("Fixture only.");')
        # Appending pointer shifts source line provenance; update fixture only.
        self.change(self.paths[4], '\t.4byte Move_COUNT @', '\t.4byte Move_FLAME_FANG\n\t.4byte Move_COUNT @')
        with (self.root / self.paths[4]).open('a') as out:
            out.write('\nMove_FLAME_FANG:\n\tend\n')
        animation = json.loads((self.root / self.paths[-1]).read_text())
        for row in animation['entries']:
            for script in row['scripts']:
                script['line'] += 1
        (self.root / self.paths[-1]).write_text(json.dumps(animation))
        r = self.design['new_moves'][0]
        r['engine_translation'] = {'target': 'MOVE_TARGET_SELECTED', 'effect': 'EFFECT_FLINCH_HIT', 'flags': 'FLAG_MAKES_CONTACT'}
        (self.root / self.paths[-2]).write_text(json.dumps(self.design))

    def test_current_generation_byte_identical(self):
        generated = reconcile.render(ROOT)
        for name, text in generated.items():
            self.assertEqual((ROOT / 'docs/moves' / name).read_bytes(), text.encode())
        self.assertEqual(validate(ROOT, self.design['new_moves']), int(re.search(r'#define MOVES_COUNT (\d+)', (ROOT / self.paths[0]).read_text())[1]))

    def test_complete_fixture_is_source_only_not_runtime_accepted(self):
        self.add_fixture()
        rows = json.loads(reconcile.render(self.root)['move-specification.json'])['moves']
        self.assertEqual(len(rows), 410)
        first = rows[354]
        self.assertEqual(first['id'], 355)
        self.assertEqual(first['implementation_status'], 'SOURCE_DATA_PRESENT_RUNTIME_UNVERIFIED')
        self.assertFalse(first['animation']['runtime_verified'])
        self.assertIsNone(rows[355]['current'])

    def test_wrong_owner_power_remains_a_discrepancy(self):
        self.add_fixture()
        self.change(self.paths[1], '.power = 90,\n        .type = TYPE_FIRE', '.power = 65,\n        .type = TYPE_FIRE')
        row = json.loads(reconcile.render(self.root)['move-specification.json'])['moves'][354]
        self.assertEqual(row['implementation_status'], 'SOURCE_DIFFERS_FROM_APPROVED_DESIGN')
        self.assertEqual(row['delta']['power'], {'current': 65, 'approved': 90})

    def test_incomplete_or_reordered_fixtures_rejected(self):
        cases = [
            (0, 'MOVE_FLAME_FANG 355', 'MOVE_FLAME_FANG 356'),
            (0, 'MOVE_FLAME_FANG 355', 'MOVE_FLASHFIRE 355'),
            (1, '[MOVE_FLAME_FANG]', '[MOVE_UNKNOWN]'),
            (1, '.category = MOVE_CATEGORY_PHYSICAL,\n        .effect = EFFECT_FLINCH_HIT', '.effect = EFFECT_FLINCH_HIT'),
            (2, '[MOVE_FLAME_FANG]', '[MOVE_UNKNOWN]'),
            (3, '[MOVE_FLAME_FANG - 1]', '[MOVE_UNKNOWN - 1]'),
            (3, 'gMoveDescription_Fixture[]', 'gMoveDescription_Missing[]'),
            (4, '.4byte Move_FLAME_FANG', '.4byte Move_NONE'),
            (4, '\nMove_FLAME_FANG:', '\nMissingLabel:'),
        ]
        self.add_fixture()
        snapshots = {p: (self.root / p).read_text() for p in self.paths}
        for index, before, after in cases:
            with self.subTest(missing=before):
                for p, text in snapshots.items():
                    (self.root / p).write_text(text)
                self.change(self.paths[index], before, after)
                with self.assertRaises(AssertionError):
                    validate(self.root, self.design['new_moves'])

    def test_duplicate_rows_rejected(self):
        self.change(self.paths[2], '\n};', '\n    [MOVE_POUND] = _("duplicate"),\n};')
        with self.assertRaisesRegex(AssertionError, 'duplicate'):
            validate(self.root, self.design['new_moves'])


if __name__ == '__main__':
    unittest.main()
