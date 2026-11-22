#include "global.h"
#include "battle_tower.h"
#include "random.h"
#include "text.h"
#include "event_data.h"
#include "string_util.h"
#include "battle_message.h"
#include "data.h"
#include "pokedex.h"
#include "overworld.h"
#include "battle_transition.h"
#include "easy_chat.h"
#include "battle_setup.h"
#include "battle_main.h"
#include "task.h"
#include "battle.h"
#include "party_menu.h"
#include "new_game.h"
#include "save.h"
#include "item.h"
#include "script_pokemon_util.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/event_objects.h"
#include "constants/trainers.h"
#include "constants/easy_chat.h"

static EWRAM_DATA u16 sSpecialVar_0x8004_Copy = 0;
static EWRAM_DATA struct Pokemon sSevenIslandBattleTowerOriginalParty[PARTY_SIZE];
static EWRAM_DATA u8 sSevenIslandBattleTowerOriginalPartyCount;
static EWRAM_DATA bool8 sSevenIslandBattleTowerPartyBackedUp = FALSE;

// Runtime-only preservation for the new Seven Island reception. Do not
// substitute SavePlayerParty/LoadPlayerParty: those operate on serialized
// save data and can overwrite the normal player's saved party.
void BackupSevenIslandBattleTowerParty(void)
{
    u8 i;

    // Do not overwrite an outstanding snapshot.
    if (sSevenIslandBattleTowerPartyBackedUp)
    {
        gSpecialVar_Result = FALSE;
        return;
    }

    sSevenIslandBattleTowerOriginalPartyCount = gPlayerPartyCount;
    for (i = 0; i < PARTY_SIZE; i++)
        sSevenIslandBattleTowerOriginalParty[i] = gPlayerParty[i];

    sSevenIslandBattleTowerPartyBackedUp = TRUE;
    gSpecialVar_Result = TRUE;
}

void RestoreSevenIslandBattleTowerParty(void)
{
    u8 i;

    if (!sSevenIslandBattleTowerPartyBackedUp)
    {
        gSpecialVar_Result = FALSE;
        return;
    }

    for (i = 0; i < PARTY_SIZE; i++)
        gPlayerParty[i] = sSevenIslandBattleTowerOriginalParty[i];
    gPlayerPartyCount = sSevenIslandBattleTowerOriginalPartyCount;
    sSevenIslandBattleTowerPartyBackedUp = FALSE;
    gSpecialVar_Result = TRUE;
}



#define TakeBravoTrainerBattleTowerOffTheAir()
#define ewram160FB (*((u8 *)&gBattleStruct->field_DA))

static void ResetBattleTowerStreak(u8 levelType);
static void SetBattleTowerTrainerGfxId(u8 trainerClass);
static void CheckMonBattleTowerBanlist(u16 species, u16 heldItem, u16 hp, u8 battleTowerLevelType, u8 monLevel, u16 *validPartySpecies, u16 *validPartyHeldItems, u8 *numValid);
static void SaveCurrentWinStreak(void);
static void ValidateBattleTowerRecordChecksums(void);
static void SetBattleTowerRecordChecksum(struct BattleTowerRecord * record);
static void ClearBattleTowerRecord(struct BattleTowerRecord * record);
static void PopulateBravoTrainerBattleTowerLostData(void);
static u16 GetCurrentBattleTowerWinStreak(u8 levelType);
static void SetEReaderTrainerChecksum(struct BattleTowerEReaderTrainer * eReaderTrainer);
static void PrintEReaderTrainerFarewellMessage(void);

// unknown unused data
static const u8 sUnused[] = {
    5, 4, 1, 16, 4, 2, 5, 6,
    3, 16, 6, 4, 0, 1, 2, 3,
    0, 2, 3
};

static const u8 sText_100[] = _("100");

// Newly authored Lightning Yellow roster. IDs 0-99 are normal trainers;
// 100-199 are reserved for record mixing, and 200 for e-Reader.
static const struct BattleTowerTrainer sBattleTowerTrainers[] =
{
    // Normal opponent 0; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_CAMPER, .name = _("ACE"), .teamFlags = 0x01, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 1; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PICNICKER, .name = _("ADA"), .teamFlags = 0x02, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 2; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_CAMPER, .name = _("AMY"), .teamFlags = 0x01, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 3; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PICNICKER, .name = _("ANN"), .teamFlags = 0x02, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 4; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_CAMPER, .name = _("ASH"), .teamFlags = 0x01, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 5; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PICNICKER, .name = _("BEN"), .teamFlags = 0x02, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 6; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_CAMPER, .name = _("BOB"), .teamFlags = 0x01, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 7; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_YOUNGSTER, .name = _("CAL"), .teamFlags = 0x02, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 8; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PICNICKER, .name = _("CAM"), .teamFlags = 0x01, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 9; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_YOUNGSTER, .name = _("DAN"), .teamFlags = 0x02, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 10; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PICNICKER, .name = _("DEB"), .teamFlags = 0x01, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 11; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_YOUNGSTER, .name = _("DEE"), .teamFlags = 0x02, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 12; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PICNICKER, .name = _("DON"), .teamFlags = 0x01, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 13; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_YOUNGSTER, .name = _("ELI"), .teamFlags = 0x02, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 14; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_YOUNGSTER, .name = _("EVA"), .teamFlags = 0x01, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 15; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BIRD_KEEPER, .name = _("FAY"), .teamFlags = 0x02, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 16; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_YOUNGSTER, .name = _("FIN"), .teamFlags = 0x01, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 17; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BIRD_KEEPER, .name = _("GAB"), .teamFlags = 0x02, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 18; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_YOUNGSTER, .name = _("GIA"), .teamFlags = 0x01, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 19; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BIRD_KEEPER, .name = _("GUS"), .teamFlags = 0x02, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 20; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_YOUNGSTER, .name = _("HAL"), .teamFlags = 0x40, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 21; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_M, .name = _("IAN"), .teamFlags = 0x01, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 22; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BIRD_KEEPER, .name = _("IDA"), .teamFlags = 0x02, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 23; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_M, .name = _("IKE"), .teamFlags = 0x40, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 24; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BIRD_KEEPER, .name = _("IVY"), .teamFlags = 0x01, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 25; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_M, .name = _("JAY"), .teamFlags = 0x02, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 26; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BIRD_KEEPER, .name = _("JEN"), .teamFlags = 0x40, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 27; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_M, .name = _("JIM"), .teamFlags = 0x01, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 28; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_M, .name = _("JOE"), .teamFlags = 0x02, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 29; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_F, .name = _("JOY"), .teamFlags = 0x40, .greeting = { EC_WORD_HELLO, EC_WORD_I, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_READY, EC_WORD_GO } },
    // Normal opponent 30; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_M, .name = _("KAI"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 31; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_F, .name = _("KEN"), .teamFlags = 0x02, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 32; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_M, .name = _("KIM"), .teamFlags = 0x40, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 33; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_F, .name = _("KIT"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 34; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_M, .name = _("LEO"), .teamFlags = 0x02, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 35; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_M, .name = _("LIA"), .teamFlags = 0x40, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 36; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_F, .name = _("LIZ"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 37; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_M, .name = _("LOU"), .teamFlags = 0x02, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 38; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_F, .name = _("LUC"), .teamFlags = 0x40, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 39; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_M, .name = _("LYN"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 40; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_COOLTRAINER_F, .name = _("MAC"), .teamFlags = 0x02, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 41; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_M, .name = _("MAX"), .teamFlags = 0x40, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 42; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_M, .name = _("MAY"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 43; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_F, .name = _("MEG"), .teamFlags = 0x02, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 44; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_M, .name = _("MIA"), .teamFlags = 0x40, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 45; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_F, .name = _("MIK"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 46; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_M, .name = _("MOE"), .teamFlags = 0x02, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 47; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_F, .name = _("NAT"), .teamFlags = 0x40, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 48; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_M, .name = _("NED"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 49; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BLACK_BELT, .name = _("NIA"), .teamFlags = 0x02, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 50; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_F, .name = _("NIK"), .teamFlags = 0x40, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 51; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BLACK_BELT, .name = _("NOA"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 52; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_F, .name = _("OLI"), .teamFlags = 0x02, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 53; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BLACK_BELT, .name = _("ORA"), .teamFlags = 0x40, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 54; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_PSYCHIC_F, .name = _("PAM"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 55; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BLACK_BELT, .name = _("PAT"), .teamFlags = 0x02, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 56; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BLACK_BELT, .name = _("PAZ"), .teamFlags = 0x40, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 57; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_BATTLE_GIRL, .name = _("PEG"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 58; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BLACK_BELT, .name = _("PET"), .teamFlags = 0x02, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 59; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_BATTLE_GIRL, .name = _("PIA"), .teamFlags = 0x40, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 60; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BLACK_BELT, .name = _("RAE"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 61; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_BATTLE_GIRL, .name = _("RAY"), .teamFlags = 0x02, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 62; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BLACK_BELT, .name = _("REX"), .teamFlags = 0x40, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 63; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_GENTLEMAN, .name = _("RIA"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 64; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_BATTLE_GIRL, .name = _("RIC"), .teamFlags = 0x02, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 65; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_GENTLEMAN, .name = _("RIO"), .teamFlags = 0x40, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 66; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_BATTLE_GIRL, .name = _("ROB"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 67; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_GENTLEMAN, .name = _("RON"), .teamFlags = 0x02, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 68; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_BATTLE_GIRL, .name = _("ROY"), .teamFlags = 0x40, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 69; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_GENTLEMAN, .name = _("RUE"), .teamFlags = 0x01, .greeting = { EC_WORD_READY, EC_WORD_BATTLE, EC_WORD_YOU, EC_WORD_I, EC_WORD_GO, EC_WORD_BATTLE } },
    // Normal opponent 70; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_GENTLEMAN, .name = _("SAM"), .teamFlags = 0x02, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 71; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BEAUTY, .name = _("SID"), .teamFlags = 0x40, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 72; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_GENTLEMAN, .name = _("SKY"), .teamFlags = 0x01, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 73; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BEAUTY, .name = _("SOL"), .teamFlags = 0x02, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 74; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_GENTLEMAN, .name = _("SUE"), .teamFlags = 0x40, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 75; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BEAUTY, .name = _("TAJ"), .teamFlags = 0x01, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 76; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_GENTLEMAN, .name = _("TAM"), .teamFlags = 0x02, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 77; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_RUIN_MANIAC, .name = _("TED"), .teamFlags = 0x40, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 78; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BEAUTY, .name = _("TIA"), .teamFlags = 0x01, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 79; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_RUIN_MANIAC, .name = _("TIM"), .teamFlags = 0x02, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 80; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BEAUTY, .name = _("TOM"), .teamFlags = 0x40, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 81; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_RUIN_MANIAC, .name = _("TOS"), .teamFlags = 0x01, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 82; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_BEAUTY, .name = _("UMA"), .teamFlags = 0x02, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 83; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_RUIN_MANIAC, .name = _("VAL"), .teamFlags = 0x40, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 84; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_RUIN_MANIAC, .name = _("VAN"), .teamFlags = 0x01, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 85; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_DRAGON_TAMER, .name = _("VIC"), .teamFlags = 0x02, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 86; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_RUIN_MANIAC, .name = _("VIN"), .teamFlags = 0x40, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 87; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_DRAGON_TAMER, .name = _("WES"), .teamFlags = 0x01, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 88; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_RUIN_MANIAC, .name = _("WIL"), .teamFlags = 0x02, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 89; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_DRAGON_TAMER, .name = _("WIN"), .teamFlags = 0x40, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 90; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_RS_RUIN_MANIAC, .name = _("YAN"), .teamFlags = 0x01, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 91; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_EXPERT_M, .name = _("YUI"), .teamFlags = 0x02, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 92; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_DRAGON_TAMER, .name = _("ZAC"), .teamFlags = 0x40, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 93; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_EXPERT_M, .name = _("ZED"), .teamFlags = 0x01, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 94; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_DRAGON_TAMER, .name = _("ZEN"), .teamFlags = 0x02, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 95; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_EXPERT_M, .name = _("ZIA"), .teamFlags = 0x40, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 96; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_DRAGON_TAMER, .name = _("ZOE"), .teamFlags = 0x01, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 97; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_EXPERT_M, .name = _("ZOR"), .teamFlags = 0x02, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 98; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_EXPERT_M, .name = _("ARI"), .teamFlags = 0x40, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
    // Normal opponent 99; original FireRed trainer table was empty.
    { .trainerClass = FACILITY_CLASS_EXPERT_F, .name = _("BEA"), .teamFlags = 0x01, .greeting = { EC_WORD_BATTLE, EC_WORD_READY, EC_WORD_I, EC_WORD_GO, EC_WORD_YOU, EC_WORD_BATTLE } },
};

