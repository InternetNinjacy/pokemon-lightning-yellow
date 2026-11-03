# Poké Ball redesign — implementation readiness

Task **BTL-006**. Audit date:2026-11-03. **READY for scoped implementation**;
production code has not been changed by this readiness audit. The owner's latest
request stops this step at readiness. Standing task-specific coding authorization
already exists; a fresh generic approval is not a technical prerequisite.

## Authority and verified base

Read all three supplied November2 Poké Ball exports completely: the Design
Register & Implementation Specification, Codex Deferred Task Brief, and Capture
Code & PR9 Source Parity. The brief and register explicitly supersede their own
older design-only paragraphs. Also reviewed canonical START HERE and change-intake
protocol, the relevant Master Plan/Work Queue amendments, and weather-duration
and Snow/Hail source passages. This is not a claim of having read every unrelated
paragraph of the283KB Master Plan or every supplied specialist register.

- [Canonical Poké Ball register](https://docs.google.com/document/d/1KhbeAi2hqUgGmrlaih34Pzy-K8-AKgYGa_5sePQqMzA/edit).
- [Owner-authorized task brief](https://docs.google.com/document/d/1wmIGcbElIxLWIB_xq1rDSLN-kI3Cx3H-bW4Opewz9-c/edit).
- [Historical source-parity audit](https://docs.google.com/document/d/1diRzhnhyOt7Kd0_Ihob51_E0ssVC6PJA9tNv7f3utcc/edit).
- Actual exports: `01_POKE_BALLS` in the November3 authority archive. They are
  accessible locally; do not ask for that ZIP again. Direct authenticated Drive
  read/write is still unavailable; links alone were not used as proof of access.

Audited checkout: `feature/mystery-vendor-network`,
**92a0600d819fe818f948e8d72bd1591531b4a07c**. Main remains
`e04252ea0783510000e634486c0f2526a736f506`. Rechecked GitHub: only#1 merged;
#2–#12 remain open, with#8 the separate coordination branch. PR#12 is ready,
not merged, with both modified/upstream checks successful at this exact head.
[Final baseline for this audit](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41673840642):
eight host suites, clean reproducibility and full mGBA273355 assertions passed.
Those tests validate the existing game, **not the unimplemented ball redesign**.

Recommended new branch when coding starts: `feature/poke-ball-redesign`, based on
that exact#12 head; separate PR targeting `feature/mystery-vendor-network`. Do not
merge or rewrite parents. This is a proposed branch, not one created by this audit.

## Locked effective requirements

|Ball|Approved effect / acquisition|
|---|---|
|Poké / Great / Ultra|Original1x /1.5x /2x; prices200/600/1200; original standard progression|
|Master / Safari|Original special behavior/acquisition; never regular shop stock|
|Repeat / Timer / Luxury|Original GenerationIII capture/friendship mechanics|
|Wet (old Dive)|3x for Surfing origin, fishing origin, active Rain or Hail; otherwise1x|
|Dust (old Net)|3x for active Sandstorm or Harsh Sunlight; otherwise1x, including Hail|
|Nest|Original level formula; initial Oak gift becomes five Nest Balls|
|Premier|3.5x on exactly Articuno, Zapdos, Moltres, Raikou, Entei, Suicune, Mewtwo, Mew; otherwise1x|
|Snow (one new item)|3x for active Hail or cave; otherwise1x; all Dive/Wet artwork reused unchanged|

Conditions combine with OR, never multiply together. Weather is evaluated at
throw time, not from a stale map header. Weather moves/abilities and expiration
must change eligibility. No modern Snow Defense bonus or unrelated mechanics.
Each one-variety purchase earns floor(quantity/10) Premier Balls; partial groups
never pool across varieties or transactions. Premier is not directly sold.
Seven specialty types cost1200 from Vermilion onward; standard prices/progression
stay intact. Preserve original item/ball graphics, palettes, particles and scripts.

## Source findings and selected technical plan

### BTL-006-M01 — Item identity, storage and graphics

**SOURCE VERIFIED:** include/constants/items.h:1–16 retains ball item IDs1..12,
Potion starts13, existing last item Sapphire374, ITEMS_COUNT375. PokemonSubstruct3
in include/pokemon.h:40–48 stores the captured ball in **four bits**. Bag item IDs
are16-bit and the ball pocket already has13 slots (include/constants/global.h:38).
Controllers exchange the captured-ball field as one byte. Blindly saving new
item375 into four bits would truncate it; assigning Snow item13 would overwrite
saved Potions. Neither approach is acceptable.

**TECHNICAL PLAN:** append Snow bag item375; increment ITEMS_COUNT, move the
icon-table sentinel accordingly; preserve all existing item numbers. Keep Wet/Dust
at old Dive7/Net6 with named aliases. Use unused **capture code13**, distinct from
bag item13, for caught Snow Pokémon. Add explicit item↔capture-code↔visual mapping;
keep MON_DATA_POKEBALL/controller packets as the original small stored code. Audit
all consumers: src/pokeball.c, src/battle_anim_special.c,
src/pokemon_summary_screen.c, capture setter paths and controllers. Never treat
capture code13 as a Potion or serialize375 into one byte. Preserve ordinary
Luxury checks and existing caught-ball codes1..12. No Pokémon/save layout change.

Map Snow's item icon, throw, particles, send-out and summary icon to the existing
Dive pointers/visual index. Keep the12-entry visual enum/tables unchanged; a
thirteenth item does not require thirteenth artwork. Existing matching-build save
and controller tests must prove this design before acceptance. Old-ROM use of
new items is unsupported; document backup/reversal limits, not bidirectional
compatibility. Same-build link tests remain distinct from real two-peer testing.

### BTL-006-M02 — Capture and battle context

**SOURCE VERIFIED:** src/battle_script_commands.c:9453–9605 has original Net/Dive/
Premier behavior. Multipliers are tenths (30/35 fit u8). Preserve the integer
operation order of catch, HP and status modifiers. The helper should return the
multiplier; existing capture success/failure and target selection stay authoritative.
Use species IDs for the eight-name whitelist, not custom Dex numeric comparisons.

Audit range assumptions: src/battle_main.c:4170 indexes a13-entry throw-script table
by item ID; player and Oak controller cancellation paths compare <=Premier;
BattleResults.catchAttempts[11] at include/battle.h:372 is indexed by item-2.
Replace classification with a shared valid-ball predicate and safe dispatch.
Legacy statistics remain bounded to their existing slots; do not index them with375
or relabel Snow as another ball's attempt. A separate Snow attempt counter is not
required by the approved gameplay spec. Preserve Master/Safari/tutorial/ghost/
trainer behavior, wild-double survivor targeting, exact-once refund/consumption
and full party/PC rejection.

**SOURCE VERIFIED:** FishingWildEncounter in src/wild_encounter.c:559–565 calls the
same StartWildBattle used by other encounters; battle terrain alone cannot prove
fishing. Add small transient encounter-origin context captured at entry and reset
on every battle setup, including roamer/scripted/trainer/tutorial paths. Mark
Surfing from the native avatar state; mark fishing explicitly. No persistent save
field. Snow cave condition uses MAP_TYPE_UNDERGROUND; indoor houses/towers/dark
lighting alone do not qualify. Test all origins and stale-context transitions.

### BTL-006-M03 — Current weather and the narrow Snow bridge

**SOURCE VERIFIED:** actual weather masks are in include/constants/battle.h:206–218;
battle_util.c clears temporary masks at expiry. AbilityBattleEffects translates
field Rain/Sandstorm/Drought at battle entry, but has no WEATHER_SNOW case.
gWeatherStartsStringIds[WEATHER_SNOW] even defaults to a rain message. Merely
checking a snow map at throw would incorrectly retain a bonus after Sunny Day.

Use current gBattleWeather masks for the approved ball conditions. A weather-
suppressing ability does not clear that state; do not add an unapproved type or
weather-effects filter to the ball predicate. Natural Snow needs a narrow bridge
into actual Hail, its existing native animation/message and end-turn processing.
Follow existing natural Rain/Sand/Sun lifetime conventions: natural Hail persists
until weather replacement/battle end; move-created Hail keeps its native duration.
This is the recommended consistent technical interpretation, not a newly quoted
owner duration decision. A named permanent-Hail flag in the existing16-bit weather
mask can implement it without save-layout changes. Test replacement/expiry and
Forecast, and document this implementation choice prominently for review.

Do not implement probabilistic regional weather, time/lighting, new weather
abilities or map distributions in this patch. Those independent workstreams are
not required to test current capture predicates and this narrow bridge.

### BTL-006-M04 — Shop transactions and stock

**SOURCE VERIFIED:** src/shop.c:978–1011 adds the bought quantity, later charges
money, and has no Premier reward. Native shopping selects one SKU per transaction;
there is no mixed cart. Native bag stacks cap at999 (src/item.c:166–260).

Preflight the entire bought quantity plus floor(quantity/10) Premier bonus against
one scratch copy of the ball pocket, then commit both and money exactly once.
If either cannot fit, decline without charging or losing a promised reward.
This all-or-nothing policy is the recommended technical full-bag safeguard;
no persistent bonus debt or new gameplay currency. Preserve purchase statistics,
quest-log flow and unrelated purchases/selling. Reward all qualifying purchase
sources, not gifts, usage, refunds or sales. Avoid separately checking two empty
slots against the same unchanged bag, which could incorrectly approve both.

Normal late city/League stock targets: Vermilion, Lavender, Celadon Department
Store2F ordinary-items counter, Fuchsia, Saffron, Cinnabar, Indigo Plateau Center,
Three/Four/Six/Seven Island Marts (11 stock lists). Add all seven specialty balls;
preserve every other item and standard-ball unlock. Viridian/Pewter/Cerulean stay
unchanged. One/Five Island have no equivalent native Mart to invent.

**Scope interpretation:** preserve Two Island's four-stage story-dependent street
vendor and Trainer Tower's facility stock as exceptional retailers, not ordinary
city Marts; do not rewrite their progression. They still earn the universal Premier
purchase reward on balls they sell. Any requested expansion of these exceptional
stock lists is a separate distribution clarification, not a blocker to the stated
city-Mart implementation. Report this boundary in the eventual PR.

### BTL-006-M05 — Oak gift and integration safety

**SOURCE VERIFIED:** initial gift is scripts.inc:660; a separate Route22 emergency
refill is at935. Both currently use the same ReceivedFivePokeBalls text. Change
only the initial parcel/Dex gift to Nest and give it separate accurate text;
keep the emergency refill, starter choice, rival, parcel and Dex progression.

Audit native giveitem_msg failure routing. If the ceremony continues after a full-
bag rejection, retain an explicit pending-five-Nest state and let Oak retry that
same gift once space exists; never silently lose/duplicate the approved reward.
Do not retroactively grant another gift to old saves that already received the
original balls. Use a verified unused event bit if needed, not a moved save field.
Future Oak Eevee/Pikachu changes must integrate against this precise event delta.

Vendor preservation tests fingerprint the whole old Oak and battle files. Update
them to exempt only these newly approved functions/blocks, while continuing to
fingerprint the unrelated content; do not refresh whole-file hashes blindly or
disable old tests. Keep all ten vendor transactions/native scenarios unchanged.

### BTL-006-M06 — Acceptance and PR

|Validation|Required evidence before completion|
|---|---|
|Capture math|Execute production helper and native handler: all13 balls, eligible/ineligible weather/origin/cave, nonstacking, all eight whitelist names and counterexamples, original HP/status/Timer/Nest/Repeat semantics|
|Environment|Native rain/Drizzle/Rain Dance, sun/Drought/Sunny Day, sand/Sand Stream/Sandstorm, temporary Hail expiry, natural Snow bridge, weather replacement, cave/indoor distinction, fishing/Surfing resets|
|Graphics and encoding|Original art hashes and shared pointers; actual Snow throw/catch/failure, stored code13, summary, PC, save/load and subsequent send-out; existing codes1..12/held Potions unchanged|
|Shopping|Actual purchase UI for all eligible types; quantities1/9/10/19/20/30/99; no cross-transaction pooling; cancel/insufficient money; full pocket/new stack/999 overflow; bonus fits and fails; money/items/stats saved exactly once|
|Stock|All11 intended lists contain exactly approved additions/prices; no Master/Safari/Premier sales; early and exceptional stock/progression unchanged|
|Oak|Actual initial event five Nest, full-bag retry, one-time persistence; old saves and separate emergency refill; no starter/Eevee redesign|
|Wild doubles|Snow and legacy balls reject two living opponents; both survivor positions; exact-once partner cancellation refund; failed/successful capture, party/PC and overworld return|
|Regression|All eight existing host suites plus new capture/shop suite, target-generated ABI guards, full default mGBA, clean matching rebuild, independent pinned upstream SHA-1 and GitHub Actions; no ROM artifacts|

Current execution status for **new** mechanics: not implemented, no new-runtime
pass claims. Existing build tools, pinned compiler/binutils, libmGBA and GitHub
access are already present; no local owner installation, purchase, ROM upload or
new authentication is needed to begin. Real peer/hardware/full-playthrough checks
remain separately reported, never inferred from host/static or emulator fixtures.

## Readiness decision and completion boundary

No essential missing specification or external permission blocks the core scope.
Technical choices above are explicit, narrow and reviewable. The most significant
risks are four-bit storage, legacy item-indexed arrays, exactly-once purchase/gift
state, natural-Hail lifetime and stock exceptions. Address these first rather than
renaming items and declaring completion. If runtime proof disproves the proposed
encoding/transaction plan, revise the technical design without changing approved
mechanics; escalate only a genuine irreconcilable gameplay requirement.

Next: implement M01→M02/M03→M04/M05→M06 on the proposed separate branch. Stop this
readiness step here. Dashboard/ledger keep prior completed work and no coding or
test credit is earned merely from design approval. No merge, auto-merge or release.
