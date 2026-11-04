# PKM-007 — approved typing reconciliation

The supplied November 3 authority archive is available locally and its 40 TXT
export checksums were verified. Document availability is separate from direct
Google Drive writing, which remains unavailable. The canonical reconciliation is
[on the coordination branch](https://github.com/InternetNinjacy/pokemon-lightning-yellow/blob/docs/project-coordination/docs/SPECIFICATION_RECONCILIATION_2026-11-04.md).

Authority: the November 2 All 252 Species Typing & 67 Move Source Matrix, Part A,
checked against the later Master Plan approvals and the owner's direct Gyarados
Water/Dragon confirmation. The source SHA-256 and regional numbers are recorded in
`tests/specifications/approved_type_overrides.json`. The old twelve-rule fixture
is retained as `historical_type_overrides_pr9.json`; it is historical, not current.
Pinsir's later pure Bug approval supersedes the earlier Bug/Steel proposal.

## Exact production scope

Relative to vendor branch merge commit d54f7a8e85d38b8fa304399dcb4d14778396d5cd,
only 24 `.types` pairs in `src/data/pokemon/species_info.h` change:

- Pure Rock: Geodude, Graveler, Golem, Onix, Rhyhorn, Rhydon, Larvitar, Pupitar.
- Pure Steel: Steelix. Pure Bug: Pinsir.
- Fire/Ghost: Vulpix and Ninetales.
- Pure Flying: Pidgey, Pidgeotto, Pidgeot, Spearow, Fearow, Farfetch'd, Doduo,
  Dodrio, Taillow and Swellow.
- Flying/Psychic: Hoothoot and Noctowl.

The other eleven already-correct explicit rules remain unchanged, including
Gyarados Water/Dragon. The effective fixture has 35 explicit rules; this does not
mean 35 new source changes. All species IDs, roster order, stats, abilities,
learnsets, evolution methods, save structures and the elemental chart are preserved.
Pure types use the engine's normal duplicated pair representation.

## Verification and limits

Run `python3 tests/run_typing_tests.py`, all existing `tests/run_*tests.py`,
`make -j2`, and `python3 tests/emulator/run.py` with the documented toolchain.
The source checks reverse exactly the 24 approved pairs and require the complete
species file to match the frozen parent hash. A separate normalized hash protects
every non-type field. Older feature preservation checks normalize this explicit
approved delta; their expected baseline hashes are not replaced.

The compiled-ROM mGBA suite loads all 35 rules through real party-to-battle code,
tests 17 controlled type-effectiveness cases in both battle and AI routines, and
nine STAB cases. Six actual summary screens additionally load the approved
mono/dual type icon data (Vulpix, Ninetales, Pidgey, Noctowl, Pinsir, Steelix).
These invoke production routines; they are not a damage simulator.
Existing category, daycare, vendor, Poké Ball, evolution and save tests also run.
A successful routine-level emulator test does not certify full progression,
every summary-screen rendering, link peers or physical cartridges.

Types are derived from species data rather than new saved type fields, so existing
Pokémon obtain the revised types without a save-layout migration. Existing stored
moves/identities are preserved. Link peers must use matching game data; vanilla or
older builds can disagree on effectiveness and are unsupported.

The separate approved Sandstorm residual-damage redesign, future move redesign,
and Vulpix/Ninetales stat/ability/learnset changes are not implemented here. Existing
weather mechanics remain unchanged. No Ground immunity workaround is added to
Gyarados, and no encounter, trainer or event specification is inferred.

Rollback: revert this feature commit on its topic branch (or revert its eventual
merge), retaining the prior vendor/ball stack. No save conversion must be undone.
This PR is reviewable independently and must not be merged or auto-merged without
separate owner approval. See the PR for exact executed results and CI links.

## Concurrent work discovered during validation

PR #15 (`feature/blaine-donphan-pidgeot-double`, inspected head
4d3b1dd51af50e1f68899ed161ce36e8213ccfa2) independently changes Pidgeot to
pure Flying, matching this PR. It also edits the workflow and Pokédex preservation
tests: combine those shared-file edits deliberately and retain both test runners.
Its newer locked Donphan/Pidgeot leader specification supersedes historical Blaine
roster notes for that PR. Its trainer data changes will need explicit approved
normalization in older preservation fixtures when integrated; do not silently
replace hashes. This branch neither imports nor alters that PR. Combined runtime
validation remains outstanding. PR #14's independent Museum work also remains
outside this patch and unmodified.
