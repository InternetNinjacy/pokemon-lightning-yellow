# WRL-001 supplemental exact-commit CI proposal

Status: separate review-only draft, not activated or executed. Prepared on
November 15, 2026 under the owner's explicit instruction to prepare this proposal.
No gameplay, existing workflow, assertion, required status or protection changes.

## Purpose and exact source

Produce supplemental hosted evidence for the already focused-validated WRL-001
correction, without executing rain or full-native tests. Success would describe
only the pinned source below. It cannot replace required modified/upstream CI,
relabel an earlier failure, approve a merge, or award points.

| Reference | Verified value |
| --- | --- |
| Main and proposal base | `02240690581eb70d70219024fb7f55da2882308b` |
| PR #249 source to execute | `0f693dd7bef4219e161b299f08a9a225ecdac9d6` |
| PR #233 reconciliation owner | `dde9c7d245b3ed9cf21ec475c65b2651f4e07921` |
| Pinned agbcc | `da598c1d918402c42c0c0d7128ba14567f3175e9` |
| Approved mGBA 0.10.5 release commit | `26b7884bc25a5933960f3cdcd98bac1ae14d42e2` |
| Pinned checkout action v4.2.2 | `11bd71901bbe5b1630ceea73d27597364c9af683` |

This branch starts from main, not PR #249 or PR #233. It contains no gift,
Pikachu, weather, Fire assertion or historical projection changes. The workflow
checks out the independent PR #249 source SHA into `source/`; it never tests a
moving branch or a synthetic PR merge commit. The controller checkout is separately
pinned to the workflow event SHA and verified. Both SHAs appear in the run log.

