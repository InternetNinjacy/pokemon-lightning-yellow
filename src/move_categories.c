#include "global.h"
#include "constants/moves.h"
#include "pokemon.h"
#include "move_categories.h"

static const u8 sPhysical[] = _("Physical");
static const u8 sSpecial[] = _("Special");
static const u8 sStatus[] = _("Status");
static const u8 sPhysicalShort[] = _("PHYS");
static const u8 sSpecialShort[] = _("SPEC");
static const u8 sStatusShort[] = _("STAT");

const u8 *GetMoveCategoryName(u16 move)
{
    switch (gBattleMoves[move].category)
    {
    case MOVE_CATEGORY_PHYSICAL: return sPhysical;
    case MOVE_CATEGORY_SPECIAL: return sSpecial;
    default: return sStatus;
    }
}

const u8 *GetMoveCategoryShortName(u16 move)
{
    switch (gBattleMoves[move].category)
    {
    case MOVE_CATEGORY_PHYSICAL: return sPhysicalShort;
    case MOVE_CATEGORY_SPECIAL: return sSpecialShort;
    default: return sStatusShort;
    }
}

static const u8 sMoveTypeShortNames[NUMBER_OF_MON_TYPES][4] = {
    [TYPE_NORMAL] = _("NRM"),
    [TYPE_FIGHTING] = _("FGT"),
    [TYPE_FLYING] = _("FLY"),
    [TYPE_POISON] = _("PSN"),
    [TYPE_GROUND] = _("GND"),
    [TYPE_ROCK] = _("RCK"),
    [TYPE_BUG] = _("BUG"),
    [TYPE_GHOST] = _("GHO"),
    [TYPE_STEEL] = _("STL"),
    [TYPE_MYSTERY] = _("???"),
    [TYPE_FIRE] = _("FIR"),
    [TYPE_WATER] = _("WTR"),
    [TYPE_GRASS] = _("GRS"),
    [TYPE_ELECTRIC] = _("ELE"),
    [TYPE_PSYCHIC] = _("PSY"),
    [TYPE_ICE] = _("ICE"),
    [TYPE_DRAGON] = _("DRG"),
    [TYPE_DARK] = _("DRK"),
};

const u8 *GetMoveTypeShortName(u16 move)
{
    u8 type = move < MOVES_COUNT ? gBattleMoves[move].type : TYPE_MYSTERY;
    if (type >= NUMBER_OF_MON_TYPES)
        type = TYPE_MYSTERY;
    return sMoveTypeShortNames[type];
}
