"""Recreate the pinned pre-typing species file for legacy frozen source hashes.

Every projected change is checked against approved source records/type/ability
snapshots. Other files retain their existing independent historical projectors.
"""
from pathlib import Path
from approved_six_species_profile_projection import prior_six_species_profile_source
from approved_weather_species_slots import prior_weather_species_source
from approved_typing_text_projection import prior_approved_typing_text
from approved_sevii_water_second_changes import restore_prior_sevii_water_second_text

SPECIES_PATH = "src/data/pokemon/species_info.h"


def prior_pre_typing_species_bytes():
    text = prior_six_species_profile_source(Path(SPECIES_PATH).read_text())
    text = prior_weather_species_source(text)
    text = restore_prior_sevii_water_second_text(SPECIES_PATH, text)
    text = prior_approved_typing_text(text)
    return text.encode()
