# Historical dynamic-weather checkpoint — 2026-11-04

Resumed: the One Island fixture scene state is corrected; the focused83-map,
interior/RTC/render/flash/SunnyDay and exact wild/trainer callback tests now pass.
This old checkpoint is history, not a current blocker. Current scope/evidence:
[Dynamic weather](DYNAMIC_WEATHER.md) and [PR29](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/29).

Paused on owner instruction to verify the already implemented rival contract.
Branch: feature/dynamic-map-weather; baseline mainc960642d. Rival gameplay remains
separate in PR27; no weather changes belong in that PR.

Weather implementation is IN PROGRESS, not production-ready or fully validated.
83 explicit approved map policies; nested interior/50% return context in exactly8
unused checksummed save bytes (other8 preserved); functional clock/night conversion,
logical battle weather and Cycling Road end hooks. Native clear-weather callbacks
avoid the stubbed drought loader; custom lighting/colors/Mansion fog remain queued.
Map exceptions include approved harbors, MtEmber outdoor summit and SSAnne deck.
SSAnne boarding exterior and prototype islands are unassigned and retain native
headers. Existing Sun renderer has no custom sunlight tint/pulse. No new art.

Host: tests/run_weather_tests.py PASS10880936 exhaustive accepted-roll assertions
plus state/fallback/recovery/save-tail tests. Old full host run was attempted before
correcting fixture map-group encoding and therefore did not finish; rerun all host
suites after this checkpoint, never claim that run passed. Modified source builds;
no clean reproducibility/full default emulator/CI acceptance completed yet.

Focused mGBA: all83 native policies/day/night256 draws each PASS; actual Cinnabar
Sun load/clear renderer/fade, sunset conversion, no dawn restore, absent RTC GPIO
fallback and recovered-night synchronization PASS. Later map warp hits
fossils.c fieldScriptDone assertion. Investigate OneIsland/PokemonCenter1F automatic
Bill/Celio story scenes in the blank fixture (scene variable0); set the appropriate
fixture-only progression variables rather than modifying production story scripts.
Indoor flash/deck/harbor/summit cave boundaries and Cycling battle tests are thus
NOT yet passed. Full renderer screenshots/manual approval and wider regression
remain pending. Native test comments/results must reflect only executed checks.

Key files: src/dynamic_weather.c, src/data/dynamic_weather.h,
include/dynamic_weather.h; hooks in field_weather_util.c, overworld.c,
battle_util.c, battle_setup.c and field_weather.c. Frozen delta normalizer protects
older feature hashes without replacing expected digests. New host/native tests
and approved_weather_maps.json capture scope. Save layout is unchanged.

Resume from this checkpoint on its branch; do not overwrite PR27 or main. Source
exports are locally available in /workspace/attachments/lightning-authority-2026-11-03.
No ROM/save/generated files committed. No weather PR opened or merge requested.
