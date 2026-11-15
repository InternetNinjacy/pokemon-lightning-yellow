# Phase 1 — strict historical reconstruction consolidation

Status: **draft, validation-only, not accepted for merger**. Owner authorization: 2026-11-14. This proposal intentionally does **not** alter gameplay, workflow files, source specifications, frozen digests, required GitHub checks, branch protection, or any existing test runner.

## Current acceptance behavior (verified against main and policy PR #245)

- `AGENTS.md` requires owner-approved gameplay, independent vanilla FireRed checksum, applicable modified-source/native/build/save/ABI checks, actual GitHub protections, and no weakened assertion. Its November 12 clarification refers to the existing risk-based TEST EQUIVALENCE POLICY, but says current Actions and mandatory statuses remain unchanged.
- `docs/CODEX_COST_EFFICIENT_WORKFLOW.md` specifies tiers 0–4, recommends focused validation during development, and requires a **separately reviewed CI-optimization PR** before any conditional workflow or required-status change. Branch-protection requirements were not readable with existing connector privileges (403); do not infer them.
- Historical source contracts compare exact source hashes specified in existing `tests/specifications/*.json` and reverse explicitly approved deltas in `tests/approved_*_changes.py`. These are *source-preservation* checks, not proof that a ROM runs or that a legendary encounter works.
- `tests/run_brock_weather_tests.py` independently nests many approved trainer/Gym reversals before checking the frozen Brock `before_sha256` and `after_sha256`. It also checks Brock's exact party and bag.
- `tests/run_elite_four_tests.py` uses `approved_ordinary_trainer_projection.py` (the 500 non-dummy ordinary Trainer lock plus reviewed Gym/Rocket exceptions), frozen Elite Four digests, exact eight-party contracts, and the Agatha native ability constructor source guard.
- `tests/run_weather_ability_tests.py` and `tests/run_weather_species_tests.py` independently reverse 16 exact weather species ability-slot changes; their older species snapshots predate other approved changes.
- Policy PR **#245** (head at review: `e22e8908a2bd69e2b6ea5f2a828aa1ebe5a8c509`) stages exact historic revisions for two Gym Leaders and six accepted species profiles. It is a separate active integration dependency, **not** a base or automatic merge for this PR.
- The latest observed #245 modified broad job **132629868493** failed `tests/run_brock_weather_tests.py:25` when comparing the reconstructed `src/data/trainer_parties.h` to the frozen approved Brock after-digest. Independent upstream baseline and focused BTL-005 passed. The exact *remaining* difference in the Brock projection is **not yet established**. Do not call this an approved-only difference until compared.

## Diagnosed gaps

1. Overlapping tests maintain independent, ordered chains of exact approvals for the same file. A later accepted change may be recognized by one historical test but not another.
2. A generic digest mismatch identifies a file, not the affected Trainer/species record or approving specification; the next suite's issue is uncovered only after the first earlier failure is corrected.
3. Partially reconstructed history can be mislabeled as a gameplay defect. Conversely, an unexplained mismatch must not be waived as stale history.
4. Ordinary Trainer record replacement must keep its owner-locked count/record identities and fail on changes outside approved blocks. Generic replacement of *all* Trainer blocks would remove protection.
5. Additional required suite stages may not execute after an early failure. Their absence is **not a pass**.
6. Existing branch protections and exact required check names need authorized verification before any future CI topology changes.

## Proposed fail-closed contract

A common test-only function may reverse **only** explicitly enumerated, owner-approved exact records or exact pre/post hunks. It must require:
- Exact trusted pinned Git commit for approved and historical snapshots, plus a scoped list of identities; the caller cannot authorize a record by comparing arbitrary current text with arbitrary old text.
- One and only one matching record in the current, approved, and historical sources; the entire current record equals the pinned approved record **byte-for-byte**; approved and historical differ.
- Existing manifests and **their original digests remain unchanged**. The caller must compare the reconstructed whole file against its frozen digest; all unrelated bytes and unknown edits therefore still fail.
- No unconditional normalization, wildcard “accept changed” behavior, catch-and-ignore, test skip, rewritten snapshot, altered protected workflow or green stand-in.
- A clear record-level error on drift, duplicate IDs, unknown source, missing trusted commit, or changed approved data.

