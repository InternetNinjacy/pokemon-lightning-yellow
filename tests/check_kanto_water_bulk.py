#!/usr/bin/env python3
"""WRL-009 six real Kanto aquatic maps, independent rod and Surf checks."""
import json
from pathlib import Path
root=Path(__file__).resolve().parents[1]
group=json.loads((root/"src/data/wild_encounters.json").read_text())["wild_encounter_groups"][0]
weights={x["type"]:x["encounter_rates"] for x in group["fields"]}
assert weights["water_mons"]==[60,30,5,4,1]
assert weights["fishing_mons"]==[70,30,60,20,20,40,40,15,4,1]
assert next(x["groups"] for x in group["fields"] if x["type"]=="fishing_mons")=={"old_rod":[0,1],"good_rod":[2,3,4],"super_rod":[5,6,7,8,9]}
expected={"MAP_VERMILION_CITY":{"labels":["sVermilionCity_FireRed","sVermilionCity_LeafGreen"],"surf":["HORSEA","HORSEA","KRABBY","KRABBY","KRABBY"],"fish":["MAGIKARP","MAGIKARP","HORSEA","HORSEA","KRABBY","HORSEA","KRABBY","HORSEA","KRABBY","KRABBY"],"surfLv":[20,24,25,27,29],"fishingLv":[5,5,12,15,14,22,24,27,30,32],"waterRate":1,"fishingRate":10},"MAP_CERULEAN_CITY":{"labels":["sCeruleanCity_FireRed","sCeruleanCity_LeafGreen"],"surf":["GOLDEEN","GOLDEEN","STARYU","STARYU","STARYU"],"fish":["MAGIKARP","MAGIKARP","GOLDEEN","GOLDEEN","STARYU","GOLDEEN","STARYU","GOLDEEN","STARYU","STARYU"],"surfLv":[22,26,25,27,29],"fishingLv":[5,5,12,14,15,20,22,26,30,32],"waterRate":1,"fishingRate":10},"MAP_ROUTE19":{"labels":["sRoute19_FireRed","sRoute19_LeafGreen"],"surf":["TENTACOOL","TENTACOOL","TENTACRUEL","TENTACRUEL","TENTACRUEL"],"fish":["MAGIKARP","MAGIKARP","HORSEA","SHELLDER","KRABBY","HORSEA","SHELLDER","HORSEA","KRABBY","KRABBY"],"surfLv":[29,33,35,37,39],"fishingLv":[5,5,16,16,17,27,26,29,32,34],"waterRate":2,"fishingRate":20},"MAP_ROUTE20":{"labels":["sRoute20_FireRed","sRoute20_LeafGreen"],"surf":["TENTACOOL","TENTACOOL","TENTACRUEL","SPHEAL","SPHEAL"],"fish":["HORSEA","SHELLDER","HORSEA","SHELLDER","STARYU","HORSEA","SHELLDER","STARYU","STARYU","STARYU"],"surfLv":[30,34,36,36,38],"fishingLv":[5,5,16,16,17,27,26,29,32,34],"waterRate":2,"fishingRate":20},"MAP_ROUTE21_NORTH":{"labels":["sRoute21North_FireRed","sRoute21North_LeafGreen"],"surf":["TENTACOOL","TENTACOOL","TENTACRUEL","TENTACRUEL","TENTACRUEL"],"fish":["HORSEA","GOLDEEN","HORSEA","GOLDEEN","SHELLDER","HORSEA","SHELLDER","GOLDEEN","SHELLDER","SHELLDER"],"surfLv":[28,32,34,36,38],"fishingLv":[5,5,16,16,17,27,26,29,32,34],"waterRate":2,"fishingRate":20},"MAP_ROUTE21_SOUTH":{"labels":["sRoute21South_FireRed","sRoute21South_LeafGreen"],"surf":["TENTACOOL","TENTACOOL","TENTACRUEL","TENTACRUEL","TENTACRUEL"],"fish":["HORSEA","GOLDEEN","HORSEA","GOLDEEN","SHELLDER","HORSEA","SHELLDER","GOLDEEN","SHELLDER","SHELLDER"],"surfLv":[30,34,36,38,40],"fishingLv":[5,5,16,16,17,27,26,29,32,34],"waterRate":2,"fishingRate":20}}
for map_id,exp in expected.items():
 rows=[r for r in group["encounters"] if r["map"]==map_id]
 assert len(rows)==2 and {r["base_label"] for r in rows}==set(exp["labels"])
 for r in rows:
  for method,names,levels,rate in (("water_mons",exp["surf"],exp["surfLv"],exp["waterRate"]),("fishing_mons",exp["fish"],exp["fishingLv"],exp["fishingRate"])):
   fld=r[method]
   assert fld["encounter_rate"]==rate and len(fld["mons"])==len(names)
   for mon,sp,lv in zip(fld["mons"],names,levels):
    assert mon=={"min_level":lv,"max_level":lv,"species":"SPECIES_"+sp},(map_id,r["base_label"],method,mon,sp,lv)
 assert rows[0]["water_mons"]==rows[1]["water_mons"]
 assert rows[0]["fishing_mons"]==rows[1]["fishing_mons"]
print("PASS six-map Kanto Surf/fishing ecology 12 version records, 180 total method slots, rod tiers and preserved per-map trigger rates")
