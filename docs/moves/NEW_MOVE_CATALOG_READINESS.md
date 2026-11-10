# New-move catalog prerequisite — BTL-005-M3A

The source catalog previously required exactly354 production moves and treated
all56 additions as absent. This tooling-only prerequisite accepts complete,
contiguous approved additions while retaining every original ID and the complete
410-design inventory. It assigns no production IDs and changes no game behavior.

`scripts/move_catalog.py` verifies contiguous unique IDs/count, approved appended
order, every explicit BattleMove field/category, nonempty names within15 local
characters, description pointers/text, and exact ordered animation dispatch with
existing script labels. Missing, duplicate, misordered or partial tables fail.
Semantic effect/target/flag translations must be explicit in design records;
otherwise they remain discrepancies. Source data presence is marked
`SOURCE_DATA_PRESENT_RUNTIME_UNVERIFIED`, never implemented gameplay acceptance.
An animation label (including a fixture's empty script) proves no playback.

`reconcile-move-specification.py` supports an isolated source root for tests and
reports source-present additions separately from354 original identities. The
historical source-baseline.csv remains originals-only. The existing production
JSON/CSV outputs remain byte-identical at this prerequisite. No frozen old hash
or regression normalizer changes. Existing-art provenance/playback audits remain
separate requirements, as do effects, ABI, save, UI and native battle acceptance.

Run `python3 tests/run_move_catalog_tests.py`: five test methods include nine
independent missing/reordered-table rejection cases, duplicate rejection,
byte-identical current generation, a complete temporary source fixture and a
wrong-owner-power discrepancy. Fixtures are temporary text, never compiled,
committed gameplay data or claims that Flame Fang exists in the game.

Next: complete355 Flame Fang,356 Flashfire,357 Blaze Rush as one production slice
with approved translations, effects, animation compositions and native tests.
Do not add any of their IDs until the complete slice and consumers are ready.
