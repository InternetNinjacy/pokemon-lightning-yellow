# Early catch rates through Misty

Status: owner-directed merge completed in PR251 at `3ed6ac1e18cc774344c5f242cfdd5020e33b83b8`; static source checks passed, full/native/upstream CI and owner playtest not newly executed. Prepared November 15, 2026 from main `02240690581eb70d70219024fb7f55da2882308b`.

The owner approved setting all 13 previously proposed catch-rate changes to **125** and requested implementation and merging. This supersedes the draft190/150/120 tiers. The other38 directly catchable species keep their original rates. All17 pre-Brock direct catches have rates of at least125; the complete early review still contains51 species. Electrike remains120 because it was not one of the13 proposed changes.

Igglybuff is an explicit consequence of the uniform-value instruction: its original170 becomes125, making it harder than original main but easier than many low-rate early replacements. The other12 changes increase their original rates. No unrelated species is reduced.

These are internal values on a 1–255 scale, not percentages. Ball type, current HP, status, and the unchanged capture formula still determine a throw’s actual outcome. A rate of 125 does not mean a guaranteed catch.

## Scope and availability

The boundary is the normal progression frontier available before defeating Misty, including optional travel after helping Bill: Routes 5/6, Vermilion, Route 11 and Diglett’s Cave. It does not extend the assignment to later regions newly reachable using Cut after Misty. Both FireRed and LeafGreen source rows are audited; this is not a new native grass-access or whole-story traversal certificate.

- Before Brock: Routes 1, 2 and 22, Viridian Forest, and Old Rod fishing in accessible Pallet/Viridian/Route22 water.
- After Brock through the Misty frontier: Route 3, all three Mt. Moon floors, Route 4, Cerulean, Routes 24/25, plus the optional southern branch listed above.
- Current main relocates the Old Rod to `data/maps/ViridianCity_House/scripts.inc`; no badge requirement. Old Rod slots 0/1 are included. Good Rod is locked behind badge 3 in `data/maps/Route11_EastEntrance_1F/scripts.inc`, so its slots are excluded. Surf, Rock Smash and Super Rod catches are excluded.
- `src/wild_double_battle.c` plus `include/wild_double_battle.h` still enable the Mt. Moon 1F 5% Onix/Cloyster special pair when two party Pokémon are usable. Both species are included; pair frequency and survivor capture rules are unchanged. The temporary pair is existing code, not a newly approved permanent encounter.
- Route 9/Cut-dependent travel, Route 23 badge-gated grass, Surf-only table rows, stationary later-story specials and external/link trades are excluded. Early shops, rod rewards, NPC routes, levels and flags are unchanged.

## Owner-approved changes

| Pokémon | Earliest catch venue | Original main | Approved | Authority |
|---|---|---:|---:|---|
| Igglybuff | Route1 | 170 | **125** | Owner instruction: uniform125 for all13 proposed species. |
| Teddiursa | Route2 | 120 | **125** | Owner instruction: uniform125 for all13 proposed species. |
| Elekid | Viridian Forest | 45 | **125** | Owner instruction: uniform125 for all13 proposed species. |
| Tyrogue | Route22 | 75 | **125** | Owner instruction: uniform125 for all13 proposed species. |
| Magby | Route3 | 45 | **125** | Owner instruction: uniform125 for all13 proposed species. |
| Castform | Route3 | 45 | **125** | Owner instruction: uniform125 for all13 proposed species. |
| Smoochum | Mt. Moon B2F | 45 | **125** | Owner instruction: uniform125 for all13 proposed species. |
| Sneasel | Mt. Moon B2F | 60 | **125** | Owner instruction: uniform125 for all13 proposed species. |
| Onix | Mt. Moon 1F | 45 | **125** | Owner instruction: uniform125 for all13 proposed species. |
| Cloyster | Mt. Moon 1F | 60 | **125** | Owner instruction: uniform125 for all13 proposed species. |
| Ditto | Route5 | 35 | **125** | Owner instruction: uniform125 for all13 proposed species. |
| Farfetch’d | Route11 | 45 | **125** | Owner instruction: uniform125 for all13 proposed species. |
| Dugtrio | Digletts Cave B1F | 50 | **125** | Owner instruction: uniform125 for all13 proposed species. |

