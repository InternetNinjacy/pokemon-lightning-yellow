#!/usr/bin/env python3
"""Exact owner-amended ordinary Viridian trainer-use Super Potion + Full Heal contract."""
from pathlib import Path
import re
from approved_viridian_bags_changes import BAG_BEFORE, BAG_AFTER, prior_viridian_bags_source
p = Path("src/data/trainers.h").read_text()
for trainer_id, before in BAG_BEFORE.items():
    pattern = r"\[TRAINER_" + re.escape(trainer_id) + r"\] = \{([\s\S]*?)\n    \},"
    matches = list(re.finditer(pattern, p))
    assert len(matches) == 1, trainer_id
    block = matches[0].group(1)
    assert block.count(BAG_AFTER) == 1, (trainer_id, "missing or duplicated item bag")
    assert ".doubleBattle = FALSE" in block, trainer_id
    assert "ITEM_REVIVE" not in block and "ITEM_MAX_REVIVE" not in block, trainer_id
restored = prior_viridian_bags_source("src/data/trainers.h", p)
for trainer_id, before in BAG_BEFORE.items():
    m = re.search(r"\[TRAINER_" + re.escape(trainer_id) + r"\] = \{([\s\S]*?)\n    \},", restored)
    assert m and before in m.group(1), trainer_id
assert prior_viridian_bags_source("src/data/trainer_parties.h") == Path("src/data/trainer_parties.h").read_text()
print("PASS: all eight Viridian ordinary Trainer bags exactly one Super Potion + one Full Heal; zero Revives; original Singles, byte-exact historical inventory reversal. Other trainers and player items untouched.")
