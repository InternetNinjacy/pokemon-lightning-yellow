# PKM-006 — Seven Island Battle Tower placement and integration boundary

Date: 2026-11-21. Draft PR #288. Source-only investigation; no build, emulator, test, diagnostics, CI or merge.

## Approved policy

Owner approved a **new Battle Tower on Seven Island**, **separate from the existing Trainer Tower**. Existing WRL/SOW ownership and normal progression must remain intact. This does not approve replacing an existing house, moving a town building, repurposing the Trainer Tower, or choosing an arbitrary tile/doorway without a map design.

## Actual maps and source evidence

- `data/maps/SevenIsland/map.json` has layout `LAYOUT_SEVEN_ISLAND` (24×20), north connection to `MAP_SEVEN_ISLAND_TRAINER_TOWER` offset −48, south connection to `MAP_SEVEN_ISLAND_SEVAULT_CANYON_ENTRANCE`. Four town warps: (11,9) House Room1, (5,9) Mart, (12,3) Pokémon Center, (16,13) Harbor. None is free.
- `data/maps/SevenIsland_TrainerTower/map.json` has an existing warp (58,7) to `MAP_TRAINER_TOWER_LOBBY` warp 1, plus existing trainers, hidden items and signs. Leave unchanged.
- `data/maps/SevenIsland_UnusedHouse/map.json` exists in `gMapGroup_IndoorSevenIsland`, uses `LAYOUT_HOUSE4` (11×9), and has **zero** warps, objects or background events; its script only contains a no-op map script. It is not an existing Battle Tower lobby or connected entrance.
- `data/layouts/RS_BattleTower` has a 29×30 layout `LAYOUT_RS_BATTLE_TOWER`, with `gTileset_General` and `gTileset_PewterCity`. It has no corresponding map JSON, scripts, warp or receptionist. The layout alone is not a complete facility and its tilesets do not establish a usable indoor reception area.
- `src/battle_tower.c` has the newly authored 100 normal opponents and 279 staged Battle Tower Pokémon replacements, but no corresponding Seven Island world entrance. Existing Trainer Tower map/script machinery is distinct.

## Safe implementation plan, pending one map-design input

1. Select the **exact Seven Island exterior doorway tile(s)** and decide whether the approved facility is represented by a new exterior building graphic/layout edit or a new annex entrance. Do not overwrite the four existing warps, NPC tiles or north Trainer Tower connection.
2. Create a dedicated `SevenIsland_BattleTowerLobby` map, registered in the Seven Island indoor map group, with an approved lobby layout and paired entry/exit warps. Do not rename or repurpose `SevenIsland_UnusedHouse` or `TrainerTower_Lobby` without separate approval.
3. Define receptionist, level-50/level-100 challenge choice, battle flow, return/exit, healing and challenge-state persistence against the actual Battle Tower C interfaces; do not substitute Trainer Tower specials without a verified API contract.
4. Apply the existing Seven Island arrival/progression gating policy, without inventing a new badge or post-League requirement. Only after the door and lobby are grounded, stage the paired map/warp/script source changes.

**Current status:** approved *location*, but the source contains no unoccupied Battle Tower doorway and no approved exterior map tile design. No production warp/map edits made because an arbitrary entrance could be inaccessible or overwrite existing town functionality. This is a precise integration blocker, not a claim that the 100-trainer source roster or 279 templates are gameplay accepted. Protected WRL wild slots and SOW story legendary scripts untouched. Preserve 50 IDs and 461 earned / 251 remaining / 712 total.
