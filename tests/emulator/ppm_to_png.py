#!/usr/bin/env python3
"""Convert raw mGBA RGB PPM frame(s) into lossless PNG; stdlib only."""
from pathlib import Path
import struct
import sys
import zlib


def pack_chunk(kind: bytes, data: bytes) -> bytes:
    return struct.pack(">I", len(data)) + kind + data + struct.pack(">I", zlib.crc32(kind + data) & 0xffffffff)


def convert(path: Path) -> Path:
    data = path.read_bytes()
    header = b"P6\n240 160\n255\n"
    if not data.startswith(header) or len(data) != len(header) + 240 * 160 * 3:
        raise ValueError(f"Unexpected frame size or RGB PPM format: {path}")
    pixels = data[len(header):]
    rows = b"".join(b"\x00" + pixels[y*240*3:(y+1)*240*3] for y in range(160))
    output = b"\x89PNG\r\n\x1a\n"
    output += pack_chunk(b"IHDR", struct.pack(">IIBBBBB", 240, 160, 8, 2, 0, 0, 0))
    output += pack_chunk(b"IDAT", zlib.compress(rows, 9))
    output += pack_chunk(b"IEND", b"")
    dest = path.with_suffix(".png")
    dest.write_bytes(output)
    print(f"Rendered mGBA framebuffer: {dest.name} ({len(output)} bytes)")
    return dest


if __name__ == "__main__":
    if len(sys.argv) < 2:
        raise SystemExit("Usage: ppm_to_png.py FRAME.ppm [FRAME.ppm ...]")
    for filename in sys.argv[1:]:
        convert(Path(filename))
