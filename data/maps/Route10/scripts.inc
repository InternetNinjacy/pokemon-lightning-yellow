Route10_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route10_Lightning_OnTransition
	.byte 0

Route10_EventScript_Unused::
	end

Route10_EventScript_NorthRockTunnelSign::
	msgbox Route10_Text_RockTunnelDetourToLavender, MSGBOX_SIGN
	end

Route10_EventScript_SouthRockTunnelSign::
	msgbox Route10_Text_RockTunnel, MSGBOX_SIGN
	end

Route10_EventScript_PowerPlantSign::
	msgbox Route10_Text_PowerPlant, MSGBOX_SIGN
	end

Route10_Lightning_OnTransition::
	call Route10_EventScript_ShowLightningRaikou
	end

@ Issue #234: original FireRed-style post-League stationary encounter.
@ The battle remains independent of all other legendary events.
Route10_EventScript_ShowLightningRaikou::
	goto_if_unset FLAG_SYS_GAME_CLEAR, Route10_EventScript_HideLightningRaikou
	goto_if_set FLAG_FOUGHT_LIGHTNING_RAIKOU, Route10_EventScript_HideLightningRaikou
	clearflag FLAG_HIDE_LIGHTNING_RAIKOU
	return

Route10_EventScript_HideLightningRaikou::
	setflag FLAG_HIDE_LIGHTNING_RAIKOU
	return

Route10_EventScript_LightningRaikou::
	goto_if_questlog EventScript_ReleaseEnd
	goto_if_unset FLAG_SYS_GAME_CLEAR, EventScript_ReleaseEnd
	goto_if_set FLAG_FOUGHT_LIGHTNING_RAIKOU, EventScript_ReleaseEnd
	special QuestLog_CutRecording
	lock
	faceplayer
	setwildbattle SPECIES_RAIKOU, 50
	waitse
	playmoncry SPECIES_RAIKOU, CRY_MODE_ENCOUNTER
	message Text_Gyaoo
	waitmessage
	waitmoncry
	delay 10
	playbgm MUS_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	special StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route10_EventScript_DefeatedLightningRaikou
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Route10_EventScript_CaughtLightningRaikou
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Route10_EventScript_RanFromLightningRaikou
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Route10_EventScript_RanFromLightningRaikou
	release
	end

Route10_EventScript_CaughtLightningRaikou::
	setflag FLAG_FOUGHT_LIGHTNING_RAIKOU
	goto EventScript_RemoveStaticMon

Route10_EventScript_DefeatedLightningRaikou::
	setflag FLAG_FOUGHT_LIGHTNING_RAIKOU
	goto EventScript_RemoveStaticMon

Route10_EventScript_RanFromLightningRaikou::
	setvar VAR_0x8004, SPECIES_RAIKOU
	goto EventScript_MonFlewAway
	end
