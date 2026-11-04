#!/usr/bin/env python3
"""Static safeguards for the owner-approved Lightning Yellow Blaine battle.

This does not replace compiling the ROM or exercising the event in mGBA.
"""
from pathlib import Path
import re

root = Path(__file__).resolve().parents[1]
parties = (root / "src/data/trainer_parties.h").read_text()
trainers = (root / "src/data/trainers.h").read_text()
script = (root / "data/maps/CinnabarIsland_Gym/scripts.inc").read_text()
text = (root / "data/maps/CinnabarIsland_Gym/text.inc").read_text()
species = (root / "src/data/pokemon/species_info.h").read_text()

def block(src, begin, end):
    assert src.count(begin) == 1, f"Expected one {begin}"
    return src.split(begin, 1)[1].split(end, 1)[0]

party = block(parties, "sParty_LeaderBlaine[] = {", "\n};")
slots = re.findall(r"\.species = SPECIES_(\w+)", party)
levels = [int(x) for x in re.findall(r"\.lvl = (\d+)", party)]
moves = re.findall(r"\.moves = \{([^}]+)\}", party)
expected = [
    ("DONPHAN", 42, "EARTHQUAKE ROCK_TOMB ROLLOUT DEFENSE_CURL"),
    ("PIDGEOT", 42, "AERIAL_ACE QUICK_ATTACK FEATHER_DANCE SAND_ATTACK"),
    ("SANDSLASH", 43, "EARTHQUAKE SLASH DIG SAND_ATTACK"),
    ("AERODACTYL", 44, "AERIAL_ACE ANCIENT_POWER BITE SCARY_FACE"),
    ("FLYGON", 47, "EARTHQUAKE DRAGON_CLAW CRUNCH SCREECH"),
]
assert len(slots) == len(levels) == len(moves) == 5
for (sp, lvl, move_names), actual_sp, actual_lvl, move_list in zip(expected, slots, levels, moves):
    assert (actual_sp, actual_lvl) == (sp, lvl)
    assert [x.strip().removeprefix("MOVE_") for x in move_list.split(",")] == move_names.split()

meta = block(trainers, "[TRAINER_LEADER_BLAINE] = {", "\n    },")
assert ".doubleBattle = TRUE" in meta
assert ".party = NO_ITEM_CUSTOM_MOVES(sParty_LeaderBlaine)" in meta
assert ".items = {ITEM_HYPER_POTION, ITEM_HYPER_POTION, ITEM_FULL_HEAL}" in meta
event = block(script, "CinnabarIsland_Gym_EventScript_Blaine::", "\nCinnabarIsland_Gym_EventScript_DefeatedBlaine::")
assert "trainerbattle_single TRAINER_LEADER_BLAINE" not in event
assert "trainerbattle_double TRAINER_LEADER_BLAINE" in event
assert "CinnabarIsland_Gym_Text_BlaineNeedTwoPokemon, CinnabarIsland_Gym_EventScript_DefeatedBlaine, NO_MUSIC" in event
assert "CinnabarIsland_Gym_Text_BlaineNeedTwoPokemon::" in text
assert "setflag FLAG_DEFEATED_BLAINE" in script
assert "setflag FLAG_BADGE07_GET" in script
assert "ITEM_TM38" in script
pidgeot = block(species, "[SPECIES_PIDGEOT] =", "\n    },")
assert ".types = {TYPE_FLYING, TYPE_FLYING}" in pidgeot
print("Blaine static checks passed. Compile and mGBA tests still required.")
