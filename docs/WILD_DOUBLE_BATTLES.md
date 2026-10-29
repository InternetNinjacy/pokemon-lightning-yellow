# Lightning Yellow — Wild Double Battle Feasibility and Implementation Report

## Verdict and scope

**CONDITIONAL PASS for the capability test; production location approval and the
remaining regression coverage below are required before general gameplay use.**
The feature is retained on a reviewable feature branch, with no merges. Real ROM
execution demonstrates ordinary walking triggering doubles, independent generation,
weighted special pairs, four-battler presentation, targeting, damage/fainting,
capture of either survivor, escape, victory and overworld return. This is more than
compilation or static inspection. It is not a certification of every battle move,
every hardware platform or a complete game playthrough.

Base: `13c0ca04dd3505533a066a1f23ee0fefb960dc16` (PR #6). Recoverable local reference:
`recovery/pre-wild-double-13c0ca04`. Branch: `feature/wild-double-battle-capability`.
PR #6 and its ancestors PRs #2–#5 remain unchanged and unmerged. This branch is
stacked on #6 so the existing split, RTC, evolution and 252-species Dex are tested
together. The independent pinned FireRed baseline and its original SHA-1 are retained.

## References actually inspected

| Project / inspected commit | Relevant implementation | Compatibility finding |
| --- | --- | --- |
| [FireRed Expansion](https://github.com/cawtds/pokefirered-expansion/tree/582676ee877aa0dabd78c579d11ce70b4a473f3b) | `src/wild_encounter.c`: `TryDoDoubleWildBattle`, standard land/surf generation; `src/battle_setup.c`: `BattleSetup_StartDoubleWildBattle`, `BattleSetup_StartScriptedDoubleWildBattle`; `src/scrcmd.c`: scripted dispatch; `src/item_use.c`: two-opponent ball restriction | Closest reference, but it imports extensive Emerald Expansion battle/party/ability/follower machinery, season/time encounter indexing and config macros. Its generation temporarily copies one Pokemon because its single generator clears the enemy party. Not a drop-in patch. |
| [Emerald Expansion](https://github.com/rh-hideout/pokeemerald-expansion/tree/e5ac5e8af282536c305e4fe613a5347bd3ee7a6b) | Same three named functions in `src/wild_encounter.c` and `src/battle_setup.c`; additional `src/wild_encounter_ow.c` integration; `src/scrcmd.c` scripted dispatch | Modern `gParties`, RNG/config/ability infrastructure, follower partners, extended battle flags and scripts differ from this pinned FireRed source. Wholesale integration would endanger the existing stack. |
| [Original FireRed](https://github.com/pret/pokefirered/tree/ce6406d8c2eaf46d543c8a2606de0c3cb1951982) | Existing Lightning Yellow import, inspected source and baseline tree: `wild_encounter.c`, `battle_setup.c`, `battle_controllers.c`, `battle_main.c`, `battle_message.c`, `battle_controller_{player,opponent}.c`, `battle_script_commands.c`, `pokemon.c`, `pokeball.c`, `battle_anim_special.c` | No natural double-wild entry point. Four-battler controllers, positioning, target selection, a two-wild introduction string and most battle machinery already exist. Capture and ordinary escape still assume singles. |

Both expansion implementations prioritize Safari exclusion and, when configured,
require two usable player Pokemon. Their scripted-double functions initialize
prepared enemies and a script return callback; they do not generate natural pairs
or establish the requested encounter probabilities. We do not add a scripted-double
command because the approved feature must use random encounters.

No expansion source files, assets, engine dependency or code bundle were imported.
The patch is written against this project's APIs, with references used to identify
integration points and failure cases. Neither inspected game repository has a root
blanket license granting wholesale reuse; individual tool licenses are not licenses
for their battle code. Existing upstream notices and tool licenses remain intact.
Further third-party copying requires its own provenance/license review.

## Current architecture and changes

`src/wild_encounter.c` resolves the current map's `WildPokemonInfo` table, checks
encounter cooldown/rate, gives the original roamer path priority, selects weighted
slots and levels, and creates encrypted `Pokemon` records. Wild battle startup uses
`battle_setup.c` field locks, the existing transition task and `CB2_EndWildBattle`.
`BATTLE_TYPE_DOUBLE` selects four existing battler controllers, sprites and health
boxes. Elemental types and the move-category split are untouched.

| System / file | Change |
| --- | --- |
| `include/wild_double_battle.h`, `src/wild_double_battle.c` | ROM-only map configuration, weighted-pair definitions, eligibility, survivor and escape target helpers. No new save fields or persistent RAM. |
| `src/wild_encounter.c` | Separate party clearing from per-slot generation; generate two records without losing the first; dispatch eligible land/cave and land Sweet Scent encounters. Original single generator contract is preserved. |
| `src/battle_setup.c`, `include/battle_setup.h` | Native-style `StartDoubleWildBattle` shares the ordinary startup/return path and sets only the doubles flag. No trainer, partner or scripted-event machinery added. |
| `src/battle_main.c` | Permit normal wild-double escape using the fastest living opponent and the existing speed/attempt formula. Ignore fainted/absent trapping opponents for wild-double escape/switch checks. Preserve single and trainer/link rules. |
| `src/battle_controller_player.c` | Ball animation targets the survivor. Repair existing backwards target wrap from invalid index 4 to valid index 3, also benefiting trainer doubles. |
| `src/item_use.c` | Reject ball use while two opponents live, with an explanatory message, before inventory removal. |
| `src/battle_script_commands.c` | Catch odds, party/PC transfer, result, nickname, canonical Dex flags, registration display and species/personality graphics all use the same actual survivor. |
| `src/battle_message.c`, `src/pokeball.c` | Resolve capture nickname/animation against the survivor, including right-side captures. Two-opponent intro still shows both original opponents. |
| `src/pokemon.c` | Give each wild opponent its own existing common/rare held-item roll; original single roll distribution remains unchanged. |
| `tests/run_wild_double_tests.py`, `tests/emulator/{run.py,runner.c,wild_double.c}` | Executed host probability/policy tests plus real-ROM integration, generated ARM ABI and local-symbol ownership guards. |
| `.github/workflows/build.yml` | Required host suite added; existing mGBA step includes wild scenarios; clean build repetition and independent baseline remain mandatory. No ROM artifacts uploaded. |

No wild encounter JSON, map, tileset, move, species, typing, ability, trainer team,
weather table or save structure is changed. Existing graphics are sufficient.

## Exact encounter policy

1. Only **Route 1** and **Mt. Moon 1F** are enabled as explicit capability-test maps.
   They are experimental placements, not a finalized game roster/encounter design.
   `LIGHTNING_WILD_DOUBLES_TEST_MAPS` can be set to `0` to disable the test maps;
   their data is centralized in `sWildDoubleMaps`. Other maps keep singles.
2. Only ordinary **land encounters**, including existing cave floor encounters,
   and land Sweet Scent are affected. Encounter rate is unchanged: 100% means
   every *eligible encounter* is double, not every walking step starts a battle.
3. Safari encounters remain Safari singles. Original roamers keep their priority,
   unique HP/status persistence and single-battle behavior. A party with fewer than
   two non-egg, non-fainted Pokemon gets the original single encounter, not a
   two-against-one battle or a blocked map. These are eligibility exceptions.
4. Select the first land slot with the original 12-slot cumulative weights
   `20/20/10/10/10/10/5/5/4/4/1/1`, then its level with the original inclusive range
   rule. Apply the original Repel test to that level and the first usable player's
   level. A failed check ends this attempt; no special-pair override bypasses it.
5. Independently select the second slot and its level through the same routines.
   Do not reject duplicate species, copy the first Pokemon, or filter/redraw the
   second selection. Its nature, IVs, personality, etc. use normal generation.
6. Roll a uniform percentile using the existing RNG with rejection of outputs
   65500–65535 to avoid modulo bias. Outcomes 0–4 select **Onix + Cloyster**, in that
   order; 5–99 retain the two ordinary draws. The special pair inherits each drawn
   slot's level range; it does not introduce fixed levels or change the wild table.
   Thus 5% is conditional on triggered, eligible, Repel-accepted normal encounters,
   not on walking steps, Safari/roamer encounters or single fallbacks.
7. Preserve FireRed's original Repel model for the first draw. The second can be
   below the lead's level; filtering it would alter its species/level distribution.
   Sweet Scent keeps its original Repel bypass and uses the same 5% pair policy.
8. Each configured map can point to a distinct array of `WildDoublePair` entries.
   Each entry has two species and a percentage; totals must be at most 100. The
   remainder is ordinary pairs. Host validation checks those invariants. Enabled
   map rows always select doubles; a per-map *partial* double rate is not needed
   for this assignment and is not introduced.

Fishing, surfing, Rock Smash, scripted/static legends, trainer doubles, tutorials,
Tower ghosts and link battles keep their existing entry paths. No time-exclusive
encounters are introduced. This checkout has RTC but no time/weather encounter-table
selector or approved dynamic weather implementation yet. The hook consumes the
resolved `WildPokemonInfo *`; a future selector must pass the same applicable
pointer to both draws and retain this ordering. Compatibility with future,
unimplemented systems cannot be represented as tested.

## Battle rules and inherited behavior

- Player controls two Pokemon through the existing doubles action, move and target
  interfaces; the existing wild-opponent controller selects both player targets.
- Both wild Pokemon register as seen. Defeat one before throwing a ball. There is
  no additional capture-target menu while two remain. Rejected balls cost neither
  an item nor a completed battle turn. Capturing the survivor ends the encounter.
- Catch odds, balls, status modifiers and PC/party/nickname behavior remain Gen III.
  Master Balls are used in deterministic integration tests; ordinary-ball failure
  probabilities and every ball-specific bonus are not claimed as newly verified.
- Normal Run uses the original formula against the fastest living wild opponent.
  Existing trapping checks, Smoke Ball, Run Away, escape items and move effects are
  retained. A fainted/absent trapper must not continue to trap the party.
- Roar/Whirlwind/Teleport and opponent escape retain FireRed's whole-encounter
  termination behavior; no later-generation individual-wild-withdrawal mechanic is
  introduced. These move scenes require additional testing before broad rollout.
- Original Stench, Illuminate, Cleanse Tag and flute rate modifiers remain before
  pair generation. FireRed does not implement Emerald's full suite of overworld
  encounter abilities; this patch does not silently add those mechanics.

## Executed validation

Host tests compile and execute extracted **production routines**, using stubbed
Pokemon creation/controller state. They are not emulator battles.

- 200,000 generated pairs: **9,867 special (4.9335%)**, **24,455 ordinary duplicates**.
  Both marginal slot distributions pass seven-sigma limits. Joint independence
  goodness-of-fit: **111.89 across 144 cells**, threshold 230.
- Every accepted 16-bit percentile input executed through the production selector:
  exactly **3,275/65,500 = 5%**. Rejected inputs exercise the retry path.
- Per-map eligibility, ordinary-map/single fallback, Safari exclusion, Repel block,
  level bounds, no stale third party member, valid configuration and target helpers.
- Original rate routines: normal/Stench/Illuminate **19,982 / 10,058 / 39,989** hits
  per 100,000 identical-seed trials; Cleanse Tag/White Flute/Black Flute
  **13,462 / 30,030 / 10,058**. These test encounter triggers, not battle animations.
- Existing split, RTC, evolution and Dex host suites pass separately, with their
  original static-audit versus executed-test distinctions preserved.

mGBA uses the built ROM and matching ELF, no downloaded reference ROM or proprietary
BIOS. Fixtures create valid game Pokemon and inventory through ROM functions, then
use directional/A/B inputs for battle and Bag scenes. Some tests call real production
entry points with controlled state; only the walking case claims ordinary walking.

- 1,000 real encrypted-pair creations: **49 special**, **458 ordinary duplicates**;
  ordinary first/second Pidgey counts **477 / 466**. All generated levels stay in the
  actual Route 1 table's ranges. No species are inserted by the harness.
- Real directional walking on unmodified grass triggers a four-battler encounter.
- Four sprites/health boxes, original two-wild intro and send-out reach action UI.
- Opponent controllers 1 and 3 choose player targets 0/2 in **98/102** and **90/110**
  out of 200 real controller calls each, restored from a battle snapshot afterwards.
- Full backwards target cycles execute the bounds fix. Independently select and
  defeat each side, including two faints leading to victory and overworld return.
- Real Bag USE rejects capture with both opponents alive without item consumption.
- Natural encounter-dispatch generation supplies Onix/Cloyster to the capture scenes.
  Capture each survivor through actual Bag/ball/Dex/nickname-decline/party scenes;
  assert correct species, result, canonical Dex flag and exactly one ball consumed.
- Captured species, Dex flags and inventory survive real flash save/load after RAM
  data is cleared/changed; this is not merely reloading an emulator snapshot.
- Ordinary Run returns to the overworld; defeated Shadow Tag user no longer traps.
- Fainted/egg lead configurations cannot qualify for doubles. One-member parties
  and ordinary Route 2 enter and exit singles. Mt. Moon enters and exits doubles.
- Real Sweet Scent under Repel at RTC 00:00 and 12:00 starts doubles. Existing rain
  enters the battle and remains on return. This does not test unfinished dynamic
  weather, snow/Hail integration or visual day/night lighting.

- Full-party capture transfers right-side Cloyster to PC, verifies the actual box
  Pokemon and consumes exactly one ball.
- A co-located roaming Pokemon preempts the double policy in the real Sweet Scent
  path, enters a two-battler single, flees normally and returns to the field.
- Focused local mGBA run: **6,576 assertions passed**. Host wild suite:
  **1,246,873 assertions passed**, including a separate multi-pair 30%/20%/50%
  fixture and all 65,500 accepted percentile inputs.
- Clean local rebuild: byte-identical SHA-256
  `e56911b027dc45bf875776215e597044ea4981236275d797d1e26d77624b810c`.

The full local combined mGBA run passed **8,251 assertions**, including the earlier
RTC, evolution, flash-save and Dex scenes. GitHub run evidence is recorded in the PR. Assertion counts also include fixture/API checks; they are not counts of
complete battles.

## Performance, compatibility and remaining work

- New module: **540 bytes of object text/constants**, **0 data/BSS bytes** at the
  measured build; no new dynamic allocations or save format changes. Linker totals:
  EWRAM **261,052/262,144 bytes**; IWRAM **29,824/32,768 bytes**. Those RAM totals
  match PR #6. Existing heap/sprite machinery already supports trainer doubles.
  Available EWRAM is tight; future features must continue to check memory budgets.
- Generation adds one normal Pokemon creation and a tiny map/percentile lookup per
  triggered encounter, not per frame. No emulator stability failure remains in the
  verified scenes. GBA hardware frame timing has not been profiled.
- No save migration: existing species IDs, canonical Dex flags, roster and struct
  layouts remain untouched. This patch does not add a persisted encounter-policy bit.
- Future work touches the same encounter/startup/ball files: resolve these patches
  during integration, do not overwrite them with a wholesale expansion engine.
  Run all stacked suites after rebases and again on the final combined branch.
- Before normal gameplay release: choose final eligible maps/pairs/levels if they
  should differ from inherited table levels; remove or replace the two test rows;
  complete a sustained playthrough and real hardware/other-emulator checks.
- Additional coverage: all balls and failed throws, nickname acceptance/rename,
  item-cancel/refund on the second player's turn, player faint/replacement/whiteout,
  unusual moves (Roar/Teleport/Self-Destruct/spread attacks), all weather and
  trapping/escape abilities, encounter-frequency stress and link regression.
  Fishing/surfing/Rock Smash/Tower/Safari preservation is principally code-path
  inspection plus explicit policy tests, not a claimed full emulator playthrough.

The capability is feasible and retained, but the verdict deliberately separates
verified functionality from broader release acceptance. No known critical failure
may be waived merely to make a test pass.

## Reproduction and recovery

After the existing cloud build setup:

```sh
python3 tests/run_wild_double_tests.py
make -j2
python3 tests/emulator/run.py
# Focused diagnosis only; CI runs the complete suite:
LIGHTNING_WILD_ONLY=1 python3 tests/emulator/run.py
```

`LIGHTNING_WILD_CAPTURE=/tmp/wild.ppm` optionally captures a local battle frame;
CI does not publish it. No ROM, save, snapshot, generated executable or proprietary
asset is committed or uploaded. Revert this feature's commit(s), or restore the
feature branch to base `13c0ca04` before merging, to remove the entire experiment
without touching PRs #2–#6. Setting the test-map switch to zero disables natural
activation while retaining the fixes for review. Do not revert parent features,
force-push their branches, merge or enable auto-merge as part of rollback.
