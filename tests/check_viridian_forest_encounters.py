#!/usr/bin/env python3
"""WRL-009 Viridian Forest approved five-species ecology and exact engine-slot contract."""
import json
from collections import defaultdict
from pathlib import Path

root=Path(__file__).resolve().parents[1]
group=json.loads((root/"src/data/wild_encounters.json").read_text())["wild_encounter_groups"][0]
weights=next(field["encounter_rates"] for field in group["fields"] if field["type"]=="land_mons")
assert weights==[20,20,10,10,10,10,5,5,4,4,1,1] and sum(weights)==100
expected={
 "SPECIES_CATERPIE":(30,{4,5}),
 "SPECIES_WEEDLE":(30,{4,5}),
 "SPECIES_SHROOMISH":(15,{4,5}),
 "SPECIES_ELEKID":(15,{4,5}),
 "SPECIES_PINECO":(10,{4,5,6}),
}
rows=[x for x in group["encounters"] if x["map"]=="MAP_VIRIDIAN_FOREST"]
assert len(rows)==2
assert {x["base_label"] for x in rows}=={"sViridianForest_FireRed","sViridianForest_LeafGreen"}
for row in rows:
 assert set(row)=={"map","base_label","land_mons"}
 land=row["land_mons"]
 assert land["encounter_rate"]==14 and len(land["mons"])==12
 totals=defaultdict(int);levels=defaultdict(set)
 for slot,weight in zip(land["mons"],weights):
  sp=slot["species"]
  assert sp in expected,("unapproved Forest wild species",sp)
  low,high=slot["min_level"],slot["max_level"]
  assert isinstance(low,int) and isinstance(high,int) and 1<=low<=high<=100
  totals[sp]+=weight
  levels[sp].update(range(low,high+1))
 assert set(totals)==set(expected)
 for sp,(pct,lvs) in expected.items():
  assert totals[sp]==pct and levels[sp]==lvs,(row["base_label"],sp,totals[sp],levels[sp])
assert rows[0]["land_mons"]==rows[1]["land_mons"]
print("PASS Viridian Forest Caterpie30 Weedle30 Shroomish15 Elekid15 Pineco10, Lv4–6, frequency14, both versions")
