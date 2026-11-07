#!/usr/bin/env python3
"""Static Lightning Yellow title asset acceptance; does not replace ROM/emulator testing."""
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
ASSETS = ROOT / "graphics/title_screen/lightning_yellow"
ORIGINAL = ROOT / "graphics/title_screen/firered"


def png_info(path):
    data = path.read_bytes()
    assert data.startswith(b"\x89PNG\r\n\x1a\n"), path
    width, height, bit_depth, color_type = struct.unpack(">IIBB", data[16:26])
    pos, palette = 8, None
    while pos < len(data):
        length = int.from_bytes(data[pos:pos + 4], "big")
        kind = data[pos + 4:pos + 8]
        if kind == b"PLTE":
            palette = data[pos + 8:pos + 8 + length]
        pos += length + 12
    assert palette is not None
    return width, height, bit_depth, color_type, palette


def check_palette(stem, expected_size):
    lines = (ASSETS / (stem + ".pal")).read_text().splitlines()
    assert lines[:3] == ["JASC-PAL", "0100", str(expected_size)]
    actual = [tuple(map(int, s.split())) for s in lines[3:]]
    assert len(actual) == expected_size
    assert all(len(c) == 3 and all(0 <= x <= 255 for x in c) for c in actual)
    return actual


def check_map(stem, tile_count, bank):
    raw = (ORIGINAL / (stem + ".bin")).read_bytes()
    assert len(raw) == 32 * 20 * 2
    words = struct.unpack("<640H", raw)
    assert max(x & 0x3ff for x in words) < tile_count
    assert all(x >> 12 == bank for x in words)


def main():
    graphics = (ROOT / "src/graphics.c").read_text()
    title = (ROOT / "src/title_screen.c").read_text()
    assert "#define TITLE_SPECIES SPECIES_PIKACHU" in title
    for stem, ext, bpp, w, h, palsize, bank in (
        ("game_title_logo", "8bpp", 8, 256, 64, 256, 0),
        ("box_art_mon", "4bpp", 4, 96, 96, 16, 13),
    ):
        path = f"graphics/title_screen/lightning_yellow/{stem}"
        assert f'{path}.{ext}.lz' in graphics, path
        assert f'{path}.gbapal' in graphics, path
        assert f'graphics/title_screen/firered/{stem}.{ext}.lz' not in graphics
        iw, ih, depth, kind, palette = png_info(ASSETS / (stem + ".png"))
        assert (iw, ih, depth, kind) == (w, h, bpp, 3)
        actual = check_palette(stem, palsize)
        for i in range(len(palette) // 3):
            assert tuple(palette[i*3:i*3+3]) == actual[i], (stem, i)
        assert len(palette) <= palsize * 3
        check_map(stem, w * h // 64, bank)
    assert 'graphics/title_screen/copyright_press_start.4bpp.lz' in graphics
    assert "CB2_InitMainMenu" in title
    print("PASS: title graphics references, PNG formats, palettes, tilemap bounds and Pikachu cry")
    print("NOT TESTED: compilation, ROM rendering, palette animation, start/menu interaction")


if __name__ == "__main__":
    main()
