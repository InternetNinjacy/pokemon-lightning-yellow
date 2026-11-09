"""Reverse precisely the two 2026-10-31 owner-locked Ponyta-family profiles.

Only used to compare older source-history guards against their unchanged
historical baselines; the forward exact profiles are separately asserted by
test_pkm009_ponyta_family.py.
"""
import re

_ORIGINAL_LEARNSETS = {
    "Ponyta": [[1,'QUICK_ATTACK'],[5,'GROWL'],[9,'TAIL_WHIP'],[14,'EMBER'],[19,'STOMP'],[25,'FIRE_SPIN'],[31,'TAKE_DOWN'],[38,'AGILITY'],[45,'BOUNCE'],[53,'FIRE_BLAST']],
    "Rapidash": [[1,'QUICK_ATTACK'],[1,'GROWL'],[1,'TAIL_WHIP'],[1,'EMBER'],[5,'GROWL'],[9,'TAIL_WHIP'],[14,'EMBER'],[19,'STOMP'],[25,'FIRE_SPIN'],[31,'TAKE_DOWN'],[38,'AGILITY'],[40,'FURY_ATTACK'],[50,'BOUNCE'],[63,'FIRE_BLAST']],
}
_EXPECTED_NEW_STATS = {
    "PONYTA": (100, 50, 85, 65),
    "RAPIDASH": (120, 60, 100, 80),
}

def prior_pkm009_ponyta_source(path, text):
    if path == "src/data/pokemon/species_info.h":
        for name, (atk, spa, old_atk, old_spa) in _EXPECTED_NEW_STATS.items():
            pattern = r"(\[SPECIES_" + name + r"\]\s*=\s*\{)(.*?)(\n    \},)"
            m = re.search(pattern, text, re.S)
            assert m, "Missing Ponyta-family species: " + name
            body = m.group(2)
            for field, new, old in (("baseAttack", atk, old_atk),
                                    ("baseSpAttack", spa, old_spa)):
                token = "." + field + " = " + str(new) + ","
                assert body.count(token) == 1, "Unreviewed stat: " + name + field
                body = body.replace(token, "." + field + " = " + str(old) + ",", 1)
            text = text[:m.start(2)] + body + text[m.end(2):]
    elif path == "src/data/pokemon/level_up_learnsets.h":
        from test_pkm009_ponyta_family import EXPECT
        for name, original in _ORIGINAL_LEARNSETS.items():
            pattern = r"(static const u16 s" + name + r"LevelUpLearnset\[\] = \{)(.*?)(\n\};)"
            m = re.search(pattern, text, re.S)
            assert m, "Missing Ponyta-family learnset: " + name
            body = m.group(2)
            actual = [(int(lvl), move) for lvl, move in re.findall(
                r"LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)", body)]
            assert actual == EXPECT[name]["learn"], "Unreviewed learnset: " + name
            assert body.count("LEVEL_UP_END") == 1
            old_body = "\n" + "\n".join("    LEVEL_UP_MOVE(%d, MOVE_%s)," % (lv, mv)
                                         for lv, mv in original) + "\n    LEVEL_UP_END"
            text = text[:m.start(2)] + old_body + text[m.end(2):]
    return text
