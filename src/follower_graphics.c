#include "global.h"
#include "follower_graphics.h"
#include "sprite.h"
#include "event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"

// The sprite engine owns tile bitmap allocation. This module owns the pair
// (sprite ID, palette tag) for its active sprite and one staged successor.
// No static OBJ tile ID, NPC palette bank, or save slot is reserved.
#define FOLLOWER_GFX_TAG_0 0xFAE0
#define FOLLOWER_GFX_TAG_1 0xFAE1
#define FOLLOWER_GFX_LOCAL_ID 126

struct FollowerGraphicsSlot
{
    struct SpriteFrameImage image;
    struct SpriteTemplate template;
    const u8 *frames[6];
    u16 paletteTag;
    u8 spriteId;
    u8 paletteSlot;
    u8 size;
    bool8 owned;
};

static struct FollowerGraphicsSlot sFollowerSlots[2];
static u8 sFollowerActive;
static bool8 sFollowerHasActive;
static bool8 sFollowerEventBound;
static u8 sFollowerEventId;

// The graphics-only loader has no animation callback or auto-follow movement.
// Six frame changes are explicit and may only copy during graphics-safe VBlank.
static bool8 FollowerGraphics_AssetValid(const struct FollowerGraphicsAsset *asset)
{
    u8 i;

    if (asset == NULL || asset->palette == NULL)
        return FALSE;
    if (asset->size != 32 && asset->size != 64)
        return FALSE;
    for (i = 0; i < 6; i++)
    {
        if (asset->frames[i] == NULL)
            return FALSE;
    }
    return TRUE;
}

static void FollowerGraphics_ClearSlot(u8 slotIndex)
{
    struct FollowerGraphicsSlot *slot = &sFollowerSlots[slotIndex];

    slot->spriteId = MAX_SPRITES;
    slot->paletteSlot = 0xFF;
    slot->paletteTag = 0;
    slot->size = 0;
    slot->owned = FALSE;
}

static void FollowerGraphics_DisposeSlot(u8 slotIndex)
{
    struct FollowerGraphicsSlot *slot = &sFollowerSlots[slotIndex];

    if (!slot->owned)
        return;
    if (slot->spriteId < MAX_SPRITES && gSprites[slot->spriteId].inUse)
        DestroySprite(&gSprites[slot->spriteId]);
    if (slot->paletteTag != 0 && GetSpritePaletteTagByPaletteNum(slot->paletteSlot) == slot->paletteTag)
        FreeSpritePaletteByTag(slot->paletteTag);
    FollowerGraphics_ClearSlot(slotIndex);
}

static const struct OamData *FollowerGraphics_OamForSize(u8 size)
{
    // Reuse the exact geometry already supported by FireRed, with no new OAM
    // allocation strategies or duplicated source structs.
    if (size == 64)
        return GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_SEAGALLOP)->oam;
    return GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_SNORLAX)->oam;
}

