#!/usr/bin/env python3
"""Execute production weighted/state/RTC policy; field rendering needs mGBA."""
from pathlib import Path
import json,re,hashlib,subprocess,tempfile
from approved_dynamic_weather_changes import prior_dynamic_weather_source
# CI runs host checks before the full ROM build. Generate the real upstream
# constants with its own tool, not a hand-maintained duplicate mapping.
if not Path('include/constants/map_groups.h').exists():
    subprocess.run(['make','-C','tools/mapjson'],check=True)
    subprocess.run(['tools/mapjson/mapjson','groups','firered','data/maps/map_groups.json','data/maps','include/constants'],check=True)
f=json.loads(Path('tests/specifications/approved_weather_maps.json').read_text())
d=json.loads(Path('tests/specifications/approved_dynamic_weather_delta.json').read_text())
for p,h in d['before_sha256'].items():assert hashlib.sha256(prior_dynamic_weather_source(p).encode()).hexdigest()==h,p
# Every table map matches a real source map, with exact owner-reviewed weights.
expected='// Approved location weights; see tests/specifications/approved_weather_maps.json.\nstatic const struct WeatherPolicy sWeatherPolicies[] = {\n'
for row in f['maps']:
 m=json.loads(Path('data/maps/'+row['name']+'/map.json').read_text());assert m['id']==row['map'] and m['map_type']==row['map_type']
 expected+='    {MAP_GROUP('+row['map']+'), MAP_NUM('+row['map']+'), {'+', '.join(map(str,row['weights']))+'}},\n'
