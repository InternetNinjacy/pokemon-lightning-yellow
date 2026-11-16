## SOW-002 active checkpoint — 2026-11-15

- Main197d85ee1a61ac319f95fa65ab298bb126fe937d verified; no workflow runs listed for that head. Protection-detail access403; do not infer absent protections.
- Branch feature/sow002-viridian-heracross-20261115, issue253: missing Forest Heracross implemented; see docs/SOW002_HERACROSS_VALIDATION.md for exact contract and remaining checks. No merge before required evidence.
- Source checks: python3 tests/check_sow002.py and python3 tests/run_sow_event_tests.py PASS. Build/native pending; focused LIGHTNING_SOW002_ONLY=1 only. Never run rain or the old broad SOW weather native selector.
- Existing PR233/249/250 retained; no Fire assertion changes, rain reruns, WRL ownership duplication or required-gate replacement. Original PR249 failure43754474983/modified132935433549 at run_new_fire_moves_tests.py:48 preserved; upstream132935433210 success. PR233 unchanged Misty2400 failure132895732192/upstream132895732475 success.
- PR251 catch125 and PR252 metadata merged at3ed6ac1e /197d85ee under specific owner direction; not a global CI waiver. PR249 remains draft.
- Remaining missing SOW001 Onix/MtMoonB2F;003 Electabuzz/Route10;005 Miltank+Tauros/Route15;006 Noctowl/Route16. Existing004/007/008 present; do not retest accepted rain. WTH002 Playtest002 snow-particle persistence awaits owner confirmation; WRL010 queued.
-50 permanent tasks461/251/712 preserved.

## Historical checkpoint (retained)

# PR251 catch125 merged — November 15, 2026

- Owner reiterated lock13 catch rates125, implement/merge and proceed after the
  unexecuted-CI/no-rain limitation had been disclosed. Normal GitHub merge with
  expected head4c89cb6b1313ac4d159f5eca41791f78ad15d77d succeeded.
- Verified merge/main `3ed6ac1e18cc774344c5f242cfdd5020e33b83b8`; PR251 merged=true,
  source tree byte-identical to approved PR head; all13 values125 and all38
  other reviewed rates unchanged. Static51-roster/scope check PASS on main.
- Fresh branches/main protected=false, mergeable_state=clean; no protection,
  status, assertion or workflow edits. This is a specific owner-directed merge,
  not global permission for unchecked merges or a claim of CI acceptance.
- Merge message [skip ci] obeys no-rain directive. Exact merged-main Actions
  list:0 runs. No full/native/upstream CI, build, owner playtest or points claimed.
- Update docs/EARLY_CATCH_RATE_REBALANCE.md and manifest to actual merged status;
  old blocked entries below are historical, superseded by this recorded merge.
- Next independent task: amend existing draft CI proposal PR250 with explicit
  label-only WRL001 opt-in so it need not be installed by a main merge. Preserve
  exact source0f693dd7, existing workflows/gates/guards and no artifacts. Prepare
  reviewable amendment only; do not activate/run before its trigger review.
- PR249 and PR233 remain unchanged/draft. Keep their Fire/Misty failures and
  owner weather/Parcel passes; #248 Playtest002 snow confirmation and WRL010
  remain pending. Preserve exactly50 IDs/461 earned/251 remaining/712 total.

---

# Owner-approved catch rates125 — November 15, 2026

- Owner supersedes the draft tiers: set all13 previously proposed species to125,
  lock the balance decision, implement/merge and continue. Merge is authorized;
  required CI/protection and the separate no-rain directive are not waived.
- Existing PR251 branch balance/early-catch-rates-through-misty-20261115.
  Base main02240690581eb70d70219024fb7f55da2882308b unchanged.
- Exactly13 catchRate values now125. Igglybuff170→125 is the explicit uniform-value
  consequence; other12 increase. The remaining38 reviewed species stay unchanged,
  including Electrike120. No encounter/stat/engine/gift/save changes.
- Updated manifest, complete51-row review and new proposal-owned source check.
  Exact125/value/scope/51-roster/preservation/accounting static check PASS;
  whitespace PASS. Original main tests/assertions/workflows are unchanged.
- Build/runtime not executed: pinned ARM assembler/compiler and libpng dev inputs
  unavailable locally; apt metadata access fails Permission denied/no packages.
  No unapproved replacement toolchain or ROM/BIOS/save artifact distribution.
- Concrete merge blocker: exact-head required modified/upstream CI is unexecuted.
  Existing broad CI includes prohibited rain tests; branch-protection read403,
  rulesets[] not proof of absence. No safe checked merge is currently available.
- Publish update [skip ci], retain pending statuses, no merge/bypass. Need an
  explicit CI/no-rain policy resolution and verified protection information to
  finish the authorized merge; do not ask for routine design approval again.
