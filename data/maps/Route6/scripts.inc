Route6_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route6_OnTransition
	.byte 0

@ Keep unlock visibility separate from the permanent collection flag.
@ An old save that visited Route 6 before Misty can still reveal the item later.
Route6_OnTransition::
	goto_if_set FLAG_GOT_ROUTE6_TM34, Route6_HideTM34
	goto_if_unset FLAG_DEFEATED_MISTY, Route6_HideTM34
	clearflag FLAG_HIDE_ROUTE6_TM34
	end

Route6_HideTM34::
	setflag FLAG_HIDE_ROUTE6_TM34
	end

Route6_EventScript_ItemTM34::
	goto_if_unset FLAG_DEFEATED_MISTY, Route6_EndTM34
	goto_if_set FLAG_GOT_ROUTE6_TM34, Route6_EndTM34
	finditem ITEM_TM34
@ Std_FindItem preserves its capacity result here, even after TM name buffering.
@ A full TM Case leaves both the item ball and permanent claim available.
	goto_if_eq VAR_0x8007, FALSE, Route6_EndTM34
	setflag FLAG_GOT_ROUTE6_TM34
Route6_EndTM34::
	end

Route6_EventScript_UndergroundPathSign::
	msgbox Route6_Text_UndergroundPathSign, MSGBOX_SIGN
	end
