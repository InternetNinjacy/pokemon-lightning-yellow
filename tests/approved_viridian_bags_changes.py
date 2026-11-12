"""Reverse only the 2026-11-12 eight ordinary Viridian Trainer-use bag amendment.

Full Heal and Super Potion already use ordinary FireRed battle item AI. This
module reverses exactly those eight Trainer inventories to preserve history.
"""
import re
from pathlib import Path

BAG_BEFORE = {"TAMER_COLE":".items = {}","BLACK_BELT_KIYO":".items = {}","COOLTRAINER_SAMUEL":".items = {ITEM_SUPER_POTION}","COOLTRAINER_YUJI":".items = {ITEM_HYPER_POTION}","BLACK_BELT_ATSUSHI":".items = {}","TAMER_JASON":".items = {}","COOLTRAINER_WARREN":".items = {ITEM_HYPER_POTION}","BLACK_BELT_TAKASHI":".items = {}"}
BAG_AFTER = ".items = {ITEM_SUPER_POTION, ITEM_FULL_HEAL}"

def prior_viridian_bags_source(path, data=None):
    if data is None:
        data = Path(path).read_text()
    if str(path) != "src/data/trainers.h":
        return data
    for trainer_id, previous in BAG_BEFORE.items():
        pattern = r"(\\[TRAINER_" + re.escape(trainer_id) + r"\\] = \\{[\\s\\S]*?\\n        )(\\.items = \\{[^}]*\\})"
        matches = list(re.finditer(pattern, data))
        assert len(matches) == 1, ("missing/duplicate trainer", trainer_id)
        match = matches[0]
        assert match.group(2) == BAG_AFTER, ("unexpected trainer item delta", trainer_id, match.group(2))
        data = data[:match.start(2)] + previous + data[match.end(2):]
    return data
