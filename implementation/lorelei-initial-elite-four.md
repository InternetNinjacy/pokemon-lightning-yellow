# Feature: Lorelei initial Pokémon League battle (Normal specialist)

**Project:** Pokémon Lightning Yellow, and no other Pokémon ROM hack  
**Status:** Ready for implementation **only after this specification PR is reviewed and merged**; gameplay not yet changed or tested  
**Approval:** Owner-approved sequentially on November 4, 2026, including final two Full Restores and unchanged original Elite Four AI  
**Handoff revision:** 2026-11-04  
**Dependencies:** Approved 252-species Pokédex, approved move categories and species learnsets, source trainer-party data. Preserve other feature branches and any pending integration prerequisites. This file does not authorize an unrelated subsystem merge.

## 1. Goal and player-facing behavior

Keep Lorelei as the first Elite Four member of the original FireRed Pokémon League. Change only her first-match battle composition from the original Ice-oriented party to the approved five-member Normal-type HARD challenge. Preserve Lorelei's character, class, visual, location, original triggering event, Single Battle format and all League progression flags. Preserve ordinary FireRed Trainer AI and Trainer-item semantics. Do not add any boss, encounter or new battle mechanics.

## 2. Approved design decisions

All values below are literal owner-locked requirements, including order of Pokémon and order of move slots.

| Slot | Species | Level | Move 1 | Move 2 | Move 3 | Move 4 | Ability | Held item | Role |
| --- | --- | ---: | --- | --- | --- | --- | --- | --- | --- |
| 1 | MILTANK | 52 | STOMP | ATTRACT | MILK_DRINK | ROLLOUT | THICK_FAT | LUM_BERRY | Whitney-inspired status/disruption opener |
| 2 | PORYGON2 | 53 | TRI_ATTACK | THUNDERBOLT | ICE_BEAM | PSYCHIC | TRACE | MAGNET | Offensive special coverage |
| 3 | TAUROS | 54 | RETURN | EARTHQUAKE | ROCK_TOMB | IRON_TAIL | INTIMIDATE | SILK_SCARF | Fast physical threat |
| 4 | BLISSEY | 54 | TOXIC | SOFT_BOILED | PROTECT | LIGHT_SCREEN | NATURAL_CURE | LEFTOVERS | No direct attacks; defensive poison attrition |
| 5 | SNORLAX (ACE) | 56 | BODY_SLAM | EARTHQUAKE | SHADOW_BALL | REST | IMMUNITY | CHESTO_BERRY | Bulky final threat, one-time Rest/Chesto |

Species specialty: NORMAL. Difficulty target: HARD, to be calibrated using the owner-approved party rather than inventing arbitrary multipliers. All five species are Normal-typed under current approved roster. All five abilities are distinct and present in original Generation III. Snorlax IMMUNITY **supersedes** the older Thick Fat decision. The accepted Lightning Yellow per-move split makes SHADOW_BALL **SPECIAL**, despite Snorlax's larger Attack stat; never reinterpret it as physical to buff Snorlax.

Trainer battle inventory: **exactly two ITEM_FULL_RESTORE**, no other Trainer-use items.

Battle format: SINGLE (source `.doubleBattle = FALSE`).

AI flags: **exactly**
`AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY`.

Do not introduce custom AI, force-use item rules, altered enemy visibility or move-selection scripting. Trainer items use existing stock engine behavior.

**Do not infer approval for Lorelei's rematch** (`TRAINER_ELITE_FOUR_LORELEI_2`), nor for the other Elite Four, Champion, or Champion rematches.

## 3. Authoritative sources and provenance

- Owner-approved living Elite Four register (2026-11-04): https://docs.google.com/document/d/1qEdSSW26GkY3NmF-Na3_bIAMIgYFuVr4tGTKruBKUMI/edit
- Master Plan and isolation rule: https://docs.google.com/document/d/1P4TuXViTpB0BycldB1SzRZTRX8sV_Rd0lFuPwyGDJQk/edit
- START HERE index/workflow: https://docs.google.com/document/d/14bV_mRB-IgcyWqGcIH2tgmvNoYLY1Ip_zSqSbdHGYtw/edit
- Approved 252 species: https://docs.google.com/document/d/1Bv5Ssu_D6AxEDvr6xM3YECPRRAxO9yO3HVSNIos1gQc/edit
- Move/TM register and special classification: https://docs.google.com/document/d/1EHByhspPMrn_H0ard-wbdZLRqInZVschQqXTWSiP5fY/edit
- Species balance/learnset progression: https://docs.google.com/document/d/1tIdBczPU7bD-31Hw2hyCJQOk_E0bN1DwgLxld-YtrK8/edit
- Change intake: https://docs.google.com/document/d/1NpCjx3uboBMdBVqoFpMwXEyVkDquD37Kcp-FJBbYWnc/edit
- Rival Champion (out-of-scope, frozen): https://docs.google.com/document/d/1Z6FYsfy606zc18eDcVSKJzd9WMmJvISbdj43qvLCYuM/edit
- Explicit owner approvals: this November 4 Elite Four & Postgame Battle Design conversation, final user approval: "Approved. Lock Lorelei's two Full Restores and original three FireRed AI flags. Complete her initial battle design and prepare the canonical documentation and technical handoff checks."

The original Ice roster in source is historical vanilla behavior, not a currently approved Lightning Yellow team. No Elite Four member identity replacement is authorized. Rival team specifications from unrelated Pokémon projects are forbidden.

## 4. Implementation boundaries

**In scope:** Change only the initial Lorelei Trainer party data, retaining the exact identity/metadata, Double Battle flag, AI flags and two Full Restores. Ensure the selected ability slot matches each owner-approved ability when instantiated in Trainer parties. If current engine does not support an exact ability choice without touching species-wide abilities, **STOP and report** rather than silently replacing the ability or changing global species data.

