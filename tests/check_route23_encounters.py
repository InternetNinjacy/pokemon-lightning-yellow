#!/usr/bin/env python3
"""Owner-delegated Route 23 late-game training encounters, untouched water methods."""
import json
from pathlib import Path
from collections import defaultdict

data=json.loads(Path("src/data/wild_encounters.json").read_text())
group=data["wild_encounter_groups"][0]
weights=next(x["encounter_rates"] for x in group["fields"] if x["type"]=="land_mons")
assert weights==[20,20,10,10,10,10,5,5,4,4,1,1]
target={
 "SPECIES_FEAROW":(30,{42,45}),
 "SPECIES_NIDORINO":(25,{40,44}),
 "SPECIES_NIDORINA":(25,{41,44,46}),
 "SPECIES_MANKEY":(20,{38,40,42,43,44}),
}
rows=[x for x in group["encounters"] if x["map"]=="MAP_ROUTE23"]
assert len(rows)==2
assert {x["base_label"] for x in rows}=={"sRoute23_FireRed","sRoute23_LeafGreen"}
for row in rows:
 land=row["land_mons"]
 assert land["encounter_rate"]==21 and len(land["mons"])==12
 pct=defaultdict(int);lv=defaultdict(set)
 for slot,weight in zip(land["mons"],weights):
  sp=slot["species"];assert sp in target
  assert slot["min_level"]==slot["max_level"]
  pct[sp]+=weight;lv[sp].add(slot["min_level"])
 for sp,(amount,levels) in target.items():
  assert pct[sp]==amount,(row["base_label"],sp,pct[sp])
  assert lv[sp]==levels,(row["base_label"],sp,lv[sp])
 assert "water_mons" in row and "fishing_mons" in row
assert rows[0]["land_mons"]==rows[1]["land_mons"]
print("PASS Route 23 pre-League land: Fearow30, Nidorino25, Nidorina25, Mankey20; levels38-46; original encounter-rate21; water/fishing retained")
