#!/usr/bin/env python3
"""Check audit coverage, deliberately not a graphics/runtime acceptance test."""
import csv
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
expected = re.findall(r'CUSTOM_DEX_ENTRY\((\d+), (\w+)\)',
                      (ROOT / 'src/data/pokemon/custom_pokedex.h').read_text())
with (ROOT / 'docs/followers/252-species-coverage.csv').open() as stream:
    rows = list(csv.DictReader(stream))
assert len(rows) == len(expected) == 252
assert [(r['custom_number'], r['species']) for r in rows] == expected
assert len({r['species'] for r in rows}) == 252
assert rows[232]['species'] == 'CASTFORM'
for row in rows:
    species = row['species'].lower()
    assert row['aarant_sheet'] == f'graphics/object_events/pics/pokemon/{species}.png'
    assert row['kasenn_sheet'] == f'graphics/pokemon/{species}/overworld.png'
    for field in ('aarant_blob_sha', 'kasenn_blob_sha'):
        assert re.fullmatch('[0-9a-f]{40}', row[field])
    for field, suffix in [('kasenn_normal_palette', 'normal'), ('kasenn_shiny_palette', 'shiny')]:
        assert row[field] == f'graphics/pokemon/{species}/overworld_{suffix}.pal'
    assert row['permission_status'].startswith('BLOCKED:')
    assert row['runtime_status'] == 'NOT IMPLEMENTED / NOT EMULATOR TESTED'
    assert row['kasenn_verified_png_dimensions'] == ('384x64' if row['species'] == 'STEELIX' else '192x32')
    assert row['kasenn_png_header_sha_status'] == 'PASS pinned blob SHA and PNG IHDR 4-bit indexed'
    assert row['aarant_alternative_dimensions'] == ('192x32' if row['species'] == 'STEELIX' else '')
    assert row['technical_asset_status'].startswith('BLOCKED' if row['species'] == 'STEELIX' else 'PENDING')
    if row['local_sheet']:
        assert (ROOT / row['local_sheet']).is_file()
meta = json.loads((ROOT / 'docs/followers/source-inventory.json').read_text())
assert meta['counts']['roster'] == 252
assert meta['counts']['local_sheets'] == sum(bool(r['local_sheet']) for r in rows)
for field in ('aarant_sheets', 'kasenn_sheets', 'kasenn_normal_palettes', 'kasenn_shiny_palettes'):
    assert meta['counts'][field] == 252
print('PASS: 252 exact roster rows and independently audited PNG header/blob records; Steelix 64x64 exception preserved')
print('NOT TESTED: external frame pixels, follower implementation, build or emulator gameplay')
