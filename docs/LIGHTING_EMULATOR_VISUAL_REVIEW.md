# Lighting preview owner review sheet (not yet approved)

**Read with:** `implementation/lighting-emulator-preview-approval.md` on this same branch.

## What Codex must give the owner

A labeled comparison index with actual 240x160 in-emulator rendered frame PNGs, the code/commit that produced them, map/time/weather/position metadata, a palette variant numeric manifest, and a nearest-neighbor contact sheet. These screenshots do not exist yet; **Codex will generate them from the real game**. Do not use mockups.

## Review sequence

1. **First color direction (Pallet + route):** Verify real unchanged noon screenshot, compare prototype Night A versus optional Night B at fixed camera; approve one or request specific color/brightness/contrast changes. The only existing style approval is *Classic Pokémon night: moderate blue, visibly darker, readable*. No RGB values approved.
2. **All periods:** Compare Dawn 06:30, Day 12:00, Dusk 18:30 and Night 21:00 on same route. Verify night is navigable and dusk warmer.
3. **Special categories:** Viridian Forest covered Day/Night only; Lavender Dusk in functional DAY and Night in functional NIGHT; all four Mansion floors reuse Pokémon Tower-style fog without custom assets.
4. **Regression:** Weather overlays (Rain, one of Sandstorm or Snow/Hail), sprite contrast, building entry, menu, battle return, save/load, RTC failure/recovery, unchanged caves and Flash.
5. **Finalize:** After the owner explicitly approves screenshots and exact variant manifest values, record a new lighting approval and only then prepare a separate production implementation specification.

## Capture/evidence checklist

- [ ] Link to actual screenshot/contact sheet files supplied through a supported channel
- [ ] Capture was from compiled Pokémon Lightning Yellow ROM under mGBA, not generated illustrative art
- [ ] Every variant includes Git SHA and full parameter set
- [ ] Screenshot frames share the exact map, position, clock and weather as their comparator
- [ ] All four ordinary open sky states documented
- [ ] Forest and Lavender distinct exception rules demonstrated
- [ ] Mansion fog, interiors and caves verified
- [ ] Weather and user interface readable
- [ ] Owner screenshot approval recorded (currently **NO**)
- [ ] Final production values approved (currently **NO**)

## Possible review decisions

- **Approve visual candidate for production planning** (requires image IDs and exact palette manifest)
- **Approve with narrowly stated changes**, requiring a second captured comparison before final lock
- **Reject / revise** with precise directions

No response from the owner and no passing automated test counts as approval.
