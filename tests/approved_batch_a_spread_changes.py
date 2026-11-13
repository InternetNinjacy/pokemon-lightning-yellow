"""Exact reversible Batch A spread-damage delta; historical hashes stay frozen."""
from pathlib import Path
import hashlib

PATH = "src/pokemon.c"
BASE_BLOB = "e00ce9a44cb056330d6204b3810e36c4da5331ab"
STAGED_BLOB = "893ea805ae6c17b6190a78e3b1be5def657c1447"
BEFORE = """        // Moves hitting both targets do half damage in double battles
        if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && gBattleMoves[move].target == MOVE_TARGET_BOTH && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE) == 2)
            damage /= 2;"""
AFTER = """        // Preserve Gen III's half damage for both-foe moves, while
        // all-other-battler moves use their approved 75% spread damage.
        if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE) == 2)
        {
            if (gBattleMoves[move].target == MOVE_TARGET_BOTH)
                damage /= 2;
            else if (gBattleMoves[move].target == MOVE_TARGET_FOES_AND_ALLY)
                damage = damage * 3 / 4;
        }"""

def blob(text):
    data = text.encode()
    return hashlib.sha1(b"blob " + str(len(data)).encode() + b"\0" + data).hexdigest()

def prior_batch_a_spread_source(path, text=None):
    path = str(path)
    if text is None:
        text = Path(path).read_text()
    if path != PATH:
        return text
    # Reverse only the exact separately approved current-main Corphish source delta.
    from approved_special_corphish_changes import prior_special_corphish_source
    import json
    hunks=json.loads(Path('tests/specifications/corphish_drizzle_delta.json').read_text())['hunks']
    if any(h['after'] in text for h in hunks):
        text=prior_special_corphish_source(path,text)
    count = text.count(AFTER)
    if count == 0:
        return text  # Already projected; frozen historical guards still apply.
    assert count == 2, "Partial/unreviewed Batch A spread delta"
    assert blob(text) == STAGED_BLOB, "Unreviewed Batch A production spread source"
    prior = text.replace(AFTER, BEFORE)
    assert blob(prior) == BASE_BLOB, "Batch A spread does not reverse to exact parent"
    return prior