static const u16 sBattleTowerHeldItems[] = {
    ITEM_NONE,
    ITEM_KINGS_ROCK,
    ITEM_SITRUS_BERRY,
    ITEM_ORAN_BERRY,
    ITEM_CHESTO_BERRY,
    ITEM_HARD_STONE,
    ITEM_FOCUS_BAND,
    ITEM_PERSIM_BERRY,
    ITEM_MIRACLE_SEED,
    ITEM_BERRY_JUICE,
    ITEM_MACHO_BRACE,
    ITEM_SILVER_POWDER,
    ITEM_CHERI_BERRY,
    ITEM_BLACK_GLASSES,
    ITEM_BLACK_BELT,
    ITEM_SOUL_DEW,
    ITEM_CHOICE_BAND,
    ITEM_MAGNET,
    ITEM_SILK_SCARF,
    ITEM_WHITE_HERB,
    ITEM_DEEP_SEA_SCALE,
    ITEM_DEEP_SEA_TOOTH,
    ITEM_MYSTIC_WATER,
    ITEM_SHARP_BEAK,
    ITEM_QUICK_CLAW,
    ITEM_LEFTOVERS,
    ITEM_RAWST_BERRY,
    ITEM_LIGHT_BALL,
    ITEM_POISON_BARB,
    ITEM_NEVER_MELT_ICE,
    ITEM_ASPEAR_BERRY,
    ITEM_SPELL_TAG,
    ITEM_BRIGHT_POWDER,
    ITEM_LEPPA_BERRY,
    ITEM_SCOPE_LENS,
    ITEM_TWISTED_SPOON,
    ITEM_METAL_COAT,
    ITEM_MENTAL_HERB,
    ITEM_CHARCOAL,
    ITEM_PECHA_BERRY,
    ITEM_SOFT_SAND,
    ITEM_LUM_BERRY,
    ITEM_DRAGON_SCALE,
    ITEM_DRAGON_FANG,
    ITEM_IAPAPA_BERRY,
    ITEM_WIKI_BERRY,
    ITEM_SEA_INCENSE,
    ITEM_SHELL_BELL,
    ITEM_SALAC_BERRY,
    ITEM_LANSAT_BERRY,
    ITEM_APICOT_BERRY,
    ITEM_STARF_BERRY,
    ITEM_LIECHI_BERRY,
    ITEM_STICK,
    ITEM_LAX_INCENSE,
    ITEM_AGUAV_BERRY,
    ITEM_FIGY_BERRY,
    ITEM_THICK_CLUB,
    ITEM_MAGO_BERRY,
    ITEM_METAL_POWDER,
    ITEM_PETAYA_BERRY,
    ITEM_LUCKY_PUNCH,
    ITEM_GANLON_BERRY
};

#include "data/battle_tower/level_50_mons.h"
#include "data/battle_tower/level_100_mons.h"

// Reuse existing facility-class mappings for player records and e-Reader records.
// Empty arrays in the upstream source made the modulo selection undefined.
static const u8 sMaleTrainerClasses[] =
{
    FACILITY_CLASS_RS_CAMPER,
    FACILITY_CLASS_RS_YOUNGSTER,
    FACILITY_CLASS_RS_COOLTRAINER_M,
    FACILITY_CLASS_RS_BLACK_BELT,
    FACILITY_CLASS_RS_GENTLEMAN,
};

static const u8 sFemaleTrainerClasses[] =
{
    FACILITY_CLASS_RS_PICNICKER,
    FACILITY_CLASS_RS_COOLTRAINER_F,
    FACILITY_CLASS_RS_BEAUTY,
    FACILITY_CLASS_RS_PSYCHIC_F,
    FACILITY_CLASS_BATTLE_GIRL,
};

static const u8 sMaleTrainerGfx[] =
{

};

static const u8 sFemaleTrainerGfx[] =
{

};

