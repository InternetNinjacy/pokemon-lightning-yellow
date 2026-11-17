ViridianCity_House_MapScripts::
	.byte 0

ViridianCity_House_EventScript_BaldingMan::
	lock
	faceplayer
	goto_if_set FLAG_GOT_OLD_ROD, ViridianCity_House_EventScript_BaldingManNickname
	msgbox ViridianCity_House_Text_OldRodFromViridian
	checkitemspace ITEM_OLD_ROD
	goto_if_eq VAR_RESULT, FALSE, ViridianCity_House_EventScript_NoRoomForOldRod
	additem ITEM_OLD_ROD
	goto_if_eq VAR_RESULT, FALSE, ViridianCity_House_EventScript_NoRoomForOldRod
	setflag FLAG_GOT_OLD_ROD
	msgreceiveditem ViridianCity_House_Text_ReceivedOldRod, ITEM_OLD_ROD
ViridianCity_House_EventScript_BaldingManNickname::
	msgbox ViridianCity_House_Text_NicknamingIsFun
	release
	end

ViridianCity_House_EventScript_NoRoomForOldRod::
	msgbox ViridianCity_House_Text_NoRoomForOldRod
	release
	end

ViridianCity_House_EventScript_LittleGirl::
	msgbox ViridianCity_House_Text_MyDaddyLovesMonsToo, MSGBOX_NPC
	end

ViridianCity_House_EventScript_Speary::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SPEAROW, CRY_MODE_NORMAL
	msgbox ViridianCity_House_Text_Speary
	waitmoncry
	release
	end

ViridianCity_House_EventScript_NicknameSign::
	msgbox ViridianCity_House_Text_SpearowNameSpeary, MSGBOX_SIGN
	end
