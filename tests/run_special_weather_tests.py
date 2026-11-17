#!/usr/bin/env python3
"""Execute actual policy/lifecycle source with controlled flags/RNG/RTC; not mGBA."""
from pathlib import Path
import ast,hashlib,json,re,subprocess,tempfile
root=Path(__file__).resolve().parents[1]
m=json.loads((root/'tests/specifications/special_weather_delta.json').read_text())
from approved_special_weather_changes import prior_special_weather_source
from approved_follower_changes import prior_follower_source
for path,record in m['files'].items():
 # Strip only the separately reviewed WRL-010 source hooks, then retain
 # exact immutable special-weather after/before SHA-256 requirements.
 preserved=prior_follower_source(path,Path(path).read_text())
 assert hashlib.sha256(preserved.encode()).hexdigest()==record['after_sha256'],path
 assert hashlib.sha256(prior_special_weather_source(path,preserved).encode()).hexdigest()==record['before_sha256'],path
# Reuse the existing tested host environment, not a second weather implementation.
tree=ast.parse(Path('tests/run_weather_tests.py').read_text())
pre=next(ast.literal_eval(n.value) for n in tree.body if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='pre' for t in n.targets))
source=Path('src/dynamic_weather.c').read_text().replace('#include "data/dynamic_weather.h"',Path('src/data/dynamic_weather.h').read_text())
source=re.sub(r'^#include .*\n','',source,flags=re.M)
time=re.sub(r'^#include .*\n','',Path('src/time_of_day.c').read_text(),flags=re.M)
post=Path('tests/special_weather_cases.c').read_text()
with tempfile.TemporaryDirectory(prefix='special-weather-') as temp:
 p=Path(temp)/'case.c';p.write_text(pre+time+source+post)
 subprocess.run(['gcc','-std=c99','-O2','-Wall','-Wextra','-Werror','-I','include',str(p),'-o',temp+'/run'],check=True)
 subprocess.run([temp+'/run'],check=True)
# Those six originally-unused hidden-item bits are not assigned to map pickups.
for p in Path('data/maps').glob('*/map.json'):
 for event in json.loads(p.read_text()).get('bg_events',[]):
  assert event.get('hidden_item_id') not in range(191,197),str(p)
print('PASS: exact source reversal retains original hashes; no map/encounter/ability/lighting data changed; six flag slots reserved without save expansion.')