The only production delta is these 13 owner-approved `.catchRate` numbers in `src/data/pokemon/species_info.h`. All other species-file bytes are identical to pinned main. Catch rates are species-wide, so later encounters use these same approved rates; Igglybuff becomes harder than its original170 and the other12 species become easier. This is intentional carryover of the chosen data-only approach; no per-map capture rule or engine override is introduced. Stats, types, abilities, moves, held items, EXP yield, evolution, encounter tables, gift ownership and save layout do not change.

## Complete direct-capture review

| Pokémon | Earliest stage | Catch venues | Original → approved |
|---|---|---|---:|
| Abra | Through Misty | Route24 | 200 → 200 |
| Bellsprout | Through Misty | Route25 | 255 → 255 |
| Castform | Through Misty | Route3 | 45 → 125 |
| Caterpie | Before Brock | Viridian Forest | 255 → 255 |
| Cleffa | Through Misty | Mt. Moon 1F | 150 → 150 |
| Cloyster | Through Misty | Mt. Moon 1F | 60 → 125 |
| Diglett | Through Misty | Digletts Cave B1F | 255 → 255 |
| Ditto | Through Misty | Route5 | 35 → 125 |
| Dugtrio | Through Misty | Digletts Cave B1F | 50 → 125 |
| Ekans | Through Misty | Route4 | 255 → 255 |
| Electrike | Through Misty | Route11 | 120 → 120 |
| Elekid | Before Brock | Viridian Forest | 45 → 125 |
| Farfetch’d | Through Misty | Route11 | 45 → 125 |
| Geodude | Through Misty | Mt. Moon 1F, Mt. Moon B1F | 255 → 255 |
| Hoothoot | Before Brock | Route2 | 255 → 255 |
| Igglybuff | Before Brock | Route1 | 170 → 125 |
| Magby | Through Misty | Route3 | 45 → 125 |
| Magikarp | Before Brock | Cerulean City, Pallet Town, Route11, Route22, Route24, Route25, Route4, Route6, Vermilion City, Viridian City | 255 → 255 |
| Mankey | Before Brock | Route22 | 190 → 190 |
| Mareep | Through Misty | Route6 | 235 → 235 |
| Meowth | Through Misty | Route3 | 255 → 255 |
| Nidoran♀ | Before Brock | Route22 | 235 → 235 |
| Nidoran♂ | Before Brock | Route22 | 235 → 235 |
| Numel | Through Misty | Route11 | 255 → 255 |
| Oddish | Through Misty | Route24 | 255 → 255 |
| Onix | Through Misty | Mt. Moon 1F | 45 → 125 |
| Paras | Through Misty | Mt. Moon 1F, Mt. Moon B1F | 190 → 190 |
| Pichu | Before Brock | Route2 | 190 → 190 |
| Pidgey | Before Brock | Route1 | 255 → 255 |
| Pineco | Before Brock | Viridian Forest | 190 → 190 |
| Poliwag | Through Misty | Route24, Route25 | 255 → 255 |
| Psyduck | Through Misty | Route4 | 190 → 190 |
| Ralts | Through Misty | Route4 | 235 → 235 |
| Rattata | Before Brock | Route1, Route5 | 255 → 255 |
| Sandshrew | Through Misty | Route6 | 255 → 255 |
| Shroomish | Before Brock | Viridian Forest | 255 → 255 |
| Skitty | Through Misty | Route5 | 255 → 255 |
| Slowpoke | Through Misty | Route25 | 190 → 190 |
| Slugma | Through Misty | Route3 | 190 → 190 |
| Smoochum | Through Misty | Mt. Moon B2F | 45 → 125 |
| Sneasel | Through Misty | Mt. Moon B2F | 60 → 125 |
| Snorunt | Through Misty | Mt. Moon B2F | 190 → 190 |
| Spearow | Before Brock | Route22 | 255 → 255 |
| Sunkern | Through Misty | Route3 | 235 → 235 |
| Swinub | Through Misty | Mt. Moon B1F, Mt. Moon B2F | 225 → 225 |
| Taillow | Through Misty | Route6 | 200 → 200 |
| Teddiursa | Before Brock | Route2 | 120 → 125 |
| Trapinch | Through Misty | Route11 | 255 → 255 |
| Tyrogue | Before Brock | Route22 | 75 → 125 |
| Weedle | Before Brock | Viridian Forest | 255 → 255 |
| Zubat | Through Misty | Mt. Moon 1F, Mt. Moon B1F | 255 → 255 |

## Other acquisition channels reviewed

