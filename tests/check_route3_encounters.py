#!/usr/bin/env python3
"""WRL-009: exact Route 3 approved ecology and delegated FireRed-era balance contract."""
import json
from pathlib import Path
from collections import defaultdict

root=Path(__file__).resolve().parents[1]
group=json.loads((root/"src/data/wild_encounters.json").read_text())["wild_encounter_groups"][0]
weights=next(f["encounter_rates"] for f in group["fields"] if f["type"]=="land_mons")
assert weights==[20,20,10,10,10,10,5,5,4,4,1,1] and sum(weights)==100
expected={
 "SPECIES_MEOWTH":(30,{6,7}),
 "SPECIES_SUNKERN":(25,{6,7}),
 "SPECIES_SLUGMA":(20,{7,8}),
 "SPECIES_MAGBY":(15,{7,8}),
 "SPECIES_CASTFORM":(10,{7,8,9}),
}
rows=[r for r in group["encounters"] if r["map"]=="MAP_ROUTE3"]
assert len(rows)==2 and {r["base_label"] for r in rows}=={"sRoute3_FireRed","sRoute3_LeafGreen"}
for row in rows:
 assert set(row)=={"map","base_label","land_mons"}
 land=row["land_mons"]
 assert land["encounter_rate"]==21 and len(land["mons"])==12
 pct=defaultdict(int);levels=defaultdict(set)
 for slot,weight in zip(land["mons"],weights):
  sp=slot["species"];assert sp in expected
  low,high=slot["min_level"],slot["max_level"]
  assert isinstance(low,int) and isinstance(high,int) and 1<=low<=high<=100
  pct[sp]+=weight;levels[sp].update(range(low,high+1))
 assert set(pct)==set(expected)
 for sp,(weight,lv) in expected.items():
  assert pct[sp]==weight and levels[sp]==lv,(row["base_label"],sp,pct[sp],levels[sp])
assert rows[0]["land_mons"]==rows[1]["land_mons"]
print("PASS Route 3 two versions: Meowth30 Sunkern25 Slugma20 Magby15 Castform10, Lv6-9, trigger21")