const u16 gBattleTowerBannedSpecies[] = {
    SPECIES_MEW,
    SPECIES_MEWTWO,
    SPECIES_HO_OH,
    SPECIES_LUGIA,
    SPECIES_CELEBI,
    SPECIES_KYOGRE,
    SPECIES_GROUDON,
    SPECIES_RAYQUAZA,
    SPECIES_JIRACHI,
    SPECIES_DEOXYS,
    0xFFFF
};

static const u16 sShortStreakPrizes[] = {
    ITEM_HP_UP,
    ITEM_PROTEIN,
    ITEM_IRON,
    ITEM_CALCIUM,
    ITEM_CARBOS,
    ITEM_ZINC
};

static const u16 sLongStreakPrizes[] = {
    ITEM_BRIGHT_POWDER,
    ITEM_WHITE_HERB,
    ITEM_QUICK_CLAW,
    ITEM_LEFTOVERS,
    ITEM_MENTAL_HERB,
    ITEM_KINGS_ROCK,
    ITEM_FOCUS_BAND,
    ITEM_SCOPE_LENS,
    ITEM_CHOICE_BAND
};

void BattleTowerMapScript2(void)
{
    u8 count = 0;
    s32 levelType;

    for (levelType = 0; levelType < 2; levelType++)
    {
        switch (gSaveBlock2Ptr->battleTower.var_4AE[levelType])
        {
        default:
        case 0:
            ResetBattleTowerStreak(levelType);
            if (count == 0)
                VarSet(VAR_TEMP_0, 5);
            break;
        case 1:
            ResetBattleTowerStreak(levelType);
            VarSet(VAR_TEMP_0, BTSPECIAL_RESULT_SAVE_SCUM);
            count++;
            break;
        case 3:
            break;
        case 4:
            VarSet(VAR_TEMP_0, BTSPECIAL_RESULT_WON7);
            count++;
            break;
        case 5:
            VarSet(VAR_TEMP_0, BTSPECIAL_RESULT_LOST);
            count++;
            break;
        case 6:
            break;
        case 2:
            VarSet(VAR_TEMP_0, BTSPECIAL_RESULT_QUICKSAVE);
            count++;
            break;
        }
    }
    if (gSaveBlock2Ptr->battleTower.var_4AE[0] == 3 && gSaveBlock2Ptr->battleTower.var_4AE[1] == 3)
        VarSet(VAR_TEMP_0, BTSPECIAL_RESULT_INACTIVE);
    ValidateBattleTowerRecordChecksums();
}

static void ResetBattleTowerStreak(u8 levelType)
{
    gSaveBlock2Ptr->battleTower.var_4AE[levelType] = 0;
    gSaveBlock2Ptr->battleTower.curChallengeBattleNum[levelType] = 1;
    gSaveBlock2Ptr->battleTower.curStreakChallengesNum[levelType] = 1;
}

static bool8 ShouldBattleEReaderTrainer(u8 levelType, u16 winStreak)
{
    u8 trainerTeamLevel;
    u8 monLevel;
    s32 i;
    u16 validPartySpecies[6];
    u16 validPartyHeldItems[6];
    u8 numValid;

    numValid = 0;

    ValidateEReaderTrainer();

    if (gSpecialVar_Result != 0 || gSaveBlock2Ptr->battleTower.ereaderTrainer.winStreak != winStreak)
        return FALSE;

    if (levelType != 0)
        trainerTeamLevel = 100;
    else
        trainerTeamLevel = 50;

    for (i = 0; i < 3; i++)
    {
        monLevel = gSaveBlock2Ptr->battleTower.ereaderTrainer.party[i].level;
        if (gSaveBlock2Ptr->battleTower.ereaderTrainer.party[i].level != trainerTeamLevel)
            return FALSE;

        CheckMonBattleTowerBanlist(
            gSaveBlock2Ptr->battleTower.ereaderTrainer.party[i].species,
            gSaveBlock2Ptr->battleTower.ereaderTrainer.party[i].heldItem,
            1,
            levelType,
            monLevel,
            validPartySpecies,
            validPartyHeldItems,
            &numValid);
    }

    return (numValid == 3);
}

static bool8 ChooseSpecialBattleTowerTrainer(void)
{
    s32 recordIndex, i;
    u8 battleTowerLevelType;
    u16 winStreak;
    bool8 retVal;
    s32 numCandidates;
    u32 trainerIds[5];

    numCandidates = 0;
    battleTowerLevelType = gSaveBlock2Ptr->battleTower.battleTowerLevelType;

    winStreak = GetCurrentBattleTowerWinStreak(battleTowerLevelType);
    if (ShouldBattleEReaderTrainer(battleTowerLevelType, winStreak))
    {
        gSaveBlock2Ptr->battleTower.battleTowerTrainerId = BATTLE_TOWER_EREADER_TRAINER_ID;
        retVal = TRUE;
    }
    else
    {
        // Check if one of the battle tower trainers from record mixing should be the next trainer.
        for (recordIndex = 0; recordIndex < 5; recordIndex++)
        {
            struct BattleTowerRecord *record = &gSaveBlock2Ptr->battleTower.records[recordIndex];
            u32 recordHasData = 0;
            u32 checksum = 0;

            for (i = 0; i < sizeof(struct BattleTowerRecord) / sizeof(u32) - 1; i++)
            {
                recordHasData |= ((u32 *) record)[i];
                checksum += ((u32 *) record)[i];
            }

            if (gSaveBlock2Ptr->battleTower.records[recordIndex].winStreak == winStreak
                && gSaveBlock2Ptr->battleTower.records[recordIndex].battleTowerLevelType == battleTowerLevelType
                && recordHasData
                && gSaveBlock2Ptr->battleTower.records[recordIndex].checksum == checksum)
            {
                trainerIds[numCandidates] = recordIndex;
                numCandidates++;
            }
        }

        if (numCandidates == 0)
        {
            retVal = FALSE;
        }
        else
        {
            gSaveBlock2Ptr->battleTower.battleTowerTrainerId =
                trainerIds[Random() % numCandidates] + BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID;
            retVal = TRUE;
        }
    }
    return retVal;
}

void ChooseNextBattleTowerTrainer(void)
{
    int i;
    u16 trainerId;
    bool8 levelType;

    levelType = gSaveBlock2Ptr->battleTower.battleTowerLevelType;
    if (ChooseSpecialBattleTowerTrainer())
    {
        SetBattleTowerTrainerGfxId(gSaveBlock2Ptr->battleTower.battleTowerTrainerId);
        gSaveBlock2Ptr->battleTower.battledTrainerIds[gSaveBlock2Ptr->battleTower.curChallengeBattleNum[levelType] - 1] = gSaveBlock2Ptr->battleTower.battleTowerTrainerId;
        return;
    }

    if (gSaveBlock2Ptr->battleTower.curStreakChallengesNum[levelType] <= 7)
    {
        if (gSaveBlock2Ptr->battleTower.curChallengeBattleNum[levelType] == 7)
        {
            do
            {
                trainerId = ((Random() & 0xFF) * 5) >> 7;
                trainerId += (gSaveBlock2Ptr->battleTower.curStreakChallengesNum[levelType] - 1) * 10 + 20;

                // Ensure trainer wasn't previously fought in this challenge.
                for (i = 0; i < gSaveBlock2Ptr->battleTower.curChallengeBattleNum[levelType] - 1 && gSaveBlock2Ptr->battleTower.battledTrainerIds[i] != trainerId; i++);
            }
            while (i != gSaveBlock2Ptr->battleTower.curChallengeBattleNum[levelType] - 1);
            gSaveBlock2Ptr->battleTower.battleTowerTrainerId = trainerId;
        }
        else
        {
            do
            {
                trainerId = (((Random() & 0xFF) * 5) >> 6);
                trainerId += (gSaveBlock2Ptr->battleTower.curStreakChallengesNum[levelType] - 1) * 10;

                // Ensure trainer wasn't previously fought in this challenge.
                for (i = 0; i < gSaveBlock2Ptr->battleTower.curChallengeBattleNum[levelType] - 1 && gSaveBlock2Ptr->battleTower.battledTrainerIds[i] != trainerId; i++);
            }
            while (i != gSaveBlock2Ptr->battleTower.curChallengeBattleNum[levelType] - 1);
            gSaveBlock2Ptr->battleTower.battleTowerTrainerId = trainerId;
        }
    }
    else
    {
        do
        {
            trainerId = (((Random() & 0xFF) * 30) >> 8) + 70;

            // Ensure trainer wasn't previously fought in this challenge.
            for (i = 0; i < gSaveBlock2Ptr->battleTower.curChallengeBattleNum[levelType] - 1 && gSaveBlock2Ptr->battleTower.battledTrainerIds[i] != trainerId; i++);
        }
        while (i != gSaveBlock2Ptr->battleTower.curChallengeBattleNum[levelType] - 1);
        gSaveBlock2Ptr->battleTower.battleTowerTrainerId = trainerId;
    }

    SetBattleTowerTrainerGfxId(gSaveBlock2Ptr->battleTower.battleTowerTrainerId);
    // Don't bother saving this trainer, since it's the last one in the challenge.
    if (gSaveBlock2Ptr->battleTower.curChallengeBattleNum[levelType] < 7)
        gSaveBlock2Ptr->battleTower.battledTrainerIds[gSaveBlock2Ptr->battleTower.curChallengeBattleNum[levelType] - 1] = gSaveBlock2Ptr->battleTower.battleTowerTrainerId;
}

