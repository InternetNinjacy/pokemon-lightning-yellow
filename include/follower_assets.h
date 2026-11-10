#ifndef GUARD_FOLLOWER_ASSETS_H
#define GUARD_FOLLOWER_ASSETS_H
#include "global.h"
#include "follower_graphics.h"

// Artwork registry is generated from the exact 252-species inventory.
// It has no runtime dependence on party order or National-Dex index.
// Castform form=0 is ordinary field appearance; 1/2/3 are optional
// sunny/rainy/snowy visual lookups, never new field-weather mechanics.
struct FollowerArtEntry
{
    u16 species;
    u8 form;
    u8 size;
    const u8 *frames;   // six sequential native 4bpp frames
    const u16 *normalPalette;
    const u16 *shinyPalette;
};

bool8 FollowerAssets_Resolve(u16 species, u8 form, bool8 shiny,
                             struct FollowerGraphicsAsset *out);
u16 FollowerAssets_Count(void);
#endif
