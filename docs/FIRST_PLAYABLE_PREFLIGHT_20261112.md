# First Playable Build — risk-based preflight and player handoff (2026-11-12)

**Status: NOT certified / NOT a ROM distribution authorization.** This is a scoped, evidence-linked readiness gate for the owner to try a fresh personal new-game save, not a release or a new gameplay design. The original **712-point / 50-task** tracker remains **461 earned, 251 remaining**; no points awarded here. Reference main `a2633c88b95c611f8e704af9ad0d27dafe85abc9` (Koga + Erika controlled integration PR #221). Recheck current main before use.

## Minimal owner playtest proposal
- **First target:** a fresh game, obtain Pikachu, play the early story through Brock, enter the next town and save/Continue/reload. This is a bounded practical smoke test; it does **not** assert that all Kanto progression is ready.
- If the first segment succeeds, extend through the next Gym and then the later Koga/Erika areas, logging actual playthrough reachability rather than assuming scripted warps prove it.
- Preserve the tester's save and note ROM/source revision, emulator/hardware, starting fresh vs existing save, time, party, location, badges and last interaction. Never instruct uploading/distributing a copyrighted ROM or save containing private data.
- On critical failure, capture a short sequence of reproducing buttons/locations and stop that path rather than modifying flags to skip it. Continue independent approved work.

## Evidence reuse: no redundant new Gym test suites
| Scope | Evidence already present | What still is NOT established |
| --- | --- | --- |
| Koga/Erika source | PR #221 exact head `6dc6f5edffbc283d7b3896dff91686d9437a490b` CI #43350775783 both modified/upstream SUCCESS, focused CI #43350775524 all four SUCCESS; approved source-contract tests `tests/check_koga_approved_battle.py`, `tests/check_erika_approved_battle.py`; Erika exact native constructor in `tests/emulator/ordinary_trainers.c` | Natural route to leaders, actual battle victory/badges, TM collection with bag full, flash/Continue, manual experience |
| Koga original Gym progression | `data/maps/FuchsiaCity_Gym/map.json` links placed leader NPC to `FuchsiaCity_Gym_EventScript_Koga` in `scripts.inc`; script has native Singles, `FLAG_DEFEATED_KOGA`, `FLAG_BADGE05_GET`, item-space gated TM06, `FLAG_GOT_TM06_FROM_KOGA` | Real contiguous playthrough and persistence of that specific interaction |
| Erika original Gym progression | `data/maps/CeladonCity_Gym/map.json` links the leader NPC to `CeladonCity_Gym_EventScript_Erika`; `scripts.inc` contains Singles, `FLAG_DEFEATED_ERIKA`, `FLAG_BADGE04_GET`, item-space gated TM19, `FLAG_GOT_TM19_FROM_ERIKA`; map uses `WEATHER_DROUGHT` | Natural Cut-tree access, weather in actual leader battle, victory/Continue |
| Proven analogs | `tests/emulator/gym_castform.c` already executes native Gym encounter and weather/battle/flash flows; `tests/emulator/blaine.c` tests real leader battle, badge/TM, flash Continue and no duplicate; Koga/Erika retain original script mechanism | Cannot substitute analogous scenario for a new changed path, save layout, progression or release-wide result |
| Starting story / persistence | Merged WRL-001 starter flow and existing native gift, daycare, weather and flash tests; `tests/emulator/wrl_oak_eevee_physical.c` explicitly stages Oak's pending gift | Full fresh-save, uninterrupted Oak/rival/story progression |
| Wild encounter access | `tests/emulator/wrl_route_grass.c` proves selected local D-pad grass cases, explicitly says a controlled warp is not whole-route access | WRL-002/009 true Cascade Badge + Cut tree + Route 8 walking, remaining 50 map checks and 22 non-wild family sources; keep issue #155 live |

## Mandatory preflight before providing a personal build
1. **Current source and workflow:** verify exact main SHA and no known release-blocking regression; check `.github/workflows/build.yml` mandatory modified native and independent upstream-baseline jobs **for that exact main commit**. Do not equate green PR-head CI with main-push CI. GitHub connector `fetch_commit_workflow_runs` exposes only PR-triggered runs; a client that can list **push** runs is needed for main-push readback. PR #221 premerge was green, but the main-push CI on `a2633c88...` remains **unverified** as of this document.
2. **Minimal smoke:** verify actual game startup/new save, Oak's Pikachu, mandatory rival/parcel flow, one ordinary trainer, Brock encounter/victory and first badge, healing/PC, save, title Continue and reload with story/badge/party state intact. Reuse preexisting tests when they prove exactly the same mechanism; do not create one elaborate automated test per Gym. Mark any unexecuted manual path as **owner playtest pending**.
3. **Stop conditions:** launch crash, save-data corruption, forced progression softlock, inaccessible mandatory route, lost key quest flag, bad weather/event persistence on Continue. Do not disable gates, inject trainer victory flags or invent encounters to produce a green result.
4. **Playtest artifact policy:** no ROM/BIOS, licensed assets or proprietary save states committed to Git or uploaded to Actions. User's lawful, locally built copy and chosen emulator/hardware must be handled outside repository artifacts. No link/hardware/complete-game claim until independently tested.
5. **Regression/risk policy:** follow `docs/CODEX_COST_EFFICIENT_WORKFLOW.md` and `AGENTS.md`. Tier-1 reused normal party/source paths get approved exact-delta and existing evidence. New engine/save/progression and release-gating changes retain targeted native and all existing required full CI. Avoid repeated long-running CI polling and duplicate specialist PR merges.

## Gating ledger
- [x] Approved Koga/Erika party source merged in PR #221 at main `a2633c88...`
- [x] PR-head native modified + upstream baseline SUCCESS (#43350775783)
- [x] Previous merged-main native + upstream SUCCESS (#43347301763), **not** proof for `a2633c88...`
- [ ] Main-push native + independent upstream SUCCESS **at exact `a2633c88...`**
- [ ] Fresh new-game Oak/Pikachu/rival/parcel first-segment playtest
- [ ] Brock badge, save/Continue on a personal test build
- [ ] Route 8 physical Cut/tree access and remaining WRL acquisition/map checks (separate wider gameplay acceptance)
- [ ] Koga/Erika organic victory/badge/reward/weather/save experience (later-phase player playtest)

**Next move:** use GitHub Actions push run listing (not the PR-only commit-run lookup) for exact main SHA. If green, assemble a **non-distributed**, reproducible personal testing checklist/build instructions. The owner can begin a bounded playtest while unrelated unapproved or postgame content remains open, provided the critical smoke gate passes; never label this a certified release.