The initial `tests/approved_historical_reconstruction.py` is an **unconsumed, test-only helper**, deliberately leaving current CI and runners completely unchanged while the accepted contracts are inventoried. It is *not* acceptance evidence.

## Proposed migration in separately reviewed increments

1. Inventory every historical guard and its pinned SHA/manifests, approvals, ordering and protected scope (Trainer parties, Trainer metadata, species, abilities, battle, map, save). Mark unknown authority **blocked**.
2. Compare common helper output against each old helper output for both **accepted** sources and deliberately mutated/unapproved source fixtures; ensure both reject each mutation. Keep the existing historical SHA assertions as the final gate.
3. Migrate one low-risk, known-overlapping historical reconstruction at a time. Preserve each test's exact byte and runtime assertions, IDs, frozen expected digest, and workflow registrations. Review differential output against the old tests before removing duplicate chains.
4. Record per-source provenance, before/after output SHA, explicit rejected mutation examples, equivalence log/run IDs, and required exact-head modified + independent baseline checks. If equivalence cannot be shown, stop and keep old guard.
5. Only after a successfully reviewed, CI-green equivalent replacement should obsolete logic be removed. Keep this PR draft until all actual current branch requirements are satisfied; even a green PR does not by itself authorize merge/release.

## Not part of Phase 1

No new gameplay design, no reinterpretation of Misty/Surf, no legendary feature edits, no CI risk-tier gating, no branch-rule changes, no release, and **no** exception beyond the explicitly documented, narrowly scoped legendary Misty acceptance decision. PRs #245, #238, and #243 remain independently gated and must not inherit an unproven exception from this draft.

## Evidence / acceptance checklist (not yet complete)

- [x] Existing policy and affected historical test paths read on repository main and #245.
- [x] Document repeated failed-check progression and unverified Brock residual.
- [x] Start an isolated validation-only branch based on main.
- [ ] Complete trusted-source/manifest inventory for all guards.
- [ ] Demonstrate bit-for-bit output equivalence to old helpers over all approved sample files.
- [ ] Demonstrate rejection of unauthorized record changes and changes outside approved records.
- [ ] Migrate reviewed callers without modifying frozen digest values or protection scope.
- [ ] Obtain passing exact-head mandatory CI and authorized review.
- [ ] Merge (NOT authorized by this document alone).

## Phase 1 proof ledger — 2026-11-14 (partial inventory)

Source facts below were read from **main** and the two pinned source commits. The inventory is **not yet repository-wide**; rows not investigated retain the original protections and remain unclassified.

