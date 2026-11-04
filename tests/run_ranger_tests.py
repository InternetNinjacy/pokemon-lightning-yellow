#!/usr/bin/env python3
"""Owner data, scene/PC structure and preservation audits; runtime uses real mGBA."""
from pathlib import Path
import json,re,hashlib,subprocess,struct
from approved_ranger_changes import prior_ranger_source
f=json.loads(Path('tests/specifications/approved_ranger_base_camp.json').read_text())
preserved=json.loads(Path('tests/specifications/approved_ranger_delta.json').read_text())
for path in preserved['hunks']:
    actual=prior_ranger_source(path).encode()
    expected=subprocess.check_output(['git','show',preserved['base']+':'+path])
    assert actual==expected,path
for path,snapshot in preserved['binary'].items():
    a=bytearray(Path(path).read_bytes());assert hashlib.sha256(a).hexdigest()==snapshot['after_sha256']
    for offset,before,after in snapshot['changes']:assert a[offset]==after;a[offset]=before
    assert hashlib.sha256(a).hexdigest()==snapshot['before_sha256'],path
parties=Path('src/data/trainer_parties.h').read_text();trainers=Path('src/data/trainers.h').read_text();constants=Path('include/constants/opponents.h').read_text();flags=Path('include/constants/flags.h').read_text()
for i,t in enumerate(f['trainers']):
    assert re.search(r'^#define '+t['constant']+r'\s+'+str(t['id'])+r'\s*$',constants,re.M)
    name='Park' if i==4 else str(i+1)
    body=re.search(r'sParty_RangerCamp'+name+r'\[\] = \{(.*?)\n\};',parties,re.S)[1]
    assert re.findall(r'\.species = SPECIES_(\w+)',body)==[p['species'] for p in t['party']]
    assert list(map(int,re.findall(r'\.lvl = (\d+)',body)))==[p['level'] for p in t['party']]
    assert re.findall(r'\.iv = (\d+)',body)==['100']*len(t['party'])
    assert [[m.strip().removeprefix('MOVE_') for m in x.split(',')] for x in re.findall(r'\.moves = \{(.*?)\}',body)]==[p['moves'] for p in t['party']]
    meta=re.search(r'\['+t['constant']+r'\] = \{(.*?)\n    \},',trainers,re.S)[1]
    assert '.trainerClass = TRAINER_CLASS_PKMN_RANGER' in meta
    assert '.trainerPic = TRAINER_PIC_POKEMON_RANGER_M' in meta
    assert '.items = {ITEM_'+t['bag'][0]+'}' in meta
    assert '.doubleBattle = '+('TRUE' if t['double'] else 'FALSE') in meta
    assert '.party = NO_ITEM_CUSTOM_MOVES(sParty_RangerCamp'+name+')' in meta
assert re.search(r'MAX_TRAINERS_COUNT\s+768',constants)
assert re.search(r'NUM_TRAINERS\s+748',constants)
assert f['trainers'][-1]['id']<768
assert re.search(r'FLAG_RANGER_CAMP_GIFT_RECEIVED\s+0x314',flags)
assert re.search(r'FLAG_RANGER_PARK_INTRO_SEEN\s+0x315',flags)
events=json.loads(Path('data/maps/SaffronCity_Dojo/map.json').read_text());original=json.loads(prior_ranger_source('data/maps/SaffronCity_Dojo/map.json'))
assert events['id']==original['id'] and events['layout']==original['layout']
assert events['warp_events']==original['warp_events'] and len(events['object_events'])==8 and events['coord_events']==[]
for i in range(5):
    a,b=events['object_events'][i],original['object_events'][i]
    assert (a['x'],a['y'],a['movement_type'])==(b['x'],b['y'],b['movement_type'])
    assert a['graphics_id']=='OBJ_EVENT_GFX_CAMPER'
    assert a['trainer_type']==('TRAINER_TYPE_NONE' if i==4 else 'TRAINER_TYPE_NORMAL')
    assert a['trainer_sight_or_berry_tree_id']==('0' if i==4 else '1')
assert {(o['x'],o['y']) for o in events['object_events'][5:]}=={(5,3),(7,3),(6,3)}
assert all(o['flag']=='FLAG_RANGER_CAMP_GIFT_RECEIVED' for o in events['object_events'][5:])
script=Path('data/maps/SaffronCity_Dojo/scripts.inc').read_text();texts=Path('data/maps/SaffronCity_Dojo/text.inc').read_text()
assert 'HITMON' not in script and 'BLACK_BELT' not in script and 'FIGHTING DOJO' not in texts
assert 'trainerbattle_double TRAINER_RANGER_PARK' in script
for i in range(1,5):assert 'goto_if_not_defeated TRAINER_RANGER_CAMP_'+str(i)+', SaffronCity_Dojo_RangerParkLocked' in script
encoding={c:int(h,16) for c,h in re.findall(r"^'(.)'\s*=\s*([0-9A-F]{2})\s*$",Path('charmap.txt').read_text(),re.M)};encoding["'"]=encoding['’']
widths=list(map(int,re.findall(r'\d+',re.search(r'sFontNormalLatinGlyphWidths\[\]\s*=\s*\{(.*?)\};',Path('src/text.c').read_text(),re.S)[1])))
maxwidth=0
for name,expected in {**f['dialogue'],**f['technical_prompts']}.items():
    block=re.search(r'^SaffronCity_Dojo_Text_'+name+r'::\n(.*?)(?=^\w+::|\Z)',texts,re.M|re.S)[1]
    encoded=''.join(re.findall(r'\.string "(.*?)"',block))
    assert re.sub(r'\\[nlp]',' ',encoded).removesuffix('$')==expected.replace('\n\n',' '),name
    for line in re.split(r'\\[nlp]',encoded.removesuffix('$')):
        n=sum(widths[encoding[c]] for c in line.replace('{STR_VAR_1}','ARCANINE'));assert n<=208,(name,line,n);maxwidth=max(maxwidth,n)
# The PC is a pre-existing primary tile with the real MB_PC behavior; it uses native code.
a=Path('data/tilesets/primary/building/metatile_attributes.bin').read_bytes();assert struct.unpack_from('<I',a,0x62*4)[0]&0x1ff==0x83
print('PASS: five approved Ranger parties/12 Pokemon, fresh trainer flags/no save expansion, native PC tile, all locked dialogue/font widths <=',maxwidth,'and exact old-source preservation (static)')
