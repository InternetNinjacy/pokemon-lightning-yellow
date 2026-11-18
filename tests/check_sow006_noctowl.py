#!/usr/bin/env python3
"""Source-only SOW-006 Noctowl owner contract; runtime/access remain separate."""
import json
from pathlib import Path

root = Path(__file__).resolve().parents[1]
map_data = json.loads((root / "data/maps/Route16/map.json").read_text())
objects = map_data["object_events"]
owls = [o for o in objects if o["script"] == "Route16_EventScript_SowNoctowl"]
assert len(owls) == 1, "Expected exactly one SOW-006 Noctowl object"
owl = owls[0]
assert (owl["x"], owl["y"], owl["elevation"]) == (12, 7, 3)
assert owl["graphics_id"] == "OBJ_EVENT_GFX_SOW_NOCTOWL"
assert owl["flag"] == "FLAG_HIDE_SOW_NOCTOWL"
assert any(w["x"] == 10 and w["y"] == 5 and w["dest_map"] == "MAP_ROUTE16_HOUSE"
           for w in map_data["warp_events"])
assert any(o["script"] == "EventScript_CutTree" and (o["x"], o["y"]) == (41, 11)
           for o in objects)
assert any(o["script"] == "Route16_EventScript_Snorlax" for o in objects)

scripts = (root / "data/maps/Route16/scripts.inc").read_text()
event = scripts.split("Route16_EventScript_SowNoctowl::", 1)[1]
assert "setwildbattle SPECIES_NOCTOWL, 36" in event
assert "callnative SowPrepareNoctowlEncounter" in event
assert "dowildbattle" in event
assert "B_OUTCOME_WON, Route16_SowNoctowlResolved" in event
assert "B_OUTCOME_CAUGHT, Route16_SowNoctowlResolved" in event
assert "B_OUTCOME_RAN, Route16_SowNoctowlResolved" not in event
assert "setflag FLAG_FOUGHT_SOW_NOCTOWL" in event
assert "removeobject VAR_LAST_TALKED" in event
assert "Route16_EventScript_Snorlax::" in scripts

util = (root / "src/script_pokemon_util.c").read_text()
helper = util.split("void SowPrepareNoctowlEncounter(void)", 1)[1].split(
    "void ScriptSetMonMoveSlot", 1)[0]
for term in ("SPECIES_NOCTOWL", "MON_DATA_LEVEL) != 36",
             "MON_DATA_HELD_ITEM) != ITEM_NONE",
             "MOVE_HYPNOSIS, MOVE_PSYBEAM, MOVE_TAKE_DOWN, MOVE_REFLECT",
             "SetMonMoveSlot(&gEnemyParty[0]", "gSpecialVar_Result = TRUE"):
    assert term in helper, term
assert "MON_DATA_ABILITY_NUM" not in helper
assert "CreateMon(" not in helper

flags = (root / "include/constants/flags.h").read_text()
assert "#define FLAG_HIDE_SOW_NOCTOWL 0x32D" in flags
assert "#define FLAG_FOUGHT_SOW_NOCTOWL 0x32E" in flags

gfx = (root / "include/constants/event_objects.h").read_text()
assert "#define OBJ_EVENT_GFX_SOW_NOCTOWL 156" in gfx
assert "#define NUM_OBJ_EVENT_GFX     157" in gfx
pics = (root / "src/data/object_events/object_event_pic_tables.h").read_text()
assert "sPicTable_SowNoctowl[]" in pics
assert pics.count("gMonIcon_Noctowl, 512") >= 9
info = (root / "src/data/object_events/object_event_graphics_info.h").read_text()
assert "gObjectEventGraphicsInfo_SowNoctowl" in info
assert "sPicTable_SowNoctowl" in info
pointers = (root / "src/data/object_events/object_event_graphics_info_pointers.h").read_text()
assert "[OBJ_EVENT_GFX_SOW_NOCTOWL]" in pointers

house = (root / "data/maps/Route16_House/scripts.inc").read_text()
assert "giveitem_msg Route16_House_Text_ReceivedHM02FromGirl, ITEM_HM02" in house
assert "Route16_House_EventScript_Fearow::" in house
species = (root / "src/data/pokemon/species_info.h").read_text()
noctowl = species.split("[SPECIES_NOCTOWL] =", 1)[1].split("    [SPECIES_LEDYBA]", 1)[0]
assert ".types = {TYPE_FLYING, TYPE_PSYCHIC}" in noctowl
assert ".abilities = {ABILITY_INSOMNIA, ABILITY_KEEN_EYE}" in noctowl
assert ".itemCommon = ITEM_NONE" in noctowl
assert ".itemRare = ITEM_NONE" in noctowl
print("PASS: SOW-006 source object, encounter, moves, native abilities, icons, flags and house preservation")
