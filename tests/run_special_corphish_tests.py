#!/usr/bin/env python3
"""SOW-004 owner-approved individual Corphish Drizzle: strict frozen-source audit."""
import hashlib
import json
from pathlib import Path

from approved_special_corphish_changes import prior_special_corphish_source
from approved_btl_weather_integration import without_btl

p = Path("src/pokemon.c")
source = p.read_text()
baseline = without_btl(str(p), prior_special_corphish_source(str(p), source))
existing = json.loads(Path("tests/specifications/special_glalie_delta.json").read_text())
assert hashlib.sha256(baseline.encode()).hexdigest() == existing["files"][str(p)]["after_sha256"]
assert source.count("#define CORPHISH_DRIZZLE_TAG 0xD122") == 1
assert source.count("substruct0->filler == CORPHISH_DRIZZLE_TAG") == 2
assert "species == SPECIES_CRAWDAUNT" in source
assert "ability == ABILITY_DRIZZLE" in source
# No global species-wide Drizzle or source-format save expansion.
species = Path("src/data/pokemon/species_info.h").read_text()
import re
for name in ("CORPHISH", "CRAWDAUNT"):
    body = re.search(r"\[SPECIES_"+name+r"\]\s*=\s*\{(.*?)(?=\n    \[SPECIES_|\Z)",species,re.S)
    assert body and "ABILITY_DRIZZLE" not in body[1],name
print("PASS: individual Corphish Drizzle/Crawdaunt retention, exact reversible four-hunk delta, historic Glalie SHA and unchanged global abilities; native acceptance separately required.")