Authority and reused gameplay evidence:
[latest CI boundary](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/249#issuecomment-6901283859),
[WRL-001 validation record at the pinned source](https://github.com/InternetNinjacy/pokemon-lightning-yellow/blob/0f693dd7bef4219e161b299f08a9a225ecdac9d6/docs/WRL001_GIFT_OWNERSHIP_VALIDATION.md).
The previous source/build/681-assertion pass is retained as previous focused
evidence; no game build or emulator test was repeated while preparing this draft.
Previously acquired SUE is still not migrated.

## Proposed execution

`.github/workflows/wrl001-supplemental.yml` has one distinct job,
`supplemental-wrl001-only`, with read-only contents permission. A dispatch must
explicitly request the approved full source SHA; any different or absent SHA fails.

Before dependencies, build or native execution, the controller verifies exact
source HEAD, clean tracked source and SHA-256 fingerprints of the reviewed Python
launcher, C runner, WRL-001 fixture and pinned compiler-preparation script. It also
requires the exact early-return `LIGHTNING_WRL001_ONLY` branch. Missing/changed
selector inputs fail; there is no fallback to the default native suite.

The proposed game commands are exclusively:

```sh
python3 tests/run_pikachu_opening_tests.py
scripts/prepare-baseline.sh
make -j2
# Executed by the guard with a sanitized environment and pinned mGBA prefix:
LIGHTNING_WRL001_ONLY=1 python3 tests/emulator/run.py
```

The existing preparation script retains the pinned compiler and upstream build
inputs. Compiler prerequisites follow the existing Ubuntu 24.04 workflow. That
distribution's packaged mGBA is older than the recorded 0.10.5 evidence, so the
proposal builds the same approved 0.10.5 release as a headless shared library at
the immutable upstream commit above, with Qt/SDL, mGBA test suites and ROM-test
tools disabled. CMake and zlib development headers support this dependency build;
the game's original compiler and test sources are untouched. The existing
`MGBA_PREFIX` interface selects the private library and headers. A missing library
or version other than 0.10.5 fails before native execution; no system fallback.

Immediately before native execution, the source guard runs again. All inherited
`LIGHTNING_*` variables, including the earlier title-capture selector, and the
inherited mGBA prefix are removed. Only `LIGHTNING_WRL001_ONLY=1` is selected.
The original runner and all its ABI/invocation/assertion guards execute unchanged.
Both process exit zero and its exact existing 681-assertion focused completion
line are required. No Fire test, weather test, whole Parcel replay, clean
reproducibility rerun or default/full-native invocation is added.

There are no artifact uploads, caches, releases, API dispatch calls or repository
writes. Game build inputs and outputs stay on the ephemeral runner. No ROM,
reference ROM, BIOS, ELF or save is committed or uploaded as an artifact.

## Publication and activation boundary

The proposal branch is `proposal/wrl001-exact-commit-ci-20261115`, outside every
existing push branch filter. Its publication commit carries `[skip ci]`, which
suppresses existing `push`/`pull_request` triggers on this draft. The new workflow
itself has only `workflow_dispatch`: no `push`, `pull_request`, schedule,
`workflow_run`, `pull_request_target` or comment trigger. None of the seven
existing workflows has an event that escapes the publication controls. Creating
this draft must be followed by an exact-head run-list readback.

Existing workflows are byte-identical to main. Their status names, assertions,
upstream checksum, triggers and gates remain in force. Suppressed/pending checks
are not green. Do not strip `[skip ci]`, push a replacement commit, retarget,
promote, merge or dispatch as a routine follow-up that would start broad tests.

**Activation is a separate review decision.** GitHub requires a manual-dispatch
workflow to exist on the default branch. This new workflow does not exist there;
the draft therefore supplies no claim that it is runnable now. A normal merge
to main could trigger the unchanged broad main-push workflow and prohibited rain
tests. Do not merge to install it or dispatch any existing broad workflow. The
review must establish an installation/trigger path consistent with the owner's
no-rain direction before execution. An explicit PR-label-only opt-in could be a
separately reviewed alternative, because current PR workflows use only default
opened/synchronize/reopened events; it is not implemented or authorized here.

References:
[GitHub skip semantics](https://docs.github.com/en/actions/how-tos/manage-workflow-runs/skip-workflow-runs),
[manual dispatch default-branch requirement](https://docs.github.com/en/actions/how-tos/manage-workflow-runs/manually-run-a-workflow),
[Ubuntu mGBA package versions](https://packages.ubuntu.com/search?keywords=mgba),
[mGBA 0.10.5 source](https://github.com/mgba-emu/mgba/tree/26b7884bc25a5933960f3cdcd98bac1ae14d42e2).

## Verification of this proposal

- `python3 tests/test_wrl001_ci_guard.py -v`: 11 launcher-safety checks PASS.
  Real pinned file bytes are inspected; Git metadata, dynamic-library loading
  and native subprocesses are mocked. No compiler, ROM or emulator is executed.
  Cases cover wrong SHA, dirty source, absent/changed selector inputs, missing
  early return, missing/wrong mGBA, inherited selector cleanup, nonzero native
  exit and missing focused completion. These are guard checks, not 11 gameplay
  tests or a new 681-assertion pass.
- Actionlint 1.7.7: PASS for the proposed workflow; YAML and Python syntax PASS.
  Shellcheck/pyflakes integrations were unavailable; actionlint ran with those
  integrations disabled. Bash steps are additionally syntax checked locally.
- Existing workflow byte comparison and trigger/no-upload audit: PASS.
- Diff/whitespace and unchanged 50-ID / 461-earned / 251-remaining / 712-total
  accounting checks: PASS. Tracker/dashboard unchanged.
- Proposed dependency installation/build and hosted source/build/native path:
  NOT EXECUTED. No new supplemental CI success claimed.

## Preserved failures and dependencies

PR #249 remains draft at its original checkpoint. Original
[run 43754474983](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/43754474983)
modified job `132935433549` remains FAILURE at
`tests/run_new_fire_moves_tests.py:48`; upstream `132935433210` remains SUCCESS.
No Fire assertion, fingerprint, learnset or PR #233 adapter is imported here.
Historical reconciliation ownership stays with PR #233.

PR #233's existing host/build passed; modified `132895732192` failed the unchanged
Misty 2400-frame assertion and upstream `132895732475` succeeded. Existing
[diagnostic comment](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/233#issuecomment-6899437328)
is preserved. No rain rerun, timeout change or gameplay investigation is authorized
by this proposal.

Branch-protection reads returned 403; `rulesets=[]` does not establish absence of
classic protection. No actual required-check list is claimed. Required-gate changes
need separate policy review and verified protection information; supplemental
success, if later obtained, does not resolve that requirement.

Owner-accepted rain, Sun, Snow palette/Hail and Parcel/Pokédex/Eevee delivery
remain accepted. WTH-002 #248 still awaits sustained snow-particle confirmation
on Playtest002; WRL-010 remains queued. No merge, acceptance credit or points.
