#!/usr/bin/env python3
"""WRL-010 source-only FireRed Steelix 64x64 resource risk audit.

No gameplay code, save-state assumptions or emulator success are implied.
"""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
graphics = (ROOT / "src/data/object_events/object_event_graphics_info.h").read_text()
movement = (ROOT / "src/event_object_movement.c").read_text()
constants = (ROOT / "include/constants/event_objects.h").read_text()
match = re.search(r"gObjectEventGraphicsInfo_Seagallop\s*=\s*\{(.*?)\};", graphics, re.S)
assert match, "Existing 64x64 Seagallop graphics record missing"
for token in (
    ".size = 2048,", ".width = 64,", ".height = 64,",
    ".oam = &gObjectEventBaseOam_64x64,",
    ".subspriteTables = gObjectEventSpriteOamTables_64x64,"
):
    assert token in match.group(1), "64x64 OAM precedent changed: " + token

count = re.search(r"#define NUM_OBJ_EVENT_GFX\s+(\d+)\b", constants)
assert count and 152 <= int(count.group(1)) < 240, "graphics IDs must remain 8-bit and below variable IDs"
assert re.search(r"#define OBJ_EVENT_GFX_VARS\s+240\b", constants)
assert 32 * 32 // 2 == 512
assert 64 * 64 // 2 == 2048
assert 64 * 64 // 64 == 64  # 64 hardware 8x8 tiles per 64x64 frame
assert "void ObjectEventSetGraphicsId(" in movement
setter = movement.split("void ObjectEventSetGraphicsId(", 1)[1].split("\nvoid ", 1)[0]
uses_fallback_to_old_frame_size = (
    "var = sprite->images->size / TILE_SIZE_4BPP;" in setter
    and "var3 = sprite->images->size / TILE_SIZE_4BPP;" in setter
    and "var2 = AllocSpriteTiles(var3);" in setter
    and "var2 = AllocSpriteTiles(var);" in setter
)
assert uses_fallback_to_old_frame_size, (
    "Ordinary graphics switching changed: re-audit resize allocation safety"
)
print("PASS: FireRed 64x64 sprite/OAM descriptor exists; Steelix single frame needs 2048 bytes/64 tiles")
print("RISK: ObjectEventSetGraphicsId can retry with the OLD smaller tile allocation size")
print("REQUIREMENT: do not use old-size allocation fallback for 32->64 follower switching")
print("NOT PROVEN: available runtime OBJ VRAM, object-event slots, dynamic palette and reflection safety")
print("NOT RUN: compiled mGBA 64x64 follower graphics allocation or transition test")
