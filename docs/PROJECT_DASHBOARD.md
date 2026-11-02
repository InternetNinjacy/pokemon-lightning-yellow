# Pokémon Lightning Yellow — Project Dashboard

Canonical branch: `docs/project-coordination`. Updated 2026-11-02.

Overall tracked scope: `[##--------]` **23.06% complete / 76.94% remaining** (107 earned + 357 remaining planning points).

All estimates are provisional relative effort, not measured hours or a claim the game is 17% release-ready. Design approval alone earns no implementation credit. Shared infrastructure is counted only under INT; feature tests stay with their feature. Adding scope changes the denominator without erasing completed work. Merging is recorded separately from implementation and requires owner approval.

Current focus: finish acceptance for [stabilization PR #9](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/9). [Coordination PR #8](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/8) is open and its CI passed. Route 5 waits for its complete register; unrelated eligible work continues. No PR will be merged or have auto-merge enabled.

| Area | Progress | Complete / remaining | Earned / remaining effort |
|---|---|---|---|
| PKM — Pokédex, species and evolution | `[#########-]` | 90% / 10% | 36 / 4 |
| BTL — Moves, types and battle mechanics | `[##--------]` | 22% / 78% | 27 / 93 |
| TIM — RTC, time of day and lighting | `[###-------]` | 33% / 67% | 16 / 32 |
| WTH — Overworld and battle weather | `[----------]` | 0% / 100% | 0 / 64 |
| DAY — Daycare, breeding and Shiny eggs | `[----------]` | 0% / 100% | 0 / 32 |
| GYM — Gyms, trainers and important battles | `[----------]` | 0% / 100% | 0 / 40 |
| WRL — Maps, encounters, items and story | `[----------]` | 0% / 100% | 0 / 56 |
| INT — Integration, testing and release readiness | `[####------]` | 44% / 56% | 28 / 36 |

## Verified repository and dependency stack

Main remains the baseline at `e04252ea0783510000e634486c0f2526a736f506`. PR #1 is merged; #2–#7 are open. #3–#5 are drafts. Stack: main → #2 split → #3 RTC → #4 evolution → #5 evolution mechanics → #6 custom252 → #7 wild doubles. These branches contain internal merge commits; that does not mean their GitHub PRs were merged. Prior green CI is evidence for its exact commit, not certification of future combinations.

The coordination PR targets main and contains no gameplay. `feature/phase-one-stabilization` starts at #7 and PR #9 targets #7, and preserve all dependency PRs. Route 5 will remain separate once its design register is available.

## Task inventory

Status dimensions (design/code/automated/emulator/PR/merge), exact refs, dependencies, next actions and estimate history are maintained in [project-tracking.json](project-tracking.json). IDs are permanent; old P1 IDs and CASTFORM-TEAM-01 remain aliases. Q01–Q08 mapping is blocked on the unseen Work Queue.

### PKM — Pokédex, species and evolution

| Task | State / next action | Dependencies | Remaining effort | Evidence or blocker |
|---|---|---|---|---|
| PKM-001 Custom 252-species Dex | implemented on unmerged stack; Preserve and rerun relevant regression tests | — | 0 | [PR #6](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/6); [prior CI](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/40968372595) |
| PKM-002 Approved evolution alternatives and Tyrogue menu | implemented on unmerged stack; Preserve and rerun relevant regression tests | — | 0 | [PR #5](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/5); [prior CI](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/40422457155) |
| PKM-003 Twelve approved species typing overrides | implemented on stabilization branch; Complete final clean-build/full mGBA and Actions validation | — | 0 | [PR](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/9); all five host suites pass; pre-National scenes and exact RTC boundaries pass; focused ROM category suite 147 checks pass |
| PKM-004 Roster-aware pre-National evolution access | implemented on stabilization branch; Complete final clean-build/full mGBA and Actions validation | PKM-001 | 0 | [PR](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/9); all five host suites pass; pre-National scenes and exact RTC boundaries pass; focused ROM category suite 147 checks pass |
| PKM-005 Expanded Dex/evolution acceptance | partially verified on stabilization branch; Complete final clean-build/full mGBA and Actions validation | PKM-003, PKM-004, TIM-002 | 4 | [PR](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/9); all five host suites pass; pre-National scenes and exact RTC boundaries pass; focused ROM category suite 147 checks pass |

### BTL — Moves, types and battle mechanics

| Task | State / next action | Dependencies | Remaining effort | Evidence or blocker |
|---|---|---|---|---|
| BTL-001 Per-move Physical/Special/Status split | implemented on unmerged stack; Preserve and rerun relevant regression tests | — | 0 | [PR #2](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/2); [prior CI](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/40252575233) |
| BTL-002 Wild double battle capability proof of concept | implemented on unmerged stack; Preserve and rerun relevant regression tests | — | 0 | [PR #7](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/7); [prior CI](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/40976362014) |
| BTL-003 Split, AI and category UI acceptance | partially verified on stabilization branch; Complete final clean-build/full mGBA and Actions validation | BTL-001 | 3 | [PR](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/9); all five host suites pass; pre-National scenes and exact RTC boundaries pass; focused ROM category suite 147 checks pass |
| BTL-004 Wild double battle hardening | not started; Execute approved stabilization plan | BTL-002 | 10 | Approved run: pending implementation and tests |
| BTL-005 410-move redesign | not started; Obtain missing specification | BTL-001 | 80 | Full approved move register, change ledger, distributions and animation decisions are not locally available; prior audit is not implementation. |

### TIM — RTC, time of day and lighting

| Task | State / next action | Dependencies | Remaining effort | Evidence or blocker |
|---|---|---|---|---|
| TIM-001 Read-only cartridge RTC | implemented on unmerged stack; Preserve and rerun relevant regression tests | — | 0 | [PR #3](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/3); [prior CI](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/40390936433) |
| TIM-002 Functional day/night foundation | implemented on stabilization branch; Complete final clean-build/full mGBA and Actions validation | TIM-001 | 0 | [PR](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/9); all five host suites pass; pre-National scenes and exact RTC boundaries pass; focused ROM category suite 147 checks pass |
| TIM-003 Approved visual periods and lighting | not started; Obtain missing specification | TIM-002 Functional day/night foundation | implemented on stabilization branch; Complete final clean-build/full mGBA and Actions validation | TIM-001 | 0 | [PR](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/9); all five host suites pass; pre-National scenes and exact RTC boundaries pass; focused ROM category suite 147 checks pass |

### WTH — Overworld and battle weather

| Task | State / next action | Dependencies | Remaining effort | Evidence or blocker |
|---|---|---|---|---|
| WTH-001 Map weather selection and persistent state | not started; Obtain missing specification | TIM-002 Functional day/night foundation | implemented on stabilization branch; Complete final clean-build/full mGBA and Actions validation | TIM-001 | 0 | [PR](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/9); all five host suites pass; pre-National scenes and exact RTC boundaries pass; focused ROM category suite 147 checks pass |
| WTH-002 Weather rendering repairs and conflicts | not started; Obtain missing specification | WTH-001, TIM-003 | 16 | Depends on weather and visual policy reconciliation. |
| WTH-003 Field-to-battle weather bridge | not started; Obtain missing specification | WTH-001 | 20 | Field Hail duration and active-battle weather precedence unspecified. |

### DAY — Daycare, breeding and Shiny eggs

| Task | State / next action | Dependencies | Remaining effort | Evidence or blocker |
|---|---|---|---|---|
| DAY-001 Independent Route 5 storage and legacy migration | not started; Obtain missing specification | — | 14 | Dedicated Route 5 register contents unavailable; need owner-supplied export or supported authenticated access. |
| DAY-002 Persistent independent 50% Route 5 Shiny eggs | not started; Obtain missing specification | DAY-001 | 8 | Dedicated Route 5 register contents unavailable; need owner-supplied export or supported authenticated access. |
| DAY-003 Caretakers and pre-Elite Four access | not started; Obtain missing specification | DAY-001 | 4 | Dedicated Route 5 register contents unavailable; need owner-supplied export or supported authenticated access. |
| DAY-004 Breeding, save/load and 10,000-egg tests | not started; Obtain missing specification | DAY-001, DAY-002, DAY-003 | 6 | Dedicated Route 5 register contents unavailable; need owner-supplied export or supported authenticated access. |

### GYM — Gyms, trainers and important battles

| Task | State / next action | Dependencies | Remaining effort | Evidence or blocker |
|---|---|---|---|---|
| GYM-001 Approved gyms and trainers | not started; Obtain missing specification | — | 20 | Exact finalized feature specifications/distribution unavailable; do not infer Phase Two choices. |
| GYM-002 Leaders, rivals and important battles | not started; Obtain missing specification | — | 18 | Exact finalized feature specifications/distribution unavailable; do not infer Phase Two choices. |
| GYM-003 Exactly one Celadon trainer Castform assignment | not started; Obtain missing specification | PKM-001 | 2 | Specific trainer, level and moveset not finalized. |

### WRL — Maps, encounters, items and story

| Task | State / next action | Dependencies | Remaining effort | Evidence or blocker |
|---|---|---|---|---|
| WRL-001 Pikachu starter and story | not started; Obtain missing specification | — | 12 | Exact finalized feature specifications/distribution unavailable; do not infer Phase Two choices. |
| WRL-002 Final encounters and map policies | not started; Obtain missing specification | — | 20 | Exact finalized feature specifications/distribution unavailable; do not infer Phase Two choices. |
| WRL-003 Approved evolution-shop framework | not started; Obtain missing specification | — | 8 | Exact finalized feature specifications/distribution unavailable; do not infer Phase Two choices. |
| WRL-004 Item placements and events | not started; Obtain missing specification | — | 16 | Exact finalized feature specifications/distribution unavailable; do not infer Phase Two choices. |

### INT — Integration, testing and release readiness

| Task | State / next action | Dependencies | Remaining effort | Evidence or blocker |
|---|---|---|---|---|
| INT-001 Pinned unchanged FireRed baseline | implemented in baseline; Preserve and rerun relevant regression tests | — | 0 | [PR #1](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/1) |
| INT-002 Shared Actions and mGBA infrastructure | implemented on unmerged stack; Preserve and rerun relevant regression tests | — | 0 | [PR #4](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/4); [prior CI](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/40392817403) |
| INT-003 Audits, durable dashboard, ledger and Drive sync | dashboard and ledger established; Drive sync pending; Record final stabilization evidence; synchronize Drive when supported access exists | — | 4 | [PR](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/8); JSON/IDs/effort validation passed; docs PR CI success; not executed |
| INT-004 Combined integration acceptance | not started; Execute approved stabilization plan | PKM-005, BTL-003, BTL-004, TIM-002 | 24 | Approved run: pending implementation and tests |
| INT-005 Review evidence and external release checks | not started; Execute approved stabilization plan | INT-004 | 8 | Approved run: pending implementation and tests |

## Execution and acceptance checkpoints

1. Commit/push dashboard and ledger independently; preserve baseline main.
2. Apply only the twelve approved type changes; independently check every other species property is unchanged.
3. Remove National-Dex evolution gates only for approved roster targets, across lookup, scenes and item warnings; validate cancellation, exact-once consumption, excluded targets and registration.
4. Add functional day/night [06:00,18:00); invalid RTC disables timed friendship, while stones work independently. No visual or weather implementation.
5. Expand category/battle/AI/UI and wild-double regression where executable; document every remaining hardware, link and full-gameplay gap.
6. Run all host suites, real mGBA scenes/battles/flash-save tests, clean reproducibility builds, and both GitHub checks.
7. Update this branch with actual commit/PR/run evidence. Route 5 proceeds only after the dedicated register is reconciled.

Route 5 planned architecture: evaluate versioned storage in unused save bytes without moving existing fields; migrate the old single deposit safely; preserve Four Island data and pending egg; advance party hatch counters once per step; persist a fresh unbiased Shiny coin and full PID/OTID at egg creation, force both outcomes to yield exactly 50%, leave all other Shiny behavior unchanged. Validate 10,000 eggs, save/reload, compatibility/Ditto/genderless, full-party/refusal/cancellation and both daycares concurrently. This is planning, not implemented behavior.

## External specification and synchronization blockers

No Google Drive reader/writer is available in this session. Network domain access alone is not authenticated Drive access. The three repository tracking files are canonical pending supported Drive synchronization. Supply exports of the Route 5 register and outstanding move/weather/lighting/Phase Two specifications; never infer their contents.

[Master Plan](https://docs.google.com/document/d/1P4TuXViTpB0BycldB1SzRZTRX8sV_Rd0lFuPwyGDJQk/edit) · [Work Queue](https://docs.google.com/document/d/1tP1db_ujpTzGg2sXcIMQ56pCGmnI8qww0PRUDXpbudU/edit) · [Route 5 register](https://docs.google.com/document/d/1Zkw2u8vky8UMmIJeky7AHq8epdNTrUcQBNHq7GKTueM/edit). These links have not been read directly.

## Resume and focus-switch protocol

Before switching areas or ending a session, update task checkpoints, evidence, remaining effort and the append-only ledger. Commit/push the clean current gameplay branch, switch to the canonical docs branch, update these files, commit/push, then return to the recorded gameplay SHA. Never discard another area or reset completed points. Review unresolved dependencies before starting the requested focus.
