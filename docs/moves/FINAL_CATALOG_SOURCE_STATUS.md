# BTL-005 — final catalog source candidate through move410

This preserves the existing remaining-moves implementation and finishes the
pending nine entries402–410: Hurricane, Brave Bird, Acrobatics, Dual Wingbeat,
Heart Stamp, Zen Headbutt, Cross Poison, Poison Jab and Gunk Shot.

Authority remains docs/moves/approved-design-overlays.json, move-specification
and animation-reuse records plus their supplied owner decisions. No distribution,
learnset, TM, species or trainer allocation is invented. Existing IDs1–401 are
preserved; MOVES_COUNT411 represents NONE plus410 moves.

The final nine use existing hit/drain/recoil/double-hit/flinch/poison effects
where applicable. Acrobatics doubles base power only with ITEM_NONE. Hurricane
uses approved Rain bypass/Sun50 accuracy, normal protection/type checks and its
airborne-hit path. Existing-art animation reuse/composition adds no new pixels.
The source branch also retains prior385–401 mechanisms (all-other-active hits,
Volt Switch, Thunder Fang and the existing-effects batch); inspect these commits
rather than reimplementing them or merging older overlapping PRs blindly.

Static reconciliation executed for all nine: exact owner-approved type,category,
power,accuracy,PP,priority,secondary chance and translated effect/target/flags;
name,description and animation entries present. Source has contiguous IDs1–410
and a data/animation entry for each. This is source inspection, not a battle test.
Private pinned-toolchain compilation is in progress at initial publication.
No current gameplay/emulator tests or diagnostic workflows were executed.

This candidate originates on the preserved older remaining-moves development
stack; current-main content and overlapping PR181/183/other BTL changes must be
reconciled before merging. Do not overwrite newer trainers,events,weather,
followers or preservation guards with this branch's older versions. Compile
success alone does not certify Singles/Doubles,AI,effects,visual playback,
Substitute/Safeguard,save/link compatibility or full410-move acceptance.

Original working checkout/index and its18 pending files remain untouched; this
snapshot is committed on a separate review branch through a temporary Git index.
No generated outputs,ROM,artwork,BIOS or credentials are committed. Formal project
accounting remains461/712 until acceptance. GitHub PR records carry exact build
results and next source-integration work; no routine duplicate handoff is needed.
