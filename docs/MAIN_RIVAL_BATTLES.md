# Main-story rivals — GYM-006

Authority: [merged v2 contract](../implementation/main-rival-eight-battles.md),
specification PR25, main baseline `c960642ddbdacc1cf608d8c5081ee0ecd7cca6e9`.

## Implementation

Eight exact approved teams cover Oak, early Route22, Cerulean, S.S.Anne, Tower,
Silph, late Route22 and first Champion. The33 approved Pokémon are represented
under all24 original starter-dependent IDs (99 native party entries). Explicit
custom moves/held-item structures replace default movesets where necessary.
Unmentioned held slots are ITEM_NONE. Original slot IVs, trainer class/name,
AI flags, trainer bag items, battle type and all other metadata are preserved.
No ID or save-layout change. No new artwork, mechanics, abilities or dependencies.

Only Dratini, Dragonair and Dragonite global level-up tables change: Tackle1,
ordinary Twister12, Dragon Breath22 replacing Dragon Rage. Evolved Lv1 Twister
and every other entry remain. Original30/55 evolution methods remain unchanged.

Every original event script is byte-identical, including optional Route22,
Oak's tutorial, starter branches, game-clear flags and Champion/rematch routing.
All three existing starter values0/1/2 resolve to equivalent approved parties.
The separate Pikachu-player starter feature is not implemented here. If that
future feature introduces a different VAR_STARTER_MON value, it must supply
and test the permitted dispatch fallback. All Champion rematch teams/data remain
unchanged; only first Champion teams change.

## Validation and limits

`python3 tests/run_rival_tests.py` checks exact approved data and the frozen
block overlays. Reversing only those blocks restores original SHA256 hashes;
all other bytes in these files are protected. Old-feature preservation hashes
remain unchanged through the explicit normalizer. Scripts, save structures,
TM compatibility and evolution tables are separately fingerprinted unchanged.
Run all `tests/run_*tests.py`, Museum static audit, modified build/clean rebuild,
and `python3 tests/emulator/run.py`. CI retains independent upstream SHA-1.

Native tests execute the actual constructor for every24 IDs/99 Pokémon, moves,
PP, held items, IV scaling and abilities, plus family learnsets/evolution lookup.
Battle fixtures invoke existing rival battle entry scripts in a controlled arena,
including the native Oak tutorial. Player fixtures are overlevelled with supplied
moves/PP; opponents are unmodified. Each opponent replacement and native victory
must be observed. This verifies battle execution, not manual moderate/hard balance.
A real full-story walk, loss/event-sequence progression, final Pikachu starter
integration, hardware/link peers and postgame playthrough remain unverified.
Static preservation of event/rematch source is not a runtime story test.

The initial tutorial fixture used a zero-filled, unterminated rival name; a valid
encoded saved name corrected the fixture without changing the game. A first
Champion fixture lost using only Dragon Claw; controlled player moves/input were
improved instead of weakening opponents or deleting assertions. Final executed
outcomes belong in the PR/ledger, separately from these design expectations.

## Recovery

Revert feature commits in reverse order from the baseline above. Existing saves
retain canonical move/species IDs, party data and progression flags. Existing
player Pokémon do not retroactively learn new moves; current learnset services
and future leveling use the new tables. Do not erase saves or widen IDs to roll
back. Keep the gameplay PR separate and unmerged pending review.

## Recorded scoped acceptance

Production/test commit `e4068bf0a0624c6d2df37631c11cc98b0350ff42`:
14 host suites plus Museum static PASS; build/clean rebuild identical SHA256
`40a5c6606272d8791490a52de5f9182be222c40297768a00e1277b575c92364c`.
Native mGBA0.10.5 data1525 and focused all-eight battle52741 assertions PASS.
[PR27](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/27)
maintains full regression outcomes and
[live independent CI](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/27/checks).
These local figures are not substituted for GitHub conclusions.