// The argument is a Battle Tower trainer ID, not a facility class.
// Normal IDs index the newly authored roster; record-mixing and e-Reader IDs
// use the facility class already stored in their respective records.
static void SetBattleTowerTrainerGfxId(u8 trainerId)
{
    u8 facilityClass;
    u16 gfxId = OBJ_EVENT_GFX_YOUNGSTER;

    if (trainerId == BATTLE_TOWER_EREADER_TRAINER_ID)
        facilityClass = gSaveBlock2Ptr->battleTower.ereaderTrainer.trainerClass;
    else if (trainerId < BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID)
        facilityClass = sBattleTowerTrainers[trainerId].trainerClass;
    else if (trainerId < BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID + 5)
        facilityClass = gSaveBlock2Ptr->battleTower.records[trainerId - BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID].trainerClass;
    else
        facilityClass = FACILITY_CLASS_RS_YOUNGSTER;

    switch (facilityClass)
    {
    case FACILITY_CLASS_RS_CAMPER: gfxId = OBJ_EVENT_GFX_CAMPER; break;
    case FACILITY_CLASS_RS_PICNICKER: gfxId = OBJ_EVENT_GFX_PICNICKER; break;
    case FACILITY_CLASS_RS_COOLTRAINER_M: gfxId = OBJ_EVENT_GFX_COOLTRAINER_M; break;
    case FACILITY_CLASS_RS_COOLTRAINER_F: gfxId = OBJ_EVENT_GFX_COOLTRAINER_F; break;
    case FACILITY_CLASS_RS_BEAUTY: gfxId = OBJ_EVENT_GFX_BEAUTY; break;
    case FACILITY_CLASS_RS_BLACK_BELT: gfxId = OBJ_EVENT_GFX_BLACK_BELT; break;
    case FACILITY_CLASS_RS_GENTLEMAN: gfxId = OBJ_EVENT_GFX_GENTLEMAN; break;
    default: break;
    }
    VarSet(VAR_OBJ_GFX_ID_0, gfxId);
}

void SetEReaderTrainerGfxId(void)
{
    SetBattleTowerTrainerGfxId(BATTLE_TOWER_EREADER_TRAINER_ID);
}

// Unreferenced; formerly used by Record Mixing in R/S
static void UpdateOrInsertReceivedBattleTowerRecord(struct BattleTowerRecord * record0)
{
    s32 i;
    s32 j;
    s32 k;
    s32 l = 0;
    struct BattleTowerRecord * record = record0;

    u16 winStreaks[6];
    u16 indices[6];

    for (i = 0; i < 5; i++)
    {
        k = 0;
        for (j = 0; j < 4; j++)
        {
            if (gSaveBlock2Ptr->battleTower.records[i].trainerId[j] != record->trainerId[j])
                break;
        }
        if (j == 4)
        {
            for (k = 0; k < 7; k++)
            {
                if (gSaveBlock2Ptr->battleTower.records[i].name[j] != record->name[j])
                    break;
                if (record->name[j] == EOS)
                {
                    k = 7;
                    break;
                }
            }
        }
        if (k == 7)
            break;
    }

    if (i < 5)
    {
        gSaveBlock2Ptr->battleTower.records[i] = *record;
        return;
    }

    for (i = 0; i < 5; i++)
    {
        if (gSaveBlock2Ptr->battleTower.records[i].winStreak == 0)
            break;
    }

    if (i < 5)
    {
        gSaveBlock2Ptr->battleTower.records[i] = *record;
        return;
    }

    winStreaks[0] = gSaveBlock2Ptr->battleTower.records[0].winStreak;
    indices[0] = 0;
    l++;

    for (i = 1; i < 5; i++)
    {
        for (j = 0; j < l; j++)
        {
            if (gSaveBlock2Ptr->battleTower.records[i].winStreak < winStreaks[j])
            {
                j = 0;
                l = 1;
                winStreaks[0] = gSaveBlock2Ptr->battleTower.records[i].winStreak;
                indices[0] = i;
                break;
            }
            if (gSaveBlock2Ptr->battleTower.records[i].winStreak > winStreaks[j])
                break;
        }
        if (j == l)
        {
            winStreaks[l] = gSaveBlock2Ptr->battleTower.records[i].winStreak;
            indices[l] = i;
            l++;
        }
    }
    i = Random() % l;
    gSaveBlock2Ptr->battleTower.records[indices[i]] = *record;
}

u8 GetBattleTowerTrainerFrontSpriteId(void)
{
    if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId == BATTLE_TOWER_EREADER_TRAINER_ID)
    {
        return gFacilityClassToPicIndex[gSaveBlock2Ptr->battleTower.ereaderTrainer.trainerClass];
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID)
    {
        return gFacilityClassToPicIndex[sBattleTowerTrainers[gSaveBlock2Ptr->battleTower.battleTowerTrainerId].trainerClass];
    }
    else
    {
        return gFacilityClassToPicIndex[gSaveBlock2Ptr->battleTower.records[gSaveBlock2Ptr->battleTower.battleTowerTrainerId - BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID].trainerClass];
    }
}

u8 GetBattleTowerTrainerClassNameId(void)
{
    if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId == BATTLE_TOWER_EREADER_TRAINER_ID)
    {
        return gFacilityClassToTrainerClass[gSaveBlock2Ptr->battleTower.ereaderTrainer.trainerClass];
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId >= BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID)
    {
        return gFacilityClassToTrainerClass[gSaveBlock2Ptr->battleTower.records[gSaveBlock2Ptr->battleTower.battleTowerTrainerId - BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID].trainerClass];
    }
    else
    {
        return gFacilityClassToTrainerClass[sBattleTowerTrainers[gSaveBlock2Ptr->battleTower.battleTowerTrainerId].trainerClass];
    }
}

void GetBattleTowerTrainerName(u8 *dest)
{
    s32 i;
    if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId == BATTLE_TOWER_EREADER_TRAINER_ID)
    {
        for (i = 0; i < 7; i++)
            dest[i] = gSaveBlock2Ptr->battleTower.ereaderTrainer.name[i];
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID)
    {
        for (i = 0; i < 3; i++)
            dest[i] = sBattleTowerTrainers[gSaveBlock2Ptr->battleTower.battleTowerTrainerId].name[i];
    }
    else
    {
        for (i = 0; i < 7; i++)
            dest[i] = gSaveBlock2Ptr->battleTower.records[gSaveBlock2Ptr->battleTower.battleTowerTrainerId - BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID].name[i];
    }
    dest[i] = EOS;
}

