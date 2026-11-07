# Full native-suite wall-clock budget — INT-001-CI2

Evidence: integrated Thunder Wave/current-main run42374018809, job128407806158,
failed only because `Execute ROM integration tests in mGBA` reached20 minutes.
The log reached Agatha's initial League victory; there is no failed game assertion.
Same-head run42374015278 succeeded, as did the local complete30-group run with
815608 assertions,26 host suites/3 checkers and byte-identical clean rebuild.
The failure is retained in the report; a green duplicate does not erase it.

The suite has grown from23 groups/~534k assertions to30 groups/>815k, including
500 ordinary Trainer parties/1219 mons and eight actual League battles. Increase
only the CI wall-clock envelope: native step20→30 minutes and overall job45→60.
This leaves room for measured19-minute package transfers, two builds and a full
native run. The allowance remains bounded; no run is assumed guaranteed to pass.

Keep every assertion, protected ABI,4-million-instruction native-call guard,
per-scene frame limit, default full-suite call, host registration and independent
upstream comparison unchanged. No source-game logic, ROM data, expected baseline,
required check or repository security setting changes. The registration checker
also records the actual26 hosts/30 native groups and explicitly guards ordinary
Trainer registration; old hardcoded25/29 printed counts were stale.

New exact-head CI must succeed before accepting this maintenance change. Matching
combined-head integration can validate these identical workflow/checker hunks.
Rollback restores only the two wall-clock numbers/checker/docs; game saves are
unaffected. Future major suite growth should consider measured test partitioning,
with full coverage and combined integration retained, rather than unlimited waits.
