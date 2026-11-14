#!/usr/bin/env python3
"""Source-level review gates for owner-approved four post-League legendary statics.

This is NOT gameplay reachability proof: native D-pad/object interaction and outcome
acceptance are separately required before PR #235 can merge.
"""
import json
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SPECS = {
    "Raikou": ("Route10", "RAIKOU", 7, 39, 3),
    "Entei": ("MtEmber_SummitPath_1F", "ENTEI", 4, 8, 0),
    "Suicune": ("SeafoamIslands_B4F", "SUICUNE", 30, 9, 4),
    "Mew": ("SevenIsland_TanobyRuins", "MEW", 105, 12, 3),
}
LAYOUTS = {
    item["id"]: item
    for item in json.loads((ROOT / "data/layouts/layouts.json").read_text())["layouts"]
    if "id" in item
}
FLAGS = (ROOT / "include/constants/flags.h").read_text()
all_flags = set()


def check_species(name, map_name, species, x, y, elevation):
    map_path = ROOT / "data/maps" / map_name
    map_json = json.loads((map_path / "map.json").read_text())
    scripts = (map_path / "scripts.inc").read_text()
    layout = LAYOUTS[map_json["layout"]]
    width, height = layout["width"], layout["height"]
    assert 0 <= x < width and 0 <= y < height, (name, "out-of-bounds", x, y)
    blockdata = (ROOT / layout["blockdata_filepath"]).read_bytes()
    assert len(blockdata) == width * height * 2, (name, "blockdata size", len(blockdata))
    cells = struct.unpack("<" + "H" * (width * height), blockdata)
    def collision_at(px, py):
        return (cells[py * width + px] >> 10) & 3
    # Original FireRed map-block collision bit: both target and an adjacent
    # interaction tile must permit standing/movement. Runtime proof is separate.
    candidates = []
    occupied = {(o["x"], o["y"]) for o in map_json["object_events"]
                if o["script"] != f"{map_name}_EventScript_Lightning{name}"}
    occupied.update((w["x"], w["y"]) for w in map_json["warp_events"])
    for py in range(max(0, y-7), min(height, y+8)):
        for px in range(max(0, x-7), min(width, x+8)):
            if (px, py) in occupied or collision_at(px, py):
                continue
            options = [(nx, ny) for nx, ny in ((px-1, py), (px+1, py), (px, py-1), (px, py+1))
                       if 0 <= nx < width and 0 <= ny < height
                       and not collision_at(nx, ny) and (nx, ny) not in occupied]
            if options:
                candidates.append((abs(px-x)+abs(py-y), px, py, (cells[py*width+px]>>12)&15, options[:2]))
    candidates.sort()
    print(f"CANDIDATE {name} current=({x},{y}) collision={collision_at(x,y)} alternatives={candidates[:18]}", flush=True)
    assert collision_at(x, y) == 0, (name, "object tile collision", x, y, collision_at(x, y))
    neighbours = [(nx, ny) for nx, ny in
                  ((x-1, y), (x+1, y), (x, y-1), (x, y+1))
                  if 0 <= nx < width and 0 <= ny < height and collision_at(nx, ny) == 0]
    assert neighbours, (name, "no adjacent approach tile", x, y)

    objects = [o for o in map_json["object_events"]
               if o["script"] == f"{map_name}_EventScript_Lightning{name}"]
    assert len(objects) == 1, (name, "missing/duplicate object")
    obj = objects[0]
    assert (obj["x"], obj["y"], obj["elevation"]) == (x, y, elevation)
    assert obj["graphics_id"] == "OBJ_EVENT_GFX_" + species
    assert obj["flag"] == "FLAG_HIDE_LIGHTNING_" + species

    fought = "FLAG_FOUGHT_LIGHTNING_" + species
    hide = "FLAG_HIDE_LIGHTNING_" + species
    for flag in (fought, hide):
        lines = [line for line in FLAGS.splitlines() if line.startswith("#define " + flag + " ")]
        assert len(lines) == 1, (name, "flag missing", flag)
        number = int(lines[0].split()[-1], 0)
        assert number not in all_flags, (name, "flag reused", flag)
        all_flags.add(number)
    assert "goto_if_unset FLAG_SYS_GAME_CLEAR" in scripts
    assert "setwildbattle SPECIES_" + species + ", 50" in scripts
    assert "special StartLegendaryBattle" in scripts
    assert f"setflag {fought}" in scripts
    assert "B_OUTCOME_CAUGHT" in scripts and "B_OUTCOME_WON" in scripts
    assert "B_OUTCOME_RAN" in scripts and "B_OUTCOME_PLAYER_TELEPORTED" in scripts
    assert "call " + map_name + "_EventScript_ShowLightning" + name in scripts

    # Preserve the individual historically authored legendary source.
    if map_name == "SeafoamIslands_B4F":
        assert "SeafoamIslands_B4F_EventScript_Articuno" in scripts
        assert "SeafoamIslands_B4F_EventScript_CheckStoppedCurrent" in scripts
        assert scripts.count("map_script MAP_SCRIPT_ON_TRANSITION") == 1
    if map_name == "SevenIsland_TanobyRuins":
        assert len(map_json["warp_events"]) == 7, "preserve all seven Unown-chamber warps"
    print(f"PASS source: {name} Lv50 on {map_name} ({x},{y}); standing-neighbours={neighbours}")


def main():
    failures = []
    for name, spec in SPECS.items():
        try:
            check_species(name, *spec)
        except (AssertionError, KeyError) as exc:
            print(f"FAIL {name}: {exc}", flush=True)
            failures.append((name, str(exc)))
    assert not failures, failures
    print("PASS four separate stationary legendary source/flag/geometry contracts")
    print("NOTE native gameplay traversal, captures, persistence, and old roamer are NOT certified.")


if __name__ == "__main__":
    main()