- Next work is the CI conflict (PR250 trigger proposal does not replace required
  full CI). Do not silently activate label-only or modify mandated gate topology.
- Preserve PR233/249/250 ownership and heads, accepted weather/Parcel scenarios,
  #248 owner Playtest002 snow result, queued WRL010 and50 IDs/461/251/712.

---

# Early catch-rate rebalance handoff — November 15, 2026

- Main/recovery stays02240690581eb70d70219024fb7f55da2882308b.
- Owner requests all obtainable Pokemon through Misty reviewed/rebalanced,
  especially before Brock, on a separate draft with no encounter/unrelated edits.
- Branch balance/early-catch-rates-through-misty-20261115 starts from main.
  Exactly13 catchRate numbers change in src/data/pokemon/species_info.h;
  all other species-file bytes and production paths are unchanged.
- Source audit:51 directly catchable species,17 before Brock. Pre-Brock floor190;
  later early-game floor120. No existing easier rate is reduced. Rates are
  species-wide, so later wild instances of the same13 also become easier.
- Changed: Igglybuff170→190, Teddiursa120→190, Elekid45→190, Tyrogue75→190;
  Magby45→150, Castform45→120, Smoochum45→190, Sneasel60→150,
  Onix45→120, Cloyster60→120, Ditto35→120, Farfetchd45→120, Dugtrio50→120.
- Includes optional pre-Misty Bill/Routes5/6/Vermilion/Route11/Diglett travel,
  actual Viridian Old Rod and enabled MtMoon1F Onix/Cloyster double pair.
  Good Rod badge3/Surf/Super Rod/later Cut routes excluded. Source inventory,
  not new natural accessibility acceptance; WRL002/009 ownership remains separate.
- Gifts, Museum egg, vendor pool, trades and evolution reviewed as non-capture
  channels. No unrelated species catch-rate adjustment or acquisition change.
- PKM009 PR173 shares species file but has no catchRate diff; left untouched.
  PR233 historical reconciliation, PR249 WRL001 and PR250 CI proposal unchanged.
- Read docs/EARLY_CATCH_RATE_REBALANCE.md and full per-version/slot manifest
  tests/specifications/early_catch_rates_through_misty.json.
- Executed static check tests/check_early_catch_rates.py PASS: exact numeric
  delta,51-source roster,17/34 stage floors, unchanged encounter/engine/checks/
  workflows/tracking. git diff --check PASS. No build/native/weather/broad CI.
- Publication [skip ci], balance/ branch outside existing push filters.
  Required checks pending, not waived/green. Keep draft; no merge or points.
- Old whole-file species fingerprints unchanged; need separately reviewed exact
  delta reconciliation before mandatory CI. No Fire assertion/PR233 imports.
- Preserve no-rain directive, original PR249 Fire failure and PR233 Misty2400
  failure, owner weather/Parcel passes, #248 Playtest002 snow confirmation,
  queued WRL010. Exactly50 IDs/461 earned/251 remaining/712 total.
- Next: review the13 chosen rates and global carryover; after acceptance arrange
  compliant focused build/runtime and required-CI/historical reconciliation.

---

# Codex execution handoff — verified November 7, 2026

Standing AGENTS policy55 authorizes approved implementation and verified merges.
Preserve design/asset holds, guards, provenance and security protections. No
worktrees, force pushes, ROM releases or fabricated acceptance. Do not restart
completed broad audits. Read live GitHub state before updating these references.

## Integrated main

Current integrated main: `a22aabd49d935e56bcb3573b394bcae13d60022d`.
Historical27/29/30/31/35/36/39–43/53 are merged. Main retains custom252 Dex,
typings/evolutions/RTC/daycare/vendors/Poke Balls/fossils/Ranger/rival/weather/
Gym Castform and move foundations/UI/saved PP. Do not duplicate implementations.

-55 autonomous policy;56 Roar;58 Viridian category sign.
-59 Museum Aron/Water Labyrinth Magby50% eggs; rooftop Tangela25 TANGO.
-60 six Lorelei/Bruno/Lance battles;61 Ground-only Sandstorm/Rain Zap Cannon.
-63 Pin Missile/Dragon Rage:29-group754034,25 hosts/3 checkers/clean PASS.
-66 global Electric paralysis immunity, including64 CI maintenance ancestry:
 full754679 and both head Actions42315238692/42315244202 SUCCESS.
-67 both-foe statuses/Scary Face Ghost15PP:755166 and both head CI SUCCESS.
-69 Spite/Grudge:merge3d06ebc3, full755386/25hosts/clean and both CI SUCCESS.
-68 Agatha is MERGED, not a live approval question: native stored ability selector,
 exact42 League Pokémon/168 moves and both Agatha rooms. Main acceptance evidence
 is recorded in the ledger. Earlier draft hold/test failure entries are historical.
