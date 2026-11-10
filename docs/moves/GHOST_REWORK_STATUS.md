# Spite / Grudge — BTL-005-M2H

Authority: final Move Register574/582, superseding earlier Ghost Status categories.
Uses the owner-approved original Move_SPITE/Move_GRUDGE animation scripts. No new
artwork, IDs, learnsets, TMs, species, saved fields or link-record expansion.

## Behavior

- Spite: Ghost/Special30/100/10PP. Normal crit/stats/type damage occurs on a
  successful hit whether or not native last-move PP depletion is eligible.
  Then the unchanged GenIII command optionally drains2–5 PP. No eligible move
  skips only that drain. Native eligibility includes the original >1PP rule,
  known last move and native transformed/mimicked-move persistence behavior.
  Immunity/miss/Protect cannot receive the post-hit PP drain. The original
  Protect/Mirror flags remain; no contact or King's Rock flag is invented.
- Grudge: Ghost/Special30/90/10PP, native five-turn Rollout lock/escalation and
  miss/end rules, excluding Defense Curl multiplication only for MOVE_GRUDGE.
  Normal Rollout keeps its bonus. The old Grudge KO-trigger status is not set.
  Selected-opponent targeting replaces USER because the approved Rollout damaging
  behavior necessarily attacks a foe; this is a required technical consequence,
  not a new self-damage rule. Retain original Protect/Mirror flags.
- Existing HP-aware AI no longer discourages Spite as a nondamaging move at low
  target HP. Damage estimation reads the authoritative Special/category data.
- Grudge's PP increases5→10: existing remaining PP is not refilled. No new
  migration is needed. Old categories remain recoverable through exact frozen
  reverse deltas; no original source/ROM checksum is fabricated or overwritten.

## Executed validation / limits

Build PASS;25 host suites/3 checkers PASS. Initial focused native225 assertions
PASS: seven actual Spite turns including missing/unavailable/unknown last move,
1/0PP eligibility and Normal immunity; damage25 with controlled Special stats,
optional drain5 or0 and onePP. Sleep isolates the opponent's own PP spending.
An earlier fixture incorrectly assumed every zero-PP opponent turn spends1PP;
corrected test isolation, no game/assertion bypass. Spite native base/AI5 checked.

Native Grudge/Rollout command controls prove power/lock/miss and Curl distinction;
actual five consecutive Grudge turns/animations prove30/60/120/240/480 despite Curl,
PP9, final lock clearance and no old Grudge status. Latest fixture adds a direct
Grudge Special-stat numeric assertion. Full latest focused/full regression,
clean reproducibility and Actions pending initial snapshot.

Not exhaustive: Spite KO/Substitute/transformed/Mimic/Protect combinations, all
Grudge Double/sleep/called-move/Pressure variants, fair trainer balance and real
link peers. Native common paths retained, but identifiers alone are not runtime
certification. No full campaign, cartridge or ROM release claim.

Stacked after67→66→64. Merge only after dependencies and final required checks.
Rollback exact production hunks/data/descriptions and their test manifests; no
save layout changes or deletion of original dormant engine effects required.
