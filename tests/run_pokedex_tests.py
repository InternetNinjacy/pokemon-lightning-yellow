#!/usr/bin/env python3
"""Execute current mapping/count/list/flag code. UI drawing is not host-emulated."""
from pathlib import Path
import csv, hashlib, json, re, subprocess, tempfile
from run_split_tests import extract_function

root = Path('.')
old = list(csv.DictReader(Path('tests/specifications/approved_251_roster.csv').open(encoding='utf-8-sig')))
expected = [int(row['internal_species']) for row in old]
constants = {k:int(v) for k,v in re.findall(r'^#define SPECIES_(\w+)\s+(\d+)\b', Path('include/constants/species.h').read_text(), re.M)}
expected.insert(232, constants['CASTFORM'])
manifest = re.findall(r'^CUSTOM_DEX_ENTRY\((\d+), (\w+)\)',Path('src/data/pokemon/custom_pokedex.h').read_text(),re.M)
assert [int(n) for n,_ in manifest] == list(range(1,253))
assert [constants[name] for _,name in manifest] == expected
assert len(set(expected)) == 252
assert [k for _,k in manifest[231:235]] == ['MILOTIC','CASTFORM','KECLEON','SHUPPET']
assert manifest[-1] == ('252','MEW')

pokemon=Path('src/pokemon.c').read_text();screen=Path('src/pokedex_screen.c').read_text()
table=pokemon[pokemon.index('static const u16 sSpeciesToNationalPokedexNum['):]
table=table[:table.index('\n};')+3]
struct=screen[screen.index('struct PokedexScreenData\n'):]
struct=struct[:struct.index('\n};')+3]
program=r'''
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "pokemon.h"
#include "pokedex.h"
#include "pokedex_screen.h"
#include "custom_pokedex.h"
#include "constants/pokedex.h"
#define SPECIES_TO_NATIONAL(name) [SPECIES_##name - 1] = NATIONAL_DEX_##name
struct ListMenuItem {const u8 *label; s32 index;};
const u8 gSpeciesNames[NUM_SPECIES][POKEMON_NAME_LENGTH + 1] = {{0}};
const u8 gText_5Dashes[] = {0xFF};
static struct SaveBlock1 save1;
static struct SaveBlock2 save2;
struct SaveBlock1 *gSaveBlock1Ptr = &save1;
struct SaveBlock2 *gSaveBlock2Ptr = &save2;
'''
program+=table+'\n'+struct+'\nstatic struct PokedexScreenData screen;\nstatic struct PokedexScreenData *sPokedexScreenData=&screen;\n'
program+='#include "src/data/pokemon/pokedex_orders.h"\n'
for f in ['SpeciesToNationalPokedexNum','NationalPokedexNumToSpecies']:
 program+=extract_function(pokemon,f)
