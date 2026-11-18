Route16_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, Route16_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, Route16_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Route16_OnWarp
	.byte 0

Route16_OnResume::
	call_if_set FLAG_SYS_SPECIAL_WILD_BATTLE, Route16_EventScript_RemoveSnorlax
	end

Route16_EventScript_RemoveSnorlax::
	removeobject VAR_LAST_TALKED
	return

Route16_OnTransition::
	call_if_eq VAR_MAP_SCENE_ROUTE16, 1, Route16_OnTransitionCyclingRoad
	goto_if_set FLAG_FOUGHT_SOW_NOCTOWL, Route16_HideSowNoctowl
	clearflag FLAG_HIDE_SOW_NOCTOWL
	end

Route16_HideSowNoctowl::
	setflag FLAG_HIDE_SOW_NOCTOWL
	end

Route16_OnTransitionCyclingRoad::
	setflag FLAG_SYS_ON_CYCLING_ROAD
	return

Route16_OnWarp::
	map_script_2 VAR_MAP_SCENE_ROUTE16, 1, Route16_OnWarpCyclingRoad
	.2byte 0

Route16_OnWarpCyclingRoad::
	special ForcePlayerOntoBike
	end

Route16_EventScript_Snorlax::
	lock
	faceplayer
	goto_if_unset FLAG_GOT_POKE_FLUTE, Route16_EventScript_SnorlaxNoPokeFlute
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	msgbox Text_WantToUsePokeFlute, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route16_EventScript_DontUsePokeFlute
	call EventScript_AwakenSnorlax
	setwildbattle SPECIES_SNORLAX, 30
	waitse
	playmoncry SPECIES_SNORLAX, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_HIDE_ROUTE_16_SNORLAX
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	dowildbattle
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route16_EventScript_FoughtSnorlax
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Route16_EventScript_FoughtSnorlax
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Route16_EventScript_FoughtSnorlax
	release
	end

Route16_EventScript_DontUsePokeFlute::
	release
	end

Route16_EventScript_FoughtSnorlax::
	msgbox Text_SnorlaxReturnedToMountains
	release
	end

Route16_EventScript_SnorlaxNoPokeFlute::
	msgbox Route16_Text_MonSprawledOutInSlumber
	release
	end

Route16_EventScript_CyclingRoadSign::
	msgbox Route16_Text_CyclingRoadSign, MSGBOX_SIGN
	end

Route16_EventScript_RouteSign::
	msgbox Route16_Text_RouteSign, MSGBOX_SIGN
	end

// SOW-006: optional, challenging wild Noctowl outside the original Fly house.
// Neither house NPCs/HM02 nor the original Snorlax script are altered.
Route16_EventScript_SowNoctowl::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	goto_if_set FLAG_FOUGHT_SOW_NOCTOWL, Route16_SowNoctowlExit
	setwildbattle SPECIES_NOCTOWL, 36
	callnative SowPrepareNoctowlEncounter
	goto_if_eq VAR_RESULT, FALSE, Route16_SowNoctowlExit
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route16_SowNoctowlResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Route16_SowNoctowlResolved
Route16_SowNoctowlExit::
	release
	end

Route16_SowNoctowlResolved::
	setflag FLAG_FOUGHT_SOW_NOCTOWL
	setflag FLAG_HIDE_SOW_NOCTOWL
	removeobject VAR_LAST_TALKED
	release
	end
