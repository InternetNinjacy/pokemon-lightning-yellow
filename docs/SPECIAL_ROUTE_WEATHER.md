# Special-route weather lifecycle (WTH / SOW-004,007,008)

Implementation slice based on main `3735096f98fc76899b23fbbeeb7b094249a09731`.
Approved decisions: [issue47](https://github.com/InternetNinjacy/pokemon-lightning-yellow/issues/47), especially LY-LW-053; priority assignment [issue105](https://github.com/InternetNinjacy/pokemon-lightning-yellow/issues/105).

## Scope and integration contract

This is an **opt-in lifecycle engine**, not installed special encounters. Current map scripts contain none of the three encounter producers. Default route weather is unchanged until a producer explicitly activates its event. Do not automatically activate these flags before providing a resolvable encounter.

| Internal ID | Event | Forced unresolved weather | Resolved baseline |
| --- | --- | --- | --- |
|0|SOW-004 Corphish, entire Route12|Rain|Rain50%, Snow50%|
|1|SOW-007 Glalie, Route21 North AND South|Snow/Hail|Rain50%, Sun50%|
|2|SOW-008 Tyranitar, entire Route23|Sandstorm|Rain/Sun/Sand/Snow25% each|

Existing baseline tables remain authoritative and unchanged. Night Sun becomes Neutral without another draw; invalid RTC retains daytime fallback. Unresolved North/South crossings consume no policy RNG. Adjacent routes retain their existing policies.

A future approved producer must:

1. On its affected route, set `VAR_0x8004` to the internal ID and invoke `SpecialWeatherActivateScript` (or use the C API). Activation must be part of the approved event's lifecycle, not a generic wild-battle callback.
2. Start the special battle. Preserve its outcome across any subsequent scripts; ordinary encounters must never resolve this event.
3. Only after successful capture/defeat AND completion of the approved aftermath, set `VAR_0x8004` to the event ID and `VAR_0x8005` to the preserved outcome.
4. `call SpecialWeather_EventScript_FinishAftermath` **before** releasing player control. It resolves once and waits for the actual current/queued field weather, loaded graphics and idle palette state. Run/loss/invalid IDs cannot resolve it.

The native wait uses the existing bytecode command0x24 explicitly because the upstream `gotonative` macro references an undeclared `source` argument. No interpreter change or additional task was introduced. A redundant `SetNextWeather` on unchanged weather is avoided: upstream leaves its completion bit false for that request. Readiness checks actual rendering state rather than trusting that bit.

## Storage and compatibility

Six source-unused persistent hidden-item flag bits191–196 (`0x4A7`–`0x4AC`) are named through `include/constants/special_weather.h`, two per event. Existing SaveBlock sizes, flags count, checksum machinery and the eight-byte regional context version remain unchanged. No Pokémon/save fields move. Resolved takes precedence over active; repeated resolution and activation of resolved events fail without drawing again. Old valid saves have the unused flags clear and retain normal baseline weather.

The host guard checks these reserved bits against map hidden-item allocations. Future item placements must not reuse them. Flash persistence is exercised in native tests; hardware power-loss and link behavior are not tested.

## Evidence and commands

Run from the documented prepared toolchain:

```sh
python3 tests/run_special_weather_tests.py
python3 tests/run_weather_tests.py
LIGHTNING_DYNAMIC_WEATHER_ONLY=1 LIGHTNING_SPECIAL_WEATHER_ONLY=1 python3 tests/emulator/run.py
LIGHTNING_DYNAMIC_WEATHER_ONLY=1 python3 tests/emulator/run.py
```

Host tests compile the actual production weather/time code:1020 new assertions, plus10,880,936 retained regional-policy assertions. All40 existing/new host runners passed. The strict new delta manifest reverses only this slice before existing frozen Gym/source checks; old baselines were not rewritten.

Native tests exercise the actual four maps, rendered weather, field-to-battle policy, persistent flash flags, capture/defeat **outcome arguments**, Run/loss rejection, all weighted results at day/night/unavailable RTC, exact RNG state and repeated-return guards. A real script interpreter runs an aftermath marker, the compiled common script, native visual wait and a post-wait marker, proving control stays locked during restoration. These are not tests of installed Corphish/Glalie/Tyranitar encounters. The focused native run passed2777 assertions, including an actual ordinary Route12 Rain→Sunny Day→Run battle preserving the unresolved override. The broader run passed131,098 assertions before that additional case; it also exercises actual weather-changing battles and Cycling Road callbacks.

CI registers the new host runner and includes these native tests through the existing full weather group. Independent pinned upstream comparison and clean modified rebuild remain separate. ROM artifacts are not uploaded.

## Remaining work / blockers

- Install actual special encounters only after exact positions, levels, moves and approved aftermath are supplied. The supplied Special Overworld register leaves coordinates/levels/moves pending; Corphish's farewell wording is provisional. Do not invent them or alter Route12 Snorlax.
- Implement persistent **individual** Corphish Drizzle and Glalie Snow Warning without species-wide changes. This slice does not implement those ability overrides or Snow Warning/Ice Body/Overcoat mechanics.
- Confirm species-wide ability-slot replacements and evolution inheritance separately; eligibility alone is insufficient.
- Actual special-event capture, defeat, loss/retry, map NPC disappearance, individual ability retention, full story progression and hardware acceptance remain unverified.
- No lighting, encounter allocation, Trainer parties, species tables or global completion percentages change here.

Rollback: revert this feature commit. No source consumer activates these flags in this slice, so rollback restores default behavior without relocating save data. If future producers are integrated, remove them together with this engine; do not repurpose the reserved persistent flags.
