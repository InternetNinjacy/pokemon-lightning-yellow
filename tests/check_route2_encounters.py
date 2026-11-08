#!/usr/bin/env python3
"""Reviewable WRL-009 Route 2 balanced encounter candidate contract.

The owner approved a general player-friendly rarity policy on 2026-11-07.
This candidate's specific 40/35/25 distribution and levels remain reviewable;
this test proves the source matches the candidate, not owner sign-off.
"""
from pathlib import Path
from collections import defaultdict
import json

root = Path(__file__).resolve().parents[1]
data = json.loads((root / "src/data/wild_encounters.json").read_text())
group = data["wild_encounter_groups"][0]
weights = next(f["encounter_rates"] for f in group["fields"] if f["type"] == "land_mons")
assert weights == [20,20,10,10,10,10,5,5,4,4,1,1] and sum(weights) == 100
expected = {
    "SPECIES_HOOTHOOT": (40, {3,4,5}),
    "SPECIES_TEDDIURSA": (35, {3,4,5}),
    "SPECIES_PICHU": (25, {2,3,4}),
}
entries = [x for x in group["encounters"] if x["map"] == "MAP_ROUTE2"]
assert len(entries) == 2
assert {x["base_label"] for x in entries} == {"sRoute2_FireRed", "sRoute2_LeafGreen"}
for e in entries:
    land = e["land_mons"]
    assert land["encounter_rate"] == 21
    assert len(land["mons"]) == 12
    pct = defaultdict(int)
    levels = defaultdict(set)
    for slot, weight in zip(land["mons"], weights):
        name = slot["species"]
        assert name in expected
        lo,hi = slot["min_level"],slot["max_level"]
        assert 1 <= lo <= hi <= 100
        pct[name] += weight
        levels[name].update(range(lo,hi+1))
    assert set(pct) == set(expected)
    for species, (wanted_pct,wanted_levels) in expected.items():
        assert pct[species] == wanted_pct, (e["base_label"],species,pct[species])
        assert levels[species] == wanted_levels, (e["base_label"],species,levels[species])
assert entries[0]["land_mons"] == entries[1]["land_mons"]
print("PASS: review candidate Route 2 FireRed/LeafGreen 40% Hoothoot Lv3-5, 35% Teddiursa Lv3-5, 25% Pichu Lv2-4, encounter rate 21")
