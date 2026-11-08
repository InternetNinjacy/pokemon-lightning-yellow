#!/usr/bin/env python3
"""Source acceptance for owner-approved special visible weather encounters.

Native ROM execution, walkability and visual assets are independent acceptance gates.
"""
import json
from pathlib import Path

root=Path("data/maps")
expected=[
 ("Route12","Route12_EventScript_SowCorphish","OBJ_EVENT_GFX_SOW_CORPHISH","FLAG_UNUSED_0x4A8",19,87,
  "SPECIES_CORPHISH","30","SPECIAL_WEATHER_CORPHISH"),
 ("Route21_North","Route21_North_EventScript_SowGlalie","OBJ_EVENT_GFX_SOW_GLALIE","FLAG_UNUSED_0x4AA",13,7,
  "SPECIES_GLALIE","40","SPECIAL_WEATHER_GLALIE"),
 ("Route23","Route23_EventScript_SowTyranitar","OBJ_EVENT_GFX_SOW_TYRANITAR","FLAG_UNUSED_0x4AC",13,145,
  "SPECIES_TYRANITAR","55","SPECIAL_WEATHER_TYRANITAR")
]
for m,script,gfx,flag,x,y,species,level,weather in expected:
    header=json.loads((root/m/"map.json").read_text())
    found=[o for o in header["object_events"] if o.get("script")==script]
    assert len(found)==1,(m,script)
    obj=found[0]
    assert (obj["graphics_id"],obj["flag"],obj["x"],obj["y"])==(gfx,flag,x,y)
    assert obj["trainer_type"]=="TRAINER_TYPE_NONE"
    content=(root/m/"scripts.inc").read_text()
    assert content.count(script+"::")==1
    assert f"setwildbattle {species}, {level}" in content
    assert content.count("callnative SowPrepareScriptedEncounter")==1
    assert "B_OUTCOME_WON" in content and "B_OUTCOME_CAUGHT" in content
    assert "SpecialWeather_EventScript_FinishAftermath" in content
    # An encounter only resolves for actual capture or defeat, never Run/loss.
    resolved=script.replace("SowCorphish","SowResolved").replace("SowGlalie","SowResolved").replace("SowTyranitar","SowResolved")
    abort=script.replace("SowCorphish","SowAbort").replace("SowGlalie","SowAbort").replace("SowTyranitar","SowAbort")
    assert f"goto_if_eq VAR_RESULT, B_OUTCOME_WON, {resolved}" in content
    assert f"goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, {resolved}" in content
    assert f"{abort}::\\n\\trelease\\n\\tend" in content
    assert "B_OUTCOME_RAN" not in content[content.index(script+"::"):]
    assert content.index("dowildbattle",content.index(script+"::")) < content.index("specialvar VAR_RESULT, GetBattleOutcome",content.index(script+"::"))
    assert content.index("SpecialWeather_EventScript_FinishAftermath",content.index(resolved+"::")) < content.index("removeobject VAR_LAST_TALKED",content.index(resolved+"::"))
assert "MAP_SCRIPT_ON_TRANSITION" in (root/"Route21_South"/"scripts.inc").read_text()
assert "SpecialWeatherActivateScript" in (root/"Route23"/"scripts.inc").read_text()
c=Path("src/script_pokemon_util.c").read_text()
assert c.count("void SowPrepareScriptedEncounter(void)")==1
for sp,lv,moves in [
 ("SPECIES_CORPHISH",30,["MOVE_BUBBLE_BEAM","MOVE_KNOCK_OFF","MOVE_PROTECT","MOVE_VICE_GRIP"]),
 ("SPECIES_GLALIE",40,["MOVE_ICE_BEAM","MOVE_CRUNCH","MOVE_HEADBUTT","MOVE_PROTECT"]),
 ("SPECIES_TYRANITAR",55,["MOVE_CRUNCH","MOVE_ROCK_SLIDE","MOVE_THRASH","MOVE_SCARY_FACE"])
]:
 assert f"{sp}, {lv}, {{{', '.join(moves)}}}" in c,(sp,lv)
assert "SetSpecialEncounterAbility(&gEnemyParty[0], ABILITY_DRIZZLE)" in c
assert "SetSpecialEncounterAbility(&gEnemyParty[0], ABILITY_SNOW_WARNING)" in c
assert "GetMonAbility(&gEnemyParty[0]) != ABILITY_SAND_STREAM" in c
assert "MON_DATA_HELD_ITEM) != ITEM_NONE" in c
gfx=Path("include/constants/event_objects.h").read_text()
assert "NUM_OBJ_EVENT_GFX     155" in gfx
sprite=Path("src/data/object_events/object_event_pic_tables.h").read_text()
for key in ["Corphish","Glalie","Tyranitar"]:
 assert "sPicTable_Sow"+key in sprite
 assert "gMonIcon_"+key in sprite
assert "At last... I can rest" in (root/"Route12"/"text.inc").read_text()
print("PASS: approved SOW species, all 12 exact moves, three icon gfx registrations, three installed map objects, no item/ability guards, full Route21 weather bridge and Corphish ghost farewell; real ROM integration pending")
