#!/usr/bin/env python3
"""Execute production encounter/policy routines on host; Pokemon creation is stubbed.
Real encrypted Pokemon creation, battle scenes and capture are tested in mGBA.
"""
from pathlib import Path
import subprocess, tempfile, json
from run_split_tests import extract_function
wild=Path('src/wild_encounter.c').read_text()
policy=Path('src/wild_double_battle.c').read_text()
prefix=r'''
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "global.h"
#include "pokemon.h"
#include "wild_encounter.h"
#include "wild_double_battle.h"
#include "battle.h"
#include "battle_anim.h"
#include "random.h"
#include "event_data.h"
#include "field_player_avatar.h"
#include "util.h"
#include "constants/maps.h"
#include "constants/items.h"
#include "constants/abilities.h"
#define WILD_AREA_LAND 0
#define WILD_AREA_WATER 1
#define WILD_AREA_ROCKS 2
#define WILD_CHECK_REPEL 1
#define MAX_ENCOUNTER_RATE 1600
struct SaveBlock1 save;
struct SaveBlock1 *gSaveBlock1Ptr=&save;
struct Pokemon gPlayerParty[PARTY_SIZE],gEnemyParty[PARTY_SIZE];
struct BattlePokemon gBattleMons[MAX_BATTLERS_COUNT];
u8 gAbsentBattlerFlags,gBattlersCount;
u32 gBattleTypeFlags,gRngValue;
const u32 gBitTable[]={1,2,4,8};
static u16 monSpecies[PARTY_SIZE];
static u8 monLevel[PARTY_SIZE];
static u16 repel,whiteFlute,blackFlute;
static u8 safari,usable=PLAYER_HAS_TWO_USABLE_MONS,ability;
static unsigned singleStarts,doubleStarts,created,checks;
#define CHECK(x) do {checks++; if(!(x)){fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#x);return 1;}}while(0)
bool32 GetSafariZoneFlag(void){return safari;}
u8 GetMonsStateToDoubles(void){return usable;}
u8 GetBattlerAtPosition(u8 p){return p;}
u8 GetBattlerSide(u8 p){return p&1;}
u16 VarGet(u16 v){(void)v;return repel;}
bool8 FlagGet(u16 v){return v==FLAG_SYS_WHITE_FLUTE_ACTIVE?whiteFlute:v==FLAG_SYS_BLACK_FLUTE_ACTIVE?blackFlute:0;}
u8 GetMonAbility(struct Pokemon *p){(void)p;return ability;}
bool8 TestPlayerAvatarFlags(u8 f){(void)f;return 0;}
u32 GetMonData3(struct Pokemon *p,s32 f,u8 *data){
 (void)data;
 if(f==MON_DATA_HP)return p->hp;
 if(f==MON_DATA_IS_EGG||f==MON_DATA_SANITY_IS_EGG)return 0;
 if(f==MON_DATA_LEVEL)return p->level;
 return 0;
}
u32 GetMonData2(struct Pokemon *p,s32 f){return GetMonData3(p,f,NULL);}
void ZeroEnemyPartyMons(void){memset(monSpecies,0,sizeof(monSpecies));memset(monLevel,0,sizeof(monLevel));}
void CreateMonWithNature(struct Pokemon *p,u16 species,u8 level,u8 iv,u8 nature){
 (void)iv;(void)nature;unsigned i=p-gEnemyParty;monSpecies[i]=species;monLevel[i]=level;created++;
}
void CreateMon(struct Pokemon *p,u16 species,u8 level,u8 iv,bool8 fixed,u32 personality,u8 ot,u32 otid){
 (void)fixed;(void)personality;(void)ot;(void)otid;CreateMonWithNature(p,species,level,iv,0);
}
void StartWildBattle(void){singleStarts++;}
void StartDoubleWildBattle(void){doubleStarts++;}
static u32 GenerateUnownPersonalityByLetter(u8 c){return c;}
static const u8 sUnownLetterSlots[7][12]={{0}};
'''
start=wild.index('struct WildEncounterData\n');end=wild.index('static EWRAM_DATA bool8',start)
# Match the upstream generator's cumulative slot macros, directly from its JSON.
rates=json.loads(Path('src/data/wild_encounters.json').read_text())['wild_encounter_groups'][0]['fields']
macros=''
for field,name in [(rates[0],'LAND'),(rates[1],'WATER')]:
 total=0
 for i,weight in enumerate(field['encounter_rates']):
  total+=weight;macros+=f'#define ENCOUNTER_CHANCE_{name}_MONS_SLOT_{i} {total}\n'
 macros+=f'#define ENCOUNTER_CHANCE_{name}_MONS_TOTAL {total}\n'
