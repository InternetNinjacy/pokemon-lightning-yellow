#!/usr/bin/env python3
"""WRL-010 pinned follower asset validation. Downloads to temporary memory only.

This is an asset-format test, NOT FireRed runtime, movement, licensing, or frame-art
certification. Never commit proprietary sprites or ROM data.
"""
import csv
import hashlib
import io
import json
import os
import re
import struct
import sys
import time
import urllib.error
import urllib.request
import zlib

from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REV = "cbe04a7a2ef645bee19d159876da074154ccf1f2"
BASE = "https://raw.githubusercontent.com/Kasenn/pokeemerald-expansion-kasen/" + REV + "/"
MAX_BYTES = 400000
CACHE = Path(os.environ.get("FOLLOWER_CACHE", "/tmp/lightning-follower-assets"))
CACHE.mkdir(parents=True, exist_ok=True)


def fetch(path):
    dest = CACHE / path
    if dest.is_file():
        return dest.read_bytes()
    url = BASE + path
    for attempt in range(4):
        try:
            req = urllib.request.Request(url, headers={"User-Agent": "LightningYellow-WRL010-AssetAudit/1"})
            with urllib.request.urlopen(req, timeout=35) as response:
                data = response.read(MAX_BYTES + 1)
            if not data or len(data) > MAX_BYTES:
                raise ValueError("empty/oversize asset: " + path)
            dest.parent.mkdir(parents=True, exist_ok=True)
            dest.write_bytes(data)
            return data
        except (OSError, urllib.error.URLError) as exc:
            if attempt == 3:
                raise RuntimeError("download failed: " + path) from exc
            time.sleep(1 + attempt * 2)


def git_blob_sha(data):
    return hashlib.sha1(b"blob " + str(len(data)).encode() + b"\0" + data).hexdigest()


