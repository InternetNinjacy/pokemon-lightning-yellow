#!/usr/bin/env python3
"""Source-exact weather species slots; native ability/evolution behavior is tested in mGBA."""
import hashlib
import json
import re
from pathlib import Path
from approved_weather_species_slots import SLOTS, prior_weather_species_source
from approved_weather_species_evolution_changes import (
    BEFORE, AFTER, prior_weather_species_evolution_source,
)

source = Path('src/data/pokemon/species_info.h').read_text()
assert len(SLOTS) == 16
for name, (old, approved) in SLOTS.items():
    match = re.search(
        r'\[SPECIES_' + name + r'\]\s*=\s*\{.*?\.abilities = \{(ABILITY_\w+), (ABILITY_\w+)\}',
        source, re.S
    )
    assert match and match.groups() == approved, (name, match.groups() if match else None)
# The upstream source, including the earlier 35 approved typing changes and every
# unrelated species field, must be byte-identical after reversing these 16 changes.
# Reverse only exact currently authorized PKM-009 species-row changes before
# checking the frozen pre-weather snapshot; other fields remain guarded.
from approved_pkm009_ponyta_changes import prior_pkm009_ponyta_source
source_for_hash = prior_pkm009_ponyta_source('src/data/pokemon/species_info.h', source)
prior = prior_weather_species_source(source_for_hash)
prior_hash = json.loads(Path('tests/specifications/special_glalie_delta.json').read_text())['species_sha256']
assert hashlib.sha256(prior.encode()).hexdigest() == prior_hash
for name, required in {
    'GYARADOS': ('ABILITY_INTIMIDATE', 'ABILITY_NONE'),
    'MILOTIC': ('ABILITY_MARVEL_SCALE', 'ABILITY_NONE'),
    'POLIWRATH': ('ABILITY_WATER_ABSORB', 'ABILITY_DAMP'),
    'CRAWDAUNT': ('ABILITY_HYPER_CUTTER', 'ABILITY_SHELL_ARMOR'),
}.items():
    match = re.search(r'\[SPECIES_' + name + r'\]\s*=\s*\{.*?\.abilities = \{(ABILITY_\w+), (ABILITY_\w+)\}', source, re.S)
    assert match and match.groups() == required, name
mon = Path('src/pokemon.c').read_text()
assert mon.count(AFTER) == 1
assert prior_weather_species_evolution_source('src/pokemon.c', mon).count(BEFORE) == 1
assert 'SetSpecialEncounterAbility(struct Pokemon *mon, u8 ability)' in mon
print('PASS: all 16 exact owner-approved slot pairs, 4 excluded evolutions/unrelated species, original source SHA preserved through reversible normalization; native acceptance required.')
