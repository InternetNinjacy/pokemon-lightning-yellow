# Pokémon Lightning Yellow — Dual-progress evidence audit

**Evidence snapshot:** 2026-11-12 at prior GitHub review; retained as a historical, non-acceptance estimation. Documentation integrated against later main without refreshing all 50 task evidence stages. **Do not interpret this snapshot as an up-to-the-minute game implementation completion estimate.** **Review artifact, not an acceptance award.**

## Summary

| Metric | Result | Meaning |
|---|---:|---|
| **Formally accepted** | **64.75% (461/712)** | Existing, unchanged, 50-task earned/remaining ledger. |
| **Evidence-backed pipeline index (provisional)** | **70.0% (498.3 planning-point equivalents/712)** | Accepted work plus a conservative visibility allowance for observed designs and staged source work. **Not merged completion, effort hours or a release estimate.** |
| **Release certification** | **0/6 final release gates verified** | No complete-game release certification has been demonstrated. Does not mean zero playable content. |

## Calculation and safeguards

For each permanent task, keep original total T and formal earned E. Set stage S to **0%** without specific evidence; **15%** for approved design only; **35%** for *partial* source implementation in open PRs; **55%** only for broad scope implementation demonstrably covered; **75%** for task-wide focused native validation; **90%** for task-wide integrated native validation; **100%** only where formally accepted. **Evidence index = SUM(T × MAX(E/T, S)) / SUM(T).** Partial source PRs are capped at 35% even if many PRs exist; it is a visibility index, **not a statement that 35% of the actual source files have been changed**. A stacked PR contributes at most the single owner-task stage; never count the parent plus each child as separate effort. No prospective stage may decrease the accepted floor. Task point allocations and chronological earned history remain untouched. Rounded per-task equivalents may not exactly sum to rounded total.

This conservative methodology retains the full 461-point formal baseline; documented unmerged implementation is visible but cannot earn acceptance credit. Source PR titles/bodies are evidence of submitted changes, **not proof that every exact PR head passed required CI**. On-head native and independent upstream checks must be verified for any upgrade to 75–90% or actual formal award.

## Task evidence register — all 50 IDs

