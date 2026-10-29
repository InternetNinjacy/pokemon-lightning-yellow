#ifndef GUARD_CUSTOM_POKEDEX_H
#define GUARD_CUSTOM_POKEDEX_H

#include "global.h"
#define CUSTOM_DEX_COUNT 252

// Zero means no custom entry. Save flags and entry resources remain National-indexed.
u16 CustomPokedexNumToSpecies(u16 number);
u16 SpeciesToCustomPokedexNum(u16 species);
bool8 IsSpeciesInCustomPokedex(u16 species);
u16 GetCustomPokedexCount(u8 flag);
bool16 HasAllCustomPokedexSpecies(void);
#endif
