# Lightning Yellow title screen: source audit and partial implementation

Status: **ART ASSETS AND SOURCE REFERENCES IMPLEMENTED; build/emulator verification outstanding.** Topic branch only. Audited against `main` at `18d245b5e35780b1300482aa536fa15f140a1d28` on 2026-11-07.

## Approved visual outcome

- The displayed game title must read **Pokémon Lightning Yellow** (not FireRed).
- The prominently featured title Pokémon must be **Pikachu** (not Charizard).
- Preserve the existing title screen's startup, prompt, input, animations, music, save/load transition, and copyright attributions unless changes prove necessary.
- Do not change Professor Oak's intro, opening story, overworld Start menu, gameplay, or ROM/header identity as part of this artwork task.

## Exact source locations

- `src/title_screen.c`: `CB2_InitTitleScreen` initializes BG0–BG3; title scene transitions, window/palette effects, prompt flashing, flames, `MUS_TITLE`, A/Start handling, and `TITLE_SPECIES` cry. The FireRed build originally uses `SPECIES_CHARIZARD`; this branch changes only its title cry to `SPECIES_PIKACHU`.
- `src/graphics.c`: FireRed/LeafGreen conditional `gGraphics_TitleScreen_*` asset declarations and `INCBIN` paths. The FireRed title currently references `graphics/title_screen/firered/`.
- `graphics/title_screen/firered/game_title_logo.png`: original logo pixels; `game_title_logo.pal`: JASC-PAL 256 colors; `game_title_logo.bin`: 1280-byte BG tilemap. The title loader uses compressed **8bpp** logo tiles on **BG0** and an 8bpp palette.
- `graphics/title_screen/firered/box_art_mon.png`: original Charizard artwork; `box_art_mon.pal`: JASC-PAL 16 colors; `box_art_mon.bin`: 1280-byte tilemap. The title loader uses compressed **4bpp** mascot tiles and palette bank **13** on **BG1**. BG1 is animated through palette blending and the slash/reveal system: this artwork is *not* a standard OBJ Pokémon sprite.
- `graphics/title_screen/firered/background.pal`: original background palette used on BG2/BG3.
- `graphics/title_screen/border_bg.png`, `graphics/title_screen/firered/border_bg.bin`: border background tiles/map used on BG3.
- `graphics/title_screen/copyright_press_start.png`, `copyright_press_start.bin`: combined copyright and press-start graphic/map on BG2. Press Start blinks by palette-index manipulation in `Task_TitleScreen_BlinkPressStart`. Do not erase or move copyright attributions.
- `graphics/title_screen/firered/flames.png`, `blank_flames.png`, `slash.pal`, `graphics/title_screen/slash.png`, `blank_sprite.png`: sprite/effect elements. FireRed spawns flame sprites; a yellow/electric effect treatment is optional polish, not required for first functional replacement.
- `src/main_menu.c`: save/new-game menu transition following the title cry. `src/start_menu.c` is the separate **in-game** Start menu, not the boot title screen.
- Existing ordinary sprite: `graphics/pokemon/pikachu/front.png` with its normal/shiny palettes. This is a Pokémon battle sprite, **not** a confirmed title-layout-compatible replacement and must not be substituted by filename alone.

### Graphics and asset pipeline

`Makefile` uses `graphics_file_rules.mk` and `tools/gbagfx/gbagfx`: `.png` to `.4bpp`/`.8bpp`, `.pal` to `.gbapal`, and source `INCBIN` dependencies to compressed `.lz` resources. The title's `.bin` maps are source inputs, not generated automatically from a new logo caption. Both current title graphics maps occupy 1280 bytes (32 × 20 16-bit tilemap cells). The GBA visible viewport is 240 × 160, so verify actual art placement within the map's 256-pixel tile-row width.

The title logo, mascot, background, prompt/copyright, and moving sprite effects are separately supplied resources. The existing animation timing and key logic can remain untouched if new assets honor the existing tilemap sizes, palettes, BG assignments and tile limits.

## Changes made in this PR

1. FireRed-specific `TITLE_SPECIES` set to `SPECIES_PIKACHU` so the title transition uses Pikachu's cry. The LeafGreen conditional stays unchanged.

**Subsequent implementation on this branch:** added `graphics/title_screen/lightning_yellow/game_title_logo.png` (256×64, indexed 8bpp) and corresponding 256-color `.pal`. The two-line lettering reads **Pokémon** / **LIGHTNING YELLOW**. Added `box_art_mon.png` (96×96, indexed 4bpp) and corresponding 16-color `.pal`, using the repository's existing Pikachu front-sprite art scaled with nearest-neighbor sampling. Switched the FireRed `src/graphics.c` asset references to these new source PNG/palette files. The original FireRed `.bin` tilemaps are reused and were statically checked for tile-index and palette-bank compatibility. Upstream assets remain unmodified. Added `tests/run_title_screen_tests.py` to guard asset dimensions, palette consistency, source references and tilemap bounds.

**Still not verified:** an actual built ROM or emulator rendering. The existing FireRed flame/border style remains; that is optional visual polish, not a reason to claim full visual verification.

## Remaining bounded work

1. Build the ROM in an authorized toolchain with `scripts/prepare-baseline.sh` and `make -j2`.
2. Run `python3 tests/run_title_screen_tests.py` and the applicable existing host tests.
3. Boot in mGBA and inspect the **actual** screen across the reveal/fade, verify Pokémon Lightning Yellow text and Pikachu art are fully visible and the Press Start prompt/copyright stay readable.
4. Verify A/Start, Pikachu cry, new/continue menu, idle restart and relevant regression checks; fix any palette, tilemap layout or runtime issues observed.
5. Review optional visual theme (FireRed flames remain intentionally unchanged until basic title acceptance succeeds).

## Validation state as of this commit

- **Static source:** confirmed loaders, file paths, bpp, palettes, tilemap resource sizes, and separate title/start-menu systems.
- **Source edit:** Pikachu cry and four replacement asset references committed. Two new indexed PNGs and matching palette text files committed; original compatible tilemaps retained.\n- **Static asset checking:** local PNG dimensions/palette index range verified; added a reproducible repository static test, but the repository test runner has not been executed against a complete checkout.
- **Build:** GitHub Actions modified-game build and clean-repeatability stage **PASSED** in workflow run 42349658763; compilation ran in CI rather than locally. Local container cannot resolve GitHub for checkout.
- **Emulator:** native mGBA regression step in workflow run 42349658763 **in progress at last check**; title-specific visual inspection remains unverified.
- **Visual verification:** **offline mockup only**; in-ROM appearance not verified.
- **CI:** host-side regression and new title asset checks PASSED; modified clean reproducibility PASSED; independent upstream baseline PASSED. Current run: https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42349658763
- **External proprietary assets:** none newly imported.
- **Project tracking:** intentionally not rewritten, to avoid colliding with parallel Master Control updates.

## Acceptance gate

The visual graphics and source registration now target Lightning Yellow/Pikachu; **acceptance remains pending** until an actual ROM boots with correct assembled title art and passes interactive checks. Never mistake offline PNG review for ROM validation.
