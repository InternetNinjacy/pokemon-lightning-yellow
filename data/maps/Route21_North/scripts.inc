Route21_North_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route21_North_SowOnTransition
	.byte 0

Route21_North_SowOnTransition::
	setvar VAR_0x8004, 1 @ SPECIAL_WEATHER_GLALIE
	callnative SpecialWeatherActivateScript
	end

Route21_North_EventScript_SowGlalie::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	setvar VAR_0x8004, 1
	setwildbattle SPECIES_GLALIE, 40
	callnative SowPrepareScriptedEncounter
	goto_if_eq VAR_RESULT, FALSE, Route21_North_EventScript_SowAbort
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route21_North_EventScript_SowResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Route21_North_EventScript_SowResolved
Route21_North_EventScript_SowAbort::
	release
	end

Route21_North_EventScript_SowResolved::
	copyvar VAR_0x8005, VAR_RESULT
	call SpecialWeather_EventScript_FinishAftermath
	removeobject VAR_LAST_TALKED
	release
	end
