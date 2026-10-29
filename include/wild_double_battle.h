#ifndef GUARD_WILD_DOUBLE_BATTLE_H
#define GUARD_WILD_DOUBLE_BATTLE_H

#include "global.h"

// Capability-test maps, not finalized encounter placements. Set to 0 to disable.
#define LIGHTNING_WILD_DOUBLES_TEST_MAPS 1

struct WildDoublePair
{
    u16 species[2];
    u8 percent;
};

struct WildDoubleMap
{
    u8 mapGroup;
    u8 mapNum;
    u8 pairCount;
    const struct WildDoublePair *pairs;
};

const struct WildDoubleMap *GetWildDoubleMap(void);
bool8 TryDoDoubleWildBattle(void);
s16 ChooseWildDoubleSpecialPair(const struct WildDoubleMap *map);
bool8 IsWildDoubleBattle(void);
u8 GetWildOpponentCount(void);
u8 GetWildCatchTarget(void);
u8 GetWildEscapeOpponent(void);

#endif
