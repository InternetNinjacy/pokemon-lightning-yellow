#!/usr/bin/env python3
"""Check the approved catch-rate implementation and complete early direct-capture roster.

Static source checks only. No emulator, weather test or game build is launched.
"""
import hashlib
import json
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[1]
M = json.loads((ROOT / "tests/specifications/early_catch_rates_through_misty.json").read_text())
BASE = "02240690581eb70d70219024fb7f55da2882308b"
PATH = "src/data/pokemon/species_info.h"


def prior(path):
    return subprocess.check_output(["git", "show", f"{BASE}:{path}"], cwd=ROOT)


assert M["base_sha"] == BASE
original = prior(PATH).decode()
assert hashlib.sha256(original.encode()).hexdigest() == M["species_info_before_sha256"]
expected = original
changes = {x["species"]: x for x in M["changes"]}
assert len(changes) == len(M["changes"]) == 13
for species, change in changes.items():
    assert 1 <= change["before"] <= 255
    assert change["after"] == M["approved_changed_species_rate"] == 125
    if species == "SPECIES_IGGLYBUFF":
        assert change["before"] == 170  # Explicit owner-approved uniform125 exception.
    else:
        assert change["before"] < change["after"]
    pattern = r'(\[' + re.escape(species) + r'\] =\s*\{.*?\.catchRate = )' + str(change["before"]) + r'(,)'
    expected, count = re.subn(pattern, lambda m: m[1] + str(change["after"]) + m[2], expected, count=1, flags=re.S)
    assert count == 1, species
assert (ROOT / PATH).read_text() == expected, "Unexpected species field or catch-rate change"

# Recompute source eligibility rather than trusting the 51-row review inventory.
group = json.loads((ROOT / "src/data/wild_encounters.json").read_text())["wild_encounter_groups"][0]
weights = {x["type"]: x["encounter_rates"] for x in group["fields"]}
roster = {}
for row in group["encounters"]:
    if row["map"] not in M["map_stages"]:
        continue
    for field in ("land_mons", "fishing_mons"):
        if field not in row:
            continue
        mons, chances = row[field]["mons"], weights[field]
        if field == "fishing_mons":
            # Only Old Rod. Good Rod requires badge 3; Surf/Super Rod are later.
            assert group["fields"][3]["groups"]["old_rod"] == [0, 1]
            mons, chances = mons[:2], chances[:2]
        for species in sorted({m["species"] for m in mons}):
            slots = [(m, w) for m, w in zip(mons, chances) if m["species"] == species]
            roster.setdefault(species, []).append({
                "map": row["map"], "stage": M["map_stages"][row["map"]],
                "method": "old_rod" if field == "fishing_mons" else "land",
                "version": row["base_label"].rsplit("_", 1)[1],
                "weight_percent": sum(w for m, w in slots),
                "min_level": min(m["min_level"] for m, w in slots),
                "max_level": max(m["max_level"] for m, w in slots),
            })
double = (ROOT / "src/wild_double_battle.c").read_text()
assert "{{SPECIES_ONIX, SPECIES_CLOYSTER}, 5}" in double
assert "{MAP_GROUP(MAP_MT_MOON_1F), MAP_NUM(MAP_MT_MOON_1F), ARRAY_COUNT(sTestPairs), sTestPairs}" in double
assert "#define LIGHTNING_WILD_DOUBLES_TEST_MAPS 1" in (ROOT / "include/wild_double_battle.h").read_text()
for species in ("SPECIES_ONIX", "SPECIES_CLOYSTER"):
    roster.setdefault(species, []).append({
        "map": "MAP_MT_MOON_1F", "stage": "through_misty",
        "method": "existing_special_double_pair", "pair_percent": 5,
        "condition": "two usable party Pokemon; existing survivor capture policy", "version": "both",
    })
review = {x["species"]: x for x in M["direct_catchable_species"]}
assert set(review) == set(roster) and len(review) == 51
assert set(changes) <= set(review)
current_rates = {s: int(n) for s, n in re.findall(
    r'\[(SPECIES_\w+)\] =\s*\{.*?\.catchRate = (\d+),', expected, re.S)}
old_rates = {s: int(n) for s, n in re.findall(
    r'\[(SPECIES_\w+)\] =\s*\{.*?\.catchRate = (\d+),', original, re.S)}
before_brock = 0
for species, sources in roster.items():
    row = review[species]
    assert row["sources"] == sources, species
    early = any(x["stage"] == "before_brock" for x in sources)
    before_brock += early
    assert row["earliest_stage"] == ("before_brock" if early else "through_misty")
    assert row["before"] == old_rates[species]
    assert row["proposed"] == current_rates[species]
    assert current_rates[species] >= (125 if early else 120), species
    if species in changes:
        assert current_rates[species] == 125, species
    else:
        assert current_rates[species] == old_rates[species], species
assert before_brock == 17

# Preserve all original sources and checks, including every encounter and gate.
allowed = {PATH, "docs/EARLY_CATCH_RATE_REBALANCE.md", "docs/CODEX_EXECUTION_HANDOFF.md",
           "docs/MASTER_IMPLEMENTATION_CHANGE_LEDGER.md", "tests/check_early_catch_rates.py",
           "tests/specifications/early_catch_rates_through_misty.json"}
changed = subprocess.check_output(["git", "diff", "--name-only", BASE, "--"], cwd=ROOT, text=True).splitlines()
assert set(changed) <= allowed, f"Out-of-scope changed paths: {set(changed) - allowed}"
for path in ("src/data/wild_encounters.json", "src/wild_double_battle.c", "include/wild_double_battle.h",
             "src/capture_rules.c", "src/battle_script_commands.c", "src/data/ingame_trades.h",
             "src/mystery_vendor.c", "docs/project-tracking.json", "docs/PROJECT_DASHBOARD.md"):
    assert (ROOT / path).read_bytes() == prior(path), path
assert "additem ITEM_OLD_ROD" in (ROOT / "data/maps/ViridianCity_House/scripts.inc").read_text()
assert "goto_if_unset FLAG_BADGE03_GET" in (ROOT / "data/maps/Route11_EastEntrance_1F/scripts.inc").read_text()
tracking = json.loads((ROOT / "docs/project-tracking.json").read_text())
assert len(tracking["tasks"]) == 50
assert sum(t["effort"]["earned"] for t in tracking["tasks"]) == 461
assert sum(t["effort"]["remaining"] for t in tracking["tasks"]) == 251
print("PASS: 51 direct-capture species audited; 17 before Brock >=125; other reviewed rates preserved")
print("PASS: exactly 13 owner-approved catchRate values set to125; every other species-file byte unchanged")
print("PASS: encounters, capture engine, existing checks/workflows and 50 IDs/461/251/712 preserved")
print("Static implementation evidence only; no native, build or required-CI success claimed")
