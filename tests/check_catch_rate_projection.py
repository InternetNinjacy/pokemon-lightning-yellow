#!/usr/bin/env python3
"""Positive/negative source preservation controls; no ROM execution."""
import hashlib
from pathlib import Path
import re
import subprocess
from approved_catch_rate_changes import prior_approved_catch_rates, PR251

BASE = '02240690581eb70d70219024fb7f55da2882308b'
PATH = 'src/data/pokemon/species_info.h'
original = subprocess.check_output(['git', 'show', BASE+':'+PATH], text=True)
assert hashlib.sha256(original.encode()).hexdigest() == 'a82ee89bdd90075a185b1058b9a459480b78c8504bbfc446e275ca350f2e0680'
current = Path(PATH).read_text()
assert len(PR251) == 13
assert prior_approved_catch_rates(current) == original
assert prior_approved_catch_rates(original) == original
main = subprocess.check_output(['git','show','197d85ee1a61ac319f95fa65ab298bb126fe937d:'+PATH],text=True)
assert prior_approved_catch_rates(main) == original
from approved_weather_species_slots import prior_weather_species_source
assert prior_weather_species_source(current) == prior_weather_species_source(original)

def change(text, species, field, value):
    pat = r'(\[SPECIES_'+species+r'\]\s*=\s*\{.*?\.'+field+r' = )\d+(,)'
    result, count = re.subn(pat, lambda m:m[1]+str(value)+m[2],text,count=1,flags=re.S)
    assert count == 1
    return result

controls = 0
for species in [*PR251, 'HERACROSS']:
    try:
        prior_approved_catch_rates(change(current,species,'catchRate',124))
    except AssertionError:
        controls += 1
    else:
        raise AssertionError(('Invalid approved field accepted',species))
try:
    prior_approved_catch_rates(change(current,'ELEKID','catchRate',45))
except AssertionError:
    controls += 1
else:
    raise AssertionError('Partial PR251 delta accepted')
# The projection must leave unrelated edits visible to the frozen downstream
# byte/hash assertions, rather than erase them or substitute a baseline blob.
for species, field, value in [('PIDGEY','catchRate',124),('HERACROSS','baseAttack',126)]:
    assert prior_approved_catch_rates(change(current,species,field,value)) != original
    controls += 1
assert controls == 17
print('PASS: exact14 approved catch fields reconstruct frozen base;3 complete stages and17 negative preservation controls')
