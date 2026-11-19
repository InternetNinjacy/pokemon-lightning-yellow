"""Reverse only the six approved PKM-009 November 9 species profiles for frozen audits.

This projection validates the complete approved current stat tuples and the two
Shadow Tag slots *before* reversing exact historical fields. It never edits
production data or replaces the frozen historical SHA-256 assertion.
"""
from pathlib import Path
import re

# Field order follows species_info.h, not the game UI's stat display order.
FIELDS = ("baseHP", "baseAttack", "baseDefense", "baseSpeed",
          "baseSpAttack", "baseSpDefense")

# (original pre-PKM-009 fields, owner-approved current fields)
PROFILES = {
    "PONYTA": ((50, 85, 55, 90, 65, 65), (50, 100, 55, 90, 50, 65)),
    "RAPIDASH": ((65, 100, 70, 105, 80, 80), (65, 120, 70, 105, 60, 80)),
    "VULPIX": ((38, 41, 40, 65, 50, 65), (48, 31, 40, 65, 75, 65)),
    "NINETALES": ((73, 76, 75, 100, 81, 100), (83, 51, 75, 100, 121, 100)),
    "EEVEE": ((55, 55, 50, 55, 45, 65), (95, 115, 85, 95, 50, 85)),
    "FLAREON": ((65, 130, 60, 65, 95, 110), (85, 130, 85, 65, 50, 110)),
}
# Only Vulpix/Ninetales acquired a second ability in this approved source delta.
SHADOW_TAG = ("VULPIX", "NINETALES")


def prior_pkm009_species_profiles_source(text=None):
    if text is None:
        text = Path("src/data/pokemon/species_info.h").read_text()

    for species, (original, approved) in PROFILES.items():
        pattern = (r"(\[SPECIES_" + species
                   + r"\]\s*=\s*\{.*?)(?=\n    \[SPECIES_|\Z)")
        matches = list(re.finditer(pattern, text, re.S))
        assert len(matches) == 1, ("PKM-009 projection record", species)
        record = matches[0].group(0)

        for field, before, after in zip(FIELDS, original, approved):
            field_pattern = r"(\." + field + r" = )(\d+)(,)"
            value_matches = list(re.finditer(field_pattern, record))
            assert len(value_matches) == 1, ("PKM-009 projection field", species, field)
            assert int(value_matches[0].group(2)) == after, (
                "Unapproved PKM-009 current stat", species, field)
            record = re.sub(
                field_pattern, lambda m, n=before: m.group(1) + str(n) + m.group(3),
                record, count=1)

        if species in SHADOW_TAG:
            new_slot = ".abilities = {ABILITY_FLASH_FIRE, ABILITY_SHADOW_TAG}"
            old_slot = ".abilities = {ABILITY_FLASH_FIRE, ABILITY_NONE}"
            assert record.count(new_slot) == 1, ("PKM-009 Shadow Tag slot", species)
            record = record.replace(new_slot, old_slot, 1)

        assert text.count(matches[0].group(0)) == 1, ("PKM-009 unique record", species)
        text = text.replace(matches[0].group(0), record, 1)

    return text
