# Both-opponent status moves — BTL-005-M2G

Authority: approved Move Register86/87/642, retained in approved-design-overlays.

Scary Face becomes Ghost/Status,15PP, both opponents;90% accuracy and Speed-2
remain. Screech retains Normal/Status85%/40PP/Defense-2; Fake Tears retains
Dark/Status100%/20PP/Sp.Def-2; both target opponents together in Doubles. Singles
retain a single opponent. All other fields/flags, IDs, effects, native scripts,
existing animations and learnsets remain. Descriptions reflect both opponents.

Reuse the native MOVE_TARGET_BOTH path, which executes each opponent's own
accuracy/effect checks and uses HITMARKER_NO_ATTACKSTRING to avoid repeated PP
payment. No new iterator or battle script is introduced. This does not implement
Thunder Wave's separate three-recipient rule. No new artwork or move distribution.

Scary Face's PP maximum increases10→15; saved remaining PP is not refilled. Existing
PP Ups and spent PP remain; old saves cannot exceed the new maximum, so no new
migration is necessary. Existing save/wire fields remain unchanged.

Build and25 hosts/3 checkers PASS. Actual native Single/Double turns show stat-2
on the appropriate opponents, no ally/user/damage, onePP, and split hit/miss
outcomes for Scary Face and Screech. Fake Tears hits both controlled opponents.
Full focused completion, clean repeatability, full regression and Actions pending
at initial snapshot; do not mark merged/accepted before final outcomes.

Magic Coat/Protect/stat-immunity behavior is inherited from unchanged native
scripts/flags, not exhaustively newly tested. Existing-art animation playback
occurs in those actual turns; no screenshot-based visual redesign approval claimed.
Rollback three rows/descriptions plus their metadata/exact regression normalizer;
no persistent storage or species data changes.
