MysteryVendor_Start::
	lock
	faceplayer
	specialvar VAR_RESULT, MysteryVendorStatus
	goto_if_eq VAR_RESULT, MYSTERY_VENDOR_SOLD, MysteryVendor_AlreadySold
	goto_if_eq VAR_RESULT, MYSTERY_VENDOR_INVALID, MysteryVendor_InvalidState
	showmoneybox 0, 0
	switch VAR_0x8009
	case 0, MysteryVendor_Intro01
	case 1, MysteryVendor_Intro02
	case 2, MysteryVendor_Intro03
	case 3, MysteryVendor_Intro04
	case 4, MysteryVendor_Intro05
	case 5, MysteryVendor_Intro06
	case 6, MysteryVendor_Intro07
	case 7, MysteryVendor_Intro08
	case 8, MysteryVendor_Intro09
	case 9, MysteryVendor_Intro10
	goto MysteryVendor_NoRoom

MysteryVendor_TryPurchase::
	goto_if_eq VAR_RESULT, NO, MysteryVendor_Declined
	specialvar VAR_RESULT, PurchaseMysteryPokemon
	goto_if_eq VAR_RESULT, 0, MysteryVendor_Party
	goto_if_eq VAR_RESULT, 1, MysteryVendor_PC
	goto_if_eq VAR_RESULT, MYSTERY_VENDOR_POOR, MysteryVendor_Poor
	goto MysteryVendor_NoRoom

MysteryVendor_Reveal::
	updatemoneybox
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	playfanfare MUS_LEVEL_UP
	message MysteryVendor_Text_Reveal
	waitmessage
	waitfanfare
	waitbuttonpress
	return

MysteryVendor_Party::
	call MysteryVendor_Reveal
	msgbox MysteryVendor_Text_Nickname, MSGBOX_YESNO
	hidemoneybox
	goto_if_eq VAR_RESULT, NO, MysteryVendor_End
	call EventScript_GetGiftMonPartySlot
	fadescreen FADE_TO_BLACK
	special ChangePokemonNickname
	waitstate
	goto MysteryVendor_End

MysteryVendor_PC::
	call MysteryVendor_Reveal
	msgbox MysteryVendor_Text_Nickname, MSGBOX_YESNO
	hidemoneybox
	goto_if_eq VAR_RESULT, NO, MysteryVendor_Transfer
	call EventScript_NameReceivedBoxMon
MysteryVendor_Transfer::
	call EventScript_TransferredToPC
	goto MysteryVendor_End

MysteryVendor_Declined::
	msgbox MysteryVendor_Text_Declined
	hidemoneybox
	goto MysteryVendor_End

MysteryVendor_Poor::
	msgbox MysteryVendor_Text_Poor
	hidemoneybox
	goto MysteryVendor_End

MysteryVendor_NoRoom::
	msgbox MysteryVendor_Text_NoRoom
	hidemoneybox
	goto MysteryVendor_End

MysteryVendor_InvalidState::
	msgbox MysteryVendor_Text_NoRoom
	goto MysteryVendor_End

MysteryVendor_AlreadySold::
	msgbox MysteryVendor_Text_AlreadySold
MysteryVendor_End::
	release
	end

MysteryVendor_Text_Declined::
	.string "No problem. The offer stands!$"
MysteryVendor_Text_Poor::
	.string "Sorry, you'll need ¥700.$"
MysteryVendor_Text_NoRoom::
	.string "You don't have room for another\n"
	.string "POKéMON. Make some space and\l"
	.string "come back!$"
MysteryVendor_Text_Reveal::
	.string "Surprise! It's {STR_VAR_1}!$"
MysteryVendor_Text_Nickname::
	.string "Would you like to give\n"
	.string "{STR_VAR_1} a nickname?$"
MysteryVendor_Text_AlreadySold::
	.string "I hope you and your new POKéMON\n"
	.string "get along!$"

MysteryVendor_Intro01::
	msgbox MysteryVendor_Text_Intro01, MSGBOX_YESNO
	goto MysteryVendor_TryPurchase

MysteryVendor_Text_Intro01::
	.string "Hey, kid! Have I got a deal\n"
	.string "for you! A fine POKéMON for\p"
	.string "just ¥700! What kind? Heh\n"
	.string "heh... That's the surprise!\p"
	.string "Interested?$"

