#!/usr/bin/env python3
"""GYM-002: owner-approved Misty four-member Double and trainer-only abilities."""
from pathlib import Path
import re
root = Path(__file__).resolve().parents[1]
party = (root/"src/data/trainer_parties.h").read_text()
trainers = (root/"src/data/trainers.h").read_text()
scripts = (root/"data/maps/CeruleanCity_Gym/scripts.inc").read_text()
battle = (root/"src/battle_util.c").read_text()
species_info = (root/"src/data/pokemon/species_info.h").read_text()
moves = (root/"include/constants/moves.h").read_text()
group=re.search(r"static const struct TrainerMonItemCustomMoves sParty_LeaderMisty\[\] = \{(.*?)\n\};",party,re.S)
assert group,"Misty party item/custom"
slots=re.findall(r"\{\s*\.iv = (\d+),\s*\.lvl = (\d+),\s*\.species = SPECIES_(\w+),\s*\.heldItem = (ITEM_\w+),\s*\.moves = \{([^}]+)\},\s*\}",group[1],re.S)
expected=[
    ("STARYU",22,"ITEM_NONE","WATER_GUN RAPID_SPIN SWIFT RECOVER"),
    ("PSYDUCK",22,"ITEM_NONE","WATER_GUN CONFUSION DISABLE TAIL_WHIP"),
    ("GOLDUCK",24,"ITEM_NONE","WATER_PULSE CONFUSION AMNESIA HYPNOSIS"),
    ("STARMIE",25,"ITEM_LUM_BERRY","PSYBEAM RECOVER BUBBLE_BEAM THUNDER_WAVE"),
]
assert len(slots)==4
for (iv,lvl,sp,item,raw),(wsp,wlvl,witem,wmoves) in zip(slots,expected):
    assert int(iv)==0 and (sp,int(lvl),item)==(wsp,wlvl,witem)
    actual=[x.strip() for x in raw.split(",")]
    assert actual==["MOVE_"+x for x in wmoves.split()]
    assert all(re.search(r"\b"+re.escape(x)+r"\b",moves) for x in actual)
meta=re.search(r"\[TRAINER_LEADER_MISTY\] = \{(.*?)\n    \},",trainers,re.S)
assert meta
for want in (
".items = {ITEM_SUPER_POTION}",".doubleBattle = TRUE",
".party = ITEM_CUSTOM_MOVES(sParty_LeaderMisty)",
".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY",
): assert want in meta[1],want
assert ("trainerbattle_double TRAINER_LEADER_MISTY, CeruleanCity_Gym_Text_MistyIntro, "
        "CeruleanCity_Gym_Text_MistyDefeat, SaffronCity_Dojo_Text_NeedTwo, "
        "CeruleanCity_Gym_EventScript_MistyDefeated, NO_MUSIC") in scripts
assert "trainerbattle_single TRAINER_LEADER_MISTY" not in scripts
assert "FLAG_BADGE02_GET" in scripts and "ITEM_TM03" in scripts
# Opponent-specific ability getter must keep wild, linked and player Pokémon unaffected.
ability=re.search(r"u8 GetBattlerInitialAbility\(u8 battler\)\s*\{(.*?)\n\}",battle,re.S)
assert ability
body=ability[1]
assert "gTrainerBattleOpponent_A == TRAINER_LEADER_MISTY" in body
assert "GetBattlerSide(battler) == B_SIDE_OPPONENT" in body
assert "BATTLE_TYPE_LINK" in body and "BATTLE_TYPE_TRAINER" in body
for sp,ability_name in [
("STARYU","NATURAL_CURE"),
("PSYDUCK","DAMP"),
("GOLDUCK","SYNCHRONIZE"),
("STARMIE","DRIZZLE"),
]:
    assert re.search(r"if \(species == SPECIES_"+sp+r"\)\s*return ABILITY_"+ability_name+r";",body),sp
    block=re.search(r"\[SPECIES_"+sp+r"\] = \{(.*?)\n    \},",species_info,re.S)
    assert block and ".abilities = " in block[1]
assert "return GetMonAbility(" in body
print("GYM-002 Misty source PASS: ordered 4, Lum, moves, Double/refusal, single Super Potion, original AI/badge/TM03 and 4 trainer-only abilities.")
