#!/usr/bin/env python3
"""WRL-009 Safari Zone source exact 8 tables; Tauros remains separate."""
import json
from pathlib import Path
from collections import defaultdict
root=Path(__file__).resolve().parents[1]
group=json.loads((root/"src/data/wild_encounters.json").read_text())["wild_encounter_groups"][0]
weights=next(x["encounter_rates"] for x in group["fields"] if x["type"]=="land_mons")
assert weights==[20,20,10,10,10,10,5,5,4,4,1,1]
expected={"MAP_SAFARI_ZONE_CENTER":{"labels":["sSafariZoneCenter_FireRed","sSafariZoneCenter_LeafGreen"],"rate":21,"slots":[{"min_level":23,"max_level":23,"species":"SPECIES_NIDORAN_F"},{"min_level":28,"max_level":28,"species":"SPECIES_NIDORAN_M"},{"min_level":29,"max_level":29,"species":"SPECIES_NIDORAN_F"},{"min_level":27,"max_level":27,"species":"SPECIES_NIDORAN_M"},{"min_level":25,"max_level":25,"species":"SPECIES_EXEGGCUTE"},{"min_level":28,"max_level":28,"species":"SPECIES_EXEGGCUTE"},{"min_level":24,"max_level":24,"species":"SPECIES_EXEGGCUTE"},{"min_level":29,"max_level":29,"species":"SPECIES_CHANSEY"},{"min_level":25,"max_level":25,"species":"SPECIES_CHANSEY"},{"min_level":28,"max_level":28,"species":"SPECIES_CHANSEY"},{"min_level":24,"max_level":24,"species":"SPECIES_CHANSEY"},{"min_level":27,"max_level":27,"species":"SPECIES_CHANSEY"}],"pcts":{"SPECIES_NIDORAN_F":30,"SPECIES_NIDORAN_M":30,"SPECIES_EXEGGCUTE":25,"SPECIES_CHANSEY":15}},"MAP_SAFARI_ZONE_EAST":{"labels":["sSafariZoneEast_FireRed","sSafariZoneEast_LeafGreen"],"rate":21,"slots":[{"min_level":24,"max_level":24,"species":"SPECIES_KANGASKHAN"},{"min_level":29,"max_level":29,"species":"SPECIES_SCYTHER"},{"min_level":27,"max_level":27,"species":"SPECIES_PARAS"},{"min_level":30,"max_level":30,"species":"SPECIES_PARAS"},{"min_level":26,"max_level":26,"species":"SPECIES_PARAS"},{"min_level":24,"max_level":24,"species":"SPECIES_KECLEON"},{"min_level":30,"max_level":30,"species":"SPECIES_SCYTHER"},{"min_level":28,"max_level":28,"species":"SPECIES_PARAS"},{"min_level":26,"max_level":26,"species":"SPECIES_KECLEON"},{"min_level":29,"max_level":29,"species":"SPECIES_KECLEON"},{"min_level":25,"max_level":25,"species":"SPECIES_KECLEON"},{"min_level":28,"max_level":28,"species":"SPECIES_KECLEON"}],"pcts":{"SPECIES_KANGASKHAN":20,"SPECIES_SCYTHER":25,"SPECIES_PARAS":35,"SPECIES_KECLEON":20}},"MAP_SAFARI_ZONE_NORTH":{"labels":["sSafariZoneNorth_FireRed","sSafariZoneNorth_LeafGreen"],"rate":21,"slots":[{"min_level":25,"max_level":25,"species":"SPECIES_RHYHORN"},{"min_level":30,"max_level":30,"species":"SPECIES_PHANPY"},{"min_level":31,"max_level":31,"species":"SPECIES_RHYHORN"},{"min_level":29,"max_level":29,"species":"SPECIES_PHANPY"},{"min_level":27,"max_level":27,"species":"SPECIES_PINSIR"},{"min_level":25,"max_level":25,"species":"SPECIES_VENONAT"},{"min_level":29,"max_level":29,"species":"SPECIES_RHYHORN"},{"min_level":29,"max_level":29,"species":"SPECIES_PINSIR"},{"min_level":27,"max_level":27,"species":"SPECIES_VENONAT"},{"min_level":30,"max_level":30,"species":"SPECIES_VENONAT"},{"min_level":26,"max_level":26,"species":"SPECIES_VENONAT"},{"min_level":29,"max_level":29,"species":"SPECIES_VENONAT"}],"pcts":{"SPECIES_RHYHORN":35,"SPECIES_PHANPY":30,"SPECIES_PINSIR":15,"SPECIES_VENONAT":20}},"MAP_SAFARI_ZONE_WEST":{"labels":["sSafariZoneWest_FireRed","sSafariZoneWest_LeafGreen"],"rate":21,"slots":[{"min_level":25,"max_level":25,"species":"SPECIES_DODUO"},{"min_level":28,"max_level":28,"species":"SPECIES_DODUO"},{"min_level":31,"max_level":31,"species":"SPECIES_DODUO"},{"min_level":29,"max_level":29,"species":"SPECIES_PSYDUCK"},{"min_level":25,"max_level":25,"species":"SPECIES_PSYDUCK"},{"min_level":28,"max_level":28,"species":"SPECIES_PSYDUCK"},{"min_level":29,"max_level":29,"species":"SPECIES_DODUO"},{"min_level":27,"max_level":27,"species":"SPECIES_PSYDUCK"},{"min_level":30,"max_level":30,"species":"SPECIES_PSYDUCK"},{"min_level":26,"max_level":26,"species":"SPECIES_PSYDUCK"},{"min_level":29,"max_level":29,"species":"SPECIES_PSYDUCK"},{"min_level":25,"max_level":25,"species":"SPECIES_PSYDUCK"}],"pcts":{"SPECIES_DODUO":55,"SPECIES_PSYDUCK":45}}}
for m,e in expected.items():
 rows=[r for r in group["encounters"] if r["map"]==m]
 assert len(rows)==2 and {r["base_label"] for r in rows}==set(e["labels"])
 for row in rows:
  land=row["land_mons"]
  assert land["encounter_rate"]==e["rate"] and len(land["mons"])==12
  count=defaultdict(int)
  for slot,w,target in zip(land["mons"],weights,e["slots"]):
   assert slot==target,(m,row["base_label"],slot,target)
   count[slot["species"]]+=w
  assert dict(count)==e["pcts"],(m,count)
  assert "SPECIES_TAUROS" not in str(land)
 assert rows[0]["land_mons"]==rows[1]["land_mons"]
print("PASS four Safari Zone sectors, eight land version tables, no Tauros grass")
