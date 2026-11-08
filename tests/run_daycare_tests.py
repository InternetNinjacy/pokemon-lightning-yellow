#!/usr/bin/env python3
from approved_typing_changes import prior_typing_source
"""Reviewed migration/Shiny policy, approved PP hook normalized; stubbed save/RNG state.
Current PP/crypto/save behavior executes separately in native acceptance.
Real breeding/crypto/hatching/scenes run separately in the compiled-ROM mGBA suite.
"""
from pathlib import Path
import subprocess, tempfile, json
from run_split_tests import extract_function
from approved_move_pp_changes import prior_move_pp_source
# Retain original migration-policy fixtures; new PP/crypto behavior runs natively.
policy=prior_move_pp_source('src/route5_daycare.c')
# Host pointer widths differ from the GBA. Production target ABI assertions stay
# enabled in the ROM build; this host executable extracts only policy functions.
functions='\n'.join(extract_function(policy,n) for n in ['GetRoute5Daycare','IsRoute5DaycareReady','IsRoute5Daycare','GetActiveDaycare','Route5EggPersonality','SetRoute5PendingEgg'])

program=r'''
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "route5_daycare.h"
#include "pokemon.h"
#include "constants/maps.h"
static struct SaveBlock1 save1;
static struct SaveBlock2 save2;
struct SaveBlock1 *gSaveBlock1Ptr=&save1;
struct SaveBlock2 *gSaveBlock2Ptr=&save2;
static u32 seed;
static unsigned rngCalls;
u16 Random(void) {seed=1103515245*seed+24691;rngCalls++;return seed>>16;}
'''+functions+r'''
static unsigned checks;
#define CHECK(x) do {checks++;if(!(x)){fprintf(stderr,"FAIL line%d: %s\n",__LINE__,#x);return 1;}}while(0)
int main(void) {
 unsigned i,j,b,s,shiny=0,transitions[2][2]={{0}},bins[10]={0},last=0;
 struct SaveBlock1 before;
 struct DayCare *care;
 unsigned char original[sizeof(struct DaycareMon)];
 memset(&save1,0x39,sizeof(save1));memset(&save1.route5Daycare,0,400);
 for(i=0;i<sizeof(original);i++)original[i]=i*37+11;
 memcpy(&save1.route5DayCareMon,original,sizeof(original));before=save1;
 care=GetRoute5Daycare();CHECK(care==&save1.route5Daycare.daycare);
 CHECK(!memcmp(&care->mons[0],original,sizeof(original)));
 CHECK(!memcmp(&save1.route5DayCareMon,original,sizeof(original)));
 CHECK(!memcmp(&save1,&before,offsetof(struct SaveBlock1,route5Daycare)));
 CHECK(!memcmp((u8*)&save1+offsetof(struct SaveBlock1,ramScript),(u8*)&before+offsetof(struct SaveBlock1,ramScript),sizeof(save1)-offsetof(struct SaveBlock1,ramScript)));
 memset(&care->mons[0],0,sizeof(care->mons[0]));CHECK(GetRoute5Daycare()==care);
 CHECK(!memcmp(&care->mons[0],&care->mons[1],sizeof(care->mons[0]))); // no resurrection
 save1.route5Daycare.version=2;before=save1;
 CHECK(!GetRoute5Daycare());CHECK(!memcmp(&save1,&before,sizeof(save1)));
 memset(&save1.route5Daycare,0,400);save1.route5Daycare.unused[7]=1;before=save1;
 CHECK(!GetRoute5Daycare());CHECK(!memcmp(&save1,&before,sizeof(save1)));
 memset(&save1,0,sizeof(save1));
 save1.location.mapGroup=MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE);
 save1.location.mapNum=MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE);
 CHECK(GetActiveDaycare()==GetRoute5Daycare());
 save1.location.mapGroup=MAP_GROUP(MAP_FOUR_ISLAND_POKEMON_DAY_CARE);
 save1.location.mapNum=MAP_NUM(MAP_FOUR_ISLAND_POKEMON_DAY_CARE);
 CHECK(GetActiveDaycare()==&save1.daycare);
 // Reduced complete reachability proof: mod25 contribution of variable bits
 // 8..14 in low/high halves and the eight possible shiny XOR values. Other
 // bits merely add a constant/permutation; excludes low16=0 candidate safely.
 for(b=0;b<128;b++) {
  unsigned seen=0;
  for(i=1;i<128;i++)for(s=0;s<8;s++)seen|=1U<<((16*(b^i)+6*i+11*s)%25);
  CHECK(seen==0x1FFFFFF);
 }
 seed=0x2A551701;
 for(i=0;i<50000;i++) {
  u32 original=(u32)Random()<<16;original|=Random()|1;
  u32 ot=(u32)Random()<<16;ot|=Random();
  for(j=0;j<2;j++) {
   u32 p=Route5EggPersonality(original,ot,j);
   CHECK((GET_SHINY_VALUE(ot,p)<8)==j);
   CHECK(p%25==original%25);CHECK((p&0x80FF)==(original&0x80FF));CHECK((p&65535)!=0);
  }
 }
 care=GetRoute5Daycare();save2.playerTrainerId[0]=0x12;save2.playerTrainerId[1]=0x34;
 save2.playerTrainerId[2]=0x56;save2.playerTrainerId[3]=0x78;
 for(i=0;i<10000;i++) {
  struct Route5DaycareSave pending;
  u32 actual,ot;
  care->offspringPersonality=0;
  // Varied deterministic independent fixture seeds, recorded for reproduction.
  seed=0x2A551701U+i;
  seed=(seed^(seed>>16))*0x7FEB352DU;seed=(seed^(seed>>15))*0x846CA68BU;seed^=seed>>16;
  rngCalls=0;
  SetRoute5PendingEgg(care,FALSE);CHECK(rngCalls==3);
  ot=save1.route5Daycare.eggOtId;actual=GET_SHINY_VALUE(ot,save1.route5Daycare.eggPersonality)<8;
  CHECK(actual==save1.route5Daycare.eggShiny);CHECK(care->offspringPersonality);
  CHECK(ot==0x78563412U);
  shiny+=actual;bins[i/1000]+=actual;if(i)transitions[last][actual]++;last=actual;
  pending=save1.route5Daycare;SetRoute5PendingEgg(care,FALSE);
  CHECK(!memcmp(&pending,&save1.route5Daycare,sizeof(pending)));CHECK(rngCalls==3);
 }
 fprintf(stderr,"STAT diagnostics: shiny=%u transitions=%u/%u/%u/%u\n",shiny,transitions[0][0],transitions[0][1],transitions[1][0],transitions[1][1]);
 CHECK(shiny>=4700 && shiny<=5300);
 for(i=0;i<10;i++)CHECK(bins[i]>400 && bins[i]<600);
 for(i=0;i<2;i++)for(j=0;j<2;j++)CHECK(transitions[i][j]>2200 && transitions[i][j]<2800);
 printf("PASS: 10000 independently generated pending Route5 Eggs: %u Shiny, %u non-Shiny; actual PID/OT XOR; seed(i)=mix32(0x2A551701+i); xor16/mul7FEB352D/xor15/mul846CA68B/xor16\n",shiny,10000-shiny);
 printf("PASS: sequential outcome counts00/01/10/11=%u/%u/%u/%u; no alternating/once-per-pickup policy\n",transitions[0][0],transitions[0][1],transitions[1][0],transitions[1][1]);
 // A second production-style stream never reseeds between Eggs.
 seed=0x51E832A9;shiny=0;memset(transitions,0,sizeof(transitions));last=0;
 for(i=0;i<10000;i++) {
  unsigned actual;
  care->offspringPersonality=0;SetRoute5PendingEgg(care,FALSE);
  actual=GET_SHINY_VALUE(save1.route5Daycare.eggOtId,save1.route5Daycare.eggPersonality)<8;
  shiny+=actual;if(i)transitions[last][actual]++;last=actual;
 }
 CHECK(shiny>=4700 && shiny<=5300);
 for(i=0;i<2;i++)for(j=0;j<2;j++)CHECK(transitions[i][j]>2200 && transitions[i][j]<2800);
 printf("PASS: additional continuous unreseeded10000-Egg stream from0x51E832A9: %u Shiny; transitions=%u/%u/%u/%u\n",shiny,transitions[0][0],transitions[0][1],transitions[1][0],transitions[1][1]);
 printf("PASS: %u save/migration/policy assertions; 50000 varied identities both outcomes, all128 reduced XOR blocks; no emulator claim\n",checks);
 return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='lightning-daycare-') as temp:
    # Fresh CI runs host tests before Make generates map_groups.h. Derive map
    # IDs from the same authoritative JSON into a temporary include directory.
    groups=json.loads(Path('data/maps/map_groups.json').read_text());header=''
    for gi,group in enumerate(groups['group_order']):
        for mi,name in enumerate(groups[group]):
            map_id=json.loads((Path('data/maps')/name/'map.json').read_text())['id']
            header+=f'#define {map_id} ({mi} | ({gi} << 8))\n'
    inc=Path(temp)/'constants';inc.mkdir();(inc/'map_groups.h').write_text(header)
    source=Path(temp)/'daycare.c';source.write_text(program)
    subprocess.run(['gcc','-std=gnu11','-O2','-D__INTELLISENSE__','-iquote',temp,'-iquote','include',str(source),'-o',temp+'/daycare'],check=True)
    subprocess.run([temp+'/daycare'],check=True)

# Independent preservation audit: these mechanics and FourIsland scripts are
# byte-identical to the reviewed integration base. Separate from execution.
import json, hashlib
snapshot=json.loads(Path('tests/specifications/daycare_preserved.json').read_text())
from approved_route22_changes import prior_route22_source
for path,expected in snapshot['files'].items():
    assert hashlib.sha256(prior_route22_source(path)).hexdigest()==expected,path
source=Path('src/daycare.c').read_text()
for name,expected in snapshot['functions'].items():
    assert hashlib.sha256(extract_function(source,name).encode()).hexdigest()==expected,name
hatch_tail=source.split('    // Hatch Egg',1)[1].split('\nbool8 ShouldEggHatch',1)[0]
assert hashlib.sha256(hatch_tail.encode()).hexdigest()==snapshot['hatch_tail_sha256']
assert extract_function(source,'ShouldEggHatch').count('TryProduceOrHatchEgg(')==1
print('PASS: static original breeding/offspring/incense/IV/move/fees/mail/hatch code, FourIsland maps/scripts, save checksums, wild data and global Shiny routines preserved')

assert 'strcpy(' not in extract_function(source,'DaycarePrintMonLvl')
assert 'StringCopy(lvlText, gText_Lv);' in extract_function(source,'DaycarePrintMonLvl')
print('PASS: static daycare level label uses EOS-aware StringCopy, not rev0 C strcpy')
