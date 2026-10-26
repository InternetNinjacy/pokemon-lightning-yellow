#!/usr/bin/env python3
"""Execute production target lookup and item-finish callback with stubbed state.
Not an emulator test of animation, menus, trade networking, or physical RTC.
"""
from pathlib import Path
import json
import hashlib
import re
import subprocess
import tempfile
from run_split_tests import extract_function

support = r'''
#include <stdio.h>
#include <assert.h>
#include "global.h"
#include "pokemon.h"
#include "item.h"
#include "event_data.h"
#include "main.h"
#include "evolution_scene.h"
#include "constants/items.h"
#include "constants/hold_effects.h"
#include "src/data/pokemon/evolution.h"
struct Pokemon gPlayerParty[PARTY_SIZE];
static struct SaveBlock1 saveBlock;
struct SaveBlock1 *gSaveBlock1Ptr = &saveBlock;
static u16 species, held, friendship, level, egg;
static u8 clockHour, clockValid, clockReads;
static unsigned removals, events, begins, bagCount, returnCalls;
static MainCallback mainCallback;
static u8 lastCanStop;
void (*gCB2_AfterEvolution)(void);
u32 GetMonData3(struct Pokemon *mon, s32 field, u8 *data) {
 switch(field) {
 case MON_DATA_SPECIES:return species; case MON_DATA_HELD_ITEM:return held;
 case MON_DATA_LEVEL:return level; case MON_DATA_FRIENDSHIP:return friendship;
 case MON_DATA_IS_EGG:return egg; case MON_DATA_ATK:return 100; case MON_DATA_DEF:return 100;
 default:return 0;
 }
}
void SetMonData(struct Pokemon *mon,s32 field,const void *data) { assert(field==MON_DATA_HELD_ITEM);held=*(const u16 *)data; }
u8 ItemId_GetHoldEffect(u16 item) { return item==ITEM_EVERSTONE ? HOLD_EFFECT_PREVENT_EVOLVE : 0; }
bool32 IsNationalPokedexEnabled(void) {return TRUE;}
bool8 RtcGetHour(u8 *hour) {clockReads++;*hour=clockHour;return clockValid;}
bool8 RemoveBagItem(u16 item,u16 count) {assert(item!=ITEM_NONE && count==1 && bagCount);bagCount--;removals++;return TRUE;}
void ItemUse_SetQuestLogEvent(u8 id,struct Pokemon *mon,u16 item,u16 param) {events++;}
void SetMainCallback2(MainCallback cb) {mainCallback=cb;}
void BeginEvolutionScene(struct Pokemon *mon,u16 target,u8 canStop,u8 slot) {assert(slot==0 && target!=species);lastCanStop=canStop;begins++;}
static void Return(void) {returnCalls++;}
'''
program = support + extract_function(Path('src/pokemon.c').read_text(), 'GetEvolutionTargetSpecies')
program += Path('src/evolution_items.c').read_text()
program += r'''
static unsigned checks;
#define CHECK(x) do {checks++;if(!(x)){fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#x);return 1;}}while(0)
static u16 Target(u8 mode,u16 item) {return GetEvolutionTargetSpecies(&gPlayerParty[0],mode,item);}
int main(void) {
 unsigned i,h,f;
 const u16 trades[][2]={{SPECIES_KADABRA,SPECIES_ALAKAZAM},{SPECIES_MACHOKE,SPECIES_MACHAMP},{SPECIES_GRAVELER,SPECIES_GOLEM},{SPECIES_HAUNTER,SPECIES_GENGAR}};
 const u16 items[][3]={{SPECIES_POLIWHIRL,ITEM_KINGS_ROCK,SPECIES_POLITOED},{SPECIES_SLOWPOKE,ITEM_KINGS_ROCK,SPECIES_SLOWKING},{SPECIES_ONIX,ITEM_METAL_COAT,SPECIES_STEELIX},{SPECIES_SCYTHER,ITEM_METAL_COAT,SPECIES_SCIZOR},{SPECIES_SEADRA,ITEM_DRAGON_SCALE,SPECIES_KINGDRA},{SPECIES_PORYGON,ITEM_UP_GRADE,SPECIES_PORYGON2},{SPECIES_CLAMPERL,ITEM_DEEP_SEA_TOOTH,SPECIES_HUNTAIL},{SPECIES_CLAMPERL,ITEM_DEEP_SEA_SCALE,SPECIES_GOREBYSS}};
 for(i=0;i<4;i++) {
  species=trades[i][0];held=ITEM_NONE;level=39;CHECK(Target(EVO_MODE_NORMAL,0)==SPECIES_NONE);
  level=40;CHECK(Target(EVO_MODE_NORMAL,0)==trades[i][1]);level=41;CHECK(Target(EVO_MODE_NORMAL,0)==trades[i][1]);
  level=5;CHECK(Target(EVO_MODE_TRADE,0)==trades[i][1]);CHECK(Target(EVO_MODE_ITEM_USE,ITEM_MOON_STONE)==SPECIES_NONE);
  held=ITEM_EVERSTONE;CHECK(Target(EVO_MODE_NORMAL,0)==SPECIES_NONE);CHECK(Target(EVO_MODE_TRADE,0)==SPECIES_NONE);held=0;
 }
 for(i=0;i<8;i++) {
  species=items[i][0];held=ITEM_NONE;CHECK(Target(EVO_MODE_TRADE,0)==SPECIES_NONE);
  CHECK(Target(EVO_MODE_ITEM_USE,items[i][1])==items[i][2]);CHECK(held==ITEM_NONE);
  held=items[i][1];CHECK(Target(EVO_MODE_ITEM_CHECK,items[i][1])==items[i][2]);CHECK(held==items[i][1]);
  CHECK(Target(EVO_MODE_ITEM_USE,ITEM_POTION)==SPECIES_NONE);CHECK(held==items[i][1]);
  CHECK(Target(EVO_MODE_TRADE,0)==items[i][2]);CHECK(held==ITEM_NONE); // Original trade consumes held item
  egg=1;CHECK(Target(EVO_MODE_ITEM_USE,items[i][1])==SPECIES_NONE);egg=0;
  held=ITEM_EVERSTONE;CHECK(Target(EVO_MODE_ITEM_USE,items[i][1])==SPECIES_NONE);
  CHECK(Target(EVO_MODE_ITEM_CHECK,items[i][1])==items[i][2]);held=ITEM_NONE;
  // Real finish callback; scene launch is stubbed. Simulate cancellation then success.
  bagCount=3;removals=events=begins=0;
  BeginItemEvolution(&gPlayerParty[0],items[i][2],0,items[i][1],Return);CHECK(begins==1 && bagCount==3 && lastCanStop);
  gCB2_AfterEvolution();CHECK(bagCount==3 && removals==0 && events==0 && mainCallback==Return);
  BeginItemEvolution(&gPlayerParty[0],items[i][2],0,items[i][1],Return);species=items[i][2];
  gCB2_AfterEvolution();CHECK(bagCount==2 && removals==1 && events==1);
  gCB2_AfterEvolution();CHECK(bagCount==2 && removals==1 && events==1); // Replay cannot consume twice
 }
 species=SPECIES_EEVEE;held=0;level=30;
 for(h=0;h<24;h++) for(f=0;f<3;f++) {
  clockHour=h;friendship=f==0?0:f==1?219:220;clockValid=1;clockReads=0;
  CHECK(Target(EVO_MODE_NORMAL,0)==(friendship<220?SPECIES_NONE:h>=12?SPECIES_ESPEON:SPECIES_UMBREON));
  CHECK(clockReads==(friendship>=220?1:0));
  clockValid=0;CHECK(Target(EVO_MODE_NORMAL,0)==SPECIES_NONE);
  CHECK(Target(EVO_MODE_ITEM_USE,ITEM_SUN_STONE)==SPECIES_ESPEON);
  CHECK(Target(EVO_MODE_ITEM_USE,ITEM_MOON_STONE)==SPECIES_UMBREON);
  CHECK(Target(EVO_MODE_ITEM_USE,ITEM_THUNDER_STONE)==SPECIES_JOLTEON);
  CHECK(Target(EVO_MODE_ITEM_USE,ITEM_WATER_STONE)==SPECIES_VAPOREON);
  CHECK(Target(EVO_MODE_ITEM_USE,ITEM_FIRE_STONE)==SPECIES_FLAREON);
 }
 // Stones use the same successful/canceled callback path.
 for(i=0;i<2;i++) {
  u16 target=i?SPECIES_UMBREON:SPECIES_ESPEON;
  u16 item=i?ITEM_MOON_STONE:ITEM_SUN_STONE;
  species=SPECIES_EEVEE;bagCount=2;removals=events=0;
  BeginItemEvolution(&gPlayerParty[0],target,0,item,Return);gCB2_AfterEvolution();CHECK(bagCount==2);
  BeginItemEvolution(&gPlayerParty[0],target,0,item,Return);species=target;gCB2_AfterEvolution();CHECK(bagCount==1 && removals==1);
 }
 species=SPECIES_PIKACHU;BeginItemEvolution(&gPlayerParty[0],SPECIES_RAICHU,0,ITEM_THUNDER_STONE,Return);
 CHECK(!lastCanStop); // Preserve legacy stone cancellation policy, including Eevee's other stones.
 gCB2_AfterEvolution();
 species=SPECIES_PIKACHU;CHECK(Target(EVO_MODE_ITEM_USE,ITEM_KINGS_ROCK)==SPECIES_NONE);
 species=SPECIES_EEVEE;egg=1;CHECK(Target(EVO_MODE_NORMAL,0)==SPECIES_NONE);CHECK(Target(EVO_MODE_ITEM_USE,ITEM_SUN_STONE)==SPECIES_NONE);
 CHECK(sizeof(struct Evolution)==6 && EVOS_PER_MON==7);
 printf("PASS: %u evolution lookup/consumption assertions (production routines, stubbed state/scene)\n",checks);
 return 0;
}
'''
with tempfile.TemporaryDirectory() as temp:
    path=Path(temp)/'evolution.c';path.write_text(program)
    subprocess.run(['gcc','-std=gnu11','-D__INTELLISENSE__','-iquote','.','-iquote','include',str(path),'-o',temp+'/evolution'],check=True)
    subprocess.run([temp+'/evolution'],check=True)

