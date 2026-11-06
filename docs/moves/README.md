# Move-system reconciliation — BTL-005

Status: **inventory reconciled; owner decisions resolved; M1a implemented; M1b UI implemented in PR41 (remote results at Checks)**.
Historical baseline inspected: `92838e5eb64d19942cb5c51cfdef6991503ac3ad`, including the tested
rival/weather/Gym integration. Main remains a separate, unmodified review baseline.
Owner requested this reconciliation and remaining automated acceptance on Nov5.
The original PR37 reconciliation changed no production code. The current foundation branch implements only the approved Curse type and safe name-buffer/summary capacity; the56 new moves and other original reworks remain unimplemented.

## Review artifacts

- [410-row CSV](move-specification.csv): complete354-source +56-new inventory.
- [Full source/approved JSON](move-specification.json): existing values, approved
  overlay, per-field basis, source lines, effects and implementation status.
- [Authored overlays](approved-design-overlays.json):21 existing redesign/type-policy
  entries including the owner-approved Curse-type row and56 new designs.
- [Readable67-entry animation review](animation-reuse.md) and [machine-readable audit](animation-reuse.json):56 new +11 focused originals.
- [TM/capacity plan](tm-and-capacity-plan.json): approved assignments, proposed
  stable-ID implementation, saved-pocket and ABI evidence.
- [Implementation/acceptance plan](IMPLEMENTATION_PLAN.md).

Every original ID1–354 is accounted for exactly once. MOVE_NONE is a non-gameplay
Status placeholder, not one of the410 moves. Current MOVES_COUNT355 is an array
bound, not a legal move. Proposed355–410 IDs are engineering reservations only,
not assigned constants. Proposed MOVES_COUNT411 includes NONE; animation count
would be412 when retaining the extra Move_COUNT sentinel. No IDs are reassigned.

