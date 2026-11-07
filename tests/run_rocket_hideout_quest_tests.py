#!/usr/bin/env python3
"""Rocket Hideout rescue-quest source contract, not a gameplay/emulator test."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
read = lambda p: (ROOT / p).read_text()

flags = read("include/constants/flags.h")
vars_ = read("include/constants/vars.h")
specials = read("data/specials.inc")
native = read("src/rocket_rescue.c")
b3f = read("data/maps/RocketHideout_B3F/scripts.inc")
b3f_map = json.loads(read("data/maps/RocketHideout_B3F/map.json"))
route = read("data/scripts/trainers.inc")
route_text = read("data/maps/Route8/text.inc")

# The quests reserve old unused save IDs but leave frozen global
# flags.h/vars.h unchanged for compatibility with older source audits.
for i in range(0x316, 0x31C):
    assert "FLAG_0x" + format(i, "03X") in flags, hex(i)
for i in range(0x40F8, 0x40FC):
    assert "VAR_0x" + format(i, "04X") in vars_, hex(i)
for name in (
    "ROCKET_RALTS_COLLECTED", "ROCKET_SNORUNT_COLLECTED",
    "ROCKET_RALTS_RETURNED", "ROCKET_SNORUNT_RETURNED",
):
    assert "#define FLAG_" + name in native, name
for special in ("RocketRescueStatus", "RocketRescueGive", "RocketRescueReturn"):
    assert f"def_special {special}" in specials
    assert f"u16 {special}(void)" in native

assert len(b3f_map["object_events"]) == 7
balls = [o for o in b3f_map["object_events"] if o.get("local_id", "").startswith("LOCALID_ROCKET_")]
assert len(balls) == 2
assert {o["script"] for o in balls} == {
    "RocketHideout_B3F_EventScript_RescueRalts",
    "RocketHideout_B3F_EventScript_RescueSnorunt",
}
for o in balls:
    assert f"{o['local_id']}::" not in b3f
    assert o["local_id"] in b3f
# Validate the actual binary layout, not merely the map event coordinates.
# GBA block entries are little-endian: metatile 0..9, collision 10..11,
# elevation 12..15. An item ball cannot be recovered from an impassable tile.
layout = (ROOT / "data/layouts/RocketHideout_B3F/map.bin").read_bytes()
width, height = 22, 27
assert len(layout) == width * height * 2
def block(x, y):
    p = 2 * (y * width + x)
    return int.from_bytes(layout[p:p+2], "little")
for o in balls:
    x, y = o["x"], o["y"]
    assert 0 <= x < width and 0 <= y < height
    value = block(x, y)
    assert (value >> 10) & 3 == 0, (o["local_id"], "collision", x, y)
    assert value >> 12 == o["elevation"], (o["local_id"], "elevation", x, y)
    assert any(0 <= a < width and 0 <= b < height
               and ((block(a, b) >> 10) & 3) == 0
               and (block(a, b) >> 12) == o["elevation"]
               for a, b in ((x-1, y), (x+1, y), (x, y-1), (x, y+1))), (o["local_id"], "no adjacent floor")
for n in (14, 15):
    assert f"TRAINER_TEAM_ROCKET_GRUNT_{n}" in b3f
assert "MAP_SCRIPT_ON_LOAD" in b3f
for old in ("RocketHideout_B3F_EventScript_ItemRareCandy",
            "RocketHideout_B3F_EventScript_ItemTM21",
            "RocketHideout_B3F_EventScript_ItemBlackGlasses"):
    assert any(o["script"] == old for o in b3f_map["object_events"]), old
for owner in ("Paige", "Julia"):
    for word in ("Quest", "Offer", "Absent", "BagFull", "OnlyMon",
                 "AfterReturn", "Reunited", "Decline"):
        assert f"Route8_EventScript_{owner}{word}::" in route
    assert f"Route8_Text_{owner}AfterReturn::" in route_text

for guard in ("CheckBagHasSpace", "AddBagItem", "ZeroBoxMonData",
              "GetBoxedMonPtr", "GetIssuedPersonality", "FindIssuedMon",
              "MON_DATA_OT_ID", "MON_DATA_PERSONALITY", "MON_DATA_OT_NAME",
              "MON_DATA_SPECIES", "FlagSet(sReturnedFlags[quest])"):
    assert guard in native, guard
assert native.index("CheckBagHasSpace") < native.index("AddBagItem")
assert native.index("AddBagItem(sRewards[quest], 1)") < native.index("FlagSet(sReturnedFlags[quest])")
print("PASS: rescue map, save flags/identity variables, native hooks and Route 8 branches")
