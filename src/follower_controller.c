#include "global.h"
#include "follower_controller.h"
#include "follower_assets.h"
#include "follower_graphics.h"
#include "follower_state.h"
#include "field_player_avatar.h"
#include "event_object_movement.h"
#include "custom_pokedex.h"
#include "pokemon.h"
#include "sprite.h"
#include "sound.h"
#include "string_util.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"

// Logical selection and graphics ownership deliberately remain independent.
// All OBJ-VRAM frame copies and acquire-before-release allocations are driven
// by VBlank; the normal field tick only prepares validated descriptors.
static bool8 sHasTrailAnchor;
static struct Coords16 sLastPlayerTile;
static struct Coords16 sTrailTile;
static u8 sLastMapGroup;
static u8 sLastMapNum;
static u8 sLastPlayerObjectId;
static bool8 sPendingAsset;
static bool8 sBoundAsset;
static bool8 sSuppressed;
static u16 sDesiredSpecies;
static u16 sBoundSpecies;
static bool8 sDesiredShiny;
static bool8 sBoundShiny;
static u8 sDesiredForm;
static u8 sBoundForm;
static struct FollowerGraphicsAsset sPendingGraphics;
static u8 sFrameToCopy;
static u8 sLastCopiedFrame;
static bool8 sHasCopiedFrame;
static bool8 sFrameMirrored;
static u8 sWalkFrameCounter;

static void ResetTrail(void)
{
    sHasTrailAnchor = FALSE;
    sLastPlayerObjectId = OBJECT_EVENTS_COUNT;
}

void FollowerController_RequestRefresh(void)
{
    // No object/sprite destruction in a party-menu callback: its callback
    // can run after the field has torn down sprite IDs.
    sPendingAsset = FALSE;
    sBoundAsset = FALSE;
    sHasCopiedFrame = FALSE;
    ResetTrail();
}

static bool8 IsSuppressed(void)
{
    return TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE
                              | PLAYER_AVATAR_FLAG_ACRO_BIKE
                              | PLAYER_AVATAR_FLAG_SURFING
                              | PLAYER_AVATAR_FLAG_FORCED);
}

