#include "global.h"
#include "pokemon.h"
#include "evolution_scene.h"
#include "evolution_items.h"
#include "item.h"
#include "item_use.h"
#include "main.h"
#include "constants/items.h"
#include "constants/quest_log.h"

extern const struct Evolution gEvolutionTable[][EVOS_PER_MON];

// Only one evolution scene can run at a time. Keep inventory out of target
// lookup/highlighting: consume a bag item only after the scene commits species.
static EWRAM_DATA u16 sEvolutionItem = ITEM_NONE;
static EWRAM_DATA u16 sTargetSpecies = SPECIES_NONE;
static EWRAM_DATA u8 sPartyId = 0;
static EWRAM_DATA void (*sReturnCallback)(void) = NULL;

static void CB2_FinishItemEvolution(void)
{
    void (*callback)(void) = sReturnCallback;
    u16 item = sEvolutionItem;

    // Clear pending state before inventory/event calls or the return callback.
    sEvolutionItem = ITEM_NONE;
    sReturnCallback = NULL;
    if (item != ITEM_NONE && GetMonData(&gPlayerParty[sPartyId], MON_DATA_SPECIES, NULL) == sTargetSpecies)
    {
        RemoveBagItem(item, 1);
        ItemUse_SetQuestLogEvent(QL_EVENT_USED_ITEM, &gPlayerParty[sPartyId], item, 0xFFFF);
    }
    if (callback != NULL)
        SetMainCallback2(callback);
}

void BeginItemEvolution(struct Pokemon *mon, u16 targetSpecies, u8 partyId, u16 item, void (*callback)(void))
{
    u32 i;
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    bool8 canStop = species == SPECIES_EEVEE && (item == ITEM_SUN_STONE || item == ITEM_MOON_STONE);

    for (i = 0; i < EVOS_PER_MON; i++)
        if (gEvolutionTable[species][i].method == EVO_TRADE_ITEM && gEvolutionTable[species][i].param == item)
            canStop = TRUE;

    sEvolutionItem = item;
    sTargetSpecies = targetSpecies;
    sPartyId = partyId;
    sReturnCallback = callback;
    gCB2_AfterEvolution = CB2_FinishItemEvolution;
    BeginEvolutionScene(mon, targetSpecies, canStop, partyId);
}
