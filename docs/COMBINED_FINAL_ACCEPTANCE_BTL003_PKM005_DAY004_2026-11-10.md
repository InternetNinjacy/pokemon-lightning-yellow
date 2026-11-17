# BTL-003 / PKM-005 / DAY-004 — combined final-acceptance evidence audit (2026-11-10)

**Disposition: no additional provisional effort credit authorized by this audit.** Three existing task IDs only; no replacement task. Starting main: `ae928a1d9eba3aed16f4252123027cb5413106a7`. The current tracker still records `4e73c90831baa9ff989f60c3c1908844956ed1ed` as its snapshot. This document does not assert that the historical tracker is current to this exact main. There are no production code, test, save, roster, or frozen-hash changes.

The exact-main GitHub Actions workflow [42925054641](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42925054641) completed **SUCCESS**, with both `modified` and independent `upstream-baseline` jobs SUCCESS. This is a broad current-head regression signal, **not** newly executed coverage of missing acceptance behaviors. Existing historical source/test evidence remains valid within its original tested scope. No 10,000-egg rerun was commissioned.

## Milestone 1 — BTL-003 — PARTIAL; 5 earned / 1 remaining

Implementation [PR9](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/9), merge `bf3c89063f0e51135349d6d715d706b68c1a5f36`, historical successful [CI 41376878452](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41376878452).

Retained: approved Physical/Special/Status split; production battle-category code and host tests; historical compiled-ROM category/damage, AI calculator, status, burn/screen/ability/item and move-selection/summary fixtures; historical 8,715-assertion native suite and real UI list/detail navigation. Current `tests/emulator/battle_categories.c` executes `AI_CalcDmg` with four categories and `AI_TypeCalc` in elemental fixtures, plus text/summary navigation. Its own header warns: **these are ROM integration fixtures, not a full trainer-AI gameplay certification**. The documented full-strategy gap cannot be inferred closed from these tests or the current overall CI success.

**Remaining exact point gate:** focused compiled-ROM trainer-battle strategy selection through the actual AI decision path with controlled Physical/Special alternatives, immunity and status choices; assert selected move and battle-state effect, including category-sensitive ability interactions, while preserving current trainer AI behavior. Existing host-level opcode/damage tests should be reused, with only a minimal new fixture for genuinely missing action-selection behavior. Separate BTL-005 custom-move integration is not a prerequisite. No new native strategy fixture was executed by this audit; do not award the point.

## Milestone 2 — PKM-005 — PARTIAL / external peer and hardware gates; 7 earned / 1 remaining

Implementation [PR9](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/9), merge `bf3c89063f0e51135349d6d715d706b68c1a5f36`; extended [PR39](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/39), merge `8ca6d046b5572ca6ba5ee36fabf789042a299d85`, successful [CI 42035414862](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42035414862).

Retained: all eleven approved actual START → Bag → use → party → evolution scene → Bag routes in `docs/BAG_EVOLUTION_ACCEPTANCE.md`, six held-item alternatives, five Eevee stones, refusal, original-versus-new cancellation rules, exact-once consumption, PID integrity, return to field and flash persistence. Focused native **49,176** assertions, full **512,774** over 19 groups, original host guards and upstream validation remain historical accepted evidence. The recorded preservation of original Fire/Water/Thunder evolution cancellation policy is deliberate, not a gap to redefine. No exhaustive item/menu permutation claim exists.

**Remaining point boundary to resolve with Master Control:** whether a real two-peer link-trade handshake and physical hardware acceptance belong to this PKM-005 final point or are formally delegated to INT-004/005. The available single-ROM trade-evolution tests and identity fixtures **cannot prove link traffic**. Source documents explicitly disclaim hardware and peer acceptance. Without recorded owner-approved delegation, do not close PKM-005. If delegation is approved, record its authority and task-level criteria separately; do not silently waive the external gate. No new link/hardware test was run.

## Milestone 3 — DAY-004 — PARTIAL / external peer and hardware plus ordinary progression; 5 earned / 1 remaining

Implementation [PR10](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/10), merge `e389172f59989b33f00e32b63171581a833d2e37`; historical successful [CI 41451641953](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41451641953).

Retained: six host suites, daycare source/preservation guards, real Route 5 caretaker interactions, save/Continue/migration/flash and hatch scenarios, independently stored Four Island state, 400-byte versioned reserved save storage, original breeding compatibility and identity retention, and exactly **5,066 Shiny of 10,000** real generated eggs. `tests/emulator/daycare.c` currently has migration, compatibility, offspring, identity/hatch, statistical, caretaker UI, hatch timing and Continue-migration native groups. No repeated 10,000-egg run is justified by this audit.

**Remaining point boundary:** ordinary uninterrupted in-game breeding progression (rather than only controlled test states) has not been certified; exhaustive quest-log playback, reverse migration to old ROM, actual second-peer ownership transfer, physical cartridge/RTC and power-interruption fault testing remain expressly unverified. Determine, via explicit existing approval or Master Control decision, which are required for DAY-004 itself and which transfer to INT-004/005. Reverse migration must not be assumed supported; it is a separate risky operation. A single-ROM ownership-data fixture is not a trade handshake. No new natural-progression/hardware test was run; do not award the point.

## Accounting and integration boundaries

At this review, checked-in tracker balances: BTL-003 5/1 of 6; PKM-005 7/1 of 8; DAY-004 5/1 of 6; combined **17 earned / 3 remaining of 20**. Project tracker snapshot **461 earned / 251 remaining / 712**, pending preexisting Master Control tracking reconciliation. **Proposed delta from this audit is zero**; do not advance to 464/248 or alter the permanent 50 IDs, histories, denominator, or other specialists' credit.

No task is marked PASS or CLOSED here. No regressions were demonstrated, no source fixes applied, no test results invented, no automatic merge requested. Historical failed runs and limitations remain intact. This report is a reviewable, append-only supplemental acceptance record; `docs/project-tracking.json`, `docs/PROJECT_DASHBOARD.md`, and the master ledger deliberately remain unmodified while PR185 is independently updating those canonical documents. Master Control should incorporate this record only after required new evidence and task-gate decisions, preventing conflicting tracker edits.

## Narrow next acceptance actions

1. BTL-003: use real compiled trainer AI move selection, with native category-sensitive selection and adverse immunity/ability cases; execute at exact test-branch head and preserve both CI jobs.
2. PKM-005: verify written owner-approved hardware/link delegation if any; otherwise obtain genuine second-peer link evidence, with physical hardware/release scope explicitly separated.
3. DAY-004: run a targeted natural walking/deposit/egg-collection/hatch/save/Continue progression, then resolve written ownership of link, power-cut, quest-log and reverse-migration external gates.

This is a **partial audit**, not a claim that every internally executable acceptance test has been completed.
