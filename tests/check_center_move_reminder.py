#!/usr/bin/env python3
"""Issue #191 static coverage guard. Gameplay/UI behavior still requires native mGBA."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CENTER_MAPS = sorted((ROOT / "data/maps").glob("*PokemonCenter_1F/map.json"))
SCRIPT = "EventScript_FreePkmnCenterMoveReminder"
assert len(CENTER_MAPS) == 19, f"Expected all 19 Centers, found {len(CENTER_MAPS)}"
for path in CENTER_MAPS:
    data = json.loads(path.read_text(encoding="utf-8"))
    objects = data["object_events"]
    reminders = [obj for obj in objects if obj["script"] == SCRIPT]
    assert len(reminders) == 1, f"{path}: expected exactly one free Move Reminder"
    reminder = reminders[0]
    assert reminder["flag"] == "0", f"{path}: must not hide service behind flag"
    assert reminder["trainer_type"] == "TRAINER_TYPE_NONE", f"{path}: must be interactive"
    coords = [(obj["x"], obj["y"]) for obj in objects]
    assert len(coords) == len(set(coords)), f"{path}: overlapping NPCs"
shared = (ROOT / "data/scripts/free_center_move_reminder.inc").read_text(encoding="utf-8")
assembly = (ROOT / "data/event_scripts.s").read_text(encoding="utf-8")
assert 'data/scripts/move_tutors.inc' in assembly
assert 'data/scripts/free_center_move_reminder.inc' in (ROOT / "data/scripts/move_tutors.inc").read_text(encoding="utf-8")
assert SCRIPT + "::" in shared
for needed in ("ChooseMonForMoveRelearner", "TeachMoveRelearnerMove",
               "IsSelectedMonEgg", "waitstate", "release"):
    assert needed in shared, f"Missing expected FireRed mechanics: {needed}"
for forbidden in ("checkitem", "removeitem", "removemoney", "FLAG_TUTOR_"):
    assert forbidden not in shared, f"Unexpected payment or one-time gating: {forbidden}"
old = (ROOT / "data/maps/TwoIsland_House/scripts.inc").read_text(encoding="utf-8")
assert "TwoIsland_House_EventScript_MoveManiac::" in old
assert "removeitem ITEM_BIG_MUSHROOM" in old
print("PASS: exactly one free service in all 19 Centers; shared UI, no payment, original Move Maniac retained")
