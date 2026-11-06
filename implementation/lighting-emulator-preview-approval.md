# Feature: Emulator Lighting Prototype, Screenshot Capture and Owner Palette Approval

**Project:** Pokémon Lightning Yellow (FireRed source-based, 252 approved species; not any separate Pokémon hack)  
**Status:** Ready for implementation of an **isolated, reversible visual preview and capture harness only**. Final production palette values are **NOT APPROVED**.  
**Approval:** Existing lighting schedule and exceptions approved in Lighting & Weather decisions LY-LW-001–004, 047–048, 052; owner's Nov 5 instruction authorizes preparation of the emulator screenshot approval workflow.  
**Handoff revision:** 2026-11-05 / LIGHT-PREVIEW-001  
**Dependencies:** Current integrated RTC source; dynamic-weather PR #29 (open/unmerged at preparation; verify again before coding), and existing mGBA/ELF test runner.  
**Scope:** Produce real emulator evidence and testable prototype choices for user visual review. Do not silently ship a final lighting system.

## 1. Goal and player-facing behavior

Build the game's **first controlled daytime-to-nighttime visual proof of concept** in the actual Lightning Yellow FireRed ROM and deliver real, labeled emulator screenshots that the owner can judge. First visual direction: **Classic Pokémon nighttime**. Use moderate bluish shading, visibly darker than FireRed's day scene, readable map details/sprites, comfortable navigation. Daytime should resemble unchanged original FireRed. Dawn modest/subtle, dusk slightly warm, night clearly darker. **All numeric color/tint/intensity/fade values are prototype candidates and must be explicitly approved by the owner after comparing actual emulator captures.** No approximated images, mockups, web reference art, image generation, or host-only palettes may masquerade as screenshots from the actual modified ROM.

This handoff authorizes **prototype code on a reviewable branch and screenshot/review tooling**, not adoption/merge of a permanent lighting feature. A separate owner approval is required before final palette production implementation or merge.

## 2. Exact approved lighting requirements (do not renegotiate)

### 2.1 Functional clock versus appearance

| RTC local hour interval | Functional day/night | Ordinary open-sky visual | Viridian Forest covered visual | Lavender Town outdoor visual |
| --- | --- | --- | --- | --- |
| 00:00–05:59 | NIGHT | Night | Covered Night | Night |
| 06:00–06:59 | DAY | Dawn | Covered Day | Dusk |
| 07:00–17:59 | DAY | Day | Covered Day | Dusk |
| 18:00–18:59 | NIGHT | Dusk | Covered Night | Night |
| 19:00–23:59 | NIGHT | Night | Covered Night | Night |

At **06:00, 07:00, 18:00 and 19:00 exactly**, lighting must update without a required map change if RTC works. The visual period NEVER changes functional DAY [06:00,18:00), NIGHT [18:00,06:00), encounter availability, friendship evolution, or weather rules. No time-exclusive species.

### 2.2 Map categories and exceptions

1. **Open sky:** Outdoor map-level, entire map uses one of the four preset appearances. Pallet Town, Route 1 and a representative more detailed outdoor town/route are suitable capture scenes; first-choice calibration scene is **Pallet Town, Neutral 100% weather**, minimizing confounds. Preserve source map art, geometry, NPCs and object positioning.
2. **Covered:** **Viridian Forest only** currently uses its own shaded DAY (06:00–18:00) and NIGHT (18:00–06:00). No covered Dawn or Dusk variants. Do not classify other maps as covered merely by being wooded.
3. **Lavender Town:** outdoor **Dusk appearance the entire functional DAY** and **Night appearance the entire functional NIGHT**. No regular Dawn/Day look, and no different dusk-at-18 preset there. Weather/RTC mechanics unchanged.
4. **Ordinary interiors/caves:** preserve original unfiltered appearance, including Flash behavior (Rock Tunnel), existing cave darkness and Pokémon Tower's fog. Do not tint menu panels, dialog boxes, battle UI or indoor maps merely because the overworld is dark.
5. **Pokémon Mansion:** 1F, 2F, 3F, B1F carry persistent Pokémon Tower-style fog at all times. This is a distinct approved visual exception, **not** outdoor nighttime tint. Test feasibility with original assets/callbacks; if unsuitable or conflicting, report specifically, do not add new graphics without approval.
6. **Invalid RTC fallback:** show ordinary daytime lighting instead of arbitrary night. On RTC recovery, current outdoor map should synchronize immediately to actual period without map transition. Preserve existing 18:00 Sun->Neutral behavior, daytime probabilities on invalid RTC and sunset conversion; do not let lighting preview interfere with gameplay or weather.

