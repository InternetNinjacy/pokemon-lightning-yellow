#!/usr/bin/env python3
"""Approved exact E4 parties and frozen unrelated trainer/source preservation."""
from pathlib import Path
import json,re,hashlib
from approved_elite_four_changes import prior_elite_four_source
from approved_thunder_wave_changes import prior_thunder_wave_source
from approved_ordinary_trainer_projection import restore_ordinary_trainer_blocks
from approved_weather_species_slots import prior_weather_species_source
from approved_six_species_profile_projection import prior_six_species_profile_source
m=json.loads(Path('tests/specifications/approved_elite_four_delta.json').read_text())
for p,v in m['files'].items():
 source=restore_ordinary_trainer_blocks(p)
 # The six-battle frozen baseline must still match after reversing ONLY the two Agatha additions.
 six_battle_source=source
 for h in v['hunks'][6:]:
  assert six_battle_source.count(h['after'])==1,(p,'new approved delta')
  six_battle_source=six_battle_source.replace(h['after'],h['before'],1)
 assert hashlib.sha256(six_battle_source.encode()).hexdigest()==v['after_sha256'],p
 assert hashlib.sha256(prior_elite_four_source(p).encode()).hexdigest()==v['before_sha256'],p
for p,h in m['preserved_files'].items():
 source=(restore_ordinary_trainer_blocks(p) if p=='src/data/trainers.h' else Path(p).read_text())
 source=prior_thunder_wave_source(p,source)
 if p=='src/data/pokemon/species_info.h':
  source=prior_weather_species_source(source)
  source=prior_six_species_profile_source(source)
 if p=='src/battle_main.c':
  approved_selector='''                // Use the existing native ability slot field only for Agatha's Alakazam.
                // This preserves its original PID, nature, gender and species ability table.
                if ((trainerNum == TRAINER_ELITE_FOUR_AGATHA || trainerNum == TRAINER_ELITE_FOUR_AGATHA_2)
                 && partyData[i].species == SPECIES_ALAKAZAM)
                {
                    u8 abilityNum = 1; // Inner Focus, the native second ability.
                    SetMonData(&party[i], MON_DATA_ABILITY_NUM, &abilityNum);
                }
'''
  assert source.count(approved_selector)==1,'Agatha native ability setter changed'
  source=source.replace(approved_selector,'',1)
 assert hashlib.sha256(source.encode()).hexdigest()==h,p

spec=json.loads(Path('tests/specifications/approved_elite_four.json').read_text())['battles'];assert len(spec)==8
source=Path('src/data/trainer_parties.h').read_text()
for b in spec:
 assert hashlib.sha256(Path(b['specification']).read_bytes()).hexdigest()==b['sha256']
 assert b['implemented'],b['trainer']
 body=re.search(r'\b'+b['array']+r'\[\] = \{(.*?)\n\};',source,re.S)[1]
 entries=re.findall(r'\{(.*?)\n    \}',body,re.S);assert len(entries)==len(b['party'])
 for text,row in zip(entries,b['party']):
  for field,v in [('iv',row['iv']),('lvl',row['level']),('species','SPECIES_'+row['species']),('heldItem','ITEM_'+row['item'])]:
   assert re.search(r'\.'+field+r' = ([^,]+),',text)[1]==str(v)
  assert re.findall(r'MOVE_(\w+)',text)==row['moves']
print('PASS: all eight exact approved Elite Four party arrays; prior six-battle baseline preserved; scoped Agatha native ability field setter verified against frozen constructor')

constructor=json.loads(Path('tests/specifications/approved_agatha_constructor_delta.json').read_text())
for p,v in constructor['files'].items():
 assert hashlib.sha256(prior_thunder_wave_source(p).encode()).hexdigest()==v['after_sha256'],p
 assert hashlib.sha256(prior_elite_four_source(p).encode()).hexdigest()==v['before_sha256'],p
print('PASS: exact draft Agatha native-field selector is reversed consistently for every historical constructor audit; no old digest replaced')
