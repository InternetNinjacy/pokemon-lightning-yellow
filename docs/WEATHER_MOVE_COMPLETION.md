# Weather move completion — WTH-003 / BTL-005-M2D

Owner authority: issue47 (LY-LW weather decisions November5) and approved Move
Register lines713/788, reiterated November6 autonomous execution. Bounded scope:
Ground Sandstorm/type immunity and Rain Zap Cannon. No deferred species ability
slots, special encounter positions/levels/dialogue, new ability IDs or Hurricane
move/distribution is inferred.

## Effective behavior

Sandstorm remains Status/TM37 with existing power/PP/duration/flags/learners, but its
move type is Ground. Only Ground among either target typing slots grants automatic
Sandstorm residual immunity. Pure Rock, pure Steel and Rock/Steel take the native
maxHP/16 damage (minimum1). Existing Sand Veil, Cloud Nine/Air Lock suppression,
underground/underwater exclusions and all other native behavior remain. Hail's Ice
type protection remains. No Rock Sp.Def bonus is added; Weather Ball's native
Sandstorm Rock typing is not changed by reclassifying the Sandstorm *move*.

Zap Cannon keeps Electric/Special100 power,50 base accuracy,5PP, guaranteed eligible
paralysis, target/flags and native animation. Rain now bypasses ordinary accuracy/
evasion using the native Thunder path; outside Rain remains50, including Sun.
Cloud Nine/Air Lock suppress the Rain benefit. Earlier Protect/underground/on-air/
underwater checks and ordinary Electric immunities remain. Thunder's original
Rain/Sun behavior is unchanged. No new moves or TM51 are introduced here.

Only production changes: one Sandstorm type field in src/data/battle_moves.h;
Rain-condition addition and removal of Rock/Steel exclusions in
src/battle_script_commands.c. Exact frozen before/after hunks preserve all remaining
source. Historical checks reverse only this reviewed delta; no baseline hashes
are replaced. The83 regional map policy rows/state/RTC/save/Gym initialization and
all species abilities are byte-identical.

## Executed tests at initial commit

-22 hosts/3 checkers PASS; includes exact source/authority delta and all prior suites.
- Build PASS; clean reproducibility/full latest native/remote CI pending initially.
- mGBA0.10.5 **5389 focused guarded assertions PASS**:
  all18×18 type pairs run actual Cmd_weatherdamage; Ground-only immunity, native
  1/16/minimum tick, Sand Veil/Cloud Nine/Air Lock and Hail Ice protection.
- Actual accuracy interpreter executes1000 controlled seeds per case: normal511,
  Rain1000 despite worst accuracy/evasion, Sun511, Rain+CloudNine511,
  Rain+AirLock511. These sampled counts demonstrate executed branching, not an
  independent statistical proof of the unchanged PRNG's distribution.
- Three real Rain Zap Cannon move turns/animation: normal494 damage with native
  paralysis, Ground0/no paralysis, Protect0/no paralysis; one PP each. Controlled
  opponent moves/HP/types isolate these paths; no campaign/general AI claim.
- First interpreter fixture used unaligned busWrite32 for script operand storage;
  GBA rotated the data, so the fixture failed. Write unaligned operands bytewise
  like actual compiled script bytes. No production workaround/assertion change.
- Residual matrix tests run the real command and inspect calculated damage; they
  are not324 separately simulated end-turn HP animations. Existing full battle/
  field-weather scenarios remain in the full suite. No new screenshot claims.

Pending weather scope: special Corphish/Glalie/Tyranitar events and exact aftermath/
resolution; Ice Body/Overcoat code and per-individual persistence; deferred global
ability slot/inheritance mappings; unfinished Gym parties; Hurricane's approved
move engine. Do not mark issue47 complete. Eight other original redesign policies
and all56 additional moves remain beyond this milestone.

Rollback: restore the two source hunks/type row. No save/ID/layout or PP maximum
changes, hence no save migration. No ROM/BIOS/artifact release.
