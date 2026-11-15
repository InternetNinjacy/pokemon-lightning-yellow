# WRL-001 gift ownership correction — November 15, 2026

PR #249 remains draft and unmerged. Owner authorized correction of the WRL-001
compile/delivery blockers, focused validation, and an audit for the same defect
across game gifts. Main/recovery and existing earned points remain unchanged.

## Defect and correction

The pinned agbcc compiler rejected a declaration after an executable statement in
`ScriptGiveSueEevee`. Declarations now precede statements. Separately,
`GiveMonToPlayer` overwrites OT name/gender/ID with player data; setting Oak's OT
before that call loses ownership and changes the secure-data encryption key
without re-encryption. This delivery behavior is deliberate for ordinary captures
and player-owned gifts, so the global routine is unchanged.

SUE now follows the established Mystery Vendor/Rocket rescue transaction: create
under the player key, deliver normally, locate the actual party or PC record,
XOR its encrypted secure words with old/new OT keys, then set OAK's ID/name/gender.
Oak's ID remains the complement of the save's player ID, including a player named
OAK. SUE remains Eevee Lv5 with its fixed nickname. No flags, storage policy,
weather, EXP formula, obedience formula or save layout changed.

## Game-wide source audit

| Gift path | Existing ownership and delivery | Same defect? |
| --- | --- | --- |
| Oak SUE | Previously foreign creation followed by OT-overwriting native delivery | Yes; corrected for party and PC |
| Ten Mystery Vendor gifts | Player-keyed delivery, then actual-record rekey to Mystery | No |
| Rocket Ralts/Snorunt | Player-keyed delivery, then actual-record rekey to PAIGE/JULIA | No |
| Three Ranger rewards | Final PARK OT at creation; direct party copy | No |
| In-game trades | Preset OT/trade metadata; trade-scene exchange, no ordinary gift delivery | No |
| Pikachu starter | Player-owned `ScriptGiveMon`; existing naming editor restored by PR #249 | No; player OT required |
| TANGO Tangela | Explicitly approved ordinary own OT; native party/PC delivery | No |
| Museum Aron / Water Labyrinth Magby eggs | Player-keyed creation; egg/hatch ownership rules retained | No |
| Ordinary scripted rewards (Lapras, fossils, Game Corner prizes) | `ScriptGiveMon` creates player-owned records | No foreign identity configured to lose |
| Daycare eggs | Native daycare/hatching paths; separately tagged Route 5 ownership policy | No ordinary foreign gift delivery |

Audit covered all `GiveMonToPlayer` call sites, scripted `givemon`/`giveegg`
sites, gift specials and preset-OT constructors. No additional configured foreign
gift passes through the defective sequence. Existing source assertions and native
identity tests for Mystery/PARK/Rocket are preserved; no repeat acceptance claimed
for those unchanged paths. Ordinary own-OT rewards were not silently converted to
traded gifts. Already-owned Pokémon are not rewritten; this correction applies
when SUE is delivered, including a pending claim, rather than migrating old saves.

## Focused executed evidence

Base: PR head `c82014f781f576b23fc8454ca377ca25f7902b03` plus the reviewed
WRL-001-only delta. Pinned agbcc
`da598c1d918402c42c0c0d7128ba14567f3175e9`; binutils 2.44; mGBA 0.10.5.

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/run_pikachu_opening_tests.py`: PASS.
  Required old starter/rival/flag/delivery contracts remain; new source guards
  require both actual-record destinations, post-delivery OT and secure rekey.
- `make -j2`: PASS after the correction. Original failed build remains recorded
  in PR comment 6900406495; it is not relabeled as success.
- First native gate: PASS, actual delivered OAK OT `87a9cbed` versus player
  `78563412`, OT bytes `c9bbc5ff`, native `IsTradedMon` true.
- `LIGHTNING_WRL001_ONLY=1 python3 tests/emulator/run.py`: **681 assertions PASS**.
  Party/PC plus flash save/reload for three player IDs (`78563412`, `00000000`,
  `ffffffff`), with player name OAK; intact species/level/nickname/secure-data
  validity, PID/moves/PP/Shiny persistence; full storage refusal and exact freed
  box3 slot7 retry.
- Delivered SUE drives real `Cmd_getexp`: own100 / SUE150, trainer150 / SUE225,
  boosted text ID. Real `IsMonDisobedient`: badge bounds10/30/50/70/all with
  over-bound disobedience and own Lv99 control.
- Real naming subscript and keyboard: player-owned Lv5 Pikachu retains PIKACHU
  or becomes A; both persist through flash. Source checks retain the starter's
  call and rival continuation. This is focused UI entry, not a new whole-story
  playthrough or evidence of a yes/no nickname prompt.

ROM SHA256: `7f6d5621ed0b969eb7e66c34d2b004c13e86c00f22f3a3d7c6a26044b07495a7`.
No ROM, reference ROM, BIOS or save binary is committed or uploaded to Actions.

## Preserved blockers and acceptance boundary

Original first remote failure remains: Battle split validation run43754474983,
modified job132935433549, `tests/run_new_fire_moves_tests.py:48` asserting the
historical Fire distribution guard for `src/data/pokemon/level_up_learnsets.h`.
Independent upstream job132935433210 succeeded. PR #233 owns the unrelated
history reconciliation; no Fire assertion, expected hash, or learnset is changed
here. Its later Misty timeout is also a separate investigation.

No rain-specific or full native suite was run. Owner-accepted rain, Sun,
Snow-palette/Hail and Parcel/Pokédex/Eevee delivery remain accepted; WTH-002 #248
still awaits owner snow-particle persistence confirmation on Playtest002.
WRL-010 remains queued. The new focused selector leaves all existing selectors,
ABI/invocation guards, assertions and default groups intact and adds WRL-001 to
default acceptance.

The update commit uses `[skip ci]` solely to obey the owner's explicit prohibition
on rain retesting while publishing the draft correction. Workflows and required
checks remain unchanged; skipped/pending CI is not green, waived or accepted.
Full required exact-head CI, organic gameplay acceptance, and any migration of
previously acquired SUE remain unverified. No merge or points awarded:
50 permanent IDs / 461 earned / 251 remaining / 712 total.
