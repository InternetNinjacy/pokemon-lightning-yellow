#!/usr/bin/env python3
"""PKM-007 source/preservation checks; runtime coverage is in mGBA, not simulated here."""
import hashlib,json,re
from pathlib import Path
source=Path('src/data/pokemon/species_info.h').read_text()
load=lambda name:json.loads(Path('tests/specifications/'+name+'.json').read_text())
rules=load('approved_type_overrides')['species']
delta=load('approved_typing_delta')['species']
preserved=load('typing_preserved')
assert len(rules)==35 and len(delta)==24
restored=source
for name,rule in rules.items():
    pattern=r'(\[SPECIES_'+name+r'\]\s*=\s*\{.*?\.types = \{)(TYPE_\w+), (TYPE_\w+)(\},)'
    match=re.search(pattern,source,re.S)
    assert match and [match[2][5:],match[3][5:]]==rule['approved'],name
    if name in delta:
        assert delta[name]['approved']==rule['approved'] and delta[name]['before']!=rule['approved']
        restored,n=re.subn(pattern,lambda m:m[1]+', '.join('TYPE_'+t for t in delta[name]['before'])+m[4],restored,count=1,flags=re.S)
        assert n==1
assert hashlib.sha256(restored.encode()).hexdigest()==preserved['original_species_file_sha256']
normalized=re.sub(r'\.types = \{[^}]+\}', '.types = {TYPES}',source)
assert hashlib.sha256(normalized.encode()).hexdigest()==preserved['species_non_type_sha256']
print('PASS:35 effective typing rules; exactly24 approved type-pair changes; every other species field preserved')
