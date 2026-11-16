# SOW-002: Viridian Forest Heracross

Status: implemented for draft review; compiled/native and required CI pending. No merge, release, task closure or points claimed.

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

`LIGHTNING_SOW002_ONLY=1 python3 tests/emulator/run.py` executes the new focused native group and returns before existing groups. Existing default registrations remain. Planned evidence: native collision/BFS route from southern entrance to interaction tile; actual loaded icon VRAM tiles and stationary position; real D-pad/A dispatch; actual party/PC Master Ball catches, actual move defeat, actual opponent-induced loss, Run, flash persistence and reentry. Fixture warps/party/HP make outcomes deterministic; this does not certify a natural full-story walk or hardware. Original native guards and ABI-generated offsets remain unchanged; added offsets are generated with the target compiler.

Commands executed: source checker PASS; existing `run_sow_event_tests.py` PASS; Python parse and `git diff --check` PASS. Build/native pending. No rain/full-native test run. All existing workflow files are byte-identical. New `sow002-supplemental.yml` is review-event-only, owner/exact-branch/same-repo/current-head gated, with explicit `RUN SOW002 ONLY <40-char-reviewed-head>` command. It verifies checkout SHA and the exact early-return selector before build/run; pinned approved agbcc and mGBA0.10.5 dependencies. No uploads/caches/releases or write permissions. Publish commits use `[skip ci]` so existing push/PR workflows cannot initiate prohibited rain. The supplemental job is evidence only and cannot replace required modified/upstream gates.

Required checks/protection information and full impacted integration/reproducibility remain separate merge requirements. Protection-details read403 and empty rulesets do not establish absent protections. This draft must remain unmerged while those requirements are unresolved.

Source review caught the native removeobject command also setting its map hide flag. Separate resolved4AD/hide4AE flags and original-bird-style transition reset prevent a fled event being mistaken for permanent success. Initial0df463d run43784089671 passed source checks and ROM build, then native runner linking failed: LIBMGBA_ONLY forces a static mGBA archive despite BUILD_SHARED=ON, producing unresolved exp2f/sincosf. LIBMGBA_ONLY=OFF with all frontends/tests disabled builds the intended headless shared library. No native assertions executed on that run. The corrected event/setup requires fresh exact-head evidence.
