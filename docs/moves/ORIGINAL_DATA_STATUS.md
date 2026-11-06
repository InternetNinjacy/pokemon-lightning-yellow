# Approved original move data — BTL-005-M2A

[PR42](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/42),
production source5a2b96f707806c8a6658dc4ff4e1f3e7bf2fee3b.
Separate `feature/approved-original-move-data` child of PR41/3031dcdb.
Owner's standing implementation authorization and the supplied approved Move
Register apply. Exact evidence/OWNER_EXPLICIT fields remain in
`approved-design-overlays.json`; this milestone adds implementation status,
not new design approval. Main and parent PRs remain unchanged/unmerged.

## Eight original moves / ten field changes

| Move | Previous | Approved implementation |
|---|---|---|
| Twineedle |25 power|40 per hit; original two-hit/poison effect retained|
| Lick |20 power|40; Physical/30% paralysis unchanged|
| Outrage |90 power|120; original rampage/confusion behavior retained|
| Mean Look |Normal|Ghost; Status and original trap/accuracy handling retained|
| Astonish |Physical/30 power|Special/50; original30% flinch/Minimize effect retained|
| Rock Tomb |50 power/80 accuracy|65/90; original10PP/Speed drop retained|
| Silver Wind |5PP|10PP; original60 power/10% all-stat effect retained|
| Icicle Spear |10 power|25 per hit; original hit-count mechanism retained|

Only `src/data/battle_moves.h` changes production code/data. All remaining347
rows (including NONE) and every other field of these eight rows remain identical
to the parent. No new IDs, learnsets, TMs, animation assets, scripts, save fields,
priorities, contact flags or targeting changes. Category and elemental type
remain independent. Astonish is an explicit owner exception to its official
Generation IV Physical category; the historical category baseline is preserved
by an exact reviewed one-entry delta.

The reconciliation generator now retains REWORKED_ORIGINAL identity after a
change is implemented, instead of accidentally treating an empty delta as proof
the move was never redesigned. Eight source-field implementations are explicitly
marked IMPLEMENTED_APPROVED_DATA; custom effects are not inferred complete merely
because their currently expressible fields match. Current source snapshots and
SHA remain separate from historical baseline provenance.

## Verification

- `python3 tests/run_original_move_data_tests.py`: PASS frozen eight-row/ten-field
  audit, authority matching, unchanged effects/flags/other rows and one category
  exception. Historical expected hashes remain intact through an exact normalizer.
- `LIGHTNING_ORIGINAL_MOVE_DATA_ONLY=1 python3 tests/emulator/run.py`: mGBA0.10.5
  **PASS100 guarded assertions**. Real ARM data reads, exact controlled numeric
  base damage for all eight moves, offensive stat selection and actual AI estimates.
  Level50/Attack200/Defense100/Sp.Atk50/Sp.Def200, neutral stages/no items or
  abilities/no weather/critical: base results37/37/107/0/7/59/8/24 in table order.
  These are real CalculateBaseDamage/AI calls, not complete damage-script turns;
  final STAB/type/random/per-hit behavior is not inferred from these numbers.
  Astonish responds to Light Screen rather than Reflect/burn; Lick responds to
  Reflect/burn; Mean Look still bypasses ordinary damage. Silver Wind max PP10/16
  and native move assignment/flash roundtrip preserving an older5PP record pass.
- The older-PP record is a controlled fixture, not a claim that every historical
  save or real link peer has been tested. Existing current PP is not refilled.
- All19 host suites/3 static checkers: PASS locally. Full22-group mGBA0.10.5:
  **PASS513952 guarded assertions**, all parent groups retained.
- Clean rebuild: PASS, identical SHA256
  11e457267e5f6dc958837ec3e83ae200c1e8b40eee96db4feca241fa5a5e221b.
- PR42 remote conclusions are recorded at its actual Checks/review description,
  separately from these local results. Parent PR41 review42076325619 and
  push42076316757 both SUCCESS, remote mGBA0.10.2 PASS513858.

Effect scripts, animation pointers/art and command code are unchanged, checked
by parent audits. This is not separate runtime certification of each two/multi-hit
poison/flinch/rampage/trap/Minimize/animation edge case; those remain in move-engine
acceptance. No new artwork or binary artifacts committed/uploaded.

## Remaining work and rollback

Leech Life's approved Special/80/10 change is deliberately atomic and separate.
Its15→10PP decrease, and Roar’s separately approved20→15PP decrease, require
a shared tested path for existing party/box/daycare
records above the new maximum, including PP Ups, without refilling spent PP or
corrupting encrypted data. This is a technical implementation dependency, not
an owner design question. The other custom original effects/targeting/weather,
56 new moves, animations, AI extensions and machine metadata remain pending.

Revert this child PR to restore just these eight rows/category metadata, retaining
PR41 and all parent features. IDs and saved layouts are unchanged; Silver Wind's
increase needs no forward schema migration. For a downgrade, use a pre-feature
save or explicitly clamp Silver Wind current PP to the restored old maximum:
new10/16PP records can exceed the old5/8 maxima. That downgrade path is not
automatically migrated or tested here. Real peer link/hardware remain untested.
Drive synchronization remains pending.

## Subsequent PP compatibility child

[Saved-PP compatibility and Leech Life](PP_COMPATIBILITY_STATUS.md) implements
the separately tested atomic Leech Life revision on a child of PR42. The earlier
remaining-work section above describes PR42 in isolation; Roar remains pending.
