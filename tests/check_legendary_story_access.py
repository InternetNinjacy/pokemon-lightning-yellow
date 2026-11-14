#!/usr/bin/env python3
"""Fail-closed source checks for ordinary postgame travel to Suicune and Mew.

This checks the real map topology, HM/Soul Badge rules and Rainbow Pass story
gate. It does NOT claim a continuous controller-driven playthrough, actual Surf
animation, or end-to-end unlocked-save acceptance.
"""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def source(path):
    return (ROOT / path).read_text()


def mapped(name):
    return json.loads(source(f"data/maps/{name}/map.json"))


def connected(source_map, destination_map):
    expected = "MAP_" + destination_map.upper()
    # Actual event map macros use separated words, not uppercase alone.
    expected = "MAP_" + "".join(("_" if c.isupper() and i else "") + c.upper()
                                   for i, c in enumerate(destination_map)).replace("__", "_")
    return expected in [c["map"] for c in mapped(source_map).get("connections", []) or []]


def map_macro(name):
    # Use the existing map name in the event data rather than reimplementing
    # the repository's special MAP_ identifier conversion.
    targets = {
        "SevenIsland": "MAP_SEVEN_ISLAND",
        "SevenIsland_SevaultCanyon_Entrance": "MAP_SEVEN_ISLAND_SEVAULT_CANYON_ENTRANCE",
        "SevenIsland_SevaultCanyon": "MAP_SEVEN_ISLAND_SEVAULT_CANYON",
        "SevenIsland_TanobyRuins": "MAP_SEVEN_ISLAND_TANOBY_RUINS",
        "Route20": "MAP_ROUTE20",
        "SeafoamIslands_1F": "MAP_SEAFOAM_ISLANDS_1F",
        "SeafoamIslands_B1F": "MAP_SEAFOAM_ISLANDS_B1F",
        "SeafoamIslands_B2F": "MAP_SEAFOAM_ISLANDS_B2F",
        "SeafoamIslands_B3F": "MAP_SEAFOAM_ISLANDS_B3F",
        "SeafoamIslands_B4F": "MAP_SEAFOAM_ISLANDS_B4F",
    }
    return targets[name]


def require_connection(a, b):
    assert any(c["map"] == map_macro(b) for c in mapped(a).get("connections", []) or []), (a, b)
    assert any(c["map"] == map_macro(a) for c in mapped(b).get("connections", []) or []), (b, a)


def require_warp(a, b):
    warps = mapped(a)["warp_events"]
    assert any(w["dest_map"] == map_macro(b) for w in warps), (a, b, "missing entrance")
    assert any(w["dest_map"] == map_macro(a) for w in mapped(b)["warp_events"]), (b, a, "missing return")


def main():
    safari = source("data/maps/SafariZone_SecretHouse/scripts.inc")
    koga = source("data/maps/FuchsiaCity_Gym/scripts.inc")
    field = source("src/party_menu.c")
    assert "giveitem_msg SafariZone_SecretHouse_Text_ReceivedHM03FromAttendant, ITEM_HM03" in safari
    assert "setflag FLAG_GOT_HM03" in safari
    assert "FuchsiaCity_Gym_EventScript_DefeatedKoga::" in koga
    assert "setflag FLAG_BADGE05_GET" in koga
    assert "FlagGet(FLAG_BADGE01_GET + fieldMove)" in field
    assert "SetUpFieldMove_Surf" in field and "PartyHasMonWithSurf()" in field
    assert "IsPlayerFacingSurfableFishableWater()" in field
    print("PASS Surf prerequisites: earned HM03, Koga's badge, real field-use rules")

    # Map connection and cave stair graph from actual Route 20 / island entry.
    require_warp("Route20", "SeafoamIslands_1F")
    for a, b in [
        ("SeafoamIslands_1F", "SeafoamIslands_B1F"),
        ("SeafoamIslands_B1F", "SeafoamIslands_B2F"),
        ("SeafoamIslands_B2F", "SeafoamIslands_B3F"),
        ("SeafoamIslands_B3F", "SeafoamIslands_B4F"),
    ]:
        require_warp(a, b)
    seafoam = mapped("SeafoamIslands_B4F")
    assert len([o for o in seafoam["object_events"] if o["script"] ==
                "SeafoamIslands_B4F_EventScript_LightningSuicune"]) == 1
    print("PASS Suicune authored route: Route 20, all Seafoam floors and object")

    celio = source("data/maps/OneIsland_PokemonCenter_1F/scripts.inc")
    ferry = source("data/scripts/seagallop.inc")
    assert "specialvar VAR_RESULT, IsNationalPokedexEnabled" in celio
    assert "OneIsland_PokemonCenter_1F_EventScript_CelioRequestRuby" in celio
    assert "goto_if_set FLAG_GOT_RUBY, OneIsland_PokemonCenter_1F_EventScript_GiveCelioRuby" in celio
    assert "OneIsland_PokemonCenter_1F_EventScript_GiveRainbowPass::" in celio
    assert "setvar VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 5" in celio
    assert "additem ITEM_RAINBOW_PASS" in celio
    assert "setflag FLAG_SYS_SEVII_MAP_4567" in celio
    assert "goto_if_ge VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 5, EventScript_SeviiDestinationsPage1" in ferry
    assert "case SEAGALLOP_SEVEN_ISLAND, EventScript_SailToSevenIsland" in ferry
    assert "EventScript_SailToSevenIsland::" in ferry
    for a, b in [
        ("SevenIsland", "SevenIsland_SevaultCanyon_Entrance"),
        ("SevenIsland_SevaultCanyon_Entrance", "SevenIsland_SevaultCanyon"),
        ("SevenIsland_SevaultCanyon", "SevenIsland_TanobyRuins"),
    ]:
        require_connection(a, b)
    tanoby = mapped("SevenIsland_TanobyRuins")
    assert len(tanoby["warp_events"]) == 7, "preserve seven Unown chambers"
    assert len([o for o in tanoby["object_events"] if o["script"] ==
                "SevenIsland_TanobyRuins_EventScript_LightningMew"]) == 1
    print("PASS Mew authored route: Celio/Rainbow Pass ferry, canyon connections, original chambers")

    for name, script in [
        ("SeafoamIslands_B4F", "SeafoamIslands_B4F_EventScript_LightningSuicune"),
        ("SevenIsland_TanobyRuins", "SevenIsland_TanobyRuins_EventScript_LightningMew"),
    ]:
        text = source(f"data/maps/{name}/scripts.inc")
        assert "goto_if_unset FLAG_SYS_GAME_CLEAR" in text
        assert "B_OUTCOME_CAUGHT" in text and "B_OUTCOME_WON" in text
        assert script + "::" in text
    print("PASS both encounters require the League win, retain native catch/defeat outcomes")
    print("LIMIT: NOT a fresh-save full D-pad/SURF travel or unlocked-story run")


if __name__ == "__main__":
    main()
