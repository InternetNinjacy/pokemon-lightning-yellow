#!/usr/bin/env python3
from approved_kanto_water_changes import prior_water_batch_source
from approved_museum_changes import prior_museum_source
"""Executable production RNG mapping and separate static scope checks.

These are not emulator or battle tests. Native transaction/UI evidence comes
from tests/emulator/run.py; no duplicated simulated purchase model is used here.
"""
from pathlib import Path
import hashlib
import json
import re
import subprocess
import tempfile
from run_split_tests import extract_function

source = Path('src/mystery_vendor.c').read_text()
program = '''#include <stdint.h>
#include <assert.h>
#include <stdio.h>
typedef uint16_t u16; typedef uint32_t u32;
static unsigned cursor,calls;
static u16 Random(void) { calls++; return (u16)cursor++; }
''' + extract_function(source, 'MysteryRandomBelow') + '''
int main(void) {
 unsigned checks=0;
 cursor=calls=0; assert(MysteryRandomBelow(0)==0 && calls==0); checks++;
 for(unsigned n=1;n<=10;n++) {
  unsigned bins[10]={0},limit=65536/n*n;cursor=calls=0;
  for(unsigned j=0;j<limit;j++){unsigned x=MysteryRandomBelow(n);assert(x<n);bins[x]++;checks++;}
  for(unsigned j=0;j<n;j++){assert(bins[j]==limit/n);checks++;}
  assert(calls==limit);checks++;
  assert(MysteryRandomBelow(n)==0);assert(calls==65537);checks+=2;
 }
 printf("PASS: %u production RNG mapping assertions; exhaustive16-bit accepted intervals, all remainders and rejection/wrap, bounds0..10\\n",checks);
}
'''
with tempfile.TemporaryDirectory(prefix='lightning-vendor-') as temp:
    path=Path(temp)/'random.c';path.write_text(program)
    binary=Path(temp)/'random'
    subprocess.run(['gcc','-std=c11','-Wall','-Wextra','-Werror',str(path),'-o',str(binary)],check=True)
    subprocess.run([str(binary)],check=True)

fixture=json.loads(Path('tests/specifications/mystery_vendor_preserved.json').read_text())
for path,digest in fixture['files'].items():
    assert hashlib.sha256(prior_water_batch_source(path)).hexdigest()==digest,path
blocks=lambda s:dict(re.findall(r'^(\w+)::\n(.*?)(?=^\w+::|\Z)',s,re.M|re.S))
for path,items in fixture['blocks'].items():
    current=blocks(Path(path).read_text())
    for label,digest in items.items():
        assert hashlib.sha256(current[label].strip().encode()).hexdigest()==digest,(path,label)
specials=prior_museum_source('data/specials.inc')
assert re.findall(r'\bdef_special\s+(\w+)',specials)==fixture['specials']+['MysteryVendorStatus','PurchaseMysteryPokemon']
assert specials.index('def_special PurchaseMysteryPokemon') < specials.index('gSpecialsEnd::')
sites=json.loads(Path('tests/specifications/mystery_vendor_sites.json').read_text())['vendors']
for v in sites:
    data=json.loads(Path('data/maps/'+v['map']+'/map.json').read_text())
    assert data['object_events'][v['local_id']-1]==v['object'],v['map']
    body=blocks(Path('data/maps/'+v['map']+'/scripts.inc').read_text())[v['entry']]
    assert re.search(r'setvar VAR_0x8009, '+str(v['id'])+r'\b',body)
    assert 'goto MysteryVendor_Start' in body
flags=Path('include/constants/flags.h').read_text()
newflags=dict(re.findall(r'^#define\s+(FLAG_MYSTERY_(?:VENDOR|SPECIES)_\d+)\s+(0x[0-9A-Fa-f]+)\s*$',flags,re.M))
assert len(newflags)==19 and {int(x,16) for x in newflags.values()}==set(range(0x300,0x313))
assert '#define FLAG_MYSTERY_VENDOR_01 FLAG_BOUGHT_MAGIKARP' in flags
# No other named event flag may consume these persistent bits.
for name,value in re.findall(r'^#define\s+(FLAG_\w+)\s+(0x[0-9A-Fa-f]+)\s*$',flags,re.M):
    if 0x300<=int(value,16)<=0x312: assert name in newflags,name
