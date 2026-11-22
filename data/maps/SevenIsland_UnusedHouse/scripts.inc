SevenIsland_UnusedHouse_MapScripts::
	.byte 0

SevenIsland_UnusedHouse_EventScript_BattleTowerReceptionist::
	lock
	faceplayer
	msgbox SevenIsland_UnusedHouse_Text_BattleTowerReceptionist
	msgbox SevenIsland_UnusedHouse_Text_ReturnToSevenIsland, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SevenIsland_UnusedHouse_EventScript_Stay
	closemessage
	warp MAP_SEVEN_ISLAND, 15, 6
	waitstate
	release
	end

SevenIsland_UnusedHouse_EventScript_Stay::
	release
	end
