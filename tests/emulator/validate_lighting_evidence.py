#!/usr/bin/env python3
"""Validate *genuine* mGBA TIM-003 capture evidence; never synthesize frames.

Usage: python3 tests/emulator/validate_lighting_evidence.py /path/to/manifest.json
Each capture record: id, file, map, time (HH:MM), weather, variant, commit,
x, y, frames, sha256. A variant dictionary maps candidate name to exact numeric
palette parameters. The capture producer must use mGBA's 240x160 framebuffer.
"""
import hashlib
import json
import pathlib
import re
import struct
import sys

PNG_MAGIC = b"\x89PNG\r\n\x1a\n"
REQUIRED = {"id", "file", "map", "time", "weather", "variant", "commit",
            "x", "y", "frames", "sha256"}
EXPECTED = {
    "pallet_dawn": ("Pallet Town", "06:30"),
    "pallet_day": ("Pallet Town", "12:00"),
    "pallet_dusk": ("Pallet Town", "18:30"),
    "pallet_night": ("Pallet Town", "21:00"),
    "route1_dawn": ("Route 1", "06:30"),
    "route1_day": ("Route 1", "12:00"),
    "route1_dusk": ("Route 1", "18:30"),
    "route1_night": ("Route 1", "21:00"),
    "forest_day": ("Viridian Forest", "12:00"),
    "forest_night": ("Viridian Forest", "21:00"),
    "lavender_day": ("Lavender Town", "12:00"),
    "lavender_night": ("Lavender Town", "21:00"),
    "mansion_1f": ("Pokemon Mansion 1F", "12:00"),
    "mansion_2f": ("Pokemon Mansion 2F", "12:00"),
    "mansion_3f": ("Pokemon Mansion 3F", "12:00"),
    "mansion_b1f": ("Pokemon Mansion B1F", "12:00"),
}
def validate_manifest(path):
    root = path.resolve().parent
    data = json.loads(path.read_text(encoding="utf-8"))
    captures = data["captures"]
    variants = data["variants"]
    if not isinstance(variants, dict) or not variants:
        raise ValueError("Missing numeric candidate palette variant manifest")
    for name, params in variants.items():
        if not isinstance(params, dict) or not params or any(
            not isinstance(v, (int, float)) or isinstance(v, bool)
            for v in params.values()
        ):
            raise ValueError("Variant %s lacks complete numeric parameters" % name)
    found = {}
    for record in captures:
        missing = REQUIRED - record.keys()
        if missing:
            raise ValueError("Capture missing fields: %s" % sorted(missing))
        key = (record["id"], record["variant"])
        if key in found:
            raise ValueError("Duplicate ID/variant %r" % (key,))
        found[key] = record
        if record["variant"] not in variants:
            raise ValueError("Unknown palette variant %r" % record["variant"])
        if not re.fullmatch(r"[0-9a-fA-F]{40}", record["commit"]):
            raise ValueError("Full Git commit SHA required")
        if not re.fullmatch(r"(?:[01][0-9]|2[0-3]):[0-5][0-9]", record["time"]):
            raise ValueError("Invalid local RTC time")
        if not all(isinstance(record[k], int) and not isinstance(record[k], bool)
                   for k in ("x", "y", "frames")) or record["frames"] < 1:
            raise ValueError("Invalid position or elapsed emulated frames")
        image_path = (root / record["file"]).resolve()
        if not image_path.is_relative_to(root):
            raise ValueError("Capture escapes evidence directory")
        pixels = image_path.read_bytes()
        if pixels[:8] != PNG_MAGIC or len(pixels) < 24:
            raise ValueError("Not a PNG: %s" % image_path)
        if pixels[12:16] != b"IHDR" or struct.unpack(">II", pixels[16:24]) != (240, 160):
            raise ValueError("Not native 240x160 PNG: %s" % image_path)
        if hashlib.sha256(pixels).hexdigest() != record["sha256"]:
            raise ValueError("Capture SHA256 mismatch: %s" % image_path)
        if record["id"] in EXPECTED:
            expected_map, expected_time = EXPECTED[record["id"]]
            if (record["map"], record["time"]) != (expected_map, expected_time):
                raise ValueError("Capture does not match matrix: " + record["id"])
    present = {record["id"] for record in captures}
    missing = set(EXPECTED) - present
    if missing:
        raise ValueError("Missing required fixed captures: " + ", ".join(sorted(missing)))
    for name in ("pallet", "route1"):
        group = [r for r in captures if r["id"].startswith(name + "_")
                 and r["variant"] == "A"]
        if len(group) == 4 and len({(r["x"], r["y"]) for r in group}) != 1:
            raise ValueError("Camera/player position changes in " + name)
    print("TIM-003 evidence metadata validated: %d native PNG captures, %d candidates"
          % (len(captures), len(variants)))
    print("NOTE: file checks cannot prove emulator provenance; attach capture logs and ROM evidence.")
if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit("usage: validate_lighting_evidence.py manifest.json")
    try:
        validate_manifest(pathlib.Path(sys.argv[1]))
    except (OSError, KeyError, TypeError, ValueError) as exc:
        sys.exit("TIM-003 evidence FAIL: %s" % exc)
