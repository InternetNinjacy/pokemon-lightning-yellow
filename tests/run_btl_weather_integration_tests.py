#!/usr/bin/env python3
"""Exact dual source projection; neither history is treated as the new baseline."""
from pathlib import Path
import hashlib,json
from approved_btl_weather_integration import without_btl,without_weather,before_species_evolution
from approved_water_moves385_changes import prior_water_moves385_source
m=json.loads(Path('tests/specifications/btl_weather_integration_delta.json').read_text())
for path,row in m['files'].items():
 # Restore only the separately approved #385-386 delta before testing the
 # frozen merged integration hash. Unknown source mutations still fail.
 merged_source=before_species_evolution(path,prior_water_moves385_source(path)) if path=='src/battle_script_commands.c' else before_species_evolution(path,Path(path).read_text())
 assert hashlib.sha256(merged_source.encode()).hexdigest()==row['merged_sha256'],path
 assert hashlib.sha256(without_weather(path).encode()).hexdigest()==row['without_weather']['sha256'],path
 assert hashlib.sha256(without_btl(path).encode()).hexdigest()==row['without_btl']['sha256'],path
 # A novel mutation must fail both views, never be silently normalized away.
 for project in [without_weather,without_btl]:
  try:project(path,Path(path).read_text()+'\n/* unapproved mutation */\n')
  except AssertionError:pass
  else:raise AssertionError('Unreviewed integration change accepted: '+path)
s=Path('src/battle_util.c').read_text()
for name in ['BattlerHasActedThisTurn','BattlerWasDamagedBy','GetBattlerInitialAbility']:assert name in s
assert 'MOVE_BELLOW, MOVE_SNARL, SOUND_MOVES_END' in s
assert 'return GetMonAbility(GetBattlerSide(battler)' in s
print('PASS: exact six shared source paths project back to both frozen BTL and merged-main histories; novel mutations rejected, move and individual-ability helpers retained. Static evidence, not native certification.')
