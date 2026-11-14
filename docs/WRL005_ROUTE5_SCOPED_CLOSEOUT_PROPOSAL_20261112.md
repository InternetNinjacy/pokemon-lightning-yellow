# WRL-005 Route 5 wild Ditto existence — scoped closeout proposal (2026-11-12)

**Review proposal only; no gameplay, source guard, current tracker, dashboard or append-only ledger changed by this branch.** Owner/Master Control acceptance is required before awarding any provisional planning points. Permanent task ID is **WRL-005**; historical scope is **2 points** (currently earned 0 / remaining 2). Do not duplicate WRL-009 ecology or WRL-002 accessibility credit.

## Previously merged evidence

1. [PR #142](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/142), merge `90d0811b46bfab6f054d9b9c9b9c64240b6f3562`: `src/data/wild_encounters.json` contains only approved Route 5 ordinary grass **Rattata/Skitty/Ditto**, in both FireRed and LeafGreen 12-slot tables. Weighted rates Rattata **50%** (Lv10–13), Skitty **40%** (Lv12–14), Ditto **10%** (Lv13–15). Encounter trigger remains 21. Source checks `tests/check_route5_encounters.py` and byte-exact reversible `tests/approved_route5_changes.py` preserve the historical pre-ecology source baseline. Exact-head [CI #42525883040](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42525883040) modified and independent upstream-baseline **SUCCESS**.
2. [PR #168](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/168), merge `06a4a19232e8a43096903bfe786cc473b73985a8`: real compiled-ROM Route 5 approved grass collision/encounter-eligibility verification and 1,000 native wild single-encounter generations, with approved Rattata/Skitty/Ditto verified; independent current-head modified/upstream [CI #42656995033](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42656995033) **SUCCESS**. Setup includes controlled grass warp; not evidence of full natural access.
3. [PR #169](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/169), merge `06c9104dd1fc471035ae173070b53828c25f0999`: independent test drives genuine D-pad movement from Route 5 northern corridor at x23,y0 down past ledge to approved grass at y5; no direct grass warp. Exact-head [CI #42730539593](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42730539593) modified and upstream-baseline **SUCCESS**.
4. [PR #178](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/178) adds separate Cerulean-to-Route 5 boundary evidence from a staged location **below** the existing Cut-tree chokepoint. It must **not** be described as unrestricted fresh-game access.

## Narrow scope decision requested

The original WRL-005 requirement is **ordinary Route 5 wild Ditto encounter existence** under the approved three-species grass roster. The above evidence supports marking that specifically bounded task **accepted on main**, subject to owner/Master Control confirming that its 2 planning points were not previously credited through WRL-009. This is **not** overall WRL-009 ecology acceptance, WRL-002 complete accessibility, or release certification.

**Proposed tracker-only delta upon acceptance, not applied by this proposal:**

- Keep unchanged permanent ID `WRL-005`, title, area WRL and total planned effort **2**.
- `effort.earned: 0 → 2`; `effort.remaining: 2 → 0`.
- Append an effort history entry dated 2026-11-12 citing merged #142/#168/#169 scope and terminal dual CI, expressly excluding ecosystem-wide maps/methods or other WRL credit.
- Replace stale `status.implementation = "not started"` with scoped implemented on main; record actual native and source checks; reference merge #142 as implementation, #168/#169 as acceptance.
- Replace obsolete level/rate blocker with **null** for this narrow task; retain remaining full connectivity/progression in WRL-002/009 and INT-004/005 rather than inventing a new WRL-005 blocker.
- Update corresponding canonical tracker, dashboard and append-only implementation ledger together, preserving complete history and all 50 IDs. Only after source evidence and formal acceptance: official totals **461/251 → 463 earned / 249 remaining out of 712** (65.03% earned).
- Verify exact-head modified native and independent upstream-baseline SUCCESS for any accounting PR and its merged-main CI if merge is separately authorized.

## Explicit non-claims

The 50 remaining ordinary method accessibility checks, 22 non-wild acquisition audits, 122-family comprehensive acquisition, protected Tanoby/Lost/Altering Cave maps, post-League special stationary Pokémon, SOW-owned flags/events, real new-game continuous map traversal, hardware/link and final release QA **remain outside WRL-005**. Do not count those as complete or weaken their current gates.

## Source of reconciliation

[Master Control issue #105 reconciliation comment](https://github.com/InternetNinjacy/pokemon-lightning-yellow/issues/105#issuecomment-6829574244). No production data, source contract, CI workflow or main-tracker modification is included in this proposal.
