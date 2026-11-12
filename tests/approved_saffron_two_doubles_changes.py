"""Reverse exactly two owner-approved Saffron ordinary Double metadata changes."""
from pathlib import Path
import re

TRAINERS = ("TRAINER_PSYCHIC_TYRON", "TRAINER_CHANNELER_AMANDA")

def prior_saffron_two_doubles_source(path, data=None):
    if data is None:
        data = Path(path).read_text()
    if str(path) != "src/data/trainers.h":
        return data
    for trainer in TRAINERS:
        pattern = re.compile(r"(\[" + re.escape(trainer) + r"\] = \{[\s\S]*?\.items = \{\},\n        )\.doubleBattle = TRUE")
        matches = list(pattern.finditer(data))
        assert len(matches) == 1, "Saffron approved Double metadata missing/duplicated: " + trainer
        data = pattern.sub(lambda m: m.group(1) + ".doubleBattle = FALSE", data, count=1)
    return data