// Called from a graphics-safe phase only. Existing loader retains the old
// allocation on a failed swap; this caller leaves the selection intact.
bool8 FollowerController_BindAsset(const struct FollowerGraphicsAsset *asset)
{
    u8 playerId = gPlayerAvatar.objectEventId;
    struct ObjectEvent *player;
    struct Sprite *playerSprite;
    enum FollowerGraphicsResult result;

    if (asset == NULL || playerId >= OBJECT_EVENTS_COUNT
     || !gObjectEvents[playerId].active || sSuppressed)
        return FALSE;
    player = &gObjectEvents[playerId];
    if (player->spriteId >= MAX_SPRITES || !gSprites[player->spriteId].inUse)
        return FALSE;
    playerSprite = &gSprites[player->spriteId];
    result = FollowerGraphics_Swap(asset, playerSprite->x,
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
    ResetTrail();
    return TRUE;
}

static void PrepareAssetForSlot(s8 slot)
{
    struct Pokemon *mon;
    u16 species;
    bool8 shiny;
    u8 form = 0;
    struct FollowerGraphicsAsset asset;

    if (slot < 0)
        return;
    mon = &gPlayerParty[slot];
    species = GetMonData(mon, MON_DATA_SPECIES);
    // The approved roster is authoritative; do not load off-roster data.
    if (SpeciesToCustomPokedexNum(species) == 0)
    {
        sPendingAsset = FALSE;
        sBoundAsset = FALSE;
        return;
    }
    shiny = IsMonShiny(mon);
    // Castform's battle-only forms are not persisted on the party Pokémon;
    // base field artwork is correct outside battle. Its sunny/rainy/snowy
    // descriptors are registered for explicitly validated form resolution.
    if (sBoundAsset && species == sBoundSpecies
     && shiny == sBoundShiny && form == sBoundForm
     && FollowerGraphics_GetSpriteId() < MAX_SPRITES
     && FollowerGraphics_GetObjectEventId() < OBJECT_EVENTS_COUNT)
        return;
    if (sPendingAsset && species == sDesiredSpecies
     && shiny == sDesiredShiny && form == sDesiredForm)
        return;

    if (!FollowerAssets_Resolve(species, form, shiny, &asset))
    {
        // No imported artwork, out-of-capacity sheet or invalid resolver:
        // suppress graphics, retain the exact selected Pokémon.
        sPendingAsset = FALSE;
        sBoundAsset = FALSE;
        return;
    }
    sPendingGraphics = asset;
    sDesiredSpecies = species;
    sDesiredShiny = shiny;
    sDesiredForm = form;
    sPendingAsset = TRUE;
}

void FollowerController_Tick(void)
{
    struct ObjectEvent *player, *follower;
    struct Sprite *playerSprite, *followerSprite;
    u8 spriteId = FollowerGraphics_GetSpriteId();
    u8 eventId = FollowerGraphics_GetObjectEventId();
    u8 playerId = gPlayerAvatar.objectEventId;
    s8 slot = FollowerState_ResolvePartySlot();
    s16 dx, dy;
    u8 direction;

    if (slot < 0)
    {
        sPendingAsset = FALSE;
        sBoundAsset = FALSE;
        if (spriteId < MAX_SPRITES)
            FollowerGraphics_Release();
        ResetTrail();
        return;
    }

    sSuppressed = IsSuppressed() || !FollowerState_CanDisplay();
    if (sSuppressed)
    {
        sPendingAsset = FALSE;
        if (spriteId < MAX_SPRITES && gSprites[spriteId].inUse)
            gSprites[spriteId].invisible = TRUE;
        ResetTrail();
        return;
    }

    if (eventId < OBJECT_EVENTS_COUNT)
    {
        // Seamless map connections can change map identity without the usual
        // sprite reset. Do not let the follower remain in the old map.
        follower = &gObjectEvents[eventId];
        if (!follower->active
         || follower->mapGroup != gSaveBlock1Ptr->location.mapGroup
         || follower->mapNum != gSaveBlock1Ptr->location.mapNum)
        {
            FollowerGraphics_Release();
            sBoundAsset = FALSE;
            spriteId = MAX_SPRITES;
            eventId = OBJECT_EVENTS_COUNT;
            ResetTrail();
        }
    }
    PrepareAssetForSlot(slot);
    if (spriteId >= MAX_SPRITES || eventId >= OBJECT_EVENTS_COUNT)
    {
        sBoundAsset = FALSE;
        ResetTrail();
        return;
    }
    if (playerId >= OBJECT_EVENTS_COUNT || !gObjectEvents[playerId].active
     || !gSprites[spriteId].inUse || !gObjectEvents[eventId].active)
    {
        ResetTrail();
        return;
    }

    followerSprite = &gSprites[spriteId];
    if (!sBoundAsset || sPendingAsset)
    {
        followerSprite->invisible = TRUE; // no stale species art
        ResetTrail();
        return;
    }
    player = &gObjectEvents[playerId];
    follower = &gObjectEvents[eventId];
    if (player->spriteId >= MAX_SPRITES
     || !gSprites[player->spriteId].inUse)
    {
        followerSprite->invisible = TRUE;
        ResetTrail();
        return;
    }
    playerSprite = &gSprites[player->spriteId];

    if (!sHasTrailAnchor || sLastPlayerObjectId != playerId
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
        sTrailTile = sLastPlayerTile;
        sLastPlayerTile = player->currentCoords;
    }

    follower->previousCoords = follower->currentCoords;
    follower->currentCoords = sTrailTile;
    follower->currentElevation = player->previousElevation;
    follower->previousElevation = player->previousElevation;
    dx = sTrailTile.x - player->currentCoords.x;
    dy = sTrailTile.y - player->currentCoords.y;
    followerSprite->x = playerSprite->x + dx * 16;
    followerSprite->y = playerSprite->y + dy * 16;
    followerSprite->x2 = playerSprite->x2;
    followerSprite->y2 = playerSprite->y2;
    followerSprite->oam.priority = playerSprite->oam.priority;
    followerSprite->invisible = FALSE;

    // Follow the direction of the follower's own completed tile step, not
    // the player's current facing: after a corner those are often different.
    // While stationary, retain the existing player-facing fallback.
    // Six-cell art contract: south 0/1, north 2/3, west 4/5,
    // east mirrored west. Frame bytes copy in VBlank only.
    if (follower->currentCoords.x > follower->previousCoords.x)
        direction = DIR_EAST;
    else if (follower->currentCoords.x < follower->previousCoords.x)
        direction = DIR_WEST;
    else if (follower->currentCoords.y > follower->previousCoords.y)
        direction = DIR_SOUTH;
    else if (follower->currentCoords.y < follower->previousCoords.y)
        direction = DIR_NORTH;
    else
        direction = GetPlayerFacingDirection();
    sFrameMirrored = (direction == DIR_EAST);
    if (direction == DIR_NORTH)
        sFrameToCopy = 2;
    else if (direction == DIR_WEST || direction == DIR_EAST)
        sFrameToCopy = 4;
    else
        sFrameToCopy = 0;
    // Animate at one native walking frame per alternating tile; no large
    // per-frame DMA copy is performed from the field callback.
    if (follower->currentCoords.x != follower->previousCoords.x
     || follower->currentCoords.y != follower->previousCoords.y)
        sFrameToCopy += (++sWalkFrameCounter >> 3) & 1;
    else
        sWalkFrameCounter = 0;
}

void FollowerController_VBlank(void)
{
    u8 id;
    if (sSuppressed)
        return;
    if (sPendingAsset)
    {
        if (!FollowerController_BindAsset(&sPendingGraphics))
            return; // object slot/VRAM/palette pressure; retry later
        sPendingAsset = FALSE;
        sBoundAsset = TRUE;
        sBoundSpecies = sDesiredSpecies;
        sBoundShiny = sDesiredShiny;
        sBoundForm = sDesiredForm;
        sLastCopiedFrame = 0;
        sHasCopiedFrame = TRUE;
    }
    if (!sBoundAsset)
        return;
    id = FollowerGraphics_GetSpriteId();
    if (id >= MAX_SPRITES || !gSprites[id].inUse)
        return;
    if (sFrameToCopy < 6 && (!sHasCopiedFrame || sFrameToCopy != sLastCopiedFrame))
    {
        if (FollowerGraphics_SetFrame(sFrameToCopy))
        {
            sLastCopiedFrame = sFrameToCopy;
            sHasCopiedFrame = TRUE;
        }
    }
    gSprites[id].hFlip = sFrameMirrored;
}

// Called only by WRL-010's interaction script. Does not touch happiness,
// friendship, party position or persistent selection.
void FollowerSpecial_PlayCry(void)
{
    s8 slot = FollowerState_ResolvePartySlot();
    if (slot < 0 || !FollowerState_CanDisplay())
        return;
    GetMonData(&gPlayerParty[slot], MON_DATA_NICKNAME, gStringVar1);
    PlayCry_Normal(GetMonData(&gPlayerParty[slot], MON_DATA_SPECIES), 0);
}
