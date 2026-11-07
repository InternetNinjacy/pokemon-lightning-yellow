#!/usr/bin/env python3
"""WRL-009 exact Mt. Moon 1F/B1F/B2F 252-Dex ordinary cave ecology contract."""
import json
from collections import defaultdict
from pathlib import Path

root=Path(__file__).resolve().parents[1]
group=json.loads((root/"src/data/wild_encounters.json").read_text())["wild_encounter_groups"][0]
weights=next(f["encounter_rates"] for f in group["fields"] if f["type"]=="land_mons")
assert weights==[20,20,10,10,10,10,5,5,4,4,1,1] and sum(weights)==100
expected={
  "1F": {
    "rate": 7,
    "species": {
      "SPECIES_ZUBAT": 40,
      "SPECIES_GEODUDE": 30,
      "SPECIES_PARAS": 20,
      "SPECIES_CLEFFA": 10
    },
    "levels": {
      "SPECIES_ZUBAT": [
        7,
        8
      ],
      "SPECIES_GEODUDE": [
        7,
        8,
        9
      ],
      "SPECIES_PARAS": [
        7,
        8,
        9,
        10
      ],
      "SPECIES_CLEFFA": [
        7,
        8,
        9
      ]
    }
  },
  "B1F": {
    "rate": 5,
    "species": {
      "SPECIES_ZUBAT": 35,
      "SPECIES_PARAS": 30,
      "SPECIES_GEODUDE": 25,
      "SPECIES_SWINUB": 10
    },
    "levels": {
      "SPECIES_ZUBAT": [
        8,
        9,
        10
      ],
      "SPECIES_PARAS": [
        8,
        9
      ],
      "SPECIES_GEODUDE": [
        9,
        10,
        11
      ],
      "SPECIES_SWINUB": [
        8,
        9,
        10,
        11
      ]
    }
  },
  "B2F": {
    "rate": 7,
    "species": {
      "SPECIES_SMOOCHUM": 30,
      "SPECIES_SNEASEL": 25,
      "SPECIES_SWINUB": 25,
      "SPECIES_SNORUNT": 20
    },
    "levels": {
      "SPECIES_SMOOCHUM": [
        10,
        12
      ],
      "SPECIES_SNEASEL": [
        11,
        12
      ],
      "SPECIES_SWINUB": [
        10,
        12,
        13
      ],
      "SPECIES_SNORUNT": [
        10,
        11,
        12,
        13
      ]
    }
  }
} 
rows=[x for x in group["encounters"] if x["map"] in {"MAP_MT_MOON_1F","MAP_MT_MOON_B1F","MAP_MT_MOON_B2F"}]
assert len(rows)==6
for floor,expect in expected.items():
 selections=[x for x in rows if x["map"]=="MAP_MT_MOON_"+floor]
 assert len(selections)==2 and {x["base_label"] for x in selections}=={"sMtMoon"+floor+"_FireRed","sMtMoon"+floor+"_LeafGreen"}
 for row in selections:
  assert set(row)=={"map","base_label","land_mons"}
  land=row["land_mons"]
  assert land["encounter_rate"]==expect["rate"] and len(land["mons"])==12
  pct=defaultdict(int);levels=defaultdict(set)
  for slot,weight in zip(land["mons"],weights):
   name=slot["species"];assert name in expect["species"],("unapproved Mt Moon species",floor,name)
   low,high=slot["min_level"],slot["max_level"]
   assert isinstance(low,int) and isinstance(high,int) and 1<=low==high<=100
   pct[name]+=weight;levels[name].add(low)
  assert dict(pct)==expect["species"],(floor,pct)
  for name,lv in expect["levels"].items():
   assert levels[name]==set(lv),(floor,name,levels[name])
 assert selections[0]["land_mons"]==selections[1]["land_mons"]
print("PASS Mt. Moon: exact approved four species per floor, exact levels/weights, cave frequency 7/5/7, version parity, no ordinary Onix")
