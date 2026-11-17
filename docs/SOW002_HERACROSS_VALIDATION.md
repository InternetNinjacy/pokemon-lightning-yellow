## Current source integration checkpoint

PR265 merged main20ae65d3: complete410-move source, combined compile and static
catalog/frozen-history checks PASS; no gameplay tests or accepted points.
PR254 Heracross is reconciled on that main: resolved4AD/hide4AE and Route6 claim4AF
retained; all other flags unchanged. Prior254 native evidence remains historical.
Current combined build and source checks PASS; no CI dispatch or guard weakening.
461/712 preserved. Runtime acceptance remains owner-run.

# SOW-002: Viridian Forest Heracross

Status: implemented on draft PR254; exact source a9198e74158f5be9d6b7d35d565a7dc3cffc2f5f built and passed3505 focused native assertions. Required CI/integration/protection evidence remains unresolved. No merge, release, task closure or points claimed.

The owner explicitly requests finalization, implementation and validation of the missing encounter. Canonical design: [Special Overworld register](https://docs.google.com/document/d/1yTOKL8qr5_3sX6Fpk9WEzTBw9c6IwoDVWLULJtTIXm8/edit), [final handoff](https://docs.google.com/document/d/1toiE5HMe9b1Wa5IVytgM2LIHTT8_v05jkVgfPURYsH4/edit). Execution ownership: [issue253](https://github.com/InternetNinjacy/pokemon-lightning-yellow/issues/253), branch `feature/sow002-viridian-heracross-20261115` from main `197d85ee1a61ac319f95fa65ab298bb126fe937d`.

| Field | Final implementation |
| --- | --- |
| Map / object | Existing northeast clearing, `(49,6)`, local12, elevation3; no terrain edit |
| Behavior | Stationary, nonreactive, no idle animation or turn to face player; A starts battle |
| Sprite | Existing Heracross party icon, first frame only, correct native palette2; new gfx155 |
| Level / moves |12; native level-up Tackle, Leer, Horn Attack, Endure |
| Ability / item | Native Swarm or Guts; no forced selector, held item or custom persistence |
| Catch rate |125; species-wide45→125, applying owner's early-catch125 instruction to this newly installed early source |
| Capture / defeat | Permanent resolved flag4AD, separate native hide flag4AE, static removal, no return after save/reentry |
| Flee / Teleport | Native static temporary removal and fled message; return on reentry, no resolved flag |
| Loss | Native whiteout; remains unresolved and available on return |
| NPC | Existing non-battling Youngster29,58: unusually strong Pokémon attacking travelers; northeast hint, no species name; thanks/safe passage after capture/defeat |
| Weather / lighting | Existing forest policy unchanged; no forced weather or special-weather producer |

Exact level/moves/item/coordinate were historically deferred. The current explicit owner instruction to finalize supplies delegated finalization authority; the choices above are recorded as new decisions, not retroactive approvals.12 is stronger than local ordinary4–6 while using only existing early level-up moves. No global move/ability/base-stat/learnset edits. Only Heracross catchRate changes in species data.

`tests/check_sow002.py` locks the original11 Forest objects, all map header/warp/coordinate/background events, terrain/border, existing battle/static producers, weather source and learnsets. It verifies the exact appended object, non-spoiling NPC, outcomes, nonanimated art, palette, single species catch delta and fail-closed selector. The previous three SOW source contracts remain; the graphics capacity assertion grows155→156 to require the appended slot, with no old checks removed.

`LIGHTNING_SOW002_ONLY=1 python3 tests/emulator/run.py` executes the new focused native group and returns before existing groups. Existing default registrations remain. Executed group coverage (results below): native collision/BFS route from southern entrance to interaction tile; actual loaded icon VRAM tiles and stationary position; real D-pad/A dispatch; actual party/PC Master Ball catches, actual move defeat, actual opponent-induced loss, Run, flash persistence and reentry. Fixture warps/party/HP make outcomes deterministic; this does not certify a natural full-story walk or hardware. Original native guards and ABI-generated offsets remain unchanged; added offsets are generated with the target compiler.

Initial local checks: source checker PASS; existing `run_sow_event_tests.py` PASS; Python parse and `git diff --check` PASS. Subsequent exact-source build/native results are recorded below. No rain/full-native test run. All existing workflow files are byte-identical. New `sow002-supplemental.yml` is review-event-only, owner/exact-branch/same-repo/current-head gated, with explicit `RUN SOW002 ONLY <40-char-reviewed-head>` command. It verifies checkout SHA and the exact early-return selector before build/run; pinned approved agbcc and mGBA0.10.5 dependencies. No uploads/caches/releases or write permissions. Publish commits use `[skip ci]` so existing push/PR workflows cannot initiate prohibited rain. The supplemental job is evidence only and cannot replace required modified/upstream gates.

Required checks/protection information and full impacted integration/reproducibility remain separate merge requirements. Protection-details read403 and empty rulesets do not establish absent protections. This draft must remain unmerged while those requirements are unresolved.

Source review caught the native removeobject command also setting its map hide flag. Separate resolved4AD/hide4AE flags and original-bird-style transition reset prevent a fled event being mistaken for permanent success. Initial0df463d run43784089671 passed source checks and ROM build, then native runner linking failed: LIBMGBA_ONLY forces a static mGBA archive despite BUILD_SHARED=ON, producing unresolved exp2f/sincosf. LIBMGBA_ONLY=OFF with all frontends/tests disabled builds the intended headless shared library. No native assertions executed on that run. The corrected event/setup requires fresh exact-head evidence.

## Executed acceptance evidence

[Run43784608441](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/43784608441), job133029340512, source/workflow a9198e74158f5be9d6b7d35d565a7dc3cffc2f5f: SUCCESS. Guard verified the exact source before dependency/build/native phases; agbccda598c1d and mGBA0.10.5/26b7884b pinned. Both SOW source contracts PASS, game build PASS, native group3505 assertions PASS. Original4M invocation and scene guards/ABI boundaries retained.

Native results: boot600 frames without supplied BIOS; RTC detected; actual Forest collision BFS finds74 land tiles from29,61 to interaction49,7; encounter49,6 is collision-free. Correct Heracross frame present in native sprite VRAM and coordinates unchanged across180 frames. Real D-pad/A starts encounter, Run temporarily removes it without success, flash/reentry restores it. Actual Bag Master Ball party/PC captures permanently remove it and preserve PID/OT/native ability/moves through flash and reentry. Actual Swift defeat and opponent-induced loss/whiteout select correct resolution/retry paths. Physical southern NPC interaction verifies warning before and thanks after resolution. No injected battle-outcome success, weather native group or rain test. Teleport routing is source-checked/reuses original static helper, not a separate executed Teleport acceptance. Collision BFS plus adjacent physical interaction does not certify a natural campaign walk.

Additional focused host checks exposed existing failures: run_special_glalie_tests.py:20 frozen weather-species SHA mismatch; run_world_reward_tests.py reaches approved_ordinary_trainer_projection.py:73 trainer-block mismatch. Both reproduce on exact main197d85ee source controls (temporary source archive using existing git history); no assertion/normalizer edits. These are not replaced with supplemental success and require their owners' reconciliation before applicable merge gates can pass. Original Fire/Misty failures on PR249/233 remain separately preserved.

Final evidence/tracking edits after a9198e7 are documentation only; build/test/workflow/production input equivalence is checked rather than rerunning unchanged focused native. Latest draft-head CI is not misrepresented as tested. No merge or Playtest002 update; main remains197d85ee.

## Required-CI blocker follow-up — 2026-11-15

Refreshed current main197d85ee, PR2540997e41, issue105, canonical START HERE/Master Plan/Change Intake/handoff/SOW register and dependency owners233/249/250. Protection detail still403; rulesets[] and protected:false metadata are not treated as a verified required-check list. A read-only browser fallback reached a logged-out settings404, so no authenticated settings evidence is available. No security/settings changes or merge attempt.

Source follow-up238ec53ac058fc715d883336337909065f21f180 adds a strict catch-rate reverse layer before the existing weather-species projection: exactly13 approved merged PR251 catch fields and Heracross45→125. Existing frozen hash assertions/snapshots are unchanged. Three complete allowed source stages and17 negative controls pass; invalid/mixed rates fail, unrelated rate/base-stat edits remain visible to downstream equality checks. Current reconstructed species source exactly equals frozen base0224069/a82ee89b. Remaining historical mismatch is exactly the six approved Vulpix/Ninetales/Ponyta/Rapidash/Eevee/Flareon records; PR233 already owns that reconciliation and the trainer projection. No assertion changes from233 are imported; old Glalie/world-reward checks remain blocked by that dependency, not green.

Separate new review-command-only sow002-build-proof.yml requests supplemental modified clean reproducibility and independent unchanged upstream twice-clean make compare evidence. Exact reviewed-head, same repo/branch/PR254/owner/state guards, pinned compiler/preparation equivalence; no native/emulator/rain commands or uploads. All prior workflows (including SOW002 native) byte-identical. Distinct supplemental names cannot replace required statuses. Run43786553053 is in progress at publication; result pending. The sibling original SOW002 workflow43786552995 correctly skipped the different BUILD PROOF command, so no native/rain suite was started. Production/native runner/fixtures/compiler are unchanged from3505-tested a9198e7; do not repeat that unchanged native evidence unnecessarily.

### Terminal clean/upstream results and exact merge decision boundary

[Run43786553053](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/43786553053), exact checkout238ec53ac058fc715d883336337909065f21f180, SUCCESS:

| Evidence | Job | Result |
| --- | --- | --- |
| Modified source contracts / strict catch projection / two clean builds with matching SHA256 / tracked-source preservation |133035404503 supplemental-sow002-clean-proof |SUCCESS |
| Independent pret/pokefirered ce6406d8 with approved pinned agbcc; make compare twice from clean and explicit firered.sha1 check |133035404219 supplemental-sow002-vanilla-proof |SUCCESS |

These are actual build/reproducibility/upstream evidence, not aliases for the required modified/upstream-baseline status checks. Neither job executes emulator or rain tests. All existing workflows remain byte-identical. New code at238ec53 changes only the source-only catch compatibility layer/check and additional isolated workflow; game/native runner/fixture/compiler inputs match3505-tested a9198e7 exactly. The original native workflow43786552995 correctly SKIPPED the different BUILD PROOF review command; do not call that a native pass. Subsequent documentation-only evidence retains both input-equivalence boundaries.

**Remaining gate 1 — historical owners:** PR233dde9c7d owns six-species/trainer/Fire historical reconciliation. Catch-only normalization does not resolve those independent deltas. Existing old host assertions remain failing, not waived. Preserve PR249 Fire48 failure43754474983/132935433549, upstream132935433210 SUCCESS; PR233 unchanged Misty2400 failure132895732192/upstream132895732475 SUCCESS and diagnostic6899437328. No tests/assertions imported from233, no timeout/gameplay/rain change.

**Remaining gate 2 — owner/access decision:** confirm the actual required-check configuration with authenticated protection/settings evidence. Connector protection403 and logged-out browser settings404 cannot supply it. Then resolve the explicit conflict between the standing no-rain/full-native restriction and the existing required full modified workflow. Either explicitly authorize that unchanged required execution, or separately review a CI policy using verified protection information while preserving the required gates. This branch does not select a waiver, relabel statuses, change protection or start prohibited tests. Source/build supplementals cannot settle this decision. PR254 remains draft/unmerged, main/Playtest002 unchanged, no points.

Current-main source integration: exact Heracross placement/data/script/art contract PASS; original objects,terrain,weather,battle paths and learnsets preserved. Catch projection PASS:14 approved fields,3 complete stages,17 negative preservation controls.410-move catalog and dual frozen BTL/weather checks PASS. Combined pinned-agbcc compilation PASS; private engineering ROM SHA256 `fdea969f7b086f743ccc268eb5b2d1249a8e82c279fa96f2294cd5ecba7f2559`. No gameplay/emulator tests or CI dispatch. Historical3505 native assertions remain historical, not current-head acceptance. Existing Route6 claim4AF and Heracross4AD/4AE all retained.461/712 unchanged.
