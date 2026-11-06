#ifndef GUARD_MOVE_PP_COMPATIBILITY_H
#define GUARD_MOVE_PP_COMPATIBILITY_H

#include "global.h"

struct BoxPokemon;
struct DayCare;
bool8 ClampApprovedMovePP(struct BoxPokemon *mon);
void ClampApprovedDaycarePP(struct DayCare *daycare);
void NormalizeLoadedMovePP(void);

#endif
