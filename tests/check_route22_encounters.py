#!/usr/bin/env python3
"""WRL-009 owner-delegated Route 22 balanced early-game source contract."""
import json
from pathlib import Path
from collections import defaultdict

data=json.loads(Path("src/data/wild_encounters.json").read_text())
group=data["wild_encounter_groups"][0]
weights=next(x["encounter_rates"] for x in group["fields"] if x["type"]=="land_mons")
assert weights==[20,20,10,10,10,10,5,5,4,4,1,1]
target={
 "SPECIES_SPEAROW":(30,{3,4}),
 "SPECIES_NIDORAN_F":(25,{3,4}),
 "SPECIES_NIDORAN_M":(20,{3,4}),
 "SPECIES_MANKEY":(15,{3,5}),
 "SPECIES_TYROGUE":(10,{4,5}),
}
rows=[x for x in group["encounters"] if x["map"]=="MAP_ROUTE22"]
assert len(rows)==2
assert {x["base_label"] for x in rows}=={"sRoute22_FireRed","sRoute22_LeafGreen"}
for row in rows:
 land=row["land_mons"];assert land["encounter_rate"]==21 and len(land["mons"])==12
 counts=defaultdict(int);levels=defaultdict(set)
 for slot,weight in zip(land["mons"],weights):
  name=slot["species"];assert name in target
  assert 1<=slot["min_level"]<=slot["max_level"]<=100
  counts[name]+=weight
  levels[name].update(range(slot["min_level"],slot["max_level"]+1))
 for name,(pct,expected_levels) in target.items():
  assert counts[name]==pct,(row["base_label"],name,counts[name])
  assert levels[name]==expected_levels,(row["base_label"],name,levels[name])
assert rows[0]["land_mons"]==rows[1]["land_mons"]
print("PASS Route 22 delegated land: Spearow30 NidoranF25 NidoranM20 Mankey15 Tyrogue10; levels and both versions")
