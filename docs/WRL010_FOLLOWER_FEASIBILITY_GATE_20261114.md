# WRL-010 — bounded follower implementation feasibility gate (2026-11-14)

**Status:** PARTIAL source inventory only; NOT gameplay implementation, NOT 252/252 art verification, NOT acceptance. Branch based on main `02240690581eb70d70219024fb7f55da2882308b`. No production code or assets changed.

## Approved authority and existing behavior reused

Merged specification: `implementation/selectable-following-pokemon.md` (PR #52, merge `97bcc9d623587e2086cfe8002ba009c98c11df52`). The approved rule is one selected **individual** Pokémon, independent of party order; faint hides without clearing; removing the selected individual clears; no fallback; starter Pikachu is the sole automatic assignment. Existing FireRed object-event, avatar, party, and PC systems are the reference, not a new battle or movement implementation.

## Verified current-main source locations

- `src/field_player_avatar.c`: FireRed player avatar and object-event movement integration; includes `event_object_movement.h`, `overworld.h`, `party_menu.h` and `field_player_avatar.h`. This is a reference integration surface, **not** evidence of existing follower behavior.
- `src/data/object_events/object_event_graphics_info.h`: current FireRed object graphics metadata uses explicit `ObjectEventGraphicsInfo` records with image tables, OAM, animations, palette slots and dimensions. This is a reference for per-species sprite mapping, **not** proof of 252 usable follower sprites.
- `src/pokemon_storage_system.c`: current storage operations expose `StorageGetCurrentBox`, `GetBoxMonDataAt` and `SetBoxMonDataAt`; these are reference operations, **not** sufficient evidence that a selected individual is cleared on every deposit/trade/release path.
- `include/global.h`: current save and Pokémon type definitions require further exact allocation inspection before any persistent field is introduced.
- `src/pokemon_menu.c` does **not** exist at this path; locate the real party-menu implementation instead of guessing or creating a parallel menu.

## Unresolved preimplementation gates — do not assume PASS

1. Derive the exact 252 roster from project species/Pokédex mapping; verify each follower sprite's walking directions, animations, palette, provenance and license. No 252/252 claim.
2. Compare FireRed follower-helper candidate and Emerald-derived assets against actual current engine APIs; review redistribution/license compatibility before copying code or graphics.
3. Inspect exact save allocations, party identity fields and every deposit/withdraw/trade/release/evolution hook. Do not store only a party slot or species; do not modify save layout without compatibility evidence.
4. Inspect existing `src/party_menu.c` (or actual path), object-event graphics ID limits, palette/OAM/VRAM limits and surf/bicycle/script suppression.
5. Locate and verify actual approved opening Pikachu and post-Parcel Eevee gift scripts before wiring automatic assignment.
6. Validate with one scoped state/identity test after implementation; full exact-head modified and independent upstream baseline CI plus native gameplay acceptance are required before merge.

## Unique new behavior and next executable increment

The first gameplay increment should be **stable instance selection plus explicit clear** with no automatic fallback, based on verified existing party/storage APIs and a proven compatible save location. Follow with a narrowly scoped native state test (duplicate-species and reorder/deposit), then the approved Pikachu script and object rendering. Do not import bulk art or invent state migration while gates 1–5 are unresolved.

**Ownership:** WRL-010 audit only; no claim over WRL-002/009, BTL-005, WTH-002, historical PR #233, or the open validation/Gym branches. No points awarded; 50 IDs, 461 earned / 251 remaining / 712 total remain unchanged.