-71 ordinary Trainers is MERGED18d245b5:500 real parties/1219 Pokémon,10 untouched
 dummies, retained130 VS variants. Source lock is
 docs/ordinary-trainers-implementation-lock.json. Do not regenerate from old57.
-75 BTL006 final acceptance and77 GYM008 dashboard reconciliation are merged.
-70 unused Jack VS Seeker map metadata, mergef9090885; both Actions42318261298
 SUCCESS. No gameplay credit; corresponding NPC/script already live on South map.
-78 bounded full-suite CI budget: verified own Actions42376310914 SUCCESS;
 native20→30min/job45→60min. No assertion/instruction/frame guard changes.

## Active implementation / coordination

-80 DRAFT feature/approved-fire-move-batch: source1951766a (later main merge6c6a089f
 preserves game source; latest test-only flash fixture repair is additional). Complete355
 Flame Fang90/100/15 PhysicalFire/flinch20/no burn;356 Flashfire40/100/30 priority+1;
357 Blaze Rush100/100/10 quarter actual damage recoil. No distribution/new artwork.
 Exact approved Crunch/Quick Attack/Hi Jump Kick +Ember commands. Engine has ONE
 animation return slot: shared Ember body inlines old hit commands, no nested calls.
 Strict new-ID `<` secondary roll; all original moves retain inclusive GenIII `<=`.
28 hosts/3 checkers/production chance numeric controls/517 native focus PASS.
 Final clean SHA a90ef37290e08dd8ed46eff2c3537e966c3e4f7874cdbe9e72bb1b5acef8b730.
 Both1951766a CI42381346263/42381341524 and /tmp/fire-final-full-native.log FAILED
 new flash fixture: old higher-counter flash survives CPU snapshots. Added strict
 negative control and isolated/restored fixture flash; no production save change.
 /tmp/fire-isolated-final-full-native.log corrected full816089 PASS, both CI
42382668646/42382673049 SUCCESS. Final premerge AI review changes only Flashfire
engine effect HIT→QUICK_ATTACK (same Hit script), preserving finisher106 vs Tackle104.
Updated focus /tmp/fire-ai-native-focus.log517 and28hosts PASS; latest CI pending. Older
 /tmp/fire-full-native.log (exec87317) predates strict chance/extra flash fixture;
 do not misrepresent its eventual result as latest-source full acceptance.
 Focus log /tmp/fire-ai-native-focus.log517 PASS; clean /tmp/fire-final-clean.log;
 host /tmp/fire-host-final.log all28 PASS. See FIRE_MOVE_BATCH_STATUS.md.
-62/65 coordination MERGED9a5d5c20;79 catalog MERGEDa22aabd4. Both79 head runs
 42379743632/42379748023 SUCCESS; source/spec/test/build input diff is empty
 through documentation-only integration2d6e9ac2. Maina22 postmerge CI42381749572 SUCCESS.
-57 CLOSED as superseded by merged71; branch/discussion retained, no candidate
 design gameplay merged by closing.74 Rocket and76 title art remain parallel drafts:
 inspect before working there.54 follower asset audit only, permission hold.
- Canonical50 tasks/712 points remain398 earned/314 remaining (55.90% provisional).
 M3A prerequisite and unmerged M3B receive no added credit. Preserve histories.

## Exact next checks

```
git status --short --branch
gh pr checks 80
tail -n 8 /tmp/fire-isolated-final-full-native.log
gh run list --branch main --limit 4
```

Toolchain: source /tmp/lightning-env.sh. Build make -j2; all28 host runners and
3 checker scripts; full python3 tests/emulator/run.py. Tests use real compiled ROM
code and mGBA, not full campaign/cartridge/real-link certification. Never weaken
4M instruction or scene frame guards. Wait for runner ROM/ABI copy and boot before
checkout/clean while native tests run. If old logs disappear, rerun, do not infer.

## Next approved work / genuine holds

1. Finish80 acceptance/merge and validate resulting main; do not restart completed audits.
2. First3/56 moves implemented on80,53 remain. Add complete contiguous batches with
 effects, descriptions, names, existing-art scripts, tests and all consumers.
 Next358 Soul Drain physical Ghost50 half-drain/contact;359 Weary Touch physical
 Ghost75 sleep22%;360 Dream Haunt special Ghost55 double power against sleep.
 Inspect approved overlays/animation reuse first. No invented distribution.
 Reconciliation now supports complete source batches; never equate data presence
 with runtime acceptance. Keep frozen355-prefix manifests authoritative.

