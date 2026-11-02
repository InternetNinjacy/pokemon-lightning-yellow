# Pokémon Lightning Yellow — Master Implementation Change Ledger

Canonical branch: `docs/project-coordination`. Established 2026-11-02.
Drive synchronization: **pending**. No document has been created on Drive. Intended destination: [existing project folder](https://drive.google.com/drive/folders/1N_n0Ke48jBbdhUWSZWzwR6rpPFqpDUpl).

Git history is the authoritative technical record. Historical entries below are backfilled from commits and their committed reports; they are not fresh executions. Commit dates are Git dates, not inferred design-approval dates. Design approval and code implementation are separate events. Preserve these entries and append corrections/supersessions.

## Current Effective Version

| System / tasks | Newest available approved design | Implemented on main | Implemented on open stack / remaining |
|---|---|---|---|
| Baseline / INT-001 | FireRed English rev0, pret commit ce6406d8c2eaf46d543c8a2606de0c3cb1951982 | e04252ea, original SHA-1 41cb23d8dccc8ebd7c649cd8fbb58eeace6e2fdc | Independently retained CI baseline |
| Dex / PKM-001,003,005 | Owner Castform expansion:252, Castform#233, preserve relative order; October28 handoff typing overrides | Original FireRed | PR#6 has252/canonical save flags; twelve typing overrides and expanded acceptance pending |
| Evolution / PKM-002,004 | Latest evolution mechanics overrides older eight direct-use pairs: six pairs, no Clamperl additions; Tyrogue choice>=15; roster-aware National gate removal approved by October28 supplied handoff | Original FireRed | PR#5 methods/menu implemented; roster-aware access pending |
| Categories / BTL-001,003 | GenIV per-move assignments; owner explicitly chose Shadow Ball SPECIAL | Type-based split | PR#2 split implemented; stronger acceptance pending |
| Wild doubles / BTL-002,004 | Owner capability assignment: designated maps, independent draws, weighted5% Onix+Cloyster | Singles | PR#7 proof of concept on experimental Route1/MtMoon1F, conditional acceptance; release hardening remains |
| Move redesign / BTL-005 | Owner October31 audit handoff; complete authoritative values not available | Original354 | No410-move implementation; audit outputs are not implementation |
| RTC / TIM-001,002 | Read-only RTC; functional DAY[06:00,18:00), NIGHT otherwise, invalid timed friendship unavailable | No RTC | PR#3 clock implemented; consumer still noon-based, correction pending |
| Visuals / TIM-003 | Four outdoor/two covered periods; Lavender and Mansion exceptions in supplied October28 handoff | Original appearance | No visual implementation; transitions/map exceptions/colors unresolved |
| Weather / WTH | October28 supplied handoff distributions/transition rules; final unanswered exceptions retained | Original weather | No dynamic implementation; Hail duration/active battle precedence and maps unresolved |
| Route5 / DAY | Owner consolidated execution request; dedicated register link supplied but contents unavailable | Original single deposit daycare | Planned independent storage/50%Shiny; no implementation; full register required |
| Gyms/world / GYM,WRL | Project direction and partial approved framework; exact new designs not supplied | Original trainers/maps/items | No invented placements; Castform trainer/level/moveset pending |

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
- Source files: include/pokemon.h, include/battle.h, include/constants/battle.h, src/data/battle_moves.h, src/pokemon.c, src/battle_script_commands.c, src/battle_ai_script_commands.c, src/battle_ai_switch_items.c, src/battle_controller_player.c, src/pokemon_summary_screen.c, tests/run_split_tests.py, docs/PHYSICAL_SPECIAL_SPLIT.md (consult commit for complete list).
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
