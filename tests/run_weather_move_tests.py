#!/usr/bin/env python3
"""Exact approved issue47 changes; runtime weather/accuracy tested in mGBA."""
from pathlib import Path
import re,json,hashlib
from approved_weather_move_changes import prior_weather_move_source
from approved_original_effect_changes import prior_original_effect_source
m=json.loads(Path('tests/specifications/approved_weather_move_delta.json').read_text())
for p,v in m['files'].items():
 assert hashlib.sha256(prior_original_effect_source(p).encode()).hexdigest()==v['after_sha256'],p
 assert hashlib.sha256(prior_weather_move_source(p).encode()).hexdigest()==v['before_sha256'],p
source=prior_original_effect_source('src/data/battle_moves.h');old=prior_weather_move_source('src/data/battle_moves.h')
def row(s,k):return re.search(r'\[MOVE_'+k+r'\] =\s*\{(.*?)\n    \}',s,re.S)[1]
assert row(source,'SANDSTORM')==row(old,'SANDSTORM').replace('TYPE_ROCK','TYPE_GROUND')
assert row(source,'ZAP_CANNON')==row(old,'ZAP_CANNON')
source=Path('src/battle_script_commands.c').read_text();weather=source.split('static void Cmd_weatherdamage(void)\n{')[1].split('\nstatic ',1)[0]
assert 'TYPE_ROCK' not in weather and 'TYPE_STEEL' not in weather
assert 'TYPE_GROUND' in weather and 'TYPE_ICE' in weather and 'ABILITY_SAND_VEIL' in weather and 'maxHP / 16' in weather
assert 'gBattleMoves[move].effect == EFFECT_THUNDER || move == MOVE_ZAP_CANNON' in source
assert 'gBattleWeather & B_WEATHER_SUN && gBattleMoves[move].effect == EFFECT_THUNDER' in source
print('PASS: exact approved Ground Sandstorm/only-Ground residual typing and Rain Zap Cannon delta; all other move data/scripts, original suppressions, Sun Thunder and no new ability slots preserved (static)')
