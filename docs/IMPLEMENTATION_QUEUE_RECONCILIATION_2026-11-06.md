# Implementation queue reconciliation — 2026-11-06

Reference point: `main` 15dacb95424ffd394113849d10c3aa2f78c343ba. This is a source/PR/CI metadata reconciliation, not a new ROM build or emulator playthrough. Latest old tracker SHA `d97712dc9393b5f98d73df901f8ffa70cc231309` reported 314/712 (44.10%) and still described PRs 59–61 as pending.

## Verified new merges
- PR #59 (merge 708f197a): Museum Aron Egg and Water Labyrinth Magby Egg, each 50% actual Shiny, plus Tangela Lv25 fixed TANGO. PR head b099cdff GitHub Actions run 42220799681 SUCCESS. Only gift portion of WRL-011 implemented; Rocket rescue/return quest is not included.
- PR #60 (merge 66117462): six initial/rematch League battles for Lorelei, Bruno and Lance, 32 Pokémon/128 moves, scripted native battle coverage. PR head f30b91cd GitHub Actions 42221063828 SUCCESS. BOTH Agatha battles remain untouched pending narrow native ability-slot-1 Inner Focus selector. Six of eight battle contracts are not eight of eight.
- PR #61 (merge 15dacb95): Ground Sandstorm immunity and Rain Zap Cannon accuracy. PR head 22314c35 GitHub Actions 42222085136 SUCCESS. No general species-wide abilities, route static encounter scripts, lighting visual approval, or full campaign acceptance.

### Provisional fixed-scope planning credits (low-confidence effort allocation)
Retain total712 points and all preexisting earned work. Allocate *incremental* credit only: WRL-011 +6/-6 (three gifts; Rocket quest left); GYM-009 +12/-12 (six of eight battle contracts; Agatha blocked); WTH-003 +2/-2 (Sandstorm weather coupling); BTL-005 +1/-1 (Zap Cannon special accuracy policy). This gives 335 earned /377 remaining =47.05%, versus earlier314/398=44.10%. These are deliberately provisional effort weights, not measured hours, objective feature percentage, complete QA or release readiness. If a specialist revises relative weights, update both earned and remaining together with history, do not double count scope.

## Only open PRs checked after these merges
- #54 FOLLOWERS draft: all-252 asset/engine/license inventory; NO follower code. Explicit upstream reuse/AI permission gate unresolved. Main Pikachu starter/Parcel integration also not complete. Do not dispatch bulk art/code until license/asset path resolved.
- #57 ORDINARY TRAINERS draft: 510-row source-ID design matrix only. 257 individually approved originals; 121 delegated original candidates;132 numeric variants, of which130 are actual VS Seeker linked and2 dormant Biker Goon variants. 1238 draft slots,4769 source-legal level-up moves,177 source-compatible TM alternatives. 423 weak STAB and102 short-level-up warnings. Four class conversions and one Shane Double conversion required. Full map/object wiring script NOT RUN in checkout, no trainer gameplay edit/compile/emulator test. Keep draft; direct ChatGPT GitHub audit first.
- #62 lightweight Codex workflow, documentation only; no gameplay.

## Open issues requiring reclassification rather than duplicate Codex dispatch
- #38 gifts: PR59 implemented scoped three changes; note completed and close after verifying issue's whole contract. Avoid re-implementing.
- #50 Elite Four: PR60 implements six; amend task to only Agatha two and selector acceptance, not eight.
- #47 weather: PR61 settles only two move/weather behaviors; retain route overrides, species ability slots, Gym weather/visual integration.
- #24 Ranger Base Camp: PR26 already merged; assess issue closure from merged acceptance.
- #17 older Feebas Museum QA: superseded by PR59 Aron Egg; archive/crosslink rather than implement outdated Feebas.
- #45 Celadon Hideout and #44 stolen Ralts/Snorunt: remain distinct pending gameplay, consolidate execution to avoid repeating story/audit.
- #51 lighting emulator screenshot prototype: remains legitimate bounded Codex task; exact visual approval separate.

## Dependency-first work order, Codex-efficiency policy
1. Direct ChatGPT: reconcile GitHub issues/PR scope, source IDs, current Drive authority, exact outstanding design and test gates; no Codex for clerical synchronization.
2. Direct ChatGPT: execute or independently reproduce read-only map/object/script wiring audit for #57; preserve dormant and owner-approved cases. No blind 510-team gameplay writing.
3. Direct ChatGPT + owner: resolve Agatha native ability selector scope; source-check before dispatch. Implement only the two missing arrays, not six already merged battles.
4. Engineering: selectable followers permission/assets and Pikachu opening dependency gate. No external asset import without permission.
5. Direct ChatGPT design: remaining 252 species stat/learnset and 410-move overlay, encounter level/rate tables; only complete approved contracts into implementation.
6. Codex: narrow screenshot/lighting emulator evidence prototype #51; remaining multi-file map/events/engine coding #45, fully approved weather overrides #47 and final integration/QA with build+emulator proof.
7. Final integrated regression and campaign checks; no ROM artifact redistribution.

Do not import any facts from the user's separate Pokémon project. Do not count open specification PRs as feature implementation or treat successful focused fixtures as full campaign acceptance.
