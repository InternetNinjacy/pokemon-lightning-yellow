#!/usr/bin/env python3
"""Build a headless mGBA runner for the actual ROM/ELF; no copyrighted BIOS."""
from pathlib import Path
import os
import json
import re
import struct
import subprocess
import tempfile
import shutil

# Generate ABI constants with the game's ARM compiler, never host offsetof.
expressions = {
 'BATTLESTRUCT_SIZE':'sizeof(struct BattleStruct)',
 'BATTLESTRUCT_TW_USER':'offsetof(struct BattleStruct,thunderWaveUser)',
 'BATTLESTRUCT_TW_TARGETS':'offsetof(struct BattleStruct,thunderWaveTargets)',
 'BATTLESTRUCT_TW_ACTIVE':'offsetof(struct BattleStruct,thunderWaveActive)',
 'BRES_AI':'offsetof(struct BattleResources,ai)',
 'AI_SCORE':'offsetof(struct AI_ThinkingStruct,score)',
 'AI_FLAGS':'offsetof(struct AI_ThinkingStruct,aiFlags)',
 'BMON_MAX_HP':'offsetof(struct BattlePokemon,maxHP)',
 'BOX_SECURE':'offsetof(struct BoxPokemon,secure)',
 'SAVE1_PARTY':'offsetof(struct SaveBlock1,playerParty)',
 'R5_VERSION':'offsetof(struct Route5DaycareSave,version)',
 'SECTOR_SIZE':'SECTOR_SIZE',
 'SECTOR_SIGNATURE':'offsetof(struct SaveSector,signature)',
 'SECTOR_CHECKSUM':'offsetof(struct SaveSector,checksum)',

 'BMOVE_POWER':'offsetof(struct BattleMove,power)',
 'BMOVE_TYPE':'offsetof(struct BattleMove,type)',
 'BMOVE_CATEGORY':'offsetof(struct BattleMove,category)',
 'BMOVE_ACCURACY':'offsetof(struct BattleMove,accuracy)',
 'WIN_WIDTH':'WINDOW_WIDTH',
 'WIN_HEIGHT':'WINDOW_HEIGHT',
 'WIN_LEFT':'WINDOW_TILEMAP_LEFT',
 'MOVE_WINDOW':'B_WIN_MOVE_NAME_1',
 'PP_WINDOW':'B_WIN_PP',
 'TYPE_WINDOW':'B_WIN_MOVE_TYPE',
 'MOVEINFO_CUR_PP':'offsetof(struct ChooseMoveStruct,currentPp)',
 'MOVEINFO_MAX_PP':'offsetof(struct ChooseMoveStruct,maxPp)',
 'RELEARNER_BG':'offsetof(struct LearnMoveGfxResources,bg1TilemapBuffer)',
 'RELEARNER_INDEX':'offsetof(struct LearnMoveGfxResources,selectedIndex)',
 'SUMMARY_NAME_BUFFERS':'offsetof(struct PokemonSummaryScreenData,summary.moveNameStrBufs)',
 'MOVE_NAME_CAPACITY':'MOVE_NAME_LENGTH + 1',
 'BATTLE_TEXT_CAPACITY':'BATTLE_TEXT_BUFFER_SIZE',
 'BMSG_SIZE':'sizeof(struct BattleMsgData)',
 'BMSG_MOVE':'offsetof(struct BattleMsgData,currentMove)',
 'BMSG_TEXT':'offsetof(struct BattleMsgData,textBuffs)',
 'PSA_SIZE':'sizeof(struct PokemonSpecialAnim)',
 'PSA_FORGOTTEN':'offsetof(struct PokemonSpecialAnim,nameOfMoveForgotten)',
 'PSA_TEACH':'offsetof(struct PokemonSpecialAnim,nameOfMoveToTeach)',
 'PSA_CANCEL':'offsetof(struct PokemonSpecialAnim,cancelDisabled)',
 'PSA_SPECIES':'offsetof(struct PokemonSpecialAnim,species)',
 'RELEARNER_SIZE':'sizeof(struct LearnMoveGfxResources)',
 'RELEARNER_NAMES':'offsetof(struct LearnMoveGfxResources,listMenuStrbufs)',
 'RELEARNER_MOVES':'offsetof(struct LearnMoveGfxResources,learnableMoves)',
 'RELEARNER_COUNT':'offsetof(struct LearnMoveGfxResources,numLearnableMoves)',
 'RELEARNER_SLOT':'offsetof(struct LearnMoveGfxResources,selectedPartyMember)',

    "SAVE1_WEATHER_CONTEXT":"offsetof(struct SaveBlock1,unused_3D24)",
    "MAPHEADER_WEATHER":"offsetof(struct MapHeader,weather)",
    "MAPHEADER_TYPE":"offsetof(struct MapHeader,mapType)",
    "WEATHER_GAMMA":"offsetof(struct Weather,gammaIndex)",
    "WEATHER_GFX":"offsetof(struct Weather,weatherGfxLoaded)",
    "WEATHER_SAND_CREATED":"offsetof(struct Weather,sandstormSpritesCreated)",
    "WEATHER_PAL_STATE":"offsetof(struct Weather,palProcessingState)",
    "WEATHER_COMPLETE":"offsetof(struct Weather,weatherChangeComplete)",
    "SAVE1_RIVAL_NAME": "offsetof(struct SaveBlock1,rivalName)",
    'BMOVE_SIZE': 'sizeof(struct BattleMove)',
    'BMOVE_PP': 'offsetof(struct BattleMove,pp)',
    'BMON_FRIENDSHIP': 'offsetof(struct BattlePokemon,friendship)',
    'BRES_HISTORY': 'offsetof(struct BattleResources,battleHistory)',
    'HISTORY_ITEMS': 'offsetof(struct BattleHistory,trainerItems)',
    'HISTORY_COUNT': 'offsetof(struct BattleHistory,itemsNo)',

    "WEATHER_DURATION": "offsetof(struct WishFutureKnock,weatherDuration)",
    'SCRIPT_EXP_STATE': 'offsetof(struct BattleScripting,getexpState)',
    'BATTLESTRUCT_EXP_VALUE': 'offsetof(struct BattleStruct,expValue)',
    'BATTLESTRUCT_EXP_GETTER': 'offsetof(struct BattleStruct,expGetterMonId)',
    'BATTLESTRUCT_SENT_IN': 'offsetof(struct BattleStruct,sentInPokes)',
    'BATTLESTRUCT_WILD_SONG': 'offsetof(struct BattleStruct,wildVictorySong)',
    'WINDOW_SIZE': 'sizeof(struct Window)',
    'WINDOW_BG': 'offsetof(struct Window,window.bg)',
    'SCRIPT_MODE': 'offsetof(struct ScriptContext,mode)',
    'SCRIPT_NATIVE': 'offsetof(struct ScriptContext,nativePtr)',
    'TEXT_COUNT': 'NUM_TEXT_PRINTERS',
    'TEXT_SIZE': 'sizeof(struct TextPrinter)',
    'TEXT_ACTIVE': 'offsetof(struct TextPrinter,active)',
    'TEXT_STATE': 'offsetof(struct TextPrinter,state)',
    'TEXT_CLEAR': 'RENDER_STATE_CLEAR',
    'TEXT_SCROLL_START': 'RENDER_STATE_SCROLL_START',
    'TEXT_WAIT': 'RENDER_STATE_WAIT',
    'FLAG_SET_SEEN': 'FLAG_SET_SEEN',
    'FLAG_SET_CAUGHT': 'FLAG_SET_CAUGHT',
    'FLAG_GET_CAUGHT': 'FLAG_GET_CAUGHT',
    'MON_SIZE': 'sizeof(struct Pokemon)',
    'SAVE1_SIZE': 'sizeof(struct SaveBlock1)',
    'SAVE2_SIZE': 'sizeof(struct SaveBlock2)',
    'SAVE2_KEY': 'offsetof(struct SaveBlock2, encryptionKey)',
    'MAIN_HELD': 'offsetof(struct Main, heldKeys)',
    'MAIN_NEW': 'offsetof(struct Main, newKeys)',
    'MAIN_CB2': 'offsetof(struct Main, callback2)',
    'TASK_SIZE': 'sizeof(struct Task)',
    'TASK_DATA': 'offsetof(struct Task, data)',
}
# Private screen layout is read from the same source compiled into this ROM.
screen_text=Path('src/pokedex_screen.c').read_text()
screen_struct=screen_text[screen_text.index('struct PokedexScreenData\n'):]
screen_struct=screen_struct[:screen_struct.index('\n};')+3]
summary_text=Path('src/pokemon_summary_screen.c').read_text()
summary_struct=summary_text[summary_text.index('struct PokemonSummaryScreenData\n'):]
summary_struct=summary_struct[:summary_struct.index('\n};')+3]
for key,expr in {
 'SCREEN_ORDERED_MENU':'offsetof(struct PokedexScreenData,orderedListMenuTaskId)',
 'SCREEN_LIST':'offsetof(struct PokedexScreenData,listItems)',
 'SCREEN_COUNT':'offsetof(struct PokedexScreenData,orderedDexCount)',
 'SCREEN_STATE':'offsetof(struct PokedexScreenData,state)',
 'SCREEN_TASK':'offsetof(struct PokedexScreenData,taskId)',
 'SCREEN_DEX_SPECIES':'offsetof(struct PokedexScreenData,dexSpecies)',
 'LIST_SIZE':'sizeof(struct ListMenuItem)',
 'LIST_INDEX':'offsetof(struct ListMenuItem,index)',
 'BMON_SIZE':'sizeof(struct BattlePokemon)',
 'BMON_SPECIES':'offsetof(struct BattlePokemon,species)',
 'BMON_ABILITY':'offsetof(struct BattlePokemon,ability)',
 'BMON_HP':'offsetof(struct BattlePokemon,hp)',
 'BMON_TYPE1':'offsetof(struct BattlePokemon,type1)',
 'BMON_TYPE2':'offsetof(struct BattlePokemon,type2)',
 'BUFFER_STRIDE':'sizeof(gBattleBufferA[0])',
 'MOVEINFO_MOVES':'offsetof(struct ChooseMoveStruct,moves)',
 'META_ALL':'METATILE_ATTRIBUTES_ALL',
 'META_BEHAVIOR':'METATILE_ATTRIBUTE_BEHAVIOR',
 'META_ENCOUNTER':'METATILE_ATTRIBUTE_ENCOUNTER_TYPE',
 'TILE_LAND':'TILE_ENCOUNTER_LAND',
 'SAVE2_PLAYER_NAME':'offsetof(struct SaveBlock2,playerName)',
 'BAG_POCKET':'offsetof(struct BagStruct,pocket)',
 'SAVE1_LOCATION':'offsetof(struct SaveBlock1,location)',
 'WILD_HEADER_SIZE':'sizeof(struct WildPokemonHeader)',
 'WILD_HEADER_LAND':'offsetof(struct WildPokemonHeader,landMonsInfo)',
 'BMON_SPEED':'offsetof(struct BattlePokemon,speed)',
 'BMON_LEVEL':'offsetof(struct BattlePokemon,level)',
 'BMON_STATUS1':'offsetof(struct BattlePokemon,status1)',
 'BMON_STATUS2':'offsetof(struct BattlePokemon,status2)',
 'BMON_ITEM':'offsetof(struct BattlePokemon,item)',
 'BMON_MOVES':'offsetof(struct BattlePokemon,moves)',
 'BMON_PP':'offsetof(struct BattlePokemon,pp)',
 'BATTLESTRUCT_MOVETARGET':'offsetof(struct BattleStruct,moveTarget)',
 'SUMMARY_TYPES':'offsetof(struct PokemonSummaryScreenData,monTypes)',
 'SUMMARY_PAGE':'offsetof(struct PokemonSummaryScreenData,curPageIndex)',
 'SUMMARY_MOVE_COUNT':'offsetof(struct PokemonSummaryScreenData,numMoves)',
 'SUMMARY_MOVES':'offsetof(struct PokemonSummaryScreenData,moveIds)',
 'BMON_ATTACK':'offsetof(struct BattlePokemon,attack)',
 'BMON_DEFENSE':'offsetof(struct BattlePokemon,defense)',
 'BMON_SPATTACK':'offsetof(struct BattlePokemon,spAttack)',
 'BMON_SPDEFENSE':'offsetof(struct BattlePokemon,spDefense)',
 'BMON_STAGES':'offsetof(struct BattlePokemon,statStages)',
 'BMON_STAGES_COUNT':'NUM_BATTLE_STATS',
 'BATTLESTRUCT_DYNAMIC_TYPE':'offsetof(struct BattleStruct,dynamicMoveType)',
 'SCRIPTING_MULTIPLIER':'offsetof(struct BattleScripting,dmgMultiplier)',
 'SCRIPT_PTR':'offsetof(struct ScriptContext,scriptPtr)',
 'SAVE1_MONEY':'offsetof(struct SaveBlock1,money)',
 'PLAYER_PREVENT_STEP':'offsetof(struct PlayerAvatar,preventStep)',
 'SAVE1_OBJECT_TEMPLATES':'offsetof(struct SaveBlock1,objectEventTemplates)',
 'TEMPLATE_SIZE':'sizeof(struct ObjectEventTemplate)',
 'TEMPLATE_GRAPHICS':'offsetof(struct ObjectEventTemplate,graphicsId)',
 'OBJECT_COUNT':'OBJECT_EVENTS_COUNT',
 'CARE_SIZE':'sizeof(struct DayCare)',
 'CARE_MON_SIZE':'sizeof(struct DaycareMon)',
 'BOX_SIZE':'sizeof(struct BoxPokemon)',
 'CARE_STEPS':'offsetof(struct DaycareMon,steps)',
 'CARE_MAIL':'offsetof(struct DaycareMon,mail)',
 'CARE_PENDING':'offsetof(struct DayCare,offspringPersonality)',
 'CARE_COUNTER':'offsetof(struct DayCare,stepCounter)',
 'SAVE1_CARE':'offsetof(struct SaveBlock1,daycare)',
 'SAVE1_ROUTE5':'offsetof(struct SaveBlock1,route5Daycare)',
 'SAVE1_LEGACY_CARE':'offsetof(struct SaveBlock1,route5DayCareMon)',
 'R5_CARE':'offsetof(struct Route5DaycareSave,daycare)',
 'R5_PID':'offsetof(struct Route5DaycareSave,eggPersonality)',
 'R5_OT':'offsetof(struct Route5DaycareSave,eggOtId)',
 'R5_SHINY':'offsetof(struct Route5DaycareSave,eggShiny)',
 'SAVE2_OTID':'offsetof(struct SaveBlock2,playerTrainerId)',
 'PARTY_MENU_SLOT':'offsetof(struct PartyMenu,slotId)',
 'MAIL_SIZE':'sizeof(struct Mail)',
 'SAVE1_MAIL':'offsetof(struct SaveBlock1,mail)',
 'MAIL_ITEM':'offsetof(struct Mail,itemId)',
 'MAIL_WORDS':'offsetof(struct Mail,words)',
 'RESULT_CAUGHT':'offsetof(struct BattleResults,caughtMonSpecies)',
}.items(): expressions[key]=expr
for name in ['STATUS','PP_BONUSES','POKEBALL','SPECIES','HELD_ITEM','LEVEL','FRIENDSHIP','IS_EGG','ATK','DEF','HP','MAX_HP','MOVE1','MOVE2','MOVE3','MOVE4','PP1','PP2','PP3','PP4','NICKNAME','PERSONALITY','OT_NAME','OT_ID','HP_IV','SANITY_IS_BAD_EGG','ABILITY_NUM','MAIL','UNUSED_RIBBONS']:
    expressions['DATA_'+name] = 'MON_DATA_'+name
