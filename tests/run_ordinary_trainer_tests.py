#!/usr/bin/env python3
"""Verify the owner's Nov 6 ordinary-Trainer implementation lock against source.

Read-only; no ROM artifacts, no external dependencies. This is a structural gate,
not an emulator encounter or move-balance simulation.
"""
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LOCK = ROOT / "docs/ordinary-trainers-implementation-lock.json"
TRAINERS = ROOT / "src/data/trainers.h"
PARTIES = ROOT / "src/data/trainer_parties.h"
VS = ROOT / "src/vs_seeker.c"
SCRIPTS = ROOT / "data/scripts/trainers.inc"
error = []


def require(value, message):
    if not value:
        error.append(message)


def assignments(text):
    return dict(re.findall(r"\.(\w+)\s*=\s*([^,\n]+)", text))


def main():
    lock = json.loads(LOCK.read_text(encoding="utf-8"))
    entries = lock["entries"]
    source = TRAINERS.read_text(encoding="utf-8")
    parties = PARTIES.read_text(encoding="utf-8")
    vs = VS.read_text(encoding="utf-8")
    script = SCRIPTS.read_text(encoding="utf-8")
    defs = {m.group(1): m.group(2) for m in re.finditer(
        r"\[(TRAINER_[A-Z0-9_]+)\]\s*=\s*\{(.*?)\n    \},", source, re.S)}
    arrays = {m.group(2): (m.group(1), m.group(3))
              for m in re.finditer(
                  r"static const struct (TrainerMon\w+)\s+(sParty_\w+)\[\]\s*=\s*\{(.*?)\};",
                  parties, re.S)}
    species = set(re.findall(r"\bSPECIES_[A-Z0-9_]+\b",
                             (ROOT / "include/constants/species.h").read_text()))
    moves = set(re.findall(r"\bMOVE_[A-Z0-9_]+\b",
                           (ROOT / "include/constants/moves.h").read_text()))

    require(len(entries) == 510, "510 ledger entries required")
    dummy_count = implemented = total_mons = item_count = 0
    for trainer_id, record in entries.items():
        definition = defs.get(trainer_id)
        if definition is None:
            error.append(f"{trainer_id}: trainer definition missing")
            continue
        party = re.search(r"\.party\s*=\s*(\w+)\((sParty_\w+)\)", definition)
        if not party:
            error.append(f"{trainer_id}: party macro missing")
            continue
        macro, symbol = party.groups()
        require(symbol == record["party_symbol"], f"{trainer_id}: party symbol altered")
        if symbol not in arrays:
            error.append(f"{trainer_id}: party array absent ({symbol})")
            continue
        array_type, body = arrays[symbol]
        if record["source_dummy"]:
            dummy_count += 1
            require("DUMMY_TRAINER_MON" in body,
                    f"{trainer_id}: intentionally dummy source party changed")
            continue
        implemented += 1
        expected = record["mons"]
        chunks = body.split("\n    {\n")[1:]
        require(len(chunks) == len(expected),
                f"{trainer_id}: expected {len(expected)} mons; found {len(chunks)}")
        has_item = any(mon["held_item"] for mon in expected)
        require(array_type == ("TrainerMonItemCustomMoves" if has_item
                               else "TrainerMonNoItemCustomMoves"),
                f"{trainer_id}: wrong C party type")
        require(macro == ("ITEM_CUSTOM_MOVES" if has_item
                          else "NO_ITEM_CUSTOM_MOVES"),
                f"{trainer_id}: wrong Trainer party macro")
        for index, desired in enumerate(expected):
            total_mons += 1
            if index >= len(chunks):
                break
            chunk = chunks[index].split("\n    },", 1)[0]
            found_species = re.search(r"\.species\s*=\s*(SPECIES_\w+)", chunk)
            found_level = re.search(r"\.lvl\s*=\s*(\d+)", chunk)
            found_iv = re.search(r"\.iv\s*=\s*(\d+)", chunk)
            found_item = re.search(r"\.heldItem\s*=\s*(ITEM_\w+)", chunk)
            found_moves = re.search(r"\.moves\s*=\s*\{([^}]+)\}", chunk)
            found_list = re.findall(r"MOVE_[A-Z0-9_]+", found_moves.group(1)) if found_moves else []
            require(found_species is not None and found_species.group(1) == desired["species"],
                    f"{trainer_id} slot {index}: species discrepancy")
            require(desired["species"] in species,
                    f"{trainer_id} slot {index}: undefined species")
            require(found_level is not None and int(found_level.group(1)) == desired["level"],
                    f"{trainer_id} slot {index}: level discrepancy")
            require(1 <= desired["level"] <= 100,
                    f"{trainer_id} slot {index}: illegal level")
            require(found_iv is not None and int(found_iv.group(1)) == desired["iv"],
                    f"{trainer_id} slot {index}: IV discrepancy")
            require((found_item.group(1) if found_item else None) == desired["held_item"],
                    f"{trainer_id} slot {index}: item discrepancy")
            if desired["held_item"]:
                item_count += 1
            expected_moves = desired["moves"] + ["MOVE_NONE"] * (4 - len(desired["moves"]))
            require(found_list == expected_moves,
                    f"{trainer_id} slot {index}: move discrepancy")
            require(all(move in moves for move in expected_moves),
                    f"{trainer_id} slot {index}: undefined move")
        require(len(expected) <= 6, f"{trainer_id}: party larger than six")

    conversions = {
        "TRAINER_CAMPER_SHANE": ("BUG_CATCHER", "BUG_CATCHER", None, "TRUE"),
        "TRAINER_YOUNGSTER_NASH": ("PKMN_RANGER", "POKEMON_RANGER_M", None, "FALSE"),
        "TRAINER_LASS_DALIA": ("PKMN_RANGER", "POKEMON_RANGER_F", "DAHLIA", "FALSE"),
        "TRAINER_LASS_JOANA": ("PKMN_RANGER", "POKEMON_RANGER_F", "JOANNA", "FALSE"),
    }
    for trainer_id, (trainer_class, pic, name, double) in conversions.items():
        definition = defs[trainer_id]
        require(f"TRAINER_CLASS_{trainer_class}" in definition,
                f"{trainer_id}: class conversion missing")
        require(f"TRAINER_PIC_{pic}" in definition,
                f"{trainer_id}: portrait conversion missing")
        require(f".doubleBattle = {double}" in definition,
                f"{trainer_id}: incorrect battle format")
        if name:
            require(f'.trainerName = _("{name}")' in definition,
                    f"{trainer_id}: approved name correction missing")

    require(re.search(r"trainerbattle_double TRAINER_CAMPER_SHANE\b", script) is not None,
            "Shane original Double script missing")
    require(re.search(r"trainerbattle_rematch_double TRAINER_CAMPER_SHANE\b", script) is not None,
            "Shane rematch Double script missing")
    route24 = json.loads((ROOT / "data/maps/Route24/map.json").read_text())
    shane = [e for e in route24["object_events"]
             if e.get("script") == "Route24_EventScript_Shane"]
    require(len(shane) == 1 and shane[0]["graphics_id"] == "OBJ_EVENT_GFX_BUG_CATCHER",
            "Shane Bug Catcher map sprite not connected")
    bush = json.loads((ROOT / "data/maps/SixIsland_PatternBush/map.json").read_text())
    for label, graphic in (("Nash", "OBJ_EVENT_GFX_LIGHTNING_RANGER_M"),
                           ("Dalia", "OBJ_EVENT_GFX_LIGHTNING_RANGER_F"),
                           ("Joana", "OBJ_EVENT_GFX_LIGHTNING_RANGER_F")):
        matches = [e for e in bush["object_events"]
                   if e.get("script") == "SixIsland_PatternBush_EventScript_" + label]
        require(len(matches) == 1 and matches[0]["graphics_id"] == graphic,
                f"{label}: Ranger overworld wiring changed")
    for trainer_id, record in entries.items():
        if record["role"] == "DELEGATED_REMATCH_FAMILY_LOCKED":
            require(trainer_id in vs, f"{trainer_id}: VS Seeker linkage missing")
        if record["role"] == "DELEGATED_SCRIPTED_STORY_CANDIDATE":
            story = (ROOT / "data/maps/ThreeIsland/scripts.inc").read_text()
            require(f"trainerbattle_no_intro {trainer_id}" in story,
                    f"{trainer_id}: scripted encounter missing")
            require(trainer_id not in vs,
                    f"{trainer_id}: do not add story battle to VS Seeker")

    require(implemented == 500 and dummy_count == 10,
            f"unexpected implemented/dummy count: {implemented}/{dummy_count}")
    require(total_mons == 1219, f"expected 1219 party slots, got {total_mons}")
    require(item_count == 170, f"expected 170 held-item slots, got {item_count}")
    if error:
        print(f"Ordinary Trainer lock verification FAILED: {len(error)} issues")
        for message in error[:100]:
            print(" -", message)
        raise SystemExit(1)
    print("Ordinary Trainer source verification PASS: 510 entries, 500 real parties, "
          "10 dummy entries, 1219 mon slots, 170 item slots, four conversions, "
          "Shane Double+rematch and 130 VS Seeker variants.")


if __name__ == "__main__":
    main()