**Out of scope:** Lorelei rematch party, other Elite Four members, Champion first/rematch teams, overworld events, map changes, dialogue/graphics, global move/category changes, global abilities/evolutions, custom AI, modified item behavior, automatic merges, ROM releases, balancing other Trainers.

**Preserve:** All original battle triggers, defeat/victory progression, Hall of Fame access after later battles, exact original class and portrait.

## 5. Technical implementation notes (source-verified November 4)

- `include/constants/opponents.h`: `TRAINER_ELITE_FOUR_LORELEI = 410`. Distinct rematch `TRAINER_ELITE_FOUR_LORELEI_2 = 735`.
- `src/data/trainers.h`: current first battle includes `.items = {ITEM_FULL_RESTORE, ITEM_FULL_RESTORE}`, `.doubleBattle = FALSE`, AI flags exactly as approved; `.party = ITEM_CUSTOM_MOVES(sParty_EliteFourLorelei)`.
- `src/data/trainer_parties.h`: `sParty_EliteFourLorelei[]` currently vanilla Dewgong/Cloyster/Slowbro/Jynx/Lapras. Existing `TrainerMonItemCustomMoves` supports held items, levels and fixed moves. Original IV field per member: 250. **Preserve original IV policy (250 for all) unless a specific approved conflicting source change is found**; no new EV/IV tuning authorized.
- `src/data/pokemon/species_info.h`: source abilities confirm Miltank THICK_FAT, Porygon2 TRACE, Tauros INTIMIDATE, Blissey NATURAL_CURE, Snorlax IMMUNITY. These are ordinary native abilities, not new global grants. Species Normal types confirmed.
- `src/data/pokemon/level_up_learnsets.h`: verified natural moves Miltank Stomp/Milk Drink/Rollout, Porygon2 Tri Attack, Blissey Soft-Boiled/Light Screen, Snorlax Body Slam/Rest.
- `src/data/pokemon/tmhm_learnsets.h`: Miltank Attract; Porygon2 Thunderbolt/Ice Beam/Psychic; Tauros Return/Earthquake/Rock Tomb/Iron Tail; Blissey Toxic/Protect/Light Screen; Snorlax Earthquake/Shadow Ball. All are represented for the appropriate species. Reconcile final approved TM remapping/learnset overrides in active branch.
- `include/constants/items.h`: verified ITEM_FULL_RESTORE, LUM_BERRY, MAGNET, SILK_SCARF, LEFTOVERS, CHESTO_BERRY constants. Follow actual source's `ITEM_` syntax.
- The TrainerMon data struct may not encode ability directly. Verify how Trainer Pokémon personality/ability assignment works; `Porygon2` and `Tauros` have only one source ability, `Blissey` has a choice, `Snorlax` has two. Ensure deterministic approved ability selection without unauthorized modifications elsewhere.
- Research source script mapping for first League battle before coding; do not change maps/events to accommodate this data edit.
- Do not assume native AI optimally sequences Miltank Attract/Rollout, Blissey Toxic/Protect, or Snorlax Rest/Chesto. No custom logic may be added merely to improve strategy.
- If any feature integration branch has a conflicting approved species/move setting, stop and reconcile through owner-approved project records rather than overwrite it.

## 6. Acceptance criteria and validation

- [ ] Source references exactly first-match Lorelei #410 (not rematch #735); original Single Battle/map trigger and identity preserved.
- [ ] Exact five species, order, levels, all 20 move slots, selected abilities and held items, with Snorlax ace last, verified in actual battle.
- [ ] Exactly two Trainer Full Restores, with default FireRed item AI; three original AI flags unchanged.
- [ ] Validate no repeated abilities and no later-generation-only ability; no global species ability changes.
- [ ] Confirm Miltank Attract/Rollout and Lum, Porygon2 Trace and damage, Tauros Intimidate, Blissey Toxic/Protect/Light Screen and Leftovers, Snorlax Immunity + Rest/Chesto behavior.
- [ ] Confirm move-specific approved split, especially SPECIAL Shadow Ball.
- [ ] Confirm Lorelei rematch #735, all other League parties/flags, Champion, map scripting and progression unchanged.
- [ ] Assess player-fair HARD challenge with normal pre-League party; note AI behaviors not guaranteed by existing mechanics.
- [ ] Run `scripts/prepare-baseline.sh`, `make -j2`, current relevant host regression tests and mGBA integration checks prescribed by `AGENTS.md`; document tests not run and why. Keep upstream checksum separate; never commit ROM outputs.
- [ ] Open implementation PR on separate feature branch, no auto merge.

## 7. Open questions and blockers

No unresolved **design** choice for this initial Lorelei party. **Required technical verification** before claiming a correct implementation: deterministic choice of Blissey NATURAL_CURE vs SERENE_GRACE and Snorlax IMMUNITY vs THICK_FAT with the existing Trainer instantiation path, and exact integrated approved move/typing data. If this requires scope or mechanics not approved, stop, keep implementation blocked and seek owner approval. Verify original battle script routing, existing AI trainer item behavior and regression.
No code changes or emulator tests were performed to create this specification.

## 8. Codex delivery requirements

After this specification PR is reviewed and merged, implement only this file's scope on a separate topic branch. Read `AGENTS.md` and the integrated project data before making changes. Open a gameplay PR with changed files, diff explanation, tests run/not run, source checks and any blocker. No automatic merge, unauthorized mechanics, extra rivals, rematches, or ROM publication.

## 9. One-line Codex handoff (after spec merge only)

`Implement implementation/lorelei-initial-elite-four.md following AGENTS.md. Preserve approved roster and original battle scripts, run required checks, and open a separate gameplay PR; report blockers rather than guessing.`
