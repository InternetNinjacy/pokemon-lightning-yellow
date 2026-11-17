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

# The approved item prices must reach the real Poké Mart purchase path.
# This is source-path acceptance, not proof that the items are stocked.
import re

shop = Path("src/shop.c").read_text()

def function_body(name):
    match = re.search(r"\bstatic void " + re.escape(name) + r"\([^;]*\)\s*\{", shop)
    assert match, f"Missing production shop function: {name}"
    depth = 1
    pos = match.end()
    start = pos
    while depth:
        assert pos < len(shop), f"Unclosed shop function: {name}"
        depth += (shop[pos] == "{") - (shop[pos] == "}")
        pos += 1
    return shop[start:pos - 1]

assert "ItemId_GetPrice(item)" in function_body("BuyMenuPrintPriceInList")
assert "GetMoney(&gSaveBlock1Ptr->money) / ItemId_GetPrice(tItemId)" in function_body("Task_BuyHowManyDialogueInit")
assert "sShopData.itemPrice = ItemId_GetPrice(tItemId) * tItemCount;" in function_body("Task_BuyHowManyDialogueHandleInput")
assert "TryBuyBallsWithPremierBonus(tItemId, tItemCount)" in function_body("BuyMenuTryMakePurchase")
assert "BuyMenuSubtractMoney" in function_body("BuyMenuTryMakePurchase")
assert "RemoveMoney(&gSaveBlock1Ptr->money, sShopData.itemPrice);" in function_body("BuyMenuSubtractMoney")
print("PASS: production Poké Mart displays item prices, limits affordable quantity, "
      "charges the dynamic price per item, and deducts the confirmed total; "
      "no vendor stock or natural purchase is asserted.")
