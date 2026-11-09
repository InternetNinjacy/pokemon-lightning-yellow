#ifndef GUARD_FOLLOWER_CONTROLLER_H
#define GUARD_FOLLOWER_CONTROLLER_H

#include "global.h"
#include "follower_graphics.h"

// Called only by the ordinary field frame loop, after sprite animation and
// camera adjustment, before OAM building. Never copies OBJ VRAM here.
void FollowerController_Tick(void);
// Requires a caller-supplied, previously validated species/form asset.
// Must be invoked during graphics-safe VBlank; never from Tick.
bool8 FollowerController_BindAsset(const struct FollowerGraphicsAsset *asset);

#endif
