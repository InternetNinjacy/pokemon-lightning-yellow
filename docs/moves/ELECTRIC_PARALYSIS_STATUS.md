# Global Electric paralysis immunity — BTL-005-M2F

Authority: approved global rule in docs/moves/README.md and
`tm-and-capacity-plan.json`; distinct from Thunder Wave's pending spread targeting.

## Implementation

- `SetMoveEffect` rejects new paralysis on an Electric effective recipient in
  either type slot, before Limber processing. This shared path covers primary,
  secondary, contact-ability and reflected/synchronized effects that use it.
- The primary paralysis script prints the existing unaffected message rather
  than falsely reporting success or attributing immunity to Limber.
- The existing bad-move AI penalizes primary paralysis against either Electric
  type slot exactly as it already penalizes Limber. Damaging moves remain usable.
- Existing paralysis is not cured; no other status/type immunity changes. Move
  data, PP maxima, targets, IDs, save fields, species abilities and artwork remain
  unchanged. This does not implement Thunder Wave's broader Double target rule.
- Exact frozen reverse hunks preserve historical source hashes; no baseline or
  runtime data is rewritten. Tests use the target-generated AI structure offsets.

## Validation

Build PASS;25 host suites/3 checkers PASS; clean rebuild byte-identical SHA256
`63dc54d61b2f0965de072feb59e8d1026494b4df067b822da952491fa7fab6ee`.
Native focused/full-suite and Actions results are being collected; do not claim
final acceptance yet.
The native tests exercise actual Thunder Wave/Stun Spore primary effects, Rain
Zap Cannon damage/guaranteed secondary, Static contact effective-recipient behavior,
existing paralysis preservation and bad-move AI scores. Controlled type overrides,
HP and RNG seeds isolate the rule; this is not a full campaign or real link test.

No save migration is required. A matching modified link peer is still required;
vanilla peers do not know this mechanic. Real link hardware remains untested.
Rollback the three production hunks together with their exact test manifest and
normalizer integration; no deposited Pokémon or persistent state is altered.
