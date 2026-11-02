#ifndef GUARD_ROUTE5_DAYCARE_H
#define GUARD_ROUTE5_DAYCARE_H

#include "global.h"

#define ROUTE5_DAYCARE_MAGIC 0x3543594C // LYC5
#define ROUTE5_DAYCARE_VERSION 1
// Encrypted/checksummed unused ribbon nibble, only while the mon is an Egg.
#define ROUTE5_EGG_NORMAL 0xA
#define ROUTE5_EGG_SHINY  0xB

struct DayCare *GetRoute5Daycare(void); // NULL means recover, never reset occupied data
struct DayCare *GetActiveDaycare(void);
bool8 IsRoute5DaycareReady(void);
bool8 IsRoute5Daycare(const struct DayCare *daycare);
u32 Route5EggPersonality(u32 original, u32 otId, bool8 shiny);
void SetRoute5PendingEgg(struct DayCare *daycare, bool8 forceMale);

#endif
