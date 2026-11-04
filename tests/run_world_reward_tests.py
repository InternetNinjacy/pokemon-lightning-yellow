#!/usr/bin/env python3
"""Static scope/preservation checks, separate from actual ROM fossil tests."""
from pathlib import Path
import hashlib
import json
import re
import subprocess
from approved_blaine_changes import prior_blaine_source
from approved_museum_changes import prior_museum_source

data = json.loads(Path('tests/specifications/world_reward_preserved.json').read_text())
for path, digest in data['files'].items():
    assert hashlib.sha256(prior_blaine_source(path, prior_museum_source(path)).encode()).hexdigest() == digest, path
lab = Path('data/maps/CinnabarIsland_PokemonLab_ExperimentRoom/scripts.inc').read_text()
for label, digest in data['lab_blocks'].items():
    m = re.search('^' + re.escape(label) + r'::\n(.*?)(?=^\w+::|\Z)', lab, re.S | re.M)
    assert m and hashlib.sha256(m[1].strip().encode()).hexdigest() == digest, label
roster = set(re.findall(r'CUSTOM_DEX_ENTRY\(\d+,\s*(\w+)\)', Path('src/data/pokemon/custom_pokedex.h').read_text()))
assert len(roster) == 252
assert set(['OMANYTE', 'KABUTO', 'AERODACTYL', 'ANORITH', 'DONPHAN', 'GYARADOS', 'SANDSLASH', 'FLYGON']) <= roster
flags = Path('include/constants/flags.h').read_text()
for name, value in [('FLAG_GOT_UNCHOSEN_MT_MOON_FOSSIL', '0x2E5'),
                    ('FLAG_REVIVED_CLAW', '0x2E6'),
                    ('FLAG_GOT_CLAW_FOSSIL_FROM_OAKS_AIDE', '0x2FA')]:
    assert re.search(r'^#define\s+' + name + r'\s+' + value + r'\s*$', flags, re.M), name
assert re.search(r'^#define FLAG_GOT_EVERSTONE_FROM_OAKS_AIDE\s+FLAG_GOT_CLAW_FOSSIL_FROM_OAKS_AIDE$', flags, re.M)
assert re.search(r'^#define MULTICHOICE_FOSSIL_REVIVAL\s+65$', Path('include/constants/menu.h').read_text(), re.M)
print(f"PASS: static preservation of {len(data['files'])} files and {len(data['lab_blocks'])} original lab service blocks; save layout, daycare, trainer/wild data, MtMoon choice, other aides and NPC objects unchanged")
print('PASS: fossil outputs and referenced team species remain within the unchanged 252 roster')
print('PASS: static reserved event-bit allocation, historical reward-bit alias and appended menu ID; actual reward/save/menu behavior requires mGBA')
subprocess.run(['python3', 'scripts/audit_species_assignments.py', '--check'], check=True)
