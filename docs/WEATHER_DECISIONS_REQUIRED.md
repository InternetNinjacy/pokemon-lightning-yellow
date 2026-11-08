# Weather reconciliation — approved decisions and remaining implementation inputs

Reconciled 2026-11-08 after owner-authorized PR #128 merge `a89194f026dcedc8942b5de29f0d93eb4a7bb72a`. This is a **design/source handoff**, not a claim that the pending features are implemented or tested. The earlier PR127 snapshot incorrectly labeled two later, already-approved decisions as missing.

Authoritative later approvals (do not replace with this GitHub summary):
- [252-Species Balance, Learnsets & Trainer Progression Master Register](https://docs.google.com/document/d/1tIdBczPU7bD-31Hw2hyCJQOk_E0bN1DwgLxld-YtrK8/edit), **2026-11-06 — OWNER APPROVED FINAL WEATHER-ABILITY SLOTS (DESIGN TASK 2 CLOSED)**; explicitly supersedes earlier eligibility-only/unapproved-slot notes.
- [Special Overworld Pokémon & Environmental Events Register](https://docs.google.com/document/d/1yTOKL8qr5_3sX6Fpk9WEzTBw9c6IwoDVWLULJtTIXm8/edit), **2026-11-06 — OWNER APPROVAL: SOW-004 SAILOR GHOST FAREWELL FINAL WORDING**, superseding the provisional farewell; final route-wide restoration timing is recorded 2026-11-05.
- [Approved weather work order #47](https://github.com/InternetNinjacy/pokemon-lightning-yellow/issues/47) and [PR #128](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/128) for engineering scope and completed engine acceptance.

## Approved species-wide weather ability slots — DESIGN CLOSED; SOURCE IMPLEMENTATION OPEN

Each slash pair is ability **slot 1 / slot 2**, in order. Existing species arrays on main have **not** been changed by PR #128, which implemented scoped battle abilities and individual trainers/Glalie only.

| Species | Final approved slot 1 | Final approved slot 2 | Replaced native slot (if any) |
| --- | --- | --- | --- |
| MAGIKARP | SWIFT_SWIM | DRIZZLE | previously empty slot 2 |
| FEEBAS | SWIFT_SWIM | DRIZZLE | previously empty slot 2 |
| GOLDEEN | SWIFT_SWIM | DRIZZLE | WATER_VEIL |
| SEAKING | SWIFT_SWIM | DRIZZLE | WATER_VEIL |
| POLIWAG | WATER_ABSORB | DRIZZLE | DAMP |
| POLIWHIRL | WATER_ABSORB | DRIZZLE | DAMP |
| POLITOED | WATER_ABSORB | DRIZZLE | DAMP |
| SNORUNT | INNER_FOCUS | ICE_BODY | previously empty slot 2 |
| GLALIE | INNER_FOCUS | ICE_BODY | previously empty slot 2 |
| SPHEAL | THICK_FAT | ICE_BODY | previously empty slot 2 |
| SEALEO | THICK_FAT | ICE_BODY | previously empty slot 2 |
| WALREIN | THICK_FAT | ICE_BODY | previously empty slot 2 |
| ONIX | ROCK_HEAD | OVERCOAT | STURDY |
| STEELIX | ROCK_HEAD | OVERCOAT | STURDY |
| PINECO | STURDY | OVERCOAT | previously empty slot 2 |
| FORRETRESS | STURDY | OVERCOAT | previously empty slot 2 |

The owner explicitly accepted loss of Goldeen/Seaking Water Veil, Poliwag-family Damp and Onix/Steelix Sturdy. **Do not add these abilities to any other species.** The same-family pairs are consistent. Preserve special trainer overrides: Brock Snorunt Ice Body, Brock Onix Overcoat, Misty Starmie Drizzle. The SOW-007 caught Glalie must keep its **individual Snow Warning**, not switch to global Ice Body.

**Engineering remaining:** implement this table in the species source with separately reviewed PKM-009/species specialist coordination; exercise actual two-slot creation, battle entry, breeding/inheritance where relevant, save/load, PC transfers and evolution transitions. In particular, Magikarp→Gyarados, Feebas→Milotic and Poliwhirl→Poliwrath must **not** give the evolved species-wide Drizzle. The approved table does not by itself prescribe a save migration for preexisting Pokémon or specify the mechanism for slot remapping where the target's slot differs; verify existing engine behavior and raise an explicit choice if the tested behavior is inconsistent with the approved exclusion. No migration or inherited Drizzle is implicitly authorized.

## SOW-004 final Corphish farewell — DESIGN CLOSED; EVENT SCRIPT OPEN

Final owner-approved, verbatim dialogue after **capture or defeat**:
> At last... I can rest in peace...

This replaces the old provisional `I can finally rest...` text. Preserve the pre-Silph-Scope Lavender Tower unidentified-GHOST graphic if technically feasible with existing assets; do not create substitute art without approval. Complete this farewell **before** ending Route 12 forced rain and drawing exactly one Rain 50% / Snow 50% baseline result, all before releasing player control. Preserve the independent fishing-house sailor-brother legend/Super Rod and original Magikarp measurement flow; no event-state branch in the legend.

## WTH-001 / WTH-003 — already implemented and merged

- PR #29 merged the 83 explicitly approved ordinary map weather policies, night Sun→Neutral conversion, context persistence, same-map/indoor/other-map rules, Cycling Road post-battle rolls, RTC fallback/recovery and field-to-battle bridge.
- PR #128 merged the opt-in event lifecycle for forced Route12 Rain, both Route21 maps Snow/Hail, Route23 Sandstorm; persistent active/resolved flags, exact-once post-aftermath restoration and native render-ready wait. It also merged Ice Body/Overcoat/Snow Warning battle mechanics, Brock team/individual trainer abilities and captured individual Glalie Snow Warning preservation.
- Existing tests include actual compiled native field/battle/RTC/flash persistence and outcome-argument acceptance with exact-head modified/upstream CI. **No installed special encounter event producer exists yet**; a lifecycle function test is not a full end-to-end encounter playthrough. WTH-002 rendering/fog cross-system acceptance is tracked separately; merged TIM-003 palettes/Mansion fog do not earn WTH credit.

## Special encounter inputs still unresolved (not permission to guess)

- **SOW-004 Route12**: the approved wooden dock beside the fishing house determines the broad area, but exact object tile/technical placement, encounter level, four move slots, any held item and scripting placement must be source-validated/finalized. This one-off Corphish has **individual Drizzle** when encountered and if caught; evolution to Crawdaunt retaining/reverting the marker still needs an explicit rule.
- **SOW-007 Route21**: Surf-accessible grass just south of Pallet on **Route21 North**, while forced Snow covers both Route21 technical maps. Exact tile, level, four moves and any item remain pending. The one-off Glalie has **individual Snow Warning**, retained through capture/PC/save per PR #128.
- **SOW-008 Route23**: between badge checkpoint guards, without blocking paths/guard events. Exact between-checkpoint tile, level, four moves and any item remain pending. One-off Tyranitar uses its native **Sand Stream**, already tested through capture/PC storage. No additional event dialogue or lore is approved.

For each installed producer: generate the intended unique wild Pokémon without regeneration after its individual ability is set; activate the forced weather only when the actual reachable encounter exists; preserve native flee/loss/retry, map guards and save flags; on capture or defeat execute approved aftermath then the native wait resolver once. End-to-end tests must cover event object progression, capture/defeat/Run/loss, reload, re-entry, both Route21 map sections, RTC boundary, exactly-once RNG, field/battle weather and storage. Hardware, link, long organic story and interrupted-power release acceptance remain independent limitations.

## Ownership and accounting

This file corrects **stale design blockers only**; it does not alter source gameplay, create species/encounter placements, certify WTH-001/003 complete, award effort points or merge a gameplay PR. Species arrays and ability evolution testing intersect PKM-009 and require Master Control coordination; event producers touch the special-overworld workstream, not the WRL-002/009 ordinary-wild-table specialist. Codex remains dedicated to BTL-005. Master Control reconciles the permanent tracker/dashboard/append-only ledger after verified milestone acceptance.
