"""Source-only clock wiring/provenance checks; not visual/runtime acceptance."""
import hashlib
import re
import subprocess
import unittest
from pathlib import Path

BASE = '28cf138cfcd19ee8f742fdd12b4bec1793b39cf4'
APPROVED = 'b9af0975c4ba9a1b5d486c324928148686d14aa6'
# Exact approved source digest avoids requiring the old experimental branch in CI.
APPROVED_SOURCE_SHA256 = '5a5521def200e2eb0c9c4279729aba7bd1e3210a5134521050910581c353931f'
PATH = 'src/field_weather_effects.c'


def historical(ref, path=PATH):
    return subprocess.check_output(['git', 'show', ref + ':' + path], text=True)


def function(text, name):
    match = re.search(r'^(?:static )?(?:void|bool8) ' + name + r'\([^\n]*\)\n\{', text, re.M)
    assert match, name
    start = match.start()
    pos = match.end()
    depth = 1
    while depth:
        depth += (text[pos] == '{') - (text[pos] == '}')
        pos += 1
    return text[start:pos]


class SnowPersistenceSourceTests(unittest.TestCase):
    def test_exact_previously_authorized_correction(self):
        source = Path(PATH).read_text()
        self.assertEqual(hashlib.sha256(source.encode()).hexdigest(), APPROVED_SOURCE_SHA256, APPROVED)
        self.assertEqual(source.count('    ++gWeatherPtr->snowflakeTimer;\n'), 1)
        old = source.replace('    ++gWeatherPtr->snowflakeTimer;\n', '', 1)
        old = old.replace('// Snow_Main advances the shared restart timer.', '// Timer is never incremented', 1)
        self.assertEqual(old, historical(BASE))

    def test_clock_runs_before_initialization_guard(self):
        main = function(Path(PATH).read_text(), 'Snow_Main')
        self.assertRegex(main, r'\{\n    \+\+gWeatherPtr->snowflakeTimer;\n    if \(gWeatherPtr->initStep == 0')
        self.assertEqual(main.count('snowflakeTimer'), 1)
        manager = Path('src/field_weather.c').read_text()
        self.assertIn('{Snow_InitVars, Snow_Main, Snow_InitAll, Snow_Finish}', manager)
        # Stable-weather task invokes Snow_Main even after initialization completes.
        task = manager.split('static void Task_WeatherMain(u8 taskId)\n{', 1)[1].split('\n\n\nstatic void None_Init', 1)[0]
        self.assertIn('else\n    {\n        sWeatherFuncs[gWeatherPtr->currWeather].main();\n    }', task)

    def test_existing_wait_recycle_and_exit_rules_preserved(self):
        source = Path(PATH).read_text()
        baseline = historical(BASE)
        wait = function(source, 'WaitSnowflakeSprite')
        self.assertIn('if (gWeatherPtr->snowflakeTimer > 18)', wait)
        self.assertIn('gWeatherPtr->snowflakeTimer = 0;', wait)
        self.assertIn('sprite->invisible = FALSE;', wait)
        self.assertIn('sprite->callback = UpdateSnowflakeSprite;', wait)
        self.assertNotIn('++sprite->tFallCounter', wait)
        for name in ['Snow_InitVars', 'Snow_InitAll', 'Snow_Finish',
                     'UpdateVisibleSnowflakeSprites', 'CreateSnowflakeSprite',
                     'DestroySnowflakeSprite', 'InitSnowflakeSpriteMovement',
                     'UpdateSnowflakeSprite']:
            with self.subTest(function=name):
                self.assertEqual(function(source, name), function(baseline, name))

    def test_shared_weather_palette_battle_and_save_sources_unchanged(self):
        for path in ['src/field_weather.c', 'src/dynamic_weather.c',
                     'src/data/dynamic_weather.h', 'include/field_weather.h',
                     'include/global.h', 'src/battle_util.c']:
            with self.subTest(path=path):
                self.assertEqual(Path(path).read_text(), historical(BASE, path))


if __name__ == '__main__':
    unittest.main()
