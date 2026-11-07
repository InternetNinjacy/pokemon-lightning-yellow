# Thunder Wave all-other-active targets — BTL-005-M2I

Authority: approved Move Register and overlays: Singles opponent; Doubles every
other active battler, including ally, never self. Native independent checks,
ordinary PP/Pressure and previously merged global Electric paralysis immunity.
No change to Electric/Status/0 power/100 accuracy/20PP/priority/flags/original art.
No new IDs, move distribution, species, abilities, save migration or link fields.

## Implementation and layout

- MOVE_TARGET_FOES_AND_ALLY advertises the actual target set without a target
  selection prompt. Existing paralysis script and per-recipient animation reused.
- Four verified-unused bytes of BattleStruct padding at0x1E4 hold original actor,
  pending-target mask, current recipient and active flag. Target ABI proves the
  structure remains0x200 bytes, existing member offsets/heap size unchanged.
  Multi/link buffer ends before this padding; no saved structure is modified.
- Initialize lazily in attack cancellation, including called Thunder Wave. Skip
  absent/fainted battlers and consume each target once. The native cancellation
  tracker is retained across recipients, so inability/obedience is checked once.
- Reuse native move-end restart/cleanup. Retain original actor across each Magic
  Coat reflection, then continue other recipients; normal type/ability/Protect/
  Safeguard checks remain per recipient. Native noPP cancellation clears the
  iterator, preventing free later hits when a faster Spite exhausts chosen PP.
- Exclude only this all-recipient move from single-target Lightning Rod redirect;
  all other Electric moves retain the original behavior. Follow Me cannot turn
  the all-recipient move into a single-target action. Native PP/Pressure code is
  unchanged, including one initial charge and no repeated base cost.

## Executed validation

Build and25 host suites/3 checkers PASS. Focused mGBA0.10.5 **565 assertions PASS**:

- Target-generated512-byte layout/padding offsets; native iterator each actor
  mask, unique nonself recipients, absent/fainted skips (routine tests).
- Actual Single and Double turns, all four actor positions including native wild
  AI actors, intended status recipients/no direct HP damage/one basePP.
- Independent Limber/Electric/Ground/Protect prevention, Magic Coat on foe or ally,
  Follow Me and Lightning Rod controls; ordinary Thunderbolt still redirects.
- Independent evasion hit/miss, once-per-move attacker paralysis (all or none),
  lastPP reaches all recipients, three Pressure recipients cost4PP once.
- Real Sleep Talk calls Thunder Wave, spending Sleep Talk PP only; real faster
  Spite exhausts chosen TW PP and no target is hit for free.

An initial generic full-turn2400-frame test timed out on Magic Coat. Diagnostic
trace proved normal completion at2423 frames: TW phase1720, remainder703. The new
fixture bounds its move phase at2400 and retains the unchanged existing2400-frame
remainder guard. No common assertion, ARM-call guard or game timing was relaxed.
A missing declaration/symbol in the initial test build was corrected; no runtime
success claimed for that failed launch.

Full29-group regression, clean repeatability and Actions pending at initial
snapshot. Do not equate focused acceptance or all21 source overlays with a complete
410-move system:56 new moves remain unimplemented. New artwork is not introduced.

## Limits and rollback

Absent/fainted selection is tested in the native iterator, not every complete
replacement scene. Metronome/Mirror Move/Assist, every ability/item/status/Safeguard
combination, real link peers, campaign and cartridge behavior are not exhaustively
certified. Existing common paths and all prior suites remain mandatory.
Matched modified link peers required; no vanilla-peer compatibility claim.
Rollback exact six production hunks/files and metadata/normalizers as one unit;
private padding returns unused and persistent data is unaffected. Parents69/67/66/
64 are merged. Main should only receive this feature after full checks pass.

## Current-main integration (not the earlier acceptance result)

Main4bb2ece0 adds merged Agatha/ordinary Trainer/capture acceptance. Preserve all
500 Trainer arrays, all8 Elite Four battles and new registrations. Integrated
26 host suites/3 checkers PASS, with old immutable hashes preserved through exact
parallel Agatha/TW/ordinary-source projections. Updated registration checker now
counts26 hosts/30 full native groups and explicitly protects ordinary Trainer
coverage; its previous printed25/29 totals were stale. New combined build/full/
clean/Actions results pending. Earlier755945 belongs only to the old72 base.
