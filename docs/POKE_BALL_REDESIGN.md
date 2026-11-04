# Approved Poké Ball redesign — BTL-006

Owner approved the final November2 register/task and subsequently explicitly
removed the readiness stopping point. Implement finalized work, ask only for
missing essential information or material conflicts. No merge, auto-merge or
release permission. Source exports were read fully; no direct Drive access claim.

Authority: https://docs.google.com/document/d/1KhbeAi2hqUgGmrlaih34Pzy-K8-AKgYGa_5sePQqMzA/edit
and https://docs.google.com/document/d/1wmIGcbElIxLWIB_xq1rDSLN-kI3Cx3H-bW4Opewz9-c/edit.
Readiness audit lives on docs/project-coordination. Base vendor#12 is
92a0600d819fe818f948e8d72bd1591531b4a07c. Work is isolated on
feature/poke-ball-redesign, with a separate PR targeting that branch, no parent
rewrite/worktree/main changes. Current implementation/testing status below is
not a full project release certification.

## Implemented scope

- Wet is old Dive item7; Dust is old Net item6. Old constants remain aliases;
  no existing saved item number changes. Wet3x on Surfing/fishing origin or actual
  current Rain/Hail; Dust3x on actual Sandstorm/Sun, not Hail. Otherwise1x.
- Snow is one new bag item375. Cave (MAP_TYPE_UNDERGROUND) or actual Hail gives3x,
  otherwise1x. All qualifying conditions OR, never stack. Only current battle
  masks are consulted; expired/replaced map weather does not persist as a bonus.
- Premier3.5x for exactly Articuno/Zapdos/Moltres/Raikou/Entei/Suicune/Mewtwo/Mew;
  otherwise1x. Species IDs, independent of custom Dex display numbering.
- Original Poké/Great/Ultra/Master/Safari/Nest/Repeat/Timer/Luxury mechanics remain.
  Preserve original integer catch/HP/status order and original Timer counter
  behavior rather than importing modern rules. Master remains guaranteed in
  eligible wild battles; ghost/trainer/tutorial exceptions stay native.
- Every qualifying single-variety purchase yields floor(quantity/10) Premier;
  all-or-nothing pocket preflight prevents losing bought balls or bonus. No
  cross-transaction/type pooling, no bonus on gifts/selling/refunds. Money remains
  in native shop flow, charged once after delivery; purchase statistics/quest log
  are retained. Bonus message reports actual quantity. A full/overflowing bonus
  stack declines the whole purchase rather than silently dropping free balls.
- Wet/Dust/Nest/Repeat/Timer/Luxury/Snow1200 at11 normal late city/League stocks:
  Vermilion, Lavender, Celadon2F ordinary counter, Fuchsia, Saffron, Cinnabar,
  Indigo Plateau Center, Three/Four/Six/Seven Island Marts. Standard stock and
  prices retain progression. Earlier Viridian/Pewter/Cerulean unchanged; no new
  One/Five Island shop invented. Preserve TwoIsland staged street vendor and
  TrainerTower facility stock as exceptional retailers; universal bonus still
  applies to their eligible purchases. Report this explicit scope interpretation
  for review; no unapproved exceptional-stock progression rewrite.
- Initial Oak parcel/Dex reward is five Nest with separate accurate text. An
  otherwise full bag leaves persistent pending flag0x313 and Oak retries once
  there is room. The native giveitem_msg macro unconditionally displays receipt,
  so the helper checks AddItem success before playing receipt. Old claimed saves
  receive no retroactive new gift. Separate original Route22 five-Poké refill,
  starter/rival/parcel/Dex progression preserved. No early Eevee design invented.

## Save, controller and artwork design

Bag ID375 is deliberately **not** stored directly in Pokémon's four-bit ball
field. Explicit conversion stores unused capture code13 and decodes only in
captured-ball visual contexts. All old codes1..12,16-bit bag IDs and one-byte
controller packets stay intact. Potion remains item13, unrelated to capture code13.
Snow and Potion coexist and survive actual flash load in native tests.
No Pokémon, SaveBlock or pocket capacity layout changes. The existing13-slot
ball pocket supports all13 types. Artwork count stays12: Snow icon/palette,
particles/throw/send-out/summary all reuse original Dive assets and visual index.
No new artwork, palette or imported animation system.

