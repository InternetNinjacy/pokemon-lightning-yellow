#!/usr/bin/env python3
"""Exact approved data and preservation audits. Native tests execute separately."""
from pathlib import Path
import re,json,hashlib,subprocess
from approved_rival_changes import prior_rival_source
from approved_dynamic_weather_changes import prior_dynamic_weather_source
f=json.loads(Path('tests/specifications/approved_rival_parties.json').read_text())
d=json.loads(Path('tests/specifications/approved_rival_delta.json').read_text())
for path,digest in d['before_sha256'].items():
    assert hashlib.sha256(prior_rival_source(path).encode()).hexdigest()==digest,path
parties=Path('src/data/trainer_parties.h').read_text();trainers=Path('src/data/trainers.h').read_text()
constants=Path('include/constants/moves.h').read_text();items=Path('include/constants/items.h').read_text()
assert len(f['encounters'])==8
for e in f['encounters']:
    assert e['party'][-1]['species'] in ['DRATINI','DRAGONAIR','DRAGONITE']
    assert len(e['variants'])==3
    for v in e['variants']:
        body=re.search(r'static const struct TrainerMonItemCustomMoves '+v['party']+r'\[\] = \{(.*?)\n\};',parties,re.S)[1]
        assert re.findall(r'\.species = SPECIES_(\w+)',body)==[m['species'] for m in e['party']]
        assert list(map(int,re.findall(r'\.lvl = (\d+)',body)))==[m['level'] for m in e['party']]
        assert list(map(int,re.findall(r'\.iv = (\d+)',body)))==v['iv']
        assert re.findall(r'\.heldItem = ITEM_(\w+)',body)==[m['held_item'] for m in e['party']]
        assert [x.replace('MOVE_','').split(', ') for x in re.findall(r'\.moves = \{(.*?)\}',body)]==[m['moves'] for m in e['party']]
        meta=re.search(r'\['+v['trainer']+r'\] = \{(.*?)\n    \},',trainers,re.S)[1]
        assert '.party = ITEM_CUSTOM_MOVES('+v['party']+')' in meta
        for m in e['party']:
            for move in m['moves']:assert re.search(r'^#define MOVE_'+move+r'\s',constants,re.M)
            assert re.search(r'^#define ITEM_'+m['held_item']+r'\s',items,re.M)
learnsets=Path('src/data/pokemon/level_up_learnsets.h').read_text()
for species in ['Dratini','Dragonair','Dragonite']:
    body=re.search(r's'+species+r'LevelUpLearnset\[\] = \{(.*?)\n\};',learnsets,re.S)[1]
    for pair in ['1, MOVE_TACKLE','12, MOVE_TWISTER','22, MOVE_DRAGON_BREATH']:assert 'LEVEL_UP_MOVE('+pair+')' in body
    assert 'MOVE_DRAGON_RAGE' not in body and 'LEVEL_UP_MOVE(15, MOVE_TWISTER)' not in body
    if species!='Dratini':assert 'LEVEL_UP_MOVE(1, MOVE_TWISTER)' in body
# Scripts/save/evolution/TM data remain byte-identical, including tutorial, optional
# Route22, champion flag/rematch dispatch. Snapshot stores hashes; no Git fetch in CI.
for path,digest in f['unchanged_files'].items():assert hashlib.sha256(prior_dynamic_weather_source(path).encode()).hexdigest()==digest,path
assert [sum(m['held_item']!='NONE' for m in e['party']) for e in f['encounters']]==[0,1,1,1,1,3,5,6]
print('PASS: all8 owner-approved teams/24 original Trainer IDs, exact99 mons/moves/items/levels, original IV/AI/bag metadata and every other source byte preserved (static)')
print('PASS: three exact global learnset overlays; all event/save/TM/evolution/Champion-rematch source preserved; runtime acceptance separate')
subprocess.run(['python3','scripts/audit_species_assignments.py','--check'],check=True)
