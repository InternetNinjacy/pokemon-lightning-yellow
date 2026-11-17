## Updated private Playtest004 — current-main integration

This revision combines main `351932f8fe7c439b84a7c7fc555a914ba56b18e8` (PR265 complete410-move source and PR254
Heracross) with the preserved private Pichu/SUE/follower source from `b2e212b408af0e597d7b9b8d66398e3ccb53e531`.
The prior Playtest004 ROM remains unchanged. This is a separate owner build,
not a public release or accepted gameplay result.

Compilation **PASS**:16777216-byte ROM, SHA256 `26e52c729d822de37e1c53c5136f38669ed8c5e79a94728c722a2eab4e330b47`.255 follower descriptors
(252 base species plus3Castform forms),765 hash-pinned inputs checked offline.
Generated art registry SHA256 `196a74da8a37623c7178d9e976b585e8b592038ccbba2ea22af86654d061d0e3`. Linked tables have
BattleMoves0x1344 bytes (411x12),MoveNames0x19B0 (411x16),FollowerArtTable0xFF0
(255x16). No emulator/gameplay tests,simulation or CI dispatch performed.

Source comparisons preserve the exact existing Pichu/Oak-ownedSUE gift and lab
files, private species stats and learnsets, follower controller, snowfall fix,
private asset converter and resolver. Main's completed engine supersedes the
older experimental partial-move prototypes: no duplicate spread traversal, real
damage-gated VoltSwitch, shared ThunderFang and current Hurricane/Acrobatics.
Private Misty ability selections and existing experimental content remain.
Existing private legendary flags0x325..0x32C are retained alongside Route6 and
Heracross flags; no new placements or battle designs are introduced.

An actual link failure exposed the missing `map_events.o` dependency on
`include/constants/flags.h`. Adding that input forces map-object reassembly on
flag changes, fixing stale undefined private legendary symbols on build retry.
The final incremental build reassembled those objects and linked successfully.
Earlier failed logs remain local evidence, not reported as successful runs.

Private local owner output is `/tmp/ly-owner-playtest004-r2/Playtest004.gba` with
its build metadata and local attribution. Previous output under
`/tmp/ly-owner-playtest004` is preserved. Art,ROM,BIOS,saves,cache and private
permission records are excluded from Git. Workspace output is temporary and is
not a public/durable release. Owner-reported artwork permissions remain private,
not independently certified; no further permission-upload request is made.

EWRAM261068/262144 (1076 bytes headroom),IWRAM30032/32768. Owner gameplay
acceptance remains pending:252 sprite visibility/colors/movement,Steelix,party/PC,
maps,battles,410 mechanics,Pichu/SUE,new rewards and inherited experimental
legendary content. No natural-progression/hardware/link certification claimed.
The branch remains draft/unmerged;461/712 accounting unchanged.

## Current source integration checkpoint

PR265 merged main20ae65d3: complete410-move source, combined compile and static
catalog/frozen-history checks PASS; no gameplay tests or accepted points.
PR254 Heracross is reconciled on that main: resolved4AD/hide4AE and Route6 claim4AF
retained; all other flags unchanged. Prior254 native evidence remains historical.
Current combined build and source checks PASS; no CI dispatch or guard weakening.
461/712 preserved. Runtime acceptance remains owner-run.

## Current autonomous execution checkpoint