pool=re.search(r'sSpecies\[.*?=\s*\{(.*?)\};',source,re.S)[1]
assert re.findall(r'SPECIES_(\w+)',pool)==['BULBASAUR','SQUIRTLE','CHARMANDER','KANGASKHAN','PINSIR','FEEBAS','CHANSEY','DRATINI','ABSOL','MAGIKARP']
print('PASS: static original NPC objects/other script blocks, save layout, starters, battle/daycare/species/world data, appended dispatch IDs and19 reserved flags')

# Independently transcribed approved handoff wording, not generated from source.
intros=[
"Hey, kid! Have I got a deal for you! A fine POKéMON for just ¥700! What kind? Heh heh... That's the surprise! Interested?",
"I've collected quite a few interesting POKéMON over the years. I'm letting one go for just ¥700. You won't know which one until it's yours. Care to take a chance?",
"With so many TRAINERS passing through the port, you never know what kind of POKéMON you'll come across. I've got one for sale. Just ¥700! Would you like it?",
"Everyone's looking for valuable NUGGETS, but I've got something much more interesting! A mystery POKéMON for ¥700! Want to see what you get?",
"I've been traveling with an unusual POKéMON, but I can't keep caring for it. For ¥700, it's yours. I won't tell you what it is until you agree. Interested?",
"Bah! I've lost enough money at those slot machines! Maybe you'll have better luck with this. ¥700 gets you one mystery POKéMON. Feeling lucky?",
"There are all sorts of fascinating POKéMON in this world! I happen to have one available for ¥700. No telling what you'll get! Want one?",
"Now that TEAM ROCKET is gone, it's good to see TRAINERS around again. I've got a POKéMON that needs a new home. ¥700, if you're interested!",
"The scientists around here sure have some remarkable POKéMON! I've got an interesting one myself. It's yours for ¥700, but its identity is a secret. Deal?",
"People come to these islands from all over. And they bring all sorts of POKéMON! I've got one to sell for ¥700. Care to try your luck?",
]
shared={'Declined':'No problem. The offer stands!', 'Poor':"Sorry, you'll need ¥700.",
'NoRoom':"You don't have room for another POKéMON. Make some space and come back!",
'Reveal':"Surprise! It's {STR_VAR_1}!",'Nickname':'Would you like to give {STR_VAR_1} a nickname?',
'AlreadySold':'I hope you and your new POKéMON get along!'}
script=Path('data/scripts/mystery_vendor.inc').read_text()
assert 'VAR_TEMP_' not in source and 'VAR_TEMP_' not in script, 'Never clobber Cinnabar Gym/map state for a gift species'
texts=blocks(script)
encoding={c:int(h,16) for c,h in re.findall(r"^'(.)'\s*=\s*([0-9A-F]{2})\s*$",Path('charmap.txt').read_text(),re.M)}
encoding["'"]=encoding['’']
widths=list(map(int,re.findall(r'\d+',re.search(r'sFontNormalLatinGlyphWidths\[\]\s*=\s*\{(.*?)\};',Path('src/text.c').read_text(),re.S)[1])))
maxwidth=0
for suffix,expected in list(shared.items())+[(f'Intro{i:02}',text) for i,text in enumerate(intros,1)]:
    text=''.join(re.findall(r'\.string "(.*)"',texts['MysteryVendor_Text_'+suffix]))
    normalized=re.sub(r'\\[nlp]',' ',text).removesuffix('$')
    assert normalized==expected,(suffix,normalized,expected)
    for line in re.split(r'\\[nlp]',text.removesuffix('$')):
        # Worst-case name is KANGASKHAN (10 uppercase glyphs).
        line=line.replace('{STR_VAR_1}','KANGASKHAN')
        width=sum(widths[encoding[c]] for c in line)
        assert width<=208,(suffix,line,width)
        maxwidth=max(maxwidth,width)
print(f'PASS: approved10 intro/shared wording and actual normal-font line widths, maximum{maxwidth}px <=208px (including longest species)')