static void FillBattleTowerTrainerParty(void)
{
    s32 partyIndex;
    s32 i;
    u16 chosenMonIndices[3];
    u8 friendship;
    u8 level;
    u8 fixedIV;
    u8 battleMonsOffset;
    u8 monPoolSize;
    u8 teamFlags;
    const struct BattleTowerPokemonTemplate *battleTowerMons;

    battleMonsOffset = 0;
    monPoolSize = 60;
    friendship = 255;

    ZeroEnemyPartyMons();

    // Different trainers have access to different sets of pokemon to use in battle.
    // The pokemon later in gBattleTowerLevel100Mons or gBattleTowerLevel50Mons are
    // stronger. Additionally, the later trainers' pokemon are granted higher IVs.
    if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 20)
    {
        fixedIV = 6;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 30)
    {
        fixedIV = 9;
        battleMonsOffset = 30;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 40)
    {
        fixedIV = 12;
        battleMonsOffset = 60;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 50)
    {
        fixedIV = 15;
        battleMonsOffset = 90;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 60)
    {
        fixedIV = 18;
        battleMonsOffset = 120;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 70)
    {
        fixedIV = 21;
        battleMonsOffset = 150;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 80)
    {
        fixedIV = 31;
        battleMonsOffset = 180;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID)
    {
        fixedIV = 31;
        battleMonsOffset = 200;
        monPoolSize = 100;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId == BATTLE_TOWER_EREADER_TRAINER_ID)
    {
        // Load E-Reader trainer's party.
        do
        {
            for (partyIndex = 0; partyIndex < 3; partyIndex++)
                CreateBattleTowerMon(&gEnemyParty[partyIndex], &gSaveBlock2Ptr->battleTower.ereaderTrainer.party[partyIndex]);
            return;
        } while (0);
    }
    else
    {
        // Load a battle tower record's party. (From record mixing)
        for (partyIndex = 0; partyIndex < 3; partyIndex++)
        {
            CreateBattleTowerMon(
                &gEnemyParty[partyIndex],
                &gSaveBlock2Ptr->battleTower.records[gSaveBlock2Ptr->battleTower.battleTowerTrainerId - BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID].party[partyIndex]);
        }
        return;
    }

    // Use the appropriate list of pokemon and level depending on the
    // current challenge type. (level 50 or level 100 challenge)
    if (gSaveBlock2Ptr->battleTower.battleTowerLevelType != 0)
    {
        battleTowerMons = gBattleTowerLevel100Mons;
        level = 100;
    }
    else
    {
        battleTowerMons = gBattleTowerLevel50Mons;
        level = 50;
    }

    teamFlags = sBattleTowerTrainers[gSaveBlock2Ptr->battleTower.battleTowerTrainerId].teamFlags;

    // Attempt to fill the trainer's party with random Pokemon until 3 have been
    // successfully chosen. The trainer's party may not have duplicate pokemon species
    // or duplicate held items. Each pokemon must have all of the trainer's team flags
    // set, as well.  If any of those conditions are not met, then the loop starts over
    // and another pokemon is chosen at random.
    partyIndex = 0;
    while (partyIndex != 3)
    {
        // Pick a random pokemon index based on the number of pokemon available to choose from
        // and the starting offset in the battle tower pokemon array.
        s32 battleMonIndex = ((Random() & 0xFF) * monPoolSize) / 256 + battleMonsOffset;

        // Ensure the chosen pokemon has compatible team flags with the trainer.
        if (teamFlags == 0 || (battleTowerMons[battleMonIndex].teamFlags & teamFlags) == teamFlags)
        {
            // Ensure this pokemon species isn't a duplicate.
            for (i = 0; i < partyIndex; i++)
            {
                if (GetMonData(&gEnemyParty[i], MON_DATA_SPECIES, NULL) == battleTowerMons[battleMonIndex].species)
                    break;
            }

            if (i != partyIndex)
                continue;

            // Ensure this pokemon's held item isn't a duplicate.
            for (i = 0; i < partyIndex; i++)
            {
                if (GetMonData(&gEnemyParty[i], MON_DATA_HELD_ITEM, NULL) != ITEM_NONE
                    && GetMonData(&gEnemyParty[i], MON_DATA_HELD_ITEM, NULL) == sBattleTowerHeldItems[battleTowerMons[battleMonIndex].heldItem])
                    break;
            }

            if (i != partyIndex)
                continue;

            // Ensure this exact pokemon index isn't a duplicate. This check doesn't seem necessary
            // because the species and held items were already checked directly above. Perhaps this
            // is leftover code before the logic for duplicate species and held items was added.
            //for (i = 0; i < partyIndex && chosenMonIndices[i] != battleMonIndex; i++);
            for (i = 0; i < partyIndex; i++)
            {
                if (chosenMonIndices[i] == battleMonIndex)
                    break;
            }

            if (i != partyIndex)
                continue;

            chosenMonIndices[partyIndex] = battleMonIndex;

            // Place the chosen pokemon into the trainer's party.
            CreateMonWithEVSpread(
                &gEnemyParty[partyIndex],
                battleTowerMons[battleMonIndex].species,
                level,
                fixedIV,
                battleTowerMons[battleMonIndex].evSpread);

            // Give the chosen pokemon its specified moves.
            for (i = 0; i < 4; i++)
            {
                SetMonMoveSlot(&gEnemyParty[partyIndex], battleTowerMons[battleMonIndex].moves[i], i);
                if (battleTowerMons[battleMonIndex].moves[i] == MOVE_FRUSTRATION)
                    friendship = 0;  // MOVE_FRUSTRATION is more powerful the lower the pokemon's friendship is.
            }

            SetMonData(&gEnemyParty[partyIndex], MON_DATA_FRIENDSHIP, &friendship);
            SetMonData(&gEnemyParty[partyIndex], MON_DATA_HELD_ITEM, &sBattleTowerHeldItems[battleTowerMons[battleMonIndex].heldItem]);

            // The pokemon was successfully added to the trainer's party, so it's safe to move on to
            // the next party slot.
            partyIndex++;
        }
    }
}

static u8 AppendBattleTowerBannedSpeciesName(u16 species, u8 count)
{
    if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
    {
        if (count == 0)
            StringAppend(gStringVar1, gText_BattleTowerBan_Space);
        count++;
        StringAppend(gStringVar1, gSpeciesNames[species]);
        switch (count)
        {
        case 2:
            StringAppend(gStringVar1, gText_BattleTowerBan_Newline2);
            break;
        case 5:
        case 8:
        case 11:
            StringAppend(gStringVar1, gText_BattleTowerBan_Newline1);
            break;
        default:
            StringAppend(gStringVar1, gText_BattleTowerBan_Space);
            break;
        }
    }
    return count;
}

static void CheckMonBattleTowerBanlist(u16 species, u16 heldItem, u16 hp, u8 battleTowerLevelType, u8 monLevel, u16 *validPartySpecies, u16 *validPartyHeldItems, u8 *numValid)
{
    s32 i;
    u32 counter = 0;

    if (species == SPECIES_EGG || species == SPECIES_NONE)
        return;

    while (1)
    {
        if (gBattleTowerBannedSpecies[counter] == 0xFFFF)
            break;

        if (gBattleTowerBannedSpecies[counter] == species)
            break;

        counter++;
    }

    if (gBattleTowerBannedSpecies[counter] != 0xFFFF)
        return;

    if (battleTowerLevelType == 0 && monLevel > 50)
        return;

    for (i = 0; i < *numValid && validPartySpecies[i] != species ; i++);
    if (i != *numValid)
        return;

    if (heldItem != ITEM_NONE)
    {
        for (i = 0; i < *numValid && validPartyHeldItems[i] != heldItem ; i++);
        if (i != *numValid)
            return;
    }

    validPartySpecies[*numValid] = species;
    validPartyHeldItems[*numValid] = heldItem;
    *numValid = *numValid + 1;
}

void CheckPartyBattleTowerBanlist(void)
{
    s32 i;
    u16 species2;
    u16 heldItem;
    u8 level;
    u16 hp;
    u32 numBanlistCaught;
    u16 validPartySpecies[6];
    u16 validPartyHeldItems[6];
    u8 counter;

    counter = 0;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        species2 = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG);
        heldItem = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM);
        level = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
        hp = GetMonData(&gPlayerParty[i], MON_DATA_HP);

        CheckMonBattleTowerBanlist(species2, heldItem, hp, gSpecialVar_Result, level, validPartySpecies, validPartyHeldItems, &counter);
    }

    if (counter < 3)
    {
        gStringVar1[0] = EOS;
        gSpecialVar_0x8004 = 1;
        counter = 0;

        for (i = 0; gBattleTowerBannedSpecies[i] != 0xFFFF; i++)
            counter = AppendBattleTowerBannedSpeciesName(gBattleTowerBannedSpecies[i], counter);

        i = StringLength(gStringVar1);
        gStringVar1[i - 1] = EOS;
        if (counter < 3)
            StringAppend(gStringVar1, gText_BattleTowerBan_Is1);
        else
            StringAppend(gStringVar1, gText_BattleTowerBan_Is2);
    }
    else
    {
        gSpecialVar_0x8004 = 0;
        gSaveBlock2Ptr->battleTower.battleTowerLevelType = gSpecialVar_Result;
    }
}