enum FollowerGraphicsResult FollowerGraphics_Swap(const struct FollowerGraphicsAsset *asset,
                                                 s16 x, s16 y, u8 subpriority)
{
    struct FollowerGraphicsSlot *candidate;
    struct SpritePalette palette;
    u8 stage, id, pal;
    u8 i;
    u16 bytes;

    if (!FollowerGraphics_AssetValid(asset))
        return FOLLOWER_GFX_INVALID;

    stage = sFollowerHasActive && sFollowerActive == 0 ? 1 : 0;
    if (sFollowerSlots[stage].owned)
        return FOLLOWER_GFX_INVALID; // Never discard another staged owner.
    candidate = &sFollowerSlots[stage];
    FollowerGraphics_ClearSlot(stage);
    candidate->paletteTag = stage == 0 ? FOLLOWER_GFX_TAG_0 : FOLLOWER_GFX_TAG_1;
    // Avoid hijacking a palette which might have been reserved by another
    // subsystem using one of these tags. No old tile/palette touched on fail.
    if (IndexOfSpritePaletteTag(candidate->paletteTag) != 0xFF)
    {
        FollowerGraphics_ClearSlot(stage);
        return FOLLOWER_GFX_NO_PALETTE;
    }

    palette.data = asset->palette;
    palette.tag = candidate->paletteTag;
    pal = LoadSpritePalette(&palette);
    if (pal == 0xFF)
    {
        FollowerGraphics_ClearSlot(stage);
        return FOLLOWER_GFX_NO_PALETTE;
    }

    candidate->paletteSlot = pal;
    candidate->owned = TRUE;
    candidate->size = asset->size;
    bytes = asset->size == 64 ? 2048 : 512;
    for (i = 0; i < 6; i++)
        candidate->frames[i] = asset->frames[i];

    candidate->image.data = asset->frames[0];
    candidate->image.size = bytes;
    candidate->template.tileTag = TAG_NONE;
    candidate->template.paletteTag = TAG_NONE;
    candidate->template.oam = FollowerGraphics_OamForSize(asset->size);
    candidate->template.anims = gDummySpriteAnimTable;
    candidate->template.images = &candidate->image;
    candidate->template.affineAnims = gDummySpriteAffineAnimTable;
    candidate->template.callback = SpriteCallbackDummy;

    // CreateSprite allocates 16 or 64 native 4bpp tiles. It returns
    // MAX_SPRITES for either sprite-slot or contiguous tile exhaustion.
    id = CreateSprite(&candidate->template, x, y, subpriority);
    if (id == MAX_SPRITES)
    {
        // Palette was acquired by this stage; release only that reservation.
        FreeSpritePaletteByTag(candidate->paletteTag);
        FollowerGraphics_ClearSlot(stage);
        return FOLLOWER_GFX_NO_SPRITE;
    }

    candidate->spriteId = id;
    gSprites[id].oam.paletteNum = pal;
    gSprites[id].invisible = TRUE;

    // Must be called in graphics-safe VBlank. Direct copying is intentional:
    // queued copies can silently overflow the sprite-copy queue and would
    // violate failure atomicity. All destination tiles are already owned.
    CpuCopy16(asset->frames[0],
              (void *)((u8 *)OBJ_VRAM0 + gSprites[id].oam.tileNum * TILE_SIZE_4BPP),
              bytes);

    // Update an already-owned object-event record only AFTER a complete
    // replacement exists. If CreateSprite/LoadSpritePalette failed above,
    // the object event still points at the original sprite with no mutation.
    if (sFollowerEventBound)
    {
        struct ObjectEvent *event = &gObjectEvents[sFollowerEventId];

        if (!event->active || event->localId != FOLLOWER_GFX_LOCAL_ID)
        {
            DestroySprite(&gSprites[id]);
            FreeSpritePaletteByTag(candidate->paletteTag);
            FollowerGraphics_ClearSlot(stage);
            return FOLLOWER_GFX_INVALID;
        }
        event->spriteId = id;
        event->graphicsId = asset->size == 64
                          ? OBJ_EVENT_GFX_SEAGALLOP : OBJ_EVENT_GFX_SNORLAX;
    }

    // Commit once all resources are allocated and staged. Both sprites may
    // coexist during staging; predecessor is untouched on failed creation.
    // A sprite with no owned object-event slot must never appear as an
    // untracked ghost in an overcrowded map. BindObjectEvent reveals it.
    gSprites[id].invisible = !sFollowerEventBound;
    if (sFollowerHasActive)
        FollowerGraphics_DisposeSlot(sFollowerActive);
    sFollowerActive = stage;
    sFollowerHasActive = TRUE;
    return FOLLOWER_GFX_OK;
}

bool8 FollowerGraphics_SetFrame(u8 frame)
{
    struct FollowerGraphicsSlot *slot;
    u16 bytes;

    if (frame >= 6 || !sFollowerHasActive)
        return FALSE;
    slot = &sFollowerSlots[sFollowerActive];
    if (!slot->owned || slot->spriteId >= MAX_SPRITES
     || !gSprites[slot->spriteId].inUse)
        return FALSE;

    bytes = slot->size == 64 ? 2048 : 512;
    CpuCopy16(slot->frames[frame],
              (void *)((u8 *)OBJ_VRAM0 + gSprites[slot->spriteId].oam.tileNum * TILE_SIZE_4BPP),
              bytes);
    return TRUE;
}

