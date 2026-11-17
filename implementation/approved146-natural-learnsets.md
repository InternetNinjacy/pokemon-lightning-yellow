# Feature: Exact146 approved natural learnsets

**Project:** Pokémon Lightning Yellow  
**Status:** Implemented in source; source checks/clean build pass; owner gameplay acceptance pending  
**Approval:** Owner approval, November17,2026, [issue105 record](https://github.com/InternetNinjacy/pokemon-lightning-yellow/issues/105#issuecomment-6951068928)  
**Handoff revision:** Exact74+72 adoption  
**Dependencies:** Existing410-move source catalog; no dependency on TM51 eligibility

## 1. Goal and player-facing behavior

Apply the exact146 natural level-up recommendations explicitly adopted by the owner. Pokémon use their approved species-specific level/move sequences through existing level-up/evolution/Move Reminder behavior. This change does not add an independent learning mechanism or edit already-owned Pokémon's current moves.

## 2. Approved design decisions

The normative146 full sequences are the `after` rows of these two original manifests at pinned PR173 commit `86f4f73a0e2767984cfc15fe52a3abe0a7396017`:

- `tests/specifications/pkm009_delegated_001_126.json`:74 rows.
- `tests/specifications/pkm009_delegated_127_252.json`:72 rows.

The new approval adopts only those exact rows. Their older WORKING labels remain historical provenance; they are not the current approval state for this narrowly adopted subset. The new [source contract](../tests/specifications/pkm009_approved146.json) records the owner decision, source-manifest Git blobs/SHA256 values,146 exact current-main before blocks and approved after blocks. No recommended move/level has been substituted.

## 3. Authoritative sources and provenance

- [Owner adoption record](https://github.com/InternetNinjacy/pokemon-lightning-yellow/issues/105#issuecomment-6951068928).
- PR173 immutable source `86f4f73a0e2767984cfc15fe52a3abe0a7396017`; only its146 adopted sequences are used.
- Baseline main `28cf138cfcd19ee8f742fdd12b4bec1793b39cf4`.
- Native table: `src/data/pokemon/level_up_learnsets.h`; all146 symbols are enumerated in the source contract.

## 4. Implementation boundaries

**In scope:**146 exact natural-level arrays only, plus scoped source validation and strict historical inverse mapping.

**Preserve:** Every other table byte; all14 owner-locked profiles; Dugtrio's FireRed/LeafGreen conditional array; species IDs, stats, typing, abilities, catch rates, evolutions, TM/HM compatibility, existing move IDs, Trainer parties, encounters and save fields.

**Excluded:** TM51 eligibility/item/reward implementation; the separate10 later proposals for Bulbasaur/Ivysaur/Venusaur, Arbok/Nidoking/Ariados, Nuzleaf/Shiftry/Breloom/Cacturne; all gameplay or distribution decisions beyond the146 adopted rows. The14 locked profiles need no further design approval and are not overwritten from an older branch.

## 5. Technical implementation notes

Only `src/data/pokemon/level_up_learnsets.h` changes in production. Each current-main array must first match its pinned original `before` sequence, then be replaced by the exact approved block. Do not copy PR173's complete learnset or species files.

The inverse helper `tests/approved_146_learnset_changes.py` accepts an entirely approved or entirely baseline set, rejecting missing/duplicate/modified/mixed protected arrays. It reverses the146 blocks for earlier frozen audits. Composed historical projections must retain their final whole-file checks; unrelated bytes remain visible and are not silently discarded. No old expected digest is rewritten.

## 6. Acceptance criteria and validation

- [x]74+72 distinct arrays map exactly to both pinned approved manifests.
- [x]All referenced moves exist in the approved410 catalog; levels remain1–100.
- [x]Exact whole-file reversal reproduces baseline main, proving every other array byte is preserved.
- [x]All14 locked profiles and the separate10 proposals are excluded; species/TMHM/evolution source unchanged.
- [x]Focused source tests reject modified, missing, duplicate and partially projected protected arrays and expose outside-scope drift.
- [x]46 shared source runners,25 history cases,6 locked-pair cases and4 Snow source controls pass; clean pinned-toolchain build passes on combined current-main production inputs.
- [ ]Owner ROM playtest for representative level-up, evolution, Move Reminder and save behavior.

Command: `GIT_DIR=/workspace/pokemon-lightning-yellow/.git python3 -m unittest discover -s tests -p test_pkm009_approved146_source.py -v` from the isolated source snapshot. Five source cases pass. Current-source CI registers this check and four preserved locked-family source suites (11 cases PASS) alongside the unchanged locked-pair check. Clean `make -j2` from an object-free snapshot passes;16MiB ROM SHA256 `774a0437a919b00e57862fd41f21a863153bf7c3c3de3555fabbf200ab6d377d`, EWRAM1076 bytes free. No gameplay/emulator/RNG execution or CI dispatch occurred; skipped CI is not a passing run.

## 7. Open questions and blockers

None for the exact146 adopted source sequences. TM51 remains a separate design dependency, not a prerequisite for these arrays. Source implementation is not runtime/release acceptance.

## 8. Codex delivery requirements

Integrate on a reviewable topic branch. Preserve the existing50 task IDs and461/251/712 accounting until owner acceptance. Record the actual source/build evidence and unexecuted runtime conditions. Standing owner source-ready merge authorization applies under existing GitHub protections; never enable automatic merging or claim skipped CI passed.

## 9. One-line Codex handoff

`Implement only this exact146-row owner adoption, preserve the14 locked profiles and every unrelated byte, retain frozen source checks, and report source/build versus unexecuted owner gameplay acceptance separately.`
