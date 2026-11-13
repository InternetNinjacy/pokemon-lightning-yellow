Route21_South_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route21_South_SowOnTransition
	.byte 0

Route21_South_SowOnTransition::
	setvar VAR_0x8004, 1 @ SPECIAL_WEATHER_GLALIE
	callnative SpecialWeatherActivateScript
	end
