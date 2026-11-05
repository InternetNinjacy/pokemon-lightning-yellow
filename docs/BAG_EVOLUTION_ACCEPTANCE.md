# Bag evolution acceptance — PKM-005-BAG

Test-only follow-up to PR37 (move reconciliation), itself stacked on PR36/35.
Production source and saved structures are unchanged. The existing approved
six direct-use trade-item pairs and all five Eevee stones are exercised, not
redefined. Clamperl is excluded by the effective evolution specification.

## Executed focused acceptance

mGBA0.10.5 executes the actual ARM ROM. Fixture setup creates a pre-National-Dex
party with Magikarp and the eligible level30, zero-friendship Pokémon, at23:00,
with two copies of the item. Actual controller input navigates START → BAG →
USE → party selection → evolution scene → Bag → field. Engine task/saved data
assertions complement real input; evolution is not started by directly calling
BeginItemEvolution. Flash save/reload checks retain species, PID and inventory.

| Pokémon | Bag item | Result |
|---|---|---|
| Eevee | Sun Stone | Espeon |
| Eevee | Moon Stone | Umbreon |
| Eevee | Fire Stone | Flareon |
| Eevee | Water Stone | Vaporeon |
| Eevee | Thunder Stone | Jolteon |
| Poliwhirl | King's Rock | Politoed |
| Slowpoke | King's Rock | Slowking |
| Onix | Metal Coat | Steelix |
| Scyther | Metal Coat | Scizor |
| Seadra | Dragon Scale | Kingdra |
| Porygon | Up-Grade | Porygon2 |

Every row checks incompatible-target refusal without consumption, successful
species commit, consumption of exactly one Bag item, unchanged PID, no Bad Egg,
field return and flash persistence. The eight newly added item routes also check
B cancellation without consumption and successful retry. Original Fire/Water/
Thunder Stone routes preserve their original noncancelable policy; this suite
does not claim new cancellation coverage for those routes. An initial test
incorrectly expected all eleven routes to cancel; that expectation was corrected
against src/evolution_items.c and docs/EVOLUTION_OVERHAUL.md, without gameplay edits.

Eevee holding Everstone refuses the Sun Stone without consumption. Poliwhirl
holding its own King's Rock retains that held item when a separate Bag King's
Rock is used. Both are actual UI paths.

Focused command: `LIGHTNING_BAG_EVOLUTION_ONLY=1 python3 tests/emulator/run.py`.
Result: **PASS,49,176 assertions**, including guarded engine-call/ABI checks.
All16 host suites and the integration, reconciliation and Museum static checkers
also pass. The native group is registered in the default full suite, not only
behind the focus selector. Full-suite and CI results are recorded below once run.

ROM SHA-256: `923fba779dbfdc05529e40abb36ec4f8442172d7b74ff65bb5010a065724bf35`.
Local execution reuses the matching parent production ROM/ELF because this patch
changes tests/docs only. CI independently builds twice from clean and validates
the pinned unchanged upstream baseline. No ROMs or save files are uploaded.

## Remaining boundaries

No real peer link-trade, hardware test, complete manual playthrough, every item/
species/menu permutation or interrupted-save fault injection is claimed. Earlier
host/native evolution tests remain registered. This work does not implement or
runtime-validate the proposed56 new moves or20 original-move redesign policies.
Rollback: revert this acceptance-only PR; gameplay and save layout are unchanged.
