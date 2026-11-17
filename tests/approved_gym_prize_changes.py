"""Exact PR263 prize-only source projection; gameplay prize values stay unchanged."""
HUNK = "        else if (gTrainerBattleOpponent_A == TRAINER_LEADER_BROCK)\n        {\n            // Approved prize, independent of the revised party's final level.\n            moneyReward = 1400 * gBattleStruct->moneyMultiplier;\n        }\n        else if (gTrainerBattleOpponent_A == TRAINER_LEADER_MISTY)\n        {\n            // Approved total for this battle; do not double it for Doubles.\n            moneyReward = 3000 * gBattleStruct->moneyMultiplier;\n        }\n"
def prior_gym_prize_source(path,text):
    if str(path) != 'src/battle_script_commands.c':
        return text
    count=text.count(HUNK)
    assert count <= 1, 'Ambiguous approved Gym prize delta'
    return text.replace(HUNK,'',1) if count else text
