"""WRL-009 exact reversible Route 5 approved 3-species land delta.

This is a source-preservation bridge, NOT a waiver of frozen source checks.
Only both explicitly approved Route 5 version tables may be reversed.
"""
import re

ORIGINAL_ROUTE5 = {
  "sRoute5_FireRed": [
    [
      10,
      10,
      "SPECIES_MEOWTH"
    ],
    [
      13,
      13,
      "SPECIES_PIDGEY"
    ],
    [
      13,
      13,
      "SPECIES_ODDISH"
    ],
    [
      12,
      12,
      "SPECIES_MEOWTH"
    ],
    [
      15,
      15,
      "SPECIES_ODDISH"
    ],
    [
      15,
      15,
      "SPECIES_PIDGEY"
    ],
    [
      16,
      16,
      "SPECIES_ODDISH"
    ],
    [
      16,
      16,
      "SPECIES_PIDGEY"
    ],
    [
      15,
      15,
      "SPECIES_PIDGEY"
    ],
    [
      14,
      14,
      "SPECIES_MEOWTH"
    ],
    [
      15,
      15,
      "SPECIES_PIDGEY"
    ],
    [
      16,
      16,
      "SPECIES_MEOWTH"
    ]
  ],
  "sRoute5_LeafGreen": [
    [
      10,
      10,
      "SPECIES_MEOWTH"
    ],
    [
      13,
      13,
      "SPECIES_PIDGEY"
    ],
    [
      13,
      13,
      "SPECIES_BELLSPROUT"
    ],
    [
      12,
      12,
      "SPECIES_MEOWTH"
    ],
    [
      15,
      15,
      "SPECIES_BELLSPROUT"
    ],
    [
      15,
      15,
      "SPECIES_PIDGEY"
    ],
    [
      16,
      16,
      "SPECIES_BELLSPROUT"
    ],
    [
      16,
      16,
      "SPECIES_PIDGEY"
    ],
    [
      15,
      15,
      "SPECIES_PIDGEY"
    ],
    [
      14,
      14,
      "SPECIES_MEOWTH"
    ],
    [
      15,
      15,
      "SPECIES_PIDGEY"
    ],
    [
      16,
      16,
      "SPECIES_MEOWTH"
    ]
  ]
}
APPROVED_ROUTE5 = {
  "sRoute5_FireRed": [
    [
      10,
      10,
      "SPECIES_RATTATA"
    ],
    [
      12,
      12,
      "SPECIES_SKITTY"
    ],
    [
      11,
      11,
      "SPECIES_RATTATA"
    ],
    [
      13,
      13,
      "SPECIES_SKITTY"
    ],
    [
      12,
      12,
      "SPECIES_RATTATA"
    ],
    [
      14,
      14,
      "SPECIES_SKITTY"
    ],
    [
      13,
      13,
      "SPECIES_RATTATA"
    ],
    [
      13,
      13,
      "SPECIES_DITTO"
    ],
    [
      14,
      14,
      "SPECIES_DITTO"
    ],
    [
      12,
      12,
      "SPECIES_RATTATA"
    ],
    [
      15,
      15,
      "SPECIES_DITTO"
    ],
    [
      11,
      11,
      "SPECIES_RATTATA"
    ]
  ],
  "sRoute5_LeafGreen": [
    [
      10,
      10,
      "SPECIES_RATTATA"
    ],
    [
      12,
      12,
      "SPECIES_SKITTY"
    ],
    [
      11,
      11,
      "SPECIES_RATTATA"
    ],
    [
      13,
      13,
      "SPECIES_SKITTY"
    ],
    [
      12,
      12,
      "SPECIES_RATTATA"
    ],
    [
      14,
      14,
      "SPECIES_SKITTY"
    ],
    [
      13,
      13,
      "SPECIES_RATTATA"
    ],
    [
      13,
      13,
      "SPECIES_DITTO"
    ],
    [
      14,
      14,
      "SPECIES_DITTO"
    ],
    [
      12,
      12,
      "SPECIES_RATTATA"
    ],
    [
      15,
      15,
      "SPECIES_DITTO"
    ],
    [
      11,
      11,
      "SPECIES_RATTATA"
    ]
  ]
}

_SLOT = re.compile(r'("min_level": )(\\d+)(,\\s+"max_level": )(\\d+)(,\\s+"species": ")(SPECIES_[A-Z0-9_]+)(")')

def restore_route5_text(path, source):
    if str(path) != "src/data/wild_encounters.json":
        return source
    for label in ("sRoute5_FireRed", "sRoute5_LeafGreen"):
        key = '"base_label": "' + label + '"'
        assert source.count(key) == 1, label
        start = source.index('"mons": [', source.index(key))
        stop = source.index("]", start)
        current = source[start:stop]
        actual = [(int(m.group(2)), int(m.group(4)), m.group(6)) for m in _SLOT.finditer(current)]
        assert actual == [tuple(row) for row in APPROVED_ROUTE5[label]], (label, actual)
        replacements = iter(ORIGINAL_ROUTE5[label])
        def old_slot(match):
            low, high, species = next(replacements)
            return (match.group(1) + str(low) + match.group(3) + str(high)
                    + match.group(5) + species + match.group(7))
        restored, count = _SLOT.subn(old_slot, current)
        assert count == 12, (label, count)
        source = source[:start] + restored + source[stop:]
    return source
