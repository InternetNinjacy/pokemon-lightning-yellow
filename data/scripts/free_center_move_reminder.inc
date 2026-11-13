@ Issue #191: reuse FireRed's existing species/level-aware Move Relearner UI.
@ This is intentionally separate from the mushroom-charging Two Island Move Maniac.
EventScript_FreePkmnCenterMoveReminder::
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	lock
	faceplayer
	msgbox Text_FreePkmnCenterMoveReminder_Offer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_FreePkmnCenterMoveReminder_Exit

EventScript_FreePkmnCenterMoveReminder_ChooseMon::
	msgbox Text_FreePkmnCenterMoveReminder_ChooseMon
	special ChooseMonForMoveRelearner
	waitstate
	goto_if_ge VAR_0x8004, PARTY_SIZE, EventScript_FreePkmnCenterMoveReminder_Exit
	special IsSelectedMonEgg
	goto_if_eq VAR_RESULT, TRUE, EventScript_FreePkmnCenterMoveReminder_Egg
	goto_if_eq VAR_0x8005, 0, EventScript_FreePkmnCenterMoveReminder_NoMoves
	msgbox Text_FreePkmnCenterMoveReminder_ChooseMove
	special TeachMoveRelearnerMove
	waitstate
	goto_if_eq VAR_0x8004, 0, EventScript_FreePkmnCenterMoveReminder_ChooseMon
	msgbox Text_FreePkmnCenterMoveReminder_Success
	goto EventScript_FreePkmnCenterMoveReminder_ChooseMon

EventScript_FreePkmnCenterMoveReminder_Egg::
	msgbox Text_FreePkmnCenterMoveReminder_Egg
	goto EventScript_FreePkmnCenterMoveReminder_ChooseMon

EventScript_FreePkmnCenterMoveReminder_NoMoves::
	msgbox Text_FreePkmnCenterMoveReminder_NoMoves
	goto EventScript_FreePkmnCenterMoveReminder_ChooseMon

EventScript_FreePkmnCenterMoveReminder_Exit::
	msgbox Text_FreePkmnCenterMoveReminder_Goodbye
	release
	end

Text_FreePkmnCenterMoveReminder_Offer::
	.string "I can help your POKéMON remember\n"
	.string "moves they've learned before.\p"
	.string "My service is always FREE.\n"
	.string "Would you like my help?$"

Text_FreePkmnCenterMoveReminder_ChooseMon::
	.string "Which POKéMON needs a reminder?$"

Text_FreePkmnCenterMoveReminder_ChooseMove::
	.string "Which move should it remember?$"

Text_FreePkmnCenterMoveReminder_Egg::
	.string "I can't teach moves to an EGG.$"

Text_FreePkmnCenterMoveReminder_NoMoves::
	.string "There aren't any eligible moves\n"
	.string "for that POKéMON right now.$"

Text_FreePkmnCenterMoveReminder_Success::
	.string "There! Remembering is free!\p"
	.string "You may choose another POKéMON.$"

Text_FreePkmnCenterMoveReminder_Goodbye::
	.string "Come back whenever you like!$"
