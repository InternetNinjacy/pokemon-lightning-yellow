#include "global.h"
#include "decompress.h"
#include "follower_assets.h"
#include "follower_controller.h"
#include "follower_graphics.h"
#include "follower_state.h"
#include "pokemon.h"
#include "pokeball.h"
#include "quest_log.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "constants/species.h"
#include "constants/songs.h"

// Only the existing one-time opening calls these specials. Neither naming nor
// Oak's separate Eevee gift mutates follower selection.
void ScriptSelectStarterPikachuFollower(void)
{
    if (!QL_IS_PLAYBACK_STATE && gPlayerPartyCount != 0
     && GetMonData(&gPlayerParty[0], MON_DATA_SPECIES) == SPECIES_PICHU
     && FollowerState_SelectPartySlot(0))
        FollowerController_RequestRefresh();
}

#define tState     data[0]
#define tTimer     data[1]
#define tBall      data[2]
#define tOwnTiles  data[3]
#define tOwnPal    data[4]
#define tMapGroup  data[5]
#define tMapNum    data[6]

static void Task_StarterFollowerEmergence(u8 taskId);

static void EndStarterFollowerEmergence(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u8 ball = task->tBall;
    // Menus/map resets may reuse sprite IDs. Only dispose our own template and
    // callback, and free only the native ball resources acquired by this task.
    if (ball < MAX_SPRITES && gSprites[ball].inUse
     && gSprites[ball].template == &gBallSpriteTemplates[BALL_POKE]
     && gSprites[ball].callback == SpriteCallbackDummy)
    {
        FreeSpriteOamMatrix(&gSprites[ball]);
        DestroySprite(&gSprites[ball]);
    }
    if (task->tOwnTiles)
        FreeSpriteTilesByTag(gBallSpriteSheets[BALL_POKE].tag);
    if (task->tOwnPal)
        FreeSpritePaletteByTag(gBallSpritePalettes[BALL_POKE].tag);
    FollowerController_SetIntroDisplay(FALSE);
    DestroyTask(taskId);
    ScriptContext_Enable();
}

u16 ScriptStartStarterFollowerEmergence(void)
{
    u8 id;
    if (QL_IS_PLAYBACK_STATE || FollowerAssets_Count() != 255 || !FollowerState_CanDisplay()
     || FollowerState_ResolvePartySlot() != 0
     || FuncIsActiveTask(Task_StarterFollowerEmergence)
     || GetTaskCount() >= NUM_TASKS)
        return FALSE;
    id = CreateTask(Task_StarterFollowerEmergence, 80);
    gTasks[id].tBall = MAX_SPRITES;
    gTasks[id].tMapGroup = gSaveBlock1Ptr->location.mapGroup;
    gTasks[id].tMapNum = gSaveBlock1Ptr->location.mapNum;
    FollowerController_SetIntroDisplay(TRUE);
    return TRUE;
}

static void Task_StarterFollowerEmergence(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u8 follower = FollowerGraphics_GetSpriteId();
    u8 ball;

    // Resource exhaustion or interruption must not softlock the opening.
    if (++task->tTimer > 180
     || task->tMapGroup != gSaveBlock1Ptr->location.mapGroup
     || task->tMapNum != gSaveBlock1Ptr->location.mapNum
     || !FollowerState_CanDisplay())
    {
        EndStarterFollowerEmergence(taskId);
        return;
    }
    if (task->tState == 0)
    {
        // Tick prepares graphics; its VBlank binding owns the OBJ transaction.
        if (follower >= MAX_SPRITES)
            return;
        ball = AllocOamMatrix();
        if (ball == 0xFF)
        {
            EndStarterFollowerEmergence(taskId);
            return;
        }
        FreeOamMatrix(ball);
        task->tOwnTiles = GetSpriteTileStartByTag(gBallSpriteSheets[BALL_POKE].tag) == TAG_NONE;
        task->tOwnPal = IndexOfSpritePaletteTag(gBallSpritePalettes[BALL_POKE].tag) == 0xFF;
        if (task->tOwnTiles)
            LoadCompressedSpriteSheetUsingHeap(&gBallSpriteSheets[BALL_POKE]);
        if (task->tOwnPal)
            LoadCompressedSpritePaletteUsingHeap(&gBallSpritePalettes[BALL_POKE]);
        if (GetSpriteTileStartByTag(gBallSpriteSheets[BALL_POKE].tag) == TAG_NONE
         || IndexOfSpritePaletteTag(gBallSpritePalettes[BALL_POKE].tag) == 0xFF)
        {
            EndStarterFollowerEmergence(taskId);
            return;
        }
        ball = CreateSprite(&gBallSpriteTemplates[BALL_POKE],
                            gSprites[follower].x, gSprites[follower].y,
                            gSprites[follower].subpriority);
        if (ball == MAX_SPRITES)
        {
            EndStarterFollowerEmergence(taskId);
            return;
        }
        task->tBall = ball;
        gSprites[ball].callback = SpriteCallbackDummy;
        gSprites[ball].coordOffsetEnabled = gSprites[follower].coordOffsetEnabled;
        gSprites[ball].oam.priority = gSprites[follower].oam.priority;
        task->tTimer = 0;
        task->tState = 1;
        PlaySE(SE_BALL);
        return;
    }
    ball = task->tBall;
    if (ball >= MAX_SPRITES || !gSprites[ball].inUse
     || gSprites[ball].template != &gBallSpriteTemplates[BALL_POKE]
     || follower >= MAX_SPRITES)
    {
        EndStarterFollowerEmergence(taskId);
        return;
    }
    if (task->tState == 1 && task->tTimer >= 12)
    {
        // Existing GenIII ball art only. Do not invoke the battle release
        // helper: its battler affine animations do not exist on follower art.
        StartSpriteAnim(&gSprites[ball], 1);
        PlaySE(SE_BALL_OPEN);
        FollowerController_SetIntroVisible(TRUE);
        PlayCry_Normal(SPECIES_PICHU, 0);
        task->tTimer = 0;
        task->tState = 2;
    }
    else if (task->tState == 2 && task->tTimer >= 36)
        EndStarterFollowerEmergence(taskId);
}
