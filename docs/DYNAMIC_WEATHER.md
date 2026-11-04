# Dynamic weather — WTH-001, WTH-003

Authority: owner-supplied October28 Lighting & Weather Specification, its recovered
Kanto map appendix and later direct corrections; standing implementation approval.
The four source exports are available locally, not missing Drive specifications.
[Explicit map catalog](DYNAMIC_WEATHER_MAPS.md) and frozen test JSON record83 maps.
No setting is inferred for unapproved prototype or ship-boarding exteriors.

## Behavior

Five independent weather categories use the approved weights, including exact
one-third subsets and60/10/10/10/10 full sets. Selection uses rejection sampling;
night converts only the selected Sun outcome to Neutral, without renormalization.
Outdoor-to-different-outdoor entry draws fresh; immediate returns can repeat.
Same-map reloads/warps retain current weather. Nested interior visits remember the
last outdoor map/weather; returning to it restores with50% probability, otherwise
uses a fresh weighted draw. Exiting into another outdoor map always draws fresh.
Native save/load retains valid context; corrupt/legacy context simply starts fresh.

Active Sun becomes Neutral at functional night (18–06), with no dawn restoration.
Unavailable RTC uses existing environmental DAY fallback. The field polls every16
frames (about0.27 seconds at60fps) while a Sun state needs synchronization; battle
entry refreshes time immediately before automatic weather benefits. RTC recovery
does not reroll any nonsun state. Stored Sun during an interior visit also clears
at night. No timer-based random rolls or time-exclusive species are introduced.

Cycling Routes16/17/18 reroll once per wild/trainer battle return, including Run;
ordinary maps retain their field weather. Sunny Day and other battle weather do
not copy back into the overworld. The existing natural Snow→permanent-Hail bridge
is retained; it is not counted as newly implemented work. Battles consume logical
saved weather instead of waiting for an unfinished visual transition.

## Architecture and save compatibility

- `src/data/dynamic_weather.h`:83 explicit real map IDs and relative weights.
- `src/dynamic_weather.c`:selection, context, clock synchronization and battle policy.
- `field_weather_util.c`:native map-header loading routes through selection.
- `overworld.c`:field update hook; `battle_setup.c`:four normal end callbacks;
  `battle_util.c`:logical weather at field-weather battle initialization.
- Exactly the first8 bytes of existing `SaveBlock1.unused_3D24[16]` hold W/version1,
  last map, weather, indoor flag, signature and XOR. Other8 bytes are preserved.
  Existing offsets, sizes, Pokémon/daycare data, IDs and native save checksums are
  unchanged. Invalid context cannot discard Pokémon or rewrite other save fields.
- Existing map JSON/warps/travel permissions remain unchanged. Explicit approved
  harbors and MtEmber summit are weather exteriors despite their engine types.
  MtEmber cave paths and event-island cave/summit interiors remain weatherless.

## Rendering boundary

FireRed's drought loader is stubbed and can stall. Mechanical WEATHER_DROUGHT now
uses existing clear/Sunny callbacks and a normal fade endpoint. No new tint/color,
art, particles, palette table or imported project code is added. Sun therefore
has clear daytime field appearance, with real sunlight mechanics in battles.
Custom Sun visuals, final day/night tinting, covered periods, Lavender lighting
and Mansion fog remain separate WTH-002/TIM-003 work requiring their own evidence.
Neutral forest weather retains the native shade header. Other existing weather
renderers/assets and licensing notices are preserved.

## Validation

Run every `tests/run_*tests.py`, Museum static checker, `make -j2`, full native
`python3 tests/emulator/run.py`, and a clean reproducibility comparison. CI also
builds unchanged pinned upstream against its original SHA-1; no ROM upload.
Host tests execute production policy and existing time-of-day code with controlled
RNG/GPIO/state stubs. Frozen reverse deltas preserve every unrelated source byte
and old feature hashes without requiring Git history in shallow checkouts.
Native tests use target-generated ABI, actual GPIO absence/recovery, loaded maps,
flash save/load, weather render/fade completion, actual Sunny Day and real battles.
Static source preservation is not a claim of full story/hardware verification.

Full manual world progression, all83 rendered-map screenshots, exhaustive quest-log
playback, physical RTC/cartridges, link peers and interrupted flash remain external
acceptance. Save backward downgrade can discard new weather metadata; preserve
user saves and do not claim tested reverse migrations. Before downgrading, back up
the save and save inside an ordinary weatherless building: an outdoor Sun save
can reach the original broken drought loader on old-ROM Continue (source-level
risk; reverse migration not executed). Source rollback: revert
weather commits back to baseline mainc960642d. Rival PR27 remains separate.

## Integration with PR27

Rival modifies trainer/learnset data; weather modifies field/battle weather hooks,
so game-source scopes do not overlap. Both PRs edit test registration, the old
snapshot normalizer, and live tracking. A combined integration must preserve both
sets of tests/ABI/header generation and both frozen normalizers before the Ranger
normalizer, then run full combined acceptance. Do not overwrite either PR or
interpret a standalone build as combined integration proof.

Read-only merge-tree analysis against rival head62c99419 found seven conflicts:
`.github/workflows/build.yml`, the three live tracker/ledger files,
`tests/approved_ranger_changes.py`, `tests/emulator/run.py` and `runner.c`.
No gameplay-source conflict was reported. This is conflict analysis, not a
successful combined build/runtime test. Resolve both feature registrations and
normalizers explicitly; never drop either suite to make integration pass.

Ongoing battles retain native entry-time weather; field Sun synchronizes on return
and before new battle benefits. A single logical weather snapshot supplies both
the initial battle effect and message, avoiding inconsistent RTC boundary reads.