**One whole-map policy, not per-tile microclimate.** No new Flash-like visibility circle, canopy shadows, glows, windows, new art assets, partial tile zones, redraws or geometry changes.

### 2.3 Weather independence and UI correctness

The dynamic weather PR #29 implements map weather rolls, battle-weather mapping and RTC recovery. Lighting MUST be applied independently of weather category. All five approved categories Neutral, Rain, Harsh Sun, Sandstorm and Snow/Hail retain their own overlays/effects and map probabilities. Do NOT introduce periodic weather rerolls, change route weighting, special event weather flags, battle weather, or Cycling Road behavior. Demonstrate test captures both without weather effects (Pallet) and with at least Rain plus one particle-heavy weather condition in a representative eligible outdoor map. Verify visual legibility and layering in all situations.

Do not alter any palette that should be owned by menu, battle, sprite, fog, weather or fade code without first documenting how it is composited and made reversible. Rendering sprites should remain identifiable; avoid double-tinting after menu/battle transitions. Palette state must not leak to indoor maps and must survive save/reload, field effect, battle return, fades and object/map changes.

## 3. Approval provenance, technical references and precedence

- Master Plan: https://docs.google.com/document/d/1P4TuXViTpB0BycldB1SzRZTRX8sV_Rd0lFuPwyGDJQk/edit
- Start Here / workflow: https://docs.google.com/document/d/14bV_mRB-IgcyWqGcIH2tgmvNoYLY1Ip_zSqSbdHGYtw/edit
- Canonical Lighting & Weather Specification: https://docs.google.com/document/d/1FLDAJ8fufYit9r5uwrvJiKA92WVjWmNWpvazlyUoEn4/edit
- Decision Register: https://docs.google.com/document/d/1SUEyKXuDK8m-LfNjTOYfr2tH7j7buHBkf5v_gGup8eA/edit (particularly LY-LW-001–004, 047, 048, 052)
- Weather maps and priority: https://docs.google.com/document/d/1knvQ2nUoFJuwcBdY05CVUsn_vlwl36XB79D7PxTqWJY/edit
- Cross-thread intake: https://docs.google.com/document/d/1NpCjx3uboBMdBVqoFpMwXEyVkDquD37Kcp-FJBbYWnc/edit
- Emulator QA doc: `docs/EMULATOR_VALIDATION.md`
- RTC doc: `docs/RTC_SUPPORT.md`
- Weather work: open PR #29 `feature/dynamic-map-weather`, `docs/DYNAMIC_WEATHER.md`, `docs/DYNAMIC_WEATHER_MAPS.md`. Inspect latest refs before work.

**Verified repository paths on 2026-11-05:** `src/palette.c` (palette/fade machinery); `src/field_weather.c` (weather gamma and effects), `src/rtc.c` (read-only RTC), `docs/EMULATOR_VALIDATION.md`, `tests/emulator/run.py` (existing test entry point). These are source leads, **not** declarations that their APIs already implement lighting. Codex must inspect exact routines, buffer lifetimes, weather palette transformations and ownership before deciding minimal safe hooks. Consult `src/overworld.c`, `src/field_weather*.c`, map headers/constants and test runners as needed; verify paths/symbols. No Emerald expansion code is assumed drop-in compatible, and no outside code should be imported without licensing and suitability review.