Shared ball predicate and safe throw-script dispatch replace <=Premier assumptions,
including double-battle cancellation refunds. Existing11 catch-attempt counters
are bounded to original item indices; Snow does not index outside them or falsely
increment Dive statistics. No new persistent statistic layout is introduced.
Transient encounter origin resets on every battle-start task; fishing explicitly
marks its path after normal battle dispatch, Surfing uses native avatar flags.

## Narrow natural Snow dependency

Original field weather code lacked a Snow→Hail battle branch and used a default
rain message. Added actual permanent natural Hail flag in the existing16-bit
battle-weather state, correct native Hail message/animation and end-turn check.
Natural Hail follows existing natural Rain/Sand/Sun lifetime until replacement or
battle end; move Hail remains its original five turns. This is an explicit
technical interpretation matching existing field-weather conventions, not a
claimed separate owner duration quote. No regional weather probabilities,
lighting, new weather abilities or GenerationIX Snow effect introduced.

## Validation and evidence status

Build currently passes. All eight preceding host suites pass with old preservation
hashes retained. tests/approved_ball_changes.py reverses only frozen approved
context hunks before those hashes, rather than changing the old baseline or
allowing arbitrary edits. New tests/run_capture_tests.py passes7315 executed
production-helper assertions and separate price/stock/graphics audits.

Focused native tests have passed policy/whitelist/origin/cave cases, all13 capture
codes, genuine encrypted Snow/Potion flash persistence, eligible purchase quantities
1/9/10/19/20/30/99, full bonus-stack rejection, mixed-type nonpooling and Oak's
actual full-bag NPC retry. Real Snow Bag guard, both survivor-side capture
animations, exact consumption, partner cancellation refund and saved code13 pass.
Actual Mart buy/quantity10/confirm/return UI delivered10 Poké plus1 Premier,
charged2000 once and survived flash. These are controlled native emulator tests,
not a manual full playthrough. Full-suite/clean/Actions results belong in the PR
and final canonical ledger; work is not complete merely from these focused passes.

Failed trials: initial C89 declaration placement and use of a declared but absent native
plural-name implementation prevented builds; corrected without changing mechanics.
A cave fainting fixture encountered Cloyster's legitimate priority Protect choice;
the capture-focused fixture now controls opponent Growl choices instead of relying
on Protect RNG. Native shop tests initially pressed during fade/text printing;
now wait for actual task/input readiness. A four-Growl/Hail turn exceeded the
2400-frame general helper bound; a separate12000-frame stress check completed
in3480 frames, without a production change. Six Splash-controlled weather turns
passed independently. No assertions or ABI guards disabled.

Reproduce with upstream toolchain prepared as README/INSTALL describe:

```
python3 tests/run_capture_tests.py
# Run all prior tests/run_*_tests.py suites as listed in the workflow.
make -j2
python3 tests/emulator/run.py
# Diagnostic subsets never replace the full required run:
LIGHTNING_BALLS_ONLY=1 python3 tests/emulator/run.py
LIGHTNING_BALL_SHOP_ONLY=1 python3 tests/emulator/run.py
LIGHTNING_BALL_WEATHER_ONLY=1 python3 tests/emulator/run.py
sha256sum pokefirered.gba > /tmp/balls.sha256
make clean
make -j2
sha256sum -c /tmp/balls.sha256
```

## Remaining acceptance and rollback

No real two-peer link handshake, physical cartridges, abrupt flash power-loss,
manual complete progression or exhaustive quest-log replay performed. Same-build
raw controller codes/save format are retained, not proof of old-ROM interoperability.
Future dynamic weather, exact starter/Eevee integration and broader species/move
redesign remain separate workstreams; the ball engine does not depend on their
unfinished design choices. Complete weather/ability/Surf/fishing and PC/summary
presentation acceptance must be reported by actual executed coverage, not static
identifier existence. No global roster compliance claim from the existing no-new-
findings audit; unrelated off-Dex assignments remain unresolved.

Back up saves before testing. Revert only this topic's commits against92a0600d,
preserving parents/unrelated work. Restoring a pre-feature save is required for
full reverse progression: new item375/pending gift/code13 are not supported as
new features by old builds. Never shift legacy item IDs, erase Pokémon or reset
main to implement rollback. No ROM/BIOS/save/build artifacts uploaded. Canonical
dashboard/append-only ledger remain in PR#8; direct Drive sync pending.
