# Roar redesign — BTL-005-M2C

Owner-approved register October30, explicit November6 autonomous execution;
`approved-design-overlays.json` retains exact evidence. Depends on merged PR43's
saved-PP compatibility; no new move IDs or save fields.

Roar is Normal/Special70 power,100 accuracy,15PP,priority0,selected target with20%
Attack-1 on a successful hit. It no longer forces switching or ends wild battles.
Uses existing `EFFECT_ATTACK_DOWN_HIT`, exact original animation, existing Protect/
Mirror Move flags, Soundproof list, TM05 and all learners. Soundproof's AI score
penalty remains correctly gated by ability; unrelated Whirlwind remains unchanged.
The description now states attack/possible Attack reduction. The effect/flags
translation is explicit metadata alongside the unchanged semantic approval.

Production delta: just Roar's row in `src/data/battle_moves.h` and its description
in `src/move_descriptions.c`. The category manifest/reconciliation reflects the
owner exception. Historical hashes are preserved through an exact reviewed reverse
normalizer, never replaced. No art, species, learners, TMs, trainers or battle
script changes. Normal has no STAB/type-system exception; Ghost immunity remains.

## Executed acceptance

-21 host suites and3 existing checkers PASS (including new exact Roar delta/authority
 audit); unchanged original move rows and all parent features retained.
- mGBA0.10.5 focused **197 guarded assertions PASS**: real Special damage9,Attack
 independence,Sp.Atk scaling17,Reflect9/Light Screen5,burn9,actual AI9; all PP-Up
 legacy caps20/24/28/32→15/18/21/24; actual flash normalization,idempotence and spentPP.
- Real Single/Double move turns and original animation execute; fixed controlled
 seeds exercise both applied/no-proc Attack drops. Selected opponent only, one PP,
 no forced switch/escape, Ghost/Soundproof immunity and actual opposing Protect.
 These tests use controlled arena stats/sleep/moves, not a full campaign.
- Clean rebuild PASS identical SHA256
 `9cc3d287edb97cb19a74f85bb204df2adcf7e435124a514bbdd166e4f1c48862`.
 The first clean tool build missed the packaged png.h include path; rerunning with
 the approved CPATH/LIBRARY_PATH completed and reproduced the same ROM. No guard
 or source behavior was changed to address that environment error.
- Full24-group native regression and GitHub CI are pending at initial commit;
 record actual final outcomes in PR/ledger before merging. Initial local full run
 began before the extra39 flash/Protect assertions; focused197 uses latest fixtures.
 No full-suite latest-fixture conclusion is inferred from that older run.

Remaining: ten other original redesign policies and56 additional moves; exhaustive
Soundproof/Protect/MirrorMove/Substitute ability interactions and real peer link/
hardware are not certified by these focused cases. All existing native groups
remain registered. No unsupported410-move completion claim.

Rollback: revert this feature while retaining parent PP compatibility. Roar restores
Status/forced-switch20PP/-6; capped currentPP stays below the restored maximum and
is not automatically refilled. No saved schema migration/downgrade required.
