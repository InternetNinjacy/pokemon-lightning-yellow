#include <stddef.h>
#include "global.h"
#include "route5_daycare.h"
#include "pokemon.h"
#include "overworld.h"
#include "random.h"
#include "constants/maps.h"

STATIC_ASSERT(NUM_NATURES == 25 && SHINY_ODDS == 8, route5_shiny_policy_constants);
STATIC_ASSERT(sizeof(struct DayCare) == 0x11C, route5_daycare_size);
STATIC_ASSERT(sizeof(struct DaycareMon) == 0x8C, route5_daycare_mon_size);
STATIC_ASSERT(sizeof(struct Route5DaycareSave) == 400, route5_reserved_save_size);
STATIC_ASSERT(sizeof(struct SaveBlock1) == 0x3D68, route5_saveblock_size);
STATIC_ASSERT(offsetof(struct SaveBlock1, daycare) == 0x2F80, route5_original_daycare_offset);
STATIC_ASSERT(offsetof(struct SaveBlock1, route5Daycare) == 0x348C, route5_new_daycare_offset);
STATIC_ASSERT(offsetof(struct SaveBlock1, ramScript) == 0x361C, route5_after_reserved_offset);
STATIC_ASSERT(offsetof(struct SaveBlock1, route5DayCareMon) == 0x3C98, route5_legacy_daycare_offset);

struct DayCare *GetRoute5Daycare(void)
{
    struct Route5DaycareSave *save = &gSaveBlock1Ptr->route5Daycare;
    u32 i;
    const u8 *bytes = (const u8 *)save;
    // Never overwrite an unknown version or occupied/corrupt reserved region.
    if (save->magic == ROUTE5_DAYCARE_MAGIC)
    {
        if (save->version != ROUTE5_DAYCARE_VERSION || save->eggShiny > 1)
            return NULL;
        if (save->daycare.offspringPersonality != 0
         && ((u16)save->eggPersonality != save->daycare.offspringPersonality
          || (GET_SHINY_VALUE(save->eggOtId, save->eggPersonality) < SHINY_ODDS) != save->eggShiny))
            return NULL;
        return &save->daycare;
    }
    for (i = 0; i < sizeof(*save); i++)
        if (bytes[i] != 0)
            return NULL;
    // Copy the COMPLETE record: encrypted box data, mail and accumulated steps.
    // Keep the legacy record untouched as a forensic recovery copy. A recognized
    // version never imports it again, even after the upgraded daycare is empty.
    save->daycare.mons[0] = gSaveBlock1Ptr->route5DayCareMon;
    save->version = ROUTE5_DAYCARE_VERSION;
    save->magic = ROUTE5_DAYCARE_MAGIC; // publish only after initialization
    return &save->daycare;
}

bool8 IsRoute5DaycareReady(void)
{
    return GetRoute5Daycare() != NULL;
}

bool8 IsRoute5Daycare(const struct DayCare *daycare)
{
    return daycare == &gSaveBlock1Ptr->route5Daycare.daycare;
}

struct DayCare *GetActiveDaycare(void)
{
    if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE)
     && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE))
        return GetRoute5Daycare();
    return &gSaveBlock1Ptr->daycare;
}

// Preserve nature, low-byte gender/ability and bit15's special offspring branch.
// At most 128*8 candidates; all 25 natures are reachable for every XOR block.
u32 Route5EggPersonality(u32 original, u32 otId, bool8 shiny)
{
    u32 i, s, low, high, candidate;
    u32 nature = original % NUM_NATURES;
    if ((GET_SHINY_VALUE(otId, original) < SHINY_ODDS) == shiny)
        return original;
    if (!shiny)
    {
        // Keep low16; high16 values congruent mod25 preserve the nature.
        for (high = (original >> 16) % NUM_NATURES; high <= 0xFFFF; high += NUM_NATURES)
        {
            candidate = (high << 16) | (original & 0xFFFF);
            if (GET_SHINY_VALUE(otId, candidate) >= SHINY_ODDS)
                return candidate;
        }
    }
    else
    {
        for (i = 0; i < 128; i++)
        {
            low = (original & 0x80FF) | ((((original >> 8) + i) & 127) << 8);
            if (low == 0) // DayCare's original zero sentinel means no pending Egg
                continue;
            for (s = 0; s < SHINY_ODDS; s++)
            {
                high = (otId >> 16) ^ (otId & 0xFFFF) ^ low ^ s;
                candidate = (high << 16) | low;
                if (candidate % NUM_NATURES == nature)
                    return candidate;
            }
        }
    }
    // Unreachable with GenIII SHINY_ODDS=8/NUM_NATURES=25; tested exhaustively
    // over the reduced XOR blocks. Never loop indefinitely on game hardware.
    return original;
}

void SetRoute5PendingEgg(struct DayCare *daycare, bool8 forceMale)
{
    struct Route5DaycareSave *save = &gSaveBlock1Ptr->route5Daycare;
    u32 original, otId;
    if (!IsRoute5Daycare(daycare) || daycare->offspringPersonality != 0)
        return;
    original = forceMale ? Random() | 0x8000 : Random() % 0xFFFE + 1;
    original |= (u32)Random() << 16;
    otId = gSaveBlock2Ptr->playerTrainerId[0]
         | (gSaveBlock2Ptr->playerTrainerId[1] << 8)
         | (gSaveBlock2Ptr->playerTrainerId[2] << 16)
         | ((u32)gSaveBlock2Ptr->playerTrainerId[3] << 24);
    // One fresh fair RNG bit per generated Egg, not alternating outcomes or
    // ordinary odds on the tails branch. No pickup-time coin or frame reseed.
    save->eggShiny = Random() & 1;
    save->eggOtId = otId;
    save->eggPersonality = Route5EggPersonality(original, otId, save->eggShiny);
    daycare->offspringPersonality = save->eggPersonality;
}

// Continue restores old saved object templates, not the new map's NPC roster.
// Refresh only this changed room when a legacy template is detected.
bool8 Route5DaycareNeedsMapRefresh(void)
{
    u32 i;
    if (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE)
     || gSaveBlock1Ptr->location.mapNum != MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE))
        return FALSE;
    for (i = 0; i < gMapHeader.events->objectEventCount; i++)
        if (gSaveBlock1Ptr->objectEventTemplates[i].localId != gMapHeader.events->objectEvents[i].localId
         || gSaveBlock1Ptr->objectEventTemplates[i].graphicsId != gMapHeader.events->objectEvents[i].graphicsId)
            return TRUE;
    return FALSE;
}