MysteryVendor_Intro02::
	msgbox MysteryVendor_Text_Intro02, MSGBOX_YESNO
	goto MysteryVendor_TryPurchase

MysteryVendor_Text_Intro02::
	.string "I've collected quite a few\n"
	.string "interesting POKéMON over the\p"
	.string "years. I'm letting one go for\n"
	.string "just ¥700. You won't know\p"
	.string "which one until it's yours.\n"
	.string "Care to take a chance?$"

MysteryVendor_Intro03::
	msgbox MysteryVendor_Text_Intro03, MSGBOX_YESNO
	goto MysteryVendor_TryPurchase

MysteryVendor_Text_Intro03::
	.string "With so many TRAINERS passing\n"
	.string "through the port, you never\p"
	.string "know what kind of POKéMON\n"
	.string "you'll come across. I've got\p"
	.string "one for sale. Just ¥700! Would\n"
	.string "you like it?$"

MysteryVendor_Intro04::
	msgbox MysteryVendor_Text_Intro04, MSGBOX_YESNO
	goto MysteryVendor_TryPurchase

MysteryVendor_Text_Intro04::
	.string "Everyone's looking for\n"
	.string "valuable NUGGETS, but I've got\p"
	.string "something much more\n"
	.string "interesting! A mystery POKéMON\p"
	.string "for ¥700! Want to see what you\n"
	.string "get?$"

MysteryVendor_Intro05::
	msgbox MysteryVendor_Text_Intro05, MSGBOX_YESNO
	goto MysteryVendor_TryPurchase

MysteryVendor_Text_Intro05::
	.string "I've been traveling with an\n"
	.string "unusual POKéMON, but I can't\p"
	.string "keep caring for it. For ¥700,\n"
	.string "it's yours. I won't tell you\p"
	.string "what it is until you agree.\n"
	.string "Interested?$"

MysteryVendor_Intro06::
	msgbox MysteryVendor_Text_Intro06, MSGBOX_YESNO
	goto MysteryVendor_TryPurchase

MysteryVendor_Text_Intro06::
	.string "Bah! I've lost enough money at\n"
	.string "those slot machines! Maybe\p"
	.string "you'll have better luck with\n"
	.string "this. ¥700 gets you one\p"
	.string "mystery POKéMON. Feeling\n"
	.string "lucky?$"

MysteryVendor_Intro07::
	msgbox MysteryVendor_Text_Intro07, MSGBOX_YESNO
	goto MysteryVendor_TryPurchase

MysteryVendor_Text_Intro07::
	.string "There are all sorts of\n"
	.string "fascinating POKéMON in this\p"
	.string "world! I happen to have one\n"
	.string "available for ¥700. No telling\p"
	.string "what you'll get! Want one?$"

MysteryVendor_Intro08::
	msgbox MysteryVendor_Text_Intro08, MSGBOX_YESNO
	goto MysteryVendor_TryPurchase

MysteryVendor_Text_Intro08::
	.string "Now that TEAM ROCKET is gone,\n"
	.string "it's good to see TRAINERS\p"
	.string "around again. I've got a\n"
	.string "POKéMON that needs a new home.\p"
	.string "¥700, if you're interested!$"

MysteryVendor_Intro09::
	msgbox MysteryVendor_Text_Intro09, MSGBOX_YESNO
	goto MysteryVendor_TryPurchase

MysteryVendor_Text_Intro09::
	.string "The scientists around here\n"
	.string "sure have some remarkable\p"
	.string "POKéMON! I've got an\n"
	.string "interesting one myself. It's\p"
	.string "yours for ¥700, but its\n"
	.string "identity is a secret. Deal?$"

MysteryVendor_Intro10::
	msgbox MysteryVendor_Text_Intro10, MSGBOX_YESNO
	goto MysteryVendor_TryPurchase

MysteryVendor_Text_Intro10::
	.string "People come to these islands\n"
	.string "from all over. And they bring\p"
	.string "all sorts of POKéMON! I've got\n"
	.string "one to sell for ¥700. Care to\p"
	.string "try your luck?$"