def read_png(data):
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError("not PNG")
    pos = 8
    chunks = []
    while pos < len(data):
        if pos + 12 > len(data):
            raise ValueError("truncated PNG chunk header")
        length = struct.unpack_from(">I", data, pos)[0]
        name = data[pos + 4:pos + 8]
        body = data[pos + 8:pos + 8 + length]
        crc = struct.unpack_from(">I", data, pos + 8 + length)[0] if pos + 12 + length <= len(data) else -1
        if len(body) != length or crc != (zlib.crc32(name + body) & 0xffffffff):
            raise ValueError("PNG chunk CRC/truncation")
        chunks.append((name, body))
        pos += length + 12
        if name == b"IEND":
            break
    if pos != len(data):
        raise ValueError("extra bytes or missing IEND")
    ihdr = next((x for n, x in chunks if n == b"IHDR"), None)
    if ihdr is None or len(ihdr) != 13:
        raise ValueError("missing IHDR")
    w, h, depth, ctype, compression, filtering, interlace = struct.unpack(">IIBBBBB", ihdr)
    if (w, h) not in ((192, 32), (384, 64)) or (depth, ctype, compression, filtering, interlace) != (4, 3, 0, 0, 0):
        raise ValueError("unexpected PNG geometry or indexed format: " + str((w,h,depth,ctype,interlace)))
    cell_size = h  # Six square cells in a horizontal strip; never silently downscale.
    palette = next((x for n, x in chunks if n == b"PLTE"), b"")
    if len(palette) == 0 or len(palette) % 3 or len(palette) > 48:
        raise ValueError("invalid 4-bit palette")
    raw = zlib.decompress(b"".join(x for n, x in chunks if n == b"IDAT"))
    stride = (w * depth + 7) // 8
    if len(raw) != h * (stride + 1):
        raise ValueError("unexpected decompressed scanline length")
    rows = []
    offset = 0
    prior = bytearray(stride)
    for _ in range(h):
        filter_type = raw[offset]
        offset += 1
        row = bytearray(raw[offset:offset + stride])
        offset += stride
        if filter_type not in (0,1,2,3,4):
            raise ValueError("unsupported PNG filter")
        for j in range(stride):
            left = row[j-1] if j else 0
            up = prior[j]
            ul = prior[j-1] if j else 0
            if filter_type == 1:
                row[j] = (row[j] + left) & 255
            elif filter_type == 2:
                row[j] = (row[j] + up) & 255
            elif filter_type == 3:
                row[j] = (row[j] + ((left+up)//2)) & 255
            elif filter_type == 4:
                p = left + up - ul
                a,b,c = abs(p-left),abs(p-up),abs(p-ul)
                predictor = left if a<=b and a<=c else up if b<=c else ul
                row[j] = (row[j] + predictor) & 255
        rows.append([v for byte in row for v in (byte >> 4, byte & 15)])
        prior = row
    if any(v >= len(palette)//3 for row in rows for v in row):
        raise ValueError("out-of-range indexed PNG pixel")
    frames = []
    for frame in range(6):
        # Six square cells; upstream animation mapping is checked independently.
        pixels = tuple(v for row in rows for v in row[frame*cell_size:(frame+1)*cell_size])
        frames.append(pixels)
    if any(not any(pixel != 0 for pixel in frame) for frame in frames):
        raise ValueError("blank 32x32 directional animation cell")
    return {
        "dimensions": [w,h], "frame_cell_size": [cell_size,cell_size],
        "bit_depth": depth, "indexed_colors": len(palette)//3,
        "unique_frame_cells": len(set(frames)),
        "nonzero_frame_cells": sum(any(p != 0 for p in f) for f in frames),
        "cell_fingerprints": [hashlib.sha256(bytes(f)).hexdigest()[:12] for f in frames],
    }


def read_pal(data):
    lines = data.decode("ascii").splitlines()
    if lines[:3] != ["JASC-PAL", "0100", "16"] or len(lines) != 19:
        raise ValueError("JASC-PAL expected 16 colors")
    result = []
    for line in lines[3:]:
        values = [int(v) for v in line.split()]
        if len(values) != 3 or any(v < 0 or v > 255 for v in values):
            raise ValueError("invalid palette RGB")
        result.append(values)
    return result


def check_upstream_animation_contract():
    """Confirm the selected six-cell format maps to all four directions."""
    path = "src/data/object_events/object_event_anims.h"
    source = fetch(path).decode("utf-8")
    mapping = {
        "sAnim_GoSouth2F": [0, 1],
        "sAnim_GoNorth2F": [2, 3],
        "sAnim_GoWest2F": [4, 5],
        "sAnim_GoEast2F": [4, 5],
    }
    for routine, expected in mapping.items():
        match = re.search(r"static const union AnimCmd\s+" + routine +
                          r"\[\]\s*=\s*\{(.*?)\};", source, re.S)
        if match is None:
            raise ValueError("missing upstream animation " + routine)
        numbers = [int(v) for v in re.findall(r"ANIMCMD_FRAME\((\d+)\s*,", match.group(1))]
        if not numbers or set(numbers) != set(expected):
            raise ValueError("incompatible frame mapping " + routine + ": " + str(numbers))
        if routine == "sAnim_GoEast2F" and ".hFlip = TRUE" not in match.group(1):
            raise ValueError("east walking must flip the west-facing image")
    table_match = re.search(r"const union AnimCmd \*const sAnimTable_Following\[\]\s*=\s*\{(.*?)\};", source, re.S)
    if table_match is None:
        raise ValueError("missing generic follower animation table")
    table = table_match.group(1)
    for direction in ("SOUTH", "NORTH", "WEST", "EAST"):
        if ("[ANIM_STD_GO_" + direction + "] = sAnim_Go" + direction.title() + "2F") not in table:
            raise ValueError("missing generic walking animation for " + direction)
    graphics = fetch("src/data/object_events/object_event_graphics_info.h").decode("utf-8")
    record = re.search(r"gObjectEventGraphicsInfo_Follower\s*=\s*\{(.*?)\};", graphics, re.S)
    if record is None or ".anims = sAnimTable_Following," not in record.group(1):
        raise ValueError("follower graphics record uses another animation table")
    return {"layout": "six consecutive 32x32 cells, [south0,south1,north0,north1,west0,west1]",
            "east": "mirrored west", "upstream_animation_table": path,
            "graphics_info": "gObjectEventGraphicsInfo_Follower"}


def main():
    animation_contract = check_upstream_animation_contract()
    with (ROOT / "docs/followers/252-species-coverage.csv").open(newline="") as f:
        rows = list(csv.DictReader(f))
    if len(rows) != 252 or len({r["species"] for r in rows}) != 252 or rows[232]["species"] != "CASTFORM":
        raise ValueError("wrong 252 roster")
    results, failures, oversized = [], [], []
    for n, row in enumerate(rows, 1):
        name = row["species"]
        try:
            png = fetch(row["kasenn_sheet"])
            sha = git_blob_sha(png)
            if sha != row["kasenn_blob_sha"]:
                raise ValueError("blob SHA changed: " + sha)
            details = read_png(png)
            for variant in ("normal", "shiny"):
                key = "kasenn_" + variant + "_palette"
                read_pal(fetch(row[key]))
            result = {"dex": n, "species": name, "png_blob": sha, **details}
            results.append(result)
            if details["frame_cell_size"] != [32, 32]:
                oversized.append({"dex": n, "species": name,
                                  "cell": details["frame_cell_size"],
                                  "reason": "requires explicitly validated FireRed graphics strategy"})
            print(f"{n:03d}/252 {name}: indexed 192x32, {details['unique_frame_cells']} distinct cells", flush=True)
        except Exception as exc:
            failures.append({"dex": n, "species": name, "error": str(exc)})
            print(f"ERROR {n:03d}/252 {name}: {exc}", file=sys.stderr, flush=True)
    forms = {}
    for form in ("sunny","rainy","snowy"):
        forms[form] = {}
        for variant in ("normal","shiny"):
            path = f"graphics/pokemon/castform/{form}/overworld_{variant}.pal"
            try:
                read_pal(fetch(path))
                forms[form][variant] = "valid"
            except Exception as exc:
                forms[form][variant] = str(exc)
                failures.append({"form": form, "variant": variant, "error": str(exc)})
        path = f"graphics/pokemon/castform/{form}/overworld.png"
        try:
            forms[form]["sprite"] = read_png(fetch(path))
        except Exception as exc:
            forms[form]["sprite"] = str(exc)
            failures.append({"form": form, "sprite": str(exc)})
    report = {"source": BASE, "verified": len(results), "total": len(rows),
              "oversized_sheet_exceptions": oversized,
              "animation_contract": animation_contract,
              "format_checked": "PNG CRC, complete scanlines, 4-bit indexed pixels, six square cells (32 or 64)",
              "forms": forms, "species": results, "failures": failures,
              "limitations": ["Does not prove cell-to-facing animation table mapping",
                              "Does not prove frame art visually correct",
                              "Does not prove conversion or FireRed runtime resource budgets",
                              "Does not independently establish redistribution rights"]}
    report_path = Path(os.environ.get("FOLLOWER_REPORT", "/tmp/lightning-followers-validation.json"))
    report_path.write_text(json.dumps(report, indent=2) + "\n")
    print(f"SUMMARY verified={len(results)}/252 failures={len(failures)} oversized={len(oversized)} report={report_path}", flush=True)
    if failures or oversized or len(results) != 252:
        sys.exit(1)


if __name__ == "__main__":
    main()
