#include "global.h"
#include "follower_assets.h"
#include "constants/species.h"
#include <string.h>

// Generated source is deliberately text-only in version control. The build-time
// converter may replace it locally from pinned permission-cleared source sheets.
// Do not commit its generated art data or publish a ROM build artifact.
#include "follower_assets_generated.inc"

bool8 FollowerAssets_Resolve(u16 species, u8 form, bool8 shiny,
                             struct FollowerGraphicsAsset *out)
{
    const struct FollowerArtEntry *entry;
    u16 i, bytes;
    u8 frame;

    if (out == NULL)
        return FALSE;
    memset(out, 0, sizeof(*out));
    if (species == SPECIES_NONE)
        return FALSE;
    // Only Castform has explicitly approved alternate follower artwork.
    if (form != 0 && species != SPECIES_CASTFORM)
        return FALSE;
    if (form > 3)
        return FALSE;

    for (i = 0; i < sFollowerArtCount; i++)
    {
        entry = &sFollowerArtTable[i];
        if (entry->species != species || entry->form != form)
            continue;
        if ((entry->size != 32 && entry->size != 64)
         || entry->frames == NULL || entry->normalPalette == NULL
         || entry->shinyPalette == NULL)
            return FALSE;
        bytes = entry->size == 32 ? 512 : 2048;
        out->size = entry->size;
        out->palette = shiny ? entry->shinyPalette : entry->normalPalette;
        for (frame = 0; frame < 6; frame++)
            out->frames[frame] = entry->frames + frame * bytes;
        return TRUE;
    }
    return FALSE;
}

u16 FollowerAssets_Count(void)
{
    return sFollowerArtCount;
}
