#include "global.h"
#include "wild_double_battle.h"
#include "battle.h"
#include "battle_anim.h"
#include "util.h"
#include "pokemon.h"
#include "random.h"
#include "safari_zone.h"
#include "constants/maps.h"

static const struct WildDoublePair sTestPairs[] =
{
    {{SPECIES_ONIX, SPECIES_CLOYSTER}, 5},
};

// Ordinary grass and cave floor; no tiles, graphics, map headers or wild tables change.
static const struct WildDoubleMap sWildDoubleMaps[] =
{
    {MAP_GROUP(MAP_ROUTE1), MAP_NUM(MAP_ROUTE1), ARRAY_COUNT(sTestPairs), sTestPairs},
    {MAP_GROUP(MAP_MT_MOON_1F), MAP_NUM(MAP_MT_MOON_1F), ARRAY_COUNT(sTestPairs), sTestPairs},
};

const struct WildDoubleMap *GetWildDoubleMap(void)
{
    u32 i;
    if (!LIGHTNING_WILD_DOUBLES_TEST_MAPS)
        return NULL;
    for (i = 0; i < ARRAY_COUNT(sWildDoubleMaps); i++)
        if (sWildDoubleMaps[i].mapGroup == gSaveBlock1Ptr->location.mapGroup
         && sWildDoubleMaps[i].mapNum == gSaveBlock1Ptr->location.mapNum)
            return &sWildDoubleMaps[i];
    return NULL;
}

bool8 TryDoDoubleWildBattle(void)
{
    return GetWildDoubleMap() != NULL && !GetSafariZoneFlag()
        && GetMonsStateToDoubles() == PLAYER_HAS_TWO_USABLE_MONS;
}

s16 ChooseWildDoubleSpecialPair(const struct WildDoubleMap *map)
{
    u16 roll;
    u16 total = 0;
    u32 i;
    if (map == NULL)
        return -1;
    // Rejection sampling avoids modulo bias for the explicitly specified percentages.
    do { roll = Random(); } while (roll >= 65500);
    roll %= 100;
    for (i = 0; i < map->pairCount; i++)
    {
        total += map->pairs[i].percent;
        if (roll < total)
            return i;
    }
    return -1; // remaining probability uses two ordinary table selections
}

bool8 IsWildDoubleBattle(void)
{
    return (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        && !(gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_LINK));
}

u8 GetWildOpponentCount(void)
{
    u32 i;
    u8 count = 0;
    for (i = 0; i < gBattlersCount; i++)
        if (GetBattlerSide(i) == B_SIDE_OPPONENT && gBattleMons[i].hp
         && !(gAbsentBattlerFlags & gBitTable[i]))
            count++;
    return count;
}

u8 GetWildCatchTarget(void)
{
    u8 left = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
    if (IsWildDoubleBattle() && (!gBattleMons[left].hp || (gAbsentBattlerFlags & gBitTable[left])))
        return GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
    return left;
}

u8 GetWildEscapeOpponent(void)
{
    u8 target = GetWildCatchTarget();
    u8 right = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
    if (IsWildDoubleBattle() && gBattleMons[right].hp && !(gAbsentBattlerFlags & gBitTable[right])
     && gBattleMons[right].speed > gBattleMons[target].speed)
        target = right;
    return target;
}
