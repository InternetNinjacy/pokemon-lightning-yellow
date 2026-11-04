#ifndef GUARD_EVOLUTION_ITEMS_H
#define GUARD_EVOLUTION_ITEMS_H

#include "global.h"
struct Pokemon;
void BeginItemEvolution(struct Pokemon *mon, u16 targetSpecies, u8 partyId, u16 item, void (*callback)(void));

#endif
