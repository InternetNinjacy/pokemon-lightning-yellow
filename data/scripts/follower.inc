// WRL-010: one small follower interaction, no affection-system mutations.
EventScript_FollowerInteraction::
	lock
	msgbox Follower_Text_Responds, MSGBOX_DEFAULT
	release
	end

Follower_Text_Responds::
	.string "{STR_VAR_1} looks at you!$"
