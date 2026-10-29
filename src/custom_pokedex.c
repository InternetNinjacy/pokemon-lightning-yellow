#include "global.h"
#include "pokemon.h"
#include "pokedex.h"
#include "custom_pokedex.h"

static const u16 sCustomDexSpecies[CUSTOM_DEX_COUNT + 1] =
{
#define CUSTOM_DEX_ENTRY(number, name) [number] = SPECIES_##name,
#include "data/pokemon/custom_pokedex.h"
#undef CUSTOM_DEX_ENTRY
};

static const u16 sSpeciesToCustomDex[NUM_SPECIES] =
{
#define CUSTOM_DEX_ENTRY(number, name) [SPECIES_##name] = number,
#include "data/pokemon/custom_pokedex.h"
#undef CUSTOM_DEX_ENTRY
};

u16 CustomPokedexNumToSpecies(u16 number)
{
    if (number == 0 || number > CUSTOM_DEX_COUNT)
        return SPECIES_NONE;
    return sCustomDexSpecies[number];
}

u16 SpeciesToCustomPokedexNum(u16 species)
{
    if (species >= NUM_SPECIES)
        return 0;
    return sSpeciesToCustomDex[species];
}

bool8 IsSpeciesInCustomPokedex(u16 species)
{
    return SpeciesToCustomPokedexNum(species) != 0;
}

u16 GetCustomPokedexCount(u8 flag)
{
    u16 number, count = 0;
    if (flag != FLAG_GET_SEEN && flag != FLAG_GET_CAUGHT)
        return 0;
    for (number = 1; number <= CUSTOM_DEX_COUNT; number++)
        if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(sCustomDexSpecies[number]), flag))
            count++;
    return count;
}

bool16 HasAllCustomPokedexSpecies(void)
{
    return GetCustomPokedexCount(FLAG_GET_CAUGHT) == CUSTOM_DEX_COUNT;
}
