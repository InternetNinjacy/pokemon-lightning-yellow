#include "global.h"
#include "follower_controller.h"
#include "follower_graphics.h"
#include "follower_state.h"
#include "field_player_avatar.h"
#include "event_object_movement.h"
#include "sprite.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"

// This field controller operates only on the existing graphics-owned slot.
// It deliberately does NOT allocate a sprite or invent a species asset.
// The all-252 graphics resolver must activate FollowerGraphics separately,
// at a safe graphics upload phase, before this controller can display it.
static bool8 sHasTrailAnchor;
static struct Coords16 sLastPlayerTile;
static struct Coords16 sTrailTile;
static u8 sLastMapGroup;
static u8 sLastMapNum;
static u8 sLastPlayerObjectId;

static void FollowerController_ResetTrail(void)
{
    sHasTrailAnchor = FALSE;
    sLastPlayerObjectId = OBJECT_EVENTS_COUNT;
}

// The all-252 asset resolver will call this with one of its pinned/converted
// descriptors at a graphics-safe phase. Failures never alter party selection.
bool8 FollowerController_BindAsset(const struct FollowerGraphicsAsset *asset)
{
    u8 playerId = gPlayerAvatar.objectEventId;
    struct ObjectEvent *player;
    struct Sprite *playerSprite;
    enum FollowerGraphicsResult result;

    if (!FollowerState_CanDisplay() || asset == NULL
     || playerId >= OBJECT_EVENTS_COUNT
     || !gObjectEvents[playerId].active)
        return FALSE;
    player = &gObjectEvents[playerId];
    if (player->spriteId >= MAX_SPRITES || !gSprites[player->spriteId].inUse)
        return FALSE;
    playerSprite = &gSprites[player->spriteId];

    // The existing loader owns both the new and old tile/palette allocations
    // until Swap commits. This call may write OBJ VRAM: the caller MUST already
    // be in a graphics-safe VBlank phase.
    result = FollowerGraphics_Swap(asset,
                                   playerSprite->x,
                                   playerSprite->y + 16,
                                   playerSprite->subpriority + 1);
    if (result != FOLLOWER_GFX_OK)
        return FALSE;
    if (!FollowerGraphics_BindObjectEvent(gSaveBlock1Ptr->location.mapGroup,
                                          gSaveBlock1Ptr->location.mapNum,
                                          player->previousCoords.x,
                                          player->previousCoords.y,
                                          player->previousElevation))
    {
        FollowerGraphics_Release();
        return FALSE;
    }
    FollowerController_ResetTrail();
    return TRUE;
}

void FollowerController_Tick(void)
{
    struct ObjectEvent *player, *follower;
    struct Sprite *playerSprite, *followerSprite;
    u8 spriteId = FollowerGraphics_GetSpriteId();
    u8 eventId = FollowerGraphics_GetObjectEventId();
    u8 playerId = gPlayerAvatar.objectEventId;
    s8 selectedSlot;

    // The selection is independent of visibility or sprite allocation.
    selectedSlot = FollowerState_ResolvePartySlot();
    if (selectedSlot < 0)
    {
        if (spriteId < MAX_SPRITES)
            FollowerGraphics_Release();
        FollowerController_ResetTrail();
        return;
    }

    if (spriteId >= MAX_SPRITES || eventId >= OBJECT_EVENTS_COUNT)
    {
        FollowerController_ResetTrail();
        return;
    }
    if (!gSprites[spriteId].inUse || !gObjectEvents[eventId].active
     || playerId >= OBJECT_EVENTS_COUNT || !gObjectEvents[playerId].active)
    {
        // A map reload may have invalidated handles. Never relocate an NPC.
        FollowerController_ResetTrail();
        return;
    }

    followerSprite = &gSprites[spriteId];

    // Fainting preserves selection. Bike/Surf and forced movement suppress
    // only the sprite. The resolved Pokémon will return without reselection.
    if (!FollowerState_CanDisplay()
     || TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE
                            | PLAYER_AVATAR_FLAG_ACRO_BIKE
                            | PLAYER_AVATAR_FLAG_SURFING
                            | PLAYER_AVATAR_FLAG_FORCED))
    {
        followerSprite->invisible = TRUE;
        FollowerController_ResetTrail();
        return;
    }

    player = &gObjectEvents[playerId];
    follower = &gObjectEvents[eventId];
    if (player->spriteId >= MAX_SPRITES
     || !gSprites[player->spriteId].inUse)
    {
        followerSprite->invisible = TRUE;
        FollowerController_ResetTrail();
        return;
    }
    playerSprite = &gSprites[player->spriteId];

    if (!sHasTrailAnchor
     || sLastPlayerObjectId != playerId
     || sLastMapGroup != follower->mapGroup
     || sLastMapNum != follower->mapNum)
    {
        sLastPlayerTile = player->currentCoords;
        sTrailTile = player->previousCoords;
        sLastPlayerObjectId = playerId;
        sLastMapGroup = follower->mapGroup;
        sLastMapNum = follower->mapNum;
        sHasTrailAnchor = TRUE;
    }
    else if (player->currentCoords.x != sLastPlayerTile.x
          || player->currentCoords.y != sLastPlayerTile.y)
    {
        // Remember the tile the player just left; unlike fixed screen
        // offsets, this also preserves bends in a one-tile-wide route.
        sTrailTile = sLastPlayerTile;
        sLastPlayerTile = player->currentCoords;
    }

    // The follower is a nonblocking visual object. Keep logical coordinates
    // aligned with the trail for event-facing/height consumers. Collision
    // exemption is limited to the graphics-owned event slot.
    follower->previousCoords = follower->currentCoords;
    follower->currentCoords = sTrailTile;
    follower->currentElevation = player->previousElevation;
    followerSprite->x = playerSprite->x
                           + (sTrailTile.x - player->currentCoords.x) * 16;
    followerSprite->y = playerSprite->y
                           + (sTrailTile.y - player->currentCoords.y) * 16;
    followerSprite->x2 = playerSprite->x2;
    followerSprite->y2 = playerSprite->y2;
    followerSprite->oam.priority = playerSprite->oam.priority;
    followerSprite->invisible = FALSE;
}
