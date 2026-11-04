# Approved Phase One stabilization

This branch is stacked on wild-double PR #7 (`bbb0459d95ed3d9b053304290961f4fff2de23eb`). It preserves the open #2–#7 dependency stack. Main remains the unchanged baseline. No dependency PR is merged, retargeted or rewritten.

## Authority and implemented differences

The owner approved the consolidated execution plan with “Implement the proposed plan.” The twelve typings, roster-aware evolution restrictions and functional clock boundaries come from the owner-supplied October 28, 2026 consolidated design handoff, reconciled before this work. Direct access to the linked Google Drive originals is unavailable; this document does not assert they were read. The later owner-approved Castform expansion supersedes the historical 251 count only: the effective roster has **252** species.

- **PKM-003:** Rattata/Raticate Dark; Arbok Poison/Dark; Persian Normal/Dark; Psyduck/Golduck Water/Psychic; Sandslash Ground/Steel; Pinsir Bug/Steel; Charizard Fire/Dragon; Gyarados Water/Dragon; Ampharos Electric/Dragon; Lickitung Normal/Ghost. All other species fields, abilities and types remain unchanged. Castform remains Normal with Forecast and its original forms. No Fairy type or balance edits.
- **PKM-004:** `IsEvolutionSpeciesAllowed` retains original Kanto/National access and additionally permits approved custom-roster targets before National Dex. Both normal/item and trade scene guards, the item warning and held-item trade consumption use the same predicate. The National Dex story flag is not enabled. Excluded species retain the original restriction. Original evolution methods, Everstone, cancellation policy and exact-once successful item consumption are preserved. Clamperl still has no direct Bag evolution alternative.
- **TIM-002:** Shared nonvisual `TryGetTimeOfDay` returns DAY at [06:00,18:00), NIGHT otherwise. Invalid RTC returns false without modifying the output; Eevee friendship uses this fail-closed API and the existing >=220 threshold. `GetTimeOfDay` offers DAY fallback for future environmental consumers; it does not change any existing weather or rendering. Stone methods remain independent of time/friendship. No save offset, clock-setting UI or RTC writes.
- **PKM-005 / BTL-003 / BTL-004:** Acceptance work extends the existing host and compiled-ROM harness. Exact executed results are recorded in the PR and canonical ledger; a test's existence is not a claim it passed.

This supersedes the *effective behavior* described by older branch reports that say National-Dex gates are retained or timed evolution uses noon. Those reports remain historical evidence for their own commits.

## Preservation and regression approach

The typing fixture lists both approved and original pairs. Tests verify every approved pair, reverse only those twelve modifications, then compare the original full species-table digest. Thus unrelated edits cannot be hidden by refreshing a checksum. The canonical species IDs, custom Dex order/count/flags, evolution table, trainers, save structs and Forecast code remain protected by their existing audits.

Host tests execute production routines with explicit stubs and separately report static wiring/data checks. mGBA executes the actual compiled ARM code, normal scenes/buttons, real encrypted Pokemon/item data, and real flash save/load. Item and level evolution scenes now run **before National Dex**, including Tyrogue's Hitmontop choice. An isolated Clamperl/Huntail scene fixture proves the excluded-target guard still cancels without consuming the item; it intentionally enters the scene directly and does not claim direct Bag use exists.

The shared time policy is tested at all hours, absent/invalid clock and exact 05:59:59, 06:00:00, 17:59:59, 18:00:00, 23:59:59 and midnight boundaries. Real ROM category fixtures use controlled asymmetric stats to distinguish attacking/defending stats and exercise screens, burn, abilities, real held items, type overrides, weather, AI damage estimation and fixed-level damage. The battle UI routine and authoritative category labels execute against the real ROM. Static audits of summary wiring/geometry remain distinct from runtime visual acceptance.

Reproduce with the existing README toolchain and all five host runners, `make -j2`, `python3 tests/emulator/run.py`, then `make clean`, rebuild and compare SHA-256. `LIGHTNING_BATTLE_ONLY=1` is a diagnostic; CI always runs the complete default suite. Original upstream SHA-1 comparison remains an independent CI job. No ROMs/BIOS/saves/screenshots are committed or uploaded as artifacts.

## Incomplete scope and compatibility

Direct Drive synchronization and the complete Route 5 register remain blocked. The 410-move redesign, visual lighting, dynamic weather, final trainer/encounter placements and Castform's future trainer details are not inferred or implemented. Existing wild-double test maps remain experimental, not finalized production placements.

Save structure, move IDs and canonical Dex flags are unchanged. Existing saved parties retain species; types load from the updated species data. Vanilla or differently modified link peers cannot be certified compatible with the custom split/types/evolution rules. A real two-peer link handshake, physical RTC timing, other emulator cores, a sustained playthrough, every trainer-AI strategy and visual approval of summary screens remain external/remaining acceptance work. EWRAM is already tight and future save/runtime additions require ABI and memory-budget checks.

Rollback: before integration, close this PR or leave its branch unused. After a separately approved merge, revert only this stabilization series through a new reviewed PR, preserving #2–#7. Reverting code restores old typings/gates/noon policy; it does not undo already completed evolutions or remove caught flags from saves. No migration is introduced by this patch.

## Runtime defects and additional hardening

Real summary list/detail navigation exposed a pre-existing split UI defect: the
full category at x=7 overlapped the existing EFFECT heading. The label now uses
x=57 beside that heading. A placement/width audit and actual Physical/Special/Status
screen navigation cover the correction; a local capture confirmed that power,
accuracy, category and description remain readable. Existing graphics are reused.
An initial audit trial missed a Python helper import; that test setup error was
corrected and the complete split host suite rerun successfully.

Additional actual mGBA scenarios cover all eleven ordinary Bag ball types rejecting
two surviving opponents without consumption; partner B cancellation refunding a
selected ball exactly once followed by successful retry; and a legitimate failed
Poke Ball throw consuming once, retaining the survivor, leaving its caught flag
unset and returning to action selection. Safari Ball belongs to unchanged Safari
mode and is explicitly excluded from the ordinary-Bag test. These add evidence;
all-ball successful captures, nickname acceptance/rename, player replacement and
whiteout, unusual forced-end/spread moves, complete weather and link testing remain
unverified. The wild-double release verdict remains conditional.
