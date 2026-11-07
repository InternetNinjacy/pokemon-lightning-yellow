#!/usr/bin/env python3
"""Focused source-contract checks for WRL-001's Pikachu opening wiring.

These are deliberately static checks; they are not substitutes for mGBA traversal.
"""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
SCRIPT = (ROOT / "data/maps/PalletTown_ProfessorOaksLab/scripts.inc").read_text()
TEXT = (ROOT / "data/maps/PalletTown_ProfessorOaksLab/text.inc").read_text()
SPECIALS = (ROOT / "data/specials.inc").read_text()
GIFTS = (ROOT / "src/approved_gifts.c").read_text()


def block(label):
    match = re.search(r"^" + re.escape(label) + r"::\n(.*?)(?=^[A-Za-z_][A-Za-z_0-9]*::|\Z)", SCRIPT, re.M | re.S)
    assert match is not None, f"missing script: {label}"
    return match.group(1)


def check():
    middle = block("PalletTown_ProfessorOaksLab_EventScript_SquirtleBall")
    assert "setvar PLAYER_STARTER_SPECIES, SPECIES_PIKACHU" in middle
    assert "setvar RIVAL_STARTER_SPECIES, SPECIES_DRATINI" in middle
    assert "setvar RIVAL_STARTER_ID, LOCALID_CHARMANDER_BALL" in middle
    assert "setvar PLAYER_STARTER_NUM, 0" in middle
    assert "EventScript_ConfirmStarterChoice" in middle

    for name in ("BulbasaurBall", "CharmanderBall"):
        unavailable = block("PalletTown_ProfessorOaksLab_EventScript_" + name)
        assert "EventScript_ReservedPokeBall" in unavailable
        assert "givemon" not in unavailable
        assert "SPECIES_BULBASAUR" not in unavailable
        assert "SPECIES_CHARMANDER" not in unavailable

    choice = block("PalletTown_ProfessorOaksLab_EventScript_ChoseStarter")
    assert "givemon PLAYER_STARTER_SPECIES, 5" in choice
    assert "copyvar VAR_STARTER_MON, PLAYER_STARTER_NUM" in choice
    assert "special ScriptNameSocketStarterPikachu" in choice
    assert "Text_GiveNicknameToThisMon" not in choice
    assert "setflag FLAG_SYS_POKEMON_GET" in choice

    rival = block("PalletTown_ProfessorOaksLab_EventScript_RivalPicksStarter")
    assert "PLAYER_STARTER_NUM, 0" in rival
    assert "EventScript_RivalWalksToCharmander" in rival

    battle = block("PalletTown_ProfessorOaksLab_EventScript_RivalBattle")
    assert "VAR_STARTER_MON, 0" in battle
    assert "EventScript_RivalApproachForBattleCharmander" in battle

    assert "PalletTown_ProfessorOaksLab_Text_OakChoosingPikachu::" in TEXT
    assert "PalletTown_ProfessorOaksLab_Text_ReservedPokeBall::" in TEXT
    assert "def_special ScriptNameSocketStarterPikachu" in SPECIALS
    assert "void ScriptNameSocketStarterPikachu(void)" in GIFTS
    assert '_("SOCKET")' in GIFTS
    assert "SPECIES_PIKACHU" in GIFTS
    flags = (ROOT / "include/constants/flags.h").read_text()
    assert "#define FLAG_OAK_EEVEE_PENDING                   0x31C" in flags
    assert "#define FLAG_GOT_OAK_PARCEL_EEVEE                0x31D" in flags
    # Rocket quest reserve 0x316..0x31B must survive intact.
    for reserved in range(0x316, 0x31C):
        assert f"FLAG_0x{reserved:03X}" in flags
    assert "FLAG_OAK_EEVEE_PENDING" in SCRIPT
    assert "FLAG_GOT_OAK_PARCEL_EEVEE" in SCRIPT
    assert "setflag FLAG_OAK_EEVEE_PENDING" in block("PalletTown_ProfessorOaksLab_EventScript_ReceiveDexScene")
    assert "call PalletTown_ProfessorOaksLab_EventScript_TryGiveEevee" in block("PalletTown_ProfessorOaksLab_EventScript_ReceiveDexScene")
    gift = block("PalletTown_ProfessorOaksLab_EventScript_TryGiveEevee")
    assert "specialvar VAR_RESULT, ScriptGiveSueEevee" in gift
    assert "PalletTown_ProfessorOaksLab_EventScript_EeveeDelivered" in gift
    assert "PalletTown_ProfessorOaksLab_EventScript_EeveeStored" in gift
    assert "PalletTown_ProfessorOaksLab_Text_EeveeNoRoom" in gift
    for label in ("EeveeDelivered", "EeveeStored"):
        success = block("PalletTown_ProfessorOaksLab_EventScript_" + label)
        assert "setflag FLAG_GOT_OAK_PARCEL_EEVEE" in success
        assert "clearflag FLAG_OAK_EEVEE_PENDING" in success
    assert "call_if_set FLAG_OAK_EEVEE_PENDING" in block("PalletTown_ProfessorOaksLab_EventScript_RetryParcelGifts")
    assert "call_if_set FLAG_OAK_NEST_BALLS_PENDING" in block("PalletTown_ProfessorOaksLab_EventScript_RetryParcelGifts")
    assert 'CreateMon(mon, SPECIES_EEVEE, 5, USE_RANDOM_IVS, FALSE, 0, OT_ID_PLAYER_ID, 0)' in GIFTS
    assert '_("SUE")' in GIFTS
    assert "def_special ScriptGiveSueEevee" in SPECIALS
    assert "FLAG_OAK_NEST_BALLS_PENDING" in SCRIPT
    assert "additem ITEM_NEST_BALL, 5" in SCRIPT
    assert "clearflag FLAG_OAK_NEST_BALLS_PENDING" in SCRIPT
    print("WRL-001 Pikachu opening source checks: PASS")


if __name__ == "__main__":
    check()