program=prefix+macros+wild[start:end]+'\n'+extract_function(Path('src/random.c').read_text(),'Random')+'\n'+policy
# forward prototypes from current source
for f in ['IsWildLevelAllowedByRepel','ApplyFluteEncounterRateMod','GetFluteEncounterRateModType','ApplyCleanseTagEncounterRateMod','IsLeadMonHoldingCleanseTag','WildEncounterRandom']:
 fn=extract_function(wild,f);program+=fn[:fn.index('{')].strip()+';\n'
for f in ['ChooseWildMonIndex_Land','ChooseWildMonIndex_WaterRock','ChooseWildMonLevel','GenerateWildMonAt','GenerateWildMon','TryGenerateWildMon','IsWildLevelAllowedByRepel','TryStartLandEncounter','GetFluteEncounterRateModType','ApplyFluteEncounterRateMod','IsLeadMonHoldingCleanseTag','ApplyCleanseTagEncounterRateMod','WildEncounterRandom','DoWildEncounterRateDiceRoll','DoWildEncounterRateTest','GetAbilityEncounterRateModType']:
 program+=extract_function(wild,f)+'\n'
program+=r'''
int main(void){
 struct WildPokemon slots[12];struct WildPokemonInfo info={20,slots};
 const double weights[]={.20,.20,.10,.10,.10,.10,.05,.05,.04,.04,.01,.01};
 unsigned marg[2][12]={{0}},joint[12][12]={{0}},special=0,duplicates=0,ordinary=0;
 unsigned i,j,k;double chi=0;
 for(i=0;i<12;i++){slots[i].species=SPECIES_BULBASAUR+i;slots[i].minLevel=3;slots[i].maxLevel=7;}
 save.location.mapGroup=MAP_GROUP(MAP_ROUTE1);save.location.mapNum=MAP_NUM(MAP_ROUTE1);
 CHECK(GetWildDoubleMap()!=NULL);CHECK(TryDoDoubleWildBattle());
 save.location.mapGroup=MAP_GROUP(MAP_MT_MOON_1F);save.location.mapNum=MAP_NUM(MAP_MT_MOON_1F);CHECK(TryDoDoubleWildBattle());
 safari=1;CHECK(!TryDoDoubleWildBattle());safari=0;
 usable=PLAYER_HAS_ONE_USABLE_MON;CHECK(!TryDoDoubleWildBattle());
 CHECK(TryStartLandEncounter(&info,0));CHECK(singleStarts==1&&doubleStarts==0&&monSpecies[1]==0);
 usable=PLAYER_HAS_TWO_USABLE_MONS;
 save.location.mapGroup=MAP_GROUP(MAP_ROUTE2);save.location.mapNum=MAP_NUM(MAP_ROUTE2);CHECK(GetWildDoubleMap()==NULL);
 CHECK(TryStartLandEncounter(&info,0));CHECK(singleStarts==2&&doubleStarts==0);
 save.location.mapGroup=MAP_GROUP(MAP_ROUTE1);save.location.mapNum=MAP_NUM(MAP_ROUTE1);
 // A blocked trigger neither starts a battle nor rolls a special-pair override.
 repel=100;gPlayerParty[0].hp=20;gPlayerParty[0].level=50;
 for(i=0;i<1000;i++){CHECK(!TryStartLandEncounter(&info,WILD_CHECK_REPEL));}
 CHECK(doubleStarts==0);CHECK(singleStarts==2);
 // Sweet Scent's zero flags bypass Repel as in the original engine.
 CHECK(TryStartLandEncounter(&info,0));CHECK(doubleStarts==1);
 repel=0;gRngValue=0x51e832a9;
 for(i=0;i<200000;i++){
  CHECK(TryStartLandEncounter(&info,WILD_CHECK_REPEL));
  CHECK(monSpecies[2]==0);
  for(j=0;j<2;j++)CHECK(monLevel[j]>=3&&monLevel[j]<=7);
  if(monSpecies[0]==SPECIES_ONIX&&monSpecies[1]==SPECIES_CLOYSTER){special++;continue;}
  ordinary++;
  for(j=0;j<2;j++){CHECK(monSpecies[j]>=SPECIES_BULBASAUR&&monSpecies[j]<=SPECIES_BULBASAUR+11);marg[j][monSpecies[j]-SPECIES_BULBASAUR]++;}
  joint[monSpecies[0]-SPECIES_BULBASAUR][monSpecies[1]-SPECIES_BULBASAUR]++;
  if(monSpecies[0]==monSpecies[1])duplicates++;
 }
 CHECK(doubleStarts==200001);CHECK(special>9600&&special<10400);CHECK(duplicates>10000);
 for(j=0;j<2;j++)for(k=0;k<12;k++){
  double expected=ordinary*weights[k],delta=marg[j][k]-expected;
  CHECK(fabs(delta)<7*sqrt(expected*(1-weights[k])));
 }
 for(j=0;j<12;j++)for(k=0;k<12;k++){
  double expected=ordinary*weights[j]*weights[k],delta=joint[j][k]-expected;
  chi+=delta*delta/expected;
 }
 CHECK(chi<230);
 printf("PASS: %u executed policy/generation assertions; %u/200000 special pairs; %u duplicates; pair independence chi-square=%.2f (144 cells)\n",checks,special,duplicates,chi);
 // Feed every accepted 16-bit outcome through the real pair-selection function.
 // Modular inverse solves one LCG step; this is input construction, not a
 // reimplementation of the selection policy being asserted.
 {unsigned hits=0;for(i=0;i<65500;i++){
   gRngValue=(((u32)i<<16)-24691U)*0xEEB9EB65U;
   hits+=ChooseWildDoubleSpecialPair(GetWildDoubleMap())==0;
  }CHECK(hits==3275);
  for(i=65500;i<65536;i++){
   gRngValue=(((u32)i<<16)-24691U)*0xEEB9EB65U;
   s16 result=ChooseWildDoubleSpecialPair(GetWildDoubleMap());CHECK(result==-1||result==0);
  }
 }

 {const struct WildDoublePair pairs[]={{{SPECIES_ONIX,SPECIES_CLOYSTER},30},{{SPECIES_PIDGEY,SPECIES_RATTATA},20}};
  const struct WildDoubleMap map={0,0,2,pairs};unsigned outcomes[3]={0};
  for(i=0;i<65500;i++){
   gRngValue=(((u32)i<<16)-24691U)*0xEEB9EB65U;
   s16 result=ChooseWildDoubleSpecialPair(&map);CHECK(result>=-1&&result<=1);outcomes[result+1]++;
  }
  CHECK(outcomes[0]==32750&&outcomes[1]==19650&&outcomes[2]==13100);
  CHECK(ChooseWildDoubleSpecialPair(NULL)==-1);
 }
 for(i=0;i<ARRAY_COUNT(sWildDoubleMaps);i++){
  unsigned sum=0;
  for(j=0;j<sWildDoubleMaps[i].pairCount;j++){
   const struct WildDoublePair *p=&sWildDoubleMaps[i].pairs[j];
   CHECK(p->percent<=100);sum+=p->percent;
   CHECK(p->species[0]>SPECIES_NONE&&p->species[0]<NUM_SPECIES);
   CHECK(p->species[1]>SPECIES_NONE&&p->species[1]<NUM_SPECIES);
  }CHECK(sum<=100);
 }
 // These tests execute original ability/rate routines, not an imitation of them.
 ability=ABILITY_STENCH;CHECK(GetAbilityEncounterRateModType()==1);
 ability=ABILITY_ILLUMINATE;CHECK(GetAbilityEncounterRateModType()==2);
 ability=ABILITY_NONE;CHECK(GetAbilityEncounterRateModType()==0);
 {unsigned counts[3]={0};for(j=0;j<3;j++){
  sWildEncounterData.rngState=1234;sWildEncounterData.abilityEffect=j;
  for(i=0;i<100000;i++)counts[j]+=DoWildEncounterRateTest(20,FALSE);
 }
 CHECK(counts[1]<counts[0]&&counts[0]<counts[2]);
 sWildEncounterData.abilityEffect=0;sWildEncounterData.leadMonHeldItem=ITEM_CLEANSE_TAG;
 unsigned tag=0,white=0,black=0;
 sWildEncounterData.rngState=1234;for(i=0;i<100000;i++)tag+=DoWildEncounterRateTest(20,FALSE);
 sWildEncounterData.leadMonHeldItem=0;whiteFlute=1;
 sWildEncounterData.rngState=1234;for(i=0;i<100000;i++)white+=DoWildEncounterRateTest(20,FALSE);
 whiteFlute=0;blackFlute=1;
 sWildEncounterData.rngState=1234;for(i=0;i<100000;i++)black+=DoWildEncounterRateTest(20,FALSE);
 CHECK(tag<counts[0]&&white>counts[0]&&black<counts[0]);
 printf("PASS: original Cleanse Tag/White Flute/Black Flute=%u/%u/%u\n",tag,white,black);
 printf("PASS: original rate dice, normal/Stench/Illuminate=%u/%u/%u\n",counts[0],counts[1],counts[2]);}
 // Capture and escape targets including a fainted left opponent, absence, speed.
 gBattleTypeFlags=BATTLE_TYPE_DOUBLE;gBattlersCount=4;
 gBattleMons[1].hp=gBattleMons[3].hp=20;gBattleMons[1].speed=10;gBattleMons[3].speed=50;
 CHECK(IsWildDoubleBattle());CHECK(GetWildOpponentCount()==2);CHECK(GetWildCatchTarget()==1);CHECK(GetWildEscapeOpponent()==3);
 gBattleMons[1].hp=0;CHECK(GetWildOpponentCount()==1);CHECK(GetWildCatchTarget()==3);
 gAbsentBattlerFlags=2;CHECK(GetWildCatchTarget()==3);
 gBattleMons[1].hp=20;gAbsentBattlerFlags=8;CHECK(GetWildOpponentCount()==1);CHECK(GetWildCatchTarget()==1);CHECK(GetWildEscapeOpponent()==1);
 gBattleTypeFlags|=BATTLE_TYPE_TRAINER;CHECK(!IsWildDoubleBattle());CHECK(GetWildCatchTarget()==1);
 gBattleTypeFlags=BATTLE_TYPE_DOUBLE|BATTLE_TYPE_LINK;CHECK(!IsWildDoubleBattle());
 printf("PASS: %u total host assertions; battle rendering/capture UI requires the separate ROM suite\n",checks);
 return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='lightning-wild-host-') as d:
 # The actual build generates this header. Host tests run before build in CI,
 # so generate only the needed constants in a temporary include directory from
 # the same authoritative map-group/map JSON, without editing the checkout.
 groups=json.loads(Path('data/maps/map_groups.json').read_text());header=''
 for group_index,group in enumerate(groups['group_order']):
  for map_index,name in enumerate(groups[group]):
   map_id=json.loads((Path('data/maps')/name/'map.json').read_text())['id']
   header+=f'#define {map_id} ({map_index} | ({group_index} << 8))\n'
 inc=Path(d)/'constants';inc.mkdir();(inc/'map_groups.h').write_text(header)
 p=Path(d)/'test.c';p.write_text(program)
 subprocess.run(['gcc','-std=gnu11','-O2','-Wall','-Wextra','-Wno-pointer-to-int-cast','-Wno-int-to-pointer-cast','-iquote',d,'-iquote','include',str(p),'-lm','-o',str(Path(d)/'test')],check=True)
 subprocess.run([str(Path(d)/'test')],check=True)
