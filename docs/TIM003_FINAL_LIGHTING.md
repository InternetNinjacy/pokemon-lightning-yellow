# TIM-003: approved production lighting

**Design approval:** owner approved November 7, 2026. The former reversible A/B prototype was resolved into one fixed production palette. No branch-time or runtime candidate switching is permitted. Historical internal `LIGHTING_PREVIEW` names are retained solely to minimize source refactoring risk; these hooks are enabled in the production build.

## Approved RGB5 color multipliers

Every channel uses `(input * multiplier + 16) / 32`, applied once to the composed hardware palette after the normal DMA transfer. The game's original weather, palette fade, RTC, save data and source buffers are preserved. Color zero of each 16-color slot and BG palette slots 13-15 remain unchanged.

| Appearance | R | G | B | Approval |
| --- | ---: | ---: | ---: | --- |
| Ordinary daytime | 32 | 32 | 32 | Original daylight preserved |
| Ordinary dawn | 23 | 28 | 32 | Stronger blue dawn approved |
| Ordinary dusk | 32 | 23 | 17 | Stronger amber dusk approved |
| Ordinary night | 16 | 19 | 27 | Candidate B approved |
| Viridian Forest covered day | 25 | 29 | 25 | Candidate A approved (identical to B daytime) |
| Viridian Forest covered night | 12 | 17 | 22 | Candidate B approved |
| Lavender Town soft daytime | 30 | 22 | 19 | Softened daytime visual direction approved |

## Schedule and exceptions

- Ordinary outdoors: 06:00-06:59 dawn; 07:00-17:59 day; 18:00-18:59 dusk; 19:00-05:59 night.
- Functional game day/night is separately governed by the RTC day/night API (06:00-17:59 day; otherwise night), and its existing code/tests must remain unchanged.
- Viridian Forest: covered day 06:00-17:59, covered night 18:00-05:59.
- Lavender Town: soft daytime 06:00-17:59, approved ordinary night 18:00-05:59.
- Pokémon Mansion 1F, 2F, 3F, B1F: existing moving horizontal weather fog, reduced blend target (6,12) rather than (12,8) on these maps only. Pokémon Tower stays at original strength.
- Ordinary caves/interiors excluded from the outdoor palette. RTC invalid fallback is neutral daylight. Other weather effects remain active.

## Testing and integration

The production capture workflow builds the actual ROM, exercises real headless mGBA and records 24 native 240x160 screenshots, SHA256 hashes, source SHA, manifest and emulator log. The standard full CI also exercises source-preservation, ROM determinism, title screen and native gameplay tests. Do not attach ROMs, save data or proprietary assets to PR evidence.

Evidence at the approved prototype source: [native capture run](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42407779122). Actual final combined approved palette requires a subsequent exact-head capture and CI pass before merge.

### Remaining integration rules

- Any source changes require updating only exact reversible source hunks in `tests/specifications/tim003_preview_delta.json`; do not loosen frozen historic checks.
- Do not merge without exact-head green checks and verifying that the production build has no A/B palette selector.
- Do not claim 32/32 TIM-003 tracker acceptance until the Master Control acceptance checklist is reconciled.
