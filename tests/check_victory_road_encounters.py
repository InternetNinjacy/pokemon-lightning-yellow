#!/usr/bin/env python3
"""Source contract for all three owner-delegated Victory Road experience-yield floors."""
from pathlib import Path
from collections import defaultdict
import json,re

data=json.loads(Path("src/data/wild_encounters.json").read_text())
group=data["wild_encounter_groups"][0]
weights=next(x["encounter_rates"] for x in group["fields"] if x["type"]=="land_mons")
assert weights==[20,20,10,10,10,10,5,5,4,4,1,1]
expected={
 "1F": {"MACHOKE":(30,{42,44}),"GRAVELER":(25,{42,46}),"ONIX":(20,{42,45}),"ARON":(15,{41,45}),"RHYHORN":(10,{43,46})},
 "2F": {"MACHOKE":(25,{44,48}),"GRAVELER":(25,{43,49}),"LAIRON":(20,{44,47}),"RHYHORN":(20,{43,47}),"LARVITAR":(10,{44,47,48})},
 "3F": {"LAIRON":(25,{46,50}),"GRAVELER":(25,{45,49}),"MACHOKE":(20,{44,48}),"ABSOL":(15,{46,48,50}),"LARVITAR":(15,{44,47,49})}
}
rows=[x for x in group["encounters"] if re.fullmatch("MAP_VICTORY_ROAD_[123]F",x["map"])]
assert len(rows)==6
for floor,target in expected.items():
 version=[x for x in rows if x["map"]=="MAP_VICTORY_ROAD_"+floor]
 assert len(version)==2 and {x["base_label"] for x in version}=={"sVictoryRoad"+floor+"_FireRed","sVictoryRoad"+floor+"_LeafGreen"}
 for entry in version:
  assert set(entry)=={"map","base_label","land_mons"}
  land=entry["land_mons"]
  assert land["encounter_rate"]==7 and len(land["mons"])==12
  pct=defaultdict(int);levels=defaultdict(set)
  for mon,weight in zip(land["mons"],weights):
   species=mon["species"].removeprefix("SPECIES_")
   assert species in target
   assert mon["min_level"]==mon["max_level"]
   pct[species]+=weight;levels[species].add(mon["min_level"])
  for sp,(rate,lvs) in target.items():
   assert pct[sp]==rate,(entry["base_label"],sp,pct[sp])
   assert levels[sp]==lvs,(entry["base_label"],sp,levels[sp])
 assert version[0]["land_mons"]==version[1]["land_mons"]
print("PASS Victory Road 1F/2F/3F version-parity, exact species probabilities, Lv41–50, unchanged cave trigger setting 7")
