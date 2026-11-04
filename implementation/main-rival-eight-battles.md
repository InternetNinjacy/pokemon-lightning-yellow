# Main rival: eight main-story battles

Project: Pokémon Lightning Yellow
Status: BLOCKED, OWNER-APPROVED DESIGN INCLUDING ITEMS, NOT IMPLEMENTATION-READY
Approval: November 3, 2026
Handoff revision: 2026-11-03 v1

## Goal
Implement exactly eight approved main-story rival parties at Oak's Lab, optional early Route 22, Cerulean City, S.S. Anne, Pokémon Tower, Silph Co., late Route 22, and first Champion battle. Battles 1–4 target moderate difficulty; 5–8 hard. Do not alter encounter scripts or postgame rematches.

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

Asterisk: move conditional on final approved Lightning Yellow TM/learnset authority. No substitution without approval. All three Dratini family species, globally: add Tackle Lv1, move ordinary Twister from Lv15 to Lv12 (retain evolved-form Lv1 Twister), replace Lv22 Dragon Rage with Dragon Breath. Do not change other approved moves, stats, typing or evolution methods. The Dragon family is always the final party slot.

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

## Blockers
1. Reconcile final TM replacements, availability, and all five marked move occurrences; default-branch TM table supports the source moves but may not reflect approved Lightning Yellow replacements.
2. Held items are approved above; confirm conversion of original no-item rival data into item-capable Trainer party definitions where necessary.
3. Preserve existing AI flags until separately approved; do not add new AI functionality.
4. Confirm final species move timing and special balance rules, including Dragonite Dragon Claw, Tyranitar Earthquake, Fire/Ghost Ninetales, and physical/special split.
5. Verify Pikachu starter selection and all eight event flags; preserve optional early Route 22 and unchanged postgame.
6. Determine project-approved gameplay CI/build gate transition. No source code or game implementation authorized by this blocked spec.

## Validation before Ready
Check exact encounter party, order, level and moves; Pikachu story route; wild/player Dratini family learnsets; ordinary Lv55 final evolutions; legal TM/abilities; event/save/regressions; and emulator-test the moderate first four and hard final four. Do not weaken baseline checks. Follow AGENTS.md and implementation/README.md.

This draft is intentionally BLOCKED. Do not hand it to Codex for gameplay implementation until the unresolved decisions and source conflicts are reconciled and the reviewed specification is merged as Ready.
