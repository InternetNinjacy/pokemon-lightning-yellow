# TIM-003 lighting preview: UNAPPROVED numerical candidates

**Status:** Reversible source prototype on `feature/tim-003-lighting-preview`. No visual approval, no production merge, and no emulator captures yet.

## Build selector

`include/lighting_preview_config.h` defines `LIGHTING_PREVIEW` and sets `LIGHTING_PREVIEW_VARIANT` to 0 (A) or 1 (B). Rebuild after switching the value. Comment out `#define LIGHTING_PREVIEW` to get the original lighting. These switches exist only on this isolated source branch.

## Numeric palette manifest

The preview converts each **weather-and-fade-composited** 5-bit RGB component with `output = (input * multiplier + 16) / 32`, preserving bit 15. Only the VBlank transfer copy is modified; engine buffers, weather selection, RTC, save fields, graphics and map headers are not altered. Palette zero of each 16-color slot and BG palette slots 13–15 remain unmodified. This is a temporary approximation, not a final color treatment.

| Visual state | Candidate A R,G,B | Candidate B R,G,B |
| --- | --- | --- |
| Day | 32,32,32 | 32,32,32 |
| Dawn | 32,31,30 | 32,31,30 |
| Dusk | 32,29,26 | 32,29,26 |
| Night | 19,21,27 | 16,19,27 |
| Viridian covered Day | 25,29,25 | 25,29,25 |
| Viridian covered Night | 14,19,23 | 12,17,22 |

These values must not be copied into production without screenshot review and explicit owner approval. RGB component mapping uses native GBA 5-bit color format. The current model scales components toward black and is **not yet a fully approved blue-shift or color-quality implementation**.

## Preview behavior

- Ordinary outdoor: 06 Dawn, 07–17 Day, 18 Dusk, 19–05 Night.
- Viridian Forest map ID: covered Day 06–17, covered Night 18–05, independent of its map type.
- Lavender Town map ID: Dusk 06–17, Night 18–05.
- Invalid RTC: unchanged daytime visuals.
- Ordinary interiors and caves excluded; other forest maps remain standard.
- Every field-frame compositing pass starts from `gPlttBufferFaded`, so no repeated tint buildup.
- RTC samples approximately twice per second; first field sample is delayed until the initial counter reaches 30. Exact hour-boundary behavior still requires native verification.

## Known gaps and acceptance blockers

1. Prototype is committed but cannot be compiled or executed from this chat runtime (no network checkout, ARM toolchain or mGBA library). No native success is claimed.
2. Sprite palette colors can overlap field UI; verify all UI and battle transitions visually. No claims of complete exclusion yet.
3. Prototype map headers now request original Pokémon Tower-style `WEATHER_FOG_HORIZONTAL` on Mansion 1F/2F/3F/B1F instead of `WEATHER_SHADE`. Rendered result and weather compatibility are not yet validated.
4. Native map/time/weather capture and PNG conversion have now been coded in `tests/emulator/lighting_preview.c` and `tests/emulator/build_lighting_review.py`, plus the dedicated capture workflow. **Actual screenshots have not yet been confirmed or delivered.**
5. Need owner selection of exact candidate screenshots before production palette coding.
6. Keep this PR **draft and unmerged** until compilation, capture and owner review.

## Native execution sequence

1. Build baseline and variant A from this feature branch with `scripts/prepare-baseline.sh` and `make -j2`; run `python3 tests/emulator/run.py` after the normal host checks. Do not commit ROM/ELF artifacts.
2. Compile variant B by changing just the config number to 1, then rebuild.
3. Set `LIGHTNING_LIGHTING_CAPTURE_DIR=/tmp/lightning-lighting`, then run `python3 tests/emulator/run.py`. The new native capture mode uses the existing real map warp/RTC/weather fixture and exports actual 240x160 framebuffer PPMs. The dedicated GitHub Actions preview workflow builds candidates A/B and attempts to upload PNG-only screenshot artifacts.
4. Convert losslessly with `python3 tests/emulator/build_lighting_review.py /tmp/lightning-lighting --variant A --sheet`, then validate via `python3 tests/emulator/validate_lighting_evidence.py /tmp/lightning-lighting/manifest.json`. Never claim capture provenance from PNG alone; retain the native mGBA log.
5. Present side-by-side genuine captures for Pallet/Route 1 before deciding production colors.
