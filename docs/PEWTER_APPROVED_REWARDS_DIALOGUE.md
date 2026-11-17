# Pewter — approved Hail reward, Avalanche Badge and dialogue

Independent source branch `implementation/pewter-approved-rewards-dialogue-20261116`
from main `197d85ee1a61ac319f95fa65ab298bb126fe937d`.

## Owner-approved source

- Master Plan, October 28 verbatim Brock and ordinary Pewter Trainer dialogue;
  Avalanche Badge name with the unchanged original Boulder Badge artwork:
  https://docs.google.com/document/d/1P4TuXViTpB0BycldB1SzRZTRX8sV_Rd0lFuPwyGDJQk/edit
- TM/Move Register: confirmed original TM07 Hail is Brock's reward, superseding
  the historical number-TBD note:
  https://docs.google.com/document/d/1EHByhspPMrn_H0ard-wbdZLRqInZVschQqXTWSiP5fY/edit
- Eight-Gym final handoff retains the same reward/name/dialogue decisions:
  https://docs.google.com/document/d/18jVindV66zc_2_6xPqZajCiXf-1738lADjShESrDxU0/edit

PR #126 explicitly implements the weather party only, preserving map scripts
and excluding badge/TM/dialogue/reward redesign. No open PR found owns this slice.
This branch does not touch Brock/Liam parties, AI, weather, artwork or gym terrain.

## Actual gameplay change

- `data/maps/PewterCity_Gym/scripts.inc`: original reward flow now checks capacity
  for and gives ITEM_TM07 instead of ITEM_TM39. The original
  `FLAG_GOT_TM39_FROM_BROCK` retains its exact save bit, tracking the reward even
  though its historic name says TM39. Already-claimed saves do not receive a
  second TM or lose their old item; an unclaimed/full-pocket reward remains
  claimable through the native retry branch. No save conversion added.
- `data/maps/PewterCity_Gym/text.inc`: install all three exact approved Brock
  sections and all three exact ordinary-Trainer sections. Only technical text-box
  line/page breaks and original music control codes are added. Brock's approved
  post-battle text is split around the existing award flow; the badge jingle is
  in the victory callback rather than replayed on a full-pocket reward retry.
  Native receipt text now names TM07. No invented dialogue or battle changes.
- `src/strings.c`: both existing first-badge display strings now say
  AVALANCHE BADGE. Original exported symbols, badge flags, ordering, field-use
  privileges, badge artwork/palette and the existing 1400 prize remain intact.

Reused mechanisms are FireRed's existing leader victory callback, badge/save
flags, checkitemspace/giveitem_msg one-time reward/retry path, msgbox/line/page
controls, existing badge music and existing shared badge-name lookup strings.
The new content is the already-approved Hail reward, badge name and exact prose.

## Status

Source implemented, untested and unmerged. No tests, builds, emulator runs,
diagnostic workflows or failed-test investigations initiated; no test, workflow
or guard edited. `[skip ci]` leaves required merge checks intact and unfulfilled.
Text rendering, battle-to-field presentation, full-pocket/save behavior and
shared badge display remain runtime-unverified. No final feature acceptance or
point award: preserve 50 IDs and 461 earned / 251 remaining / 712 total.

Route 6 TM34 pickup is independent PR #259, not included in this branch. Other
existing feature owners retain their branches. Further source rollout should
continue with another approved, unowned reward or narrative slice; unresolved
ordinary-Gym species-to-source-ID mapping must not be guessed.
