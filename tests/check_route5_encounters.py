#!/usr/bin/env python3
"""WRL-009 exact approved Route 5 Ditto/Skitty/Rattata ordinary ecology contract.

The owner locked species presence; rates/levels below are assistant-delegated
source implementation choices, not new individual owner approvals.
"""
from collections import defaultdict
import hashlib
import json
from pathlib import Path
from approved_route5_changes import APPROVED_ROUTE5, restore_route5_text
from approved_route8_drowzee_changes import restore_route8_text

ROOT = Path(__file__).resolve().parents[1]
path = ROOT / "src/data/wild_encounters.json"
text = path.read_text(encoding="utf-8")
source = json.loads(text)
assert len(source["wild_encounter_groups"]) == 1
group = source["wild_encounter_groups"][0]
weights = next(x["encounter_rates"] for x in group["fields"] if x["type"] == "land_mons")
assert weights == [20,20,10,10,10,10,5,5,4,4,1,1] and sum(weights) == 100
rows = [r for r in group["encounters"] if r["map"] == "MAP_ROUTE5"]
assert len(rows) == 2
assert {r["base_label"] for r in rows} == {"sRoute5_FireRed","sRoute5_LeafGreen"}
expected_percent = {"SPECIES_RATTATA":50,"SPECIES_SKITTY":40,"SPECIES_DITTO":10}
expected_levels = {
    "SPECIES_RATTATA": {10,11,12,13},
    "SPECIES_SKITTY": {12,13,14},
    "SPECIES_DITTO": {13,14,15},
}
for row in rows:
    assert set(row) == {"map","base_label","land_mons"}
    land = row["land_mons"]
    assert land["encounter_rate"] == 21 and len(land["mons"]) == 12
    pct,levels = defaultdict(int),defaultdict(set)
    actual = []
    for w,slot in zip(weights,land["mons"]):
        sp = slot["species"];lo,hi = slot["min_level"],slot["max_level"]
        assert sp in expected_percent, sp
        assert 1 <= lo <= hi <= 100
        pct[sp] += w
        levels[sp].update(range(lo,hi+1))
        actual.append([lo,hi,sp])
    assert actual == APPROVED_ROUTE5[row["base_label"]]
    assert dict(pct) == expected_percent, (row["base_label"],pct)
    assert dict(levels) == expected_levels, (row["base_label"],levels)
assert rows[0]["land_mons"] == rows[1]["land_mons"]

# Immutable source scope gate: restore precisely the two expected Route 5
# blocks, then require byte-identical original merged-main source.
original = restore_route5_text("src/data/wild_encounters.json",
                               restore_route8_text("src/data/wild_encounters.json", text)).encode("utf-8")
blob = b"blob " + str(len(original)).encode("ascii") + b"\0" + original
assert hashlib.sha1(blob).hexdigest() == "2f871858046a4dc6bec1e921a39c0dac893d2d8c", (
    "Encounter data changed outside the approved Route 5 delta")
print("PASS: Route 5 FR/LG exact three-species roster, encounter rate 21, "
      "Rattata 50% Lv10-13 / Skitty 40% Lv12-14 / Ditto 10% Lv13-15, "
      "strict original-source restoration")
print("NOTE: no guarantee of physical grass-accessibility until native map QA")
