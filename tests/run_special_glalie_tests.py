#!/usr/bin/env python3
"""Source/ABI preservation only; real crypto/capture/storage execute in mGBA."""
import hashlib,json,re
from pathlib import Path
from approved_special_glalie_changes import prior_special_glalie_source
from approved_special_corphish_changes import prior_special_corphish_source
from approved_weather_species_evolution_changes import prior_weather_species_evolution_source
from approved_weather_species_slots import prior_weather_species_source
m=json.loads(Path('tests/specifications/special_glalie_delta.json').read_text())
for path,row in m['files'].items():
    assert hashlib.sha256(prior_weather_species_evolution_source(path,prior_special_corphish_source(path)).encode()).hexdigest()==row['after_sha256'],path
    assert hashlib.sha256(prior_special_glalie_source(path).encode()).hexdigest()==row['before_sha256'],path
# New functions/prototypes only: every old Pokémon record field stays byte-identical.
s=Path('include/pokemon.h').read_text();old=prior_special_glalie_source('include/pokemon.h')
assert s.replace('bool8 SetSpecialEncounterAbility(struct Pokemon *mon, u8 ability);\n','')==old
p=Path('include/constants/pokemon.h').read_text();old=prior_special_glalie_source('include/constants/pokemon.h')
for name,value in re.findall(r'#define (MON_DATA_\w+)\s+(\d+)',old):
    assert re.search(r'#define '+name+r'\s+'+value+r'\b',p),name
assert '#define MON_DATA_INDIVIDUAL_ABILITY       89' in p
assert hashlib.sha256(prior_weather_species_source().encode()).hexdigest()==m['species_sha256']
print('PASS: exact opt-in Glalie delta, unchanged Pokémon layouts/original data IDs/species slots; field89 uses existing checksummed growth filler. Static source audit, not capture/save execution.')
