#!/usr/bin/env python3
"""Convert real native mGBA PPM frames to lossless PNG review evidence.

This script NEVER renders or alters game imagery. It only losslessly encodes
pixels emitted by the linked ROM-running mGBA runner. Keep the original PPMs.
"""
import argparse
import hashlib
import json
import pathlib
import struct
import subprocess
import zlib

# Exact fixture matrix in tests/emulator/lighting_preview.c.
ROWS = [
    ("pallet_dawn", "Pallet Town", "06:30", 8, 8, "Neutral"),
    ("pallet_day", "Pallet Town", "12:00", 8, 8, "Neutral"),
    ("pallet_dusk", "Pallet Town", "18:30", 8, 8, "Neutral"),
    ("pallet_night", "Pallet Town", "21:00", 8, 8, "Neutral"),
    ("route1_dawn", "Route 1", "06:30", 9, 12, "Neutral"),
    ("route1_day", "Route 1", "12:00", 9, 12, "Neutral"),
    ("route1_dusk", "Route 1", "18:30", 9, 12, "Neutral"),
    ("route1_night", "Route 1", "21:00", 9, 12, "Neutral"),
    ("forest_day", "Viridian Forest", "12:00", 8, 12, "Map-defined"),
    ("forest_night", "Viridian Forest", "21:00", 8, 12, "Map-defined"),
    ("lavender_day", "Lavender Town", "12:00", 9, 8, "Map-defined"),
    ("lavender_night", "Lavender Town", "21:00", 9, 8, "Map-defined"),
    ("mansion_1f", "Pokemon Mansion 1F", "12:00", 8, 8, "Indoor"),
    ("mansion_2f", "Pokemon Mansion 2F", "12:00", 8, 8, "Indoor"),
    ("mansion_3f", "Pokemon Mansion 3F", "12:00", 8, 8, "Indoor"),
    ("mansion_b1f", "Pokemon Mansion B1F", "12:00", 8, 8, "Indoor"),
    ("interior_day", "Pewter Museum 1F", "12:00", 11, 5, "Indoor"),
    ("interior_night", "Pewter Museum 1F", "21:00", 11, 5, "Indoor"),
    ("cave_day", "Rock Tunnel 1F", "12:00", 8, 8, "Indoor"),
    ("cave_night", "Rock Tunnel 1F", "21:00", 8, 8, "Indoor"),
    ("weather_rain_day", "One Island", "12:00", 6, 10, "Rain"),
    ("weather_rain_night", "One Island", "21:00", 6, 10, "Rain"),
    ("weather_sand_day", "Route 17", "12:00", 6, 10, "Sandstorm"),
    ("weather_sand_night", "Route 17", "21:00", 6, 10, "Sandstorm"),
]
PALETTES = {
    "B": {"day": (32,32,32), "dawn": (23,28,32), "dusk": (32,23,17),
          "night": (16,19,27), "covered_day": (25,29,25),
          "covered_night": (12,17,22), "lavender_day": (30,22,19)},
}

def chunk(name, payload):
    return struct.pack(">I", len(payload)) + name + payload + struct.pack(">I", zlib.crc32(name + payload) & 0xffffffff)

def read_real_frame(ppm):
    data = ppm.read_bytes()
    header = b"P6\n240 160\n255\n"
    if not data.startswith(header) or len(data) != len(header) + 240*160*3:
        raise ValueError(f"Expected native 240x160 RGB24 mGBA PPM: {ppm}")
    return data[len(header):]

def write_png(path, rgb):
    rows = b"".join(b"\x00" + rgb[y*240*3:(y+1)*240*3] for y in range(160))
    ihdr = struct.pack(">IIBBBBB", 240, 160, 8, 2, 0, 0, 0)
    data = b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", ihdr) + chunk(b"IDAT", zlib.compress(rows, 9)) + chunk(b"IEND", b"")
    path.write_bytes(data)
    return hashlib.sha256(data).hexdigest()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("directory", type=pathlib.Path, help="mGBA LIGHTNING_LIGHTING_CAPTURE_DIR")
    parser.add_argument("--variant", choices=("B",), required=True)
    parser.add_argument("--commit", help="40-digit ROM source Git commit, defaults to current HEAD")
    parser.add_argument("--sheet", action="store_true", help="generate labeled contact sheet (requires Pillow)")
    args = parser.parse_args()
    root = args.directory.resolve()
    commit = args.commit or subprocess.check_output(["git", "rev-parse", "HEAD"], text=True).strip()
    if len(commit) != 40 or any(c not in "0123456789abcdefABCDEF" for c in commit):
        raise ValueError("Full compiled-ROM source commit SHA required")
    params = {f"{name}_{channel}": value for name, triplet in PALETTES[args.variant].items()
              for channel, value in zip("rgb", triplet)}
    captures = []
    for ident, location, clock, x, y, weather in ROWS:
        rgb = read_real_frame(root / f"{ident}.ppm")
        png = root / f"{ident}.png"
        digest = write_png(png, rgb)
        captures.append({"id": ident, "file": png.name, "map": location, "time": clock,
                         "weather": weather, "variant": args.variant, "commit": commit,
                         "x": x, "y": y, "frames": 180, "sha256": digest})
    manifest = {"source": "actual mGBA ROM framebuffer (native 240x160); see attached runtime log",
                "prototype_only": True, "owner_palette_approved": False,
                "variants": {args.variant: params}, "captures": captures}
    (root / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    if args.sheet:
        try:
            from PIL import Image, ImageDraw
        except ImportError as exc:
            raise RuntimeError("Contact sheet requires Pillow; individual evidence PNGs and manifest have been saved") from exc
        scale = 2
        cellw, cellh = 480, 350
        cols = 4
        sheet = Image.new("RGB", (cols*cellw, ((len(captures)+cols-1)//cols)*cellh), (238,238,238))
        draw = ImageDraw.Draw(sheet)
        for n, row in enumerate(captures):
            img = Image.open(root / row["file"]).convert("RGB").resize((480, 320), Image.Resampling.NEAREST)
            left, top = (n % cols)*cellw, (n // cols)*cellh
            sheet.paste(img, (left, top+25))
            draw.text((left+8, top+7), f"{row['id']} | {row['time']} | {row['weather']} | {args.variant}", fill=(0,0,0))
        sheet.save(root / "contact_sheet.png")
    print(f"Converted {len(captures)} genuine mGBA PPMs to PNG, created manifest for {args.variant}")
    print("NOTE: manifest provenance must be supported by the original mGBA runner log and matching build")

if __name__ == "__main__":
    main()
