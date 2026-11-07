# Pokémon Lightning Yellow — Master Implementation Change Ledger

Canonical branch: `main`. Established on historical `docs/project-coordination` on 2026-11-02; owner-authorized consolidation moved the live record to main.
Drive synchronization: **pending**. No document has been created on Drive. Intended destination: [existing project folder](https://drive.google.com/drive/folders/1N_n0Ke48jBbdhUWSZWzwR6rpPFqpDUpl).

Git history is the authoritative technical record. Historical entries below are backfilled from commits and their committed reports; they are not fresh executions. Commit dates are Git dates, not inferred design-approval dates. Design approval and code implementation are separate events. Preserve these entries and append corrections/supersessions.

## Current Effective Version

| System / tasks | Current approved authority | Implemented on main | Feature branch / remaining |
|---|---|---|---|
| Baseline / INT-001 | FireRed rev0; pret ce6406d8; pinned compiler | Independent upstream original SHA-1 verified | Retain separate unchanged build |
| Dex/typing / PKM-001,003,005,007 | 252; Castform233;35 effective approved typing rules | Implemented and merged | Broader evolution/Dex/link acceptance remains |
| Evolution / PKM-002,004,008 | Six direct-use trade-item pairs; Tyrogue choice; Eevee; Ponyta30 | Implemented and merged | Preserve exclusions and pending external acceptance |
| Categories / BTL-001,003 | GenIV move categories; Shadow Ball SPECIAL | Implemented and merged; damage/AI/UI acceptance | Broader trainer-AI strategy testing |
| Wild doubles / BTL-002,004 | Independent pairs/5% special pair; experimental maps | Implemented and merged proof of concept | Extended capture/faint/forced-end/weather acceptance |
| Move redesign / BTL-005 | Supplied complete register/ledger/type reviews available | Original354 plus approved split; redesign not implemented | PR40 M1a Curse/name storage/summary implemented; M1b layouts and new IDs/effects/animations/distribution remain |
| RTC / TIM-001,002 | Read-only RTC; functional06–18 day, invalid-time policy | Implemented and merged | Hardware/platform acceptance |
| Visuals / TIM-003 | Approved periods/map exceptions/fallback supplied | Original visual appearance | Implement/prototype; final tint visual approval |
| Weather / WTH | Approved83-map distributions/entry/recovery; fixed Gym exceptions supplied | Original regional weather; narrow Snow-to-Hail bridge | PR29 selection/persistence/RTC/native bridge implemented and CI passed; PR31 fixed Gym scope passed; combined PR35 acceptance; final tints/fog/manual coverage remain |
| Route5 / DAY | Two independent daycares; persistent50% Shiny eggs | Implemented and merged; actual UI/save/hatch/statistical tests | Real link/hardware/full breeding progression acceptance |
| Gyms / GYM-001..007 | Exact approved Castform sets in PR30; other handoffs preserve unassigned balancing | Blaine correction and Ranger Base Camp merged | PR31 three exact Castform parties/Double/fixed weather passed native/CI; PR35 adds Blaine victory/badge/TM38/Continue/revisit acceptance; GYM-003 superseded by007, no missing Castform levels/moves |
| Rivals / GYM-006 | Ready v2 specPR25 mergedc960642d | Original rival gameplay remains on main | Exact eight teams/24 IDs/family learnsets implemented and CI passed in PR27; combined PR35 acceptance; natural story progression remains separate |
| Fossils/Museum / WRL-004,006,007 | Owner final handoffs and Museum event | Implemented and merged | Other item/event scope and extended progression acceptance |
| World eligibility / PKM-006 | Global252-only rule; no off-Dex exceptions | Audit/no-new-findings guard implemented | Strict compliance not passed; approved replacements/reference review remain |
| Mystery vendors / WRL-008 | Approved v1.0/CSV; ten vendors; foreign OT;90% Shiny | Implemented and merged | Separate starter integration/external acceptance |
| Poké Balls / BTL-006 | Approved register; existing IDs/art and stored codes | Implemented and merged; capture/shop/weather/native tests | Extended all-ball presentation/progression acceptance |
| Starter/encounters/shops / WRL-001..005 | Supplied approved portions; unassigned values not invented | Pikachu starter/final encounters/evolution shops not yet implemented | Finish exact approved portions, preserve SOW HOLD and pending values |

Current view reconciled November4 after Ranger merge and rival specification merge.
Historical execution entries below retain their original dated evidence; they are
not current blockers. Supplied exports are available; direct Drive writing is not.

## Design and authority history

- **DES-001** — Supplied October28 coordinator handoff establishes exact historical251 roster, twelve typing overrides, roster-aware evolution access and functional06–18 clock. Linked Drive originals were not independently read. The local reconciliation transcription preserves the rules. Approval to execute is supplied separately by the owner, most recently “Implement the proposed plan.”
- **DES-002** — Owner explicitly expands251→252 with Castform#233; supersedes DES-001 only for roster size/order insertion. All existing species/order preserved. Castform Normal/Forecast/forms unchanged; exactly one future Celadon trainer pending details.
- **DES-003** — Latest evolution mechanics specification supersedes older direct Clamperl item alternatives. Six direct-use trade-item pairs remain; original trade methods stay. Do not reintroduce excluded methods from PR#4.
- **DES-004** — Owner's latest consolidated execution/dashboard/ledger request and subsequent approval authorize this execution plan. They preserve separate approval for merging, auto-merge, releases and destructive actions. Unknown finalized design values must be obtained, not invented.

## Implementation history

### 2026-10-25 — INT-001 — e04252ea (historical backfill)

- Previous → new: Empty project → pinned upstream FireRed source, retained licenses/build structure and server-side baseline verification.
- Design authority/version: Owner repository-setup request; upstream ce6406d8c2eaf46d543c8a2606de0c3cb1951982.
- Source files: README.md, AGENTS.md, docs/UPSTREAM.md, scripts/prepare-baseline.sh, .github/workflows/build.yml; upstream source import.
- References: branch `setup/firered-baseline`, [commit](https://github.com/InternetNinjacy/pokemon-lightning-yellow/commit/e04252ea0783510000e634486c0f2526a736f506), [PR#1](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/1).
- Implementation status: merged baseline.
- Automated / emulator evidence: Committed baseline/build documentation; original SHA-1 retained independently. No new reference ROM.
- Save compatibility, dependencies and unresolved issues: No custom save changes; only merged PR in current stack.
- Supersession: only explicit changes above supersede earlier behavior; no whole-feature replacement implied.

### 2026-10-26 — BTL-001 — e265bcf3 (historical backfill)

- Previous → new: Type-based damage categories → explicit per-move Physical/Special/Status, including Shadow Ball Special.
- Design authority/version: Owner approved split assignment and explicit Shadow Ball Special correction.
- Source files: include/pokemon.h, include/battle.h, include/constants/move_categories.h, src/data/battle_moves.h, src/pokemon.c, src/battle_script_commands.c, src/battle_ai_script_commands.c, src/battle_controller_player.c, src/pokemon_summary_screen.c, tests/run_split_tests.py, docs/PHYSICAL_SPECIAL_SPLIT.md (consult commit for complete list).
- References: branch `feature/physical-special-split`, [commit](https://github.com/InternetNinjacy/pokemon-lightning-yellow/commit/e265bcf3efa07531a4419fd471f6157fb0980cad), [PR#2](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/2).
- Implementation status: implemented on unmerged dependency stack.
- Automated / emulator evidence: Host numeric production-routine tests and static audits; prior CI green. Full emulator UI/AI acceptance not established by this historical commit.
- Save compatibility, dependencies and unresolved issues: Move IDs preserved; modified ROM intentionally differs from vanilla. PR open.
- Supersession: only explicit changes above supersede earlier behavior; no whole-feature replacement implied.

### 2026-10-26 — TIM-001 — a5b98e9b (historical backfill)

- Previous → new: No clock → read-only cartridge GPIO RTC with validity return.
- Design authority/version: Owner separately reviewed RTC dependency choice.
- Source files: src/rtc.c, include/rtc.h, tests/run_rtc_tests.py, docs/RTC_SUPPORT.md.
- References: branch `feature/rtc-support`, [commit](https://github.com/InternetNinjacy/pokemon-lightning-yellow/commit/a5b98e9b), [PR#3](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/3).
- Implementation status: implemented on unmerged dependency stack.
- Automated / emulator evidence: Initial host tests; later actual emulator exposed wire-order defect, corrected below.
- Save compatibility, dependencies and unresolved issues: No save offset/clock writes; physical hardware unverified.
- Supersession: only explicit changes above supersede earlier behavior; no whole-feature replacement implied.

### 2026-10-26 — TIM-001 — 472cbba3 (historical backfill)

- Previous → new: RTC tests not in CI → automated RTC validation in build workflow.
- Design authority/version: Same RTC dependency approval.
- Source files: .github/workflows/build.yml, tests/run_rtc_tests.py.
- References: branch `feature/rtc-support`, [commit](https://github.com/InternetNinjacy/pokemon-lightning-yellow/commit/472cbba3), [PR#3](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/3).
- Implementation status: implemented on unmerged dependency stack.
- Automated / emulator evidence: Historical CI coverage added; not physical GPIO proof.
- Save compatibility, dependencies and unresolved issues: Superseded only where later decoder/serial corrections apply.
- Supersession: only explicit changes above supersede earlier behavior; no whole-feature replacement implied.

### 2026-10-26 — TIM-001 — f94b21e1 (historical backfill)

- Previous → new: Decoder-only checks → serial-loop validation.
- Design authority/version: Same RTC approval.
- Source files: tests/run_rtc_tests.py, src/rtc.c.
- References: branch `feature/rtc-support`, [commit](https://github.com/InternetNinjacy/pokemon-lightning-yellow/commit/f94b21e1), [PR#3](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/3).
- Implementation status: implemented on unmerged dependency stack.
- Automated / emulator evidence: Stubbed GPIO serialization tests; later corrected command expectation.
- Save compatibility, dependencies and unresolved issues: No runtime hardware claim.
- Supersession: only explicit changes above supersede earlier behavior; no whole-feature replacement implied.

### 2026-10-26 — TIM-001 — 91f3bce4 (historical backfill)

- Previous → new: Insufficient presence/12h checks → calendar validation and correct12h decoding.
- Design authority/version: Same RTC approval.
- Source files: src/rtc.c, tests/run_rtc_tests.py.
- References: branch `feature/rtc-support`, [commit](https://github.com/InternetNinjacy/pokemon-lightning-yellow/commit/91f3bce4), [PR#3](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/3).
- Implementation status: implemented on unmerged dependency stack.
- Automated / emulator evidence: Production decoder calendar/leap/absent input cases executed historically.
- Save compatibility, dependencies and unresolved issues: All-zero device rejected; valid midnight retained.
- Supersession: only explicit changes above supersede earlier behavior; no whole-feature replacement implied.

### 2026-10-26 — PKM-002 — 548cc10b (historical backfill)

- Previous → new: Trade-only targets → level40 alternatives, item-use alternatives and independent Eevee stones/timed friendship; defer consumption until successful scene.
- Design authority/version: Original evolution overhaul owner request; later DES-003 narrows item pairs.
- Source files: src/pokemon.c, src/evolution_scene.c, src/party_menu.c, src/item_use.c, src/data/pokemon/evolution.h, tests/run_evolution_tests.py, docs/EVOLUTION_OVERHAUL.md (full list in commit).
- References: branch `feature/evolution-system-overhaul`, [commit](https://github.com/InternetNinjacy/pokemon-lightning-yellow/commit/548cc10b), [PR#4](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/4).
- Implementation status: implemented on unmerged dependency stack.
- Automated / emulator evidence: Host target/consumption tests; initial scene coverage incomplete.
- Save compatibility, dependencies and unresolved issues: Original trade retained; cancellation preserves item. Clamperl direct use later superseded by851c76c9.
- Supersession: only explicit changes above supersede earlier behavior; no whole-feature replacement implied.

### 2026-10-26 — TIM-001 — ce126c5c (historical backfill)

- Previous → new: RTC commands LSB-first → MSB-first commands, LSB-first responses.
- Design authority/version: Same RTC approval; concrete mGBA-discovered defect repair.
- Source files: src/rtc.c, tests/run_rtc_tests.py, docs/RTC_SUPPORT.md.
- References: branch `feature/rtc-support`, [commit](https://github.com/InternetNinjacy/pokemon-lightning-yellow/commit/ce126c5c7c7f9bf3a052f58dc536a2a32e2580bd), [PR#3](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/3).
- Implementation status: implemented on unmerged dependency stack.
- Automated / emulator evidence: Actual stable mGBA GPIO exercise confirmed correction; companion ancestry commit296b78e4 contains equivalent correction.
- Save compatibility, dependencies and unresolved issues: Count as one behavior repair, not two features. Physical timing remains unverified.
- Supersession: only explicit changes above supersede earlier behavior; no whole-feature replacement implied.

### 2026-10-26 — INT-002 — 426415d8 (historical backfill)

- Previous → new: Host-only evolution validation → compiled-ROM headless mGBA scenes, clock, encrypted party/bag and flash save/load tests in Actions.
- Design authority/version: Owner emulator setup/testing request.
- Source files: tests/emulator/run.py, tests/emulator/runner.c, .github/workflows/build.yml, docs/EMULATOR_VALIDATION.md.
- References: branch `feature/evolution-system-overhaul`, [commit](https://github.com/InternetNinjacy/pokemon-lightning-yellow/commit/426415d853e141960a5ecfc60eb8798fdd3cb2dd), [PR#4](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/4).
- Implementation status: implemented on unmerged dependency stack.
- Automated / emulator evidence: Real scene/normal button loop execution documented; not full playthrough/link hardware.
- Save compatibility, dependencies and unresolved issues: Harness guard RAM/ABI assertions retained. No copyrighted BIOS or ROM artifacts uploaded.
- Supersession: only explicit changes above supersede earlier behavior; no whole-feature replacement implied.

### 2026-10-26 — PKM-002 — 851c76c9 (historical backfill)

- Previous → new: Older evolution table → approved baby/Feebas levels, Tyrogue>=15 choice menu, six direct item pairs and Clamperl rollback.
- Design authority/version: Latest owner supplied Evolution Mechanics Implementation specification, DES-003.
- Source files: src/data/pokemon/evolution.h, src/evolution_scene.c, src/pokemon.c, tests/run_evolution_tests.py, tests/emulator/runner.c, docs/EVOLUTION_MECHANICS.md.
- References: branch `feature/evolution-mechanics`, [commit](https://github.com/InternetNinjacy/pokemon-lightning-yellow/commit/851c76c94ac5bbcfda5a48a34bdb9a9521ab202f), [PR#5](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/5).
- Implementation status: implemented on unmerged dependency stack.
- Automated / emulator evidence: Host tests plus actual scene/menu/stat-independent choices and flash reload. National gates/time correction still pending.
- Save compatibility, dependencies and unresolved issues: Save layout and IDs preserved. Supersedes PR#4 only for specifically changed methods.
- Supersession: only explicit changes above supersede earlier behavior; no whole-feature replacement implied.

### 2026-10-29 — PKM-001 — 13c0ca04 (historical backfill)

- Previous → new: National/Kanto views → approved custom252 ordering/counts/completion using canonical species/seen/caught IDs, Castform#233.
- Design authority/version: Supplied historical roster fixture plus owner252 expansion DES-002.
- Source files: src/custom_pokedex.c, include/custom_pokedex.h, src/data/pokemon/custom_pokedex.h, src/pokedex.c, src/pokedex_screen.c, src/pokemon.c, tests/run_pokedex_tests.py, tests/specifications/approved_251_roster.csv, docs/CUSTOM_POKEDEX.md.
- References: branch `feature/custom-252-pokedex`, [commit](https://github.com/InternetNinjacy/pokemon-lightning-yellow/commit/13c0ca04dd3505533a066a1f23ee0fefb960dc16), [PR#6](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/6).
- Implementation status: implemented on unmerged dependency stack.
- Automated / emulator evidence: Host mapping/list/flag tests, real pre-National Dex UI/Forecast/save tests; prior CI green.
- Save compatibility, dependencies and unresolved issues: No species removed from internal engine or save fields. Twelve custom types not part of this commit; excluded roster handling documented.
- Supersession: only explicit changes above supersede earlier behavior; no whole-feature replacement implied.

### 2026-10-29 — BTL-002 — bbb0459d (historical backfill)

- Previous → new: Ordinary wild singles → configurable experimental land/cave doubles, independent second draw and5% Onix+Cloyster pair; survivor-based capture.
- Design authority/version: Owner Wild Double Battle Capability Test request.
- Source files: src/wild_encounter.c, src/battle_setup.c, src/battle_script_commands.c, src/battle_util.c, tests/run_wild_double_tests.py, tests/emulator/wild_double.c, docs/WILD_DOUBLE_BATTLES.md (complete file list in commit).
- References: branch `feature/wild-double-battle-capability`, [commit](https://github.com/InternetNinjacy/pokemon-lightning-yellow/commit/bbb0459d95ed3d9b053304290961f4fff2de23eb), [PR#7](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/7).
- Implementation status: implemented on unmerged dependency stack.
- Automated / emulator evidence: Host statistical/production checks and actual walking/battle/capture/run/save mGBA scenarios; prior CI green. Conditional acceptance.
- Save compatibility, dependencies and unresolved issues: Route1/MtMoon1F are test locations; not final encounter placements. All balls/replacements/forced ending/complete weather/link coverage outstanding.
- Supersession: only explicit changes above supersede earlier behavior; no whole-feature replacement implied.

### 2026-11-02 — INT-003 — canonical coordination established

- Previous: reports were scattered across external session artifacts; no repository dashboard/ledger.
- New: permanent task registry, effort-weighted dashboard and append-only ledger on independent docs branch. Drive synchronization remains pending.
- Design authority: latest owner consolidated dashboard/ledger request, followed by “Implement the proposed plan.”
- Files: docs/PROJECT_DASHBOARD.md, docs/MASTER_IMPLEMENTATION_CHANGE_LEDGER.md, docs/project-tracking.json.
- References: docs/project-coordination; commit and PR resolved by branch history and next milestone entry.
- Status: documentation prepared; no gameplay changed in this entry.
- Tests: JSON parsing, stable task-ID uniqueness, effort totals and git diff validation; no game/emulator tests applicable to this docs-only change.
- Dependencies/risks: direct Drive access and missing specification exports remain external blockers. Documentation progress is not gameplay completion. No superseded historical entries removed.

## Resume requirements

Append meaningful changes, failed validations and fixes, actual test outputs, exact commit/PR/CI links and remaining limitations. Update Current Effective Version and task status together. Never equate green compilation with acceptance. Keep each system's design, implemented code, automated results, emulator evidence, review and merge state separate.

### 2026-11-02 — PKM-003 / PKM-004 / TIM-002 — stabilization implementation

- Previous → new: twelve original type pairs become the exact approved custom pairs; pre-National approved roster evolutions no longer auto-cancel; timed friendship changes from noon split to DAY[06:00,18:00).
- Design authority: DES-001/002 and owner-approved consolidated execution plan. No unseen Drive version claimed.
- Files: src/data/pokemon/species_info.h; src/custom_pokedex.c/include/custom_pokedex.h; src/pokemon.c; src/evolution_scene.c; src/party_menu.c; new include/constants/time_of_day.h, include/time_of_day.h and src/time_of_day.c; corresponding host fixtures/tests.
- Branch: feature/phase-one-stabilization, stacked on bbb0459d. Commits: 95fd5870 (types), 672c5376 (shared gate), add8c5a1 (time). [PR #9](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/9), draft while final verification proceeds.
- Automated: all five host suites pass (split1299 +6 static audits; RTC3011; evolution755 +static audits; Dex6300 +preservation; wild1246873). Build passes; focused compiled-ROM battle suite147 checks passes. Full final run/clean repeatability/Actions still pending at this milestone.
- Emulator: actual pre-National item scenes success/cancel, excluded-target guard and Tyrogue/level scenes pass; exact RTC seconds pass. No physical RTC/link claim.
- Save/dependency: unchanged layouts, IDs, canonical flags and parent PRs. Excluded targets retain gates; no National story flag enabled; no Clamperl Bag route added.
- Supersedes: old effective type pairs, retained gates for roster targets and noon policy only. Historical PR reports remain valid for their original commits.

### 2026-11-02 — BTL-003 / PKM-005 — stronger compiled-ROM acceptance

- Previous → new: host-only split numeric evidence supplemented with actual ARM damage/AI/item/screen/burn fixtures, twelve real party-to-battle typing loads and actual move-selection text checks.
- Commit: 69522141; branch/PR #9 above. Files: tests/emulator/battle_categories.c, tests/emulator/run.py, tests/emulator/runner.c, docs/PHASE_ONE_STABILIZATION.md, README.md, AGENTS.md.
- Tests: focused147 checks passed, local screenshot inspected for type/category/PP legibility. Summary runtime acceptance remains outstanding at this checkpoint.
- Failed trial: initial text assertion expected PSYCHIC; source and runtime use unchanged FireRed abbreviation PSYCHC. Corrected test expectation, reran successfully; no game text changed. No failed run counted as passing.
- Compatibility: isolated controlled battle fixtures do not certify every AI strategy, complete gameplay or links. No new gameplay balance/effect in this acceptance commit.

### 2026-11-02 — INT-003 — coordination publication checkpoint

[PR #8](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/8) opened from docs/project-coordination, initial commit626c6ad2. [Actions41375601930](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41375601930) completed SUCCESS. Both docs and gameplay PRs remain unmerged. Drive sync remains pending. Permanent task progress updated by actual scoped work, not by design approval.

### 2026-11-02 — BTL-003 / BTL-004 / PKM-005 — runtime UI repair and acceptance expansion

- Previous → new: full category text overlapped FireRed's EFFECT heading at x=7; it now occupies x=57 beside the heading. A runtime screen inspection exposed the issue; it was not detected by the earlier width-only audit.
- Design authority: approved split readable Physical/Special/Status summary indicators; the owner-approved stabilization plan authorizes demonstrated defect fixes. No new artwork or move balance.
- Source files: src/pokemon_summary_screen.c; tests/audit_split.py; tests/emulator/battle_categories.c; tests/emulator/run.py; tests/emulator/runner.c; tests/emulator/wild_double.c; docs/PHASE_ONE_STABILIZATION.md.
- Branch/commit/PR: feature/phase-one-stabilization, `6ff574dc205a007a93a4724fd6eda867eeff8fdf`, [PR#9](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/9), now ready for review and unmerged.
- Automated/emulator: actual summary list/detail navigation selects physical/special/status moves; local final captures of all three inspected and legible. Eleven ordinary Bag ball types reject two live foes without consumption; partner B cancellation refunds exactly once and a retry catches/consumes once; legitimate failed Poke Ball throw consumes once, retains survivor, does not set caught flag and resumes battle. Safari Ball is explicitly outside the ordinary Bag fixture.
- Additional evolution acceptance: Crobat/Blissey and earlier item/level/Tyrogue scenes run before National Dex; assert the National flag stays disabled and custom caught count/flags survive real flash reload.
- Test-development corrections: a host evolution fixture initially retained its egg flag and a new static audit initially missed an import. Corrected fixtures/import and reran; these were not game bugs and no failed run is counted as passing.
- Save/dependency effects: no layout, species/move ID, trainer, encounter-policy or item-mechanics change. Parent #2–#7 remain unchanged. New typing behavior was separately approved, not silently imported from another project.
- Unverified: full overworld Bag navigation, actual two-peer link/trade scene handshake, physical RTC/other cores/full progression, complete AI strategy, nickname acceptance/rename, successful catches with every ball, faint/replacement/whiteout, forced-end/spread moves and unfinished weather. Wild-double release verdict remains CONDITIONAL. No known failed scenario is waived.
- Supersedes only the overlapping category placement; other split behavior preserved.

### 2026-11-02 — INT-004 — final combined stabilization verification

- Branch/head: feature/phase-one-stabilization `6ff574dc205a007a93a4724fd6eda867eeff8fdf`; #9 stacked on #7. No merge/auto-merge/release.
- All five local host suites PASS: split1,299 +6 separate static audits; RTC3,011; evolution755 +separate data/wiring audits; Dex6,300 +strict type/other-data preservation; wild1,246,873 (including200,000 draws). Static evidence is not represented as emulator execution.
- Complete local mGBA0.10.5 PASS: **8,715 assertions**, including API/guard safety checks, not that many whole battles. Actual ROM boot, scenes, controlled ARM battle/AI routines, summary and move-selection UI, wild battles, saves and clock are exercised by the default suite. Focused diagnostics do not substitute for this complete run.
- Build and clean rebuild PASS; SHA-256 `d17d2e6206496b9e48fe578872f91df9658d31a61b3ec8ccf33b0adcc5b9e069` identical. Linker EWRAM261,052/262,144; IWRAM29,824/32,768; ROM15,407,440/33,554,432 bytes. No runtime RAM increase from these helpers; budget remains tight.
- [GitHub Actions41376878452](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41376878452) **SUCCESS** for exact head above. API confirms modified host tests, clean reproducibility and mGBA steps SUCCESS; separate upstream-baseline original SHA-1 twice-from-clean step SUCCESS. Assertion counts above are local execution evidence; API step conclusions are CI evidence. Inaccessible Actions log downloads are not claimed as read.
- Current main remains `e04252ea0783510000e634486c0f2526a736f506`. Working tree diff checks and no tracked ROM/save/archive checks pass. ROMs, proprietary BIOS, save snapshots and screenshots were not uploaded/committed.
- Remaining integrated feature tests wait for absent Route5 register, final move/weather/lighting specifications and later implementation. Scoped current-stack acceptance does not mean Phase One or the whole game is complete.
- Rollback: before merge leave/close #9; after separate integration approval revert only this five-commit series in a reviewed PR, preserving parents. Saved completed evolutions/caught flags are not undone by code rollback.

### 2026-11-02 — INT-003 — final session checkpoint

The canonical dashboard, JSON registry and this ledger now record final #9 evidence and remaining gaps. Provisional tracked effort is123 earned /341 remaining (26.51%); the same464-point denominator is retained. INT-004 credits combined build/repeatability/stack checks, while per-feature test work stays with its task. Existing completed work is not reset or counted again. Drive sync remains pending; no Google Drive tool is exposed and network configuration alone is not an authenticated connector. Next required design input is the full Route5 register export. Other missing specs remain independent blockers. All PRs remain unmerged.

### 2026-11-02 — DES-005 — Route5 handoff supplied; former blocker resolved

Owner supplied the complete Route5 Daycare Breeding handoff, consolidated November2,2026. This is approved design and explicit implementation authorization, not runtime evidence. DAY-001..004 retain their permanent IDs and now have complete supplied requirements. Linked Drive originals remain unavailable; their contents are not claimed as read. Added WRL-005 for separately approved local Ditto existence, blocked on unapproved rate/levels. No new encounter values chosen. Exact coordinates/dialogue remain reviewable technical implementation choices using the approved reliable/default-or-both-inside criterion.

### 2026-11-02 — DAY-001 / DAY-002 / DAY-003 — implementation checkpoint

- Previous → new: Route5 single deposit/no breeding → independent two-parent facility with original GenIII compatibility/inheritance/XP/fees, persistent genuine50% Shiny eggs and two existing sprites inside the existing room. No time/encounter/battle balance changes.
- Base audited: feature/phase-one-stabilization `6ff574dc205a007a93a4724fd6eda867eeff8fdf`. Recoverable ref recovery/pre-route5-daycare-2026-11-02. Branch feature/route5-daycare-breeding; [PR#10](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/10), draft pending final validation.
- Commits: f4deb0df376c9d11186dce0bbf88f8510cdd9fa9 (storage/context/identity/scripts/tests);5fae1a90741d8ee79a591b4ba7d42b4b4920ead4 (expanded runtime/UI evidence and real rev0 formatter fix);a1dbe8a323a180f4365cb80c6c149663ef7b8e8d (fresh-checkout host-test setup).
- Authority: DES-005 owner complete handoff. No generic coding reapproval needed; merges/releases remain separate.
- Material files: include/global.h, include/route5_daycare.h, src/route5_daycare.c, src/daycare.c, data/scripts/day_care.inc, data/maps/Route5_PokemonDayCare/map.json, data/specials.inc; workflow; host/ABI/emulator tests and preservation fixtures; docs/ROUTE5_DAYCARE.md, README.md, AGENTS.md.
- Storage: exact400 bytes at0x348C, DaycareMon0x8C, DayCare0x11C, SaveBlock1 unchanged0x3D68. FourIsland0x2F80, legacy0x3C98 and following ramScript0x361C unchanged. Native target assertions pass. Original full legacy record copied byte-for-byte into slot0; archived record stays untouched and never re-imported after migration. Unknown/corrupt/versioned occupied storage fails closed, never silently resets or redirects into FourIsland. Existing sector checksum/save layout unchanged.
- Identity: one fresh RNG coin at generation; save full PID/OT/outcome before pickup. Force both actual XOR outcomes; preserve nature, gender/ability low bits and Nidoran/Illumise branch. Mark only Route5 eggs using encrypted/checksummed unused ribbon nibble A/B. Matching-build hatch preserves intention across receiving OT reassignment; marker clears after hatch. Untagged eggs and global Shiny threshold remain unchanged. Native ownership-data fixture is not a link handshake.
- NPCs: existing deposit object becomes OLD_WOMAN at4,4; GENTLEMAN egg service at2,4, both stationary inside on existing floor tiles. Real walking proves both reachable. Existing FourIsland text reused; its furniture-specific movement omitted on Route5. Route5 refusal keeps the same pending Egg; FourIsland rejection behavior unchanged.
- Tests: six host suites pass; new460161 policy assertions and separate preservation audits. Focused core103114 and native UI1359 assertions pass; actual10000 Route5 eggs5066 Shiny/4934 normal; actual10000 FourIsland controls1 Shiny/no marker. Host independent varied seeds also5066; continuous unreseeded control4998. Mail/1234-step migration, both pending/flash, compatibility/offspring/incense/inherited moves+IVs, ownership/hatch/flash, real menu/cancellation/both sexes/last healthy/full party/payment1800 pass. Clean rebuild identical SHA25641bf9004f6500692fd69ab93fa62640777bcb874eb7045e48ebcacde76246ca3. Final combined suite/Actions pending at checkpoint.
- Real defect: rev0 DaycarePrintMonLvl used C strcpy on EOS0xFF text; level menu stalled/corrupted. Replace with StringCopy as later upstream revisions do. Shared formatter fix also necessarily benefits FourIsland; breeding/state rules and FourIsland map/scripts remain preserved. Runtime failing withdrawal case now passes.
- Failed development trials: host pointer widths required separate target ABI proof; linearly reseeded RNG fixture introduced artificial serial correlation and was replaced with standard mixed varied seeds plus an independent unreseeded stream; emulator save pointers must follow game relocation; installed mGBA flags must match optional vtable ABI; flash fixtures explicitly verified blank; live Pokemon crypto must not be reentered while inspecting animation; field text/turn timing and encrypted game-stat initialization corrected. These fixture fixes are separate from the real formatter repair; failed trials are not passes.
- CI issue: initial modified run failed at host test because Make had not generated map_groups.h. a1dbe8a3 generates temporary constants from authoritative map JSON before compiling its host fixture. Final run pending; failure not hidden.
- Rollback/save risk: old ROMs do not understand migrated deposits or egg markers. Code revert is not safe reverse migration; stale archived legacy data can duplicate/resurrect an old deposit and hide new deposits. Retain pre-upgrade backups, return/recover deposits on the new build or use a reviewed reverse migration. Never erase unknown data. No ROM/save/BIOS/screenshot upload.
- Remaining: final combined/CI checks, real peer link/hardware/power interruption, full quest-log replay/progression, review of technical coordinates/dialogue, Ditto rate/levels. No production-complete claim or merge authorization.

### 2026-11-02 — DAY-001 / DAY-003 — legacy Continue safety and stronger flash proof

- Authority: DES-005, owner complete November 2 daycare handoff; preserving old deposited Pokémon and existing-save functionality requires this technical migration, with no new gameplay design.
- Previous → new: a save made inside the old room restored its one saved NPC template, so the new egg caretaker could remain missing until re-entry; Continue now detects the legacy Route5 template layout, performs the normal full room load and safely moves a player standing on the new NPC tile one tile south. No other room is refreshed; original explicit continue-warps retain priority. Upgraded Route5 and ordinary FourIsland Continue paths pass unchanged.
- Commits: f6b5a10acc4458113296e4b98226c8898f38922e strengthens flash evidence by clearing both save blocks plus global party/count before reload; 596265d41f92fd3e42ee6b0a08d2cb8e0ed62e5d corrects its host constant and implements the Continue fix/tests.
- Material files added to the previous change set: src/overworld.c; updated include/route5_daycare.h, src/route5_daycare.c, tests/emulator/daycare.c, tests/emulator/run.py, docs/ROUTE5_DAYCARE.md. Full modified-file inventory is in PR#10.
- Focused actual Continue fixture PASS199 assertions: legacy saved template/active actor and player overlap, old deposit456 steps, real flash load and CB2_ContinueSavedGame, both NPCs respawn, deposit preserved; subsequent upgraded Continue and FourIsland Continue pass. Final full run below repeats these cases, not merely the focused shortcut.
- Failed trial disclosed: f6b5a10a's stronger host C helper referenced undefined PARTY_SIZE. Actions41449787513/41449767556 failed emulator compilation. Corrected only the host fixture constant in596265d4; production assertions/checks were not disabled. Prior a1dbe8a3 had full-suite114120 and Actions41447962744 SUCCESS, but neither that earlier green head nor focused passes substitute for final-head verification.

### 2026-11-02 — DAY-004 / INT-004 — final combined Route5 verification

- Final branch/head: feature/route5-daycare-breeding `596265d41f92fd3e42ee6b0a08d2cb8e0ed62e5d`; [PR#10](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/10) open and ready for review, targeting feature/phase-one-stabilization at6ff574dc205a007a93a4724fd6eda867eeff8fdf. Main remains e04252ea0783510000e634486c0f2526a736f506. Parent PRs #2–#7 and #9 are unchanged; no merges, auto-merge or release.
- Build/clean rebuild PASS: make -j2; sha256sum capture; make clean; make -j2; sha256sum -c. Identical measured SHA-256 `0d719dc65d697e9df0ca82596c14a829fa4d179f95be1dad7cfbc09265e8dcfe`. This is repeatability evidence, not a fabricated replacement expected checksum. Linker EWRAM261052/262144, IWRAM29824/32768 unchanged; ROM15408256/33554432. RAM remains tight. Target ABI assertions preserve400-byte use and all established save offsets/sizes; original checksum/sector code remains unchanged.
- Six host commands PASS: python3 tests/run_split_tests.py (1299 plus6 audits); tests/run_rtc_tests.py (3011); tests/run_evolution_tests.py (755 plus preservation/routing audits); tests/run_pokedex_tests.py (6300 plus preservation); tests/run_wild_double_tests.py (1246873); tests/run_daycare_tests.py (460161 plus static preservation). Execute each tests path with python3. Host/stubbed/static evidence is separate from ROM runtime evidence.
- Full default `python3 tests/emulator/run.py` on compiled ROM/ELF and libmGBA0.10.5 **PASS114313 assertions**, including API/register/fixture safety assertions rather than that many whole gameplay scenarios. Includes prior split/RTC/evolution/Dex/wild battles; actual legacy/current/FourIsland Continue, service menus/walking/deposits/refusals/experience/payment1800, independent parents/pending/flash, old full deposit/mail/1234-step migration, compatibility/offspring/incense/IV+move inheritance, one hatch clock, full-party/refusal identity retention, actual744-frame Shiny hatch and return to field, receiving-OT trait preservation and pending/party/hatched flash states. The separate global party/count and both save blocks are cleared before reload; no leftover RAM is used as proof of persistence.
- Actual ARM-generated Route5 statistical log: `PASS: 10000 FULL actual ARM-generated Route5 Eggs: 5066 Shiny/4934 non-Shiny, IsMonShiny on encrypted stored data; seed=mix32(0x2A551701+i); transitions=2474/2460/2459/2606`. Every egg goes through native pickup/data creation. Exact mix transform xor16/mul7FEB352D/xor15/mul846CA68B/xor16 is checked in. Observed50.66% lies within4700–5300 tolerance; one fresh coin, both true XOR outcomes, pending no-reroll and reduced128-block nature reachability are independently deterministic-checked.
- Actual FourIsland control log: `PASS: control10000 actual Four Island Eggs: 1 Shiny, no Route5 marker/transformation (normal odds, not a precise rate proof)`. Host additional unreseeded10000 stream from0x51E832A9:4998 Shiny; transitions2491/2511/2510/2487. No wild/global Shiny odds or unrelated egg policy changed. Full baseline compatibility includes isolated excluded-species fixtures, not additions to the252 roster.
- [Actions41451641953](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41451641953) **SUCCESS**, exact final head: modified job host/clean reproducibility/mGBA steps SUCCESS; upstream-baseline job original SHA-1 twice from clean SUCCESS. [Actions41451631233](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41451631233) also SUCCESS. Independent pinned upstream SHA-1 remains41cb23d8dccc8ebd7c649cd8fbb58eeace6e2fdc. CI conclusions are obtained through the GitHub API; inaccessible detailed log downloads were not read. Assertion counts/statistical output are local executed logs retained here and in tests; no false CI-log attribution.
- Working-tree/diff checks and no new tracked ROM/save/archive/screenshot checks PASS. No proprietary BIOS/reference ROM was downloaded and no binary artifact was uploaded. The gh pr edit command encountered a deprecated Projects(classic) query; the identical prepared PR body was successfully saved through the supported REST PATCH endpoint. This was a client command failure, not a code/test failure.
- Unverified: actual two-peer link handshake, foreign-ROM marker semantics, hardware/physical power interruption, other emulator cores, full quest-log playback and progression. Receiving-OT fixture+hatch is not link traffic. No old-ROM reverse migration. NPC coordinates/reused dialogue are technical choices presented for review, not separately finalized narrative design. No production-complete claim.
- Rollback: preserve recoverable base recovery/pre-route5-daycare-2026-11-02; decline/close unmerged PR or revert only its five commits through separate review after authorized integration. Code rollback alone is unsafe for upgraded saves: old ROMs cannot see new parents/eggs/markers and stale archived legacy data may resurrect withdrawn Pokémon. Retain a private pre-upgrade save backup, recover parents with the new build or use a separately reviewed reverse migration. Never erase unknown storage.
- Supersedes the prior checkpoint's pending combined/CI status and stale hash, not its historical evidence. All approved breeding behavior, independent FourIsland storage, existing roster/types and parent features remain intact. WRL-005 stays blocked on owner-approved Ditto rate/levels; no values invented.

### 2026-11-02 — INT-003 — durable final Route5 checkpoint

Updated Current Effective Version, all four permanent DAY tasks, combined integration evidence and the feature dashboard. Repaired malformed dashboard table rows by rendering the task registry without changing other tasks' status/effort. DAY stays31 earned/1 remaining (97%/3%); overall154 earned/312 remaining, total466 (33.05%/66.95%). No duplicate credit for repeated verification; WRL-005's two-point scope expansion remains explicit. All33 task IDs, aliases, histories and outstanding unrelated blockers survive. Google Drive sync is still pending; no Drive document was created. PR#10 is ready for review, #8 holds this documentation, and no merge/auto-merge/release is authorized.

### 2026-11-02 — DES-006 / INT-003 — new supplied update and missing package

Owner supplied the complete Wild Encounters, Gym Roster, Fossils and Pokedex Compliance update and subsequently instructed Continue. It authorizes fully specified implementation without routine reapproval, with no merge/auto-merge/release. The queued Codex Start Prompt was read completely; its named ZIP was not uploaded. Seven linked Drive exports returned401 even with unrestricted HTTP and no exposed Drive reader/writer. No source documents/package mechanics are claimed read. Poké Ball implementation stays blocked on the actual governance/16 and sources/15 files, not a request for generic coding approval.

### 2026-11-02 — WRL-006 / WRL-007 — approved fossil implementation

- Previous → new: Route10 aide's one Everstone at20 caught → one Claw Fossil at the same threshold/location and reward bit0x2FA. New dialogue names Claw; other aides unchanged. Historical claimed rewards stay claimed; no retroactive second reward, Everstone replacement or Rock Tunnel dig site. Old-save Anorith availability after a previously claimed Everstone is not solved by inventing another location.
- Previous → new: MtMoon choice excluded the other fossil → original saved choice remains immutable and existing experimental-room scientist offers the opposite fossil once, only after a valid completed choice. Refusal/full bag retains availability; gift flag publishes only after success. No new NPC/map asset, original lab services preserved and pending revival takes priority.
- Previous → new: old lab assumed one MtMoon fossil and stopped after it+Amber → four-option existing-style menu with possession/completion checks, both fossils and Claw→Anorith through original level5/pending/entrance/party/PC/nickname flow. New event flags0x2E5/0x2E6 verified unused; menu65 appended. Save/item/species IDs/layout remain unchanged. Original service blocks fingerprinted.
- Branch/commit: feature/fossil-rewards-and-roster-audit `b0bd3b86f4e03dd3f540f0776a5de87f207e84e4`, [PR#11](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/11), stacked on unchanged PR#10 at596265d41f92fd3e42ee6b0a08d2cb8e0ed62e5d. Main remains e04252ea0783510000e634486c0f2526a736f506; parent PRs unchanged and unmerged.
- Files: data/maps/Route10_PokemonCenter_1F/scripts.inc,text.inc; data/maps/CinnabarIsland_PokemonLab_ExperimentRoom/scripts.inc,text.inc; include/constants/flags.h,menu.h; src/script_menu.c; scripts/audit_species_assignments.py; tests/emulator/fossils.c,run.py,runner.c; tests/run_world_reward_tests.py; tests/specifications/world_reward_preserved.json,unresolved_species_assignments.json; workflow, README, AGENTS and docs/FOSSIL_REWARDS_AND_SPECIES_AUDIT.md. No map objects, trainer parties, wild data, daycare or global typing modified.
- Actual native scenarios PASS:19/20 caught/full bag/one-time and historical reward bit; both opposite gifts/no choice/both-choice corruption/refusal/full bag/retry/save; all FOUR fossil revivals in ONE SAVE including cancellation, exact-once consumption, pending/ready flash, actual original entrance transition, level5 correct species, custom membership/seen+caught registration and repeat rejection. Native script entry points are invoked on actual maps with normal messages/menus/buttons; not a claimed manual full playthrough or MtMoon fossil-choice walk.
- New production defect discovered and fixed: full-bag scientist used common aide text containing STR_VAR_2 without initializing it, causing unbounded text expansion and a native crash. Buffer the gift fossil name before that message. Same native failing case now passes. This was a new implementation error, not an upstream defect. Separate fixture corrections target actual key-item pocket, both seen/caught anticheat copies, Thumb task pointers and final text native waits; longer existing aide dialogue needs a longer harness completion budget. No inventory/save/assertion requirement waived. Focused pre-cleanup run395 assertions is historical; final complete result below is authoritative.
- Seven host suites PASS: split1299+6 audits, RTC3011, evolution755+audits, Dex6300+preservation, wild1246873, daycare460161+preservation, world static19-file/13-block preservation plus flag/roster/no-new-conflict checks. Static checks are not runtime tests.
- Final full default python3 tests/emulator/run.py on libmGBA0.10.5: **114706 assertions PASS** including safety checks. Prior daycare10000 actual eggs5066/4934 and ordinary FourIsland control1/10000 are retained. No actual link/hardware/full-PC failure/nickname acceptance/full quest-log or full progression claim; original unmodified delivery blocks are source preservation, not those unexecuted scenarios.
- Build and clean rebuild PASS: make -j2; hash; make clean; make -j2; sha256sum -c. Identical measured SHA256 `80f96e1945b11519841ed1bc869dcc0aad13614e11f043d2dab18f880529c9ad`; no replacement expected ROM hash. [Actions41529505085](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41529505085) SUCCESS: modified hosts/clean/mGBA and separate pinned upstream checksum twice from clean. Local assertion counts are execution logs; CI job/step conclusions are API evidence.
- Rollback: decline/close unmerged PR or review-revert only this commit, preserving parents. Code revert does not undo collected items, restored Pokemon or event flags. Resolve Claw restorations on the new build before downgrade; old builds do not understand fossil kind4. Keep private pre-upgrade backups and original daycare rollback precautions. No ROM/save/BIOS/screenshot upload or release.

### 2026-11-02 — PKM-006 / GYM-004 / WRL-005 — explicit remaining conflicts

- Reproducible strict species audit FAIL remains recorded:4913 assignments/review candidates;281 authored off-Dex occurrences (199 wild,24 ordinary trainer,54 Trainer Tower,4 direct events). Additional279 legacy battle-tower records and12 variable/C references require role review, not inferred live encounters.572 findings across93 distinct excluded names overall; not572 gameplay failures. Snapshot is UNRESOLVED inventory, not approved exceptions. CI --check passes only no-new-findings; strict noncompliance is not hidden or waived. Full context/path/line inventory is checked in. Internal canonical data remains available without adding253rd roster species.
- Direct conflicts include Togepi Water Labyrinth egg and Deoxys/Lugia/Ho-Oh events; approved locked stationary spec is unavailable. Ordinary team conflicts include Marill, Hoppip/Skiploom, Smeargle, Qwilfish, Chinchou/Lanturn, Girafarig and Snubbull. Do not invent replacements, delete locked events or silently apply draft ecology. Coordination of replacements remains in encounter/trainer tasks; audit closure earns no duplicate implementation credit.
- Blaine approved design: Donphan/Gyarados opening, Sandslash, Aerodactyl, Flygon ace, Double Battle. All names are in252. Actual source remains Growlithe/Ponyta/Rapidash/Arcanine, doubleBattle FALSE and single event; there is no Skarmory entry to replace. Full approved levels/moves/items/seven ordinary teams unavailable. Implemented Gyarados Water/Dragon conflicts with new Water/Flying Ground-immunity rationale; asynchronous owner question pending. No species-type or trainer configuration invented.
- Ditto Route5 existence remains approved under WRL-005, rate/levels/table pending. No draft wild tables or final double-map/pair placements implemented. Full ZIP/approved Poké Ball mechanics required next; routine implementation permission is not requested again.

### 2026-11-02 — INT-003 — queue/dashboard continuity

Preserved all33 permanent prior IDs/aliases/history and added WRL-006, WRL-007, PKM-006, GYM-004, BTL-006 (38 total). New detailed scope adds38 points,466→504; earned174/remain330 =34.52%/65.48%. Existing earned154 is retained, with16 fossil points and4 audit points added. GYM-004's3 pending points are carved from GYM-00120→17, not duplicated. Other placement estimates exclude these fossils; feature tests are not counted again under shared integration. DAY remains31/32 (97%/3%). Estimates remain low-confidence planning effort, not release certification. Canonical docs remain in PR#8; Drive sync pending. No merge/auto-merge/release.

### 2026-11-03 — DES-007 / GYM-004 — Gyarados typing confirmed

- Exact owner decision: “Gyarados is Water Dragon, confirmed.”
- Effective design: Gyarados remains Water/Dragon, matching PR#9's existing approved implementation. The Water/Flying and resulting Ground-immunity rationale in the later Blaine addendum is superseded. Five-member roster/opening-pair approval is not otherwise changed.
- No gameplay code, typing table, ability, item, move, partner, price or battle mechanic changed. No new build/emulator result is claimed for this documentation-only decision; no implementation credit added. No merge/auto-merge/release.
- Next: reconcile supplied complete Gym exports for actual finalized levels/moves/items. Any strategy revision must follow approved design; do not silently manufacture immunity.
- Authority package intake: Pokemon_Lightning_Yellow_Codex_Authority_Exports_2026-11-03.zip received; archive integrity and all40 listed source SHA256 values verified. START_HERE and conflict register read; full-domain exports not yet all read. This supersedes the missing-export access blocker, not unfinished designs or the special-overworld hold.

### 2026-11-03 — DES-008 / WRL-008 — approved Mystery Vendor Network

- Design authority: owner-supplied Mystery Vendor Codex Implementation Handoff v1.0, read in full; approved vendor pool CSV; owner explicitly approved implementing independently of unfinished starter work. The40-file November3 authority archive is received, checksums verified; it supplies future domain registers, not a claim that all have been read or implemented. Owner Gyarados Water/Dragon confirmation remains DES-007; special-overworld HOLD remains in force.
- Previous → new: ten existing optional NPC interactions (including the500 fixed Magikarp sale) → exactly one700 purchase per original seller, level5 ten-species uniform pool without replacement, independent90% real Shiny draw, stable foreign Mystery OT with ordinary traded EXP/badge obedience. Other caught/seen/evolved/released/traded acquisition does not deplete offers. Exact approved dialogue, money box, party/actual-PC nickname and correct-box messages; No/699/full storage/cancellation safeguard money and depletion.
- Recoverable base: fossil#11 `b0bd3b86f4e03dd3f540f0776a5de87f207e84e4`. Separate branch `feature/mystery-vendor-network`; [PR#12](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/12) targets feature/fossil-rewards-and-roster-audit. Main and parents unchanged. Commits a7bacdb5 (scope boundary),8601d1b0 (supplied CSV intake),b9efabb2 (core/scripts),fda17676 (UI/dispatch/regression), final `92a0600d819fe818f948e8d72bd1591531b4a07c` (map-state safety). No merge, auto-merge or release.
- Material source files: src/mystery_vendor.c, include/mystery_vendor.h, include/constants/mystery_vendor.h, include/constants/flags.h, data/scripts/mystery_vendor.inc, data/event_scripts.s, data/specials.inc; existing scripts for Route4_PokemonCenter_1F (and old sale text),CeruleanCity_House1,VermilionCity,Route10_PokemonCenter_1F,LavenderTown,CeladonCity,FuchsiaCity,SaffronCity,CinnabarIsland,OneIsland. Tests: tests/run_vendor_tests.py, tests/specifications/mystery_vendor_{sites,preserved}.json, tests/emulator/mystery_vendor{,_ui}.c plus runner/run.py; required workflow, species-audit C scan, README/AGENTS and docs/MYSTERY_VENDOR_NETWORK.md. Original NPC map.json files are unchanged; all30 changed paths are visible in the PR.
- Ownership correction: GiveMonToPlayer overwrites OT. Setting OT_ID alone does not re-encrypt secure data; first native tests caught invalid data. The delivered record is rekeyed with oldOT xor newOT before setting the final header/name, preserving decrypted data/checksum/PID. No global Pokémon/trade/EXP/obedience routines changed. Final identity tests include player named Mystery and native PC-to-party conversion.
- Dispatch correction: append new specials before gSpecialsEnd, preserving old indices. Initial direct function tests did not catch the early outside-table placement; actual NPC script execution did. Corrected without weakening assertions or ABI guards.
- Map-state correction: source review after the first full fda17676 test pass found VAR_TEMP_1 also controls Cinnabar Gym locking. Final92a0600d uses script-only VAR_0x8008 and native PC message leaves without that generic gift wrapper. Expanded native UI checks preserve the map variable on all ten maps and through Cinnabar PC naming/save/load with and without the Secret Key. Previous passing tests did not prove this missing case; it is not concealed as original coverage.
- Save compatibility: no save/Pokémon structure changes;19 previously unused flag bits0x300..312 plus original Route4 bought-Magikarp bit0x249. A legacy purchased Magikarp reserves that seller/species, without overwriting the old Pokémon, retroactive pricing/OT or another sale. Corrupt mismatched depletion counts fail closed. No filesystem save artifacts committed.
- Host commands: python3 tests/run_split_tests.py (1299 plus audits), run_rtc_tests.py (3011), run_evolution_tests.py (755 plus audits), run_pokedex_tests.py (6300 plus preservation), run_wild_double_tests.py (1246873), run_daycare_tests.py (460161 plus audits), run_world_reward_tests.py (19files/13lab blocks plus no-new-species guard), run_vendor_tests.py (655425 executed production RNG mapping assertions plus static preservation/dialogue/font/dispatch/flag/map-state checks). All PASS. Execute each tests path with python3; host/static evidence is distinct from ROM runtime.
- Clean build: make -j2; SHA capture; make clean; make -j2; sha256sum -c PASS. Measured identical ROM SHA-256 `a2bb356cfbf0e989cd550fc945af10b377033d3dba3d297dda8eb805476b935e`, not a fabricated expected gameplay checksum. EWRAM261052/262144 and IWRAM29824/32768 unchanged; ROM15409424/33554432; temporary100-byte transaction allocation, no new graphics.
- Emulator: python3 tests/emulator/run.py with packaged libmGBA0.10.5, no commercial reference ROM or supplied BIOS, **273355 assertions PASS** (includes fixture/API/register checks, not that many whole-game scenarios). Full prior split/RTC/evolution/Dex/wild/daycare/fossil scenarios rerun with vendor source. Focused final UI6478 PASS; transaction152183 PASS. Actual ten-map NPC entry scripts cover exact intro/No/699/700/repeat, party and outdoor-PC keyboard A, PC No/default naming, correct species/box transfer text, full420-slot rejection/retry, exact charge, Saffron visibility, Cinnabar map state, real flash persistence and pre-sale reload. Ten complete varied seller-order pools yield all ten once. Source fingerprints protect original other NPC/story blocks but do not prove full progression.
- Statistical native evidence:10000 actual delivered purchases;8989 Shiny/1011 non-Shiny according to native decrypted stored identity and IsMonShiny; seeds mix32(0x705A9D13+i). Species chi-square18.434, joint species/Shiny27.442, fixed thresholds40/60; Shiny interval8800..9200. Host exhausts complete16-bit accepted intervals for n1..10 and rejected tails plus0; this proves mapping rather than abstract PRNG independence. Normal wild/FourIsland/gift Shiny routines unchanged.
- Battle evidence: controlled actual Cmd_getexp values wild100/150 and trainer150/225, switched recipient and actual boosted text; native disobedience at badge thresholds10/30/50/70/all plus above-threshold and player-owned level99 controls. This is not a full victory/level-up playthrough or two-peer link test.
- Final Actions: [41673840642](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41673840642) **SUCCESS**, exact head92a0600d: modified host/clean reproducibility/full mGBA and independent upstream original SHA-1 twice from clean PASS. Earlier fda17676 runs41672984256/41673094089 were superseded/cancelled (not failures or final acceptance); earlier b9ef raw transaction green does not certify later UI. No ROM/BIOS/save artifacts uploaded.
- Remaining: explicit future starter exclusivity integration under WRL-001, manual full-world progression, hardware/two-peer link, abrupt flash power-loss, exhaustive quest-log replay and independent Shiny-summary visual approval. Unrelated281 authored off-Dex assignments remain unresolved; no-new-findings is not global compliance. Poké Ball primary register/governance intake next, on another branch. No fabricated missing-ZIP blocker now that actual exports exist.
- Rollback: revert only this topic's commits together against b0bd3b86, preserving unrelated work. Keep pre-feature save backups; consumed sale bits remain after a code revert and cannot be cleared to duplicate offers. Ordinary purchased Pokémon format needs no migration. Full progression reversal requires restoring a pre-feature save, not deleting Pokémon or resetting main.
- Progress: new permanent WRL-00820-point scope,19 earned/1 remaining starter integration acceptance; all38 prior IDs/history/174 earned preserved. Registry39 tasks, total504→524,193 earned/331 remaining =36.83%. Vendor95%/5%; WRL area35/94=37%; repeated combined tests earn no duplicate INT-004 credit. Canonical Drive synchronization still pending; no Drive write tools exposed. No production release-complete claim.

### 2026-11-03 — BTL-006 — Poké Ball readiness audit, no production changes

- Authority: all three supplied Poké Ball exports read fully; final November2 explicit owner coding authorization supersedes older design-only headings. Relevant Master/Queue amendments, canonical START HERE/intake and Snow/Hail passages reviewed; no claim every unrelated source register was read. Latest owner instruction: continue until ready to implement.
- Source audited: vendor branch92a0600d819fe818f948e8d72bd1591531b4a07c; GitHub#12 still open/ready, both exact-head Actions checks PASS, main/parents unchanged. These baseline passes do not test future ball changes.
- Deliverable: docs/POKE_BALL_IMPLEMENTATION_READINESS.md, permanent milestones BTL-006-M01..M06 within existing BTL-006 budget (not separately double-counted). Appended Snow item375 plus separate capture code13 proposed to preserve four-bit Pokémon storage, native byte controllers and old item IDs. Existing13-slot bag supports thirteen varieties; all art reuses Dive. Audited unsafe dispatch/cancellation/statistic indices, actual weather/field-Snow gap, fishing context,11 stock lists and native full-bag/Oak-gift paths.
- Recommended technical safeguards: atomic purchased quantity+Premier bonus or no charge, map-independent capture state, narrow natural Snow→Hail bridge following existing field-weather lifetime, initial Oak five-Nest pending retry without changing emergency refill. Preserve TwoIsland staged vendor and TrainerTower special stock; universal purchase bonuses still apply there. Explicit review boundaries, not invented new ball designs.
- Status: READY for core scoped implementation; no essential missing source/permission. Proposed branch feature/poke-ball-redesign based on#12, not created in this step. No production code, build, new-mechanic emulator execution or feature PR performed. No merge/auto-merge/release. Existing vendor/fossil/daycare code retained.
- Tracker: BTL-006 design/source readiness updated; existing earned0/remaining16 retained, overall193/524=36.83% unchanged. Direct Drive sync pending. Runtime proof of encoding, weather lifetime, stock boundaries, transaction/gift edge cases and full regression required before implementation completion.

### 2026-11-03 — DES-009 / BTL-006 — approved Poké Ball implementation

- **Authority supersession:** owner explicitly states Codex must no longer stop at readiness; fully implement finalized specifications, asking only on genuinely incomplete information/conflicts. This supersedes the prior readiness stopping point, not merge/auto-merge/release gates or specific special-overworld HOLDs. Final November2 ball register and express task authorization remain the gameplay authority; no new ball designs or unrelated balancing inferred.
- **Previous → new:** original Net type bonus/Dive underwater bonus/Premier1x/no purchase bonus/initial five Poké gift → approved Wet/Dust weather/origin conditions, fixed eight-species Premier3.5x, unique Snow item with shared Dive art, per-ten identical-purchase Premier rewards, eleven1200 late specialty stocks, initial five Nest with persistent full-bag retry. Standard Master/Safari/Poké/Great/Ultra/Nest/Repeat/Timer/Luxury mechanics and separate Route22 refill preserved.
- **References/recoverability:** branch feature/poke-ball-redesign based on unchanged vendor#12 `92a0600d819fe818f948e8d72bd1591531b4a07c`; commits `3512a7d32e4dba7bfecf43459a16e28bb5d3ea23` (production/tests), final `ad86d58b0c0dec7117ffa524ae4a66926b92cd88` (stronger native weather/PC tests and correct ability argument order). [PR#13](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/13) targets feature/mystery-vendor-network. Main/parents unchanged; no worktree, merge, auto-merge or release.
- **Material files/systems:** new src/capture_rules.c and include/capture_rules.h; include/constants/items.h,battle.h,flags.h; src/data/items.json,item_icon_table.h; capture dispatch/handler, player/tutorial cancellation controllers, battle setup/encounter origin, battle weather/messages, ball animations/send-out/summary decoding, shop; eleven Mart stock scripts and Oak scripts/text. Tests add tests/run_capture_tests.py, tests/emulator/capture_rules.c plus ABI/runner hooks and one diagnostic in wild_double.c. Prior evolution/Dex/daycare/vendor preservation suites use tests/approved_ball_changes.py and frozen approved context hunks while retaining their original baseline hashes. All47 changed paths visible in PR; original art assets/save structures remain unchanged.
- **Storage safety:** Snow bag item375 does not renumber any old item, Potion remains13. Captured Snow uses separately mapped four-bit code13, with explicit decoding only in ball visual contexts; native byte controllers and old codes1..12 retained. Existing13-slot ball pocket and12 visual arrays unchanged; exact Dive icon/palette/particles/animation pointers reused. Bounded legacy throw-script/statistics/refund paths avoid indexing375. New pending Oak flag0x313 was unused; no SaveBlock/Pokémon layout migration or erased deposits.
- **Weather/shop technical choices:** current battle masks at throw, nonstacking OR; field Snow now enters natural permanent Hail via existing16-bit state and native animation/message. Natural lifetime follows original field Rain/Sand/Sun conventions; move Hail still expires after five actual turns. No broader weather/lighting or modern Snow stat mechanic. Purchase+bonus simulated in one encrypted pocket then committed together; a full/999 bonus stack rejects the whole transaction, no bonus debt or lost reward. TwoIsland staged vendor/TrainerTower exceptional stock preserved, but eligible purchases there earn universal bonuses. These explicit boundaries remain visible for review, not invented new distribution design.
- **Host commands/results:** python3 tests/run_split_tests.py1299+audits; run_rtc_tests.py3011; run_evolution_tests.py755+preservation; run_pokedex_tests.py6300+preservation; run_wild_double_tests.py1246873; run_daycare_tests.py460161+preservation; run_world_reward_tests.py19files/13lab blocks/no-new-species; run_vendor_tests.py655425+static; new run_capture_tests.py**7315 executed production-helper assertions** plus item/price/eleven-stock/art audits. All nine PASS. Execute each tests path with python3. Static/host/stubbed evidence is separate from native gameplay execution.
- **Build/reproducibility:** make -j2; SHA capture; make clean; make -j2; sha256sum -c PASS. Measured identical ROM SHA-256 `28e43f8fdd0eb815413cec515d644946362d04fe60c96b729989764f3d3bdb19`; not a fabricated expected gameplay hash. EWRAM261052/262144, IWRAM29824/32768 unchanged; ROM15410000/33554432. Existing cloud toolchain/mGBA used, no local owner installation or proprietary BIOS/reference ROM required.
- **Final native integration:** python3 tests/emulator/run.py with libmGBA0.10.5 **274675 assertions PASS** (includes fixture/API/register guards, not that many whole-game scenarios). Repeats entire prior split/RTC/evolution/Dex/wild/daycare/fossil/vendor stack. New cases execute policy/weather/origin/cave/whitelist and all13 code roundtrips, actual encrypted Snow/Potion flash persistence, all purchasable types with quantities1/9/10/19/20/30/99, bonus overflow/mixed nonpooling, real Snow Bag two-foe rejection, both survivor captures/animations, partner cancellation refund, exact consumption, full-party PC capture and saved code13. Actual Mart quantity10/confirm/return gives10 Poké+1 Premier, charges2000 once and survives flash. Actual Oak NPC rejects full999 Nest stack with pending flag, persists it, retries exactly five and clears/saves flag.
- **Weather evidence distinction:** focused114 assertions PASS: actual field Snow battle entry, six full permanent-Hail turns, native Sunny Day replacement, five actual temporary-Hail turns to expiry, controlled native Drizzle/Drought/Sand Stream and Rain Dance/Sandstorm opcodes, Forecast Ice typing. These controlled calls are not every complete ability/move animation scene. Separate four-Growl/Hail stress completed in3480 frames under bounded12000-frame diagnostic; earlier general2400-frame wait timed out because the combined spread animations took longer. Splash-controlled duration tests pass independently; no gameplay workaround or disabled assertions.
- **Failures/corrections:** initial C89 declaration placement and declared-but-absent plural-name function prevented compilation; corrected. Snow capture fixture originally relied on fainting a Cloyster with native Protect; controlled Growl opponent choices isolate capture logic. Shop tests initially pressed during fade/text; corrected task readiness/timing. [Actions41682653644](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41682653644) at3512a7d3 failed modified emulator test (native ability override passed in the wrong argument); corrected in ad86d58b. It is not hidden or used as final validation. Original upstream job passed even in that earlier run.
- **Final Actions:** [41688991039](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41688991039) **SUCCESS** at exact ad86d58b: modified host/clean reproducibility/full mGBA and independent upstream-original SHA-1 twice from clean PASS. Original SHA-1 remains41cb23d8dccc8ebd7c649cd8fbb58eeace6e2fdc. No ROM/BIOS/save/build artifact uploaded.
- **Remaining acceptance:** no actual two-peer/hardware/power-interruption, manual full progression, exhaustive quest-log replay, every13-ball success/failure/presentation, whole parcel ceremony or complete field rod/Surfing UI test. Native condition matrix/source hooks do not prove all those interactions. Summary/send-out reuse is source/mapper verified, not independent visual approval of every new-code path. Future full dynamic weather/starter/Eevee/type/move/world work is separate; existing off-Dex findings remain unresolved, no new-findings guard is not global compliance.
- **Rollback:** revert only topic commits against92a0600d, keeping unrelated source and all parents. Back up/restoring pre-feature saves is required for complete reverse progression/new item semantics; do not renumber old IDs, erase Pokémon or reset main. No old-ROM/new-feature interoperability promise.
- **Progress/ledger:** permanent BTL-006 retained and retitled from queued intake to implemented ball system; original16-point budget now15 earned/1 extended acceptance. All39 permanent tasks/history retained, other effort unchanged; total524,208 earned/316 remaining =39.69%. BTL area48/136=35.29%; feature93.75%. Combined INT-004 test reruns receive no duplicate credit. Canonical docs on PR#8, Drive sync pending. No release-complete or merge claim.

### 2026-11-04 — INT-003 / PKM-007 — supplied-source reconciliation before typing edits

- Owner explicitly requests reconciling old missing-document notes, then implementing remaining approved types. Verified all40 source checksums and primary final handoffs/passage precedence; exact read scope and true gaps in docs/SPECIFICATION_RECONCILIATION_2026-11-04.md and verbatim manifest. Tracker15 workstreams now reference available exports rather than asking for the same package. Full35 effective type rule set confirmed,24 actual source mismatches. No production code changed at this checkpoint.
- Later appendices resolve weather inheritance/harbors/RTC fallback, Celadon Trainer7 Castform slot, Route5 Ditto/Skitty/Rattata roster,41/41 class pools and Giovanni cumulative4/5/6. Precise palette colors, individual battle/encounter balancing, some shop/gift details and SOW implementation HOLD remain distinct. No invented settings.
- Recorded owner-authorized PR#13 merge d54f7a8e into vendor#12 only; historical unmerged#13 notes superseded, main/other PRs unchanged. Museum#14 independent main-based draft, feature build passes/baseline fails and combined integration is unverified; only read its source diff, no change made to that PR.
- Existing39 stable tasks and208 earned points preserved; add PKM-0070 earned/6 pending for newly reconciled24-row correction/test scope.40 tasks530 points,208/530=39.25%; no completion credit from source availability or duplicated existing bridge work. Drive sync remains pending. Next: isolated typing implementation/verification/unmerged PR on d54f7a8e. No merge/auto-merge/release authority for this task.


## 2026-11-04 — PKM-007 approved typing corrections; source/tracker reconciliation

**Current Effective Version — species typing:** supplied November2 252-species
matrix PartA plus later Master Plan approvals (lines81/878/1208/1211) and the
owner's explicit Gyarados Water/Dragon confirmation.35 explicit rules are effective;
24 differed from the actual owner-merged vendor/ball source. Historical PKM-003's
twelve-rule snapshot is retained; Pinsir Bug/Steel is superseded by pure Bug.
This version is implemented on PR#16, not main. Other approved species stats,
abilities and learnsets are separately scoped and not silently implemented.

**Before → after:** Rock/Ground Geodude/Graveler/Golem/Onix/Rhyhorn/Rhydon/Larvitar/
Pupitar → pure Rock; Steel/Ground Steelix → pure Steel; Bug/Steel Pinsir → pure Bug;
pure Fire Vulpix/Ninetales → Fire/Ghost; Normal/Flying Pidgey/Pidgeotto/Pidgeot,
Spearow/Fearow, Farfetch'd, Doduo/Dodrio, Taillow/Swellow → pure Flying;
Normal/Flying Hoothoot/Noctowl → Flying/Psychic. Eleven already-correct rules,
including Gyarados Water/Dragon, remain unchanged. No elemental-chart, move,
trainer, encounter, ability, stat, learnset, evolution, roster-number or ID change.

**References/status:** `feature/approved-typing-corrections`, commits
`dfd3b2bc9eeee719b254e60f3f4afbb4b9015824` and `b1ff74d0e129815baf45633d54073f24af4087d0`;
[PR#16](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/16)
targets `feature/mystery-vendor-network` at d54f7a8e85d38b8fa304399dcb4d14778396d5cd.
Main remains e04252ea0783510000e634486c0f2526a736f506. Existing owner-approved
PR#13 merge into vendor is preserved. This task does not merge, auto-merge,
retarget or edit other feature PRs. PR#16 is ready for scoped review.

**Files/systems:** sole production file `src/data/pokemon/species_info.h` (24 type
pairs). Fixture `tests/specifications/approved_type_overrides.json` expands to35;
`historical_type_overrides_pr9.json` preserves its original snapshot;
`approved_typing_delta.json` records all24 before/after pairs;
`typing_preserved.json` freezes parent full-file/non-type digests.
`tests/approved_typing_changes.py` explicitly reverses only this approved delta
when older daycare/vendor preservation tests run; expected historical hashes stay
unchanged. Updated `run_pokedex_tests.py`, `run_daycare_tests.py`,
`run_vendor_tests.py`; new `run_typing_tests.py`; native
`tests/emulator/battle_categories.c`/`run.py`; CI, AGENTS, README and
`docs/TYPING_CORRECTIONS.md`. Canonical tracking/reconciliation lives separately
on this documentation branch with the verbatim40-file source manifest.

**Executed tests:** all10 `tests/run_*tests.py` PASS (capture, daycare, evolution,
Pokédex, RTC, split, typing, vendor, wild-double, world-reward).35 authoritative
pairs and exact24-delta whole-file reversal PASS; all other species fields are
byte-preserved. Full local mGBA0.10.5 routine/UI/scene/save regression PASS,
274765 assertions on the modified ROM; final test-harness addition separately
PASS,46 focused summary assertions. New runtime coverage: all35 real
party→battle types;17 controlled battle/AI effectiveness checks;9 STAB checks;
six actual summary screens load approved mono/dual type icon data. Existing
native evolution, RTC, wild captures, independent daycare, vendor, fossils,
Poké Ball/UI/weather and flash save/load tests remain passing. Route5 native
10000-egg sample5066 Shiny/4934 non-Shiny remains unchanged. No fabricated
probability or runtime result is substituted for a static audit.

**Build/CI:** `make -j2`, `make clean`, `make -j2`, and
`sha256sum --check /tmp/typing-first-build.sha256` PASS. Both builds produced
`35c18c2c45f305691e3523724e6a7a7bc10a43c373db0cf993b8566d58fbc0c0`;
this measured reproducibility result is not a new expected vanilla checksum.
[Final-head Actions](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41760396005) SUCCESS: modified build/host/full-ROM mGBA and unchanged
pinned-upstream clean SHA-1 comparison. Earlier intermediate-head runs were
superseded/cancelled, not represented as successes. EWRAM261052/262144 and
IWRAM29824/32768 bytes are unchanged; no new RAM/save fields or artwork.
No ROM/save/generated binary is committed or uploaded as an artifact.

**Compatibility/limits:** types are derived from species data, so saved Pokémon
need no layout migration; identity/moves and existing encrypted save structures
are preserved. Existing full flash regressions pass; this is not an old-ROM
cross-version save comparison or full-world progression certification. Real link
peers/physical cartridges and every visual state remain unverified. Matching game
data is required for link battles. Separate Sandstorm residual policy and future
stats/abilities/learnsets remain unfinished, not imported into this data-only patch.
Strict global species audit still has281 authored off-Dex findings; no claim of
global compliance. External Museum#14 and Blaine#15 are not integrated/tested in
combination here. PR15 duplicates Pidgeot's approved pair, touches shared tests/
workflow and currently fails compiling CinnabarIsland_Gym/text.inc:12 (backslash
mapping) at inspected head9b6563a2/run41760575099; older tracker claims that no
Blaine implementation exists are superseded. No changes made to that PR.

**Source/tracker reconciliation:** all40 TXT export checksums verified; no new
copy requested. Corrected stale unavailable-document authority/blocker notes;
actual unresolved palette, encounter-distribution, held-feature and integration
gaps are retained. Direct Drive synchronization still pending; availability of
snapshot specifications is not authenticated live Drive writing.

**Progress/recovery:** PKM-007 six pending points → six earned; all39 previous
IDs/208 earned retained.40 tasks,214 earned+316 remaining=530 provisional points,
40.38% overall. PKM area48 earned/4 remaining=92.31%. No duplicated integration or
external Pidgeot credit. Rollback: revert the two topic commits (or eventual merge)
while preserving the vendor/ball parent; no save conversion needs reversal.

Final-head Actions native result: **274791 mGBA assertions PASS**, including all six new summary screens in the full run. Local full274765 and focused46 above are separate executed results, not added together as a CI count.


## 2026-11-04 — DES-010 consolidation approval / WRL-004-MUSEUM and GYM-004 acceptance

The owner explicitly instructs: merge all existing project PRs and continue the
next approved unfinished feature. Repository `allow_auto_merge` was already true
and remains true. The prior statement about disabled auto-merge described the
absence of a request on PR16, not a change to the repository setting. This
consolidation authorization supersedes older no-merge notes for existing work;
it does not approve invented designs, failed checks, ROM release or destructive
history rewrites. Recovery tag `recovery/pre-consolidated-merge-20261104` retains
b1ff74d0; original feature histories are preserved.

Documentation PR8 and18 merged to main. Blaine15 had already merged into vendor;
its source now passes integration with all35 typing rules. The shared Pidgeot
change must be normalized once, not twice. Historical hash expectations remain
unchanged; frozen approved Blaine/Museum deltas protect unrelated source bytes.

Museum14 is the next unfinished feature, not a new invented gameplay system.
The original NPC23,6 was in the isolated back room; source collision analysis
and actual admission-path checks proved it inaccessible from the paid exhibit.
It now stands11,4 in the approved paid exhibit. No new artwork, ticket rule,
quiz wording/answers, prize, fee, badge check, hatch timing or other NPC changed.
All previous script-special indices are preserved; the Museum special is
appended after vendor entries and before gSpecialsEnd. The formerly ineffective
label regex is repaired. The redundant compile-only feature workflow is replaced
by the existing stronger modified/upstream matrix, including Museum static and
full ROM tests; no checksum gate is applied to modified ROMs.

Executed local acceptance: eleven host suites plus Museum static placement/state
checks PASS. Museum1307 focused mGBA assertions PASS:32 diverse OT/RNG true-Shiny
fixtures, actual50 admission, pre-badge collision-path walking/A interaction,
decline, each wrong answer/retry, correct NO/YES/NO, saved full-party pending
reward, exactly-one Egg receipt without a quiz fee, stored identity/flash,
ordinary hatch and controlled-level evolution to Shiny Milotic/flash reload.
Blaine46 focused assertions PASS: one usable Pokémon refusal, actual trainer
Double battle entry, all five species/levels, Donphan/Pidgeot leads and native
Earthquake ally immunity. This does not certify Blaine battle victory, badge/TM38
or revisit flow, full progression, physical cartridges or link peers.

Combined full-suite CI and remaining merges are pending at this checkpoint.
No additional earned points yet; current214/530 retained until combined acceptance.
Drive synchronization remains pending; supplied exports are available. Current
feature details and test commands are in docs/PEWTER_MUSEUM_TRIVIA.md.


## 2026-11-04 — DES-010 executed consolidation / current effective main

All audited open PRs were merged under the owner's explicit instruction. Docs8,
18,19,20,21 are merged; rival19 retains BLOCKED, Ranger21 supersedes the20
filename with implementation/saffron-ranger-base-camp.md (specified, not implemented).
Typing16 merged into vendor, Museum14 then merged into that stack. PRs12,11,10,
9,7,6,5,4,3 were merged in dependency order, validating every intermediate Git
tree matched the tested8daf2568 source. Final PR2 merged to main at
93b62d6837e61a483f67d00e2a055f559dc33d53; its preview tree exactly matched the
validated gameplay plus reviewed documentation. Source history is retained.
Repository allow_auto_merge remained true throughout; absent per-PR requests
were never a repository-setting change. Recovery tag
recovery/main-before-consolidation-20261104 was pushed before gameplay integration.

Current effective gameplay includes the split/RTC/evolutions/custom252/Doubles,
independent daycare, fossils, vendors, Poké Balls,35-rule species typings, Blaine
and repaired Museum reward. All11 host suites plus Museum static checks, local
and [Actions41765403411](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41765403411)
full276132 native assertions PASS. Modified clean repeatability and unchanged
pinned-upstream SHA-1 jobs both PASS. Measured combined ROM SHA256:
2c8a61840cd4a0bafc21d60d90395c1756509e494a9197a08c5e9b2840f9e9bb.
No binary artifacts committed/uploaded. Museum1307 and Blaine46 focused checks
are separate results, not added into the full assertion count. Full-world,
physical hardware/link and Blaine victory/badge/TM38/revisit remain outstanding.

Canonical tracker/ledger now live on main; the former coordination branch is
historical. Direct Drive synchronization remains pending, not specification
availability. Museum earns4 of WRL-004's existing16 points (12 other points
retained); Blaine earns2 of its3 (1 extended test point retained). No double
credit for repeated regression, Pidgeot or existing integrations. Prior214 earned
points survive, now220 before the separately new Ponyta/Ranger tasks.

## 2026-11-04 — PKM-008 Ponyta / GYM-005 Ranger intake

Approved Ponyta Lv30 was explicitly identified in supplied November2 Evolution
& Item Supply Source Parity section3 (SHA2566a39563906572b3ab6870e754473a5a4daef593c3783c92d80a0eaf02322b237).
Actual main still used40. Implemented only that threshold in
src/data/pokemon/evolution.h on feature/ponyta-level30-evolution, code commit
eb18e780. No other evolution, species stat/ability/learnset or save-layout change.
Exact before/after and base-table digest live in approved_ponyta_delta.json;
prior_ponyta_source reverses that single delta for older frozen hashes. Host
boundary/Everstone checks join run_evolution_tests.py; dedicated static exact-file
check run_ponyta_tests.py joins Actions. Native ponyta.c uses actual target lookup,
B cancellation and successful evolution/Dex/flash scenes before National Dex.
Build,12 host suites/Museum static and35 focused native assertions PASS. Full
final Actions pending before merge. Source rollback: revert threshold/test commit;
no save migration. Existing eligible Ponyta evolve on a later normal level-up,
not forcibly on save loading. Broader Ponyta balancing remains separate.

Ranger final contract21 adds distinct GYM-005 with16 provisional pending points:
four regular Singles, Park Double, three exact foreign PARK gifts and PC/map/save
acceptance. It excludes ordinary Gym/class work and other WRL-004 events; no
implementation or test credit from merging a spec. Rival remains explicitly held
by its own incomplete-design handoff. No questions are reopened from older exports.

42 permanent tasks: prior530-point denominator +2 Ponyta +16 Ranger =548.
221 earned/327 remaining =40.33% at the local-acceptance checkpoint; Ponyta retains
1 final-CI point. All previous earned work remains; denominator growth explains
the provisional percentage change. No duplicate integration credit.


## 2026-11-04 — PKM-008 final acceptance and merge / current effective source

Ponyta ordinary evolution is now Lv30 on main, implementation eb18e780,
[PR22](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/22), actual merge
7284dc0aac833f751882826e339273a97694ce9e. Every other evolution-table byte, save layout,
species stat/ability/learnset and ID remains unchanged. Existing eligible Ponyta
need a normal level-up; no load-time species conversion is added.

Executed12 host suites plus Museum static checks PASS.35 focused native
assertions cover29/30, Everstone, actual animation B cancellation, success,
pre-National Rapidash registration and flash reload. Full local native276161
assertions PASS. [Final-head Actions](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41767198787)
SUCCESS at083edbbf: modified/clean/reproducible build, all host/full native and
unchanged pinned-upstream clean SHA-1 jobs PASS. ROM SHA256
6f230066210635d5f5e3fa77d7ad8a50d1521482caeb52ad71c4eb09d6935f32
matches before/after make clean. This is a measured result, not a replacement
vanilla checksum. No ROM/save/generated artifacts uploaded or committed.

One initial local full-suite launch failed before ROM initialization because
make clean temporarily removed its ROM input; this was a test-launch race, not
a gameplay assertion failure. It was rerun only after clean rebuild completed;
the final full run above passed. GitHub's independent run was unaffected.
Redundant intermediate-merge workflow runs were cancelled only for the exact
already-validated Git trees; they are not claimed as passes. Main and final
Ponyta validation remained enabled. No assertion or protection was disabled.

Canonical live dashboard is regenerated from42 permanent task records by
scripts/render-project-dashboard.py; stale pre-merge/current-table mixtures are
removed from its live view. Historical snapshots remain in Git and this complete
append-only ledger. Existing source unavailable claims remain superseded by
supplied exports/repo handoffs. Direct Drive synchronization remains pending.
All earlier task effort remains unchanged; PKM-008 earns its final1 point:
222 earned/326 remaining of548=40.51%. Ranger's new16 pending points remain
separate from completed ordinary tasks, with no implementation claim. Rival is
still BLOCKED. Next eligible feature is GYM-005 Ranger Base Camp, not yet coded.

Remaining acceptance includes full-world progression, hardware/link peers,
Blaine victory/badge/TM38/revisit, strict world roster cleanup and future approved
weather/lighting/move-engine tasks. Repository auto-merge remains true. The
source recovery tag and ordinary revert paths preserve history; no release,
force-push or destructive save operation occurred.

## 2026-11-04 — GYM-005 Ranger Base Camp implementation

Authority: final `implementation/saffron-ranger-base-camp.md` (merged spec#21).
Baseline main700587a8; gameplay commit7b7ba6af on
`feature/saffron-ranger-base-camp`. This entry supersedes the historical
“not implemented” status for Ranger; it does not claim a main merge.

Old Dojo becomes four any-order Singles, gated Park Double, exactly one of three
foreign PARK Lv30 gifts, and a native PC. Trainer data/abilities/friendship,
scripts/dialogue/map objects, PC tile, appended IDs/flags, gift helper and legacy
Continue refresh are changed. Full details/files/rollback in
`docs/RANGER_BASE_CAMP.md`. No new artwork/save-layout fields/AI/balance system.
Old trainer IDs and legacy flags retained; no unrelated source changes hidden
by snapshot replacement. Exact reverse-hunk plus original SHA256 tests preserve
all other source bytes and work in shallow CI.

All13 host suites plus Museum static PASS. Focused actual mGBA0.10.5 data368,
UI3276 and battles458 assertions PASS. Includes nickname YES/NO, actual PC
six-to-five deposit, legacy flash Continue, actual four Singles in3/1/4/2 order,
Park loss/blackout/rechallenge and all four opponents/replacements/victory.
Unmodified opponents face controlled level100 player fixtures, not a claimed
manual balance/progression playthrough. Local PC screenshot visually inspected.
Final clean/full-native/Actions outcomes will be appended when executed.

One added nickname test initially reused a vendor helper containing an unrelated
money-window postcondition; isolated Ranger keyboard helper fixed that fixture.
No gameplay assertion was removed or disabled. Earlier battle/PC input timing
fixture corrections preserve native gameplay and guards.

GYM-005 now15earned/1remaining; total237/548=43.25%, other task credit unchanged.
Repository auto-merge remains enabled; Ranger implementation PR must stay unmerged
for explicit review. Direct Drive synchronization remains unavailable/pending.

### GYM-005 final local acceptance / Current Effective Version

PR#26: https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/26
Gameplay/test source7c9e2f5e49dc65ec4b5bb04b6464f050c568713e, following
production7b7ba6af; remaining changes are evidence/tracker documentation only.
Full local mGBA0.10.5 **280245 assertions PASS** (guarded API/register checks
included), all13 host suites plus Museum static PASS, clean build/rebuild PASS.
Measured modified ROM SHA256:
1386257b114134228fe4b7c285e8ee5f9b53e9f8932a848cc467c59af5db695f.
Native screenshots of camp/gifts/Rangers and PC were visually inspected locally.
No binary/artifact distribution. Full manual balance/progression, hardware/link,
power interruption and exhaustive quest-log remain unverified.

Required independent CI evidence is maintained at
https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/26/checks ;
local results are not presented as GitHub conclusions. Initial run41832117816
independently passed vanilla upstream SHA-1; modified emulator job was still
running when this evidence entry was written. Final live checks supersede that
transient status. No source/test changes followed the local full PASS.

Ranger implementation is ready for owner review, not merged. Keep15earned/1
remaining for review/external acceptance: overall237earned/311remaining=43.25%.
All41 other task records are byte-equivalent as parsed JSON;42 permanent IDs
retained. Direct Drive synchronization remains pending. Global auto-merge is
still enabled; this feature PR has no auto-merge request. No main write, merge,
release or destructive operation performed for Ranger.

## 2026-11-04 — tracker reconciliation / GYM-006 rival implementation

Owner said continue after remaining-work audit, under standing implementation and
prior consolidation/merge authority. Reviewed exact PR25 v2, its two-file-only
scope and passing modified/upstream checks; specification mergedc960642d.
Ranger PR26 merged887bf958 on explicit owner instruction; post-merge main run
41835690963 SUCCESS. No Ranger or Museum/Blaine merge remains pending.
Corrected live table/tracker, retaining dated history. Supplied design exports
remain available; no missing-Drive claim reinstated. Direct Drive sync pending.

New permanent GYM-006 separates eight-rival scope from GYM-002: transfer10 of its
18 pending planning points, leaving8 for other important battles. No new workload
denominator or duplicate credit; all existing IDs/history retained. Ranger earns
its final scoped review/merge point (16/0); general external release tests remain
INT-005. Rival implementation/native data earns6/10 provisionally; battle/full
regression/CI/review still pending at this checkpoint. Overall244/548=44.53%.

Branch feature/main-rival-eight-battles, recovery baselinec960642d. Only three
production data files change: src/data/trainer_parties.h, src/data/trainers.h and
src/data/pokemon/level_up_learnsets.h. Exact33 approved party members represented
under24 original starter IDs; custom held/move data; preserve original slot IVs,
AI flags, trainer bag items and every other field. Global Tackle1/Twister12/
DragonBreath22 overlays only; preserve evolved Lv1 Twister and30/55 evolutions.
Every event script/save/TM/evolution table and Champion rematch is unchanged.

All14 host suites plus Museum static PASS. Native data1525 assertions PASS;
clean build/rebuild identical SHA256. Actual battle fixtures are in progress;
no full-story/manual-difficulty or Pikachu-starter implementation claim.
An unterminated rival-name fixture was corrected, not game code. A first
controlled Champion player lost; improving player inputs/moves instead of
weakening approved opponents. Guards and victory assertions stay enabled.
Detailed validation/compatibility/rollback: docs/MAIN_RIVAL_BATTLES.md.
Separate gameplay PR forthcoming, not automatically merged.

### GYM-006 scoped acceptance and separate PR27

Core production/test commit e4068bf0a0624c6d2df37631c11cc98b0350ff42.
All eight actual battle victories/replacements PASS:52741 focused native
assertions, including native Oak tutorial; data1525 PASS, all14 host/Museum
PASS. Identical clean modified SHA256:
40a5c6606272d8791490a52de5f9182be222c40297768a00e1277b575c92364c.
Production RAM unchanged (EWRAM261052/IWRAM29824); ROM fits original limit.

Separate gameplay https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/27
created as draft while full local/CI acceptance runs. Final executed conclusions
and review readiness are recorded in that PR and its live checks, not assumed
from this checkpoint. Following commits contain only tracking/documentation;
no native game/test change is implied. Review/story integration retains1 point,
GYM-0069/1, total247earned/301remaining of548=45.07%,43 permanent IDs.
Owner has not merged this gameplay implementation. Repo auto-merge remains true,
no auto-merge request. No ROM/save/generated artifacts uploaded. Full natural
story/manual balance/loss event progression/Pikachu starter integration and
hardware/link acceptance remain explicitly separate/unverified. Existing beaten
flags are preserved; no forced story reset. Source rollback uses ordinary revert
of feature commits; preserve user saves and canonical move/species IDs.

## 2026-11-04 — WTH-001/003 in-progress checkpoint; owner switches to rival

Weather work is safely checkpointed on its separate feature branch. Builds and
10880936 host weather assertions pass; focused native83 policies/Sun/RTC phase
pass, later map fixture fieldScriptDone fails. Not a full regression/CI pass.
No new completion credit, no production-complete claim or weather PR. Full resume
notes: docs/WEATHER_WORK_CHECKPOINT.md. No source/design decisions invented for
unassigned boarding/prototype exteriors; lighting/tints/fog remain separate.
Owner explicitly requests the main rival contract; verify existing ready PR27
rather than duplicate implementation. Preserve this weather branch unchanged
while returning to feature/main-rival-eight-battles. No merge/auto-merge action.

## 2026-11-04 — WTH-001/002/003 scoped implementation and PR29

Resumed weather checkpoint. OneIsland/Celio automatic story scene was a blank-save
fixture issue; set fixture-only scene variables, no story source changes. Native
same-weather complete flag is not a graphics-ready predicate; rendering tests now
check actual current ID, graphics readiness and palette idle, with timeouts intact.
Native Sandstorm particles are visible in inspected successive captures.

Source core e04a7bc6369e47fa363631208547d6724629f9bc; branch
feature/dynamic-map-weather; PR29 separate from ready rivalPR27.83 explicit approved
maps, exact relative weights/night conversion,8-byte state within existing unused16,
outdoor and nested indoor behavior, same-map preservation, Sun/RTC fallback/recovery,
logical battle weather and Cycling end hooks implemented. Native drought's stubbed
loader bypasses to existing clear renderer; no new tint/art. Final color/lighting,
Lavender/Mansion fog, unassigned boarding/prototype settings and SOW remain separate.
A single battle weather read supplies effect and text at clock boundaries.

All14 host suites/Museum static PASS;10880936 accepted-roll assertions plus genuine
rejection/state tests. Focused native128355 with6 local captures PASS:83 native
day/night policy samples, actual map loads/interiors/flash/tail retention, GPIO
absence/recovery, native SunnyDay/field-Rain isolation, real wild Run and trainer
victory with callback instruction observation proving exactly one RNG draw and
predicted output. Initial full wild capture assertion assumed one-turn faint;
Cloyster can legitimately Protect. Bounded retries preserve its moves/stats and
all target-faint assertions; focused wild6589 PASS. No safety guard disabled.

Final modified clean rebuild SHA256 reproducible:
8e2273f6494139cca4716d9ce081e50c1592797a27dc8eb245ec8dfcb2981768.
Full local/CI in progress at this checkpoint; final live evidence/results:
https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/29/checks .
No result from an unfinished run is counted as a full PASS.

Read-only merge-tree with rival62c99419: seven conflicts in CI, test registration,
normalizer and live tracker files, none in gameplay source. This is not combined
runtime proof; preserve both registrations/ABI headers/normalizers and rerun combined
acceptance when integrating. No other PR modified, merged or auto-merge requested.
New documentation-only PR28 sign/PR30 Castform handoffs noted as available, not yet
implemented or treated as missing designs; their review/intake stays separate.

Provisional non-overlapping credit: WTH-00124/4, WTH-0021/15 for essential drought
stability only, WTH-00314/6; no duplicate credit for earlier Snow/Hail. All43 IDs and
previous earned work retained. Total286earned/262remaining of548=52.19%. Review,
full manual/integration/hardware/quest-log/power-interruption and final visuals remain.
Source rollback uses ordinary reverts; back up and save in a weatherless building
before downgrading because the original loader can hang on an outdoor Sun Continue
(source-level risk, no reverse-save runtime claim). No ROM/save/generated artifacts
uploaded/committed; Drive sync remains pending, repository auto-merge stays enabled.

### 2026-11-04 — GYM-007: three approved Gym Castform battles

Authority: PR30 head0d9cb4f5 exact three parties, November4 owner instruction to
complete existing branch/PR31, and supplied eight-Gym register's fixed weather
and ordinary no-item rules. Owner execution instruction supersedes the earlier
preparation-only status for this specific scope. Historical ce49150b staging
implemented party data only; it did not implement Doubles or weather.

Previous: Liam/Diana/Mary exact parties staged, but Diana still Single, all three
Gym headers clear, Mary legacy bag Super Potion retained. Current: Diana native
mandatory Double with two-usable-mon refusal and original event/flag flow; Liam
Single under indoor snow/permanent starting Hail; Mary Single under permanent
starting Sun with no bag item. Six exact levels/parties/move slots retained;
no general Castform learnset/Forecast, other trainer/leader/AI/IV/reward changes.

Dependency: exact weather PR29 head061f8780 integrated in cd7be97e into PR31 only;
no other PR or main modified/merged. Fixed indoor headers precede outdoor policy,
ignore outdoor nighttime Sun conversion, preserve stored outdoor context, and
refresh legacy inside-Gym clear weather on Continue without save-layout changes.
No new graphics/palettes; Sun retains PR29 native-clear renderer.

Changed: two Gym map.json weather fields, Cerulean Diana command, Diana Double and
Mary bag metadata, dynamic_weather.c fixed Gym dispatch/update, scoped tests,
CI registration, exact delta normalizer, local contract/docs/AGENTS/README/tracking.
Source manifest freezes all six approved production-file deltas against PR29;
older audit expectations remain unchanged. Existing party implementation history
ce49150b and predecessors remains recoverable.

Executed: all15 host suites/Museum PASS; focused native359 assertions PASS using
mGBA0.10.5: exact constructor data, gate with one/fainted-second, all three actual
victories/flag flash persistence, Forecast, naturally AI-selected Rain Dance,
Weather Ball actual opcode typing/multiplier, real old-clear flash/Continue and
outside Rain context isolation. Controlled player parties only; opponents unchanged.
Clean/full/CI conclusions and final commit are maintained in live PR31 checks/body,
not inferred from compiling. No manual full-playthrough, balance, hardware/link
or combined rival27 coverage claimed. Known seven rival/weather test/tracker
integration conflicts remain separate; this Gym dependency merge itself was clean.

Tracking: stable GYM-007 takes five points from GYM-001, earns four/reserves one;
no reset/double credit/denominator increase. Drive synchronization remains pending.
Rollback only the subsequent scoped Gym implementation delta to retain weather29;
backup saves first, preserve original defeat flags and indoor-save precaution before
any rollback to the old drought loader. No merge/auto-merge/release performed.

### 2026-11-05 — INT-004 / GYM-004: isolated combined acceptance PR35

Standing owner continuation/implementation/testing authorization; no new gameplay
design. Reviewed exact source heads: rival27/62c99419, weather29/061f8780 and
Castform31/02c97b7f. New feature/phase-one-integration-20261105 combines their code
only on a review branch, retaining original source branches, PRs and main intact.
Nine conflicts resolved in registrations/docs/tracking/normalization; both full
native feature sets and all16 host runners retained. Expected old hashes remain;
only frozen approved deltas normalized. Python syntax checking caught a truncated
first resolution; corrected complete runners and registration guard supersede it.
No runtime success claimed for that failed checkpoint. Recovery merge51b38c36,
correction/native test754350e8; future final source/CI evidence maintained inPR35.

GYM-004: actual fresh Blaine battle now wins against all five unmodified opponents,
then original badge/TM38/flag scripts, flash/Continue and no-duplicate revisit run.
Focused75 native assertions PASS with controlled player Mewtwo; old gate/type
fixture discarded before fresh victory. No trainer/event/save production changes.
Source files: normalizers/host assertions, CI and complete native registrations,
new Blaine acceptance/registration audit, docs and tracker. Incoming source diffs
are exactly existing rival learnsets/trainer data plus already included weather/Gym.
All16 host suites, Museum and registration audit PASS. Clean/full/CI exact final
conclusions are live inPR35; no emulator/manual/link claims inferred from source.

Current effective source combination is documented in COMBINED_INTEGRATION_20261105.
Main still has its prior accepted behavior; PR27/29/31/35 remain unmerged.
GYM-004's one scoped acceptance point earned; INT-004 two reconciliation points
(not duplicate component credit). GYM-003's obsolete missing-level/moves note is
superseded by approved PR30/implemented GYM-007; remove its two duplicate pending
points without adding earned credit or deleting the stable ID/history. Provisional
tracked workload now546 rather than548; completed work remains completed.

Broader balance/story/hardware/link/quest-log/interrupt and final visual coverage
remain unverified. New documentation PR32–34 awaits review before its separate
execution contract; no speculative League or other Gym parties implemented.
Drive synchronization still pending; rollback can abandon integration branch and
preserve all original feature heads, with existing weather save-downgrade warning.

### 2026-11-05 — BTL-004-NICK: captured survivor naming and persistence

Standing continuation/testing authorization. No new gameplay design or production
change. Test branch feature/wild-capture-nickname-acceptance, PR36, stacked on
integration35/aaa71bb2; parent and original feature PRs remain unmerged/intact.
Existing capture-name coverage declined nicknames; new test uses the real naming
keyboard to set A for both surviving Double opponents, with party and full-party
PC delivery. Verifies actual identity/species/OT, existing party names, valid
secure data, sole caught-species Dex flag, exactly one Master Ball, field return
and flash persistence. Opponents unchanged; natural encounter and player attacks
remain real, including bounded retries for Protect/misses.

Fixed-frame PC confirmation initially timed out; driving native confirmation to
callback exit resolved the test fixture. No game-code fix, skipped assertion or
success stub. Source/test commitde3aa2ca; focused367 mGBA0.10.5 PASS; all16 host,
Museum and18-group registration static checks PASS. Production ROM matches clean
PR35 SHA923fba77; reused same ELF/ROM locally, CI fresh clean/upstream and full
final outcomes recorded in PR36 rather than guessed. Files: new native test,
runner registration/checker, documentation/tracker. No source/save layout changes.

Effective behavior remains the original implemented capture flow. BTL-004 earns
one narrowly scoped acceptance point; other ball/naming/whiteout/forced-end/weather
and external matrices remain. Total provisional progress294/546=53.85%; no reset
or duplicate implementation credit. Prior known rival/weather registration conflict
notes now link completed combined native/CI463243 inPR35. Drive sync pending.
Rollback only this test/docs delta to leave the parent game unchanged. No ROM/save/
BIOS/capture artifact distribution, merge, retarget or auto-merge performed.

### 2026-11-05 — BTL-005:410-move reconciliation and implementation plan, PR37

Owner explicitly selected move-system reconciliation and remaining automated
acceptance. Source reference is tested nickname/integration parent92838e5e.
No production code/data, move IDs, balancing, effects or artwork changed.
Artifacts docs/moves contain354-source+56-new records,20 approved original
redesign policies and one explicit Curse type ambiguity, source SHA/line evidence,
67 existing-art candidates, portable generation/checking, capacity and M1–M8 plan.
333 originals retain source fields under the existing split; no approved redesign
is falsely described as implemented. Newly declared custom Bug Bite differs from
the official berry move. Historical proposals/values are visibly superseded.

Native target ABI executed: BattleMove12/category9/flags8; ELF4260bytes355records.
Nine-bit level-up IDs support proposed355–410, without saved move-field widening.
Proposed explicit machine metadata preserves old item IDs and58 saved TM slots:
51TMs+7obtainableHMs fit; HM08Dive remains defined, old full-pocket handling stays.
No new SaveBlock1 slot is inserted. Four approved names exceed12; propose15-char
support with buffer/pixel/runtime review, never silent abbreviations. TM09LeechSeed
and49SignalBeam later approvals supersede early vanilla inventory rows.

Checks: generator repeatability, complete410 identities/categories/exclusions,
custom numeric exceptions and67 script-reference existence PASS; all16 earlier
host suites and3 static checkers PASS. Fifty animation choices remain explicitly
proposed; no new/composite playback or redesigned battle execution is claimed.
The existing parent463604 native proof does not cover unimplemented new moves.
Targeted Curse type question asked: TYPE_MYSTERY source versus Ghost inventory.
Other recorded version/animation decisions and species distribution are not
silently guessed. All17 type balance reviews remain locked.

Review branch docs/move-system-reconciliation-20261105, PR37, source/tooling
commit62853bc4 plus reviewed corrections; no merge or auto-merge. Six planning/
reconciliation points earned of existing80,74 implementation/acceptance points
remain. Overall provisional300/546=54.95%; no duplicate old-source test credit.
Drive sync pending. Further actual Bag/battle acceptance must be a separate test
change, not confused with this static design inventory. Rollback this docs/tooling
PR leaves the game unchanged.


## 2026-11-05 — PKM-005-BAG — actual Bag evolution acceptance, PR39

Owner requested remaining automated acceptance alongside BTL-005 reconciliation.
Separate branch feature/bag-evolution-acceptance stacked on37; source test commit
a6e179a3. No production code, gameplay, artwork or save layout changed. New native
bag_evolution.c plus runner/ABI symbol registrations exercise actual controller
START/Bag/USE/party/evolution/Bag/field input for all eleven approved item routes.
Before: full Bag-navigation gap. Now: focused mGBA49176 PASS including incompatible
targets, eight cancellation/retry routes, eleven one-item commits and flash
persistence. Everstone refusal and retained held King's Rock verified. The three
original Eevee stones retain noncancelable behavior; initial incorrect test
expectation corrected without source changes. All16 host suites/3 static checks
PASS; default full native and CI evidence pending at this checkpoint.

Authority unchanged: docs/EVOLUTION_MECHANICS.md, EVOLUTION_OVERHAUL.md and supplied
final evolution decisions; no Clamperl expansion or other methods invented.
Files/report: docs/BAG_EVOLUTION_ACCEPTANCE.md, tests/emulator/{bag_evolution.c,
run.py,runner.c}, integration registration checker and dashboard renderer. Fixes
two stale renderer summary sentences to match already documented Ranger/rival/
Blaine evidence. Real link-trade peer/hardware and exhaustive permutations remain.
One existing acceptance point earned (7/8 PKM-005), no denominator increase or
double-counting. Historical merged PR9 references retained; PR39 is follow-up,
not a false implementation/merge claim. Rollback acceptance PR leaves gameplay
unchanged; Drive synchronization still pending. No merge or auto-merge.


### PKM-005-BAG final local acceptance / BTL-005 CI evidence

Full default local mGBA0.10.5 suite PASS512774 guarded assertions, all19 registered
groups including all eleven Bag routes. All16 host suites/3 static checkers PASS.
No old group dropped. Native CI step budget15→20 minutes for added bounded scenes,
job limit still30 minutes. This does not waive internal scene timeout assertions.
Source/test revision a6e179a3; subsequent edits only docs/tracking/workflow budget.
ROM/ELF production SHA unchanged923fba77…bf35; no local new clean rebuild claimed.
PR39 checks retain actual final clean build/upstream/native logs; final remote
conclusions are reported in its review description, not inferred from local tests.
BTL-005 PR37 CI42032378420 SUCCESS: both jobs, reproducible clean modified build,
independent original upstream checksum and existing463604 native assertions.
These old-feature tests do not execute the56 unimplemented new moves. New move
runtime/animation proof, broader capture/AI matrices and external peer/hardware
remain unverified. Overall provisional301earned/245remaining=55.13% of546 points.
Review only: PR37 and39 remain open without auto-merge; main unchanged.


## 2026-11-05 — BTL-005-M1A — move-engine implementation begins

Owner resolves all three outstanding decisions: Curse Ghost/Status,50 animation
assignments approved for existing-art implementation/playback, Generation IV
Payback/switch and Dragon Rush/Minimize reference. Authority recorded in
`docs/moves/OWNER_DECISIONS_2026-11-05.md`; older question/proposal labels are
superseded, historical entries retained. Standing implementation authorization
and explicit “Lets do that now” authorize the foundation branch. No merge.

Before:12-char name capacity, two fixed13-byte TM buffers,25 fixed13-byte
relearner rows,16-byte used-move buffer, Curse???. After:15-char named capacity,
16-byte name buffers/rows,17-byte local battle buffer for name+!+EOS, Ghost Curse.
Summary switches to existing small font only when a full name exceeds the pane.
No other move fields, original IDs, battle scripts, save fields or artwork change.
The16-byte wire text record and64-byte BattleMsgData remain unchanged. No claim
of vanilla/foreign peer compatibility follows; matching-build testing remains.

Frozen eight-file source delta and old expected hashes protect all unrelated
bytes. Files: constants/global.h, battle_message.h, pokemon_special_anim_internal.h,
battle_main.c, battle_message.c, battle_moves.h, learn_move.c, pokemon_summary_screen.c plus tests,
owner/design artifacts, workflow registration and tracking. New native group20
and host suite17 retain every original group. Focused mGBA298 PASS tests temporary
ROM-copy names13–15 chars, actual StringCopy/message/TM allocation/relearner and
summary buffers; all patched bytes/canaries restored. Summary screenshot inspected
and complete. All17 hosts/3 static checks PASS; local clean rebuild identical to
ee0b18041ba9f042e47b8f3fc27e34862e116480a9391da98b20fb33ea6d40d0.

Initial clean-tool build lacked libpng search variables; retry with the existing
complete toolchain environment passed. Initial full local execution failed the foreign EXP text halfword read. An
overlapping rebuild was initially suspected; a fresh immutable-ROM execution
reproduced it. Measured gBattleTextBuff2 address02022ac9 was odd after17-byte
expansion. Explicit ALIGNED(4) preserves prior ARM halfword alignment, and new
native guards enforce it without weakening the original EXP assertion. The
runner also uses an immutable temporary ROM copy. Fresh final native/CI results
are linked in the foundation PR description, never inferred from focus.
No commercial ROM/BIOS/save files or screenshots uploaded/committed.

M1 remains partial: battle64px and relearner80px/cursor layouts need adaptation
before exposing long-name move entries. No IDs355–410, fake placeholder effects,
new mechanics/animation runtime or distribution have been enabled. Two existing
BTL-005 points earned (8/80 total);72 remain. Do not double-count parent acceptance.
Rollback reviewed foundation diff restores old name capacity/Curse type without
save migration; parent integrations/PRs/main stay intact. Drive sync pending.

M1a alignment refinement: focused foundation299 PASS; focused vendor battle309
PASS retains the original boosted-EXP/obedience assertions. Final aligned ROM
SHA2560cdab88878218a366909aa87f601447c8e1308b57538fbf038270303efb83ba3;
clean/full final conclusions are recorded in PR40 checks and description.

M1a additionally reconstructs EOS in each expanded local text-buffer tail after
the unchanged16-byte wire copy. This prevents stale unterminated text while
retaining64-byte controller framing; native guards cover the reconstructed tail
and every copied adjacent-buffer byte. No protocol or gameplay design changes.

M1a final tail refinement focused314/vendor309 PASS; all17 host suites/3 checkers
PASS. Pre-tail full integration run513067 PASS. Final source ROM SHA256
9a6e4fd94ee6ef9f12e2696a81c8f4da6c9e9cbfdfaa42534a7c8826ebc6c1a9;
its own clean/full final evidence is linked in PR40, not backdated to the earlier
full pass. No new move namespace/effects or broad runtime claims are introduced.

Final M1a consumer audit: the larger relearner name rows moved its u8 BG1 tilemap
to an odd offset. Explicit ALIGNED(4) preserves tilemap/halfword/DMA alignment.
The focused test now opens the actual relearner, navigates, cancels and returns
to field without changing moves; target BG pointer/alignment and screenshot
inspection PASS (focused330). Summary buffers precede no affected tilemaps;
PSA scene alignment follows its pointer-bearing struct. Full513082 before this
last safeguard passed; final source gets independent clean/full checks in PR40.

### M1a final executed local acceptance

Latest production source aaf898e413b11a7a0cc63d38c241cded19f6bf36: full20-group
mGBA0.10.5 PASS513097 guarded assertions; focused330, vendor309 and17 host suites/
3 checkers PASS. Clean rebuild identical e9fe4bd0034278dc289eef2a6a61dc1b4f7ef2c9ed32834dc836e4f5d6b68b1e.
Measured pinned target ABI offsets original0x264, unaligned expansion0x2AF,
explicit alignment0x2B0. Both rendered summary/relearner screens inspected.
Remote run conclusions in PR40 checks/review description remain distinct from
local proof. Later evidence-only tip commits do not change the tested production
source. Final milestone credit remains8/80, overall303/546=55.49% provisional; no
extra credit for reruns. All remaining M1–M8 work and external coverage retained.

## 2026-11-05 — BTL-005-M1B long-name UI

Owner's continuous approved move-system implementation authorization; authoritative
410-name inventory and November5 decisions unchanged. Separate child branch
`feature/move-engine-long-name-ui`, parent PR40/ee3a0e3c; no changes to main or
other PR heads. Previous64px battle cells/80px relearner list could clip approved
13–15-character names. New shared168px battle canvas provides76px per name;
88px relearner list plus compact numeric stats keeps full names and values.
40px side pane uses unique three-letter type tags and PHYS/SPEC/STAT from the
same authoritative move data, with PP preserved. Existing frame/cursor/font
artwork reused; original assets, move data, IDs, save/wire layouts unchanged.

Seven production files and frozen delta/test registrations are listed in
`docs/moves/LONG_NAME_UI_STATUS.md`. Static audits now detect overlapping battle
and relearner character allocations. Actual focused mGBA797 guarded assertions
PASS, Single/Double/menu input/reordering/PP/flash persistence/partner/relearner
scroll/cancel, all54 type/category combinations. Screenshots inspected locally;
no binary artifacts uploaded. All18 host suites/3 checkers PASS. Full21-group,
clean rebuild and remote CI are not inferred from focused success; final evidence
is appended when executed. Revert child PR only for rollback; parent functionality
preserved, no migration needed. Real peer link/hardware and full move-effects/
animations remain separate. Drive synchronization pending.

Current effective move-engine version: original354 playable moves with approved
Ghost/Status Curse,15-character capacity and M1b full-name layouts;56 new IDs and
new mechanics are not yet enabled. Provisional BTL-00510 earned/70 remaining,
overall305/546=55.86%; credit within existing scope, no duplicate regression credit.

### M1b final executed local acceptance / review reference

Production source715e1dff255a6aacaa22498ca5fb62063a26d3dd: full21-group
mGBA0.10.5 **PASS513858**, focused797, all18 host suites/3 checkers PASS.
Clean rebuild identical SHA256d6b1a818de5441d399bba63152b3aede8617c9e3a14f6a8314aadbb5176206a0.
BG0 window buffers retain the original344 tiles/11008 bytes, with disjoint
allocations. PR41 https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/41
is open against PR40; no merge/auto-merge. Actual remote CI conclusions belong to
PR41 Checks/review description; local success is not a remote success claim.
Later evidence-only commits preserve the production source above. Current earned
credit remains305/546,55.86%; remaining namespace and M2–M8 are not counted complete.

## 2026-11-05 — BTL-005-M2A eight approved original move revisions

Production source5a2b96f707806c8a6658dc4ff4e1f3e7bf2fee3b, branch
`feature/approved-original-move-data`, PR42:
https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/42
Parent PR41/3031dcdb retained, main/other PR heads untouched. Owner standing
implementation authorization and supplied approved Move Register v1 with later
owner updates govern the exact fields/evidence in approved-design-overlays.json.

Twineedle25→40 power; Lick20→40; Outrage90→120; Mean Look Normal→Ghost;
Astonish Physical30→Special50; Rock Tomb50/80→65/90; Silver Wind5→10PP;
Icicle Spear10→25 per hit. Only production file src/data/battle_moves.h changed.
All347 other rows (including NONE), effects, flags, targets, priority, unrelated
PP/accuracy and saved IDs/layouts remain. No graphics/animation/script/learnset/TM
changes. Historical category baseline/frozen source hashes retained with exact
normalization; Astonish is an explicit owner exception. Reconciliation preserves
REWORKED_ORIGINAL identity after implementation, with eight explicitly implemented
data markers and twelve original policies still pending, not inferred complete.

All19 hosts/3 checkers PASS. Native mGBA0.10.5 focused100/full22-group513952
PASS: exact controlled base damage/stat/AI data calls, Astonish screens/burn,
Lick Reflect/burn, Mean Look Status, Silver Wind10/16PP and legacy5PP flash
persistence. These do not certify every retained specialized effect/animation
scene. Clean identical11e457267e5f6dc958837ec3e83ae200c1e8b40eee96db4feca241fa5a5e221b.
Actual PR42 remote conclusions remain in its Checks/review description, not
inferred from local passes. Parent PR41 review42076325619/push42076316757 are
both SUCCESS; actual remote mGBA0.10.2 PASS513858 at3031dcdb.

Current effective move source: original354 IDs, Ghost/Status Curse, safe15-char
capacity/full-name UI and these eight exact revisions. No56 new IDs/effects are
enabled. Leech Life15→10PP and Roar20→15PP require tested saved-current-PP
handling before their atomic revisions. Real peer link/hardware and remaining
custom effects/animation/TM/namespace work are pending. Revert this child PR for
rollback, preserving parents; use pre-feature saves or clamp Silver Wind PP to
old5/8 maxima on downgrade. Automatic downgrade migration is not implemented.
No merge/auto-merge or binary artifact release. Drive synchronization pending.

Provisional BTL-00513 earned/67 remaining; overall308/546=56.41%. Three points
within existing scope, no extra credit for reruns or unsupported56-move completion.

## 2026-11-05 — BTL-005-M2B saved-PP compatibility / atomic Leech Life

Production4f7f71d753d7c4840b22617b65095fd56d9b57f5; latest test refinements
7c5334d0. Branch feature/move-pp-compatibility, PR43:
https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/43
Stacked on PR42/51df94f7, main and parent PR heads untouched. Owner standing
implementation authority and Leech Life's precise OWNER_EXPLICIT Move Register
fields/evidence in approved-design-overlays.json authorize Special80/10PP.

Previously, old records could retain15/18/21/24 current PP above new10/12/14/16.
Valid normal/recovered-backup loads now cap only excess Leech Life/Roar PP using
the authoritative maximum; preserve spent PP, PP Ups, other fields and unrelated
moves. Roar's current20PP/data behavior stays unchanged pending its separate
redesign. Live/serialized party,420 PC slots and both daycares covered. Route5
import remains lazy; only active copied parents capped, read-only recovery
record preserved. Pending eggs/mail/steps/unknown/corrupt storage unchanged.
No new save fields/version/offsets, erasure, RNG or immediate flash writes.

src/pokemon.c adds a copy-based one-decrypt/checksum/encrypt helper reusing
unchanged private primitives. src/move_pp_compatibility.c/header traverse sites;
src/save.c handles OK and valid-backup ERROR but skips EMPTY/INVALID/NO_FLASH;
src/route5_daycare.c caps the active legacy copy before publish. Only Leech Life's
category/power/PP change in battle_moves.h; original drain/contact/flags/target/
priority/accuracy/animation remain. No other row, ID, learnset, TM, asset, ability
or trainer data change. Historical hashes/category baselines retained through
exact frozen normalization; old host policy tests explicitly distinguish their
normalized migration checks from real native crypto/save tests.

All20 hosts/3 checkers PASS. Final native focus20105 PASS: all slots/PP Ups/
values/24 block orders/Shiny/tagged Egg identity, encrypted exact-byte preservation,
full420-PC stress within unchanged budget, real flash and corrupt-newer-slot
backup recovery, invalid/unknown data protections, nonzero mail/steps and pending
egg identities. Actual Single/Double Leech Life animation/HP drain and Liquid Ooze
reversal, one PP consumed, Run/field/flash; numerical Special/stats/AI/screens/burn
and Ether/PP Up/healing routines PASS. These item calls are not Bag UI proof.

Optimized full23-group local534050 PASS before final one-assertion AI/negative
fixture refinements; final focus20105 PASS afterward, production identical. Final
full latest-fixture remote count/conclusions are in PR43 Checks/review description,
never inferred. Clean identical8162003f6283a46766e453db66283287913bbd778ba171649f734d465689822c.
Initial repeated-getter full-PC timeout was fixed, not relaxed; superseded full
run stopped. A CPU-snapshot/flash-isolation test leak was fixed independently.

Current effective source: original354 move IDs, ten source-field policies now
implemented including Curse/nine original revisions, safe full-name UI and saved
PP handling. Eleven original policies and all56 new moves/effects/animations/AI/
TMs remain. Peer link/hardware, historical Quest Log party snapshots and exhaustive
drain interactions remain unverified. Revert child PR only; capped PP stays below
restored Leech Life15PP maximum, with no automatic refill or recovery re-import.
Parent Silver Wind downgrade limitations remain. No merge/auto-merge or ROM release.
Drive sync pending. Parent PR42 review42079507194/push42079496239 both SUCCESS,
actual remote mGBA0.10.2 full513952 at51df94f7.

Provisional BTL-00517 earned/63 remaining, overall312/546=57.14%. Four additional
points within existing scope, not rerun/56-move credit. Dashboard narrative now
uses the actual summed effort denominator rather than the old inconsistent
historical arithmetic sentence; no task history or effort totals reset.


## Historical PR53 scope proposal — superseded by reconciliation below



## 2026-11-05 — OWNER-DIRECTED FROZEN-SCOPE REBASELINE (DOCUMENTATION PR, NO GAMEPLAY MERGE)

Owner assumption: no additional Pokemon, no newly invented Trainer records, no new gameplay feature beyond existing approved commitments. Reconciled live Drive approvals and GitHub PR states. Prior ledger remains unchanged above.

- Existing 42 permanent task IDs and all historical earned values preserved; no task points removed. Updated scoped estimates for native-tested OPEN PRs #27, #29, #31, #35, #36, #39, #40-43 and MERGED #26; nonmerged code distinguished from main.
- All five Elite Four documentation PRs #32/#33/#34/#48/#46 MERGED. Eight specs approved, NO gameplay implementation credit. Codex issue #50.
- Follower 252 all-party system approved and DOCS-ONLY PR #52 OPEN. No sprite/license/coding credit.
- Ordinary non-Gym Trainer source records: 251/510 individual species assignments approved; 259 remaining. Party levels, moves and implementation separate.
- New **incremental only** tasks: PKM-009 28pt, GYM-006 52pt, GYM-007 16pt, WRL-009 20pt, WRL-010 40pt, WRL-011 10pt. Do not count any of these again as original broad tasks, including GYM-001/GYM-002, WRL-001/WRL-002/WRL-004.
- Museum Shiny Feebas code outdated: approved reward Aron Egg 50% true Shiny. Also approved Magby Egg 50% and Tangela TANGO. Rocket hideout #45 pending source changes.
- Rebased totals: **300 earned / 414 remaining / 714 all = 42.02% estimated**. Previous237/311/548=43.25% tracker was incomplete-scope historical baseline. New numbers reflect both progress and an explicit larger fixed backlog. Estimates are LOW CONFIDENCE, not time/hours or end-to-end release readiness.
- No existing PR merged, no code/main edited by this report. Changes published for owner review on branch docs/frozen-scope-progress-audit-20261105. Upon review/merge of this DOCS-ONLY PR, update main dashboard. No automatic merge.


## 2026-11-06 — Owner-authorized consolidation and stable-ID scope reconciliation

Owner explicitly authorized merging ready work, then requested selectable followers
from the merged main contract after consolidation. Recovery tag:
`recovery/main-before-consolidation-20261106` at2503a78a. The source chain
43→42→41→40→39→37→36→35 was merged upward preserving each commit. Main now
contains rival27, weather29 and Gym31 as well; all are recorded MERGED by GitHub.
Every intermediate feature-head tree matched tested PR43 treef7635334c9a1fc3865c18f7b06029af619bd20c6.
The main merge preserved eight newer Elite Four documentation files; no production
source differed from tested85032f28. Final source proof:20 hosts/3 checkers,
clean reproducibility and remote mGBA0.10.2 full534051 PASS in both PR43 runs.

Approved documentation PR28 (sign),49 (prototype-only lighting) and52 (followers)
were merged without gameplay changes. Draft30 had only a missing authorization
appendix already on main: resolved by retaining main's complete approved record,
verified exact main tree, marked ready and merged. No final lighting palette
approval, new gift implementation, ROM release or auto-merge is inferred.

PR53's planning inventory was reviewed rather than copied blindly. It collided
with permanent GYM-006 rival/GYM-007 Castform IDs and omitted their newer branch
history. Keep every one of the existing44 task records and their effort/history
unchanged; assign the six genuine new scope units PKM-009/GYM-008/GYM-009/
WRL-009/WRL-010/WRL-011. No duplicate rival/Castform credit in generic buckets.
The historical300/714 proposal above is superseded: actual preserved312 earned
of546 prior points plus166 net incremental scope gives a fixed712-point scope,
312 earned/400 remaining =43.82%, provisional. Previously completed work is not
reset; the percentage falls because omitted approved scope is now inventoried.
No new implementation credit for merging documents or branches.

Current effective code is the tested consolidated stack. New gift designs
(Aron/Magby/Tangela/quest) are NOT falsely claimed implemented; legacy reward
code remains until its dedicated approved implementation. WRL-010 is the owner's
next priority under merged implementation/selectable-following-pokemon.md;
0/40 points, no follower code/asset coverage claimed. Roar remains queued.
Follower source/graphics/license/save/opening feasibility gate must precede bulk
imports or unsafe save migrations. Direct Drive synchronization remains pending.

## 2026-11-06 — BTL-005-M2C approved damaging Roar

- Standing autonomous implementation/merge authority persisted first in AGENTS
  commit67bdb816/PR55. Existing stack through43/53 already merged, no duplicate code.
- Old Roar: Status/forced switch,20PP,-6priority. New approved register behavior:
  Normal/Special70/100/15/priority0/20% Attack-1, selected target. Source row and
  description only; native AttackDownHit effect/original animation/flags/access
  retained; Soundproof AI gate and unrelated Whirlwind unchanged.
- Source/tests/design translation and exact historical-delta normalization recorded
  in docs/moves/ROAR_STATUS.md. No save layout/art/new IDs/distribution changes.
- Executed21 hosts/3 checkers PASS; focused mGBA0.10.5 197 assertions PASS including
  actual Single/Double turns, outcomes/immunities/Protect and flash legacy cap.
  Clean reproducibility PASS9cc3d287edb97cb19a74f85bb204df2adcf7e435124a514bbdd166e4f1c48862.
  Full regression/CI pending initial commit; no merge/readiness claim yet.
- No additional effort credit until full acceptance. Rollback restores only Roar;
  prior capped PP is not refilled, save ABI and parent features unchanged.

## 2026-11-06 — WRL-004-SIGN Viridian category tip

- Approved merged contract PR28; standalone feature/viridian-move-category-sign
  from policy67bdb816, independent of pending Roar. No source behavior erased.
- Before: north sign recommends catching more Pokémon. After: exact approved type
  versus per-move Physical/Special tip and team-building message. Only that text
  block changes production source. Original south PP sign/map/scripts unchanged.
- Build PASS, exact-word/block-preservation check PASS, mGBA0.10.5 focused463 PASS:
  actual native north sign script/paging twice, southern text and field return.
  Full CI/clean/reproducibility pending; no merge/complete progress credit yet.
- Added native test module/registration/target ABI symbols and technical note
  docs/VIRIDIAN_MOVE_CATEGORY_SIGN.md. No save/flag/asset changes; revert text-only
  feature safely. Design authority is not changed by routine line wrapping.

## 2026-11-06 — Autonomous merges / effective Roar acceptance

- Policy PR55 merged b43f92cf1424a107362de2e3daa828548292b292 after both checks PASS.
- Roar PR56 merged844b450e7b60b7f0699fef348ca8b7acb8d73824 after exact head
  42410891 push run42215345347 SUCCESS,21 hosts/3 checkers, clean reproducibility
  and full latest534242 mGBA assertions PASS. Redundant review run still pending
  at merge; no required protection bypass/security change/auto-merge used.
- Earlier local full534203 omitted39 added flash/Protect checks; final remote full
  includes them. Current effective status supersedes initial pending note above.
- Credit2 within BTL-005's existing80 points (19 earned/61 remaining); total314/712.
  Main postmerge validation continues separately; no410-move completion claim.
- Sign PR58 merged main into its branch, retaining both Roar and sign native groups,
  all21 host registrations and append-only histories; actual integrated CI required.

## 2026-11-06 — WRL-011-GIFTS / issue38 three owner-approved corrections

- Museum guaranteed-Shiny Feebas→Aron50% genuine Shiny; Water Labyrinth Togepi→
  Magby50%; roof EeveeLv25 nickname prompt→TangelaLv25 fixedTANGO. Existing one-time
  bits/claimed saves preserved; no owned mon replaced, starter/Parcel untouched.
- Added final-PID-at-creation binary-outcome helper and safe native gift specials;
  preserve ordinary daycare/wild/other eggs, cycles, hatch/naming, party/PC and
  gates. Museum special index unchanged; two specials appended inside table.
- Exact source/delta/rollback details in docs/APPROVED_GIFT_REVISIONS.md. Historical
  Museum docs retained with clear superseding effective-version note.
- Existing20 hosts/3 checkers plus new gift audit PASS; build PASS; mGBA0.10.5
  focused80112 PASS (Aron5030/10000,Magby5030/10000 stored trueShiny), Museum1306
  PASS including real trivia, delayed claim, hatch/evolution and flash. Full/clean
  CI pending initial commit; no completion/merge credit yet. No ROM assets/save
  changes. New independent branch feature/approved-gift-revisions.

## 2026-11-06 — GYM-009 independent six Elite Four party implementation

- Issue50/eight exact merged contracts inspected. Only six Lorelei/Bruno/Lance
  initial/rematch arrays changed; all metadata/scripts/save/Champion/rival and
  both Agatha arrays preserved. Exact32 Pokémon/128 moves/held items,250/255 IV
  policy, Lance six-member levels-only rematch, native ability slot0 choices.
- All21 hosts/3 checkers and build PASS; native constructors verify all32 exact
  abilities/items/levels/IVs/moves. Initial four actual room battles PASS; Lance
  saw all6 but postbattle test input used stale battle controller. Fixture fixed
  to gate battle controls, no assertion/timeout/production workaround. Final six
  battle rerun/full/clean CI pending initial commit.
- Both Agatha encounters explicitly not implemented: constructor forces even PID
  and slot0 Synchronize, approved Inner Focus is slot1. Existing contract requires
  conflict report; owner asked about narrowly scoped native selector. No global
  species override or invented substitute. See docs/ELITE_FOUR_IMPLEMENTATION.md.
- Standalone feature/approved-elite-four-battles; no effort/completion credit yet.
  Revert six arrays safely, retaining prior histories/flags. New native fixtures
  use deliberately boosted player stats, not balance or complete strategy proof.
- GYM-009 failure diagnosis correction: stale input and HP changes alone did NOT
  resolve Lance. His postbattle `{RIVAL}` expanded an unterminated zero-filled name
  in the synthetic save, corrupting fixture state. Initialize valid EOS name like
  existing rival tests, clear unused party slots; no game-code fix/guard relaxation.
  Latest actual initial Lance victory/all6/field/flash PASS. Rematch/full latest
  acceptance still pending; earlier hypothesized root cause above is superseded.

## 2026-11-06 — WRL-004-SIGN effective merge / WTH-003 move milestone

- Sign PR58 merged d97712dc9393b5f98d73df901f8ffa70cc231309. Exact integrated head
  2b98586b review42217742516 SUCCESS,21 hosts/3 checkers, clean rebuild and full
  native534699 PASS; focused463. Main postmerge checks remain separately visible.
- WTH-003 / BTL-005-M2D approved issue47/register: Sandstorm typeRock→Ground,
  remove only Rock/Steel residual typing immunity; add Zap Cannon to native Rain
  guaranteed-accuracy path. Existing suppressions,1/16minimum1,Ice Hail, Thunder/
  Forecast/WeatherBall,83 map rules/species slots/state/save/IDs retained.
- Only src/data/battle_moves.h and src/battle_script_commands.c production hunks.
  Exact reviewed delta normalizers retain old hashes. No new abilities/Hurricane/
  TM51, event placements or missing global ability mappings invented.
-22 host suites/3 checkers PASS; focused mGBA0.10.5 5389 PASS (324 native residual
  type pairs; normal/Rain/Sun/CloudNine/AirLock accuracy; actual Rain Zap turns
  damage/paralysis,Ground/Protect/onePP). Native interpreter fixture first failed
  due unaligned bus writes; corrected operand byte storage, no game/assertion fix.
- Build PASS; clean/full latest CI pending initial commit. No added effort credit
  before full acceptance. Technical scope/evidence/limits/rollback in
  docs/WEATHER_MOVE_COMPLETION.md; no saved-schema migration or binary artifacts.

## 2026-11-06 — INT-001-CI early bounded mGBA dependency installation

- PR62 run42233960801 cancelled at30m18; modified step9 installing libmgba-dev
  spent24m35 and native step was skipped. This is NOT an emulator assertion failure.
  Upstream baseline passed; exact network/lock cause not exposed by step metadata.
- Move dependency into initial modified-only package set, bounded APT transport/
  retry/lock waits; later mGBA step verifies installed package only. All native/
  host/build/repro/upstream gates, API guards and job/native timeouts retained.
- No production/security setting change. docs/CI_DEPENDENCY_STALL.md records
  exact evidence/limitations/rollback. Actual maintenance CI pending before merge.

## 2026-11-06 — Effective autonomous integration / current versions

-59→708f197a0b3889bc96c1272c6d52f4eb837c727b;60→66117462336bff24fe7ea3115f91db6851e27980;
 61→15dacb95424ffd394113849d10c3aa2f78c343ba. Exact-head CI SUCCESS respectively
 42220799681/42221063828/42222085136; both modified/upstream jobs passed before
 direct merges, no admin bypass/security change/auto-merge. Main content tree
 d6af90108bce62c464aca34297a026378a307419 equals validated61 head22314c35.
- Combined local24 hosts/3 checkers and full28-group753726 native PASS; clean
 bba80adbe5de31b285e704f202cdda61e8aa4f4ca2256c1d8762082c73984b77. Individual
 focus: gifts latest80123/Museum1306, E4 integrated133086, weather5389.
- Current effective gifts: Aron50%/Magby50%/TANGO, not old Feebas/Togepi/Eevee.
 Current E4: six exact battles, Agatha two NOT implemented. Current move milestone:
13 original policies on main; M2E63 two originals tested314 and cleanf8d6d10d...
 but full/CI pending, no credit;56 new moves/global Electric paralysis still due.
- Effort: BTL-00521/59 (+2 for weather within80),GYM-00912/4 (six exact battles,
 two held),WRL-0113/7 (three gifts, Rocket quests remain). No sign/CI/shared-weather
 duplicate credit or scope reset. Source exports available; follower54 verified
 audit/permission hold restored to live tracker, not reset to an unstarted audit.
-62 workflow guide inspected: no authority/gate change. Replace its unverified
 handoff template with factual <=100-line resume state, refs/commands/limits.
 Includes CI64 dependency until that verified maintenance PR merges. Main-source
 docs provenance/diff proof separate from pending new workflow run; no fresh game
 implementation or source unavailability claim.
## 2026-11-06 — BTL-005-M2E Pin Missile and Dragon Rage

- Approved register664/749: Pin Missile2–5×14→one55 hit/30% poison (85acc/20PP
  retained); Dragon Rage fixed40→current user level (Dragon/Special100acc/10PP/
  power1 sentinel retained). Native POISON_HIT and LEVEL_DAMAGE scripts reused.
- Only two move rows/descriptions production delta; existing flags including
  King's Rock/Protect/Mirror, animations, IDs, all other fields/rows/learners/TMs
  and PP maxima preserved. Explicit source translation/normalizer retains hashes.
- Build and25 hosts/3 checkers PASS; focused native314 PASS, Single/Double numeric
  one-hit and poison/nonpoison/miss outcomes, eight actual fixed-level1/27/50/100
  turns despite skewed stats/Steel resistance. Native AI Pin50 checked; fixed-move
  AI stock heuristics retained, not an exhaustive new estimator certification.
- Full29-group/clean/CI pending initial commit; no added completion/merge credit
  yet. Branch feature/pin-missile-dragon-rage, details/limits/rollback in
  docs/moves/ORIGINAL_EFFECT_STATUS.md. No PP migration or binary assets.

## 2026-11-06 — BTL-005-M2F Electric paralysis immunity implementation

- Approved global Electric immunity: shared effective-recipient paralysis guard,
  primary unaffected message and existing bad-move AI type check. No move data,
  targets, saved fields, abilities, PP maxima or artwork changed.
- Build and25 hosts/3 checkers PASS. Actual focused/full mGBA, clean repeatability
  and remote CI pending final snapshot; no merge/completion credit yet.
- Native fixtures cover primary, guaranteed secondary with retained damage,
  Static contact, preexisting paralysis and AI score controls. Details and limits
  in docs/moves/ELECTRIC_PARALYSIS_STATUS.md. No new Thunder Wave spread claim.

## 2026-11-06 — BTL-005-M2G both-opponent status move implementation

- Approved Scary Face Ghost15PP and three both-opponent targets, retaining90/85/
  100 accuracy and native Speed/Defense/Sp.Def-2. Native target iterator/scripts,
  flags, assets, original IDs and learnsets retained; descriptions updated.
- Build and25 hosts/3 checkers PASS. Native actual Single/Double controls confirm
  onePP/no ally or user effect/no direct damage, both hits and independent split
  hit/miss for Scary Face/Screech. Final focused/full/clean/CI pending. No credit
  for Thunder Wave targeting, Spite/Grudge or56 new moves.
- Stacked on isolated Electric immunity66; no new save schema or PP refill.
## 2026-11-06 — INT-001-CI early bounded mGBA dependency installation

## 2026-11-06 — BTL-005-M2E final acceptance / integration

- PR63 merged767bb39c after full29-group754034 guarded mGBA assertions,25 host suites/3 checkers, clean identical rebuild and both exact-head Actions42311654769/42311756718 SUCCESS. Scope/limits remain ORIGINAL_EFFECT_STATUS.md.
- Two points earned within existing80 move effort; total333/712 (46.77%) provisional. No56-new-move credit.
- Transient worker GitHub401 recovered without credential changes; repository permissions/protections unchanged. CI64 and tracking62 still pending, not merged based on pending checks.
## 2026-11-06 — INT-001-CI measured slow package transfer

- Live job128215173523 proves APT62.7MB/19min7s/54.7kB/s, then build start.
  Initial idle timeouts cannot prevent slow continuing transfers. Overall Actions
  job limit30→45min; native20min, instruction guards, clean builds, all checks and
  upstream comparison unchanged. No gameplay/security/protection changes.
- Exact-head CI pending; not represented as accepted/merged.

## 2026-11-06 — Postmerge queue reconciliation after PRs 59, 60 and 61

- PR #59 (merge 708f197a): Museum Aron Egg and Water Labyrinth Magby Egg, each 50% actual Shiny, plus Tangela Lv25 fixed TANGO. PR head b099cdff GitHub Actions run 42220799681 SUCCESS. Only gift portion of WRL-011 implemented; Rocket rescue/return quest is not included.
- PR #60 (merge 66117462): six initial/rematch League battles for Lorelei, Bruno and Lance, 32 Pokémon/128 moves, scripted native battle coverage. PR head f30b91cd GitHub Actions 42221063828 SUCCESS. BOTH Agatha battles remain untouched pending narrow native ability-slot-1 Inner Focus selector. Six of eight battle contracts are not eight of eight.
- PR #61 (merge 15dacb95): Ground Sandstorm immunity and Rain Zap Cannon accuracy. PR head 22314c35 GitHub Actions 42222085136 SUCCESS. No general species-wide abilities, route static encounter scripts, lighting visual approval, or full campaign acceptance.

### Provisional fixed-scope planning credits (low-confidence effort allocation)
Retain total712 points and all preexisting earned work. Allocate *incremental* credit only: WRL-011 +6/-6 (three gifts; Rocket quest left); GYM-009 +12/-12 (six of eight battle contracts; Agatha blocked); WTH-003 +2/-2 (Sandstorm weather coupling); BTL-005 +1/-1 (Zap Cannon special accuracy policy). This gives 335 earned /377 remaining =47.05%, versus earlier314/398=44.10%. These are deliberately provisional effort weights, not measured hours, objective feature percentage, complete QA or release readiness. If a specialist revises relative weights, update both earned and remaining together with history, do not double count scope.

Full open-queue triage, explicit unresolved dependencies and effort weighting: `docs/IMPLEMENTATION_QUEUE_RECONCILIATION_2026-11-06.md`. This is a reviewable documentation update; no additional ROM or emulator suite was run by this tracking-only change.

## 2026-11-06 — INT-001 parallel tracker reconciliation

- Preserved PR65 source/PR/issue snapshot and append-only historical entry in PR62.
  Its335-point allocation was an alternative provisional estimate, not extra work.
  Retain canonical per-task histories/333 of712 (46.77%) after merged63: gifts3,
  six E4 battles12, weather2, Pin/Dragon2 incremental points. Do not add the same
  weather/gift scope twice or reset follower audit progress.
- New isolated PR66 Electric paralysis and stacked67 three spread statuses are
  built/25-host/clean tested, full native/CI pending. No completion points yet.
- CI64 latest overall45min envelope preserves20min native limit; exact-head CI
  required. Transient worker authentication recovered without credential changes.

## 2026-11-07 — BTL-005-M2F / INT-001-CI validated combined integration

- PR66 merged as d3a462e5e991f64ac8245b7e11c990c895df61ad. Its ancestry includes
  PR64/d3d38c7a; GitHub marks64 merged through63b152f5. This uses the owner's
  permitted validated integration strategy, not a claim that64's separate pending
  dispatch42314671025 passed. Both combined-head Actions42315238692/42315244202
  modified and upstream-baseline SUCCESS; actual game/test source equals locally
  tested f376e342 (full29-group754679 PASS,25hosts/3checkers/clean PASS).
- Maintenance64's workflow changes are unchanged; combined workflow adds only the
  original-effect host suite already merged through63. Every guard/assertion,
  native20min and clean/upstream gate retained. No security/protection changes.
- Global Electric rule earns1 point within existing80; canonical334/712 (46.91%).
  CI/shared acceptance not counted twice. Main postmerge Actions still pending.
- PR67 full755166 PASS incl Clear Body/Protect/stat-floor; remote CI pending.
  PR69 focused225 initial,25hosts/clean PASS; latest full pending. Draft68 repaired
  shared hash normalizer,25hosts/clean PASS, both Agatha native focus39058 PASS;
  full/CI and specific selector approval status remain pending, not merged.

## 2026-11-07 — BTL-005-M2G final acceptance / integration

- PR67 merged4526a137573c722d1f685ac42a8057fc9b36497a after full755166/25hosts/3checkers/clean and both Actions42315257252/42315264496 SUCCESS. Main tree equals tested144917d5. Two points within80; canonical336/712 (47.19%). New56 moves not counted. Main postmerge CI pending.
- PR69 latest full and draft68 latest full remain active at this snapshot; no pass inferred from intermediate logs. Updated durable handoff with exact refs/commands/holds.
## 2026-11-07 — BTL-005-M2H Spite / Grudge implementation

- Final approved register574/582: Special30 damaging Spite plus optional original
  PP drain; Special30/90/10 Grudge using native Rollout without Defense Curl bonus,
  selected opponent required by damaging Rollout semantics, no original KO status.
- Retain original flags/art/IDs; update descriptions/categories/reconciliation,
  exact reverse manifests and HP-aware Spite AI. Existing PP is not refilled.
- Build/25 hosts/3 checkers PASS; initial focused225 native assertions PASS,
  seven actual Spite turns and complete five-turn Grudge. Latest numeric control,
  full/clean/CI pending; no merge/effort credit. See GHOST_REWORK_STATUS.md limits.
- Native PP test isolated the sleeping opponent after detecting an incorrect
  zero-PP turn-spending assumption; no production fix or weakened baseline.

## 2026-11-07 — GYM-009 draft68 validation repair (not acceptance)

- Existing draft68 proposed both Agatha parties and the scoped native ability
  setter. Did not duplicate gameplay. Its Actions42315909041 failed historical
  battle_main preservation before compilation; exact constructor reverse manifest
  now participates in every old audit, preserving all original digests.
- Added native stored ability1/even PID controls, actual battle-loaded Inner Focus
  observation, Dark-aware legal controlled-player attacks and Agatha-only focus.
  Opponents, AI, production selector and parties unchanged from draft.
- Build/25 hosts/3 checkers PASS; latest focused/full/clean/CI pending. Draft/design
  status retained; do not equate existing native API availability with an answered
  owner selector question. CI64 dependency integrated, no test gates removed.


## 2026-11-07 — GYM-009 dashboard point reconciliation
PR #68 merged to main at f3a113a44d89acacc0b584f61c4dead868ef2607; exact-head Actions 42321156769 passed host, clean build, baseline, all eight native Elite Four encounters, 42 Pokémon/168 moves, 793818 mGBA assertions. GYM-009 previously 0 earned/16 remaining now 16 earned/0 remaining, with historical effort entry retained. Overall 314/398 -> 330/382 of unchanged 712 planning points: 44.10% -> 46.35%. GYM area 32/90 -> 48/74 of unchanged 122 points: 26.2% -> 39.3%. All other task effort records unchanged. Subjective balance and hardware tests not claimed.


## 2026-11-07 — GYM-008 ordinary Trainer implementation acceptance and progress reconciliation

- Reviewed PR71 completion handoff https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/71#issuecomment-6703579243 and confirmed squash merge on main `18d245b5e35780b1300482aa536fa15f140a1d28` (preserving concurrent Agatha GYM-009 source).
- Scope accepted: 510 ordinary Trainer source IDs; 500 non-dummy party arrays and 1,219 Pokémon instantiated in native ROM tests; 10 dummy source records intentionally unchanged. Includes approved class/name transformations, Shane Route24 Double/rematch/refusal and existing-source-compatible gendered Ranger aliases. No bespoke Ranger overworld art claimed.
- Exact PR-head Actions 42346786298 SUCCESS for split battle validation (modified and baseline), and 42346786287 SUCCESS for ordinary Trainer preflight/source-preservation. These validate source/build/native scoped work, not complete manual progression.
- Historical GYM-008 effort changed from 0 earned/52 remaining to 52 earned/0 remaining; denominator712 unchanged; overall 330/382 to 382/330 =53.65%; GYM area 48/74 to 100/22=81.97% (rounded82.0%). GYM-009 16/0 and all other task point values remain untouched. No duplicate implementation credit.
- Optional experiential QA remains distinctly under existing INT-005 stabilization/release acceptance: approach every or representative overworld Trainer, exercise all 130 live VS Seeker rematch routes, Shane double/refusal, victory flags, story states and existing-art Ranger appearance. These checks are NOT claimed executed or necessary to regard GYM-008 source implementation as merged. No additional points added; retain INT-005 original 0/8 status.
- This reconciliation is documentation-only on a review branch. The older PR65 335/377 planning calculation and Agatha hold predate PR68/73/71 and MUST NOT be merged unchanged.

## 2026-11-07 — INT-001 converged parallel progress / BTL-005-M2H integration

- Preserve main's completed GYM-00852/0 and GYM-00916/0, plus coordination's
  BTL-00526/54 and WRL-0113/7. Add only merged69's3 existing-scope points: moves
  29/51; canonical395/712 (55.48%). Every other task uses latest main; all prior
  effort histories retained. This is provisional effort, not release readiness.
-69 merged3d06ebc3 after755386/25hosts/clean and both CI PASS. Parallel68/71/75
  are already merged; old draft/blocker claims are historical, not live state.
-72 current-main c33c8650 has26 host suites/3 checkers/clean PASS; old755945 was
  pre-integration. Fresh30-group/full CI pending; no new completion credit.
