# PR #233 Move Reminder fixture reconciliation — November 15, 2026

Owner requested continuation of the bundled review/correction after read-only
diagnosis. Starting exact head 516bc2224f4a04bfed51b15cccc5b20dcfd7421b, draft and
unmerged. Battle split 43730052511 modified job 132860306341 passed all existing
host guards and clean reproducible ROM builds, then failed its focused existing
Center Reminder assertion at center_move_reminder_learning.c:23 (Tackle).
Independent upstream-baseline job 132860306569 SUCCESS; focused BTL run
43730052533 SUCCESS. This is an obsolete fixture expectation, not a demonstrated
gameplay failure. Do not relabel the old CI failure as success.

## Approved requirements and existing correction reused

Issue #191 locks free service, current-species level-limited eligibility, known
move exclusion, zero/no-available cases, four-slot replacement, and persistence.
The service was merged in PR #192. Merged PR #153 separately approves the current
Eevee/Vaporeon/Espeon learnsets. Vaporeon learns Bubble/Helping Hand at Lv1,
Water Gun at Lv8, Yawn at Lv14; Tackle is absent. Eevee has Tackle/Tail Whip
at Lv1; Espeon has Confusion/Reflect at Lv1 and Swift at Lv8.

An existing specialist fix already exists in draft PR #239, source commit
331a99f2d2e5f9a22df7924d2bafa2199608df4c. Copied ONLY its existing native fixture
file byte-for-byte. PR #239 and its separate workflow are untouched. No new
scenario, workflow, game mechanic, learnset or source fingerprint was created.
All 57 CHECK calls, 16 physical button presses and three save/reload operations
are retained. The coordinated review covers the ENTIRE file:

- Vaporeon Lv7/Lv8 now exercises the approved Water Gun eligibility boundary.
- Bubble replaces obsolete Tackle expectation; Tail Whip is the actual
  pre-evolution-only exclusion for Vaporeon/Espeon under current approval.
- Eevee's no-eligible case knows its two current Lv1 moves, not obsolete Helping Hand.
- The real NPC and full-slot fixtures stage known Helping Hand so the eligible
  list remains exactly Bubble, Water Gun. Existing index1 physical menu choice,
  actual selected-slot replacement, no-cost checks and flash persistence remain.

## Evidence and limits

PR #239 exact head d9f90011d83e35868126f7e2a8ea3d67a6705d7e focused native run
43551267805 job 132314016122 SUCCESS, including a real ROM build and existing
learning/physical UI flows. Confirmed identical relevant src/include, all 19
Center map files, shared Center service, runner and physical fixture against
#233; unrelated rod-event scripts differ and are NOT imported. This focused
success is reusable evidence, not a substitute for required new-head CI.

Local existing tests/check_center_move_reminder.py PASS; git diff --check PASS.
The prior all-95-host success is reused because no host input/helper changed.
No additional local ROM build or emulator run was initiated. All existing
mandatory automatic checks must run on the new #233 head. Keep draft/unmerged;
never weaken checks, change frozen hashes or touch #229/#246/legendary branches.
Stop on unexplained failures and do not reopen Misty/Surf. No task credit.

---

# PR #233 historical correction checkpoint — November 15, 2026

Scope: owner-authorized bundled validation-only correction on draft PR #233,
branch `validation/combined-fire-ghost-trainer-projection-20261113`, starting
at exact `ed38a682ac5d8578270394b91f08cd18d2d514db`. No merge authorization.
PRs #229/#246 and legendary/policy branches are independent and untouched.

## Actual execution and correction

A complete fresh local Git checkout was obtained through the normal HTTPS
origin. Both pinned commits and original frozen sources are accessible. Earlier
reports of unavailable local execution no longer describe this environment.
Read AGENTS.md, PR #233 discussion, current ownership comments in issue #105,
and the terminal old-head Actions logs before editing.

Old-head Battle split run 43716545377: modified job 132819225507 FAILURE,
first assertion in run_brock_weather_tests.py:25; upstream-baseline job
132819225744 SUCCESS. Focused BTL run 43716545380 SUCCESS.

The EXACT Brock reconstruction (existing imports and both existing expression
chains evaluated without running CI/emulators) leaves only these residuals:

- trainer_parties.h: sParty_LeaderLtSurge, sParty_LeaderSabrina (2 records).
- trainers.h: TRAINER_LEADER_LT_SURGE, TRAINER_LEADER_SABRINA,
  TRAINER_PSYCHIC_TYRON, TRAINER_CHANNELER_AMANDA (4 records).
- No additions/deletions of records or differences outside records, for either
  before/after comparison. There are 644 parties and 748 metadata records.
- ZERO ordinary-lock or Rocket Hideout residual records. The earlier suggestion
  of a 513-record Brock residual was incorrect: those changes already belong to
  the Brock frozen source and must NOT be reverted for this comparison.
- The entire restore_ordinary_trainer_blocks() result has a different historical
  target and cannot replace Brock's frozen reference.

