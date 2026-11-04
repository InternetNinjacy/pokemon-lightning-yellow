# Main rival: eight main-story battles

Project: Pokémon Lightning Yellow
Status: READY FOR IMPLEMENTATION AFTER REVIEW AND MERGE OF THIS SPECIFICATION. Game code must be implemented on a separate PR.
Approval: November 3, 2026
Handoff revision: 2026-11-04 v2 (technical implementation contract)

## Goal
Implement exactly eight approved main-story rival parties at Oak's Lab, optional early Route 22, Cerulean City, S.S. Anne, Pokémon Tower, Silph Co., late Route 22, and first Champion battle. Battles 1–4 target moderate difficulty; 5–8 hard. Preserve original battle triggers, sequencing and rematch behavior. Minimal encounter-script routing changes are allowed only when required for the approved Pikachu-player route; do not alter postgame rematches.

## Approved parties
Order is the trainer-party table order; slash separates move slots. Empty fourth slots remain empty.

1. Oak's Lab: Dratini Lv5 [Tackle / Wrap / Leer].
2. Early Route 22: Goldeen Lv9 [Peck / Tail Whip / Water Sport]; Dratini Lv9 [Tackle / Wrap / Leer / Thunder Wave].
3. Cerulean: Goldeen Lv16 [Peck / Horn Attack / Tail Whip / Supersonic]; Slugma Lv17 [Ember / Rock Throw / Smog / Yawn]; Geodude Lv16 [Tackle / Rock Throw / Defense Curl / Magnitude]; Dratini Lv18 [Tackle / Twister / Thunder Wave / Wrap].
4. S.S. Anne: Goldeen Lv19 [Horn Attack / Peck / Supersonic / Tail Whip]; Slugma Lv20 [Ember / Rock Throw / Smog / Yawn]; Geodude Lv20 [Magnitude / Rock Throw / Tackle / Defense Curl]; Dratini Lv22 [Dragon Breath / Twister / Thunder Wave / Wrap].
5. Pokémon Tower: Poliwhirl Lv27 [Water Gun / Hypnosis / Double Slap / Rain Dance]; Beedrill Lv27 [Twineedle / Fury Attack / Focus Energy / Rage]; Slugma Lv28 [Ember / Rock Throw / Yawn / Harden]; Geodude Lv28 [Magnitude / Rock Throw / Rollout / Defense Curl]; Dratini Lv29 [Dragon Breath / Thunder Wave / Slam / Twister].
6. Silph Co.: Poliwhirl Lv39 [Body Slam / Hypnosis / Water Gun / Rain Dance]; Ariados Lv39 [Sludge Bomb* / Night Shade / Leech Life / Scary Face]; Rapidash Lv40 [Stomp / Fire Spin / Take Down / Agility]; Graveler Lv40 [Magnitude / Rock Blast / Rollout / Self-Destruct]; Dragonair Lv42 [Dragon Breath / Thunder Wave / Slam / Agility].
7. Late Route 22: Manectric Lv50 [Thunder / Bite / Quick Attack / Thunder Wave]; Scyther Lv50 [Wing Attack / Slash / Swords Dance / Agility]; Ninetales Lv50 [Flamethrower* / Confuse Ray / Quick Attack / Fire Spin]; Milotic Lv51 [Hydro Pump / Recover / Water Pulse / Safeguard]; Pupitar Lv53 [Crunch / Rock Slide / Thrash / Scary Face]; Dragonair Lv54 [Dragon Breath / Slam / Thunder Wave / Agility].
8. Champion: Magneton Lv56 [Spark / Thunder Wave / Tri Attack / Supersonic]; Ninetales Lv56 [Flamethrower* / Confuse Ray / Fire Spin / Quick Attack]; Heracross Lv57 [Megahorn / Brick Break / Counter / Reversal]; Milotic Lv57 [Hydro Pump / Recover / Water Pulse / Safeguard]; Tyranitar Lv58 [Crunch / Rock Slide / Earthquake* / Thrash]; Dragonite Lv60 [Dragon Claw* / Wing Attack / Thunder Wave / Earthquake*].