| ID | Formal points | Evidence stage | Evidence equivalent | Evidence source / limit |
|---|---:|---:|---:|---|
| PKM-001 | 14/14 | 100.0% | 14.0/14 | PR#6 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| PKM-002 | 10/10 | 100.0% | 10.0/10 | PR#5 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| PKM-003 | 4/4 | 100.0% | 4.0/4 | PR#9 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| PKM-004 | 4/4 | 100.0% | 4.0/4 | PR#9 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| PKM-005 | 7/8 | 87.5% | 7.0/8 | PR#9 merged; historical branch evidence retained — No extra speculative stage credit above the canonical earned floor. |
| BTL-001 | 14/14 | 100.0% | 14.0/14 | PR#2 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| BTL-002 | 10/10 | 100.0% | 10.0/10 | PR#7 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| BTL-003 | 5/6 | 83.3% | 5.0/6 | PR#9 merged; historical branch evidence retained — No extra speculative stage credit above the canonical earned floor. |
| BTL-004 | 5/10 | 50.0% | 5.0/10 | PR#36 merged; historical branch evidence retained — No extra speculative stage credit above the canonical earned floor. |
| BTL-005 | 32/80 | 40.0% | 32.0/80 | Recent Earth Power PR110 merged; additional PR82/85/89/91/92/93 open and other move work pending — No extra speculative stage credit above the canonical earned floor. |
| TIM-001 | 8/8 | 100.0% | 8.0/8 | PR#3 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| TIM-002 | 8/8 | 100.0% | 8.0/8 | PR#9 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| TIM-003 | 32/32 | 100.0% | 32.0/32 | PR90 merged — Task has fully earned formal credit; release-wide checks tracked separately. |
| WTH-001 | 28/28 | 100.0% | 28.0/28 | PR139 merged; PR137/138 superseded closed unmerged; isolated diagnostic PR163 closed unmerged — Task has fully earned formal credit; release-wide checks tracked separately. |
| WTH-002 | 1/16 | 15.0% | 2.4/16 | PR #29 merged partial drought-loader and transition fix — Rendering/fog and region-wide custom visual checks remain. |
| WTH-003 | 14/20 | 70.0% | 14.0/20 | PR#29 merged; historical branch evidence retained — No extra speculative stage credit above the canonical earned floor. |
| DAY-001 | 14/14 | 100.0% | 14.0/14 | PR#10 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| DAY-002 | 8/8 | 100.0% | 8.0/8 | PR#10 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| DAY-003 | 4/4 | 100.0% | 4.0/4 | PR#10 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| DAY-004 | 5/6 | 83.3% | 5.0/6 | PR#10 merged; historical branch evidence retained — No extra speculative stage credit above the canonical earned floor. |
| GYM-001 | 0/12 | 35.0% | 4.2/12 | PRs #152, #159–160, #162, #196–197, #199–205 — Multiple approved trainer slices on open stacks. Not all Gym trainers are implemented; no head-wide CI acceptance inferred. |
| GYM-002 | 0/8 | 35.0% | 2.8/8 | PRs #145, #147–148, #151, #157–158 — Specific leader/important-battle source branches. No all-leader merge/CI acceptance inferred. |
| GYM-003 | 0/0 | 100.0% | 0.0/0 | PR#31 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| WRL-001 | 12/12 | 100.0% | 12.0/12 | PR#88 merged; branch retained for history — Task has fully earned formal credit; release-wide checks tracked separately. |
| WRL-002 | 0/20 | 15.0% | 3.0/20 | Approved access/map policy and specialist WRL-002/009 issue #155 — Staged ecology and Cut-route source overlap cannot alone justify WRL-002 implementation credit; map-policy / native grass arrival verification outstanding; 15% design-only ceiling. |
| WRL-003 | 0/8 | 35.0% | 2.8/8 | PR #193 — Specific item-price source delta, not final shop inventory, NPC placement, or badge gates. |
| WRL-004 | 4/16 | 25.0% | 4.0/16 | PR#14 merged; historical branch evidence retained — No extra speculative stage credit above the canonical earned floor. |
| INT-001 | 8/8 | 100.0% | 8.0/8 | PR#1 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| INT-002 | 8/8 | 100.0% | 8.0/8 | PR#4 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| INT-003 | 16/16 | 100.0% | 16.0/16 | PR8 original system and PR117 reconciliation merged; obsolete PR83/87/109 closed without merge — Task has fully earned formal credit; release-wide checks tracked separately. |
| INT-004 | 10/24 | 41.7% | 10.0/24 | PR#35 merged; historical branch evidence retained — No extra speculative stage credit above the canonical earned floor. |
| INT-005 | 0/8 | 0.0% | 0.0/8 | Canonical release QA contract — No external/hardware/manual release gate verified. |
| WRL-005 | 0/2 | 15.0% | 0.3/2 | Canonical roster policy — Route 5 Ditto species intended; final rates/levels and in-game encounter not proven. |
| WRL-006 | 8/8 | 100.0% | 8.0/8 | PR#11 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| WRL-007 | 8/8 | 100.0% | 8.0/8 | PR#11 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| PKM-006 | 4/6 | 66.7% | 4.0/6 | PR#11 merged; historical branch evidence retained — No extra speculative stage credit above the canonical earned floor. |
| GYM-004 | 3/3 | 100.0% | 3.0/3 | PR#15 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| BTL-006 | 16/16 | 100.0% | 16.0/16 | PR13 and PR75 merged — Task has fully earned formal credit; release-wide checks tracked separately. |
| WRL-008 | 20/20 | 100.0% | 20.0/20 | PR#12 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| PKM-007 | 6/6 | 100.0% | 6.0/6 | PR#16 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| PKM-008 | 2/2 | 100.0% | 2.0/2 | PR#22 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| GYM-005 | 16/16 | 100.0% | 16.0/16 | PR#26 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| GYM-006 | 10/10 | 100.0% | 10.0/10 | PR#27 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| GYM-007 | 5/5 | 100.0% | 5.0/5 | PR#31 merged; historical branch evidence retained — Task has fully earned formal credit; release-wide checks tracked separately. |
| PKM-009 | 0/28 | 35.0% | 9.8/28 | PRs #166–167, #171–175, #173 — 146 delegated learnset sources plus a stacked 158-profile candidate and TM/HM parity contract; not 158 complete stats/abilities/TM/HM packages. One combined candidate counted, not additive PR credit. |
| GYM-008 | 52/52 | 100.0% | 52.0/52 | PR #71 merged — Task has fully earned formal credit; release-wide checks tracked separately. |
| GYM-009 | 16/16 | 100.0% | 16.0/16 | PR #68 merged — Task has fully earned formal credit; release-wide checks tracked separately. |
| WRL-009 | 0/20 | 35.0% | 7.0/20 | Specialist ecology stack; issue #155 — Partial land encounter tables; final Surf/rod/route reachability and exact-head integration outstanding. |
| WRL-010 | 0/40 | 15.0% | 6.0/40 | Merged contract and draft PR #54 — Asset/license feasibility and follower game implementation not established; design-only 15% provisional. |
| WRL-011 | 10/10 | 100.0% | 10.0/10 | PR59 and PR74 merged — Task has fully earned formal credit; release-wide checks tracked separately. |

