#!/usr/bin/env python3
"""Execute production ball policy; separately audit item/stock/art invariants."""
from pathlib import Path
import json,re,subprocess,tempfile
from run_split_tests import extract_function
s=Path('src/capture_rules.c').read_text()
code='''#include <stdint.h>
#include <assert.h>
#include <stdio.h>
typedef uint8_t u8; typedef uint16_t u16; typedef u8 bool8;
#define ITEM_SNOW_BALL 375
#define CAPTURE_CODE_SNOW 13
#include "constants/items.h"
#include "constants/species.h"
#include "constants/battle.h"
'''
for name in ['IsPokeBallItem','BallItemToCaptureCode','CaptureCodeToBallItem','GetBallCatchMultiplier']:code+=extract_function(s,name)
code+='''
int main(void) {
 unsigned checks=0;
 for(unsigned species=0;species<413;species++) {
  unsigned special=species==SPECIES_ARTICUNO||species==SPECIES_ZAPDOS||species==SPECIES_MOLTRES||species==SPECIES_RAIKOU||species==SPECIES_ENTEI||species==SPECIES_SUICUNE||species==SPECIES_MEWTWO||species==SPECIES_MEW;
  assert(GetBallCatchMultiplier(ITEM_PREMIER_BALL,species,5,0,0,0,0,0)==(special?35:10));checks++;
 }
 for(unsigned weather=0;weather<512;weather++)for(unsigned origin=0;origin<2;origin++)for(unsigned cave=0;cave<2;cave++) {
  assert(GetBallCatchMultiplier(ITEM_DIVE_BALL,1,5,0,0,weather,origin,cave)==((origin||(weather&(B_WEATHER_RAIN|B_WEATHER_HAIL)))?30:10));
  assert(GetBallCatchMultiplier(ITEM_NET_BALL,1,5,0,0,weather,origin,cave)==((weather&(B_WEATHER_SUN|B_WEATHER_SANDSTORM))?30:10));
  assert(GetBallCatchMultiplier(ITEM_SNOW_BALL,1,5,0,0,weather,origin,cave)==((cave||(weather&B_WEATHER_HAIL))?30:10));checks+=3;
 }
 for(unsigned item=0;item<ITEMS_COUNT;item++) {
  unsigned ball=(item>=1&&item<=12)||item==ITEM_SNOW_BALL;assert(IsPokeBallItem(item)==ball);checks++;
  if(ball){unsigned raw=BallItemToCaptureCode(item);assert(raw<16);assert(CaptureCodeToBallItem(raw)==item);checks+=2;}
 }
 for(unsigned level=1;level<=100;level++){assert(GetBallCatchMultiplier(ITEM_NEST_BALL,1,level,0,0,0,0,0)==(level<30?40-level:10));checks++;}
 for(unsigned turn=0;turn<256;turn++){u8 old=turn+10;assert(GetBallCatchMultiplier(ITEM_TIMER_BALL,1,5,turn,0,0,0,0)==(old>40?40:old));checks++;}
 printf("PASS: %u production capture-policy assertions (host; not whole battle execution)\\n",checks);
}
'''
with tempfile.TemporaryDirectory(prefix='lightning-balls-') as d:
 p=Path(d)/'capture.c';p.write_text(code);out=Path(d)/'capture'
 subprocess.run(['gcc','-std=c11','-Wall','-Werror','-iquote','include',str(p),'-o',str(out)],check=True);subprocess.run([str(out)],check=True)
items=json.loads(Path('src/data/items.json').read_text())['items'];assert len(items)==376
items={x['itemId']:x for x in items}
for name in ['NET','DIVE','NEST','REPEAT','TIMER','LUXURY','SNOW']:assert items['ITEM_'+name+'_BALL']['price']==1200
assert items['ITEM_NET_BALL']['english']=='DUST BALL' and items['ITEM_DIVE_BALL']['english']=='WET BALL'
assert items['ITEM_SNOW_BALL']['english']=='SNOW BALL'
for name,price in [('POKE',200),('GREAT',600),('ULTRA',1200)]:assert items['ITEM_'+name+'_BALL']['price']==price
names=['VermilionCity_Mart','LavenderTown_Mart','CeladonCity_DepartmentStore_2F','FuchsiaCity_Mart','SaffronCity_Mart','CinnabarIsland_Mart','IndigoPlateau_PokemonCenter_1F','ThreeIsland_Mart','FourIsland_Mart','SixIsland_Mart','SevenIsland_Mart']
for name in names:
 text=Path('data/maps/'+name+'/scripts.inc').read_text();body=re.search('^'+name+r'_Items::\n(.*?)(?=^\w+::|\Z)',text,re.M|re.S)[1]
 for ball in ['NET','DIVE','NEST','REPEAT','TIMER','LUXURY','SNOW']:assert body.count('ITEM_'+ball+'_BALL')==1,(name,ball)
 for ball in ['MASTER','SAFARI','PREMIER']:assert 'ITEM_'+ball+'_BALL' not in body
assert '[ITEM_SNOW_BALL] = {gItemIcon_DiveBall, gItemIconPalette_DiveBall}' in Path('src/data/item_icon_table.h').read_text()
assert 'case ITEM_SNOW_BALL:\n        return BALL_DIVE;' in Path('src/battle_anim_special.c').read_text()
print('PASS: static376 item identities/prices,11 complete city/League stocks, no forbidden ball sales there, Snow shares Dive icon/animation mapping; native UI/runtime separate')