| Existing guard / file | Source of approved change and frozen expected values | Protected scope / open gap |
| --- | --- | --- |
| `tests/approved_gym001_baily_changes.py` | Approved `src/data/trainer_parties.h` from main commit `02240690581eb70d70219024fb7f55da2882308b` versus historical `21a3630b3d51481ab10a1af9637fa1ced73722cc`; exact one `sParty_EngineerBaily` record, Pichu Lv8 and Flaaffy Lv22 vs original Voltorb/Magnemite Lv21 | Scoped source adapter only; downstream callers retain immutable whole-file digests. Migrated in this draft; original BEFORE/AFTER guards retained. |
| `tests/run_brock_weather_tests.py` | `tests/specifications/brock_weather_party_delta.json`: protected `src/data/trainer_parties.h` after `7492be71499ad7281010b0197ee19ca5dd82341babba833ab319fa88b5ef8938`, before `3ef0ccce270c2e5cd854b2712b2acf8b48a32261d78875b67fb6e52985efc659`; Trainer metadata after `cd23c29c2b19955e4b57435f88c034d45de283b8d24b69bea24ffc9b000f880e`, before `251e6a22e9882641cce980c45ff01a45e5cc3da465f2e84532b31bce445ac739` | Original exact Brock party, bag, sequential Gym/Trainer approval normalization; latest policy #245 broad check failed an **unclassified** trainer-parties after-digest. No change in #246 to that test/digest. |
| `tests/run_elite_four_tests.py` | `tests/specifications/approved_elite_four_delta.json`: eight reviewed party hunks; `trainer_parties.h` after `d9388c7b38b8f0c417bebefd9e753497c2a6511e4181ab0282c331f6baa8227f`, before `7ea7161fbad37377d9270077f649ecbeafe2c8018295e29ef99d7924fce989a2`; six additional preserved-file hashes | Entire protected Trainer records, Elite Four exact parties and preserved battle/ABI/species files. Existing ordinary lock contains **510 entries, 500 non-dummy authorized**. No migration here yet. |
| `tests/run_weather_ability_tests.py` | `tests/specifications/weather_ability_delta.json` holds before/after hashes and exact source hunks for eight implementation files, plus four preserved-file hashes. Preserved `species_info.h` SHA `e37cfe0a03344890eb7c95ddf6193e635eb726e08c7198b7b7062c715ac5f267` | Original ability namespace, 16 known weather species changes, unchanged save/source data; six accepted species profiles may require a separately approved historical projection but never replacement of frozen hash. |
| `tests/run_weather_species_tests.py` | `tests/specifications/special_glalie_delta.json` `species_sha256`, plus `tests/approved_weather_species_slots.py` 16 exact ability-slot old/new pairs; four explicitly excluded evolutions checked | Complete species file + native behavior distinct. Existing strict #245 six-profile correction remains separate, not imported into this draft. |
| `tests/approved_thunder_wave_changes.py` | `tests/specifications/approved_thunder_wave_delta.json`: six implementation file paths with original before/after SHA-256 and exact hunks | Source normalization only, native Thunder Wave / battle acceptance separate; not migrated. |
| `tests/approved_celadon_four_changes.py` | Four literal, exact prior/approved party snapshots: Lass Kay, Lass Lisa, Picnicker Tina, Beauty Tamia | Independent source adapter permits preexisting old values as well as exact approved values; any shared migration must **not silently tighten or loosen** those explicit contracts. Not migrated. |

### Baily adapter differential evidence before migration

- Read actual `src/data/trainer_parties.h` at original `21a3630...`, approved `02240690...`, and current main: the Baily record occurs **exactly once** in each; approved current equals the reviewed new Pichu/Flaaffy snapshot byte-for-byte; historical equals the old Voltorb/Magnemite snapshot byte-for-byte. The reviewed new and historical records differ.
- A read-only JS source-equivalent comparison of the old Baily adapter and proposed `reverse_pinned_records` (same whole-record regex and exact pinned data) produced **identical complete 331,516-character normalized output** for actual current main source.
- Mutation cases against that actual source: Baily species edit, Baily level edit, duplicate new Baily block, and duplicate historical Baily block were rejected by **both** old and proposed guards. A modification elsewhere in trainer parties and unrelated comment edits passed both record-only guards; this is expected and **must** still be rejected by the *unchanged* downstream frozen whole-file fingerprint, not by the scoped adapter.
- An isolated Python unittest of the shared helper's record-reversal and digest-check logic, with controlled mock pinned sources (not a full repo checkout), passed **4 cases** covering positive exact equivalence, Baily illegal edits/duplicates, bad allowlists, and unrelated-byte SHA rejection. This is useful algorithm-level evidence but **not** exact-head repository CI, actual full-helper integration, or complete regression equivalence.
- Only `tests/approved_gym001_baily_changes.py` was migrated: original `BEFORE` and `AFTER` literal contracts and their count/duplicate assertions are unchanged; final substitution is performed by the additional pinned strict record helper. Source hash manifests, run_brock_weather_tests.py and all other runners stay unchanged. This migration deliberately fails closed on any unverified approved record drift.
- **Open proof gap:** exact-head host test executions and downstream Brock/E4 source digest checks on the new branch must finish, and the broad current-main historical failure outside Baily must be separately classified. Until then keep draft and do not merge.