# Separate static wiring/data audit; not executed party UI or animation tests.
items = json.loads(Path('src/data/items.json').read_text())['items']
ids = set(re.findall(r'{EVO_TRADE_ITEM, (ITEM_\w+),', Path('src/data/pokemon/evolution.h').read_text()))
for item in items:
    if item['itemId'] in ids:
        assert item['type']=='ITEM_TYPE_PARTY_MENU' and item['fieldUseFunc']=='FieldUseFunc_EvoItem', item['itemId']
party=Path('src/party_menu.c').read_text()
callback=extract_function(party,'CB2_UseEvolutionStone')
assert 'BeginItemEvolution(' in callback and 'RemoveBagItem(' not in callback
assert 'GetEvolutionTargetSpecies(' in extract_function(party,'ItemUseCB_EvolutionStone')
assert 'TASK_BIT_CAN_STOP' in Path('src/evolution_scene.c').read_text()
print('PASS: static item routing, deferred consumption and unchanged cancellation-path audits (not UI execution)')

# Restore only the approved Use-menu metadata changes and compare every other
# item property (including all held effects) to the split branch baseline.
normalized = json.loads(Path('src/data/items.json').read_text())
for item in normalized['items']:
    if item['itemId'] in ids:
        item['type'] = 'ITEM_TYPE_BAG_MENU'
        item['fieldUseFunc'] = 'FieldUseFunc_OakStopsYou'
assert hashlib.sha256(json.dumps(normalized, sort_keys=True).encode()).hexdigest() == '14d96833271efdc07a8db346ccf695bf19450e9f57d92a2d40418dff96cd257d'
print('PASS: all other item data and battle held effects match the PR #2 baseline (static audit)')