## Exact-head review and concurrent documentation reconciliation (2026-11-12)

- **Documentation PR #207:** the prior exact head `b174803d3228326f22ddf85ce5fb554367686faa` had [CI #43145582474](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/43145582474) terminal **SUCCESS** for both modified and independent upstream-baseline. The documentation branch is being rebased onto newer main, so this earlier success cannot certify the new head. Verify new head before merge.
- **Closeout PR #185:** already merged before this integration; all four authoritative documentation/tracker files on main matched PR #185 exactly at readback. It preserves the newer Move Reminder ledger entry and 461/251/712 ledger. PR #207 must not rewrite those files or copy stale pre-merge content.
- **Source-verified open PR examples:** PKM-009 PR #173 changes real `level_up_learnsets.h` and `species_info.h`, but is **37 ahead / 97 behind** main and cannot be treated as accepted current-main integration. GYM-001 PR #197 has a genuine `trainer_parties.h` delta but is **6 ahead / 25 behind** main; the cumulative PR #205 stack includes multiple ancestors and cannot be counted as separate task units. WRL-003 PR #193 changes `src/data/items.json` but does not establish shop inventory, badge gates, or final acceptance. BTL-005 PR #182 has substantive engine and data diffs but is **205 ahead / 196 behind** main, making an exact-head combined integration claim unwarranted. This review checked source diffs, **not exact-head terminal CI for all these gameplay PRs**.
- **WRL-002 adjustment:** prior 35% task stage relied on overlapping ecology/Cut source evidence without proving separate map-policy implementation. Reduced to 15% design-only (3.0/20 vs 7.0/20), lowering the pipeline index by 4.0 weighted equivalents. WRL-009 retains separate staged ecology credit, with no duplicated implementation credit assigned to WRL-002. The release gate remains OPEN.

The **70.0%** index is a **dated, bounded proxy for documented work activity**, not a measured proportion of executable gameplay or time expended. The verified source changes above substantiate limited stage recognition, but unresolved exact-head CI prevents advancing source branches to validated/merged stages.

## Release certification (six independent, all currently open)

1. **Complete approved source/content integration:** all 252-species data, 410-move contract, Gym and world implementations merged and scoped-accepted — **OPEN**.
2. **Wild acquisition and progression accessibility:** all 50 map checks, 22 non-wild paths, legitimate Cut interaction/grass arrival, final rod/Surf rules — **OPEN**.
3. **System-level combined native mGBA:** exact merged-head cross-feature battle, weather, events, follower, save/reload regressions — **OPEN**.
4. **Reproducible modified and independent upstream CI at final candidate SHA:** exact final candidate not nominated, so historical successful jobs do not clear this gate — **OPEN**.
5. **Natural end-to-end playthrough and visual QA:** gyms/postgame/quest progression, manual weather/art/follower/capture visuals — **OPEN**.
6. **External/link/hardware and release asset checks:** link peers, cartridge/RTC/save/power, license/sprite audit — **OPEN**.

**Historical proven subset, not final release certification:** WTH-001 merged-main modified and upstream Actions #42666557371 both report SUCCESS and PR #139 merged into `4e73c90831baa9ff989f60c3c1908844956ed1ed`. The current canonical tracker references a prior main SHA and must not be treated as a live final-candidate head. There are 60 open PR records in the search snapshot; open, stacked and stale branches can overlap substantially.

## Missing evidence and parallel ownership

- **BTL-005:** open mechanics-grouped batches #181–183 and prior move stack; exact-head CI/native outcomes, remaining move legality, combined integration. Specialist-owned, no source edits here.
- **PKM-009:** combined 158-source candidate is not all 252 complete stats/abilities/learnsets/TM legality; require authoritative source coverage and exact-head validation; avoid double-counting #166/#167/#171/#173.
- **GYM-001/002:** Viridian #197→#199→#200→#201→#202→#204→#205, Saffron #148→#203, Vermilion #159→#196; integrate in dependency order and retain separate owner-approved trainer slices. No bulk trainer completion implied.
- **WRL-002/009:** delegated specialist, actual native Cut-tree interaction and Route 8 grass arrival, 50 accessibility and 22 acquisition audit items; no duplicate manual acceptance.
- **WRL-010:** 252-follower asset/license audit, save compatibility, animation and native game proof; design does not equal playable follower.
- **WTH-002/003 and INT-004/005:** weather presentation and external/hardware release QA remain separate from accepted WTH-001 and TIM-003.

**Update policy:** retain this audit as noncanonical review until the documentation PR merges. Refresh evidence and calculate again on merge or a substantive PR change; no new earned points, no merge authorization, no specialist source modifications.