## Phase 1 continuation — exact-head CI and Tucker equivalence (2026-11-14)

### Existing CI on prior exact head `529bfd29711b962661d95c72156222e697a7e657`

- `Battle split validation` run **43657299764**: **FAILURE**: independent upstream-baseline job **132638633044 SUCCESS**, modified job **132638633260 FAILURE**.
- First modified error: `tests/run_new_fire_moves_tests.py:48` found a `MOVE_FLAME_FANG` / `MOVE_FLASHFIRE` / `MOVE_BLAZE_RUSH` reference in `src/data/pokemon/level_up_learnsets.h` where the existing test demands **none**. The generic `any()` assertion does **not** identify which of those three move tokens caused the failure. Do not infer authorization, dismiss the failure as a stale hash, or change the move data/test in this validation PR.
- `BTL-005 Batch A focused native validation` run **43657299771**: **SUCCESS**.
- No claim of complete modified-game acceptance or passing checks not reached after the first failure. This unrelated blocker stays open for owner-scoped review outside Phase 1.

### Second scoped adapter: Gentleman Tucker

- Verified the **entire** `sParty_GentlemanTucker` and `TRAINER_GENTLEMAN_TUCKER` records at pinned historical `21a3630b3d51481ab10a1af9637fa1ced73722cc`, approved `02240690581eb70d70219024fb7f55da2882308b`, and live main. Exactly **one** occurrence of each was found in each version. Approved-main and current-main records were byte-identical.
- The reviewed party is Voltorb Lv22 with Charge, Spark and two empty move slots; the original was Pikachu Lv23. Trainer metadata changes only the party binding from `NO_ITEM_DEFAULT_MOVES` to `NO_ITEM_CUSTOM_MOVES`; original trainer class, items, Singles setting and other fields remain protected.
- The existing `tests/approved_gym001_tucker_changes.py` accepts either exactly approved current content (reverse to historical) **or** already-historical content (leave unchanged). This is different from the Baily helper and was explicitly preserved.
- Read-only source-equivalent differential comparison on both actual full `trainer_parties.h` and `trainers.h` showed old/new **identical whole-file output** for approved current and already-historical Tucker records. Altered party species was rejected by both; duplicate approved record was rejected by both. Changes to unrelated records were tolerated by **both scoped adapters**, and still require the original final whole-file SHA checks. A change matching an **exact original** metadata record is accepted by both, by the pre-existing historical-input contract—not a new exception.
- Migrated only the Tucker historical adapter's exact-approved branch to `reverse_pinned_records`. Its original two `REVIEWED` before/after literal records, approved-or-historical selection, count checks, and unrelated-path behavior remain untouched. The shared helper independently checks that both current and historical records match their pinned approved snapshots before reversal.
- **Limit:** This read-only JS comparison checks equivalent source logic and real pinned source records; it is not an execution of the new Python helper in the full GitHub runner and not full coverage of all adversarial inputs. Further targeted differential tests and final strict original-digest/CI evidence remain required. No merge is authorized.

### Updated unresolved gaps

- Full inventory of all historical adapters, protected files, manifests and owner-approved record identities; current table remains partial.
- Explicit negative mutation fixture suite exercising the **actual imported Python helper and migrated adapters**, including outside-record edits rejected at final SHA; compare against baseline old runner behavior.
- Exact-head mandatory checks after the Tucker adapter change; the previous head has a separate Fire move distribution failure and cannot certify this head.
- Check and preserve exact original frozen fingerprints and policy; investigate Fire distribution separately under its own owner-approved work, not here.