required = [
    "SetSavedWeather", "CB2_EndWildBattle", "CB2_EndTrainerBattle", "DynamicWeatherForMap", "DynamicWeatherForBattle", "DynamicWeatherAfterBattle", "DynamicWeatherUpdate", "WeatherContextValid", "GetCurrentWeather", "gWeatherPtr", "gMapHeader", "StartWildBattle",
    "gLevelUpLearnsets", "OakOldManHandleInputChooseMove",
    "PalletTown_ProfessorOaksLab_EventScript_RivalBattleSquirtle",
    "Route22_EventScript_EarlyRivalSquirtle", "Route22_EventScript_LateRivalSquirtle",
    "CeruleanCity_EventScript_RivalSquirtle", "SSAnne_2F_Corridor_EventScript_RivalSquirtle",
    "PokemonTower_2F_EventScript_RivalSquirtle", "SilphCo_7F_EventScript_RivalSquirtle",
    "PokemonLeague_ChampionsRoom_EventScript_BattleSquirtle",
    'BattleMainCB2', 'gBattleMoves', 'gTrainerBattleOpponent_A', 'CreateNPCTrainerParty', 'ApplyRangerTrainerData', 'RangerGiveGift', 'RangerNeedsMapRefresh',
    'SaffronCity_Dojo_EventScript_Ranger1', 'SaffronCity_Dojo_EventScript_Ranger2',
    'SaffronCity_Dojo_EventScript_Ranger3', 'SaffronCity_Dojo_EventScript_Ranger4',
    'SaffronCity_Dojo_EventScript_RangerPark', 'SaffronCity_Dojo_EventScript_RangerGift0',
    'SaffronCity_Dojo_EventScript_RangerGift1', 'SaffronCity_Dojo_EventScript_RangerGift2',
    'SaffronCity_Dojo_Text_ParkLocked', 'SaffronCity_Dojo_Text_ParkNo',
    'SaffronCity_Dojo_Text_ParkBeforeGift', 'SaffronCity_Dojo_Text_ParkAfterGift',
    'SaffronCity_Dojo_Text_GiftLocked', 'SaffronCity_Dojo_Text_GiftFull',
    'SaffronCity_Dojo_Text_GiftTaken', 'Cmd_friendshiptodamagecalculation', 'gBattleResources',
    'Task_PCMainMenu', 'Task_PokeStorageMain', 'Task_OnSelectedMon', 'Task_DepositMenu', 'Task_OnBPressed',
    'SetLastHealLocationWarp',

    "CheckBagHasItem", "RemoveBagItem", "Task_ShopMenu", "Task_BuyMenu", "Task_BuyHowManyDialogueHandleInput", "Task_CallYesOrNoCallback", "FindTaskIdByFunc", "VermilionCity_Mart_EventScript_Clerk", "gWishFutureKnock", "Cmd_setrain", "Cmd_setsandstorm", "AbilityBattleEffects", "CastformDataTypeChange", "Cmd_sethail", "Cmd_setsunny", "GetBallCatchMultiplier", "IsPokeBallItem", "BallItemToCaptureCode", "CaptureCodeToBallItem", "TryBuyBallsWithPremierBonus", "ItemIdToBallId", "gCaptureOrigin",
    "PalletTown_ProfessorOaksLab_EventScript_ProfOak", "PalletTown_ProfessorOaksLab_EventScript_TryGiveNestBalls",

    'gSpeciesNames',
    'gStringVar4',
    'MysteryVendor_Text_Intro10',
    'MysteryVendor_Text_Intro09',
    'MysteryVendor_Text_Intro08',
    'MysteryVendor_Text_Intro07',
    'MysteryVendor_Text_Intro06',
    'MysteryVendor_Text_Intro05',
    'MysteryVendor_Text_Intro04',
    'MysteryVendor_Text_Intro03',
    'MysteryVendor_Text_Intro02',
    'MysteryVendor_Text_Intro01',
    'MysteryVendor_Text_AlreadySold',
    'MysteryVendor_Text_NoRoom',
    'MysteryVendor_Text_Poor',
    'MysteryVendor_Text_Declined',
    'MysteryVendor_Text_Reveal',
    'OneIsland_EventScript_BaldingMan',
    'CinnabarIsland_EventScript_OldMan',
    'SaffronCity_EventScript_WorkerM',
    'FuchsiaCity_EventScript_OldMan',
    'CeladonCity_EventScript_OldMan2',
    'LavenderTown_EventScript_WorkerM',
    'Route10_PokemonCenter_1F_EventScript_FatMan',
    'VermilionCity_EventScript_Woman',
    'CeruleanCity_House1_EventScript_BadgeGuy',
    'Route4_PokemonCenter_1F_EventScript_MagikarpSalesman',
    'CB2_NamingScreen', 'sNamingScreen', 'gWindows', 'sMoneyBoxWindowId',
    'IsMonDisobedient', 'Cmd_getexp', 'BufferStringBattle', 'gBattleControllerExecFlags',
    'gBattlerPartyIndexes', 'gBattleTextBuff2', 'gCurrMovePos',
    'ZeroBoxMonAt',
    'BoxMonToMon', 'GetBoxedMonPtr', 'PurchaseMysteryPokemon', 'MysteryVendorStatus', 'MysteryRandomBelow', 'RocketRescueStatus', 'RocketRescueGive', 'RocketRescueReturn', 'gSpecialVar_0x8009',
    'IsTradedMon', 'FlagClear', 'ResetPokemonStorageSystem', 'GetAndCopyBoxMonDataAt',
    'gSpecialVar_MonBoxId', 'gSpecialVar_MonBoxPos',
    'IsSpeciesInCustomPokedex', 'WaitForAorBPress', 'sTextPrinters', 'gSpecialVar_0x8006', 'FlagGet', 'VarGet', 'FuncIsActiveTask', 'CheckBagHasSpace', 'ItemId_GetPocket',
    'Task_MultichoiceMenu_HandleInput', 'Task_YesNoMenu_HandleInput',
    'Route10_PokemonCenter_1F_EventScript_Aide',
    'CinnabarIsland_PokemonLab_ExperimentRoom_EventScript_FossilScientist',
    'AddBagItem', 'BagGetQuantityByItemId', 'BeginItemEvolution', 'CB2_LoadMap', 'CB2_OpenPokedexFromStartMenu',
    'CB2_Overworld', 'CastformDataTypeChange', 'ClearBag', 'CreateMon', 'CustomPokedexNumToSpecies',
    'DexScreen_CountMonsInOrderedList', 'DisableNationalPokedex', 'EnableNationalPokedex', 'EvolutionScene', 'FlagSet',
    'GetBoxMonDataAt', 'GetBoxNamePtr', 'GetCurrentMapWildMonHeaderId', 'GetCustomPokedexCount', 'GetEvolutionTargetSpecies',
    'GetMonAbility', 'GetMonData3', 'GetSav1Weather', 'GetSetPokedexFlag', 'GetWildCatchTarget',
    'GetWildOpponentCount', 'HandleInputChooseAction', 'HandleInputChooseMove', 'HandleInputChooseTarget', 'HasAllCustomPokedexSpecies',
    'HasAllMons', 'InitRoamer', 'IsRunningFromBattleImpossible', 'ListMenuGetScrollAndRow', 'LoadGameSave',
    'MapGridGetMetatileAttributeAt', 'Menu_GetCursorPos', 'Menu_MoveCursorNoWrapAround', 'OpponentHandleChooseMove', 'PlayerGetDestCoords',
    'ResetInitialPlayerAvatarState', 'ResetTasks', 'GetTimeOfDay', 'TryGetTimeOfDay', 'RtcGetHour', 'Save_ResetSaveCounters', 'SetBagPocketsPointers',
    'SetControllerToOpponent', 'SetControllerToPlayer', 'SetMonData', 'SetWarpDestination', 'SetWeather',
    'SpeciesToCustomPokedexNum', 'SpeciesToNationalPokedexNum', 'SpeciesToPokedexNum', 'StandardWildEncounter', 'SweetScentWildEncounter',
    'TryDoDoubleWildBattle', 'TrySavingData', 'TryStartLandEncounter', 'VarSet', 'WarpIntoMap',
    'ZeroPlayerPartyMons', 'gActionSelectionCursor', 'gActiveBattler', 'gBagMenuState', 'gBattleBufferA',
    'gBattleBufferB', 'gBattleMons', 'gBattleOutcome', 'gBattleResults', 'gBattleTypeFlags',
    'gBattleWeather', 'gBattlerControllerFuncs', 'gBattlersCount', 'gBattlescriptCurrInstr', 'gCB2_AfterEvolution',
    'gEnemyParty', 'gFlashMemoryPresent', 'gMain', 'gMoveSelectionCursor', 'gMultiUsePlayerCursor',
    'gPaletteFade', 'gPlayerParty', 'gPlayerPartyCount', 'gPokemonStorage', 'gPokemonStoragePtr',
    'gRngValue', 'gSaveBlock1', 'gSaveBlock1Ptr', 'gSaveBlock2', 'gSaveBlock2Ptr',
    'gTasks', 'gWildMonHeaders', 'sEvoStructPtr', 'sPlttBufferTransferPending', 'sPokedexScreenData',
    'sEggHatchData', 'sGlobalScriptContext', 'sGlobalScriptContextStatus', 'EggHatch', 'CB2_EggHatch_0', 'CB2_EggHatch_1',
    'CB2_UpdatePartyMenu', 'SetMoney', 'GetMoney', 'GetGameStat', 'SetGameStat',
    'ArePlayerFieldControlsLocked', 'gPlayerAvatar', 'gQuestLogState',
    'CB2_ContinueSavedGame', 'Route5DaycareNeedsMapRefresh', 'RemoveObjectEventByLocalIdAndMap', 'GetObjectEventIdByLocalIdAndMap',
    'GetRoute5Daycare', 'GetActiveDaycare', 'IsRoute5DaycareReady', 'TriggerPendingDaycareEgg',
    'SetRoute5PendingEgg', 'GiveEggFromDaycare', 'RejectEggFromDayCare', 'ShouldEggHatch',
    'StoreSelectedPokemonInDaycare', 'StorePokemonInDaycare', 'TakePokemonFromDaycare',
    'GetDaycareCost', 'GetNumLevelsGainedFromDaycare', 'GetDaycareState', 'GetDaycarePokemonCount',
    'GetDaycareCompatibilityScore', 'GiveMailToMon', 'GetCursorSelectionMonId', 'GetMonGender',
    'IsMonShiny', 'ScriptHatchMon', 'CreateEgg', 'MapGridGetCollisionAt', 'MapGridGetElevationAt',
    'ScriptContext_SetupScript', 'ScriptContext_IsEnabled', 'EventScript_EggHatch',
    'Route5_PokemonDayCare_EventScript_DaycareWoman', 'Route5_PokemonDayCare_Egg_DaycareMan',
    'FourIsland_PokemonDayCare_EventScript_DaycareWoman', 'FourIsland_EventScript_DaycareMan',
    'gSpecialVar_0x8004', 'gSpecialVar_0x8005', 'gSpecialVar_Result', 'gPartyMenu', 'gSelectedObjectEvent',
    'sRoamerLocation', 'IsNationalPokedexEnabled', 'CopyPlayerPartyMonToBattleData', 'CalculateBaseDamage', 'TypeCalc', 'AI_TypeCalc', 'AI_CalcDmg', 'Cmd_dmgtolevel',
    'CinnabarIsland_Gym_EventScript_Blaine', 'CreateShinyEgg', 'ScriptGiveMuseumAronEgg', 'PewterCity_Museum_1F_EventScript_TriviaEmployee', 'PewterCity_Museum_1F_Text_TriviaOffer',
    'GetMoveCategoryName', 'GetMoveCategoryShortName', 'MoveSelectionDisplayMoveType',
    'gCurrentMove', 'gCritMultiplier', 'gDynamicBasePower', 'gBattlerAttacker', 'gBattlerTarget',
    'ShowPokemonSummaryScreen', 'sMonSummaryScreen', 'sMoveSelectionCursorPos', 'gSideStatuses', 'gBattleStruct', 'gBattleScripting', 'gBattleMoveDamage', 'gDisplayedStringBattle',
]
required += ['NextThunderWaveTarget','gAbsentBattlerFlags']
required += ['Cmd_rolloutdamagecalculation','gDisableStructs','gLockedMoves','gLastMoves']
required += ['BattleAI_SetupAIData','BattleAI_ChooseMoveOrAction','RecordAbilityBattle']
required += ['Cmd_weatherdamage','Cmd_accuracycheck','gStatuses3','gHitMarker','gMoveResultFlags']
required += ['ViridianCity_EventScript_TrainerTips1','ViridianCity_EventScript_TrainerTips2','ViridianCity_Text_CatchMonsForEasierBattles','ViridianCity_Text_MovesLimitedByPP']

