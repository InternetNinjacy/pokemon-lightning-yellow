# First appended Fire batch — BTL-005-M3B

Implementation branch: `feature/approved-fire-move-batch`, following tooling79.
Not merged or production-complete while full/clean/CI acceptance is pending.

| ID | Approved name | Type/category | Power/accuracy/PP | Effect |
|---|---|---|---|---|
|355|Flame Fang|Fire/Physical|90/100/15|20% flinch; no burn|
|356|Flashfire|Fire/Physical|40/100/30|priority+1 hit|
|357|Blaze Rush|Fire/Physical|100/100/10|quarter actual damage recoil|

Original IDs0–354 remain fixed. Tables/names/descriptions/animation dispatch are
complete at MOVES_COUNT358 (NONE included). No learnset, TM, encounter, Trainer,
item or ability distribution is invented. Existing generic callers such as
Metronome may naturally select newly valid moves. Flashfire is a move, not Flash Fire.
New move secondary effects use a strict `<` comparison over0..99 so20 accepts
20 outcomes. Original moves retain FireRed's inclusive `<=` behavior unchanged.
The executed production chance routine is tested across all100 rolls for
0/20/22/30/50/100, with/without Serene Grace and a no-effect result. Only RNG and
effect application callbacks are stubbed in that host test; it is not a battle.

Default native selected-opponent/contact/Protect/Mirror Move flags follow Bite,
Quick Attack and Take Down respectively; native King's Rock eligibility retained
for the latter two. Engine effects are FLINCH_HIT/HIT/RECOIL. Neutral priority0 for
Flame Fang/Blaze Rush is the documented technical translation. No modern official
Flame Fang power/accuracy/burn or Hi Jump Kick crash behavior is imported.

## Existing artwork only

The approved composites reuse original Crunch/Quick Attack/Hi Jump Kick command
bodies followed by Ember visual commands. Original scripts/artwork are unchanged.
The first native test caught an animation hang: src/battle_anim.c stores only ONE
return address. Calling a shared Ember subroutine which called EmberFireHit
nested returns and looped. Fixed by inlining the existing EmberFireHit command
body three times inside the shared visual subroutine, with no nested calls or
interpreter changes. The failed test is not counted as passing evidence.

Existing scripts still clean their mon backgrounds/blending; composites wait for
visual tasks before adding Ember and returning. No new graphics/sprites/palettes/
backgrounds/audio or external source dependency. Runtime checks verify completion
and actual move behavior, not artistic approval or pixel-perfect screenshot QA.

## Preservation and tests

Frozen exact appended deltas reverse only these six source files for historical
355-entry audits; all original digests remain unchanged. Dedicated current-source
checks cover all three additions, and the strict catalog validates every actual
row. Save/wire/packed learnset layouts remain unchanged;358 fits9-bit move IDs.
No saved PP migration/refill is needed for formerly nonexistent IDs. Reverting to
an older build after acquiring appended IDs is unsafe; retain a prefeature save
or remove those moves before rollback. Real old-build link peers are not certified.

Commands:

```
source /tmp/lightning-env.sh
make -j2
python3 tests/run_new_fire_moves_tests.py
python3 scripts/check-move-reconciliation.py
LIGHTNING_ORIGINAL_EFFECTS_ONLY=1 LIGHTNING_NEW_FIRE_MOVES_ONLY=1 python3 tests/emulator/run.py
python3 tests/emulator/run.py
```

All28 host runners/3 checkers and build PASS. Pre-boundary-fix focused475 native assertions
PASS against the approved Crunch+Ember composition: physical damage/AI independent
of special stats, actual Single/Double animation completion, selected target/onePP,
no burn, flinch/nonflinch, exact recoil, Protect/miss with no crash, Flash Fire
immunity, and priority before a faster ordinary attack. New IDs/spent PP/PP Ups and exact encrypted record survive real flash save/load.
Pre-boundary-fix clean reproducibility PASS (SHA256
8c09397903f2c6bceb7c2a1e62ad6d9e8626fb5882d255cfc17a774db8877792).
Latest boundary-fixed focus/full/clean and GitHub CI pending. Do not infer410-move completion from this
slice. The reconciliation retains conservative source/runtime distinctions;
approved animation candidates do not automatically become globally verified.
