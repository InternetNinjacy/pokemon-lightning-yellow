#!/usr/bin/env python3
"""Focused source-contract checks for WRL-001's Pikachu opening wiring.

These are deliberately static checks; they are not substitutes for mGBA traversal.
"""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
SCRIPT = (ROOT / "data/maps/PalletTown_ProfessorOaksLab/scripts.inc").read_text()
TEXT = (ROOT / "data/maps/PalletTown_ProfessorOaksLab/text.inc").read_text()


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
    assert "setflag FLAG_SYS_POKEMON_GET" in choice

    rival = block("PalletTown_ProfessorOaksLab_EventScript_RivalPicksStarter")
    assert "PLAYER_STARTER_NUM, 0" in rival
    assert "EventScript_RivalWalksToCharmander" in rival

    battle = block("PalletTown_ProfessorOaksLab_EventScript_RivalBattle")
    assert "VAR_STARTER_MON, 0" in battle
    assert "EventScript_RivalApproachForBattleCharmander" in battle

    assert "PalletTown_ProfessorOaksLab_Text_OakChoosingPikachu::" in TEXT
    assert "PalletTown_ProfessorOaksLab_Text_ReservedPokeBall::" in TEXT
    print("WRL-001 Pikachu opening source checks: PASS")


if __name__ == "__main__":
    check()