static void BufferBattleTowerTrainerMessage(const u16 *greeting)
{
    s32 i;
    if (EC_DoesEasyChatStringFitOnLine(greeting, 3, 2, 18))
    {
        ConvertEasyChatWordsToString(gStringVar4, greeting, 2, 3);
        i = 0;
        while (gStringVar4[i++] != CHAR_NEWLINE)
            ;
        while (gStringVar4[i] != CHAR_NEWLINE)
            i++;
        gStringVar4[i] = CHAR_PROMPT_SCROLL;
    }
    else
    {
        ConvertEasyChatWordsToString(gStringVar4, greeting, 3, 2);
    }
}

void PrintBattleTowerTrainerGreeting(void)
{
    if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId == BATTLE_TOWER_EREADER_TRAINER_ID)
        BufferBattleTowerTrainerMessage(gSaveBlock2Ptr->battleTower.ereaderTrainer.greeting);
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID)
        BufferBattleTowerTrainerMessage(sBattleTowerTrainers[gSaveBlock2Ptr->battleTower.battleTowerTrainerId].greeting);
    else
        BufferBattleTowerTrainerMessage(gSaveBlock2Ptr->battleTower.records[gSaveBlock2Ptr->battleTower.battleTowerTrainerId - BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID].greeting);
}

static void CB2_FinishEReaderBattle(void)
{
    s32 i;
    u16 heldItem;

    switch (sSpecialVar_0x8004_Copy)
    {
    case 0:
        break;
    case 1:
        for (i = 0; i < PARTY_SIZE; i++)
        {
            heldItem = GetMonData(&gSaveBlock1Ptr->playerParty[i], MON_DATA_HELD_ITEM);
            SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, &heldItem);
        }
        break;
    case 2:
        PrintEReaderTrainerFarewellMessage();
        break;
    }

    SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

static void Task_WaitBT(u8 taskId)
{
    if (IsBattleTransitionDone() == TRUE)
    {
        gMain.savedCallback = CB2_FinishEReaderBattle;
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_InitBattle);
        DestroyTask(taskId);
    }
}


void StartSpecialBattle(void)
{
    s32 i;
    u16 heldItem;
    u8 transition;

    sSpecialVar_0x8004_Copy = gSpecialVar_0x8004;
    switch (sSpecialVar_0x8004_Copy)
    {
    case 0: // battle tower battle
        gBattleTypeFlags = (BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_TRAINER);
        gTrainerBattleOpponent_A = 0;

        FillBattleTowerTrainerParty();

        CreateTask(Task_WaitBT, 1);
        PlayMapChosenOrBattleBGM(0);
        transition = BattleSetup_GetBattleTowerBattleTransition();
        BattleTransition_StartOnField(transition);
        break;
    case 1: // secret base battle
        for (i = 0; i < PARTY_SIZE; i++)
        {
            heldItem = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM);
            SetMonData(&gSaveBlock1Ptr->playerParty[i], MON_DATA_HELD_ITEM, &heldItem);
        }

        CreateTask(Task_WaitBT, 1);
        PlayMapChosenOrBattleBGM(0);
        transition = BattleSetup_GetBattleTowerBattleTransition();
        BattleTransition_StartOnField(transition);
        break;
    case 2: // e-reader trainer battle
        ZeroEnemyPartyMons();

        for (i = 0; i < 3; i++)
            CreateBattleTowerMon(&gEnemyParty[i], &gSaveBlock2Ptr->battleTower.ereaderTrainer.party[i]);

        gBattleTypeFlags = (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER);
        gTrainerBattleOpponent_A = 0;

        CreateTask(Task_WaitBT, 1);
        PlayMapChosenOrBattleBGM(0);
        transition = BattleSetup_GetBattleTowerBattleTransition();
        BattleTransition_StartOnField(transition);
        break;
    }
}

// Read-only result classification for the future Seven Island challenge script.
// 0 = victory, 1 = defeat/draw, 2 = interrupted or unrecognized.
// This deliberately does not advance streaks, save, or modify the party.
void GetBattleTowerBattleResult(void)
{
    switch (gBattleOutcome)
    {
    case B_OUTCOME_WON:
        gSpecialVar_Result = 0;
        break;
    case B_OUTCOME_LOST:
    case B_OUTCOME_DREW:
        gSpecialVar_Result = 1;
        break;
    default:
        gSpecialVar_Result = 2;
        break;
    }
}

void SetBattleTowerProperty(void)
{
    s32 i;
    u8 battleTowerLevelType = gSaveBlock2Ptr->battleTower.battleTowerLevelType;

    switch (gSpecialVar_0x8004)
    {
    case 0:
        ewram160FB = gSaveBlock2Ptr->battleTower.var_4AE[battleTowerLevelType];
        gSaveBlock2Ptr->battleTower.var_4AE[battleTowerLevelType] = gSpecialVar_0x8005;
        break;
    case 1:
        gSaveBlock2Ptr->battleTower.battleTowerLevelType = gSpecialVar_0x8005;
        break;
    case 2:
        gSaveBlock2Ptr->battleTower.curChallengeBattleNum[battleTowerLevelType] = gSpecialVar_0x8005;
        break;
    case 3:
        gSaveBlock2Ptr->battleTower.curStreakChallengesNum[battleTowerLevelType] = gSpecialVar_0x8005;
        break;
    case 4:
        gSaveBlock2Ptr->battleTower.battleTowerTrainerId = gSpecialVar_0x8005;
        break;
    case 5:
        for (i = 0; i < 3; i++)
            gSaveBlock2Ptr->battleTower.selectedPartyMons[i] = gSelectedOrderFromParty[i];
        break;
    case 6:
        if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId == BATTLE_TOWER_EREADER_TRAINER_ID)
            ClearEReaderTrainer(&gSaveBlock2Ptr->battleTower.ereaderTrainer);
        if (gSaveBlock2Ptr->battleTower.totalBattleTowerWins < 9999)
            gSaveBlock2Ptr->battleTower.totalBattleTowerWins++;
        gSaveBlock2Ptr->battleTower.curChallengeBattleNum[battleTowerLevelType]++;
        SaveCurrentWinStreak();
        gSpecialVar_Result = gSaveBlock2Ptr->battleTower.curChallengeBattleNum[battleTowerLevelType];
        gStringVar1[0] = gSaveBlock2Ptr->battleTower.curChallengeBattleNum[battleTowerLevelType] + 0xA1;
        gStringVar1[1] = 0xFF;
        break;
    case 7:
        if (gSaveBlock2Ptr->battleTower.curStreakChallengesNum[battleTowerLevelType] < 1430)
            gSaveBlock2Ptr->battleTower.curStreakChallengesNum[battleTowerLevelType]++;
        SaveCurrentWinStreak();
        gSpecialVar_Result = gSaveBlock2Ptr->battleTower.curStreakChallengesNum[battleTowerLevelType];
        break;
    case 8:
        gSaveBlock2Ptr->battleTower.unk_554 = gSpecialVar_0x8005;
        break;
    case 9:
        break;
    case 10:
        SetGameStat(GAME_STAT_BATTLE_TOWER_BEST_STREAK, gSaveBlock2Ptr->battleTower.bestBattleTowerWinStreak);
        break;
    case 11:
        if (gSaveBlock2Ptr->battleTower.var_4AE[battleTowerLevelType] != 3)
            ResetBattleTowerStreak(battleTowerLevelType);
        break;
    case 12:
        gSaveBlock2Ptr->battleTower.var_4AE[battleTowerLevelType] = ewram160FB;
        break;
    case 13:
        gSaveBlock2Ptr->battleTower.currentWinStreaks[battleTowerLevelType] = GetCurrentBattleTowerWinStreak(battleTowerLevelType);
        break;
    case 14:
        gSaveBlock2Ptr->battleTower.lastStreakLevelType = gSaveBlock2Ptr->battleTower.battleTowerLevelType;
        break;
    }
}

