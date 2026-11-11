#!/usr/bin/env python3
"""WRL-003 approved evolution-item purchase prices (no shop/location changes)."""
import json
from pathlib import Path

items = json.loads(Path("src/data/items.json").read_text())["items"]
from collections import Counter

counts = Counter(item["itemId"] for item in items)
# The upstream item table intentionally reuses ITEM_NONE for unused slots.
# Require exactly one row for each approved priced item instead.
by_id = {item["itemId"]: item for item in items}
prices = {
    "ITEM_FIRE_STONE": 2100,
    "ITEM_WATER_STONE": 2100,
    "ITEM_THUNDER_STONE": 2100,
    "ITEM_LEAF_STONE": 2100,
    "ITEM_SUN_STONE": 2100,
    "ITEM_MOON_STONE": 2100,
    "ITEM_KINGS_ROCK": 3000,
    "ITEM_METAL_COAT": 3000,
    "ITEM_DRAGON_SCALE": 3000,
    "ITEM_UP_GRADE": 3000,
}
for item_id, expected in prices.items():
    assert counts[item_id] == 1, (item_id, counts[item_id])
    assert by_id[item_id]["price"] == expected, (
        item_id, by_id[item_id]["price"], expected
    )
print("PASS: WRL-003 approved six stone and four trade-item purchase prices; "
      "availability, badge restrictions and vendor layout are not claimed.")
