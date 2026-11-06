# Saved PP compatibility and Leech Life — BTL-005-M2B

[PR43](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/43).
Branch `feature/move-pp-compatibility`, parent PR42/51df94f7. Production source
4f7f71d753d7c4840b22617b65095fd56d9b57f5. Standing owner authorization and
Leech Life's OWNER_EXPLICIT fields/evidence in approved-design-overlays.json
authorize Special/80 power/10PP. No other gameplay design is inferred.

## Effective behavior

Leech Life becomes Bug/Special,80 power,100 accuracy,10PP. Its original HP drain,
contact/protect/mirror flags, selected targeting, priority, secondary chance and
existing FireRed animation remain. Original IDs and all other move-table fields/
rows remain. Roar's damaging redesign and20→15PP change are NOT enabled here.

Valid older Leech Life records can hold15/18/21/24 current PP. On normal save
load, cap only above the current authoritative maximum10/12/14/16 (PP Ups0–3).
Zero/spent PP below the maximum stays unchanged; PP Ups are retained. The same
narrow helper recognizes Roar, using its unchanged current20/24/28/32 maximum
until its separately implemented redesign. Unrelated move PP is never repaired.

Coverage: live and serialized party, all14×30 PC slots, Four Island parents and
validated active Route5 parents. Pending eggs, mail and daycare step counters
are untouched. Route5 legacy import remains lazy: copy the complete record,
cap the active copy, preserve the original recovery record byte-for-byte. No
new saved field/version, offset change, Pokémon erasure or immediate flash write.
The next ordinary save persists the capped values.

`LoadGameSave` status ERROR can mean a healthy older slot was recovered after
the newer slot failed; normalization applies to that loaded save as well as OK.
EMPTY/INVALID/NO_FLASH do not trigger it. Unknown/corrupt Route5 storage is left
alone. Bad Egg, empty, checksum-invalid and unknown-species box records are not
changed. Inspection uses a temporary record, avoiding GetBoxMonData's side effect
of marking corrupt originals as Bad Eggs.

The per-record helper lives beside the existing private Pokémon crypto functions.
It decrypts a copy once, verifies the existing checksum, accesses the existing
substructure mapping, caps the permitted PP fields, recomputes the checksum and
re-encrypts before replacing a changed valid record. Crypto primitives themselves
are unchanged; no cipher or checksum algorithm is duplicated. No RNG or refill.

## Source and validation

Production changes:
- `src/data/battle_moves.h`: exactly Leech Life's category/power/PP.
- `src/pokemon.c`: new bounded per-record helper, reusing existing crypto.
- `src/move_pp_compatibility.c` / `include/move_pp_compatibility.h`: site traversal.
- `src/save.c`: successful/recovered normal-load hook.
- `src/route5_daycare.c`: cap active parents after legacy copying, before publish.

`python3 tests/run_move_pp_tests.py` freezes those exact deltas and authority;
all historical expected hashes remain, with narrowly scoped reverse normalization.
The older host daycare policy fixtures explicitly normalize the new cap hook;
they do not pretend to execute new crypto. Current crypto/save behavior is tested
in the actual ROM below. All20 host suites and3 static checkers PASS locally.

`LIGHTNING_MOVE_PP_ONLY=1 python3 tests/emulator/run.py`: mGBA0.10.5 **PASS20105**
guarded assertions. Includes:
- All4 move slots/4 PP-Up counts and zero/spent/at/above/legacy maximum cases.
  Exact encrypted record comparison permits only intended PP/checksum changes.
- All24 secure-block orders, varied OT keys, actual Shiny/non-Shiny identities and
  a tagged Egg; unrelated moves, Bad Eggs, corrupt/unknown/empty data unchanged.
- Full420-slot PC normalization within the unchanged ARM invocation budget;
  separate `LIGHTNING_PP_FULL_PC_ONLY=1` PASS432.
- Actual flash save/load of party/PC/both independent pending daycares, nonzero
  mail/step metadata, lazy legacy import and untouched recovery copy, unknown or
  inconsistent Route5 headers and idempotent second loads.
- Deliberately corrupt the latest flash slot, recover the older111-money save
  instead of newer222, retain ERROR status and correctly cap PP. Actual empty,
  invalid and absent-flash paths skip normalization.
- Controlled numeric Special80 damage, attacking stats, actual AI estimate, Reflect/Light Screen/burn.
- Four actual Single/Double battle turns, original animation, one PP consumed,
  half-actual-damage healing and Liquid Ooze reversal; Run, field return and flash
  persistence. Fixtures control stats/HP/types and opposing sleep/ability; these
  are not unmodified campaign battles or proof of every drain interaction.
- Actual Ether/PP Up item-effect routines and ordinary restore use10/12PP.
  These item calls are not an actual Bag UI acceptance claim.

The initial field-by-field implementation exceeded the full-PC instruction
budget. It was replaced, not given a relaxed timeout. A superseded full run was
stopped; final acceptance must use the optimized source above. A test-only flash
isolation error was also corrected: restoring CPU state does not restore flash.
Neither earlier experiment is represented as a final pass.

Optimized local full23-group suite **PASS534050** before the final one-assertion
AI addition/negative-fixture strengthening; final focused20105 PASS afterward.
Production code is identical. The final CI runs the complete latest fixtures; its
actual count/conclusions are recorded at PR43 Checks/review description, separately
from local results. Clean rebuild PASS, identical SHA256
8162003f6283a46766e453db66283287913bbd778ba171649f734d465689822c. No ROM/BIOS/save/screenshot artifacts
are committed or uploaded. Direct Drive synchronization remains pending.

## Limits, remaining scope and rollback

Real peer link, physical cartridges, every historic save variant, Quest Log
historical-party snapshots and all drain/Substitute/protect/status interactions
are not independently certified. Older or foreign link peers are not supported
by these save-load fixtures; no real handshake is claimed. Existing scene tests
remain in the full suite. Eleven original policies and all56 new moves/effects/
animations/AI/TMs remain separate work; no placeholder moves are exposed.

Revert this child PR only, retaining PR42 and parents. Leech Life returns to
Physical20/15PP, so capped records remain below that restored maximum; lost
above-cap PP is not automatically refilled. The read-only Route5 recovery copy
is preserved for recovery, never automatically re-imported. No schema downgrade
is needed for this child; broader parent rollback (e.g. Silver Wind10→5) still
requires the parent PR's PP precautions. No merge/auto-merge/release authorized.
