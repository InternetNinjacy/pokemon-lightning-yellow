#include "global.h"
#include "event_data.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "constants/species.h"

// Default names apply only to the four Cinnabar fossil handoffs.
// Native givemon and the optional player nickname prompts remain in charge.
void ScriptSetFossilDefaultNickname(void)
{
    static const u8 sOmanyteName[] = _("SPIRAL");
    static const u8 sKabutoName[] = _("SNIPS");
    static const u8 sAnorithName[] = _("LEGS");
    static const u8 sAerodactylName[] = _("KEVIN");
    const u8 *nickname;
    struct Pokemon *mon;
    u16 species = VarGet(VAR_TEMP_1);

    switch (species)
    {
    case SPECIES_OMANYTE:
        nickname = sOmanyteName;
        break;
    case SPECIES_KABUTO:
        nickname = sKabutoName;
        break;
    case SPECIES_ANORITH:
        nickname = sAnorithName;
        break;
    case SPECIES_AERODACTYL:
        nickname = sAerodactylName;
        break;
    default:
        return;
    }

    // Do not alter VAR_RESULT: the existing script still branches on
    // party delivery, PC delivery, or full-storage refusal.
    if (gSpecialVar_Result == MON_GIVEN_TO_PARTY)
    {
        if (gPlayerPartyCount == 0)
            return;
        mon = &gPlayerParty[gPlayerPartyCount - 1];
        if (GetMonData(mon, MON_DATA_SPECIES) == species
         && GetMonData(mon, MON_DATA_LEVEL) == 5)
            SetMonData(mon, MON_DATA_NICKNAME, nickname);
    }
    else if (gSpecialVar_Result == MON_GIVEN_TO_PC)
    {
        if (gSpecialVar_MonBoxId < TOTAL_BOXES_COUNT
         && gSpecialVar_MonBoxPos < IN_BOX_COUNT
         && GetBoxMonDataAt(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos, MON_DATA_SPECIES) == species
         && GetBoxMonDataAt(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos, MON_DATA_LEVEL) == 5)
            SetBoxMonNickAt(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos, nickname);
    }
}
