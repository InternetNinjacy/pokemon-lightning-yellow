"""Reverse exact owner-locked 12 PKM-009 profiles for older whole-file checks.

Only changes four audited families; exact resulting rows are independently
asserted by their dedicated full-profile test modules.
"""
from approved_pkm009_eevee_water_fire_changes import prior_pkm009_eevee_water_fire_source
from approved_pkm009_vulpix_changes import prior_pkm009_vulpix_source
from approved_pkm009_ponyta_changes import prior_pkm009_ponyta_source
from approved_pkm009_oddish_changes import prior_pkm009_oddish_source

def prior_pkm009_locked_12_source(path,text):
    if path not in ('src/data/pokemon/level_up_learnsets.h',
                    'src/data/pokemon/species_info.h'):
        return text
    text=prior_pkm009_eevee_water_fire_source(path,text)
    text=prior_pkm009_vulpix_source(path,text)
    text=prior_pkm009_ponyta_source(path,text)
    if path=='src/data/pokemon/level_up_learnsets.h':
        text=prior_pkm009_oddish_source(text)
    return text