void FollowerGraphics_Release(void)
{
    // Revoke the owned event slot before any sprite disposal. Never call
    // RemoveObjectEvent() here: it assumes species/static descriptor sizes
    // when freeing sprite tiles and may release the wrong tile count.
    if (sFollowerEventBound)
    {
        struct ObjectEvent *event = &gObjectEvents[sFollowerEventId];
        if (event->active && event->localId == FOLLOWER_GFX_LOCAL_ID)
        {
            event->active = FALSE;
            event->spriteId = MAX_SPRITES;
        }
        sFollowerEventBound = FALSE;
    }
    if (sFollowerHasActive)
        FollowerGraphics_DisposeSlot(sFollowerActive);
    sFollowerHasActive = FALSE;
}

void FollowerGraphics_DetachForMapTransition(void)
{
    // Call before the map's normal sprite/palette reset. The selected party
    // instance, if any, belongs to a separate feature state layer.
    FollowerGraphics_Release();
}

void FollowerGraphics_InvalidateAfterSpriteReset(void)
{
    // Only call AFTER ResetSpriteData; old IDs/palette tags no longer exist.
    // Never free old ID from a newly loaded map.
    FollowerGraphics_ClearSlot(0);
    FollowerGraphics_ClearSlot(1);
    sFollowerHasActive = FALSE;
    sFollowerEventBound = FALSE;
}

u8 FollowerGraphics_GetObjectEventId(void)
{
    if (!sFollowerEventBound)
        return OBJECT_EVENTS_COUNT;
    return sFollowerEventId;
}

bool8 FollowerGraphics_BindObjectEvent(u8 mapGroup, u8 mapNum,
                                        s16 mapX, s16 mapY, u8 elevation)
{
    u8 i;
    struct ObjectEvent *event;

    if (!sFollowerHasActive)
        return FALSE;

    // One owned graphics object per map. Do not overwrite an NPC or a
    // preexisting special event with the same reserved local identifier.
    if (!sFollowerEventBound)
    {
        for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
        {
            if (gObjectEvents[i].active && gObjectEvents[i].localId == FOLLOWER_GFX_LOCAL_ID
              && gObjectEvents[i].mapGroup == mapGroup && gObjectEvents[i].mapNum == mapNum)
            {
                FollowerGraphics_Release();
                return FALSE;
            }
        }
        i = GetFirstInactiveObjectEventId();
        if (i >= OBJECT_EVENTS_COUNT)
        {
            FollowerGraphics_Release();
            return FALSE;
        }
        sFollowerEventId = i;
        event = &gObjectEvents[i];
        memset(event, 0, sizeof(*event));
        event->localId = FOLLOWER_GFX_LOCAL_ID;
        event->mapGroup = mapGroup;
        event->mapNum = mapNum;
        event->movementType = MOVEMENT_TYPE_FACE_DOWN;
        event->facingDirection = DIR_SOUTH;
        event->movementDirection = DIR_SOUTH;
        event->active = TRUE;
        sFollowerEventBound = TRUE;
    }

    event = &gObjectEvents[sFollowerEventId];
    event->spriteId = sFollowerSlots[sFollowerActive].spriteId;
    event->graphicsId = sFollowerSlots[sFollowerActive].size == 64
                      ? OBJ_EVENT_GFX_SEAGALLOP : OBJ_EVENT_GFX_SNORLAX;
    event->initialCoords.x = mapX;
    event->initialCoords.y = mapY;
    event->currentCoords = event->initialCoords;
    event->previousCoords = event->initialCoords;
    event->currentElevation = elevation;
    event->previousElevation = elevation;
    // Reveal only after a real graphics object-event slot is owned.
    gSprites[sFollowerSlots[sFollowerActive].spriteId].invisible = FALSE;
    return TRUE;
}

u8 FollowerGraphics_GetSpriteId(void)
{
    if (!sFollowerHasActive)
        return MAX_SPRITES;
    return sFollowerSlots[sFollowerActive].spriteId;
}

u8 FollowerGraphics_GetPaletteSlot(void)
{
    if (!sFollowerHasActive)
        return 0xFF;
    return sFollowerSlots[sFollowerActive].paletteSlot;
}

u8 FollowerGraphics_GetSize(void)
{
    if (!sFollowerHasActive)
        return 0;
    return sFollowerSlots[sFollowerActive].size;
}