Asterisk: move previously flagged for TM compatibility. The dedicated Lightning Yellow TM register and existing TM compatibility were reviewed: TM02 Dragon Claw, TM26 Earthquake, TM35 Flamethrower and TM36 Sludge Bomb remain approved. Implement these exact moves without substitution. If current main contains a later contrary approval, report the conflict and halt the affected scope rather than guessing. All three Dratini family species, globally: add Tackle Lv1, move ordinary Twister from Lv15 to Lv12 (retain evolved-form Lv1 Twister), replace Lv22 Dragon Rage with Dragon Breath. Do not change other approved moves, stats, typing or evolution methods. The Dragon family is always the final party slot.

## Approved held items (all other held slots ITEM_NONE)

1. Oak: Dratini none.
2. Early Route 22: Goldeen none; Dratini Oran Berry.
3. Cerulean: Goldeen, Slugma, Geodude none; Dratini Oran Berry.
4. S.S. Anne: Goldeen, Slugma, Geodude none; Dratini Oran Berry.
5. Tower: Poliwhirl, Beedrill, Slugma, Geodude none; Dratini Sitrus Berry.
6. Silph: Poliwhirl none; Ariados Poison Barb; Rapidash none; Graveler Hard Stone; Dragonair Lum Berry.
7. Late Route 22: Manectric Magnet; Scyther Sharp Beak; Ninetales none; Milotic Mystic Water; Pupitar Hard Stone; Dragonair Lum Berry.
8. Champion: Magneton Magnet; Ninetales Charcoal; Heracross Black Belt; Milotic Mystic Water; Tyranitar Hard Stone; Dragonite Lum Berry.

Approved item counts: 0,1,1,1,1,3,5,6. No trainer bag item approval. Current source verifies all ten item constants and TrainerMonItemCustomMoves supports a heldItem field; original rival party definitions generally use no-item structures. Implementer must apply exact items without introducing other items or unapproved AI.

## Authority and code
Master Plan: https://docs.google.com/document/d/1P4TuXViTpB0BycldB1SzRZTRX8sV_Rd0lFuPwyGDJQk/edit
Approved Pokédex: https://docs.google.com/document/d/1Bv5Ssu_D6AxEDvr6xM3YECPRRAxO9yO3HVSNIos1gQc/edit
Species balance: https://docs.google.com/document/d/1tIdBczPU7bD-31Hw2hyCJQOk_E0bN1DwgLxld-YtrK8/edit
Exact rival register: https://docs.google.com/document/d/1Z6FYsfy606zc18eDcVSKJzd9WMmJvISbdj43qvLCYuM/edit
Change log: https://docs.google.com/document/d/1NpCjx3uboBMdBVqoFpMwXEyVkDquD37Kcp-FJBbYWnc/edit
Source files: src/data/trainer_parties.h, src/data/trainers.h, src/data/pokemon/level_up_learnsets.h, src/data/pokemon/tmhm_learnsets.h. Current code still uses starter-dependent Kanto rival teams. Implementer must locate event script branching.

## Implementation decisions and bounded technical work

