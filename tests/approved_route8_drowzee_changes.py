"""Strict WRL-009 Route 8 Drowzee family introduction, reversible delta.

Only the third 10%-weight Ponyta slot is replaced by Drowzee Lv20 in each
version. Does not permit changes to Route 11's owner-approved roster.
"""
import re

ORIGINAL_ROUTE8 = {
  "sRoute8_FireRed": [
    [
      16,
      16,
      "SPECIES_PONYTA"
    ],
    [
      21,
      21,
      "SPECIES_HOUNDOUR"
    ],
    [
      20,
      20,
      "SPECIES_PONYTA"
    ],
    [
      19,
      19,
      "SPECIES_HOUNDOUR"
    ],
    [
      18,
      18,
      "SPECIES_CACNEA"
    ],
    [
      20,
      20,
      "SPECIES_CACNEA"
    ],
    [
      16,
      16,
      "SPECIES_PONYTA"
    ],
    [
      21,
      21,
      "SPECIES_NATU"
    ],
    [
      17,
      17,
      "SPECIES_NATU"
    ],
    [
      19,
      19,
      "SPECIES_NATU"
    ],
    [
      21,
      21,
      "SPECIES_NATU"
    ],
    [
      17,
      17,
      "SPECIES_NATU"
    ]
  ],
  "sRoute8_LeafGreen": [
    [
      16,
      16,
      "SPECIES_PONYTA"
    ],
    [
      21,
      21,
      "SPECIES_HOUNDOUR"
    ],
    [
      20,
      20,
      "SPECIES_PONYTA"
    ],
    [
      19,
      19,
      "SPECIES_HOUNDOUR"
    ],
    [
      18,
      18,
      "SPECIES_CACNEA"
    ],
    [
      20,
      20,
      "SPECIES_CACNEA"
    ],
    [
      16,
      16,
      "SPECIES_PONYTA"
    ],
    [
      21,
      21,
      "SPECIES_NATU"
    ],
    [
      17,
      17,
      "SPECIES_NATU"
    ],
    [
      19,
      19,
      "SPECIES_NATU"
    ],
    [
      21,
      21,
      "SPECIES_NATU"
    ],
    [
      17,
      17,
      "SPECIES_NATU"
    ]
  ]
}
DELEGATED_ROUTE8 = {
  "sRoute8_FireRed": [
    [
      16,
      16,
      "SPECIES_PONYTA"
    ],
    [
      21,
      21,
      "SPECIES_HOUNDOUR"
    ],
    [
      20,
      20,
      "SPECIES_DROWZEE"
    ],
    [
      19,
      19,
      "SPECIES_HOUNDOUR"
    ],
    [
      18,
      18,
      "SPECIES_CACNEA"
    ],
    [
      20,
      20,
      "SPECIES_CACNEA"
    ],
    [
      16,
      16,
      "SPECIES_PONYTA"
    ],
    [
      21,
      21,
      "SPECIES_NATU"
    ],
    [
      17,
      17,
      "SPECIES_NATU"
    ],
    [
      19,
      19,
      "SPECIES_NATU"
    ],
    [
      21,
      21,
      "SPECIES_NATU"
    ],
    [
      17,
      17,
      "SPECIES_NATU"
    ]
  ],
  "sRoute8_LeafGreen": [
    [
      16,
      16,
      "SPECIES_PONYTA"
    ],
    [
      21,
      21,
      "SPECIES_HOUNDOUR"
    ],
    [
      20,
      20,
      "SPECIES_DROWZEE"
    ],
    [
      19,
      19,
      "SPECIES_HOUNDOUR"
    ],
    [
      18,
      18,
      "SPECIES_CACNEA"
    ],
    [
      20,
      20,
      "SPECIES_CACNEA"
    ],
    [
      16,
      16,
      "SPECIES_PONYTA"
    ],
    [
      21,
      21,
      "SPECIES_NATU"
    ],
    [
      17,
      17,
      "SPECIES_NATU"
    ],
    [
      19,
      19,
      "SPECIES_NATU"
    ],
    [
      21,
      21,
      "SPECIES_NATU"
    ],
    [
      17,
      17,
      "SPECIES_NATU"
    ]
  ]
}

_SLOT = re.compile(r'("min_level": )(\d+)(,\s+"max_level": )(\d+)(,\s+"species": ")(SPECIES_[A-Z0-9_]+)(")')

def restore_route8_text(path, source):
    if str(path) != "src/data/wild_encounters.json":
        return source
    for label in ("sRoute8_FireRed", "sRoute8_LeafGreen"):
        token = '"base_label": "' + label + '"'
        assert source.count(token) == 1, label
        start = source.index('"mons": [', source.index(token))
        end = source.index("]", start)
        section = source[start:end]
        matches = list(_SLOT.finditer(section))
        actual = [(int(m.group(2)),int(m.group(4)),m.group(6)) for m in matches]
        assert actual == [tuple(row) for row in DELEGATED_ROUTE8[label]], label
        previous = iter(ORIGINAL_ROUTE8[label])
        def revert(m):
            lo,hi,sp = next(previous)
            return m.group(1)+str(lo)+m.group(3)+str(hi)+m.group(5)+sp+m.group(7)
        restored,count = _SLOT.subn(revert, section)
        assert count == 12, (label,count)
        source = source[:start] + restored + source[end:]
    return source
