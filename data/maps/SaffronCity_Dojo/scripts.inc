SaffronCity_Dojo_MapScripts::
	.byte 0

SaffronCity_Dojo_EventScript_Ranger1::
	trainerbattle_single TRAINER_RANGER_CAMP_1, SaffronCity_Dojo_Text_Ranger1Intro, SaffronCity_Dojo_Text_Ranger1Defeat
	msgbox SaffronCity_Dojo_Text_Ranger1After, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_Ranger2::
	trainerbattle_single TRAINER_RANGER_CAMP_2, SaffronCity_Dojo_Text_Ranger2Intro, SaffronCity_Dojo_Text_Ranger2Defeat
	msgbox SaffronCity_Dojo_Text_Ranger2After, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_Ranger3::
	trainerbattle_single TRAINER_RANGER_CAMP_3, SaffronCity_Dojo_Text_Ranger3Intro, SaffronCity_Dojo_Text_Ranger3Defeat
	msgbox SaffronCity_Dojo_Text_Ranger3After, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_Ranger4::
	trainerbattle_single TRAINER_RANGER_CAMP_4, SaffronCity_Dojo_Text_Ranger4Intro, SaffronCity_Dojo_Text_Ranger4Defeat
	msgbox SaffronCity_Dojo_Text_Ranger4After, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_RangerPark::
	lock
	faceplayer
	goto_if_defeated TRAINER_RANGER_PARK, SaffronCity_Dojo_RangerParkWon
	goto_if_set FLAG_RANGER_PARK_INTRO_SEEN, SaffronCity_Dojo_RangerCheckFour
	msgbox SaffronCity_Dojo_Text_ParkLore
	setflag FLAG_RANGER_PARK_INTRO_SEEN
SaffronCity_Dojo_RangerCheckFour::
	goto_if_not_defeated TRAINER_RANGER_CAMP_1, SaffronCity_Dojo_RangerParkLocked
	goto_if_not_defeated TRAINER_RANGER_CAMP_2, SaffronCity_Dojo_RangerParkLocked
	goto_if_not_defeated TRAINER_RANGER_CAMP_3, SaffronCity_Dojo_RangerParkLocked
	goto_if_not_defeated TRAINER_RANGER_CAMP_4, SaffronCity_Dojo_RangerParkLocked
	msgbox SaffronCity_Dojo_Text_ParkChallenge, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SaffronCity_Dojo_RangerParkNo
	trainerbattle_double TRAINER_RANGER_PARK, SaffronCity_Dojo_Text_ParkYes, SaffronCity_Dojo_Text_ParkDefeat, SaffronCity_Dojo_Text_NeedTwo, SaffronCity_Dojo_RangerParkWon
	release
	end

SaffronCity_Dojo_RangerParkLocked::
	msgbox SaffronCity_Dojo_Text_ParkLocked
	release
	end

SaffronCity_Dojo_RangerParkNo::
	msgbox SaffronCity_Dojo_Text_ParkNo
	release
	end

SaffronCity_Dojo_RangerParkWon::
	goto_if_set FLAG_RANGER_CAMP_GIFT_RECEIVED, SaffronCity_Dojo_RangerParkAfterGift
	msgbox SaffronCity_Dojo_Text_ParkBeforeGift
	release
	end

SaffronCity_Dojo_RangerParkAfterGift::
	msgbox SaffronCity_Dojo_Text_ParkAfterGift
	release
	end

SaffronCity_Dojo_EventScript_RangerGift0::
	lock
	faceplayer
	setvar VAR_0x8004, 0
	setvar VAR_0x8005, SPECIES_ARCANINE
	goto SaffronCity_Dojo_RangerGift

SaffronCity_Dojo_EventScript_RangerGift1::
	lock
	faceplayer
	setvar VAR_0x8004, 1
	setvar VAR_0x8005, SPECIES_NOCTOWL
	goto SaffronCity_Dojo_RangerGift

SaffronCity_Dojo_EventScript_RangerGift2::
	lock
	faceplayer
	setvar VAR_0x8004, 2
	setvar VAR_0x8005, SPECIES_GOLDUCK
	goto SaffronCity_Dojo_RangerGift

SaffronCity_Dojo_RangerGift::
	goto_if_set FLAG_RANGER_CAMP_GIFT_RECEIVED, SaffronCity_Dojo_RangerGiftTaken
	goto_if_not_defeated TRAINER_RANGER_PARK, SaffronCity_Dojo_RangerGiftLocked
	bufferspeciesname STR_VAR_1, VAR_0x8005
	showmonpic VAR_0x8005, 10, 3
	msgbox SaffronCity_Dojo_Text_GiftPrompt, MSGBOX_YESNO
	hidemonpic
	goto_if_eq VAR_RESULT, NO, SaffronCity_Dojo_RangerEndGift
	specialvar VAR_RESULT, RangerGiveGift
	goto_if_ne VAR_RESULT, MON_GIVEN_TO_PARTY, SaffronCity_Dojo_RangerGiftFull
	removeobject 6
	removeobject 7
	removeobject 8
	playfanfare MUS_LEVEL_UP
	message SaffronCity_Dojo_Text_GiftReceived
	waitmessage
	waitfanfare
	msgbox Text_GiveNicknameToThisMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SaffronCity_Dojo_RangerEndGift
	call EventScript_GetGiftMonPartySlot
	call EventScript_ChangePokemonNickname
SaffronCity_Dojo_RangerEndGift::
	release
	end

SaffronCity_Dojo_RangerGiftFull::
	msgbox SaffronCity_Dojo_Text_GiftFull
	release
	end

SaffronCity_Dojo_RangerGiftLocked::
	msgbox SaffronCity_Dojo_Text_GiftLocked
	release
	end

SaffronCity_Dojo_RangerGiftTaken::
	msgbox SaffronCity_Dojo_Text_GiftTaken
	release
	end

SaffronCity_Dojo_EventScript_Statue::
	msgbox SaffronCity_Dojo_Text_Identity, MSGBOX_SIGN
	end

SaffronCity_Dojo_EventScript_LeftScroll::
	msgbox SaffronCity_Dojo_Text_Identity, MSGBOX_SIGN
	end

SaffronCity_Dojo_EventScript_RightScroll::
	msgbox SaffronCity_Dojo_Text_Identity, MSGBOX_SIGN
	end
