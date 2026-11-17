"""Exact reviewed 24 type-pair reversal for old species source hash snapshots."""
import json
import re
from pathlib import Path

DELTAS = json.loads(Path("tests/specifications/approved_typing_delta.json").read_text())["species"]


def prior_approved_typing_text(text):
    assert len(DELTAS) == 24
    for species, row in DELTAS.items():
        pat = r"(\[SPECIES_" + species + r"\]\s*=\s*\{.*?\.types = \{)(TYPE_\w+), (TYPE_\w+)(\},)"
        match = re.search(pat, text, re.S)
        assert match and [match[2][5:], match[3][5:]] == row["approved"], species
        old = "TYPE_" + row["before"][0] + ", TYPE_" + row["before"][1]
        text, count = re.subn(pat, lambda m: m[1] + old + m[4], text, count=1, flags=re.S)
        assert count == 1, species
    return text
