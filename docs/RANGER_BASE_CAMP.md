# Saffron Ranger Base Camp — GYM-005

Authority: [final owner-approved contract](../implementation/saffron-ranger-base-camp.md).
Implementation replaces the Dojo, preserving its map ID, layout dimensions and warps.
Four one-time Singles can be fought in any order. Four actual wins unlock Park's
four-Pokémon Double Battle; losing does not award a win. One party-only level30
foreign PARK gift follows victory. All teams, moves, bag items and owner dialogue
are frozen in `tests/specifications/approved_ranger_base_camp.json`.

## Architecture and compatibility

- Appended trainer IDs743–747 preserve all original IDs and the existing768-entry
  trainer-flag capacity. Flags0x314/315 track gift receipt and Park introduction.
  Old Dojo flags and old trainer definitions remain intact.
- `ApplyRangerTrainerData` selects approved native ability slots and raises
  Persian/Fearow friendship to255 (Return power102). Other trainers are untouched.
  Native IV parameter100 follows the existing Dojo convention; no new AI or stats.
- Regular NPC positions/facing are unchanged. One-tile sight permits any-order
  navigation; obsolete coordinate triggers are removed. Park is script-gated.
- Existing Camper overworld art matches native Ranger Nicolas; Ranger battle art
  is reused. The ordinary Building PC tile at(1,12) uses existing storage scripts.
  No new artwork, assets, animation system or licensing dependency is introduced.
- Native `CreateMon` constructs gifts directly under foreign OT ID equal to the
  player's complemented ID; fixed-width PARK OT data avoids over-reading strings.
  Native random personality/IV/ability/nature and genuine traded EXP/obedience
  remain. Direct party copy deliberately avoids the native gift wrapper's OT
  overwrite and automatic PC fallback. Full party/cancellation awards nothing.
- Continue detects legacy saved Dojo objects and reloads the map. Players saved
  on the new PC/third-ball tiles move one tile down. No save structure grows, no
  deposited/party Pokémon are discarded, and old story flags are preserved.
- EWRAM261052/262144 and IWRAM29824 usage are unchanged. New tables live in ROM;
  gift construction uses temporary native heap allocation.

## Repeatable validation

Run `python3 tests/run_ranger_tests.py`, all other `tests/run_*tests.py`,
`python3 scripts/check-pewter-museum-trivia.py`, `make -j2`, then
`python3 tests/emulator/run.py` with installed mGBA development libraries.
Run `make clean && make -j2` and compare SHA256 for reproducibility. Actions runs
these checks and separately rebuilds unchanged pinned upstream against its
original SHA-1. ROM binaries are never uploaded.

Host validation checks exact teams, all48 battle moves and gift data, dialogue
wording/font widths, map objects, native PC attributes, IDs/flags and source
preservation. Frozen reverse patches plus original SHA256 hashes work in shallow
CI checkouts without fetching Git history; unrelated snapshots remain unchanged.

Actual-ROM mGBA tests exercise native trainer construction, ability slots, PP,
friendship/Return, gift identity and EXP/obedience, dialogue/gates, party capacity,
all three deliveries, removal/save persistence, PC deposit, legacy Continue,
actual Singles victories in order3/1/4/2, Park loss/blackout/rechallenge, all four
Double replacements and victory. Opponent stats are not weakened: controlled
level100 player Pokémon provide deterministic battle coverage. These are scoped
acceptance fixtures, not proof of level-balanced manual progression.

Hardware, two-peer link interoperability, exhaustive quest-log playback,
interrupted flash writes and full-world manual progression remain unverified.
Gift ownership data is tested locally; a real link peer is a separate acceptance
boundary. Review the PR's final CI and native assertion totals for executed results.

## Recovery

Baseline is main700587a805e9f67cedec10d8778f64ade3d3f3b8; implementation begins at
7b7ba6af. Keep this feature PR unmerged pending review. Revert the feature commits
in reverse order to restore source. Existing trainer/flag entries are retained
for forward compatibility, but downgrading saves containing newly awarded gifts
is not an independently tested migration. Never overwrite user saves to roll back.

## Recorded acceptance — 2026-11-04

Validated gameplay/test revision7c9e2f5e49dc65ec4b5bb04b6464f050c568713e:
all13 host suites plus Museum static PASS; full mGBA0.10.5 run **280245**
assertions PASS, including guarded ABI/register checks. Focused data368, UI3276
and battle458 assertions PASS. Clean rebuilt ROM SHA256:
`1386257b114134228fe4b7c285e8ee5f9b53e9f8932a848cc467c59af5db695f`.
This measured modified output is not an expected vanilla checksum. Local native
screenshots of the camp and PC were inspected; no binaries uploaded.
[PR#26](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/26)
remains unmerged. [Live checks](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/26/checks)
provide final workflow conclusions separately from these local results.
