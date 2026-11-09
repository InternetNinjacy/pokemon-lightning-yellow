#!/usr/bin/env python3
"""Strict source/namespace preservation; actual mechanics execute in native mGBA."""
import hashlib
import json
import re
from pathlib import Path
from approved_weather_ability_changes import prior_weather_ability_source
from approved_special_glalie_changes import prior_special_glalie_source
from approved_weather_species_slots import prior_weather_species_source

manifest = json.loads(Path('tests/specifications/weather_ability_delta.json').read_text())
for path, row in manifest['files'].items():
    assert hashlib.sha256(prior_special_glalie_source(path).encode()).hexdigest() == row['after_sha256'], path
    assert hashlib.sha256(prior_weather_ability_source(path).encode()).hexdigest() == row['before_sha256'], path
constants = Path('include/constants/abilities.h').read_text()
prior = prior_weather_ability_source('include/constants/abilities.h')
old = dict(re.findall(r'#define (ABILITY_\w+) (\d+)', prior))
current = dict(re.findall(r'#define (ABILITY_\w+) (\d+)', constants))
assert len(current) == 81 and len(set(current.values())) == 81
assert all(current[k] == v for k, v in old.items())
assert {k for k in current if k not in old} == {'ABILITY_ICE_BODY', 'ABILITY_OVERCOAT', 'ABILITY_SNOW_WARNING'}
text = Path('src/data/text/abilities.h').read_text()
for key in current:
    assert text.count('[' + key + ']') == 2, key
for name in re.findall(r'\[ABILITY_\w+\] = _\("([^"]*)"\)', text):
    assert len(name) <= 12, name
# No allocation or species/save schema changes in this mechanics-only slice.
for path, digest in manifest['preserved'].items():
    if path == 'src/data/pokemon/species_info.h':
        # Normalize only exact owner-locked PKM-009 data; older weather scope
        # preservation still rejects every other species/engine change.
        source = Path(path).read_text()
        from approved_pkm009_ponyta_changes import prior_pkm009_ponyta_source
        source = prior_pkm009_ponyta_source(path, source)
        source = prior_weather_species_source(source)
    else:
        source = prior_special_glalie_source(path)
    assert hashlib.sha256(source.encode()).hexdigest() == digest, path
print('PASS:81 unique ability IDs with all78 original IDs unchanged, complete names/descriptions, exact frozen-source reversals and unchanged species/save data (static; native runtime required).')