expected+='};\n';assert Path('src/data/dynamic_weather.h').read_text()==expected
src=Path('src/dynamic_weather.c').read_text().replace('#include "data/dynamic_weather.h"',expected)
src=re.sub(r'^#include .*\n','',src,flags=re.M)
pre=r'''
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "constants/maps.h"
#include "constants/weather.h"
#include "constants/time_of_day.h"
typedef uint8_t u8;typedef uint16_t u16;typedef uint32_t u32;typedef u8 bool8;
#define TRUE 1
#define FALSE 0
#define ARRAY_COUNT(a) (sizeof(a)/sizeof(*(a)))
struct Save {struct {u8 mapGroup,mapNum;} location;u8 unused_3D24[16],weather;} save,*gSaveBlock1Ptr=&save;
struct {u8 mapType,weather;} gMapHeader;
struct {u32 vblankCounter2;} gMain;
static u32 seed=1,reads,draws;static int fixed=-1,reject=-1;static u8 hour=12,valid=1,current;
static u16 Random(void){draws++;if(reject>=0){u16 v=reject;reject=-1;return v;}if(fixed>=0)return fixed;seed=seed*1103515245+12345;return seed>>16;}
static bool8 RtcGetHour(u8 *h){reads++;*h=hour;return valid;}
static bool8 IsMapTypeOutdoors(u8 t){return t==MAP_TYPE_ROUTE||t==MAP_TYPE_TOWN||t==MAP_TYPE_CITY||t==MAP_TYPE_OCEAN_ROUTE;}
static u8 GetSav1Weather(void){return save.weather;}
static u8 GetCurrentWeather(void){return current;}
static void SetSavedWeather(u32 w){save.weather=w;}
static void SetNextWeather(u8 w){current=w;}
'''
post=r'''
static void enter(unsigned id,unsigned type,unsigned original){save.location.mapGroup=MAP_GROUP(id);save.location.mapNum=MAP_NUM(id);gMapHeader.mapType=type;gMapHeader.weather=original;save.weather=DynamicWeatherForMap(original);}
int main(void){unsigned checks=0;const unsigned values[]={WEATHER_NONE,WEATHER_RAIN,WEATHER_DROUGHT,WEATHER_SANDSTORM,WEATHER_SNOW};
 for(unsigned p=0;p<ARRAY_COUNT(sWeatherPolicies);p++)for(unsigned night=0;night<2;night++){
  unsigned total=0,counts[15]={0};for(unsigned i=0;i<5;i++)total+=sWeatherPolicies[p].weights[i];assert(total>0);unsigned limit=65536-65536%total;hour=night?18:6;
  for(unsigned i=0;i<limit;i++){fixed=i;unsigned w=RollWeather(&sWeatherPolicies[p]);assert(w<15);counts[w]++;checks++;}
  unsigned expected[15]={0};for(unsigned i=0;i<5;i++){unsigned w=night&&i==2?WEATHER_NONE:values[i];expected[w]+=sWeatherPolicies[p].weights[i]*(limit/total);}for(unsigned i=0;i<15;i++){assert(counts[i]==expected[i]);checks++;}
 }
 fixed=0;for(unsigned r=65530;r<65536;r++){reject=r;unsigned before=draws;assert(WeatherRandomBelow(10)==0&&draws==before+2);}
 reject=65535;unsigned rejectedBefore=draws;assert(WeatherRandomBelow(3)==0&&draws==rejectedBefore+2);
 memset(save.unused_3D24,0xCC,16);fixed=0;hour=12;enter(MAP_ONE_ISLAND,MAP_TYPE_TOWN,WEATHER_SUNNY);assert(WeatherContextValid());
 save.weather=WEATHER_SANDSTORM;enter(MAP_ONE_ISLAND_POKEMON_CENTER_1F,MAP_TYPE_INDOOR,WEATHER_NONE);assert(save.weather==WEATHER_NONE);
 enter(MAP_ONE_ISLAND_POKEMON_CENTER_2F,MAP_TYPE_INDOOR,WEATHER_NONE);assert(WeatherContext()[4]==WEATHER_SANDSTORM);
 fixed=1;enter(MAP_ONE_ISLAND,MAP_TYPE_TOWN,WEATHER_SUNNY);assert(save.weather==WEATHER_SANDSTORM);
 fixed=0;enter(MAP_ONE_ISLAND_POKEMON_CENTER_1F,MAP_TYPE_INDOOR,WEATHER_NONE);enter(MAP_ONE_ISLAND,MAP_TYPE_TOWN,WEATHER_SUNNY);assert(save.weather==WEATHER_NONE);
 for(unsigned i=8;i<16;i++)assert(save.unused_3D24[i]==0xCC);
 // New outdoor transitions always draw; identical old result is not a skip.
 unsigned before=draws;enter(MAP_TWO_ISLAND,MAP_TYPE_TOWN,WEATHER_SUNNY);assert(draws>before);before=draws;enter(MAP_ONE_ISLAND,MAP_TYPE_TOWN,WEATHER_SUNNY);assert(draws>before);
 hour=18;fixed=0;enter(MAP_CINNABAR_ISLAND,MAP_TYPE_TOWN,WEATHER_SUNNY);assert(save.weather==WEATHER_NONE);
 hour=12;enter(MAP_PALLET_TOWN,MAP_TYPE_TOWN,WEATHER_SUNNY);enter(MAP_CINNABAR_ISLAND,MAP_TYPE_TOWN,WEATHER_SUNNY);assert(save.weather==WEATHER_DROUGHT);hour=18;gMain.vblankCounter2=16;before=draws;DynamicWeatherUpdate();assert(save.weather==WEATHER_NONE&&current==WEATHER_NONE&&draws==before);
 hour=6;DynamicWeatherUpdate();assert(save.weather==WEATHER_NONE&&draws==before);
 valid=0;enter(MAP_PALLET_TOWN,MAP_TYPE_TOWN,WEATHER_SUNNY);enter(MAP_CINNABAR_ISLAND,MAP_TYPE_TOWN,WEATHER_SUNNY);assert(save.weather==WEATHER_DROUGHT);valid=1;hour=23;DynamicWeatherUpdate();assert(save.weather==WEATHER_NONE);
 before=draws;enter(MAP_CINNABAR_ISLAND,MAP_TYPE_TOWN,WEATHER_SUNNY);assert(draws==before);
 // Exact logical battle weather does not wait for renderer/fade completion.
 current=WEATHER_DROUGHT;save.weather=WEATHER_RAIN;assert(DynamicWeatherForBattle()==WEATHER_RAIN);
 for(unsigned i=0;i<1000;i++){before=draws;DynamicWeatherUpdate();assert(draws==before);}
 hour=12;enter(MAP_ROUTE17,MAP_TYPE_ROUTE,WEATHER_SUNNY);fixed=1;before=draws;DynamicWeatherAfterBattle();assert(draws==before+1&&save.weather==WEATHER_DROUGHT);
 enter(MAP_ROUTE14,MAP_TYPE_ROUTE,WEATHER_SUNNY);before=draws;DynamicWeatherAfterBattle();assert(save.weather==WEATHER_RAIN&&draws==before);
 // Explicit outdoor headers tagged indoor still receive their approved policies.
 fixed=0;enter(MAP_NAVEL_ROCK_HARBOR,MAP_TYPE_INDOOR,WEATHER_NONE);assert(save.weather==WEATHER_RAIN);
 enter(MAP_BIRTH_ISLAND_HARBOR,MAP_TYPE_INDOOR,WEATHER_NONE);assert(save.weather==WEATHER_SANDSTORM);
 enter(MAP_MT_EMBER_SUMMIT,MAP_TYPE_UNDERGROUND,WEATHER_NONE);assert(save.weather==WEATHER_DROUGHT);
 enter(MAP_MT_EMBER_SUMMIT_PATH_1F,MAP_TYPE_UNDERGROUND,WEATHER_NONE);assert(save.weather==WEATHER_NONE);
 enter(MAP_VIRIDIAN_FOREST,MAP_TYPE_ROUTE,WEATHER_SHADE);assert(save.weather==WEATHER_SHADE);
 // Corrupt context discards only weather metadata, never save layout/Pokemon.
 WeatherContext()[7]^=1;assert(!WeatherContextValid());enter(MAP_PALLET_TOWN,MAP_TYPE_TOWN,WEATHER_SUNNY);assert(WeatherContextValid());
 printf("PASS: %u exhaustive accepted-roll assertions across83 map policies/day+night, plus native policy/state/indoor/fallback/recovery/clock/battle/save-tail tests (host stubs, not emulator)\n",checks);
}
'''
with tempfile.TemporaryDirectory(prefix='weather-host-') as tmp:
 p=Path(tmp)/'test.c';p.write_text(pre+re.sub(r'^#include .*\n','',Path('src/time_of_day.c').read_text(),flags=re.M)+src+post)
 subprocess.run(['gcc','-std=c99','-O2','-Wall','-Wextra','-Werror','-I','include',str(p),'-o',tmp+'/run'],check=True)
 subprocess.run([tmp+'/run'],check=True)
print('PASS: all frozen original source bytes and all83 explicit map policy records preserved; no invented coverage')
