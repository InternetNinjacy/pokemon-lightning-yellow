#!/usr/bin/env python3
"""WRL-009: approved Route 1 ordinary grass encounter contract.

Source: November 7, 2026 owner approval, Route 1 50/40/10, levels 2-5/2-5/3-4.
This verifies source table contents, not real encounter triggering or experimental
wild-double special-pair overrides, which remain separately tracked.
"""
import json
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
source = json.loads((ROOT / "src/data/wild_encounters.json").read_text(encoding="utf-8"))
groups = source["wild_encounter_groups"]
assert len(groups) == 1, "Review additional encounter groups before expanding scope"
group = groups[0]
weights = next(f["encounter_rates"] for f in group["fields"] if f["type"] == "land_mons")
assert weights == [20, 20, 10, 10, 10, 10, 5, 5, 4, 4, 1, 1]
assert sum(weights) == 100

expected = {
    "SPECIES_PIDGEY": (50, {2, 3, 4, 5}),
    "SPECIES_RATTATA": (40, {2, 3, 4, 5}),
    "SPECIES_IGGLYBUFF": (10, {3, 4}),
}
entries = [e for e in group["encounters"] if e["map"] == "MAP_ROUTE1"]
assert {e["base_label"] for e in entries} == {"sRoute1_FireRed", "sRoute1_LeafGreen"}
assert len(entries) == 2
tables = []
for entry in entries:
    assert set(entry).issuperset({"map", "base_label", "land_mons"})
    assert "water_mons" not in entry and "fishing_mons" not in entry
    land = entry["land_mons"]
    assert land["encounter_rate"] == 21
    assert len(land["mons"]) == len(weights) == 12
    percent = defaultdict(int)
    levels = defaultdict(set)
    for weight, slot in zip(weights, land["mons"]):
        species = slot["species"]
        assert species in expected, f"Unapproved Route 1 wild species: {species}"
        lo, hi = slot["min_level"], slot["max_level"]
        assert isinstance(lo, int) and isinstance(hi, int) and 1 <= lo <= hi <= 100
        percent[species] += weight
        levels[species].update(range(lo, hi + 1))
    assert set(percent) == set(expected)
    for species, (wanted_percent, wanted_levels) in expected.items():
        assert percent[species] == wanted_percent, (entry["base_label"], species, percent[species])
        assert levels[species] == wanted_levels, (entry["base_label"], species, levels[species])
    tables.append(land)
assert tables[0] == tables[1], "FireRed / LeafGreen Route 1 table drift"
print("PASS: Route 1 FireRed and LeafGreen ordinary grass, 12 slots, 21 encounter rate, Pidgey 50% Lv2-5, Rattata 40% Lv2-5, Igglybuff 10% Lv3-4")
print("NOTE: experimental wild-double special pair and emulator gameplay are NOT certified")
