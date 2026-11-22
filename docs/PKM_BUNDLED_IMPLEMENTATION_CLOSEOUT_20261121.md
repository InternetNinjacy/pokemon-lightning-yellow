# PKM-005 / PKM-006 / PKM-009 implementation-first closeout (2026-11-21)

## Owner policy
Owner performs gameplay playtesting. Do not run builds, emulators, tests, or broad CI for this implementation pass. Never translate deferred testing into gameplay acceptance. This document records actual source changes separately from unverified design/source reconciliation.

## PKM-005 — implementation complete, hardware acceptance deferred
Existing implementation and historic scoped acceptance are recorded in project-tracking.json. Owner directs implementation closeout; real peer link-trade/hardware acceptance is a separate playtest/release checklist item, not a production implementation blocker. Existing earned 7/remaining 1 retained pending formal point-policy decision; no points retroactively awarded.

## PKM-009 — concrete approved production correction
The default-branch source read on 2026-11-21 showed Pichu ThunderShock at level 1 and Pichu evolution at level 20. Owner's latest locked requirements: ThunderShock at level 9; Pichu evolves to Pikachu at level 15. This branch updates only the Pichu natural learnset entry and Pichu evolution entry. No Pikachu/Raichu evolution edits. No build or runtime acceptance claimed.

Previously recorded source integration: 146 adopted natural arrays and 14 locked profiles. Historical TM/HM source contract: all 252 regional species and 5,831 original TM/HM memberships across TM01–TM50 and HM01–HM08. This is not proof that every new machine, especially TM51/TM52, is complete. Owner approved 22 TM compatibility decisions in recent conversation; exact species-to-machine rows must be retrieved from canonical approval before further edits; do not infer or fabricate a matrix. TM51 Zap Cannon compatibility and reward integration remain independently reconciled; do not overwrite the TM52 work or original 58-machine compatibility bits.

## PKM-006 — outstanding roster compliance
Historic audit recorded 281 authored off-Dex occurrences. This is an occurrence count, not 281 species or automatically 281 production fixes. The audit guard is merged. Current production-role review and approved replacements must be compared with latest source, including ordinary trainers, special encounters, wild ecology, scripts, and harmless reference-only occurrences. No off-Dex exceptions authorized. Do not modify separately owned WRL-002/009, GYM, or stationary-overworld work without reconciling ownership. No claim that 281 are still live in current main or all corrected.

## Accounting and next implementation queue
Preserve 50 permanent IDs, 461 earned / 251 remaining / 712 total until evidenced accounting acceptance. PKM-005 is implementation-complete with external acceptance deferred; PKM-006 remains source-policy reconciliation open; PKM-009 remains source reconciliation open. Next: retrieve exact approved 22 compatibility rows, reconcile current TM51/TM52 code and Pichu source, enumerate current off-Dex role occurrences against latest main and ownership, then implement only approved unowned corrections. No tests, builds, emulators, merges, or release claims.

## 2026-11-21 follow-up: exact owner-approved TM51 species matrix

The exact owner approval is TM51 Zap Cannon compatibility for Pichu, Pikachu, Raichu, Magnemite, Magneton, Voltorb, Electrode, Elekid, Electabuzz, Jolteon, Mareep, Flaaffy, Ampharos, Electrike, Manectric, Zapdos, Raikou, Porygon, Porygon2, Forretress, Staryu and Starmie (22 total); Mew explicitly excluded. This PR stages exactly 22 source eligibility entries in tmhm_learnsets.h and aliases reserved ITEM_15B (347) as TM51; TM52 remains item 348. These edits alone do not implement a functional TM item, its TM Case/move mapping, or Surge reward. No build or test executed. Do not duplicate the separate private implementation commit 71ea8dca5f3c4aed5bfcf93a73649cff94ab0211 without comparing actual source; that commit was not fetchable from the public repository.

## PKM-006 historical inventory classification (read-only, no audit run)

The committed unresolved inventory has 572 total findings, divided into 281 historical authored off-Dex assignments (199 wild, 24 ordinary Trainer, 54 Trainer Tower, four direct event scripts), 279 legacy Battle Tower references, eight script species-variable review candidates and four C reference candidates. This is the old audited-base snapshot, not a fresh count of current source. The Water Labyrinth direct Togepi egg finding is demonstrably stale: current script now refers to MAGBY in its post-egg species text and no longer contains TOGEPI. Conversely, current Birth Island and Navel Rock scripts still contain DEOXYS and LUGIA event assignments, so they are not universally resolved. Trainer and wild counts cannot be asserted current without comparing current tables. No off-Dex species exception is granted. Source changes to wild/Trainer Tower/ordinary Trainers are deferred pending precise current mapping and ownership with WRL/GYM specialists.
