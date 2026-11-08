#!/usr/bin/env python3
"""WRL-009 Sevii regional aquatic source contract for 8 distinct map headers."""
import json
from pathlib import Path
root=Path(__file__).resolve().parents[1]
group=json.loads((root/"src/data/wild_encounters.json").read_text())["wild_encounter_groups"][0]
f={x["type"]:x for x in group["fields"]}
assert f["water_mons"]["encounter_rates"]==[60,30,5,4,1]
assert f["fishing_mons"]["encounter_rates"]==[70,30,60,20,20,40,40,15,4,1]
assert f["fishing_mons"]["groups"]=={"old_rod":[0,1],"good_rod":[2,3,4],"super_rod":[5,6,7,8,9]}
expected={"MAP_ONE_ISLAND_KINDLE_ROAD":{"labels":["sOneIslandKindleRoad_FireRed","sOneIslandKindleRoad_LeafGreen"],"surf":["TENTACOOL","TENTACOOL","TENTACRUEL","TENTACRUEL","TENTACRUEL"],"fish":["MAGIKARP","MAGIKARP","HORSEA","KRABBY","HORSEA","KRABBY","HORSEA","KRABBY","HORSEA","KRABBY"],"surfLv":[33,36,39,42,44],"fishLv":[5,5,18,19,20,29,31,33,36,39],"waterRate":2,"fishRate":20},"MAP_TWO_ISLAND_CAPE_BRINK":{"labels":["sTwoIslandCapeBrink_FireRed","sTwoIslandCapeBrink_LeafGreen"],"surf":["POLIWAG","GOLDEEN","PSYDUCK","POLIWAG","GOLDEEN"],"fish":["MAGIKARP","MAGIKARP","POLIWAG","GOLDEEN","PSYDUCK","POLIWAG","GOLDEEN","PSYDUCK","GOLDEEN","POLIWAG"],"surfLv":[33,36,39,42,44],"fishLv":[5,5,18,19,20,29,31,33,36,39],"waterRate":2,"fishRate":20},"MAP_THREE_ISLAND_BOND_BRIDGE":{"labels":["sThreeIslandBondBridge_FireRed","sThreeIslandBondBridge_LeafGreen"],"surf":["KRABBY","KRABBY","HORSEA","HORSEA","HORSEA"],"fish":["MAGIKARP","MAGIKARP","KRABBY","HORSEA","KRABBY","HORSEA","KRABBY","HORSEA","KRABBY","HORSEA"],"surfLv":[33,36,39,42,44],"fishLv":[5,5,18,19,20,29,31,33,36,39],"waterRate":2,"fishRate":20},"MAP_FIVE_ISLAND_RESORT_GORGEOUS":{"labels":["sFiveIslandResortGorgeous_FireRed","sFiveIslandResortGorgeous_LeafGreen"],"surf":["TENTACOOL","TENTACOOL","TENTACRUEL","TENTACRUEL","TENTACRUEL"],"fish":["MAGIKARP","MAGIKARP","HORSEA","STARYU","HORSEA","STARYU","HORSEA","STARYU","STARYU","HORSEA"],"surfLv":[33,36,39,42,44],"fishLv":[5,5,18,19,20,29,31,33,36,39],"waterRate":2,"fishRate":20},"MAP_FIVE_ISLAND_WATER_LABYRINTH":{"labels":["sFiveIslandWaterLabyrinth_FireRed","sFiveIslandWaterLabyrinth_LeafGreen"],"surf":["TENTACOOL","TENTACOOL","TENTACRUEL","TENTACRUEL","TENTACRUEL"],"fish":["MAGIKARP","MAGIKARP","HORSEA","STARYU","HORSEA","HORSEA","STARYU","HORSEA","STARYU","STARYU"],"surfLv":[33,36,39,42,44],"fishLv":[5,5,18,19,20,29,31,33,36,39],"waterRate":2,"fishRate":20},"MAP_SIX_ISLAND_GREEN_PATH":{"labels":["sSixIslandGreenPath_FireRed","sSixIslandGreenPath_LeafGreen"],"surf":["KRABBY","KRABBY","CORPHISH","CORPHISH","CORPHISH"],"fish":["MAGIKARP","MAGIKARP","KRABBY","CORPHISH","CARVANHA","KRABBY","CORPHISH","CARVANHA","CARVANHA","CORPHISH"],"surfLv":[33,36,39,42,44],"fishLv":[5,5,18,19,20,29,31,33,36,39],"waterRate":2,"fishRate":20},"MAP_SIX_ISLAND_WATER_PATH":{"labels":["sSixIslandWaterPath_FireRed","sSixIslandWaterPath_LeafGreen"],"surf":["KRABBY","KRABBY","CORPHISH","CORPHISH","CORPHISH"],"fish":["MAGIKARP","MAGIKARP","KRABBY","CORPHISH","CARVANHA","KRABBY","CORPHISH","CARVANHA","CARVANHA","CORPHISH"],"surfLv":[33,36,39,42,44],"fishLv":[5,5,18,19,20,29,31,33,36,39],"waterRate":2,"fishRate":20},"MAP_SEVEN_ISLAND_TANOBY_RUINS":{"labels":["sSevenIslandTanobyRuins_FireRed","sSevenIslandTanobyRuins_LeafGreen"],"surf":["TENTACOOL","TENTACOOL","TENTACRUEL","TENTACRUEL","TENTACRUEL"],"fish":["MAGIKARP","MAGIKARP","HORSEA","HORSEA","HORSEA","HORSEA","HORSEA","HORSEA","HORSEA","HORSEA"],"surfLv":[33,36,39,42,44],"fishLv":[5,5,18,19,20,29,31,33,36,39],"waterRate":2,"fishRate":20}}
for m,c in expected.items():
 rows=[r for r in group["encounters"] if r["map"]==m]
 assert len(rows)==2 and {r["base_label"] for r in rows}==set(c["labels"])
 for row in rows:
  for method,names,levels,rate in (("water_mons",c["surf"],c["surfLv"],c["waterRate"]),("fishing_mons",c["fish"],c["fishLv"],c["fishRate"])):
   d=row[method]
   assert d["encounter_rate"]==rate and len(d["mons"])==len(names)
   for item,name,level in zip(d["mons"],names,levels):
    assert item=={"min_level":level,"max_level":level,"species":"SPECIES_"+name},(m,row["base_label"],method,item)
 assert rows[0]["water_mons"]==rows[1]["water_mons"]
 assert rows[0]["fishing_mons"]==rows[1]["fishing_mons"]
print("PASS eight Sevii coast map headers, 16 source map/version records, 32 independent Surf and rod tables")