- Integrated main `421cecfc4c75f53e67690fc594aa93a8016c90ba`: PR264 combines255/259/260 with original source histories retained. New-game PC Exp.Share, Route6 post-Misty TM34 pickup, Pewter TM07/Avalanche Badge/exact dialogue. Combined native compilation PASS; readback confirms merged gameplay blobs match the compiled source. No gameplay/emulator tests or owner acceptance claimed.
- Original gift corrections Aron/Magby/TANGO are already in main; obsolete missing-implementation tracker prose corrected, no duplicate code or credit.
- PR258 `b2e212b408af0e597d7b9b8d66398e3ccb53e531`: source compiled at e94cba8e, private252-species plus3Castform registry from765 pinned inputs; local Playtest004 owner binary exists. No ROM/pixels in Git. Owner permission confirmation accepted privately; no further correspondence upload requested. Runtime acceptance pending. Follower source remains unmerged, EWRAM1076-byte margin.
- BTL-005 PR265 `7511b353fbf7fb26545c989731be7d282aa784ff` on `feature/btl005-final-catalog-through-410`: source catalog through410, static final-nine reconciliation and standalone pinned-agbcc build PASS. This source candidate preserves the old remaining-move stack and18 pending original files; it is not combined-main acceptance.
- Source merge preview against main found nine conflicts: `.github/workflows/build.yml`, `data/battle_anim_scripts.s`, `docs/moves/move-specification.csv`, `include/constants/moves.h`, `src/data/battle_moves.h`, `src/data/text/move_names.h`, `src/move_descriptions.c`, `tests/emulator/original_effects.c`, `tests/run_skitter_slash_tests.py`. No conflict markers committed or old tree selected over newer main.
- Next engineering work: resolve the catalog overlap, preserving main's BatchA implementations/animations and native water tests, add remaining host registration without duplicate/removing existing checks, retain frozen source guards, compile combined source. Inspect shared all-other-active mechanisms rather than blindly replacing PetalBlizzard's existing dispatch. Existing PR181/183 stacks remain untouched; compare them rather than duplicate.
- Resume reads: `git fetch origin main feature/btl005-final-catalog-through-410`; `git merge-tree --write-tree --name-only origin/main origin/feature/btl005-final-catalog-through-410` in the existing repository (read-only conflict preview, no checkout). Original dirty branch/index preserved; never reset it or create a worktree without authorization. Existing source candidate PR body contains exact scope/build evidence.
- Current owner reserves gameplay tests for personal ROM playtesting; no automated gameplay/emulator/diagnostic workflow dispatch. Compilation/source checks permitted. No check/security/protection changes. Source-ready approved merges authorized; missing gameplay designs are not filled by assumptions.50 permanent tasks461 earned/251 remaining/712 total unchanged. Full712 cannot be marked accepted from source presence/compilation.

## Current autonomous source integration checkpoint

Approved early-game source integration: PR255 new-game PC Exp.Share; PR259 Route6 post-Misty TM34 pickup; PR260 Pewter Hail TM07/Avalanche Badge/exact dialogue. Combined compilation pending; owner manual gameplay acceptance pending. Source integration does not award points. Preserve461/712 and all existing task history.

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

## Private Playtest003 — 2026-11-15 (current experimental branch)

- Owner requests only their playtest-note fixes, a private playable download, then manual testing before broader work.
- Experimental gameplay1a8b36de9d8689e98fcccdec38fbff98c617f1d0 starts from002153cb092. Includes13catch125+Heracross125,249SUE/OAK/Pikachu naming,254Heracross,255PCExp.Share1; keeps002snow fix and all earlier content.
- One integration conflict: separate existing opening follower selection from forcedSOCKET; preserve original special slot, run selection after native nickname editor. SUE function exactly249. No external art.
- make-j2 SUCCESS. Output Lightning_Yellow_Playtest_003.gba16MiB, SHA256eecfbe7268877993f786451a8082273bc4a53e958c1b083ad6f809a8307fdcd1. Header/compiledPC182,1,0,0/event symbols PASS. No automated gameplay/emulator/rain test. Owner runtime confirmation pending, fresh game needed.
- See docs/playtest/2026-11-15-playtest-003-notes.json and docs/EXPERIMENTAL_PLAYTEST.md. Private artifact delivered in originating chat; no ROM Git/Actions upload.
- Main197d85ee/recovery02240690/original249254255 unchanged. Tests/workflows/tracker byte-identical to002.50 IDs461/251/712; no points/formal merge/acceptance.

# Experimental Playtest 002 checkpoint — November 15, 2026

