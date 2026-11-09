#ifndef GUARD_FOLLOWER_GRAPHICS_H
#define GUARD_FOLLOWER_GRAPHICS_H

#include "global.h"

// Graphics-only resource owner. Party assignment, script objects, movement and
// saves are deliberately owned by higher-level WRL-010 modules, not this file.
struct FollowerGraphicsAsset
{
    // Six native GBA 4bpp tile-major frames. 512 bytes each at 32x32;
    // 2048 bytes each at 64x64. Must remain valid throughout a swap.
    const u8 *frames[6];
    const u16 *palette; // 16 native BGR555 colors; normal OR Shiny.
    u8 size;            // exactly 32 or 64.
};

enum FollowerGraphicsResult
{
    FOLLOWER_GFX_OK,
    FOLLOWER_GFX_INVALID,
    FOLLOWER_GFX_NO_PALETTE,
    FOLLOWER_GFX_NO_SPRITE,
};

// Caller must invoke Swap / SetFrame during a graphics-safe VBlank phase:
// image tiles are written directly to OBJ VRAM, never to another sprite's tiles.
// The old sprite/palette remains intact if acquisition fails. No party state
// or object-event position/identity is changed by an allocation failure.
enum FollowerGraphicsResult FollowerGraphics_Swap(const struct FollowerGraphicsAsset *asset,
                                                 s16 x, s16 y, u8 subpriority);
bool8 FollowerGraphics_SetFrame(u8 frame);
void FollowerGraphics_Release(void);
void FollowerGraphics_DetachForMapTransition(void);
void FollowerGraphics_InvalidateAfterSpriteReset(void);
u8 FollowerGraphics_GetSpriteId(void);
u8 FollowerGraphics_GetPaletteSlot(void);
u8 FollowerGraphics_GetSize(void);

#endif
