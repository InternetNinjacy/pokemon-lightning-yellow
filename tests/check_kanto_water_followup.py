#!/usr/bin/env python3
"""WRL-009 Kanto freshwater/coastal water followup: exact rods and Seafoam map records."""
import json
from pathlib import Path
root=Path(__file__).resolve().parents[1]
group=json.loads((root/"src/data/wild_encounters.json").read_text())["wild_encounter_groups"][0]
fields={f["type"]:f for f in group["fields"]}
assert fields["water_mons"]["encounter_rates"]==[60,30,5,4,1]
assert fields["fishing_mons"]["encounter_rates"]==[70,30,60,20,20,40,40,15,4,1]
assert fields["fishing_mons"]["groups"]=={"old_rod":[0,1],"good_rod":[2,3,4],"super_rod":[5,6,7,8,9]}
expected={"MAP_ROUTE12":{"labels":["sRoute12_FireRed","sRoute12_LeafGreen"],"surf":["KRABBY","CORPHISH","CORPHISH","KRABBY","CORPHISH"],"fish":["KRABBY","KRABBY","CORPHISH","KRABBY","CORPHISH","CORPHISH","CARVANHA","FEEBAS","FEEBAS","CARVANHA"],"surfLevels":[24,27,30,31,33],"fishLevels":[5,5,16,17,18,26,28,30,34,36],"waterRate":2,"fishRate":60},"MAP_ROUTE23":{"labels":["sRoute23_FireRed","sRoute23_LeafGreen"],"surf":["GOLDEEN","POLIWAG","GOLDEEN","POLIWAG","POLIWAG"],"fish":["MAGIKARP","MAGIKARP","POLIWAG","GOLDEEN","POLIWAG","POLIWAG","GOLDEEN","GOLDEEN","GOLDEEN","POLIWAG"],"surfLevels":[36,38,40,42,44],"fishLevels":[5,5,16,17,18,26,28,30,34,36],"waterRate":2,"fishRate":20},"MAP_FUCHSIA_CITY":{"labels":["sFuchsiaCity_FireRed","sFuchsiaCity_LeafGreen"],"surf":["TENTACOOL","TENTACOOL","TENTACRUEL","TENTACRUEL","TENTACRUEL"],"fish":["MAGIKARP","MAGIKARP","HORSEA","SHELLDER","KRABBY","HORSEA","SHELLDER","HORSEA","KRABBY","SHELLDER"],"surfLevels":[28,32,34,36,38],"fishLevels":[5,5,16,17,18,26,28,30,34,36],"waterRate":1,"fishRate":10},"MAP_SEAFOAM_ISLANDS_B3F":{"labels":["sSeafoamIslandsB3F_FireRed","sSeafoamIslandsB3F_LeafGreen"],"surf":["SEEL","SPHEAL","DEWGONG","SEALEO","SHELLDER"],"fish":["SHELLDER","SHELLDER","SEEL","SPHEAL","SHELLDER","SEEL","SPHEAL","DEWGONG","SEALEO","SHELLDER"],"surfLevels":[31,33,36,38,40],"fishLevels":[5,5,16,17,18,26,28,30,34,36],"waterRate":2,"fishRate":20},"MAP_SEAFOAM_ISLANDS_B4F":{"labels":["sSeafoamIslandsB4F_FireRed","sSeafoamIslandsB4F_LeafGreen"],"surf":["SEEL","SPHEAL","DEWGONG","SEALEO","SHELLDER"],"fish":["SHELLDER","SHELLDER","SEEL","SPHEAL","SHELLDER","SEEL","SPHEAL","DEWGONG","SEALEO","SHELLDER"],"surfLevels":[34,36,39,41,43],"fishLevels":[5,5,16,17,18,26,28,30,34,36],"waterRate":2,"fishRate":20}}
for map_id,config in expected.items():
 entries=[r for r in group["encounters"] if r["map"]==map_id]
 assert len(entries)==2 and {x["base_label"] for x in entries}==set(config["labels"])
 for row in entries:
  for method,names,levels,rate in (("water_mons",config["surf"],config["surfLevels"],config["waterRate"]),("fishing_mons",config["fish"],config["fishLevels"],config["fishRate"])):
   data=row[method]
   assert data["encounter_rate"]==rate and len(data["mons"])==len(names)
   for mon,name,lv in zip(data["mons"],names,levels):
    assert mon=={"min_level":lv,"max_level":lv,"species":"SPECIES_"+name},(map_id,row["base_label"],method,mon)
 assert entries[0]["water_mons"]==entries[1]["water_mons"]
 assert entries[0]["fishing_mons"]==entries[1]["fishing_mons"]
print("PASS 5 Kanto water maps 10 map-version records 20 independent Surf/fishing tables")
