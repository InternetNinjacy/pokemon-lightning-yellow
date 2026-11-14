#!/usr/bin/env python3
"""Exact approved parties/metadata/maps plus byte preservation. Not battle tests."""
from pathlib import Path
import subprocess,sys
import json,re,hashlib
from approved_gym_castform_changes import prior_gym_castform_source
from approved_six_species_profile_projection import prior_six_species_profile_source
f=json.loads(Path('tests/specifications/approved_gym_castform.json').read_text())
d=json.loads(Path('tests/specifications/approved_gym_castform_delta.json').read_text())
def historical_gym_source(path):
 source=prior_gym_castform_source(path)
 if path=='src/data/pokemon/species_info.h':
  source=prior_six_species_profile_source(source)
 return source
for p,h in d['before_sha256'].items():
 assert hashlib.sha256(historical_gym_source(p).encode()).hexdigest()==h,p
for p,h in d['unchanged_sha256'].items():
 assert hashlib.sha256(historical_gym_source(p).encode()).hexdigest()==h,p
parties=Path('src/data/trainer_parties.h').read_text();meta=Path('src/data/trainers.h').read_text();opponents=Path('include/constants/opponents.h').read_text()
for t in f['trainers']:
 assert re.search(r'^#define '+t['constant']+r'\s+'+str(t['id'])+r'$',opponents,re.M)
 body=re.search(r'sParty_'+t['party']+r'\[\] = \{(.*?)\n\};',parties,re.S)[1]
 assert re.findall(r'\.species = SPECIES_(\w+)',body)==[x[0] for x in t['pokemon']]
 assert list(map(int,re.findall(r'\.lvl = (\d+)',body)))==[x[1] for x in t['pokemon']]
 assert [[x.strip().removeprefix('MOVE_') for x in moves.split(',')] for moves in re.findall(r'\.moves = \{(.*?)\}',body)]==[x[2] for x in t['pokemon']]
 m=re.search(r'\['+t['constant']+r'\] = \{(.*?)\n    \},',meta,re.S)[1]
 assert '.doubleBattle = '+('TRUE' if t['double'] else 'FALSE') in m
 assert '.items = {}' in m and '.party = NO_ITEM_CUSTOM_MOVES(sParty_'+t['party']+')' in m
 mapdata=json.loads(Path('data/maps/'+t['map']+'/map.json').read_text());assert mapdata['weather']==t['weather']
 assert mapdata['map_type']=='MAP_TYPE_INDOOR'
script=Path('data/maps/CeruleanCity_Gym/scripts.inc').read_text()
assert 'trainerbattle_double TRAINER_PICNICKER_DIANA, CeruleanCity_Gym_Text_DianaIntro, CeruleanCity_Gym_Text_DianaDefeat, SaffronCity_Dojo_Text_NeedTwo' in script
# Everything outside the six frozen deltas matches weather dependency exactly,
# including every other party/leader/AI/IV, Castform learnset and original events.
print('PASS: exact six approved Pokemon/levels/24 move slots, unchanged Trainer IDs/IV/AI and unrelated parties; ordinary no held/bag items; native Diana Double command/refusal and three map weather headers (static).')
print('Runtime Forecast, Weather Ball, weather persistence, targeting and event completion require mGBA; this audit does not substitute for them.')

# Historical source-only all-252 TM/HM parity guard; not TM51 gameplay.
subprocess.run([sys.executable, 'tests/test_pkm009_tmhm_252_source.py'], check=True)
