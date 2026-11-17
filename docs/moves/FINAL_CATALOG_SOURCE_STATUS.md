# BTL-005 — complete catalog integrated with current main source

The original standalone candidate is7511b353 (history retained). Current
integration preserves all397 existing main move rows and fills14 previously
reserved/missing slots, for NONE plus410 approved moves. Main's existing BatchA
animations, main event/species/trainer changes, weather behavior and validation
registrations are retained. No move distribution or new artwork is added.

The older alternate spread script was removed: Discharge now uses the existing
PARALYZE_HIT effect and main's all-other-target traversal; PetalBlizzard keeps
main's exact HIT path. Thus the two implementations cannot iterate targets
twice. VoltSwitch remains the dedicated real-damage switch mechanism; ThunderFang
reuses the independently rolled fang effect, and Acrobatics/Hurricane retain the
approved item/weather behavior. TidalCrash keeps the later Nov10 NO DefenseCurl
rule; the unexecuted native fixture now checks that negative and retains a
positive Curl control for ordinary Rollout.

Compilation PASS in an isolated archive snapshot with pinned agbcc. Static
catalog/source-preservation checks PASS: check-move-reconciliation,
run_remaining_moves_tests and run_btl_weather_integration_tests, including
rejection of novel mutations. No gameplay/emulator tests or diagnostic workflows
run. The exact build head/hash and subsequent integration result are in PR265.
Runtime acceptance,animation playback,save/link and full410-move release
certification remain outstanding, handled separately from source integration.

All existing main tests/specifications files remain byte-identical. New exact
remaining_moves_current_main_delta.json reverses only this layer to verified
main; old remaining_moves_delta.json is retained as standalone history. Existing
BatchA/Water/ThunderWave/weather projections and expected hashes are preserved.
A narrow exact PR263 prize reversal allows those historical checks to see their
original source without changing actual1400/3000 rewards. Current catalog
validation checks actual411 entries; a second assertion retains the previous
accepted catalog hash. Original working checkout/index and18 pending source
files remain untouched. No points awarded:461/712 until owner acceptance.