Current `main` and open PR status are authoritative over historical source audits. Reuse existing RTC and weather features, do not fork their gameplay logic.

## 4. Implementation boundaries

**In scope:** Research exact render/palette hooks; implement a minimal **opt-in preview-only** runtime/palette treatment; deterministic clock control/test fixtures without modifying genuine RTC semantics; representative lighting variants; real mGBA frame capture; screenshot comparison/contact sheet production; headless and visual tests; explicit prototype-parameter manifest; evidence/review notes.

**Out of scope:** Final production palettes, merging gameplay lighting, permanent tint presets without owner screenshot approval, new graphics, redesign of maps or sprites, further weather gameplay implementation, evolving Pokémon at altered times, battle mechanics redesign, map weather distribution adjustments, indoor lighting redesign, extra covered maps, custom assets, long-term saved lighting parameters, date/season mechanics. Do not merge this prototype with #29 or other gameplay feature PRs solely to make the preview.

**Preserve:** Original FireRed palette during Day, map/layout/tiles, RTC evolution logic, save and Pokémon data, weather semantics, battle sprites, menu readability, weather overlay functions, Flash, Fog and native transitions.

## 5. Source implementation and capture plan

### 5.1 Dependency baseline

1. Read `AGENTS.md`, `implementation/README.md`, `docs/EMULATOR_VALIDATION.md`, `docs/RTC_SUPPORT.md`, existing weather PR #29 and the current source/test directory first. Record actual current `main` SHA, #29 state/head, and other overlapping PRs.
2. This handoff is based on `main` for **documentation publication**. For the prototype, prefer current main if enough weather and RTC exists; otherwise identify a branch depending on #29 explicitly, without merging #29 to main or silently duplicating it.
3. First run existing source build + mGBA smoke tests and document baseline. If ROM build/mGBA runtime unavailable, create deterministic harness and precise commands but **do not call it an emulator screenshot**. No screenshot can be owner-approved until the actual ROM is rendered.
4. Identify and document exactly when map palettes, sprite palettes, weather gamma, fades, and menu/battle palette restoration occur. Decide whether a single compositing hook or controlled recalculation is safest; confirm no double tint.
5. Choose temporary, reviewable candidate values in a `prototype` or test-only config. **They are candidates, not approved values.** Keep them labeled A/B (and C if informative), with all numerical components recorded. Prefer small meaningful variance around the moderate-blue Classic direction: e.g. A standard moderate blue, B subtly cooler/darker, possibly C lighter. Do not infer that variants A/B imply final approval.

### 5.2 Deterministic emulator capture requirements

Use mGBA actual ROM execution, not a static palette transformation script. Create an executable, documented capture command (prefer extending existing `tests/emulator` infrastructure or a clearly scoped script) to:
- Fix the in-emulator RTC to chosen **hour and minute** or use a test-only read override, with production RTC path unchanged; include tests precisely around 05:59/06:00, 06:59/07:00, 17:59/18:00, 18:59/19:00. No dependency on developer system timezone.
- Set up real in-game camera/player map state at canonical recognizable, reproducible coordinates, or script safe travel to them using native save/teleport paths. Do not invent a location snapshot and label it real gameplay.
- Render at native GBA **240x160** frame buffer before any scaling. Save lossless screenshot PNG (or native lossless PPM plus PNG conversion) with original 240x160 bytes represented exactly. The optional existing `LIGHTNING_MENU_CAPTURE=/tmp/menu.ppm` is MENU ONLY and is not evidence of outdoor lighting capture: a new field-frame capture method is needed.
- Capture at steady state after field transitions and weather animation settle; report emulated frames, RTC, map ID, map type, chosen weather, palette variant, Git SHA and checksum in a manifest. Do not compare incompatible scene positions or weather states in A/B panels.
- Keep frame capture itself from changing gameplay or palette state; assert same maps/coordinates/sprites across variant comparisons.
- Render labeled, nearest-neighbor enlarged contact sheets for visual review, with original 240x160 files retained; compare side-by-side against the actual same-frame unfiltered daytime baseline. Give a text description of each actual frame and frame hash. Contact sheets may be created outside ROM using actual screenshots; they are not themselves emulated frames.
- Save review files to local output folder excluded from git, such as `/tmp/lightning-lighting-review/` or a user-specified `--out` path. Do not commit ROMs, save states, proprietary assets or generated binary captures into source. If a product/workflow permits sharing PNG output, provide the actual PNGs to owner via that supported channel; report exact paths and manual transfer instructions otherwise. Do NOT claim screenshots were supplied via GitHub Actions or ChatGPT if no supported upload exists.

