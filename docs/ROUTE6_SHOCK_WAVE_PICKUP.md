# Route 6 — approved TM34 Shock Wave pickup

Source implementation on `implementation/route6-shock-wave-pickup-20261116`,
based on main `197d85ee1a61ac319f95fa65ab298bb126fe937d`.

The owner-approved TM/Move Register assigns TM34 Shock Wave to a normal Route 6
item ball that appears after Misty's defeat. Exact tile selection is delegated
engineering work. Authority:
https://docs.google.com/document/d/1EHByhspPMrn_H0ard-wbdZLRqInZVschQqXTWSiP5fY/edit
and `docs/moves/tm-and-capacity-plan.json` approved assignment 34.

## Gameplay source

- Append local object 7 at (18,15), elevation 3, southwest of the Underground Path
  doorway. Reuse `OBJ_EVENT_GFX_ITEM_BALL` and stationary face-down movement.
  Original six trainer objects, sign, two hidden items, door warps, terrain and
  encounter tables are unchanged.
- Route 6's transition script checks the existing `FLAG_DEFEATED_MISTY`, rather
  than badge count or whether the player has collected Misty's TM03 reward.
- Reuse native `finditem ITEM_TM34` / `Std_FindItem`: existing TM Case capacity,
  Shock Wave move-name text, fanfare, inventory insertion and object removal.
  The native capacity result in `VAR_0x8007` controls the permanent claim flag.
- Separate `FLAG_HIDE_ROUTE6_TM34` (0x0B1) and `FLAG_GOT_ROUTE6_TM34` (0x4AF)
  prevent pre-unlock hiding from becoming permanent collection. Map entry hides
  the ball before Misty or after collection, and reveals it otherwise. Full-pocket
  refusal does not claim the reward. No save structure or original flag changes.

Existing native precedent is `data/scripts/obtain_item.inc` and the original
Route 9 TM40 item ball. The new behavior is only the approved Misty-dependent
Route 6 appearance and independent one-save pickup.

## Integration boundary

TM51 Zap Cannon item/UI/compatibility and Surge reward conversion are separate,
unfinished work. The merged Surge implementation deliberately retained its old
TM34 reward pending TM51 (PR #157). This branch does not modify that owner's Gym
script. Consequently the source on this branch still has that historical second
TM34 source; do not call the final TM reward relocation/integration complete.
No permission to invent TM51 learners follows from this pickup implementation.

PR #175 owns source-parity documentation and checks, not Route 6 production code.
The existing ecology specialist owns encounter tables, which are untouched.
Flags 0x0B1/0x4AF were still unused in current main and the inspected current
legendary, Heracross and Playtest004 heads before this reservation.

## Execution boundary

Owner's current implementation-first directive excludes tests, builds, emulator
runs and diagnostics. None were initiated or added. Source was read only as
needed to implement native scripting and select the tile. Raw map source is not
in-game reachability evidence. No runtime, save/reload, full-pocket or graphical
acceptance is claimed. The commit uses `[skip ci]` to avoid automatic push/PR test
activation; all workflow definitions and required merge gates remain intact.
Keep the PR draft/unmerged with no acceptance points. Accounting stays 50 task
IDs and 461 earned / 251 remaining / 712 total.
