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

## Personal build instructions — local only; not a certified release

These are source-build instructions for a user who has the necessary rights to build and use the inputs. GitHub Actions and this PR must **never publish the ROM**. GitHub access may require authentication if the repository is private. Follow `INSTALL.md`, `README.md` and `docs/UPSTREAM.md` before the first build; avoid downloading any reference FireRed ROM or BIOS.

On supported Ubuntu/Debian (including a supported Windows WSL environment), after installing the upstream toolchain prerequisites:

```sh
git clone https://github.com/InternetNinjacy/pokemon-lightning-yellow.git
cd pokemon-lightning-yellow
git checkout main
git rev-parse HEAD
sudo apt-get update
sudo apt-get install -y build-essential binutils-arm-none-eabi git libpng-dev python3 libmgba-dev
scripts/prepare-baseline.sh
python3 tests/run_pikachu_opening_tests.py
python3 tests/run_brock_weather_tests.py
make -j2
sha256sum pokefirered.gba
```

These commands are **documented, not executed by the author of this preflight**. `prepare-baseline.sh` downloads pinned compiler and build-only upstream dependencies, **not a reference ROM**; run only where authorized. `pokefirered.gba` is the local generated binary name in this repository; do not commit, attach, distribute or upload it. To obtain a byte-for-byte reproduction of the audited revision rather than a later evolving main, use `git checkout a2633c88b95c611f8e704af9ad0d27dafe85abc9` **before** preparation/build. The checksum is for comparing *your own locally built revisions*, not evidence of a certified release. Emulators must be obtained/installed separately through their official distributions. The upstream notes require clean builds before reproducibility claims.

The host source checks above are short preliminary checks only. The existing CI modified/full native and independent upstream-baseline remain mandatory in GitHub; do not skip or label them successful because these focused scripts pass. For a local full native regression on an appropriately prepared Linux environment: `python3 tests/emulator/run.py` after `make -j2`; this is optional for a personal preliminary smoke **in addition to, not instead of, required GitHub checks**. A local run alone is not proof of an uninterrupted full campaign.

## First owner playtest — eight independently reportable checkpoints

| # | What to do on an unmodified fresh save | Pass criterion / record if it fails |
| --- | --- | --- |
| 1 | Launch the local build and select New Game | No startup hang, broken dialogue or graphics preventing interaction |
| 2 | Visit Oak's Lab and select the one available starter | Pikachu Lv5 received; the other two original balls cannot grant unapproved starters |
| 3 | Attempt the mandatory first rival encounter | Real tutorial battle starts and returns to normal overworld/story state without a softlock |
| 4 | Follow the Pallet–Viridian–Route 2–Viridian Forest–Pewter route | Ordinary movement, warps, item/menu interactions, encounters and mandatory story gates are reachable without test warps or debug flag edits |
| 5 | Enter Pewter Gym and battle Brock normally | Brock's actual scripted encounter starts; accepted Snorunt/Onix party is used and the battle can conclude |
| 6 | Win the battle and speak to Brock again | Boulder Badge and TM39 outcome recorded; no duplicate badge/automatic second battle; if Bag is full, note retry behavior |
| 7 | Use a Pokémon Center, save, quit to title, and choose Continue | Same party, trainer name, badge, location, items and story progress return; no corrupt-save prompt |
| 8 | Exit Pewter through the normal next-story route | First-badge world progression and subsequent warp are accessible; note any gating anomaly |

No checkpoint above is currently claimed as a successfully executed **uninterrupted** playtest. Log: date/time, `git rev-parse HEAD`, local ROM SHA256, emulator/version, fresh or old save, checkpoint number, reproducible steps, exact dialogue/map location and screenshot where helpful (no proprietary ROM uploaded). If a hard block occurs, stop that path and report it; do not repair by toggling trainer/badge flags. Complete checkpoints 1–7 before describing the first segment as playable, and 8 before claiming the first story gate is cleared.

## Later Koga / Erika owner playtest (not a first-segment blocker)

After actual story traversal reaches their Gyms, check original NPC interactions, approved teams, trainer bag item use when naturally triggered, Singles rule, Celadon Gym Sun, victory badges (Koga Soul/Erika Rainbow), TM06/TM19, bag-full reward retry, and save/reload with no repeated battle or duplicated TMs. Do not force item activation or claim the AI must spend a healing item in every winning battle. The earlier PR-head green test evidence remains valid only for its exact source head and tested paths.

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
