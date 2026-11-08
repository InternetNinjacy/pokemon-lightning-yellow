#!/usr/bin/env python3
"""WRL-009 Route 8 Drowzee first-family-access, delegated ecology source audit."""
from collections import defaultdict
import hashlib
import json
from pathlib import Path
from approved_route8_drowzee_changes import DELEGATED_ROUTE8, restore_route8_text

ROOT=Path(__file__).resolve().parents[1]
text=(ROOT/"src/data/wild_encounters.json").read_text(encoding="utf-8")
groups=json.loads(text)["wild_encounter_groups"]
assert len(groups)==1
weights=next(f["encounter_rates"] for f in groups[0]["fields"] if f["type"]=="land_mons")
assert weights==[20,20,10,10,10,10,5,5,4,4,1,1]
rows=[e for e in groups[0]["encounters"] if e["map"]=="MAP_ROUTE8"]
assert len(rows)==2
expected={"SPECIES_PONYTA":25,"SPECIES_HOUNDOUR":30,"SPECIES_CACNEA":20,
          "SPECIES_NATU":15,"SPECIES_DROWZEE":10}
for row in rows:
    assert row["base_label"] in DELEGATED_ROUTE8
    land=row["land_mons"]
    assert land["encounter_rate"]==21 and len(land["mons"])==12
    actual=[[x["min_level"],x["max_level"],x["species"]] for x in land["mons"]]
    assert actual==DELEGATED_ROUTE8[row["base_label"]]
    totals=defaultdict(int)
    for weight,slot in zip(weights,land["mons"]):
        assert slot["species"] in expected and 1<=slot["min_level"]<=slot["max_level"]<=100
        totals[slot["species"]]+=weight
    assert dict(totals)==expected,(row["base_label"],totals)
assert rows[0]["land_mons"]==rows[1]["land_mons"]

# Before this approved delegated two-slot update, Route 5 PR #142 plus native
# PR #141 was the exact parent source. The SHA is a Git blob checksum, not a
# comparison to a mutable branch reference.
previous=restore_route8_text("src/data/wild_encounters.json",text).encode("utf-8")
blob=b"blob "+str(len(previous)).encode("ascii")+b"\0"+previous
assert hashlib.sha1(blob).hexdigest()=="c76e68dffb182057c59cb2e24b61058e9713c5ea", (
    "Route 8 Drowzee addition changed unrelated wild source")
print("PASS: Route 8 FR/LG Drowzee 10% Lv20, Ponyta25/Houndour30/Cacnea20/Natu15, "
      "strict exact-source delta; locked Route 11 roster untouched")
print("NOTE: Drowzee is source-placed; actual walkable map accessibility is separate native QA")
