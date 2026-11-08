"""Exact owner-approved 2026-11-06 weather ability slot reversals for older source guards.

Restore only the 16 reviewed abilities before computing the old frozen hashes.
Do not change historical snapshots or other species fields.
"""
from pathlib import Path
import re

# species, (before slot0, before slot1), (approved slot0, approved slot1)
SLOTS = {
    "MAGIKARP": (("ABILITY_SWIFT_SWIM", "ABILITY_NONE"), ("ABILITY_SWIFT_SWIM", "ABILITY_DRIZZLE")),
    "FEEBAS": (("ABILITY_SWIFT_SWIM", "ABILITY_NONE"), ("ABILITY_SWIFT_SWIM", "ABILITY_DRIZZLE")),
    "GOLDEEN": (("ABILITY_SWIFT_SWIM", "ABILITY_WATER_VEIL"), ("ABILITY_SWIFT_SWIM", "ABILITY_DRIZZLE")),
    "SEAKING": (("ABILITY_SWIFT_SWIM", "ABILITY_WATER_VEIL"), ("ABILITY_SWIFT_SWIM", "ABILITY_DRIZZLE")),
    "POLIWAG": (("ABILITY_WATER_ABSORB", "ABILITY_DAMP"), ("ABILITY_WATER_ABSORB", "ABILITY_DRIZZLE")),
    "POLIWHIRL": (("ABILITY_WATER_ABSORB", "ABILITY_DAMP"), ("ABILITY_WATER_ABSORB", "ABILITY_DRIZZLE")),
    "POLITOED": (("ABILITY_WATER_ABSORB", "ABILITY_DAMP"), ("ABILITY_WATER_ABSORB", "ABILITY_DRIZZLE")),
    "SNORUNT": (("ABILITY_INNER_FOCUS", "ABILITY_NONE"), ("ABILITY_INNER_FOCUS", "ABILITY_ICE_BODY")),
    "GLALIE": (("ABILITY_INNER_FOCUS", "ABILITY_NONE"), ("ABILITY_INNER_FOCUS", "ABILITY_ICE_BODY")),
    "SPHEAL": (("ABILITY_THICK_FAT", "ABILITY_NONE"), ("ABILITY_THICK_FAT", "ABILITY_ICE_BODY")),
    "SEALEO": (("ABILITY_THICK_FAT", "ABILITY_NONE"), ("ABILITY_THICK_FAT", "ABILITY_ICE_BODY")),
    "WALREIN": (("ABILITY_THICK_FAT", "ABILITY_NONE"), ("ABILITY_THICK_FAT", "ABILITY_ICE_BODY")),
    "ONIX": (("ABILITY_ROCK_HEAD", "ABILITY_STURDY"), ("ABILITY_ROCK_HEAD", "ABILITY_OVERCOAT")),
    "STEELIX": (("ABILITY_ROCK_HEAD", "ABILITY_STURDY"), ("ABILITY_ROCK_HEAD", "ABILITY_OVERCOAT")),
    "PINECO": (("ABILITY_STURDY", "ABILITY_NONE"), ("ABILITY_STURDY", "ABILITY_OVERCOAT")),
    "FORRETRESS": (("ABILITY_STURDY", "ABILITY_NONE"), ("ABILITY_STURDY", "ABILITY_OVERCOAT")),
}


def prior_weather_species_source(text=None):
    if text is None:
        text = Path("src/data/pokemon/species_info.h").read_text()
    for species, (old, approved) in SLOTS.items():
        pat = r"(\[SPECIES_" + species + r"\]\s*=\s*\{.*?\.abilities = \{)(ABILITY_\w+), (ABILITY_\w+)(\})"
        found = re.search(pat, text, re.S)
        assert found and (found[2], found[3]) == approved, species
        text, count = re.subn(pat, lambda m: m[1] + ", ".join(old) + m[4], text, count=1, flags=re.S)
        assert count == 1, species
    return text