333 original records retain source fields under the existing approved split;
21 have approved redesign/effect/type-policy changes, including the November5 Ghost/Status Curse decision.
All56 additions are damaging (41 named imported moves and15 custom designs,
including this project's custom Bug Bite). No rejected move is counted.
The data keeps original GenIII values, including Thunderbolt95/Thunder120 and
other unaffected moves. It does not substitute contemporary online statistics.

`approved` is a design-layer record, NOT directly compilable BattleMove data.
Semantic effects/targets name required behavior without pretending a new enum or
script exists. Null flags mean engineering translation remains, not unapproved
move selection; independent effect rolls cannot be represented as one probability
byte. Aura Sphere's accuracy0 is a proposed sentinel plus a real bypass effect,
not a0%-accuracy attack. Custom normal priority0 is marked proposed where the
source does not state priority explicitly. Fixed-damage power1 is a sentinel.

## Source and decision precedence

Owner-supplied authority exports are available locally. Direct live Drive writing
is unavailable; that does not make the supplied specifications unavailable.
The JSON source registry records exact filenames, raw SHA256 and line counts.
Evidence pointers use those exported line numbers. Relevant originals:

- [Master Move Register](https://docs.google.com/document/d/1EHByhspPMrn_H0ard-wbdZLRqInZVschQqXTWSiP5fY/edit)
- [All-Type Tracker](https://docs.google.com/document/d/1uVZe6PPLp-8C4BQKmLGJ8ei1Kvg8I-0s1CFRG3vPkwI/edit)
- [Approval Ledger](https://docs.google.com/document/d/1-b5kd3RIVwM3Glwbrh2l81Eepf48exFPB9LtWmvnhPY/edit)
- [Animation draft](https://docs.google.com/document/d/1FGuRTdlF-SBxQwBYnu0a7Hw0DkW26T-OdrDqCtg-oQk/edit)
- [Raw baseline](https://docs.google.com/document/d/1PTfPlyeNTqUE1GR6Q553P_nhhl21t5zvF74SbLwBf7E/edit), independently regenerated here from actual repository records.

All17 type-design reviews are closed. Old pending-type headings, early ORIGINAL
labels, Fire Crunch/Flame Dash/Night Terror names and old partial values are
superseded.55 ledger paragraphs match the register directly; two first entries
share section-heading paragraphs but are also present verbatim. The tracker adds
history/recaps, not a conflicting later approved lineup. No type balance reopened.

## Existing changes outside the11 animation reworks

| Original | Required difference |
|---|---|
| Roar | Normal Special70/100/15, priority0,20% Attack -1; no switching; retain TM05/access/original animation |
| Lick / Astonish | Physical40; Special50 respectively; original secondary effects retained |
| Spite | Special30/100/10; optional native2–5PP drain; absent eligible move never cancels damage |
| Grudge | Special30/90/10 native five-turn Rollout escalation, no Defense Curl; old KO effect removed |
| Scary Face / Mean Look | Ghost Status; Scary Face hits both foes in Doubles, separate90% checks |
| Twineedle / Pin Missile |40 per hit twice; single55 with30% poison respectively |
| Leech Life / Silver Wind | Special80/100/10 half drain; Silver Wind PP10 |
| Sandstorm | Ground Status; only Ground grants typing immunity; preserve other suppressions/no Rock SpDef boost |
| Rock Tomb / Icicle Spear / Outrage |65 power/90 accuracy;25 per hit;120 power |
| Dragon Rage | Night Shade-style user-level fixed damage, not ordinary stat/power damage |
| Thunder Wave | Both foes+ally, not user, in Doubles; independent checks |
| Screech / Fake Tears | Both foes only, separate native accuracy checks |
| Zap Cannon | Rain guaranteed hit/50% otherwise; native other parameters |

Electric paralysis immunity is a global rule, not merely a Thunder Wave effect.
Helping Hand/Scary Face family access is approved but exact levels remain a later
species decision. Do not mutate those learnsets to make the move engine pass.

## Source-verified capacity and integration findings

Target agbcc ABI measurement: BattleMove size12, flags offset8, category offset9;
ELF gBattleMoves is4260 bytes =355*12. Do not substitute host sizeof (which differs).
Adding56 same-size records would add672 ROM bytes; no battle/save struct expansion
is required for move IDs. Level-up encoding has9 move bits (0x1FF);410 fits.
Current effect IDs0–213 fit a u8; append/reuse must be counted before exceeding255.

Four approved names exceed MOVE_NAME_LENGTH12: Skitter Slash13, Scorching Sands15,
Petal Blizzard14, Dual Wingbeat13. Proposed remedy: support15-character names,
audit every buffer/stride/link/display consumer and use existing fonts/layout.
Do not abbreviate or rename approved moves without a real decision.

TM09 Leech Seed and TM49 Signal Beam later approvals supersede vanilla inventory
rows. TM51 Zap Cannon is separate, proposed item376 after Snow Ball375; never
insert it before existing HMs or shift saved item IDs. Existing58 TM-pocket slots
fit51TMs+7 obtainable HMs exactly; unobtainable HM08 Dive remains defined. A legacy
save containing all59 definitions must get normal full-pocket handling, not data
loss. Keep original compatibility bits0–57; append TM51 bit58 in the existing64-bit
ROM mask and use explicit item/number/HM/compatibility metadata instead of current
contiguous arithmetic. This is a proposal to implement/test, not a migration claim.

## Animation findings — no new artwork

All354 original moves resolve to existing same-name scripts. All referenced
sources for the67 candidate entries exist. Fifty formerly proposed new-move candidates are now
**OWNER APPROVED FOR IMPLEMENTATION/PLAYBACK TESTING**, four concepts are recorded composites, two Water moves have exact
reuse instructions, and Roar has an exact original visual. Label presence is
SOURCE VERIFIED; no new/composite move playback is claimed. No new sprite,
particle, background, palette or artwork is proposed. Original moves retain
original visuals by default, including reworks not in the67-row focused matrix.

## Effective owner decisions and remaining technical work

[November5 explicit owner decisions](OWNER_DECISIONS_2026-11-05.md) supersede the
previous three unresolved questions: Curse is Ghost/Status; all50 former proposed
animation assignments are approved for implementation/playback testing; Payback
and Dragon Rush use Generation IV edge cases. No runtime animation success follows
from this approval. Distribution remains Phase Two and no learner rows are invented.

M1a name-buffer capacity and Curse type are implemented in the foundation branch;
see [foundation status](FOUNDATION_STATUS.md). No56-move namespace or effects are
yet enabled. The separate [M1b UI child](LONG_NAME_UI_STATUS.md) adapts battle/relearner layouts and records its own acceptance. New IDs/effects still require correct implementation before exposure.
Foul Play modifier research, effect translation, AI and new mechanics remain
technical implementation work, not requests to reopen17 approved type reviews.

## Executed checks and limits

`python3 scripts/reconcile-move-specification.py --check` and
`python3 scripts/check-move-reconciliation.py` reproduce/verify410 identities,
all354 original IDs/categories, custom exceptions, rejected-move exclusion and67
source-present animation rows. These are **static reconciliation checks**, not
executed410-move battle tests. The target ABI measurement above was executed.
Existing tested parent native suite463604 is unchanged; it does not test the56
unimplemented moves or approved original redesigns. No new playable feature or
runtime animation success is claimed by this audit. Rebuild/CI results are linked
from this reconciliation PR when available, separately from acceptance follow-ups.

The [source baseline CSV](source-baseline.csv) contains the354 actual current
move records, including the already implemented split; it is not mislabeled as a
vanilla pre-split category table. The original upstream namespace is unchanged.

Generated records distinguish the historical baseline commit from the current
source fingerprint (`current_battle_moves_sha256`); current Curse fields are
not falsely attributed to the original baseline commit.

## M2a approved original data

[PR42 eight-move data milestone](ORIGINAL_DATA_STATUS.md) implements ten approved
fields and preserves specialized effects/other rows. Nine original policies now
have implemented source fields including Curse; twelve policies and all56 new
moves remain. Source implementation is not exhaustive runtime effect acceptance.

## M2b saved PP and Leech Life

[PR43 PP compatibility/Leech Life](PP_COMPATIBILITY_STATUS.md) safely caps old
records and implements the atomic Special80/10 revision. Ten original policies
now have implemented source fields including Curse; eleven and all56 new moves
remain. Full latest-fixture remote acceptance is recorded in PR43 Checks.
