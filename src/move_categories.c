#include "global.h"
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
