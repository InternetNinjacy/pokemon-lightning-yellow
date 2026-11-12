"""Strict, reversible Thunder Wave completion fix over the existing approved Battle source.

Restores the exact Batch A traversal blob before frozen ancestor projections.
Retains independently reviewed historical hashes and rejects any other source drift.
"""
import hashlib

PATH = "src/battle_script_commands.c"
STAGED_BLOB = "98cb2bf3b5ec53435a99cfa54a45f7cd442d4fa9"
PARENT_BLOB = "ed6b6d863f22e17d04ee9a40da05bb6814fb389d"
BEFORE = "            if (!(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)\n                && (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)\n                && !gProtectStructs[gBattlerAttacker].chargingTurn\n                && gBattleMoves[gCurrentMove].target == MOVE_TARGET_FOES_AND_ALLY\n                && GetBattlerSide(gBattlerTarget) != GetBattlerSide(gBattlerAttacker))"
AFTER = "            // Thunder Wave already consumed its three recipients above. The\n            // ordinary spread-move loop would otherwise replay the ally after\n            // the final foe, extending battle text and applying the effect twice.\n            if (gCurrentMove != MOVE_THUNDER_WAVE\n                && !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)\n                && (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)\n                && !gProtectStructs[gBattlerAttacker].chargingTurn\n                && gBattleMoves[gCurrentMove].target == MOVE_TARGET_FOES_AND_ALLY\n                && GetBattlerSide(gBattlerTarget) != GetBattlerSide(gBattlerAttacker))"

def _blob(text):
    data = text.encode()
    return hashlib.sha1(b"blob " + str(len(data)).encode() + b"\0" + data).hexdigest()

def prior_thunder_wave_completion_source(path, text):
    if str(path) != PATH:
        return text
    if AFTER not in text:
        return text  # Already projected by a strict ancestor caller.
    assert text.count(AFTER) == 1, "Ambiguous Thunder Wave completion source"
    assert _blob(text) == STAGED_BLOB, "Unapproved Thunder Wave production source"
    previous = text.replace(AFTER, BEFORE, 1)
    assert _blob(previous) == PARENT_BLOB, "Thunder Wave fix does not reverse to exact parent"
    return previous
