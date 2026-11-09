# PKM-009 — Current main source reconciliation (252 species)

Snapshot: 2026-11-09, GitHub main commit `37230a9c513ed013fc6b402ec800bd1d13960492`.

**Purpose:** Source evidence for the existing PKM-009 workstream, not a new design register, implementation authorization, progress tracker, or release certificate. The [living register](https://docs.google.com/document/d/1tIdBczPU7bD-31Hw2hyCJQOk_E0bN1DwgLxld-YtrK8/edit) and [historical v0.6 source audit](https://docs.google.com/document/d/19BrMcvejhSokPSplifEzGUD0BaN5dhX1bkq4d1B1kz8/edit) retain their authority hierarchy. Later owner approvals supersede the old v0.6 recommendations.

| Exact regional entries | Source snapshot |
|---|---|
| #001–063 | [JSON rows 001–063](pkm009/source-reconciliation-001-063.json) |
| #064–126 | [JSON rows 064–126](pkm009/source-reconciliation-064-126.json) |
| #127–189 | [JSON rows 127–189](pkm009/source-reconciliation-127-189.json) |
| #190–252 | [JSON rows 190–252](pkm009/source-reconciliation-190-252.json) |

## Snapshot findings
- 252/252 approved custom-Dex source species, natural learnset, TM/HM and evolution records captured.
- 78 current main learnsets match the historical v0.6 **proposed** full sequences; 174 differ, which is not equivalent to 174 approved missing implementations.
- 14 owner-locked individual species/eespective family profiles distinguished from delegated design. Three Dratini global learnset exceptions appear on main.
- All 16 latest owner-approved (Nov 6) weather species-wide ability slots match current main.
- Twelve historical/locked target records reference identifiers absent from this pinned main; **Jolteon Volt Switch/Discharge and Umbreon Snarl/Payback/Foul Play are owner-locked dependencies**. No substitute move is authorized.
- Existing source TM/HM compatibility is captured, not expanded. Custom/new TM legality remains a separate approval/integration issue.
- Species already changed in PKM-009 draft implementation PRs #144, #149, #150, #153 are explicitly **not merged**. The snapshot tracks main, not the latest feature-branch source.

## Evidence limits
This matrix is not proof of owner approval for each older delegated learnset proposal, exhaustive 252-stage runtime balance, emulator/hardware validation, TM51 eligibility, or accounting acceptance. In the individual row files, `source_main` is pinned source, `approval` is the historically classified target and explicit overrides, `comparison` is source-relative status and blocker evidence, and `historical_v06` records the older audit for traceability.

**No project effort points are awarded by this documentation snapshot.** Master Control remains responsible for integration and accounting.
