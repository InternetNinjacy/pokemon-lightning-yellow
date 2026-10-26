# Evolution methods and inventory safety

## Review stack

Main currently does not contain PR #2's physical/special split. This branch is
stacked on `feature/rtc-support` (PR #3), itself stacked on PR #2. Neither dependency
is merged. Review in order: split → read-only RTC → evolution methods. No existing
split code is reverted. Once dependencies are approved/merged, retarget dependent
PRs deliberately and rerun checks; do not merge or enable auto-merge without owner
approval. No textual integration conflict was found against those current heads.

## Approved changes

Kadabra→Alakazam, Machoke→Machamp, Graveler→Golem and Haunter→Gengar each retain
EVO_TRADE and gain EVO_LEVEL at 40. A level-up to 40 or later can trigger evolution;
no evolution item is added. Normal B-button cancellation and Everstone remain.

Every existing EVO_TRADE_ITEM row is also matched by direct item use. The existing
eight routes cover Poliwhirl/Slowpoke + King's Rock, Onix/Scyther + Metal Coat,
Seadra + Dragon Scale, Porygon + Up-Grade and Clamperl + Deep Sea Tooth/Scale. Six
distinct items now expose the standard party-menu Use action. Their price, battle
held effects, parameters, descriptions and other metadata are unchanged. Original
held-item trading logic is unchanged, including consumption of the held item when
its existing conditions permit evolution. Direct use consumes one **bag** item,
not the Pokémon's held item. This is data-driven through EVO_TRADE_ITEM, not eight
species-specific checks. Future approved trade-item rows inherit lookup behavior;
a new item definition must also expose FieldUseFunc_EvoItem/ITEM_TYPE_PARTY_MENU.
No new species, items or excluded-species entries are introduced.

Eevee gains Sun Stone→Espeon and Moon Stone→Umbreon. Thunder/Water/Fire Stone rows
are unchanged. Existing friendship threshold >=220 is preserved. Timed rows now
read the RTC dependency once per lookup and use the original disabled source's
conditions: day 12:00–23:59, night 00:00–11:59. Invalid/unavailable RTC means no timed
evolution; it never invents a default time. Stones ignore time and friendship.
There is no clock-setting UI or day/night palette system. Read docs/RTC_SUPPORT.md
for required cartridge/emulator support and the outstanding hardware checks.

All existing National Pokédex and Everstone restrictions remain. In particular,
FireRed's existing UI/scene gates still prevent non-Kanto evolution before the
National Pokédex, even when lookup finds a matching method. This patch does not
unlock species or remove progression restrictions. Eggs cannot evolve. Highlight
checks may still indicate potential compatibility while an Everstone is held,
matching the existing ITEM_CHECK policy.

## Inventory lifecycle and cancellation

ItemUseCB_EvolutionStone checks target compatibility without touching inventory;
CB2_UseEvolutionStone starts the normal evolution scene through BeginItemEvolution.
The return callback removes exactly one item only if the Pokémon has actually
become the expected target species. Pending state is cleared before inventory,
quest-log and return calls, so replaying the callback cannot consume twice.
Incompatible targets, B cancellation and automatic National-Dex cancellation do
not consume the bag item. Successful item-use quest logging is deferred too.

New direct trade-item routes and the two new Eevee stone routes allow B-button
cancellation. Existing unrelated stone routes retain their original noncancelable
policy; Eevee's other three stone methods are not changed. Rare Candy consumption,
ordinary level evolution and trade animations/networking are not modified.
The original animation and species-commit sequence is reused without edits.

## Layout and scope

EVOS_PER_MON expands 5→7 to fit Eevee's seven explicit rows. Evolution records remain
three u16 fields (6 bytes); only table stride changes. Source consumers use the
shared constant; no assembly references/fixed literal strides were found. Save
layouts are unchanged. External ROM table editors assuming five rows must adapt.
Files: src/data/pokemon/evolution.h, include/constants/pokemon.h, src/pokemon.c,
src/data/items.json, src/party_menu.c, src/evolution_items.c and its header,
tests/run_evolution_tests.py, and the workflow/documentation.

## Validation

Run `python3 tests/run_evolution_tests.py`, `python3 tests/run_rtc_tests.py` and
`python3 tests/run_split_tests.py` from the checkout root. The evolution harness
executes the actual GetEvolutionTargetSpecies and item-consumption callback with
real evolution tables and stubbed Pokémon fields, bag, clock and scene launch.
741 assertions cover level 39/40/41, retained trading, every required direct-use
pair, wrong items, eggs, Everstone, no held-item mutation during item checks,
success/cancellation/replayed callbacks, old stone cancellation policy, all 24
hours at friendship 0/219/220, failed clock reads and unchanged Eevee stones.
Separate static audits check Use-menu routing and unchanged item/held-effect data.
These are not animation, live menu, encrypted save-Pokémon, physical RTC or trade
network tests. RTC tests validate decoding/logical serial IO with simulated pins,
not real cartridge/emulator clock transactions. Automated mGBA ROM integration now covers real GPIO reads, encrypted Pokémon
and bag methods, and the actual evolution scenes described in
[EMULATOR_VALIDATION.md](EMULATOR_VALIDATION.md). Full playthrough, visual UI,
physical RTC and link-peer acceptance remain outstanding. Compilation is not gameplay proof.

Actions retains modified-game tests/clean reproducibility and the independent
unchanged upstream checksum job. No ROM artifacts are uploaded. Tests/build/CI
results and failures are recorded in the PR when run. Generated ROMs and fetched
payloads are removed from the cloud checkout after verification.

## Rollback and risks

Revert the evolution commit via a reviewed PR to restore original methods, item
Use metadata and consumption path while preserving PR #2 and the separately
reviewed RTC dependency. Revert RTC separately only after removing its consumer.
Never force-push main. Remaining acceptance work: overworld bag navigation/highlighting, manual visual
checks, link-trade networking and physical RTC-capable cartridges. Automated mGBA
coverage now includes scene success/cancellation, inventory, RTC absence and hourly
friendship boundaries; see the emulator validation document for precise limits.
