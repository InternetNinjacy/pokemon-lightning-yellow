# Supplied specification reconciliation — November4,2026

Owner request: reconcile supplied exports with old tracker blockers, then implement
remaining fully approved typing corrections. This checkpoint precedes production
edits. Historical approval and implementation entries remain preserved.

## Availability and precedence

All **40 complete TXT exports** in the November3 archive are locally available;
all40 SHA-256 entries reverified. [Verbatim package inventory and source hashes](SUPPLIED_SPECIFICATION_MANIFEST.json)
identify exact snapshots. Archive recovery: owner upload
`file_00000000bbc081f5b71ce88948438545`. A missing local cache in a future cloud
session calls for retrieving this existing upload, not asking for the same ZIP.
Direct authenticated Google Drive writing remains unavailable; source exports
solve specification intake, not live Drive synchronization.

Read in full for this reconciliation: the252 typing/source matrix (including its
252 baseline-only distinctions and67-move audit), Trainer Class Eligibility FINAL
handoff, Move System Design Completion handoff, Weather Map Assignments handoff,
current Lighting & Weather Specification with all later appendices, Eight-Gym
Leader & Trainer handoff with all later appendices, and Evolution & Item Supply
source-parity report. Relevant Master Plan, approved Dex, ecology and Work Queue
passages were examined. This is not a claim that all unrelated paragraphs in all40
registers, or all individual species-profile documents, have been read/supplied.

Latest explicit owner decisions override older draft headings and source-only
snapshots. In particular: Gyarados Water/Dragon confirmed directly by owner after
the conflicting Gym addendum;41/41 ordinary class pools closed in the final
November3 handoff; Giovanni cumulative4/5/6 with Tyranitar ace locked; no arbitrary
individual teams inferred. SOW implementation HOLD remains specific and active.

## Corrected blocker classifications

|Task|Old unavailable/unfinished claim|Current evidence and remaining dependency|
|---|---|---|
|PKM-003/007|Historical twelve rows treated as final typings|Master lines81/878/1208/1211 and matrix PartA establish35 effective rules;24 still differ in current source. PKM-003 history retained; PKM-007 tracks the actual correction work.|
|BTL-005|Move register/ledger/distributions unavailable|All five move exports present;17/17 design reviews locked. Engineering354+56 overlay/effects/IDs/animation feasibility still needs implementation audit. Proposed composites and unfinished per-species learners are not approvals.|
|TIM-003|Classification/transition/RTC answers missing|Viridian Forest, Lavender, Mansion, timing, failure fallback and immediate recovery are supplied. Precise palette/tint values and visual approval remain unresolved.|
|WTH-001|Map exceptions/reentry/RTC decisions missing|Weighted Kanto and island groups,50/50 indoor reentry, sunset/no-renormalization, Cycling Road and RTC recovery are supplied. Source-ID/transition integration is engineering work.|
|WTH-002|Rendering scope unavailable|Fog/indoor/cave scope supplied; technical palette/fade/render compatibility and pending visual values remain.|
|WTH-003|Field Hail duration/bridge wholly absent|Ball PR#13 already implements/tests natural Snow→Hail with field-weather lifetime and five-turn move expiry. Wider regional integration remains; don't count existing bridge credit twice.|
|GYM-001|Gym/class specifications unavailable|All eight Gym/41 Gym-Trainer species supplied;41/41 non-Gym class pools closed. Missing individual battle levels/moves still prevents fabricating complete trainer parties.|
|GYM-002|All important teams undecided|Giovanni Hideout4/Silph5/Gym6 and Tyranitar ace now locked. Several late-leader sets, rival/Elite Four particulars and revival-AI implementation remain separate.|
|GYM-003|Celadon Castform trainer not selected|Celadon ordinary Trainer7=Cacnea+Castform is approved. Castform level/moves and source-slot mapping remain; Brock/Misty replacements also supersede old Swinub/Staryu.|
|GYM-004|Skarmory/off-Dex and Gyarados type unresolved|Skarmory replaced in design by Gyarados; owner settles Water/Dragon. Five Blaine species and ordinary Singles retained; no invented Ground immunity, levels/moves/items or workaround.|
|WRL-001|Eevee gift location unspecified|Oak parcel-return Eevee approved; existing Nest reward implemented. Starter/rival/story engineering and unspecified Eevee attributes remain, not missing archive.|
|WRL-002|All ecology documents unavailable|Living placement matrix/capacity exports available. Dated species-presence approvals do not select encounter weights, levels, rods or physical slots. SOW HOLD preserved.|
|WRL-003|Evolution-shop framework/prices missing|Celadon renewable supply policy,2100 stones and3000 trade items supplied. Exact early stones, badge gate identities/quests and unassigned shop-counter policy remain.|
|WRL-004|All event specifications unavailable|Events are documented individually; unfinished details and specific holds remain. Independent Museum draft#14 tracked within existing event budget, not silently integrated.|
|WRL-005|Only Ditto approved; whole Route5 roster unknown|Ecology line796 explicitly approves Ditto/Skitty/Rattata; exact levels/weights/grass allocation still unapproved.|
|PKM-006|Stationary specification unavailable / off-Dex policy uncertain|Eight SOW designs are supplied but held. Global252-only rule is explicit; strict281 authored-source findings/reference roles still require real approved reconciliation.|
|INT-003|Drive connection equated to source access|Local exports available; direct Drive sync still pending. No settings/domain-only claim of authenticated access.|

