#ifndef GUARD_BATTLE_BG_H
#define GUARD_BATTLE_BG_H

#include "bg.h"

#define BATTLE_MOVE_COLUMN_WIDTH 84
#define BATTLE_MOVE_ROW_HEIGHT 16
#define BATTLE_MOVE_NAME_X 8
#define BATTLE_MOVE_NAME_PIXELS 76
#define BATTLE_MOVE_PANEL_TILES 21

extern const struct BgTemplate gBattleBgTemplates[];

void InitBattleBgsVideo(void);
void DrawBattleEntryBackground(void);
void InitLinkBattleVsScreen(u8 taskId);
void LoadBattleMenuWindowGfx(void);
void LoadBattleTextboxAndBackground(void);
void BattleInitBgsAndWindows(void);
void DrawMainBattleBackground(void);

#endif // GUARD_BATTLE_BG_H
