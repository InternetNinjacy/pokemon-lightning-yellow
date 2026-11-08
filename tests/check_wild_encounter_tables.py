#!/usr/bin/env python3
"""Read-only structural validation for FireRed wild encounter source data.

Run: python3 tests/check_wild_encounter_tables.py
This guard deliberately does not certify ecological approvals, map access or the
252-species roster. Those require separate approved design and source audits.
"""
import json
import pathlib
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
PATH = ROOT / "src/data/wild_encounters.json"
EXPECTED = {"land_mons": 12, "water_mons": 5, "rock_smash_mons": 5, "fishing_mons": 10}
ROD_GROUPS = {"old_rod": [0, 1], "good_rod": [2, 3, 4], "super_rod": [5, 6, 7, 8, 9]}

def check(data):
    problems = []
    groups = data.get("wild_encounter_groups", [])
    if not isinstance(groups, list) or not groups:
        return ["missing wild_encounter_groups"]
    for group_index, group in enumerate(groups):
        prefix = "group " + str(group_index)
        fields = {field.get("type"): field for field in group.get("fields", [])}
        for method, count in EXPECTED.items():
            field = fields.get(method)
            if field is None:
                problems.append(prefix + ": missing method " + method)
                continue
            weights = field.get("encounter_rates", [])
            if len(weights) != count or any(type(w) is not int or w < 0 for w in weights):
                problems.append(prefix + ": invalid weights for " + method)
            elif method == "fishing_mons":
                if field.get("groups") != ROD_GROUPS:
                    problems.append(prefix + ": fishing rod groups differ from engine contract")
                for rod, indexes in ROD_GROUPS.items():
                    if sum(weights[i] for i in indexes) != 100:
                        problems.append(prefix + ": " + rod + " weights must sum to 100")
            elif sum(weights) != 100:
                problems.append(prefix + ": " + method + " weights must sum to 100")
        seen = set()
        for entry in group.get("encounters", []):
            key = (entry.get("map"), entry.get("base_label"))
            if key in seen:
                problems.append(prefix + ": duplicate map/label " + str(key))
            seen.add(key)
            if not isinstance(entry.get("map"), str) or not entry["map"].startswith("MAP_"):
                problems.append(prefix + ": invalid map key " + str(key))
            for method, count in EXPECTED.items():
                info = entry.get(method)
                if info is None:
                    continue
                mons = info.get("mons", [])
                rate = info.get("encounter_rate")
                if type(rate) is not int or not 0 <= rate <= 100:
                    problems.append(str(key) + ": invalid encounter chance for " + method)
                if len(mons) != count:
                    problems.append(str(key) + ": " + method + " requires " + str(count) + " slots")
                for slot, mon in enumerate(mons):
                    lo, hi, species = mon.get("min_level"), mon.get("max_level"), mon.get("species")
                    if (type(lo) is not int or type(hi) is not int
                            or not 1 <= lo <= hi <= 100):
                        problems.append(str(key) + ": invalid levels in " + method + " slot " + str(slot))
                    if not isinstance(species, str) or not species.startswith("SPECIES_"):
                        problems.append(str(key) + ": invalid species symbol in " + method + " slot " + str(slot))
    return problems

def main():
    data = json.loads(PATH.read_text(encoding="utf-8"))
    problems = check(data)
    for issue in problems:
        print("FAIL:", issue)
    if problems:
        return 1
    records = sum(len(g.get("encounters", [])) for g in data["wild_encounter_groups"])
    print("PASS: structurally valid encounter data, %d map/version records" % records)
    print("NOTE: does not validate 252-Dex legality, owner approval or map accessibility")
    return 0

if __name__ == "__main__":
    sys.exit(main())