Approval provenance: merged PR #157 (Surge, e7b328ad6a3a92d4ccc0fb104a277fc62bd2d571),
#148 (Sabrina, b26198f0aa08e26a5c17ff0f94df5fcb8c7a801d), and #203
(Tyron/Amanda Double metadata, dbd67b61a7fa19942d1fc2998b20652d1554c69b).
Reused byte-identical existing helpers approved_two_leader_party_projection.py
and approved_four_trainer_metadata_projection.py and existing Brock caller from
5ec10a8c0b282b7c75a618eeb793ca473be6312e. They compare entire records against
pinned approved main 02240690581eb70d70219024fb7f55da2882308b before restoration.

## Computed fingerprints (original expected values unchanged)

Brock historical sources are 368b3119c7c50c251324f76573d232cdb035e8bd (before)
and 6a8b8010834e5e84c5bd00a90d217c389d9a7e53 (after).

| File/state | Old incomplete reconstruction | Corrected and original frozen SHA-256 |
| --- | --- | --- |
| trainer_parties.h after | 988db2813972b68f19bc66977c23e20d1dc57e0196f698d9e748327b51aa1ec9 | 7492be71499ad7281010b0197ee19ca5dd82341babba833ab319fa88b5ef8938 |
| trainer_parties.h before | 798a4891691ed4f1d3107db7d3cf493a70e86703e5ec762ad348b42e8a787778 | 3ef0ccce270c2e5cd854b2712b2acf8b48a32261d78875b67fb6e52985efc659 |
| trainers.h after | 90deb5315ea1d275e6bed93beeb84844258ff516ff657508f0bb5ecb9d0ad42f | cd23c29c2b19955e4b57435f88c034d45de283b8d24b69bea24ffc9b000f880e |
| trainers.h before | 6f6722ce10a7e2029d392a04e31b43fe23afa4e455b7a481d960711ccd935c0d | 251e6a22e9882641cce980c45ff01a45e5cc3da465f2e84532b31bce445ac739 |

## Coordinated existing-caller inventory and confirmed additional fixes

All 95 existing host commands registered in build.yml were inspected and executed
in their existing order. Corrections only explain exact approved later deltas:

- Special Glalie, custom Dex and typing callers: reuse the existing strict six
  complete-record PKM-009 helper already on #233. Original hashes match exactly.
- Shared pre-typing projection: apply six-profile reversal BEFORE typing reversal;
  the latter legitimately changes Vulpix/Ninetales Fire/Ghost to historical Fire.
  This repairs daycare and its downstream preservation consumers without
  accepting hybrid or unexpected species records.
- Gym Castform historical helper: same strict six-profile reversal and the
  existing strict twelve-learnset helper. The latter is copied byte-identically
  from d1e578631578628f5a015db364446854b422c944, not a new test scenario.
  Rival historical checks inherit this same exact source reconstruction.
- Twelve complete learnsets: Oddish/Gloom/Vileplume/Bellossom (#144),
  Ponyta/Rapidash (#149), Vulpix/Ninetales (#150),
  Eevee/Vaporeon/Flareon/Espeon (#153), all merged owner-locked approvals.
  After existing rival reversals these are the only 12 differing arrays, with no
  non-record difference from the original frozen file at e04252ea0783510000e634486c0f2526a736f506.

Computed six-profile/weather result (Glalie/Castform):
`e37cfe0a03344890eb7c95ddf6193e635eb726e08c7198b7b7062c715ac5f267`.
Custom Dex species result:
`9c2da1284f7859700b315abda0499334c422f2c6776d576c788fcd1e4050f71e`.
Typing/daycare whole-file result:
`10021c12ed42a009e6a7a9d75a7439a9ff802f23c972559757bbbd989e4db3d6`.
Typing non-type result:
`a2cbf1d6340fab9a9bf7aaf7e0e4c7e20fb047739bccbfa42404c0a6382b3e99`.
Castform/rival learnset result:
`d6563a88fa893d1433a02027cb13ba9574a6828a96b1a730b1d767fd7c5a0cc6`.
Every listed computed value equals its unchanged original expected digest.

## Verification, remaining work and safeguards

Final local sequence: ALL 95 EXISTING workflow host commands PASS on the combined
working tree; earlier pre-correction local failures are superseded only for
this working tree, not retrospectively relabeled. Existing registration/move
reconciliation/Museum checkers also PASS. git diff --check PASS.
No new test scenarios, game source, frozen manifests, workflow registrations,
required statuses, branch protections, ROM builds or emulator runs changed.
Whole-record equality, exactly-one-record checks and whole-file hashes remain.
No new mutation suite was created or claimed executed. Local host success does
not establish native gameplay/build acceptance or exact-new-head Actions success.

Next: inspect actual new-head mandatory Actions; keep draft and unmerged. Do not
rerun successful batches without a new change/failure. Resolve only independently
approved historical deltas; stop on an unexplained failure. No Misty/Surf
investigation or exemption, no interaction with #229/#246 or legendary branches.
Permanent accounting remains 50 task IDs, 461 earned /251 remaining /712 total.

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
