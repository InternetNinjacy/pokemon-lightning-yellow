// Hardware-free support for executing extracted production damage routines.
#include <assert.h>
#include <stdio.h>
#include "global.h"
#include "battle.h"
#include "battle_util.h"
#include "item.h"
#include "event_data.h"
#include "constants/abilities.h"
#include "constants/items.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_ai.h"
#include "move_categories.h"
#include "src/data/battle_moves.h"

u16 gBattleMovePower, gCurrentMove, gBattleWeather, gDynamicBasePower;
u32 gBattleTypeFlags, gStatuses3[MAX_BATTLERS_COUNT];
s32 gBattleMoveDamage;
u8 gCritMultiplier, gBattlerAttacker, gBattlerTarget;
u16 gSideStatuses[2];
struct BattlePokemon gBattleMons[MAX_BATTLERS_COUNT];
struct BattleEnigmaBerry gEnigmaBerries[MAX_BATTLERS_COUNT];
struct ProtectStruct gProtectStructs[MAX_BATTLERS_COUNT];
struct BattleScripting gBattleScripting;
struct BattleStruct battleStruct;
struct BattleStruct *gBattleStruct = &battleStruct;
static struct ResourceFlags resourceFlags;
static struct BattleResources resources;
struct BattleResources *gBattleResources = &resources;
static u8 alive = 1, fieldAbility;
static u16 randomValue;
const u8 *gBattlescriptCurrInstr;
u8 GetBattlerPosition(u8 id) { return id; }
u8 GetBattlerSide(u8 id) { return id & 1; }
bool8 FlagGet(u16 flag) { return FALSE; }
u8 CountAliveMonsInBattle(u8 id) { return alive; }
u8 AbilityBattleEffects(u8 c, u8 b, u8 ability, u8 special, u16 move)
{ return ability && ability == fieldAbility; }
u16 Random(void) { return randomValue; }
// Tests encode held effects in the item field so every production branch can
// be exercised without pulling in the item-use engine. This is a test stub.
u8 ItemId_GetHoldEffect(u16 item) { return item; }
u8 ItemId_GetHoldEffectParam(u16 item) { return 10; }
struct SideTimer gSideTimers[2];
struct SpecialStatus gSpecialStatuses[MAX_BATTLERS_COUNT];
static struct AI_ThinkingStruct thinking;
static struct BattleHistory history;
static const u8 *sAIScriptPtr;
#define AI_THINKING_STRUCT (gBattleResources->ai)
#define BATTLE_HISTORY (gBattleResources->battleHistory)

#include "battle_controllers.h"
u32 gHitMarker, gBattleControllerExecFlags;
u8 gActiveBattler, gMoveResultFlags, gTakenDmgByBattler[MAX_BATTLERS_COUNT];
s32 gTakenDmg[MAX_BATTLERS_COUNT], gHpDealt;
struct DisableStruct gDisableStructs[MAX_BATTLERS_COUNT];
const u8 BattleScript_SubstituteFade[] = {0};
u8 GetBattlerForBattleScript(u8 id) { return id == BS_TARGET ? gBattlerTarget : gBattlerAttacker; }
void BattleScriptPushCursor(void) {}
void BtlController_EmitSetMonData(u8 b, u8 r, u8 m, u8 n, void *d) {}
void MarkBattlerForControllerExec(u8 id) {}
