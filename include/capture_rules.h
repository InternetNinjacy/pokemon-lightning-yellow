#ifndef GUARD_CAPTURE_RULES_H
#define GUARD_CAPTURE_RULES_H

#define CAPTURE_ORIGIN_SURFING 1
#define CAPTURE_ORIGIN_FISHING 2
#define CAPTURE_CODE_SNOW 13

extern u8 gCaptureOrigin;
bool8 IsPokeBallItem(u16 item);
u8 BallItemToCaptureCode(u16 item);
u16 CaptureCodeToBallItem(u8 code);
u8 GetBallCatchMultiplier(u16 item, u16 species, u8 level, u8 turn, bool8 caught, u16 weather, bool8 waterOrigin, bool8 cave);
bool8 TryBuyBallsWithPremierBonus(u16 item, u16 count);
#endif
