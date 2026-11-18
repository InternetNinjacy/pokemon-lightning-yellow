CeladonCity_DepartmentStore_4F_MapScripts::
	.byte 0

CeladonCity_DepartmentStore_4F_EventScript_Man::
	msgbox CeladonCity_DepartmentStore_4F_Text_GettingPokeDollAsPresent, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_4F_EventScript_Youngster::
	msgbox CeladonCity_DepartmentStore_4F_Text_CanRunAwayWithPokeDoll, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_4F_EventScript_FloorSign::
	msgbox CeladonCity_DepartmentStore_4F_Text_FloorSign, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_4F_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	@ Keep a single clerk. Badge flags alone govern renewable Water/Thunder sales.
	goto_if_unset FLAG_BADGE01_GET, CeladonCity_DepartmentStore_4F_NoWater
	goto_if_unset FLAG_BADGE02_GET, CeladonCity_DepartmentStore_4F_WaterOnly
	pokemart CeladonCity_DepartmentStore_4F_ItemsBothBadges
	goto CeladonCity_DepartmentStore_4F_ClerkEnd

CeladonCity_DepartmentStore_4F_NoWater::
	goto_if_unset FLAG_BADGE02_GET, CeladonCity_DepartmentStore_4F_NeitherBadge
	pokemart CeladonCity_DepartmentStore_4F_ItemsThunderOnly
	goto CeladonCity_DepartmentStore_4F_ClerkEnd

CeladonCity_DepartmentStore_4F_WaterOnly::
	pokemart CeladonCity_DepartmentStore_4F_ItemsWaterOnly
	goto CeladonCity_DepartmentStore_4F_ClerkEnd

CeladonCity_DepartmentStore_4F_NeitherBadge::
	pokemart CeladonCity_DepartmentStore_4F_ItemsNeitherBadge

CeladonCity_DepartmentStore_4F_ClerkEnd::
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
CeladonCity_DepartmentStore_4F_ItemsNeitherBadge::
	.2byte ITEM_POKE_DOLL
	.2byte ITEM_RETRO_MAIL
	.2byte ITEM_FIRE_STONE
	.2byte ITEM_LEAF_STONE
	.2byte ITEM_SUN_STONE
	.2byte ITEM_MOON_STONE
	.2byte ITEM_KINGS_ROCK
	.2byte ITEM_METAL_COAT
	.2byte ITEM_DRAGON_SCALE
	.2byte ITEM_UP_GRADE
	.2byte ITEM_NONE

	.align 2
CeladonCity_DepartmentStore_4F_ItemsWaterOnly::
	.2byte ITEM_POKE_DOLL
	.2byte ITEM_RETRO_MAIL
	.2byte ITEM_FIRE_STONE
	.2byte ITEM_WATER_STONE
	.2byte ITEM_LEAF_STONE
	.2byte ITEM_SUN_STONE
	.2byte ITEM_MOON_STONE
	.2byte ITEM_KINGS_ROCK
	.2byte ITEM_METAL_COAT
	.2byte ITEM_DRAGON_SCALE
	.2byte ITEM_UP_GRADE
	.2byte ITEM_NONE

	.align 2
CeladonCity_DepartmentStore_4F_ItemsThunderOnly::
	.2byte ITEM_POKE_DOLL
	.2byte ITEM_RETRO_MAIL
	.2byte ITEM_FIRE_STONE
	.2byte ITEM_THUNDER_STONE
	.2byte ITEM_LEAF_STONE
	.2byte ITEM_SUN_STONE
	.2byte ITEM_MOON_STONE
	.2byte ITEM_KINGS_ROCK
	.2byte ITEM_METAL_COAT
	.2byte ITEM_DRAGON_SCALE
	.2byte ITEM_UP_GRADE
	.2byte ITEM_NONE

	.align 2
CeladonCity_DepartmentStore_4F_ItemsBothBadges::
	.2byte ITEM_POKE_DOLL
	.2byte ITEM_RETRO_MAIL
	.2byte ITEM_FIRE_STONE
	.2byte ITEM_THUNDER_STONE
	.2byte ITEM_WATER_STONE
	.2byte ITEM_LEAF_STONE
	.2byte ITEM_SUN_STONE
	.2byte ITEM_MOON_STONE
	.2byte ITEM_KINGS_ROCK
	.2byte ITEM_METAL_COAT
	.2byte ITEM_DRAGON_SCALE
	.2byte ITEM_UP_GRADE
	.2byte ITEM_NONE