Owner continued the smallest #248 snow particle persistence fix. Gameplay commit
`b9af0975c4ba9a1b5d486c324928148686d14aa6` on experimental/playtest-2026-11-15 only.
Single runtime change: increment snowflakeTimer once in Snow_Main, enabling the
existing >18/reset/recycle path. Comment updated; no other gameplay/source rules.
`make -j2` compilation SUCCESS, no automated tests/emulator/rain retest.
Playtest 002 SHA-256 `230c9ec7d1e5376fb1489b10d5d88427cad56e7f8693670e42a83d89616081a3`,
16,777,216 bytes. Full manifest docs/playtest/2026-11-15-playtest-002-snow.json.

Next: owner observes snow persistence in Playtest 002. Do not claim runtime
acceptance or close #248 from compilation. Rain and all other explicitly reported
successful owner scenarios stay accepted: do NOT retest rain. No weakening or
deleting assertions. Main and recovery/pre-playtest-2026-11-15 stay pinned
02240690581eb70d70219024fb7f55da2882308b; do not promote the experimental build.
WRL-010 and initial Pikachu player-OT/optional nickname + SUE Oak-OT/traded EXP/
badge obedience remain queued. No earned points:50 permanent IDs,461/251/712.

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

## 2026-11-16 — Current implementation-first continuation

Main `197d85ee1a61ac319f95fa65ab298bb126fe937d`. Implemented fixed approved Brock1400/Misty3000 base prizes on `implementation/approved-brock-misty-prize-money-20261116` in `src/battle_script_commands.c`; native Amulet Coin/moneyMultiplier remains and Doubles does not double the explicit Misty base again. No tests/builds/emulator/diagnostics/failed-test investigation/merge. Source-only draft, unaccepted;50 IDs461/251/712 retained. Reserve existing PR259 Route6 flags0x0B1/0x4AF and leave all other owners' branches alone. PR260/261/262 hold Pewter/Cerulean/Vermilion prose. PR262 current1fe88d78278fffd559b549076d2f1025f7bc5dab includes full Surge postbattle source despite stale PR body. TM51 missing item/UI/compatibility and owner-approved species matrix: cannot safely substitute reward yet; do not invent learners. Next independent gameplay: choose another finalized unowned source increment, preserving all SOW/WRL/follower/Pichu/legendary/ExpShare/BTL ownership. Do not stop for unapproved tests or repeated blocker-only auditing.

## Master Control review — 2026-11-15 (latest)

- Main197d85ee unchanged. PR255 reviewed at35f003315c13f997b631774e2d7e16dd4f6223a2: exact Potion1→Exp.Share1 PASS; every existing test/script/workflow unchanged.0 exact-head Actions/check-runs/statuses; combined statuspending. Draft/mergeable=true is not acceptance. No SHA-specific owner merge authorization; no merge/test run.
- Matching proposed response rule now names owner-required GenUI.issueNewTurn and forbids Markdown/bracket/copy substitutes. Both active automation prompts updated; schedules/enabled states retained. Runtime has no exposed GenUI action/tool/documentation, so genuine continuation UI remains a capability blocker, never claimed fixed.
- PR254c984bee5 unchanged: reuse native3505 at a9198e7/run43784608441 and clean/upstream proof238ec53/run43786553053 SUCCESS. No required CI substitution or rain rerun.
- Policy245 has advanced toe22e8908a2bd69e2b6ea5f2a828aa1ebe5a8c509. Latest full43654474642: modified132629868493 FAILURE at run_brock_weather_tests.py:25, trainer_parties.h assertion; upstream132629868370 SUCCESS. Focused43654474723 SUCCESS. Read existing first failure only; no new troubleshooting/tests/edits under stop-on-new-failure policy. Ownership retained.
- SOW001/003/005/006 remain missing onmain; levels/moves/items except Onix18 are deferred. Do not invent combat designs.005 requires scripted double/party/joint persistence path;243/244 legendary work already owned, closed Surf experiment not reopened. Raw collision graphs alone do not prove route access: door/Cut/ledge/Surf/object behavior must remain separate.
- Protection403/rulesets[] inconclusive; no-rain/full-required-CI conflict unresolved.50 IDs461/251/712 unchanged; Playtest002 unchanged.

## Owner starting-PC / response contract correction — 2026-11-15 (latest)