| Channel | Pokémon or source | Catch-rate disposition |
|---|---|---|
| Starter | Pikachu, native starter script | Player receives it without a capture roll; unchanged. PR249 nickname/OT work remains separate. |
| Parcel reward | SUE Eevee | Gift/party-PC delivery; no capture roll. Existing PR249 ownership correction and previously acquired SUE migration boundary are unchanged. |
| Museum egg | Aron, existing Pewter Museum trivia reward | Hatching uses no capture roll; Aron catch rate unchanged. |
| Mystery Vendors | Bulbasaur, Squirtle, Charmander, Kangaskhan, Pinsir, Feebas, Chansey, Dratini, Absol, Magikarp | Any can be offered from the existing uniform pool; only the early reachable sellers are available. No capture roll; no changes to the pool, price, shiny rules or ownership. Magikarp is also in the direct-capture audit and remains 255. |
| In-game trades | Abra → Mr. Mime on Route 2, Poliwhirl → Jynx in Cerulean, Spearow → Farfetch’d in Vermilion | Source trade definitions reviewed; trade/party exchange has no capture roll. Trade-only Mr. Mime/Jynx rates unchanged; Farfetch’d changes because of its independent Route 11 wild encounter. No new natural trade-access certification claimed. |
| Evolution, friendship, stones and eligible eggs | Evolutions/breeding of the preceding obtainable Pokémon under current source rules | No capture roll for producing the owned result. Species available only by these means retain their catch-rate fields. No invented evolution/item/egg access, changed species identity, new capture opportunity or outside-player trade assumption. |

The non-capture channel review does not imply every evolution/item combination was traversed or every possible evolved species is newly catchable. The 51-species denominator describes direct capture opportunities only, not all owned Pokédex entries.

## Ownership and validation

Live main, AGENTS, handoff, issue105, current open PRs and catch-related issue search were refreshed before editing. No active catch-rate work was found. PKM-009 PR173 is an older stats/learnset integration candidate sharing the species file; its patch has no catchRate additions/removals. That ownership is preserved: this draft does not import, modify or merge it. WRL-002/009 accessibility, PR233 historical reconciliation, WRL001 PR249 and CI proposal PR250 remain separate.

`PYTHONDONTWRITEBYTECODE=1 python3 tests/check_early_catch_rates.py`: PASS. The check reconstructs all 13 approved numeric changes from frozen main, requires every other byte of the species file unchanged, recomputes the full 51-species inventory from both version tables plus the existing special pair, checks the17/34 stage split, all13 exact125 values, the explicit Igglybuff exception and unchanged remaining38 species, and verifies production-path/encounter/engine/workflow/accounting preservation. `git diff --check`: PASS. No game build, capture simulation, weather test, native mGBA or broad CI was run.

The existing capture engine reads `gSpeciesInfo[species].catchRate` in `src/battle_script_commands.c` and uses the existing HP/status/ball calculation. That mechanism is byte-identical; this draft changes the data it reads. Source preservation is not a new native gameplay or subjective balance PASS.

Old whole-file species fingerprint assertions are deliberately unchanged. This new data delta will require separately reviewed exact reconciliation before mandatory CI can accept an integrated candidate; existing failures are neither fixed nor waived here. Do not import Fire assertions or PR233 adapters. Required exact-head modified/upstream CI, a build and focused changed-rate gameplay verification remain pending before any merge/acceptance.

Publication uses `[skip ci]` on a `balance/` branch outside the existing push filters to honor the no-rain directive. Existing required checks remain pending, not green. No workflow/status/protection changes or artifact uploads. After disclosure of unexecuted full CI and the no-rain restriction, the owner reiterated the merge instruction. Standard GitHub merge with exact head guard succeeded at `3ed6ac1e18cc774344c5f242cfdd5020e33b83b8`. The fresh branches/main response reported protected=false and GitHub mergeable_state=clean; no security setting, status or assertion was changed. This records the owner-directed merge, not full/native/upstream CI acceptance. Exact merged-main Actions list was empty; the merge commit carries [skip ci] to avoid rain reruns. This specific action is not general authority to waive tests or alter required gates on other work. Preserve owner rain/Sun/Snow-palette/Hail/Parcel acceptance, #248 Playtest002 snow-persistence confirmation and queued WRL010.

Accounting remains exactly 50 permanent task IDs / 461 earned / 251 remaining / 712 total. No new points or task closure. This is a PKM-009 catch-rate balance proposal with WRL acquisition evidence, not a new permanent task.

Machine-readable complete per-version slot weights, levels, venues, source SHA, before/after values and stage scope: `tests/specifications/early_catch_rates_through_misty.json`.
