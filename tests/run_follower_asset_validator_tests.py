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


def png(side, corruption=False, transparency=None, filter_type=0):
    width = side * 6
    height = side
    ihdr = struct.pack(">IIBBBBB", width, height, 4, 3, 0, 0, 0)
    # 16 separate palette entries; index zero intentionally not special-cased.
    palette = b"".join(bytes(((i * 13) % 256, (i * 17) % 256, (i * 19) % 256)) for i in range(16))
    scanlines = bytearray()
    for y in range(height):
        scanlines.append(filter_type)
        values = [((x // side) + (x % side) // 8 + y // 8) % 15 + 1 for x in range(width)]
        row = bytearray((values[j] << 4) | values[j + 1] for j in range(0, width, 2))
        if filter_type == 1:
            prev = 0
            for index, item in enumerate(row):
                row[index] = (item - prev) & 255
                prev = item
        elif filter_type != 0:
            raise ValueError("synthetic fixture supports filters 0 and 1")
        scanlines.extend(row)
    trns_chunk = chunk(b"tRNS", bytes(transparency)) if transparency is not None else b""
    data = b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", ihdr) + chunk(b"PLTE", palette) + trns_chunk + chunk(b"IDAT", zlib.compress(bytes(scanlines))) + chunk(b"IEND", b"")
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


    def test_png_sub_filter_roundtrip(self):
        result = mod.read_png(png(32, filter_type=1))
        self.assertEqual(result["unique_frame_cells"], 6)
        self.assertEqual(result["gba_4bpp_bytes_per_frame"], 512)

    def test_indexed_transparency_is_explicit(self):
        result = mod.read_png(png(32, transparency=[255, 255, 0]))
        self.assertEqual(result["transparent_indices"], [2])
        self.assertFalse(result["transparency_metadata_missing"])

    def test_png_without_transparency_metadata_not_certified(self):
        result = mod.read_png(png(32))
        self.assertTrue(result["transparency_metadata_missing"])

    def test_partial_alpha_is_reported(self):
        result = mod.read_png(png(32, transparency=[255, 127]))
        self.assertEqual(result["partial_alpha_indices"], [1])

    def test_overlong_transparency_table_rejected(self):
        with self.assertRaises(ValueError):
            mod.read_png(png(32, transparency=[255] * 17))

    def test_png_missing_final_chunk_is_rejected(self):
        with self.assertRaises(ValueError):
            mod.read_png(png(32)[:-12])

    def test_png_unsupported_filter_rejected(self):
        source = png(32)
        # Rebuild a valid PNG with a filter number not supported by the decoder.
        with self.assertRaises(ValueError):
            mod.read_png(png(32, filter_type=9))

    def test_tile_roundtrip_rejects_invalid_index(self):
        pixels = [1] * (32 * 32)
        pixels[-1] = 16
        with self.assertRaises(ValueError):
            mod.gba_tiles_roundtrip(pixels, 32)


if __name__ == "__main__":
    unittest.main()
