# PKM-008 — Ponyta evolves at level30

The owner-approved November2 Evolution & Item Supply Source Parity audit,
section3, explicitly identifies level40 in source versus approved level30 as an
implementation gap. Its source SHA256 and exact before/after rule are recorded
in `tests/specifications/approved_ponyta_delta.json`.

Only Ponyta's ordinary level threshold changes: level29 does not evolve; level30
or a later level-up can evolve to Rapidash. Everstone and B cancellation retain
normal behavior. Existing Pokémon are not force-evolved on loading a save.
No stats, abilities, moves, species IDs, other evolution rules or save fields change.

Run all `tests/run_*tests.py`, `scripts/check-pewter-museum-trivia.py`, build with
`make -j2` and run the compiled-ROM mGBA suite. Focused coverage:
`LIGHTNING_PONYTA_ONLY=1 python3 tests/emulator/run.py`.
Host production lookup tests cover29/30/31 and Everstone; native tests execute
threshold lookups, actual animation cancellation, successful Rapidash evolution,
Dex registration and flash save/load before National Dex. Exact whole-file
normalization preserves every other evolution-table byte and historical hashes.

Local and final Actions builds/all twelve host suites/Museum static checks,35
focused and full native276161 assertions passed; clean rebuilds match.
PR22 is merged into main. Exact code/merge and CI references are in the ledger. Broader
Ponyta species stat/ability/learnset design is not implemented by this threshold
correction. Full progression, physical hardware and peer link tests remain outside
this focused acceptance. Rollback is an ordinary revert; no save migration.
