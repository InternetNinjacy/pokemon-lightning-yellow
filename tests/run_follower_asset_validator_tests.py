#!/usr/bin/env python3
"""Offline deterministic tests for WRL-010 asset validator primitives.

Generates synthetic indexed PNGs in memory; no external artwork or network.
"""
import importlib.util
import struct
import sys
import unittest
import zlib
from pathlib import Path

SCRIPT = Path(__file__).resolve().parents[1] / "scripts/verify-follower-assets.py"
spec = importlib.util.spec_from_file_location("follower_assets", SCRIPT)
mod = importlib.util.module_from_spec(spec)
spec.loader.exec_module(mod)


def chunk(tag, data):
    return struct.pack(">I", len(data)) + tag + data + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)


def png(side, corruption=False):
    width = side * 6
    height = side
    ihdr = struct.pack(">IIBBBBB", width, height, 4, 3, 0, 0, 0)
    # 16 separate palette entries; index zero intentionally not special-cased.
    palette = b"".join(bytes(((i * 13) % 256, (i * 17) % 256, (i * 19) % 256)) for i in range(16))
    scanlines = bytearray()
    for y in range(height):
        scanlines.append(0)
        values = [((x // side) + (x % side) // 8 + y // 8) % 15 + 1 for x in range(width)]
        scanlines.extend((values[j] << 4) | values[j + 1] for j in range(0, width, 2))
    data = b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", ihdr) + chunk(b"PLTE", palette) + chunk(b"IDAT", zlib.compress(bytes(scanlines))) + chunk(b"IEND", b"")
    if corruption:
        data = data[:-5] + bytes([data[-5] ^ 1]) + data[-4:]
    return data


class FollowerAssetTests(unittest.TestCase):
    def test_32x32_4bpp(self):
        result = mod.read_png(png(32))
        self.assertEqual(result["dimensions"], [192, 32])
        self.assertEqual(result["gba_4bpp_bytes_per_frame"], 512)
        self.assertEqual(result["gba_4bpp_total_six_frames"], 3072)
        self.assertEqual(result["unique_frame_cells"], 6)

    def test_64x64_4bpp(self):
        result = mod.read_png(png(64))
        self.assertEqual(result["dimensions"], [384, 64])
        self.assertEqual(result["gba_4bpp_bytes_per_frame"], 2048)
        self.assertEqual(result["gba_4bpp_total_six_frames"], 12288)

    def test_corrupt_chunk_crc(self):
        with self.assertRaises(ValueError):
            mod.read_png(png(32, corruption=True))

    def test_bad_png_signature(self):
        with self.assertRaises(ValueError):
            mod.read_png(b"not a png")

    def test_palette_format(self):
        pal = "JASC-PAL\n0100\n16\n" + "\n".join("0 0 0" for _ in range(16)) + "\n"
        self.assertEqual(len(mod.read_pal(pal.encode("ascii"))), 16)

    def test_tile_roundtrip_rejects_invalid_index(self):
        pixels = [1] * (32 * 32)
        pixels[-1] = 16
        with self.assertRaises(ValueError):
            mod.gba_tiles_roundtrip(pixels, 32)


if __name__ == "__main__":
    unittest.main()
