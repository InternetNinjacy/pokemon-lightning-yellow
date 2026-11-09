"""Reverse only PKM-009 owner-locked Oddish-family insertions for old source guards.

The four frozen profile arrays are independently tested by
tests/test_pkm009_oddish_family.py; the reverse operation additionally requires
exact ordered inserted hunks and never masks unrelated learnset changes.
"""
import re

_INSERTIONS = {
    "Oddish": (
        "    LEVEL_UP_MOVE(7, MOVE_SWEET_SCENT),\n"
        "    LEVEL_UP_MOVE(12, MOVE_MEGA_DRAIN),",
        "    LEVEL_UP_MOVE(7, MOVE_SWEET_SCENT),"
    ),
    "Gloom": (
        "    LEVEL_UP_MOVE(14, MOVE_POISON_POWDER),\n"
        "    LEVEL_UP_MOVE(15, MOVE_MEGA_DRAIN),",
        "    LEVEL_UP_MOVE(14, MOVE_POISON_POWDER),"
    ),
    "Vileplume": (
        "    LEVEL_UP_MOVE(1, MOVE_MEGA_DRAIN),\n"
        "    LEVEL_UP_MOVE(24, MOVE_ACID),\n"
        "    LEVEL_UP_MOVE(30, MOVE_SLUDGE),\n"
        "    LEVEL_UP_MOVE(38, MOVE_MOONLIGHT),",
        "    LEVEL_UP_MOVE(1, MOVE_MEGA_DRAIN),"
    ),
    "VileplumeLate": (
        "    LEVEL_UP_MOVE(44, MOVE_PETAL_DANCE),\n"
        "    LEVEL_UP_MOVE(50, MOVE_SLUDGE_BOMB),",
        "    LEVEL_UP_MOVE(44, MOVE_PETAL_DANCE),"
    ),
    "Bellossom": (
        "    LEVEL_UP_MOVE(1, MOVE_MAGICAL_LEAF),\n"
        "    LEVEL_UP_MOVE(24, MOVE_MEGA_DRAIN),\n"
        "    LEVEL_UP_MOVE(30, MOVE_SLEEP_POWDER),\n"
        "    LEVEL_UP_MOVE(36, MOVE_SYNTHESIS),",
        "    LEVEL_UP_MOVE(1, MOVE_MAGICAL_LEAF),"
    ),
}

def prior_pkm009_oddish_source(text):
    for key, (after, before) in _INSERTIONS.items():
        species = key.removesuffix("Late")
        pattern = (r"(static const u16 s" + species
                   + r"LevelUpLearnset\[\] = \{)(.*?)(\n\};)")
        match = re.search(pattern, text, re.S)
        assert match is not None, "Missing PKM-009 array: " + species
        body = match.group(2)
        assert body.count(after) == 1, "Unreviewed PKM-009 hunk: " + key
        updated = body.replace(after, before, 1)
        text = text[:match.start(2)] + updated + text[match.end(2):]
    return text