void BattleTowerUtil(void)
{
    u8 battleTowerLevelType = gSaveBlock2Ptr->battleTower.battleTowerLevelType;

    switch (gSpecialVar_0x8004)
    {
    case 0:
        gSpecialVar_Result = gSaveBlock2Ptr->battleTower.var_4AE[battleTowerLevelType];
        break;
    case 1:
        gSpecialVar_Result = gSaveBlock2Ptr->battleTower.battleTowerLevelType;
        break;
    case 2:
        gSpecialVar_Result = gSaveBlock2Ptr->battleTower.curChallengeBattleNum[battleTowerLevelType];
        break;
    case 3:
        gSpecialVar_Result = gSaveBlock2Ptr->battleTower.curStreakChallengesNum[battleTowerLevelType];
        break;
    case 4:
        gSpecialVar_Result = gSaveBlock2Ptr->battleTower.battleTowerTrainerId;
        break;
    case 5:
    case 6:
    case 7:
        break;
    case 8:
        gSpecialVar_Result = gSaveBlock2Ptr->battleTower.unk_554;
        break;
    case 9:
        gSpecialVar_Result = GetCurrentBattleTowerWinStreak(battleTowerLevelType);
        break;
    case 10:
        SetGameStat(GAME_STAT_BATTLE_TOWER_BEST_STREAK, gSaveBlock2Ptr->battleTower.bestBattleTowerWinStreak);
        break;
    case 11:
        ResetBattleTowerStreak(battleTowerLevelType);
        break;
    case 12:
        gSaveBlock2Ptr->battleTower.var_4AE[battleTowerLevelType] = ewram160FB;
        break;
    case 13:
        gSaveBlock2Ptr->battleTower.currentWinStreaks[battleTowerLevelType] = GetCurrentBattleTowerWinStreak(battleTowerLevelType);
        break;
    case 14:
        gSaveBlock2Ptr->battleTower.lastStreakLevelType = gSaveBlock2Ptr->battleTower.battleTowerLevelType;
        break;
    }
}

void SetBattleTowerParty(void)
{
    s32 i;

    for (i = 0; i < 3; i++)
        gSelectedOrderFromParty[i] = gSaveBlock2Ptr->battleTower.selectedPartyMons[i];

    ReducePlayerPartyToThree();
}

static void SaveCurrentWinStreak(void)
{
    u8 levelType = gSaveBlock2Ptr->battleTower.battleTowerLevelType;
    u16 streak = GetCurrentBattleTowerWinStreak(levelType);

    if (gSaveBlock2Ptr->battleTower.recordWinStreaks[levelType] < streak)
        gSaveBlock2Ptr->battleTower.recordWinStreaks[levelType] = streak;

    if (gSaveBlock2Ptr->battleTower.recordWinStreaks[0] > gSaveBlock2Ptr->battleTower.recordWinStreaks[1])
    {
        streak = gSaveBlock2Ptr->battleTower.recordWinStreaks[0];
        SetGameStat(GAME_STAT_BATTLE_TOWER_BEST_STREAK, streak);

        if (streak > 9999)
            gSaveBlock2Ptr->battleTower.bestBattleTowerWinStreak = 9999;
        else
            gSaveBlock2Ptr->battleTower.bestBattleTowerWinStreak = streak;
    }
    else
    {
        streak = gSaveBlock2Ptr->battleTower.recordWinStreaks[1];
        SetGameStat(GAME_STAT_BATTLE_TOWER_BEST_STREAK, streak);

        if (streak > 9999)
            gSaveBlock2Ptr->battleTower.bestBattleTowerWinStreak = 9999;
        else
            gSaveBlock2Ptr->battleTower.bestBattleTowerWinStreak = streak;
    }
}

static void SetPlayerBattleTowerRecord(void)
{
    s32 i;
    u8 trainerClass;
    struct BattleTowerRecord *playerRecord = &gSaveBlock2Ptr->battleTower.playerRecord;
    u8 battleTowerLevelType = gSaveBlock2Ptr->battleTower.battleTowerLevelType;

    if (gSaveBlock2Ptr->playerGender != MALE)
    {
        trainerClass = sFemaleTrainerClasses[(gSaveBlock2Ptr->playerTrainerId[0] + gSaveBlock2Ptr->playerTrainerId[1]
                                              + gSaveBlock2Ptr->playerTrainerId[2] + gSaveBlock2Ptr->playerTrainerId[3]) % NELEMS(sFemaleTrainerClasses)];
    }
    else
    {
        trainerClass = sMaleTrainerClasses[(gSaveBlock2Ptr->playerTrainerId[0] + gSaveBlock2Ptr->playerTrainerId[1]
                                            + gSaveBlock2Ptr->playerTrainerId[2] + gSaveBlock2Ptr->playerTrainerId[3]) % NELEMS(sMaleTrainerClasses)];
    }

    playerRecord->battleTowerLevelType = battleTowerLevelType;
    playerRecord->trainerClass = trainerClass;

    CopyTrainerId(playerRecord->trainerId, gSaveBlock2Ptr->playerTrainerId);
    StringCopy_PlayerName(playerRecord->name, gSaveBlock2Ptr->playerName);

    playerRecord->winStreak = GetCurrentBattleTowerWinStreak(battleTowerLevelType);

    for (i = 0; i < 6; i++)
        playerRecord->greeting[i] = gSaveBlock1Ptr->easyChatBattleStart[i];

    for (i = 0; i < 3; i++)
        ConvertPokemonToBattleTowerPokemon(&gPlayerParty[gSaveBlock2Ptr->battleTower.selectedPartyMons[i] - 1],
                                           &playerRecord->party[i]);

    SetBattleTowerRecordChecksum(&gSaveBlock2Ptr->battleTower.playerRecord);
    SaveCurrentWinStreak();
}

void SaveBattleTowerProgress(void)
{
    u8 battleTowerLevelType = gSaveBlock2Ptr->battleTower.battleTowerLevelType;

    if (gSpecialVar_0x8004 == 3 || gSpecialVar_0x8004 == 0)
    {
        if (gSaveBlock2Ptr->battleTower.curStreakChallengesNum[battleTowerLevelType] > 1
            || gSaveBlock2Ptr->battleTower.curChallengeBattleNum[battleTowerLevelType] > 1)
            SetPlayerBattleTowerRecord();
    }

    PopulateBravoTrainerBattleTowerLostData();

    gSaveBlock2Ptr->battleTower.battleOutcome = gBattleOutcome;

    if (gSpecialVar_0x8004 != 3)
        gSaveBlock2Ptr->battleTower.var_4AE[battleTowerLevelType] = gSpecialVar_0x8004;

    VarSet(VAR_TEMP_0, BTSPECIAL_TEST);
    gSaveBlock2Ptr->battleTower.unk_554 = 1;
    TrySavingData(SAVE_EREADER);
}

void BattleTower_SoftReset(void)
{
    DoSoftReset();
}

static void ValidateBattleTowerRecordChecksums(void)
{
    u32 i;
    s32 recordIndex;
    struct BattleTowerRecord *record;
    u32 checksum;

    record = &gSaveBlock2Ptr->battleTower.playerRecord;
    checksum = 0;
    for (i = 0; i < (sizeof(struct BattleTowerRecord) / sizeof(u32)) - 1; i++)
        checksum += ((u32 *)record)[i];

    if (gSaveBlock2Ptr->battleTower.playerRecord.checksum != checksum)
        ClearBattleTowerRecord(&gSaveBlock2Ptr->battleTower.playerRecord);

    for (recordIndex = 0; recordIndex < 5; recordIndex++)
    {
        record = &gSaveBlock2Ptr->battleTower.records[recordIndex];
        checksum = 0;
        for (i = 0; i < (sizeof(struct BattleTowerRecord) / sizeof(u32)) - 1; i++)
            checksum += ((u32 *)record)[i];

        if (gSaveBlock2Ptr->battleTower.records[recordIndex].checksum != checksum)
            ClearBattleTowerRecord(&gSaveBlock2Ptr->battleTower.records[recordIndex]);
    }
}

static void SetBattleTowerRecordChecksum(struct BattleTowerRecord *record)
{
    u32 i;

    record->checksum = 0;
    for (i = 0; i < (sizeof(struct BattleTowerRecord) / sizeof(u32)) - 1; i++)
        record->checksum += ((u32 *)record)[i];
}

static void ClearBattleTowerRecord(struct BattleTowerRecord *record)
{
    u32 i;

    for (i = 0; i < sizeof(struct BattleTowerRecord) / sizeof(u32); i++)
        ((u32 *)record)[i] = 0;
}

static void PopulateBravoTrainerBattleTowerLostData(void)
{
    s32 i;

    GetBattleTowerTrainerName(gSaveBlock2Ptr->battleTower.defeatedByTrainerName);
    gSaveBlock2Ptr->battleTower.defeatedBySpecies = gBattleMons[1].species;
    gSaveBlock2Ptr->battleTower.firstMonSpecies = gBattleMons[0].species;

    for (i = 0; i < POKEMON_NAME_LENGTH; i++)
        gSaveBlock2Ptr->battleTower.firstMonNickname[i] = gBattleMons[0].nickname[i];
}

