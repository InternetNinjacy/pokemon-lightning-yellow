"""Precisely reverse issue #191 map-only NPC additions for older frozen digests.

The new event is checked against all approved fields before removal; unrelated edits
remain visible to the original historical SHA-256 preservation checks.
"""
from pathlib import Path
import json

def restore_free_center_reminder_text(path, source):
    path = Path(path)
    if not (path.name == "map.json" and path.parent.name.endswith("PokemonCenter_1F")
            and path.parent.parent.name == "maps"):
        return source
    obj = json.loads(source)
    candidates = [o for o in obj["object_events"]
                  if o.get("script") == "EventScript_FreePkmnCenterMoveReminder"]
    assert len(candidates) == 1, f"Missing or duplicate approved reminder: {path}"
    actual = obj["object_events"][-1]
    x, y = (18, 13) if path.parent.name.startswith("IndigoPlateau") else (10, 4)
    expected = {
        "type": "object",
        "graphics_id": "OBJ_EVENT_GFX_POKE_MANIAC",
        "x": x, "y": y,
        "elevation": 3,
        "movement_type": "MOVEMENT_TYPE_FACE_DOWN",
        "movement_range_x": 1,
        "movement_range_y": 1,
        "trainer_type": "TRAINER_TYPE_NONE",
        "trainer_sight_or_berry_tree_id": "0",
        "script": "EventScript_FreePkmnCenterMoveReminder",
        "flag": "0",
    }
    assert actual == expected, f"Unapproved Center event delta: {path}"
    obj["object_events"].pop()
    return json.dumps(obj, indent=2) + "\n"
