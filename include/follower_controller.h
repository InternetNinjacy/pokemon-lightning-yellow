#ifndef GUARD_FOLLOWER_CONTROLLER_H
#define GUARD_FOLLOWER_CONTROLLER_H

// Called only by the ordinary field frame loop, after sprite animation and
// camera adjustment, before OAM building. Never copies OBJ VRAM here.
void FollowerController_Tick(void);

#endif