static u16 GetCurrentBattleTowerWinStreak(u8 battleTowerLevelType)
{
    u16 winStreak = ((gSaveBlock2Ptr->battleTower.curStreakChallengesNum[battleTowerLevelType] - 1) * 7 - 1)
                    + gSaveBlock2Ptr->battleTower.curChallengeBattleNum[battleTowerLevelType];

    if (winStreak > 9999)
        return 9999;
    else
        return winStreak;
}

void DetermineBattleTowerPrize(void)
{
    u8 levelType = gSaveBlock2Ptr->battleTower.battleTowerLevelType;

    if (gSaveBlock2Ptr->battleTower.curStreakChallengesNum[levelType] - 1 > 5)
        gSaveBlock2Ptr->battleTower.prizeItem = sLongStreakPrizes[Random() % NELEMS(sLongStreakPrizes)];
    else
        gSaveBlock2Ptr->battleTower.prizeItem = sShortStreakPrizes[Random() % NELEMS(sShortStreakPrizes)];
}

void GiveBattleTowerPrize(void)
{
    u8 battleTowerLevelType = gSaveBlock2Ptr->battleTower.battleTowerLevelType;

    if (AddBagItem(gSaveBlock2Ptr->battleTower.prizeItem, 1) == TRUE)
    {
        CopyItemName(gSaveBlock2Ptr->battleTower.prizeItem, gStringVar1);
        gSpecialVar_Result = 1;
    }
    else
    {
        gSpecialVar_Result = 0;
        gSaveBlock2Ptr->battleTower.var_4AE[battleTowerLevelType] = 6;
    }
}

void AwardBattleTowerRibbons(void)
{
    s32 i;
    u32 partyIndex;
    struct Pokemon *pokemon;
    u8 ribbonType;
    u8 battleTowerLevelType = gSaveBlock2Ptr->battleTower.battleTowerLevelType;

    if (battleTowerLevelType != 0)
        ribbonType = MON_DATA_VICTORY_RIBBON;
    else
        ribbonType = MON_DATA_WINNING_RIBBON;

    gSpecialVar_Result = 0;

    if (GetCurrentBattleTowerWinStreak(battleTowerLevelType) > 55)
    {
        for (i = 0; i < 3; i++)
        {
            partyIndex = gSaveBlock2Ptr->battleTower.selectedPartyMons[i] - 1;
            pokemon = &gPlayerParty[partyIndex];
            if (!GetMonData(pokemon, ribbonType))
            {
                gSpecialVar_Result = 1;
                SetMonData(pokemon, ribbonType, &gSpecialVar_Result);
            }
        }
    }

    if (gSpecialVar_Result != 0)
        IncrementGameStat(GAME_STAT_RECEIVED_RIBBONS);
}

// This is a leftover debugging function that is used to populate the E-Reader
// trainer with the player's current data.
static void Debug_FillEReaderTrainerWithPlayerData(void)
{
    struct BattleTowerEReaderTrainer *ereaderTrainer;
    s32 i;
    s32 j;

    ereaderTrainer = &gSaveBlock2Ptr->battleTower.ereaderTrainer;

    if (gSaveBlock2Ptr->playerGender != MALE)
    {
        ereaderTrainer->trainerClass = sFemaleTrainerClasses[(gSaveBlock2Ptr->playerTrainerId[0] + gSaveBlock2Ptr->playerTrainerId[1]
                                                              + gSaveBlock2Ptr->playerTrainerId[2] + gSaveBlock2Ptr->playerTrainerId[3]) % NELEMS(sFemaleTrainerClasses)];
    }
    else
    {
        ereaderTrainer->trainerClass = sMaleTrainerClasses[(gSaveBlock2Ptr->playerTrainerId[0] + gSaveBlock2Ptr->playerTrainerId[1]
                                                            + gSaveBlock2Ptr->playerTrainerId[2] + gSaveBlock2Ptr->playerTrainerId[3]) % NELEMS(sMaleTrainerClasses)];
    }

    CopyTrainerId(ereaderTrainer->trainerId, gSaveBlock2Ptr->playerTrainerId);
    StringCopy_PlayerName(ereaderTrainer->name, gSaveBlock2Ptr->playerName);

    ereaderTrainer->winStreak = 1;

    j = 7;
    for (i = 0; i < 6; i++)
    {
        ereaderTrainer->greeting[i] = gSaveBlock1Ptr->easyChatBattleStart[i];
        ereaderTrainer->farewellPlayerLost[i] = j;
        ereaderTrainer->farewellPlayerWon[i] = j + 6;
        j++;
    }

    for (i = 0; i < 3; i++)
        ConvertPokemonToBattleTowerPokemon(&gPlayerParty[i], &ereaderTrainer->party[i]);

    SetEReaderTrainerChecksum(ereaderTrainer);
}

u8 GetEreaderTrainerFrontSpriteId(void)
{
    return gFacilityClassToPicIndex[gSaveBlock2Ptr->battleTower.ereaderTrainer.trainerClass];
}

u8 GetEreaderTrainerClassId(void)
{
    return gFacilityClassToTrainerClass[gSaveBlock2Ptr->battleTower.ereaderTrainer.trainerClass];
}

void CopyEReaderTrainerName5(u8 *trainerName)
{
    s32 i;

    for (i = 0; i < 5; i++)
        trainerName[i] = gSaveBlock2Ptr->battleTower.ereaderTrainer.name[i];

    trainerName[i] = EOS;
}

// Checks if the saved E-Reader trainer is valid.
void ValidateEReaderTrainer(void)
{
    u32 i;
    u32 checksum;
    struct BattleTowerEReaderTrainer *ereaderTrainer;

    gSpecialVar_Result = 0;
    ereaderTrainer = &gSaveBlock2Ptr->battleTower.ereaderTrainer;

    checksum = 0;
    for (i = 0; i < sizeof(struct BattleTowerEReaderTrainer) / sizeof(u32) - 1; i++)
        checksum |= ((u32 *)ereaderTrainer)[i];

    if (checksum == 0)
    {
        gSpecialVar_Result = 1;
        return;
    }

    checksum = 0;
    for (i = 0; i < sizeof(struct BattleTowerEReaderTrainer) / sizeof(u32) - 1; i++)
        checksum += ((u32 *)ereaderTrainer)[i];

    if (gSaveBlock2Ptr->battleTower.ereaderTrainer.checksum != checksum)
    {
        ClearEReaderTrainer(&gSaveBlock2Ptr->battleTower.ereaderTrainer);
        gSpecialVar_Result = 1;
    }
}

static void SetEReaderTrainerChecksum(struct BattleTowerEReaderTrainer *ereaderTrainer)
{
    s32 i;

    ereaderTrainer->checksum = 0;
    for (i = 0; i < sizeof(struct BattleTowerEReaderTrainer) / sizeof(u32) - 1; i++)
        ereaderTrainer->checksum += ((u32 *)ereaderTrainer)[i];
}

void ClearEReaderTrainer(struct BattleTowerEReaderTrainer *ereaderTrainer)
{
    u32 i;

    for (i = 0; i < sizeof(struct BattleTowerEReaderTrainer) / sizeof(u32); i++)
        ((u32 *)ereaderTrainer)[i] = 0;
}

void BufferEReaderTrainerGreeting(void)
{
    BufferBattleTowerTrainerMessage(gSaveBlock2Ptr->battleTower.ereaderTrainer.greeting);
}

static void PrintEReaderTrainerFarewellMessage(void)
{
    if (gBattleOutcome == B_OUTCOME_DREW)
        gStringVar4[0] = EOS;
    else if (gBattleOutcome == B_OUTCOME_WON)
        BufferBattleTowerTrainerMessage(gSaveBlock2Ptr->battleTower.ereaderTrainer.farewellPlayerWon);
    else
        BufferBattleTowerTrainerMessage(gSaveBlock2Ptr->battleTower.ereaderTrainer.farewellPlayerLost);
}

void Dummy_TryEnableBravoTrainerBattleTower(void)
{
    s32 i;

    for (i = 0; i < 2; i++)
    {
        if (gSaveBlock2Ptr->battleTower.var_4AE[i] == 1)
            TakeBravoTrainerBattleTowerOffTheAir();
    }
}
