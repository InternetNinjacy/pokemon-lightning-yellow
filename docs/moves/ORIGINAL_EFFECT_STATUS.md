# Pin Missile / Dragon Rage — BTL-005-M2E

Authority: approved Move Register lines664 and749, retained evidence in
approved-design-overlays.json; November6 autonomous execution. Parent main includes
merged policy55,Roar56,sign58,gifts59,six Elite Four60 and weather61.

- Pin Missile: Physical/Bug, single55-power hit (replaces2–5 hits),30% poison,
  original85 accuracy/20PP/selected target/priority0. Reuse native POISON_HIT effect.
- Dragon Rage: Dragon/Special, fixed damage equal to current user level (replaces
  fixed40), original100 accuracy/10PP/power1 fixed-damage sentinel. Reuse native
  LEVEL_DAMAGE effect/script, exactly Night Shade's damage method, retaining type.
- Existing flags, including Protect/Mirror Move/King's Rock eligibility, remain
  unchanged; no new flag-based balance policy is inferred. Existing same-name
  animations retained, no new artwork. Pin Missile's multiple visible projectiles
  do not represent multiple damage hits. Descriptions state the approved behavior.
- No new IDs/learners/TMs/types/categories/PP maxima/save fields/item rules. No PP
  migration is needed. Every other move field/row remains identical. Exact semantic
  effect/flags-to-engine translation and frozen reverse deltas retain old hashes.

## Executed at initial commit

- Build PASS;25 hosts/3 checkers PASS, including exact delta/evidence/other-row audit.
- mGBA0.10.5 **314 focused guarded assertions PASS**. Controlled numeric Pin Missile
  base damage50 and actual AI50; actual Single/Double turns give one42..50 direct
  hit, with poison/125 residual, nonpoison and85%-accuracy miss outcomes, onePP19,
  selected opponent only. Battle Armor suppresses crits to isolate exact hit count;
  low-level enemy Splash/1000HP/types are controlled fixtures.
- Eight actual Dragon Rage turns/animations: Single and Double, user levels1/27/50/
  100 each give exactly that HP damage despite deliberately skewed offensive/
  defensive stats and Steel resistance; PP9, no spread/forced exit. Native Level
  Damage AI effect heuristic is reused; no exact numeric fixed-damage AI overhaul
  is claimed. Other immunities/Protect/King's Rock combinations not exhaustively
  retested; common scripts/flags are unchanged and parent suite retained.
- Full latest29-group native, clean reproducibility and remote CI pending initial
  commit. Report actual final results before merge; no410-move completion claim.

Six other original redesign policies, global Electric paralysis immunity and all56
new moves remain. Rollback restores two rows/descriptions; PP/save ABI unchanged.
No ROM/BIOS/save/graphics artifacts or release.