program+=extract_function(screen,'DexScreen_GetSetPokedexFlag')
program+=extract_function(Path('src/pokedex.c').read_text(),'GetSetPokedexFlag')
program+=Path('src/custom_pokedex.c').read_text()
program+=extract_function(pokemon,'SpeciesToPokedexNum')
program+=extract_function(screen,'DexScreen_CountMonsInOrderedList')
program+=extract_function(screen,'DexScreen_CanShowMonInDex')
program+='\nstatic const u16 expected[]={'+','.join(map(str,expected))+'};\n'
program+=r'''
static unsigned checks;
#define CHECK(x) do {checks++;if(!(x)){fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#x);return 1;}}while(0)
int main(void) {
 struct ListMenuItem list[CUSTOM_DEX_COUNT+1];
 unsigned i,j,n,ndex,orders[]={DEX_ORDER_NUMERICAL_KANTO,DEX_ORDER_NUMERICAL_NATIONAL,DEX_ORDER_ATOZ,DEX_ORDER_TYPE,DEX_ORDER_LIGHTEST,DEX_ORDER_SMALLEST};
 screen.listItems=list;
 CHECK(CustomPokedexNumToSpecies(0)==SPECIES_NONE);
 CHECK(CustomPokedexNumToSpecies(253)==SPECIES_NONE);
 CHECK(CustomPokedexNumToSpecies(65535)==SPECIES_NONE);
 CHECK(SpeciesToCustomPokedexNum(0)==0);
 CHECK(SpeciesToCustomPokedexNum(NUM_SPECIES)==0);
 CHECK(SpeciesToCustomPokedexNum(65535)==0);
 CHECK(GetCustomPokedexCount(FLAG_SET_CAUGHT)==0);
 CHECK(!DexScreen_GetSetPokedexFlag(0,FLAG_SET_SEEN,FALSE));
 CHECK(!DexScreen_GetSetPokedexFlag(387,FLAG_SET_SEEN,FALSE));
 for(i=0;i<252;i++) {
  CHECK(CustomPokedexNumToSpecies(i+1)==expected[i]);
  CHECK(SpeciesToCustomPokedexNum(expected[i])==i+1);
  CHECK(SpeciesToPokedexNum(expected[i])==i+1);
  CHECK(DexScreen_CanShowMonInDex(expected[i]));
  ndex=SpeciesToNationalPokedexNum(expected[i]);
  CHECK(ndex>=1 && ndex<=386);
  GetSetPokedexFlag(ndex,FLAG_SET_SEEN);GetSetPokedexFlag(ndex,FLAG_SET_CAUGHT);
  CHECK(GetCustomPokedexCount(FLAG_GET_SEEN)==i+1);
  CHECK(GetCustomPokedexCount(FLAG_GET_CAUGHT)==i+1);
  CHECK(HasAllCustomPokedexSpecies()==(i==251));
 }
 for(i=1;i<NUM_SPECIES;i++)if(!IsSpeciesInCustomPokedex(i)) {
  CHECK(SpeciesToPokedexNum(i)==0xFFFF);CHECK(!DexScreen_CanShowMonInDex(i));
  ndex=SpeciesToNationalPokedexNum(i);
  if(ndex>=1 && ndex<=386) {GetSetPokedexFlag(ndex,FLAG_SET_SEEN);GetSetPokedexFlag(ndex,FLAG_SET_CAUGHT);}
 }
 CHECK(GetCustomPokedexCount(FLAG_GET_CAUGHT)==252);
 for(i=0;i<6;i++) {
  list[252].index=0x12345678;
  n=DexScreen_CountMonsInOrderedList(orders[i]);CHECK(n==252);
  CHECK(list[252].index==0x12345678);
  for(j=0;j<n;j++) {
   CHECK(IsSpeciesInCustomPokedex((u16)list[j].index));
   CHECK(((u32)list[j].index>>16)==3);
   if(i<2)CHECK((u16)list[j].index==expected[j]);
  }
 }
 // Old-format canonical flags remain the representation, including all three
 // redundant seen bits. Remove only Castform: completion must become false.
 ndex=SpeciesToNationalPokedexNum(SPECIES_CASTFORM)-1;
 save2.pokedex.owned[ndex/8]&=~(1<<(ndex%8));
 CHECK(GetCustomPokedexCount(FLAG_GET_CAUGHT)==251);CHECK(!HasAllCustomPokedexSpecies());
 CHECK(SpeciesToCustomPokedexNum(SPECIES_CASTFORM)==233);
 CHECK(SpeciesToCustomPokedexNum(SPECIES_KECLEON)==234);
 CHECK(SpeciesToCustomPokedexNum(SPECIES_MEW)==252);
 printf("PASS: %u executed custom Dex mapping/registration/count/completion/list assertions (host, stubbed labels)\n",checks);
 return 0;
}
'''
with tempfile.TemporaryDirectory() as temp:
 p=Path(temp)/'pokedex.c';p.write_text(program)
 subprocess.run(['gcc','-std=gnu11','-D__INTELLISENSE__','-iquote','.','-iquote','include','-iquote','src',str(p),'-o',temp+'/pokedex'],check=True)
 subprocess.run([temp+'/pokedex'],check=True)
# Separate preservation evidence: whole systems stay byte-identical to PR5.
preserved=json.loads(Path('tests/custom_dex_preserved.json').read_text())
for path,expected_hash in preserved.items():
 assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==expected_hash,path
print('PASS: static preservation of species/types/abilities, Forecast implementation, trainers, evolution data and save layout')
