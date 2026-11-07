# BTL-005 — Move #370 Scorching Sands

Status: **approved design, implementation pending**. Base: accepted PR #96 merge `549030b3d751ff1a8373779cd0fcbd5d6de9e5f4` on `main`. This document does not itself implement or verify the move.

## Authoritative owner-approved parameters

- Move ID: **370** (`MOVE_SCORCHING_SANDS`). New imported Generation VIII move.
- Ground type, Special category, 70 base power, 100% accuracy, 10 PP, priority 0, selected target.
- Successful hit has a **30% chance to burn** the target. Use the native burn-hit effect with `secondaryEffectChance=30`, including existing type/status immunity and ordinary Serene Grace handling rather than bespoke non-approved behavior.
- Flags to translate into FireRed engine conventions; don't assume imported-game flags, learnsets, TMs or distribution.
- Owner-approved existing-art candidate: **Sandstorm + Ember composite**, marked approved **for implementation/playback testing**, not yet composition- or runtime-verified; do not add new art. This must be actually played in mGBA and checked for animation termination in Singles and Doubles.

## Implementation and mandatory review gates

Append only the new constant/data/name/description/animation without renumbering existing moves or changing save structures. Reconcile the 410-row source catalog and animation reuse references and preserve historical exact-source reverse manifests by adding a new strict delta layer, never relaxing earlier tests. Confirm no learnset, TM, trainer-party, or item distribution change. Implement host/source audits and native Singles/Doubles cases for category/AI damage, 70 power, 100 accuracy, ten PP, Ground immunities, burn/no-burn trials, burn immunity/status constraints, selected targeting, and animation completion. Require exact-head fast host, clean reproducible modified-ROM build, complete native mGBA suite, and independent upstream FireRed baseline before accepting/merging a reviewable PR. Do not count source presence or this contract as a completed move.

## Source authority

`docs/moves/approved-design-overlays.json`, `docs/moves/animation-reuse.json`, owner decisions of 2026-10-30 and 2026-11-05, the current source code and approved source-history manifests.