Follower54:252 path audit, NO follower gameplay; aarant FireRed/AI asset permission
still needed. Starter Pikachu/Parcel Eevee: exact Eevee level/full-party behavior
missing. Global weather ability slots/inheritance and exact special encounter
placements/aftermath remain design dependencies. Final lighting RGB owner hold;
reversible real-emulator prototype/screenshots authorized. No time-exclusive wilds.
Do not let those independent holds block fully specified move work.

Drive writing unavailable;40 authority exports ARE available under
/workspace/attachments/lightning-authority-2026-11-03. Git ledger/dashboard is
canonical; Drive synchronization pending. Keep prior completed progress/history.

Child82 feature/approved-ghost-move-batch inherited80/a4681848 throughc1550cbd,
main81 through63be5662.29hosts/Ghost331/Fire517 native focus/build PASS on current
parent. Pre-parent-AI56ab full816414 and clean01e548bd…19ed7 PASS are historical;
current full/clean/CI pending. No new progress credit;81 adds only BTL006+1, so
current399/313 of712 (56.04%). Source current maincd9650c5. Next after80 acceptance:
finish82, then361+ exact Bug designs (read register/art, never assume official Bug Bite).

## Weather specialist checkpoint — 2026-11-08

Current scoped stack: PR118 (`feature/weather-ability-mechanics`,368b3119) → PR126 (`feature/brock-approved-weather-party`,6a8b8010) → PR127 (`feature/special-glalie-ability-persistence`, game/test head e6b699f9579e0a917d86a0ddf444571e090f16bf). Independent route-lifecycle PR113 remains separate. PR113 and PR118 full modified/upstream CI passed. PR126/127 latest CI status must be read live; do not call pending checks successful. No merge/auto-merge authorized in this assignment.

Local completed evidence: exact approved Brock Snorunt14/Onix16 moves/items/one Potion; actual Ice Body Hail healing and Overcoat reserve immunity; Glalie-only opt-in saved Snow Warning using existing encrypted growth filler/data selector89 (all old layouts/IDs preserved); actual party/full-party-PC captures, flash reload, subsequent player entry; baseline Tyranitar Sand Stream retained after actual party/PC captures. All44 combined hosts/three checkers, final combined native11864 assertions, focused individual captures5408 assertions, clean ROM SHA-256 reproducibility passed. No ROM artifacts. See PR bodies and scoped docs; full story/hardware/link/summary visual acceptance is not certified.

Resume on the existing PR127 branch without creating worktrees. Read `docs/SPECIAL_GLALIE_ABILITY.md`, `docs/BROCK_WEATHER_PARTY.md`, `docs/WEATHER_DECISIONS_REQUIRED.md`; check `gh pr checks 126` and `gh pr checks 127`, inspect/fix genuine failures. Focus commands: `python3 tests/run_special_glalie_tests.py`, `python3 tests/run_brock_weather_tests.py`, `LIGHTNING_WEATHER_MOVES_ONLY=1 LIGHTNING_SPECIAL_GLALIE_ONLY=1 python3 tests/emulator/run.py`; full weather group drops the second environment variable. Native tests require the documented prepared toolchain/mGBA; `/tmp/lightning-env.sh` exists only in the current environment.

Pending owner/design answers (already asked together): SOW-004/007/008 levels/four moves/exact approved placements/final Corphish ghost aftermath; exact16-species two-slot replacements and evolution remaps; special Corphish→Crawdaunt Drizzle inheritance. Supplied Special Overworld final handoff explicitly defers encounter balance/coordinates; issue47 explicitly defers slots. Do not invent them. Once answered, extend individual Corphish persistence with the approved evolution rule, generate/mark actual event Pokémon before battle entry, connect existing route active/resolved flags and after-aftermath visual-wait resolver, then validate installed capture/defeat/flee/loss/retry/save/RTC/boundaries. Never auto-activate overrides before a resolvable event exists. Tyranitar's native Sand Stream needs no new global assignment. Lighting/followers/other backlog remain outside this weather assignment; no global progress percentage changed.

### Weather merge execution — 2026-11-08

Owner now authorizes merging the ready weather work. Integration branch `feature/advanced-weather-integration` starts at current main e684f7e0 and combines original PRs113/118/126/127 without squash/reimplementation. Original commits are preserved; two append-only ledger conflicts retain both sides. All45 host suites/three checkers/build passed. Finish full combined native, clean reproducibility and exact-head modified/upstream CI, then merge the integration PR with a merge commit under existing protections. Original PRs can be marked integrated/superseded if GitHub does not auto-close them. Do not merge unresolved encounter designs or choose global slots; no new gameplay decision is implied. Report actual integration merge SHA/CI and keep pending decisions explicit.