- Main197d85ee unchanged. Independent branch feature/new-game-pc-exp-share-20261115 starts from main; only gameplay delta is gNewGamePCItems Potion1→Exp.Share1. Existing saves are not migrated. No engine, quantity, terminator or tutorial reward edit.
- Exact source-delta/ID/table/initializer preservation checks PASS; no build/native/new harness/workflow or rain tests initiated for this one-field unchanged-mechanism change. Required CI unresolved; keep reviewable/unmerged. No ROM update/release.
- Persistent response fix: removed contradictory functioning-button demand from Master Pipeline automation; added verified continuation response contract to both active project automation prompts, preserving schedules/enabled state/all unrelated instructions. Canonical START HERE front notice and Change Intake record the owner correction. Matching AGENTS rule forbids fake bracket/Markdown buttons; use only verified native control, otherwise a clearly labeled copyable Next prompt. Do not claim to have changed ChatGPT interface capabilities.
- Existing SOW002 PR254c984bee5 remains draft; native3505 a9198e7 and source238ec53 clean/upstream43786553053 PASS. Required CI/protection and233-owned history remain unresolved. No changes to254/233/249/250/243/244 or their tests.
-50 IDs461/251/712 unchanged. No point award or merge.



## 2026-11-16 — Current implementation-first handoff

Owner now directs actual approved gameplay implementation without tests, ROM builds, emulator/diagnostic runs or failed-test troubleshooting. Keep all required merge checks intact; no merge authorized. Main remains `197d85ee1a61ac319f95fa65ab298bb126fe937d`. New independent branch `implementation/route6-shock-wave-pickup-20261116` implements approved Route6 post-Misty TM34 item ball in three gameplay files; see `docs/ROUTE6_SHOCK_WAVE_PICKUP.md`. No gameplay acceptance claimed; 50 IDs and 461/251/712 unchanged. Separate TM51 eligibility/item/UI/Surge reward change remains incomplete, so Surge temporarily retains TM34 on this draft branch. Preserve all existing follower/Pichu/Heracross/legendary/Exp.Share/WRL002/009 ownership and PR245 proposed-only specialist. Next implement another unowned approved ordinary-Gym party whose exact source-ID mapping and original level slots are resolved; do not repeat Route6 work or invent SOW levels. Do not initiate tests/CI to fill acceptance gaps.


## 2026-11-16 — Implementation-first production continuation

Owner directs actual approved gameplay source without tests/builds/emulator/diagnostics or failed-test investigation; no merge. Main `197d85ee1a61ac319f95fa65ab298bb126fe937d`. Route6 TM34 after Misty is separate draft PR259 at `3a40f893d6a7862d90dd3b129f4150653df7286b`; its two new flags0x0B1/0x4AF are reserved even though absent on this independent branch. Current independent `implementation/pewter-approved-rewards-dialogue-20261116` implements Hail TM07 reward, Avalanche Badge display and all six verbatim approved Brock/Liam dialogue sections. See `docs/PEWTER_APPROVED_REWARDS_DIALOGUE.md`. Untested/unmerged; no CI acceptance/points. Preserve 50 IDs461/251/712. Existing TM reward flag bit prevents duplicate reward in old saves; no migration or old-item deletion. Do not duplicate follower/Pichu/Heracross/Exp.Share/legendary/WRL002/009 or PR245 ownership. Next choose another exact approved unowned Gym reward/dialogue slice; ordinary Saffron species-to-source-ID mapping remains unresolved, do not guess it. Keep required checks unmodified and do not merge.

## PR254 blocker follow-up — 2026-11-15 (latest)

