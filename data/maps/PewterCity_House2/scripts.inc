PewterCity_House2_MapScripts::
	.byte 0

PewterCity_House2_EventScript_OldMan::
	msgbox PewterCity_House2_Text_MonsLearnTechniquesAsTheyGrow, MSGBOX_NPC
	end

PewterCity_House2_EventScript_LittleBoy::
	msgbox PewterCity_House2_Text_MonsEasierCatchIfStatused, MSGBOX_NPC
	end

@ Both southern tiles of the lower-right table share this permanent pickup.
@ A full Bag leaves the Dust Ball unclaimed so the player can retry.
PewterCity_House2_EventScript_DustBall::
	goto_if_set FLAG_GOT_PEWTER_HOUSE2_DUST_BALL, PewterCity_House2_EventScript_DustBallEnd
	finditem ITEM_DUST_BALL
	goto_if_eq VAR_0x8007, FALSE, PewterCity_House2_EventScript_DustBallEnd
	setflag FLAG_GOT_PEWTER_HOUSE2_DUST_BALL
PewterCity_House2_EventScript_DustBallEnd::
	end
