#!/usr/bin/env python3
"""SOW-002 source contract; runtime acceptance is a separate native gate."""
import hashlib
from approved_jolteon_umbreon_changes import prior_locked_pair_source
from approved_146_learnset_changes import prior_146_learnset_source
import json
import re
from pathlib import Path

lock = json.loads(Path('tests/specifications/sow002_preservation.json').read_text())
for name, digest in lock['unchanged'].items():
    actual=Path(name).read_bytes()
    if name=='src/data/pokemon/level_up_learnsets.h':actual=prior_locked_pair_source(name,prior_146_learnset_source(name,actual.decode())).encode()
    assert hashlib.sha256(actual).hexdigest() == digest, name
forest = json.loads(Path('data/maps/ViridianForest/map.json').read_text())
original = lock['original_forest']
assert forest['object_events'][:-1] == original['object_events']
assert {k:v for k,v in forest.items() if k != 'object_events'} == {k:v for k,v in original.items() if k != 'object_events'}
obj = forest['object_events'][-1]
assert obj == dict(type='object', graphics_id='OBJ_EVENT_GFX_SOW_HERACROSS',
    x=49, y=6, elevation=3, movement_type='MOVEMENT_TYPE_FACE_DOWN',
    movement_range_x=0, movement_range_y=0, trainer_type='TRAINER_TYPE_NONE',
    trainer_sight_or_berry_tree_id='0', script='ViridianForest_EventScript_SowHeracross',
    flag='FLAG_HIDE_SOW_HERACROSS')
s = Path('data/maps/ViridianForest/scripts.inc').read_text()
assert 'goto_if_set FLAG_SOW_HERACROSS_RESOLVED, ViridianForest_EventScript_SowTransitionEnd\n\tclearflag FLAG_HIDE_SOW_HERACROSS' in s
event = s.split('ViridianForest_EventScript_SowHeracross::')[1]
assert 'setwildbattle SPECIES_HERACROSS, 12' in event
assert 'special StartLegendaryBattle\n\twaitstate' in event
for outcome, target in [('CAUGHT','Resolved'),('WON','Resolved'),('RAN','Fled'),('PLAYER_TELEPORTED','Fled')]:
    assert f'goto_if_eq VAR_RESULT, B_OUTCOME_{outcome}, ViridianForest_EventScript_Sow{target}' in event
assert event.count('setflag FLAG_SOW_HERACROSS_RESOLVED') == 1
assert 'goto EventScript_RemoveStaticMon' in event and 'goto EventScript_MonFlewAway' in event
assert not re.search(r'faceplayer|applymovement|setweather|SpecialWeather|SowPrepareScriptedEncounter',event)
npc = s.split('ViridianForest_EventScript_Youngster::')[1].split('ViridianForest_EventScript_Boy::')[0]
assert 'goto_if_set FLAG_SOW_HERACROSS_RESOLVED, ViridianForest_EventScript_SowThanks' in npc
assert 'msgbox ViridianForest_Text_SowWarning' in npc
text = Path('data/maps/ViridianForest/text.inc').read_text()
warning = text.split('ViridianForest_Text_SowWarning::')[1].split('ViridianForest_Text_SowThanks::')[0]
assert 'HERACROSS' not in warning and 'unusually strong' in warning and 'northeast' in warning
assert 'safely again' in text.split('ViridianForest_Text_SowThanks::')[1]
pics = Path('src/data/object_events/object_event_pic_tables.h').read_text().split('sPicTable_SowHeracross[] = {')[1].split('};')[0]
assert pics.count('{gMonIcon_Heracross, 512}') == 9 and '+' not in pics
gfx = Path('src/data/object_events/object_event_graphics_info.h').read_text().split('gObjectEventGraphicsInfo_SowHeracross = {')[1].split('};')[0]
assert '.inanimate = TRUE' in gfx and '.paletteTag = OBJ_EVENT_PAL_TAG_SOW_ICON_2' in gfx
movement = Path('src/event_object_movement.c').read_text()
assert '{gMonIconPalettes[2],                     OBJ_EVENT_PAL_TAG_SOW_ICON_2}' in movement
assert '[SPECIES_HERACROSS]   = 2' in Path('src/pokemon_icon.c').read_text()
species = Path('src/data/pokemon/species_info.h').read_text().split('[SPECIES_HERACROSS] =')[1].split('[SPECIES_')[0]
assert '.catchRate = 125,' in species
all_species = Path('src/data/pokemon/species_info.h').read_text()
normalized = all_species.replace(species, species.replace('.catchRate = 125,', '.catchRate = 45,'), 1)
assert hashlib.sha256(normalized.encode()).hexdigest() == lock['species_info_before_catch_delta_sha256']
learnset = Path('src/data/pokemon/level_up_learnsets.h').read_text().split('sHeracrossLevelUpLearnset[] = {')[1].split('};')[0]
assert re.findall(r'LEVEL_UP_MOVE\(\s*(\d+), (MOVE_\w+)\)',learnset)[:4] == [('1','MOVE_TACKLE'),('1','MOVE_LEER'),('6','MOVE_HORN_ATTACK'),('11','MOVE_ENDURE')]
runner = Path('tests/emulator/runner.c').read_text()
selector = runner.split('if(getenv("LIGHTNING_SOW002_ONLY"))')[1].split('if(getenv(')[0]
assert 'sow002Tests();' in selector and 'return 0;' in selector
assert selector.index('sow002Tests();') < selector.index('return 0;')
assert runner.index('LIGHTNING_SOW002_ONLY') < runner.index('LIGHTNING_BALL_WEATHER_ONLY')
assert runner.count('    sow002Tests();') == 2 # focused + preserved default coverage
print('PASS: SOW-002 exact placement/data/script/art contract; original objects, terrain, weather/battle paths and learnsets preserved')