required += ['CreateHalfShinyGiftEgg','ScriptGiveMagbyEgg','ScriptGiveTangoTangela','FiveIsland_WaterLabyrinth_EventScript_EggGentleman','CeladonCity_Condominiums_RoofRoom_EventScript_EeveeBall']

required += ['PokemonLeague_LoreleisRoom_EventScript_Lorelei','PokemonLeague_BrunosRoom_EventScript_Bruno','PokemonLeague_AgathasRoom_EventScript_Agatha','PokemonLeague_LancesRoom_EventScript_Lance']
required += ['PewterCity_Gym_EventScript_Liam','CeruleanCity_Gym_EventScript_Diana','CeladonCity_Gym_EventScript_Mary','Cmd_setweatherballtype','gBattleMonForms','SaffronCity_Dojo_Text_NeedTwo']
required += ['MonRestorePP','ExecuteTableBasedItemEffect']
required += ['ClampApprovedMovePP','ClampApprovedDaycarePP','NormalizeLoadedMovePP','GetBoxMonData3','SetBoxMonData','SavePlayerParty','gSaveCounter']
required += ['CalculatePPWithBonus','SetMonMoveSlot']
required += ['GetWindowAttribute','MoveSelectionDisplayMoveNames','MoveSelectionDisplayPpString','MoveSelectionDisplayPpNumber','GetMoveTypeShortName','HandleMoveSwitching']
required += ['TeachMoveRelearnerMove','CB2_MoveRelearner','GetBgTilemapBuffer']
required += ['gMoveNames','GetStringWidth','StringCopy','AllocPSA','Free','AllocZeroed','sMoveRelearner','MoveRelearnerInitListMenuBuffersEtc','BufferStringBattle','gBattleTextBuff2','gBattleTextBuff3']
required += ['LightingPreviewGetAppearance', 'LightingPreviewIsActive']  # preview-branch native assertions
required += ['Task_StartMenuHandleInput','sStartMenuCursorPos','sStartMenuOrder','Task_BagMenu_HandleInput','Task_HandleChooseMonInput','Task_EvolutionScene']
symbols={}
end_ewram=0
for line in subprocess.check_output(['arm-none-eabi-nm','-S','pokefirered.elf'],text=True).splitlines():
    f=line.split()
    if len(f)==4 and f[2] in 'bBdD' and 0x02000000<=int(f[0],16)<0x02040000:
        end_ewram=max(end_ewram,int(f[0],16)+int(f[1],16))
