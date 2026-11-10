#include <assert.h>
#include <stdint.h>
typedef uint32_t u32;
#define ABILITY_SERENE_GRACE 32
#define MOVE_FLAME_FANG 355
#define MOVE_EFFECT_CERTAIN 128
#define MOVE_EFFECT_BYTE 0
#define MOVE_RESULT_NO_EFFECT 1
#define FALSE 0
static struct {unsigned ability;} gBattleMons[4];
static struct {unsigned secondaryEffectChance;} gBattleMoves[358];
static struct {unsigned multihitMoveEffect;} gBattleScripting;
static unsigned gBattlerAttacker,gCurrentMove,gBattleCommunication[8],gMoveResultFlags;
static unsigned char script[4],*gBattlescriptCurrInstr;
static unsigned roll,calls;
static unsigned Random(void){return roll;}
static void SetMoveEffect(unsigned primary,unsigned certain){(void)primary;(void)certain;calls++;}