### 5.3 Review capture matrix (minimum)

| Set | Map | Time / state | Requirement |
| --- | --- | --- | --- |
| CAL-01 | Pallet Town | 06:30, 12:00, 18:30, 21:00; plus Day 12:00 unfiltered control | First **four-period** A/B comparison, same position & camera; visible sprite/terrain readability |
| CAL-02 | Route 1 (or another source-verified ordinary route) | 06:30, 12:00, 18:30, 21:00 | Open-sky policy consistency; map entry/return no palette leak |
| CAL-03 | Viridian Forest | 12:00 and 21:00 (plus at 06:30/18:30 to verify two-state grouping) | Covered only Day/Night, visibly shaded relative to open-sky, not four different variants |
| CAL-04 | Lavender Town | 06:30, 12:00, 18:30, 21:00 | Functional day uses exactly Dusk look; functional night exactly Night look |
| CAL-05 | Pokémon Mansion 1F, 2F, 3F, B1F | Representative 12:00 and 21:00 | Fog all four floors, time-independent, original interior lighting otherwise |
| CAL-06 | Representative ordinary indoor building and a Flash-dependent cave | 12:00 and 21:00, Flash off/on where applicable | No outdoor tint, preserve cave darkness and Flash |
| CAL-07 | Approved eligible outdoor weather map | Rain and one of Sandstorm/Snow-Hail at 12:00 and 21:00 | Weather overlay and palette interplay, sprite legibility; no weather probability changes |
| CAL-08 | Same outdoor map | RTC invalid -> valid night recovery, field entry, battle return, menu close, save/load | Correct period/synchronization; no double tint, ghost palettes or unintended weather roll |
| CAL-09 | Ordinary open sky | Exact boundaries 05:59, 06:00, 06:59, 07:00, 17:59, 18:00, 18:59, 19:00 | Automated pixel/state classifications, not necessarily eight redundant stills |

**Minimum visual deliverables:** 4 Pallet + 4 Route + 2 Forest + 2 Lavender + 4 Mansion floor images + 2 indoor/cave + 4 weather-condition captures + candidate A/B panels and one labeled index/contact sheet. More samples are welcome if they resolve specific risks. For each map verify exact source map ID, camera/location, visuals. Do not invent a screenshot that cannot be captured; mark unavailable tests explicitly.

### 5.4 Owner approval stages (no artificial approval)

- **Stage 1:** Owner compares actual Day baseline versus first Classic-night prototype in Pallet Town and a route, then chooses or requests adjustments to blue hue, brightness, contrast, sprite visibility. **Pending.**
- **Stage 2:** Owner reviews dawn/dusk and Viridian covered presets, Lavender two-state and fog compatibility. **Pending.**
- **Stage 3:** Owner reviews weather overlays and UI/transition regression images; exact palette numeric manifest is matched to selected images. **Pending.**
- **Stage 4:** Separate implementation specification updates/production lighting PR **only after** owner explicitly approves precise palette values and the named screenshots. **Not authorized by this handoff.**