Later weather appendices resolve Kindle Road, Trainer Tower exterior, Tanoby
outdoors and event-island harbors. Do not reopen these as owner questions. Old
weather spec sections calling them unresolved are superseded by their own later
appendices. Save/load weather retention is preferred, native reroll permitted;
this is not a new mandatory persistent-storage requirement.

## Repository state / integration

Verified#13 merged at owner-approved
`d54f7a8e85d38b8fa304399dcb4d14778396d5cd` into
`feature/mystery-vendor-network` (#12), **not main**. Main remains
`e04252ea0783510000e634486c0f2526a736f506`. Other feature PRs remain open;
this new task does not inherit permission to merge or enable auto-merge.

Independent#14 Museum quiz/Shiny Feebas Egg draft is based on main at
`68e06491208d956cb0b13f77e74e10f4ecae3a90`: feature-build passed, baseline
failed at inspected head; runtime/placement/stack integration unverified. Its
`src/daycare.c` and `data/specials.inc` edits overlap the existing stack, so later
integration needs actual combined checks. Its0x0AF/0x0B0 flags do not overlap
vendor0x300..312 or Oak0x313; no collision claimed. No changes to#14 are made here.

## Typing implementation scope following this checkpoint

Base the isolated `feature/approved-typing-corrections` branch on owner-merged
vendor head d54f7a8e. Source comparison confirms exactly24 changed rows:

- Geodude, Graveler, Golem, Onix, Rhyhorn, Rhydon, Larvitar, Pupitar → pure Rock.
- Steelix → pure Steel; Pinsir → pure Bug.
- Vulpix, Ninetales → Fire/Ghost.
- Pidgey, Pidgeotto, Pidgeot, Spearow, Fearow, Farfetch'd, Doduo, Dodrio,
  Taillow, Swellow → pure Flying.
- Hoothoot, Noctowl → Flying/Psychic.

Keep all11 already-correct effective custom rows including Water/Dragon Gyarados,
and every other species/type/stat/ability/learnset/order unchanged. Separately
approved Vulpix/Ninetales stat/ability/learnset changes are outside this typing
patch. No weather-immunity or Gym team workaround is included.

Acceptance:35-row authoritative fixture, exact24-row delta and all non-type bytes
preserved; target native party→battle type loads and effectiveness/STAB/immunity
cases; summary types; saved old Pokémon refresh and ordinary evolution type load;
all prior host suites/full mGBA, clean reproducibility and independent upstream
baseline. Prepare a separate unmerged PR. No feature/runtime pass claimed yet.

## Progress accounting

All39 existing IDs and earned208 points retained. New PKM-007 budgets six pending
provisional points for newly reconciled24-row implementation/testing.40 tasks,
530 total points:208 earned/322 remaining =39.25%. The slight decrease from39.69%
reflects added tracked scope, not erased work. Metadata/source access alone earns
no new implementation credit. Museum integration remains within WRL-004, with
stable subreference WRL-004-MUSEUM and no duplicated budget.
