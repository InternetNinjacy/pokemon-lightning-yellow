# Custom 252-species Pokédex

## Approved source and authorization

The owner-supplied Lightning_Yellow_Codex_Handoff_2026-10-28.docx Appendix A is
the approved 251-entry source, preserved in tests/specifications/approved_251_roster.csv.
The later standing implementation authorization inserts Castform as #233 and shifts
former #233–251 to #234–252. No previous species or relative order is removed.
The source document was supplied as an attachment; direct Drive connectivity has
not been established. src/data/pokemon/custom_pokedex.h is the explicit current
252-entry authoritative X-macro manifest used for both mapping directions.

Branch feature/custom-252-pokedex is based on evolution PR #5 at
851c76c94ac5bbcfda5a48a34bdb9a9521ab202f. This is a separate stacked PR, leaving
PRs #2–#5 and main unchanged and unmerged. Original FireRed source/license notices
and original independent checksum provenance remain intact.

## Identity, resources and saves

Internal species IDs, canonical National IDs, Pokemon structs, save-block layouts,
and canonical seen/caught bits (including both redundant seen mirrors) remain
unchanged. Existing canonical registration paths from capture/evolution still work.
The new mapping is for custom numbers/display, not indexing National resources.
Names, descriptions, sprites, footprints, cries, habitats and entry resources still
resolve using their original internal/canonical identities. Invalid canonical flag
indices are rejected instead of underflowing the bit array.

Custom count/completion iterate only the 252 included species. Species absent from
the custom roster retain internal definitions and any existing save flags but do
not appear in its numerical/habitat/sorted lists or inflate its counts. No new
external-trade rejection, species deletion or save migration is introduced.

Both pre/post National-unlock Dex views use FireRed's original single-count layout
and the custom order. The saved National-unlock story flag is not changed. The
National/Hoenn/Kanto constants and low-level flag APIs remain canonical for unrelated
story/Easy Chat consumers. Displayed Dex counts in start/save/trainers' card screens
use the custom count. HasAllMons uses all 252 custom caught entries, including
Castform and Mew, as requested by the updated completion target. Legacy Kanto-only
story/rating/reward checks remain canonical; no new diploma, reward or dialogue is
invented. Revising those story/reward requirements is a separate design task.

Existing National-Dex *evolution* gates are not removed in this roster patch. That
separately approved work and the 12 custom type overrides remain queued, preserving
a focused review boundary and the existing evolution regression expectations.

## Castform

Castform #233 reuses the existing species, Normal typing, Forecast ability,
Generation III weather/type/form implementation and graphics. No new forms, abilities,
moves, stats or weather rules are added. Kecleon is #234; Mew is #252. The existing
Celadon Gym trainer parties currently have no Castform and are unchanged.

Pending task CASTFORM-TEAM-01: assign Castform to exactly one Celadon Gym trainer
when the owner supplies the trainer identity, level and moveset. Do not choose a
placeholder trainer or deploy duplicate assignments. The eventual party change
must test exactly one approved Gym trainer contains Castform and confirm its forms
under the finalized battle weather implementation.

## Validation

Run python3 tests/run_pokedex_tests.py, all existing host suites, the established
build/clean-rebuild comparison, and python3 tests/emulator/run.py. Actions runs
these plus the separate unchanged upstream comparison, without ROM artifacts.

The new host runner compiles and executes current production mappings, counts,
completion, canonical flag access and all six list orders against actual order
arrays with stubbed label storage. It independently derives expected order by
inserting Castform into the frozen supplied CSV. Separate static hashes verify
unchanged species/Forecast/trainer/evolution/save definitions; they are not battle
or rendering tests. Future approved changes must deliberately update those hashes.

The real-ROM harness adds complete mapping/flag/count checks, Castform creation and
Forecast helper tests for Normal/Fire/Water/Ice transitions, actual flash save/reload,
pre-National Dex opening, sorted list filtering, A/DOWN navigation to Castform's
entry and B return. LIGHTNING_DEX_CAPTURE optionally writes a local PPM screenshot;
CI does not publish images or ROMs. Record actual executed results in the PR.

Remaining limits: controlled fixtures are not a full playthrough or second link peer.
Forecast helper tests do not establish full animated live-battle behavior. Restart/
Continue-menu acceptance, owner visual approval, legacy save files supplied from
outside the harness and finalized trainer placement remain separate checks.

## Rollback

Before integration, close this PR to retain #5 unchanged. After an owner-authorized
merge, revert this PR through another reviewed change. Canonical saved flags are
not renumbered; rollback restores prior displays without remapping stored species.
No merge, auto-merge, ROM release or distribution is authorized by passing tests.