Codex may use internally selected candidate RGB/brightness numbers **for review only**; every screenshot must name its variant. Never claim owner has approved colors or that an image is original FireRed unless it is actually rendered from the ROM. If a named map/fog effect cannot be safely prototyped, provide the completed other images and an exact blocker, not a fabricated substitute.

## 6. Acceptance criteria and validation

- [ ] Clearly labeled preview-only branch, no production palette lock or unapproved merge.
- [ ] Real mGBA ROM captured as 240x160 lossless files with reproducible map/RTC/variant setup and manifest with head SHA, map ID, weather, capture state and hash.
- [ ] `CAL-01` through `CAL-09` completed or individually marked unsupported with precise technical explanation.
- [ ] Actual same-position, same-weather side-by-side Day reference and candidate nighttime alternatives; labels distinguish preview A/B from approved values.
- [ ] Functional RTC period boundaries and separate appearance boundaries correct; invalid RTC fallback and immediate recovery verified, gameplay clock unchanged.
- [ ] Viridian Forest has ONLY covered Day/Night; Lavender has ONLY Dusk/ Night; Mansion fog present regardless of time where feasible; interiors/caves/Flash unchanged.
- [ ] Real weather overlays coexist; no changes to distributions, battle outcomes, Cycling Road, special weather events or RTC evolution functionality.
- [ ] Palette transitions/fades, menu, battle start/end, building entry/exit and save/load cause no visual artifacts or double application.
- [ ] Host deterministic tests for classify-period/map policy, candidate palette bounds and restoration, and real mGBA integration frames/tests. Do not replace emulator evidence with synthetic screenshots.
- [ ] Run `scripts/prepare-baseline.sh`, `make -j2`, appropriate host suites and `python3 tests/emulator/run.py` when viable; report exact test results, runtime versions and all skips. Preserve independent baseline compare and existing assertions.
- [ ] Neither ROM nor proprietary asset/save-state content committed/uploaded; no weakening CI or rebaselining assertions to hide errors.
- [ ] PR description includes real capture file list/transfer means, contact sheet, stable reproduction commands, test outcomes, current weather PR dependency and **owner visual approval outstanding**.

## 7. Open questions / blockers

- **Final color values / prototype choice:** explicitly unresolved; this task generates evidence so owner can decide. No question to owner is required before prototyping.
- **Exact palette hooks, screenshot extraction and fog asset API:** Codex source research required; implement safe minimal prototype if technically feasible. Do not invent APIs or silently import Emerald code.
- **Dependency on #29:** branch strategy must account for current unmerged dynamic weather implementation. No automatic merge and no assumptions that weather integration is on `main`.
- **Distribution of image files from cloud Codex to owner:** depends on Codex environment. Produce lossless files and contact sheet, then use any supported PR/attachment/file return path; failing that, provide reproducible commands and exact output paths with explicit limitation. No false claims about image delivery.
- **Exact palette/transition approval:** owner must inspect screenshots and approve; this is intentionally outside preview implementation authorization.

## 8. Codex delivery requirements

On a **separate gameplay-prototype topic branch from the current reviewed base**, implement preview-only lighting/capture functionality following `AGENTS.md`. Open a separate reviewable prototype PR and link this spec PR. Publish precise source diffs, images/manifest via a supported sharing mechanism, tests, limits and blockers; do not merge automatically or assume final palette approval.

Keep this **specification PR separate from the future prototype PR**. When unmerged, the specification is available on branch `docs/lighting-emulator-approval-handoff-20261105` but not assumed to be on `main`.

## 9. One-line Codex handoff

`Read implementation/lighting-emulator-preview-approval.md from branch docs/lighting-emulator-approval-handoff-20261105 in InternetNinjacy/pokemon-lightning-yellow. Follow AGENTS.md; implement ONLY the approved reversible lighting prototype and real mGBA screenshot capture workflow, run the relevant tests, open a separate reviewable PR, and report any technical blockers. The specification PR is not yet merged; no final palette values or gameplay merge are approved.`
