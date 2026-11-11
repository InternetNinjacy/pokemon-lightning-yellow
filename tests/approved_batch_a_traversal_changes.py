"""Strict reversible projection of the approved BTL-005 all-target traversal.

Preserves preexisting weather/BTL frozen integration hashes byte-for-byte.
Neither the staged nor predecessor source SHA-1 may drift unnoticed.
"""
import hashlib

PATH = "src/battle_script_commands.c"
STAGED_BLOB = "ed6b6d863f22e17d04ee9a40da05bb6814fb389d"
PARENT_BLOB = "a5f5aef00fd95d27db0e119061c6fdb3b785fa6d"
HUNK = "            // A move targeting every other battler must visit the second foe\n            // and then the user's ally. Retain MOVE_TARGET_BOTH's original\n            // two-opponent handling and its normal 50% damage behavior.\n            if (!(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)\n                && (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)\n                && !gProtectStructs[gBattlerAttacker].chargingTurn\n                && gBattleMoves[gCurrentMove].target == MOVE_TARGET_FOES_AND_ALLY\n                && GetBattlerSide(gBattlerTarget) != GetBattlerSide(gBattlerAttacker))\n            {\n                u8 nextTarget;\n\n                if (!(gHitMarker & HITMARKER_NO_ATTACKSTRING))\n                {\n                    // The first recipient was a foe: try their partner.\n                    nextTarget = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gBattlerTarget)));\n                    if (gBattleMons[nextTarget].hp == 0)\n                        nextTarget = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gBattlerAttacker)));\n                }\n                else\n                {\n                    // After the second foe, the only remaining recipient is\n                    // the attacker's partner. Do not bounce between foes.\n                    nextTarget = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gBattlerAttacker)));\n                }\n\n                if (nextTarget != gBattlerAttacker && gBattleMons[nextTarget].hp != 0)\n                {\n                    gBattlerTarget = nextTarget;\n                    gHitMarker |= HITMARKER_NO_ATTACKSTRING;\n                    gBattleScripting.moveendState = 0;\n                    MoveValuesCleanUp();\n                    BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);\n                    gBattlescriptCurrInstr = BattleScript_FlushMessageBox;\n                    return;\n                }\n            }\n"


def _blob(text):
    data = text.encode()
    return hashlib.sha1(b"blob " + str(len(data)).encode() + b"\\0" + data).hexdigest()


def prior_batch_a_traversal_source(path, text):
    if str(path) != PATH:
        return text
    if HUNK not in text:
        return text  # An ancestor projection may already have reversed it.
    assert text.count(HUNK) == 1, "Ambiguous Batch A traversal source"
    assert _blob(text) == STAGED_BLOB, "Unapproved Batch A traversal source"
    previous = text.replace(HUNK, "", 1)
    assert _blob(previous) == PARENT_BLOB, "Batch A traversal does not reverse to parent"
    return previous
