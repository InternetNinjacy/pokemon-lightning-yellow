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
static u8 sLastFollowerDirection;
static bool8 sHasFollowerDirection;

static void ResetTrail(void)
{
    sHasTrailAnchor = FALSE;
    sLastPlayerObjectId = OBJECT_EVENTS_COUNT;
    // An old direction cannot carry across a map load, follower change,
    // or a period when the selected Pokémon is not visible.
    sHasFollowerDirection = FALSE;
    sWalkFrameCounter = 0;
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
        // An unsupported species cannot use the last Pokémon's image.
        // Keep the player's choice, but free the old follower's resources.
        sPendingAsset = FALSE;
        sBoundAsset = FALSE;
        if (FollowerGraphics_GetSpriteId() < MAX_SPRITES)
            FollowerGraphics_Release();
        ResetTrail();
        return;
    }
    shiny = IsMonShiny(mon);
    // Castform's battle-only forms are not persisted on the party Pokémon;
    // base field artwork is correct outside battle. Its sunny/rainy/snowy
    // descriptors are registered for explicitly validated form resolution.
    if (sPendingAsset
     && (species != sDesiredSpecies || shiny != sDesiredShiny || form != sDesiredForm))
    {
        // The player changed the selected Pokémon before the pending graphic
        // was attached. Never display that previous choice after the switch.
        sPendingAsset = FALSE;
    }
    if (sBoundAsset && species == sBoundSpecies
     && shiny == sBoundShiny && form == sBoundForm
     && FollowerGraphics_GetSpriteId() < MAX_SPRITES
     && gSprites[FollowerGraphics_GetSpriteId()].inUse
     && FollowerGraphics_GetObjectEventId() < OBJECT_EVENTS_COUNT
     && gObjectEvents[FollowerGraphics_GetObjectEventId()].active)
        return;
    if (sPendingAsset && species == sDesiredSpecies
     && shiny == sDesiredShiny && form == sDesiredForm)
        return;

    if (!FollowerAssets_Resolve(species, form, shiny, &asset))
    {
        // No usable artwork for this selected Pokémon. Release any previous
        // follower's sprite and palette rather than leaving a hidden old
        // species occupying the very resources a later selection needs.
        // The saved party selection is intentionally not changed.
        sPendingAsset = FALSE;
        sBoundAsset = FALSE;
        if (FollowerGraphics_GetSpriteId() < MAX_SPRITES)
            FollowerGraphics_Release();
        ResetTrail();
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
    bool8 followerStepped;

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
    // Preparing artwork can release an outdated follower. Never continue
    // using sprite/event numbers captured before that release.
    spriteId = FollowerGraphics_GetSpriteId();
    eventId = FollowerGraphics_GetObjectEventId();
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
    // Count real follower tile changes, not how many field callbacks
    // happened while the player was partway through one tile.
    followerStepped = follower->currentCoords.x != follower->previousCoords.x
                   || follower->currentCoords.y != follower->previousCoords.y;
    follower->currentElevation = player->previousElevation;
    follower->previousElevation = player->previousElevation;
    dx = sTrailTile.x - player->currentCoords.x;
    dy = sTrailTile.y - player->currentCoords.y;
    followerSprite->x = playerSprite->x + dx * 16;
    followerSprite->y = playerSprite->y + dy * 16;
    followerSprite->x2 = playerSprite->x2;
    followerSprite->y2 = playerSprite->y2;
    followerSprite->oam.priority = playerSprite->oam.priority;
    // The player's depth can change while walking behind trees, terrain,
    // or bridges. Keep the follower just behind the player rather than
    // retaining its initial draw order for the entire map.
    followerSprite->subpriority = playerSprite->subpriority == 255
                               ? 255 : playerSprite->subpriority + 1;
    followerSprite->invisible = FALSE;

    // Face the direction the follower actually walked. Turning the player
    // in place must not rotate a stationary Pokémon or make it look as though
    // it took the player's next turn before moving around the corner.
    // On a fresh map/follower, use the player-facing direction once.
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
    else if (sHasFollowerDirection)
        direction = sLastFollowerDirection;
    else
        direction = GetPlayerFacingDirection();
    sLastFollowerDirection = direction;
    sHasFollowerDirection = TRUE;
    sFrameMirrored = (direction == DIR_EAST);
    if (direction == DIR_NORTH)
        sFrameToCopy = 2;
    else if (direction == DIR_WEST || direction == DIR_EAST)
        sFrameToCopy = 4;
    else
        sFrameToCopy = 0;
    // Alternate the two native walking poses when the follower actually
    // reaches its next tile. This does not depend on the number of callbacks
    // between tiles; copying the selected frame still happens in VBlank.
    if (followerStepped)
        sWalkFrameCounter ^= 1;
    sFrameToCopy += sWalkFrameCounter;
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
