#!/usr/bin/env python3
"""Static regression checks for the Pewter Museum trivia event.

Does not replace emulator playtesting.
"""
import json
import re
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
map_path = ROOT / "data/maps/PewterCity_Museum_1F/map.json"
layout_path = ROOT / "data/layouts/PewterCity_Museum_1F/map.bin"
script_path = ROOT / "data/maps/PewterCity_Museum_1F/scripts.inc"
text_path = ROOT / "data/maps/PewterCity_Museum_1F/text.inc"
flags_path = ROOT / "include/constants/flags.h"

event = json.loads(map_path.read_text())
npcs = [o for o in event["object_events"] if o.get("script", "").endswith("EventScript_TriviaEmployee")]
assert len(npcs) == 1, "Expected exactly one trivia employee"
npc = npcs[0]
assert npc["graphics_id"] == "OBJ_EVENT_GFX_WORKER_M"
assert (npc["x"], npc["y"]) != (16, 5), "Must not overwrite ticket attendant"
assert 16 <= npc["x"] < 28, "NPC must be on paid exhibit side"
assert 0 <= npc["y"] < 11
width, height = 28, 11
block_data = layout_path.read_bytes()
assert len(block_data) == width * height * 2
tiles = struct.unpack(f"<{width * height}H", block_data)
def tile(x, y):
    raw = tiles[y * width + x]
    return dict(id=raw & 0x3ff, collision=(raw & 0xc00) >> 10, elevation=raw >> 12)
print(f"Trivia NPC ({npc['x']},{npc['y']}) tile: {tile(npc['x'],npc['y'])}")
assert tile(npc["x"], npc["y"])["collision"] == 0, "Trivia NPC placed on blocked map tile"
assert tile(npc["x"], npc["y"])["elevation"] in (0, 3), "NPC elevation not compatible with museum floor"
surroundings = []
for x, y in [(npc["x"]-1,npc["y"]), (npc["x"]+1,npc["y"]), (npc["x"],npc["y"]-1),(npc["x"],npc["y"]+1)]:
    if 0 <= x < width and 0 <= y < height:
        surroundings.append((x,y,tile(x,y)))
print("Adjacent:",surroundings)
assert any(t["collision"] == 0 for x,y,t in surroundings), "No adjacent standing tile"
s = script_path.read_text()
txt = text_path.read_text()
assert all(q in txt for q in ("RATTATA", "CATERPIE", "BROCK", "MISTY", "DAISY", "VIOLET", "LILY"))
wrong = re.findall(r"goto_if_eq VAR_RESULT, (YES|NO), PewterCity_Museum_1F_TriviaWrong",s)
assert wrong == ["YES", "NO", "YES"], wrong
labels = set(re.findall(r"PewterCity_Museum_1F_Text_Trivia\\w+", s))
missing = [l for l in labels if l + "::" not in txt]
assert not missing, missing
flags = flags_path.read_text()
assert "FLAG_PEWTER_MUSEUM_TRIVIA_WON" in flags and "FLAG_PEWTER_MUSEUM_SHINY_EGG_RECEIVED" in flags
assert s.index("setflag FLAG_PEWTER_MUSEUM_TRIVIA_WON") < s.index("specialvar VAR_RESULT, ScriptGiveShinyFeebasEgg")
assert s.index("specialvar VAR_RESULT, ScriptGiveShinyFeebasEgg") < s.index("setflag FLAG_PEWTER_MUSEUM_SHINY_EGG_RECEIVED")
print("Trivia script, persistent flags, and static placement assertions pass")
