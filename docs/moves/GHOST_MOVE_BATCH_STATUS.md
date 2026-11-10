# Appended Ghost moves — BTL-005-M3C

Branch `feature/approved-ghost-move-batch`, child of Fire PR80/source9ab273ce.
Source implemented; not merged or production-complete while full/clean/CI pending.

| ID | Name | Category | Power/accuracy/PP | Approved effect |
|---|---|---|---|---|
|358|Soul Drain|Physical|50/100/15|half actual damage drain; contact|
|359|Weary Touch|Physical|75/100/15|22% sleep secondary|
|360|Dream Haunt|Special|55/100/15|double base power against sleep|

All are Ghost, normal priority, selected opponent. No distribution is invented.
Soul Drain reuses native Absorb with contact; Weary Touch uses appended effect214,
setting native MOVE_EFFECT_SLEEP before the ordinary hit script; Dream Haunt uses
ordinary Hit with a scoped base-power condition in CalculateBaseDamage, shared by
AI. Dream Haunt neither drains nor requires sleep to hit nor wakes the target.
New-move strict secondary probability from80 applies to22; legacy moves unchanged.
Contact/Protect/Mirror/King's Rock flags follow Absorb+contact/Lick/Shadow Ball as
technical translations. IDs0–357 and save/wire layouts remain fixed; no new state.

Approved original-animation composites: Lick+Mega Drain, Lick+Hypnosis and
Night Shade+Dream Eater. Inline original bodies and shallow original helper calls
preserve FireRed's single return slot. No new artwork/palettes/backgrounds or
interpreter changes; no fixed damage, paralysis, draining or sleep-only failure
mechanics are imported just because those visuals are reused.

Frozen eight-file delta restores exact Fire-parent source before historical
normalizers; every old digest remains. Explicit already-normalized input in the
Thunder Wave audit avoids applying later source layers twice, while retaining its
exact before/after digest checks. No regression baselines/assertions are rewritten.
Catalog fixtures derive actual appended-pointer count; all29 host suites and3
checkers PASS. Target build PASS; native focused331 PASS: physical46/68 and special8
base/AI damage, asleep Dream Haunt14 (double power, not final damage), actual Single/
Double composite playback, Soul Drain heal/Liquid Ooze, Weary Touch sleep/plain,
Dream Haunt awake/asleep/no heal/no wake, normal immunity, onePP/selected target.
Focus flags now select only their requested tests; default full mode retains all
old and new groups. Full30-group native/clean/GitHub CI pending. Not full campaign,
all ability/item combinations, pixel approval, real link peers or hardware testing.

```
source /tmp/lightning-env.sh
make -j2
python3 tests/run_new_ghost_moves_tests.py
LIGHTNING_ORIGINAL_EFFECTS_ONLY=1 LIGHTNING_NEW_GHOST_MOVES_ONLY=1 python3 tests/emulator/run.py
python3 tests/emulator/run.py
```

No EWRAM/IWRAM allocation changes; ROM-only tables/scripts and one power condition.
Current static EWRAM usage261064/262144 bytes leaves1080 bytes: future state must
be budgeted carefully; this slice allocates none. Revert the feature commit(s)
with a pre-Ghost save or remove IDs358+ before loading an older build. Older peer
builds with different move tables are not compatible/certified.

## Parent synchronization

Inherited final Fire priority translationa4681848 (QUICK_ATTACK dispatches the
same Hit script and retains finisher bonus) throughc1550cbd. Updated exact Ghost
manifest before-state to that parent; original hashes unchanged. Current-parent
29 hosts/3 checkers/build, Ghost331 and Fire517 focused native assertions PASS.
Earlier56ab candidate full816414 and clean01e548bd…19ed7 PASS predate this metadata
change; they are not final-current full acceptance. Current clean/full/CI pending.
Main81 closure/INT005 notes preserved:399 earned/313 remaining/712, no new credit.
