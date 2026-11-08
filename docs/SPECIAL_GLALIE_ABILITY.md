# SOW-007: persistent individual Glalie Snow Warning

Branch `feature/special-glalie-ability-persistence`, based on PR118 head `368b3119c7c50c251324f76573d232cdb035e8bd`. Independent of Brock-party PR126 and route-lifecycle PR113. No merge authorized. Scope: issue47 individual SOW-007 capture retention, not installing the encounter.

## Implementation and storage

`SetSpecialEncounterAbility(mon, ABILITY_SNOW_WARNING)` explicitly marks a **non-Egg Glalie**. Ordinary creation never marks anything; other species/abilities and bad Eggs are rejected. Ability NONE clears the known marker. Corphish is deliberately unsupported until its evolution-inheritance contract is resolved; Tyranitar's native Sand Stream requires no new species assignment.

The exact marker `0x57A1` occupies the existing, previously unused16-bit `PokemonSubstruct0.filler`, inside the native encrypted/checksummed growth substructure. No Pokémon/BoxPokemon/SaveBlock fields or sizes change. Data selector89 is appended; original0–88 stay unchanged. Unknown nonzero filler is not overwritten. Legacy zero filler remains zero/native. Getter recognizes only the exact tag on non-Egg, valid Glalie; no generic arbitrary ability injection through the marker.

Native Get/SetMonData encryption/checksum handling is reused. GetMonAbility resolves the individual marker before normal species-slot lookup. Initial/reserve battler loading and CopyPlayerPartyMonToBattleData use the actual stored Pokémon. Summary ability name/description use the same getter. Trainer-specific approved abilities remain separate. Skill Swap/Trace do not write the stored marker.

The future actual event producer must generate Glalie, mark that generated Pokémon **before** battle entry, then start battle without regenerating it. Merely setting event flags or calling ordinary setwildbattle followed by a routine that recreates the enemy will not mark the new Pokémon. This PR installs no event object, coordinate, level, moveset or default map-weather activation.

## Executed evidence

```sh
python3 tests/run_special_glalie_tests.py
LIGHTNING_WEATHER_MOVES_ONLY=1 LIGHTNING_SPECIAL_GLALIE_ONLY=1 python3 tests/emulator/run.py
LIGHTNING_WEATHER_MOVES_ONLY=1 python3 tests/emulator/run.py
```

- All43 host runners passed. Source checks prove exact reversible changes, original data IDs/layouts and unchanged species arrays. Old frozen manifests/digests are not rewritten; the newest exact reverse adapter precedes earlier guards.
- Focused native mGBA:5324 assertions passed. All24 encrypted substructure permutations; legacy/invalid byte preservation; exact PID/OT; native PC copy, flash save/load and restoration; no bad-Egg conversion; explicit clear/reject cases and no Corphish inheritance invention.
- **Actual** Bag/Master Ball captures of marked Glalie into the party and, separately, a full-party PC passed. Both retain Snow Warning and identity after flash save/reload. A captured PC Glalie restored to the player's party starts Snow Warning in a subsequent actual battle; Run restores field weather without changing the saved ability.
- The capture fixtures use explicit test-only Route1/Lv30 data, not approved production placement/balance. They prove capture/storage/ability plumbing, not the unimplemented Route21 event or its resolution flags.
- The initial capture fixture used a foreign preset wild OT and became a bad Egg when vanilla GiveMonToPlayer replaced OT without rekeying. Corrected the fixture to use the native wild-generation/player OT contract; no unrelated OT-setting engine behavior changed. Foreign OT identities are still exercised unchanged by the independent24-permutation storage tests.
- Broader regression, clean reproducibility and CI evidence are recorded in the PR. No ROM/BIOS artifacts are uploaded.

## Limits and continuation

Glalie has no further GenerationIII evolution, so no unapproved evolutionary inheritance is chosen. Global species slots, breeding inheritance, Corphish→Crawdaunt and special individual Drizzle remain separate decisions/work. Actual PC menu deposit/withdrawal UI, summary-screen visual presentation, link peers, external save editors, interrupted flash, hardware and old-ROM downgrade gameplay are not certified by these tests. PC transport uses real native storage APIs and actual capture-to-PC, not a hand-built save schema.

Legacy valid untagged Pokémon stay unchanged. A vanilla/older game ignores the marker while preserving standard layout/checksum; its gameplay behavior and external editing tools were not tested. Preserve save backups. Future features must not reuse this growth filler or data selector89. Source rollback: revert this feature commit and its test registrations; existing tagged Pokémon remain valid records but lose the explicit ability effect on code that ignores it. No default world event consumes this API yet.
