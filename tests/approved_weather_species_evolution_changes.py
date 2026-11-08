"""Reverse the narrowly approved two-family ability fallback for frozen source audits."""
from pathlib import Path

BEFORE = """u8 GetAbilityBySpecies(u16 species, bool8 abilityNum)
{
    if (abilityNum)"""
AFTER = """u8 GetAbilityBySpecies(u16 species, bool8 abilityNum)
{
    // These evolved species have only one approved ability. A formerly
    // second-slot Magikarp/Feebas must not become ABILITY_NONE or Drizzle.
    if (abilityNum && (species == SPECIES_GYARADOS || species == SPECIES_MILOTIC))
        abilityNum = FALSE;
    if (abilityNum)"""


def prior_weather_species_evolution_source(path, data=None):
    if data is None:
        data = Path(path).read_text()
    if str(path) == "src/pokemon.c":
        assert data.count(AFTER) == 1, "Weather evolution fallback changed unexpectedly"
        return data.replace(AFTER, BEFORE, 1)
    return data