assert end_ewram<0x0203FD00, 'No safe test-stack space at end of EWRAM'
for line in subprocess.check_output(['arm-none-eabi-nm','-n','pokefirered.elf'],text=True).splitlines():
    fields=line.split()
    if len(fields)==3 and fields[2] in required:
        symbols.setdefault(fields[2],int(fields[0],16)) # first matching local symbol (player controller precedes tutorial controllers)
assert set(required)<=symbols.keys(), set(required)-symbols.keys()
# Private input function names also exist in tutorial controllers. Verify the
# selected local symbols belong to the actual player controller's address range.
for name in ['HandleInputChooseAction','HandleInputChooseMove','HandleInputChooseTarget','MoveSelectionDisplayMoveNames','MoveSelectionDisplayMoveType','MoveSelectionDisplayPpNumber','HandleMoveSwitching']:
    assert symbols['SetControllerToPlayer'] < symbols[name] < symbols['SetControllerToOpponent'], name
with tempfile.TemporaryDirectory(prefix='lightning-emulator-') as temp:
    temp=Path(temp)
    source='#include "save.h"\n#include "field_weather.h"\n#include <stddef.h>\n#include "global.h"\n#include "pokemon.h"\n#include "main.h"\n#include "task.h"\n'
    source+='#include "text.h"\n#include "pokedex.h"\n#include "battle_controllers.h"\n#include "item_menu.h"\n#include "wild_encounter.h"\n#include "battle.h"\n#include "list_menu.h"\n#include "party_menu.h"\n#include "script.h"\n#include "field_player_avatar.h"\n'+screen_struct+'\n'+summary_struct+'\n'
    learn=Path('src/learn_move.c').read_text();learn=learn[learn.index('struct LearnMoveGfxResources\n'):];learn=learn[:learn.index('\n};')+3]
    source+='#include "battle_message.h"\n#include "pokemon_special_anim_internal.h"\n'+learn+'\n'
    source+='const unsigned long abi[] = {'+', '.join(expressions.values())+'};\n'
    (temp/'abi.c').write_text(source)
    with (temp/'abi.i').open('w') as out:
        subprocess.run(['gcc','-E','-nostdinc','-undef','-iquote','include','-I','tools/agbcc/include',
                        '-I','tools/agbcc',str(temp/'abi.c')],stdout=out,check=True)
    subprocess.run(['tools/agbcc/bin/agbcc','-O2','-o',str(temp/'abi.s'),str(temp/'abi.i')],check=True)
    subprocess.run(['arm-none-eabi-as','-mcpu=arm7tdmi','-o',str(temp/'abi.o'),str(temp/'abi.s')],check=True)
    subprocess.run(['arm-none-eabi-objcopy','-O','binary','-j','.rodata',str(temp/'abi.o'),str(temp/'abi.bin')],check=True)
    values=struct.unpack('<'+'I'*len(expressions),(temp/'abi.bin').read_bytes())
    header='\n'.join('#define ABI_'+name+' '+str(value) for name,value in zip(expressions,values))+'\n'
    scene_source=Path('src/evolution_scene.c').read_text()
    for group in re.findall(r'enum\s*\{(.*?)\};',scene_source,re.S):
        names=re.findall(r'\b(?:EVOSTATE|MVSTATE)_\w+',group)
        if names:
            assert len(names)==len(set(names))
            header+=''.join('#define E_'+name+' '+str(i)+'\n' for i,name in enumerate(names))
    header+='\n'.join('#define SYM_'+name+' 0x%08xU'%value for name,value in symbols.items())+'\n'
    header+='#define ABI_EWRAM_END 0x%08xU\n'%end_ewram
    (temp/'symbols.h').write_text(header)
    gf=json.loads(Path('tests/specifications/approved_gym_castform.json').read_text())
    out=''
    for label,field in [('Species',0),('Levels',1),('Moves',2)]:
        rows=[]
        for t in gf['trainers']:
            vals=[]
            for mon in t['pokemon']:
                value=mon[field]
                vals.append(('{'+','.join('MOVE_'+x for x in value)+'}') if field==2 else ('SPECIES_'+value if field==0 else str(value)))
            rows.append('{'+','.join(vals)+'}')
        out+='static const unsigned gymExpected'+label+'[3][2]'+('[4]' if field==2 else '')+'={'+','.join(rows)+'};\n'
    (temp/'gym_castform_expected.h').write_text(out)
    approved_rangers=json.loads(Path('tests/specifications/approved_ranger_base_camp.json').read_text())
    expected='static const unsigned rangerExpectedMoves[5][4][4] = {\n'
    for trainer in approved_rangers['trainers']:
        expected+=' {'+','.join('{'+','.join('MOVE_'+m for m in mon['moves'])+'}' for mon in trainer['party'])+'},\n'
    expected+='};\n'
    (temp/'ranger_expected.h').write_text(expected)

    wf=json.loads(Path('tests/specifications/approved_weather_maps.json').read_text())
    expected='struct WeatherExpected {unsigned map,original,weights[5];};\nstatic const struct WeatherExpected weatherExpected[]={\n'
    for row in wf['maps']:
        expected+=' {'+row['map']+','+row['native_weather']+',{'+','.join(map(str,row['weights']))+'}},\n'
    expected+='};\n'
    (temp/'weather_expected.h').write_text(expected)
    rivals=json.loads(Path('tests/specifications/approved_rival_parties.json').read_text())
    expected='static const unsigned rivalSizes[8]={'+','.join(str(len(e['party'])) for e in rivals['encounters'])+'};\n'
    expected+='static const unsigned rivalIds[8][3]={'+','.join('{'+','.join(v['trainer'] for v in e['variants'])+'}' for e in rivals['encounters'])+'};\n'
    expected+='struct RivalExpected {unsigned species,level,item,iv,moves[4];};\nstatic const struct RivalExpected rivalMons[8][6]={\n'
    for e in rivals['encounters']:
        expected+=' {'+','.join('{SPECIES_'+m['species']+','+str(m['level'])+',ITEM_'+m['held_item']+','+str(e['variants'][0]['iv'][i])+',{'+','.join('MOVE_'+v for v in m['moves'])+'}}' for i,m in enumerate(e['party']))+'},\n'
    expected+='};\n'
    (temp/'rival_expected.h').write_text(expected)


    # Generate strict native-ROM expectations from the authorized 510-row lock.
    # Placeholder-only source records intentionally are not constructed as foes.
    ordinary = json.loads(Path('docs/ordinary-trainers-implementation-lock.json').read_text())['entries']
    ordinary_active = [(name, row) for name, row in ordinary.items() if not row['source_dummy']]
    assert len(ordinary_active) == 500
    ordinary_mons = [mon for _, row in ordinary_active for mon in row['mons']]
    assert len(ordinary_mons) == 1219
    expected = '#define ORDINARY_TRAINER_COUNT 500\n#define ORDINARY_TRAINER_MON_COUNT 1219\n'
    expected += 'static const unsigned ordinaryTrainerIds[] = {' + ','.join(name for name, _ in ordinary_active) + '};\n'
    expected += 'static const unsigned ordinaryTrainerSizes[] = {' + ','.join(str(len(row['mons'])) for _, row in ordinary_active) + '};\n'
    expected += 'struct OrdinaryExpected {unsigned species,level,item,iv,moves[4];};\n'
    expected += 'static const struct OrdinaryExpected ordinaryTrainerMons[] = {\n'
    for mon in ordinary_mons:
        move_list = mon['moves'] + ['MOVE_NONE'] * (4 - len(mon['moves']))
        expected += ' {' + ','.join([
            mon['species'], str(mon['level']), mon['held_item'] or 'ITEM_NONE',
            str(mon['iv']), '{' + ','.join(move_list) + '}']) + '},\n'
    expected += '};\n'
    (temp / 'ordinary_trainers_expected.h').write_text(expected)

    command=['gcc','-std=gnu11','-O2','-Wall','-Wextra','-I',str(temp),'-iquote','include',
             'tests/emulator/runner.c','-o',str(temp/'runner')]
    # Local workspace dependency extraction; Actions installs libmgba-dev normally.
    prefix=os.environ.get('MGBA_PREFIX')
    if prefix:
        command+=['-I',prefix+'/usr/include','-L',prefix+'/usr/lib/x86_64-linux-gnu',
                  '-Wl,-rpath,'+prefix+'/usr/lib/x86_64-linux-gnu']
    command+=['-lmgba']
    subprocess.run(command,check=True)
    shutil.copyfile('pokefirered.gba',temp/'test.gba')
    subprocess.run([str(temp/'runner'),str(temp/'test.gba')],check=True,env={**os.environ,'TZ':'UTC'})