1. Exact team species, levels, party-slot order, moves and all held items are owner-approved, as are the eight encounter locations and difficulty goals. No design selection remains for these fields.
2. Preserve the existing rival AI flags and existing trainer-use bag-item inventory (empty for existing rival definitions). No new healing bag items, abilities, IV targets, EV adjustments, AI features, special weather, dialogue or story changes. Existing default source values for unrelated fields remain unchanged.
3. To prevent Pikachu starter routing from blocking team work, update each of the **three original starter-dependent rival Trainer IDs per approved encounter** to resolve to the exact same approved Lightning Yellow party. Where this requires distinct party data structures or three identical references, do so without altering unrelated trainers. Keep original event triggers, optional early Route 22, battle tutorials, victory/loss handling, and save flags. If the future Pikachu-player starter implementation uses a new value of VAR_STARTER_MON other than 0/1/2, an approved minimal dispatch fallback may be added, but DO NOT implement a separate player Pikachu starter feature under this handoff. Report if cannot route safely.
4. First Champion uses only `TRAINER_CHAMPION_FIRST_*` parties; the distinct `TRAINER_CHAMPION_REMATCH_*` parties MUST remain unchanged. Locate Champion battle event script and ensure initial Champion uses an approved party. Source symbols verified in `src/data/trainer_parties.h` and `src/data/trainers.h`.
5. Apply previously owner-approved global learnset edits for Dratini, Dragonair and Dragonite ONLY as specified above. Ensure no loss of evolved-form Lv1 Twister or other existing separately approved stage changes. Do not implement an entire species-balance redesign as a side effect.
6. The approved four TM assignments match source compatibility for all marked rival move occurrences. Their implementation requires no new TM system. Retain all current lawful source definitions unless superseded by an already-approved change; don't import older or other-project proposals.
7. The battle source supports `TrainerMonItemCustomMoves` / `.heldItem` for trainer parties and all required `ITEM_*` constants exist. Convert rival data structures only where necessary. Unmentioned Pokémon explicitly hold `ITEM_NONE`.
8. The source currently includes original FireRed level-up data and rival teams, so these are intentional implementation changes, not already delivered functionality. Do not require unrelated unreleased Fire/Ghost Ninetales, Milotic stat/type changes, full 252-Dex, or broader physical/special work to be newly implemented here. When an approved separate dependency is present, preserve it and test the rival against it; when absent, report expected future integration and do not invent substitutes.
9. Use current `AGENTS.md` validation instructions for modified builds, independent upstream baseline and host/emulator tests. Never weaken the upstream checksum or change CI acceptance to conceal gameplay changes. A normal approved gameplay build should use the modified-build workflow, not require the old unmodified checksum.
10. No separate approval is needed to locate file paths, select existing supported trainer party structures, reuse original Trainer IDs, or add in-scope deterministic tests. These are engineering choices, not gameplay design decisions.

**Stop conditions:** A source conflict with an explicit later approved rival team/move/item decision, inability to preserve event and Champion rematch behavior, or a requirement for unapproved gameplay mechanics. Escalate only the affected conflict; do not silently improvise.

TM source of truth: https://docs.google.com/document/d/1EHByhspPMrn_H0ard-wbdZLRqInZVschQqXTWSiP5fY/edit

## Additional read-only source-parity findings (2026-11-04)

All seven pre-Champion original rival locations have verified event script paths: `data/maps/PalletTown_ProfessorOaksLab/scripts.inc`, `data/maps/Route22/scripts.inc` (early and late), `data/maps/CeruleanCity/scripts.inc`, `data/maps/SSAnne_2F_Corridor/scripts.inc`, `data/maps/PokemonTower_2F/scripts.inc`, and `data/maps/SilphCo_7F/scripts.inc`. All contain `VAR_STARTER_MON` conditional branches into Kanto-starter trainer IDs; preserving the correct battle execution, optionality, and event flags requires explicitly mapping the Pikachu starter path. The final Champion event-script path is a Codex implementation lookup, not a missing design decision; `src/data/trainer_parties.h` confirms separate `sParty_ChampionFirst*` and `sParty_ChampionRematch*` families and `src/data/trainers.h` confirms distinct `TRAINER_CHAMPION_FIRST_*` and `TRAINER_CHAMPION_REMATCH_*` IDs. Only the first Champion is approved.

Reviewed the canonical TM & Move Register: TM02 Dragon Claw, TM26 Earthquake, TM35 Flamethrower, and TM36 Sludge Bomb are preserved, matching source TM eligibility for the approved rival party moves. Final reconciliation still needs any later approved overrides and game implementation status. Held-item sets are owner-frozen; do not infer Trainer bag healing items. No source code/build/emulator tests performed. These prior technical dependencies are now scoped as implementation checks with explicit stop conditions. A documentation PR is not evidence that game code is implemented or validated.

## Acceptance checks for implementation PR
Check exact encounter party, order, level and moves; Pikachu story route; wild/player Dratini family learnsets; ordinary Lv55 final evolutions; legal TM/abilities; event/save/regressions; and emulator-test the moderate first four and hard final four. Do not weaken baseline checks. Follow AGENTS.md and implementation/README.md.

This specification is READY FOR IMPLEMENTATION once this documentation PR is reviewed and merged. Codex must implement on a separate topic branch and open its own PR; do not merge automatically. Any explicit stop condition above still blocks the affected code change, not the entire approved design package.
