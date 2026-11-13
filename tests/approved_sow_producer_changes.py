"""Reversible WTH-001 scripted encounter producer source delta for old audits."""
from pathlib import Path

EXTRA_INCLUDES = "\n#include \"pokemon.h\"\n#include \"constants/moves.h\"\n#include \"constants/abilities.h\"\n#include \"constants/special_weather.h\""
EXTRA_FUNCTION = "// Configure the already-created (setwildbattle) *single* special encounter.\n// Never recreate the monster after choosing its personality/individual ability.\n// No held items or species-wide ability replacement is permitted here.\nvoid SowPrepareScriptedEncounter(void)\n{\n    static const struct\n    {\n        u16 species;\n        u8 level;\n        u16 moves[MAX_MON_MOVES];\n    } eventData[SPECIAL_WEATHER_COUNT] =\n    {\n        [SPECIAL_WEATHER_CORPHISH] = {SPECIES_CORPHISH, 30, {MOVE_BUBBLE_BEAM, MOVE_KNOCK_OFF, MOVE_PROTECT, MOVE_VICE_GRIP}},\n        [SPECIAL_WEATHER_GLALIE] = {SPECIES_GLALIE, 40, {MOVE_ICE_BEAM, MOVE_CRUNCH, MOVE_HEADBUTT, MOVE_PROTECT}},\n        [SPECIAL_WEATHER_TYRANITAR] = {SPECIES_TYRANITAR, 55, {MOVE_CRUNCH, MOVE_ROCK_SLIDE, MOVE_THRASH, MOVE_SCARY_FACE}},\n    };\n    u16 event = gSpecialVar_0x8004;\n    u8 slot;\n    gSpecialVar_Result = FALSE;\n    if (event >= SPECIAL_WEATHER_COUNT\n     || GetMonData(&gEnemyParty[0], MON_DATA_SPECIES) != eventData[event].species\n     || GetMonData(&gEnemyParty[0], MON_DATA_LEVEL) != eventData[event].level\n     || GetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM) != ITEM_NONE)\n        return;\n    for (slot = 0; slot < MAX_MON_MOVES; slot++)\n        SetMonMoveSlot(&gEnemyParty[0], eventData[event].moves[slot], slot);\n    if (event == SPECIAL_WEATHER_CORPHISH\n     && !SetSpecialEncounterAbility(&gEnemyParty[0], ABILITY_DRIZZLE))\n        return;\n    if (event == SPECIAL_WEATHER_GLALIE\n     && !SetSpecialEncounterAbility(&gEnemyParty[0], ABILITY_SNOW_WARNING))\n        return;\n    if (event == SPECIAL_WEATHER_TYRANITAR\n     && GetMonAbility(&gEnemyParty[0]) != ABILITY_SAND_STREAM)\n        return;\n    gSpecialVar_Result = TRUE;\n}\n\n"


def prior_sow_producer_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    if str(path) != "src/script_pokemon_util.c":
        return text
    if EXTRA_FUNCTION in text:
        assert text.count(EXTRA_FUNCTION) == 1, "Unexpected SOW constructor edits"
        assert text.count(EXTRA_INCLUDES) == 1, "Unexpected SOW include edits"
        text = text.replace(EXTRA_FUNCTION, "", 1).replace(EXTRA_INCLUDES, "", 1)
    return text
