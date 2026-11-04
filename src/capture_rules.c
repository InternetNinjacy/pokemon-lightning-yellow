#include "global.h"
#include "capture_rules.h"
#include "item.h"
#include "constants/battle.h"
#include "constants/items.h"

EWRAM_DATA u8 gCaptureOrigin = 0;

bool8 IsPokeBallItem(u16 item)
{
    return (item >= ITEM_MASTER_BALL && item <= ITEM_PREMIER_BALL) || item == ITEM_SNOW_BALL;
}

u8 BallItemToCaptureCode(u16 item)
{
    return item == ITEM_SNOW_BALL ? CAPTURE_CODE_SNOW : item;
}

u16 CaptureCodeToBallItem(u8 code)
{
    return code == CAPTURE_CODE_SNOW ? ITEM_SNOW_BALL : code;
}

u8 GetBallCatchMultiplier(u16 item, u16 species, u8 level, u8 turn, bool8 caught, u16 weather, bool8 waterOrigin, bool8 cave)
{
    u8 multiplier;
    switch (item)
    {
    case ITEM_ULTRA_BALL: return 20;
    case ITEM_GREAT_BALL:
    case ITEM_SAFARI_BALL: return 15;
    case ITEM_DIVE_BALL:
        return waterOrigin || (weather & (B_WEATHER_RAIN | B_WEATHER_HAIL)) ? 30 : 10;
    case ITEM_NET_BALL:
        return weather & (B_WEATHER_SANDSTORM | B_WEATHER_SUN) ? 30 : 10;
    case ITEM_SNOW_BALL:
        return cave || (weather & B_WEATHER_HAIL) ? 30 : 10;
    case ITEM_NEST_BALL:
        return level < 30 ? 40 - level : 10;
    case ITEM_REPEAT_BALL: return caught ? 30 : 10;
    case ITEM_TIMER_BALL:
        multiplier = turn + 10;
        return multiplier > 40 ? 40 : multiplier;
    case ITEM_PREMIER_BALL:
        switch (species)
        {
        case SPECIES_ARTICUNO: case SPECIES_ZAPDOS: case SPECIES_MOLTRES:
        case SPECIES_RAIKOU: case SPECIES_ENTEI: case SPECIES_SUICUNE:
        case SPECIES_MEWTWO: case SPECIES_MEW: return 35;
        }
        return 10;
    default: return 10;
    }
}

// Simulate both additions in the same encrypted pocket before committing either.
// Native shop transactions contain one purchased variety; quantities never pool.
bool8 TryBuyBallsWithPremierBonus(u16 item, u16 count)
{
    struct ItemSlot slots[BAG_POKEBALLS_COUNT];
    struct BagPocket *pocket = &gBagPockets[POCKET_POKE_BALLS - 1];
    struct ItemSlot *original = pocket->itemSlots;
    u16 bonus = count / 10;
    bool8 success;
    if (!IsPokeBallItem(item) || item == ITEM_MASTER_BALL || item == ITEM_SAFARI_BALL || item == ITEM_PREMIER_BALL)
        return AddBagItem(item, count);
    memcpy(slots, original, sizeof(slots));
    pocket->itemSlots = slots;
    success = AddBagItem(item, count);
    if (success && bonus)
        success = AddBagItem(ITEM_PREMIER_BALL, bonus);
    pocket->itemSlots = original;
    if (success)
        memcpy(original, slots, sizeof(slots));
    return success;
}
