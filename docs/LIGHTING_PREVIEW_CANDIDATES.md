# TIM-003 lighting preview: UNAPPROVED numerical candidates

**Status:** Reversible source prototype on `feature/tim-003-lighting-preview`. Candidate B Night selected by owner; stronger Dawn and Dusk unapproved, no production merge. Previously captured genuine A/B mGBA evidence is archived in GitHub Actions run 42395893275; revised evidence to follow.

## Build selector

`include/lighting_preview_config.h` defines `LIGHTING_PREVIEW` and sets `LIGHTING_PREVIEW_VARIANT` to 0 (A) or 1 (B). Rebuild after switching the value. Comment out `#define LIGHTING_PREVIEW` to get the original lighting. These switches exist only on this isolated source branch.

## Numeric palette manifest

The preview converts each **weather-and-fade-composited** 5-bit RGB component with `output = (input * multiplier + 16) / 32`, preserving bit 15. Only the VBlank transfer copy is modified; engine buffers, weather selection, RTC, save fields, graphics and map headers are not altered. Palette zero of each 16-color slot and BG palette slots 13–15 remain unmodified. This is a temporary approximation, not a final color treatment.

| Visual state | Candidate A R,G,B | Candidate B R,G,B |
| --- | --- | --- |
| Day | 32,32,32 | 32,32,32 |
| Dawn (revised pending visual approval) | 23,28,32 | 23,28,32 |
| Dusk (revised pending visual approval) | 32,23,17 | 32,23,17 |
| Night | 19,21,27 | 16,19,27 |
| Viridian covered Day | 25,29,25 | 25,29,25 |
| Viridian covered Night | 14,19,23 | 12,17,22 |

Candidate B Night is approved as a direction from genuine mGBA comparisons. Dawn and Dusk values above are revised for more noticeable cool/warm separation, and still require owner review before production. RGB component mapping uses native GBA 5-bit color format. The current model scales components toward black and is **not yet a fully approved blue-shift or color-quality implementation**.

## Preview behavior

- Ordinary outdoor: 06 Dawn, 07–17 Day, 18 Dusk, 19–05 Night.
- Viridian Forest map ID: covered Day 06–17, covered Night 18–05, independent of its map type.
- Lavender Town map ID: Dusk 06–17, Night 18–05.
- Invalid RTC: unchanged daytime visuals.
- Ordinary interiors and caves excluded; other forest maps remain standard.
- Every field-frame compositing pass starts from `gPlttBufferFaded`, so no repeated tint buildup.
- RTC samples approximately twice per second; first field sample is delayed until the initial counter reaches 30. Exact hour-boundary behavior still requires native verification.

## Known gaps and acceptance blockers

1. Prototype is committed but cannot be compiled or executed from this chat runtime (no network checkout, ARM toolchain or mGBA library). Previous mGBA candidate runs compiled and captured genuine frames; revised stronger Dawn/Dusk values require recapture.
2. Sprite palette colors can overlap field UI; verify all UI and battle transitions visually. No claims of complete exclusion yet.
3. Prototype map headers now request original Pokémon Tower-style `WEATHER_FOG_HORIZONTAL` on Mansion 1F/2F/3F/B1F instead of `WEATHER_SHADE`. Rendered result and weather compatibility are not yet validated.
4. Native map/time/weather capture and PNG conversion have now been coded in `tests/emulator/lighting_preview.c` and `tests/emulator/build_lighting_review.py`, plus the dedicated capture workflow. Earlier 24-frame captures per candidate passed. The revised Dawn/Dusk comparison is pending a new native capture.
5. Owner selected Candidate B for Night. Dawn and Dusk were too subtle and require another real screenshot review.
6. Keep this PR **draft and unmerged** until compilation, capture and owner review.

## Native execution sequence

1. Build baseline and variant A from this feature branch with `scripts/prepare-baseline.sh` and `make -j2`; run `python3 tests/emulator/run.py` after the normal host checks. Do not commit ROM/ELF artifacts.
2. Compile variant B by changing just the config number to 1, then rebuild.
3. Set `LIGHTNING_LIGHTING_CAPTURE_DIR=/tmp/lightning-lighting`, then run `python3 tests/emulator/run.py`. The new native capture mode uses the existing real map warp/RTC/weather fixture and exports actual 240x160 framebuffer PPMs. The dedicated GitHub Actions preview workflow builds candidates A/B and attempts to upload PNG-only screenshot artifacts.
4. Convert losslessly with `python3 tests/emulator/build_lighting_review.py /tmp/lightning-lighting --variant A --sheet`, then validate via `python3 tests/emulator/validate_lighting_evidence.py /tmp/lightning-lighting/manifest.json`. Never claim capture provenance from PNG alone; retain the native mGBA log.
5. Present side-by-side genuine captures for Pallet/Route 1 before deciding production colors.