- Main197d85ee1a61ac319f95fa65ab298bb126fe937d unchanged; PR254 remains draft/unmerged on feature/sow002-viridian-heracross-20261115. No Playtest002 update or points.
- Code238ec53ac058fc715d883336337909065f21f180: strict14 approved catch-field reverse layer,3 complete stages,17 negative controls. Every existing assertion/frozen hash retained. Remaining weather-species mismatch is exactly6 approved records and trainer projection, owned by233; no Fire/233 assertion import.
- Exact code238ec53 run43786553053 SUCCESS: modified clean proof133035404503 (source checks/two builds SHA256 match), independent vanilla proof133035404219 (pinned upstream/agbcc/twice-clean compare). Supplemental names do not replace required statuses. Existing workflows byte-identical; no native/rain executed in this run.
- Reuse3505 native PASS at a9198e7/run43784608441: all game/native runner/fixture/compiler inputs unchanged. Final evidence-only docs do not imply current-head required CI success.
- Actual blocker: authenticated required-check/protection information (API403, browser logged out) plus owner resolution of no-rain versus unchanged full required CI; historical reconciliation remains233-owned. Do not merge, relabel statuses or waive gates.
- Remaining SOW audit advanced: correct Onix-room guard local7 Grunt2 / room warp17,31; raw-collision113-cell component, not native traversal.005 needs scripted double/party eligibility/joint cleanup;003/006 exact battle/tile details deferred.
- Legendary ownership already active on24371f25df and2440d3ceb8. Latest243 full43639203124 modified132582692188 FAILURE/upstream132582692424 SUCCESS, focused SUCCESS.244 Mew experiment43631949077 failed physical water step67,4 at unchanged96-frame guard after7 dry steps and Surf08; no unreachable/root-cause claim or edits/reruns.
- Canonical50 tasks461/251/712 unchanged. Details in SOW002_HERACROSS_VALIDATION.md and SOW_MISSING_ENCOUNTER_AUDIT.md.

## SOW-002 active checkpoint — 2026-11-15

- Main197d85ee1a61ac319f95fa65ab298bb126fe937d verified; no workflow runs listed for that head. Protection-detail access403; do not infer absent protections.
- Branch feature/sow002-viridian-heracross-20261115, issue253: missing Forest Heracross implemented; see docs/SOW002_HERACROSS_VALIDATION.md for exact contract and remaining checks. No merge before required evidence.
- Source a9198e74158f5be9d6b7d35d565a7dc3cffc2f5f: source/build/native3505 PASS, run43784608441/job133029340512; focused LIGHTNING_SOW002_ONLY=1 only. Never run rain or the old broad SOW weather native selector.
- Main baseline host controls reproduce Glalie species-hash20 and world-reward trainer-projection73 failures; no assertion changes. Required CI/integration/clean/protection remain unresolved. No merge or Playtest002 update.
- docs/SOW_MISSING_ENCOUNTER_AUDIT.md covers remaining4 SOW and separate legendary issue234; current PR7 merged55665c6d does not provide scripted doubles.
- Existing PR233/249/250 retained; no Fire assertion changes, rain reruns, WRL ownership duplication or required-gate replacement. Original PR249 failure43754474983/modified132935433549 at run_new_fire_moves_tests.py:48 preserved; upstream132935433210 success. PR233 unchanged Misty2400 failure132895732192/upstream132895732475 success.
- PR251 catch125 and PR252 metadata merged at3ed6ac1e /197d85ee under specific owner direction; not a global CI waiver. PR249 remains draft.
- Remaining missing SOW001 Onix/MtMoonB2F;003 Electabuzz/Route10;005 Miltank+Tauros/Route15;006 Noctowl/Route16. Existing004/007/008 present; do not retest accepted rain. WTH002 Playtest002 snow-particle persistence awaits owner confirmation; WRL010 queued.
-50 permanent tasks461/251/712 preserved.

## Historical checkpoint (retained)


Current-main source integration: exact Heracross placement/data/script/art contract PASS; original objects,terrain,weather,battle paths and learnsets preserved. Catch projection PASS:14 approved fields,3 complete stages,17 negative preservation controls.410-move catalog and dual frozen BTL/weather checks PASS. Combined pinned-agbcc compilation PASS; private engineering ROM SHA256 `fdea969f7b086f743ccc268eb5b2d1249a8e82c279fa96f2294cd5ecba7f2559`. No gameplay/emulator tests or CI dispatch. Historical3505 native assertions remain historical, not current-head acceptance. Existing Route6 claim4AF and Heracross4AD/4AE all retained.461/712 unchanged.
