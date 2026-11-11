# WRL-010 — Failure-atomic follower graphics loader engineering contract

**Status:** Dedicated graphics-only production-source module implemented and native mGBA accepted on a disposable current-main checkout. Not merged into main or connected to follower movement/party/save systems. See executed run 42542468450.

## Evidence and source constraints

- The exact approved roster and sprite format checks remain in `docs/followers/252-species-coverage.csv`. Standard species: six 32×32 cells, 16 4bpp tiles per active frame. Steelix: six 64×64 cells, 64 4bpp tiles per active frame.
- [Native mGBA run 42539269278](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42539269278) passed **1,666 native assertions** against real current-main ROM code: 32→64→32 acquire-first switching, forced tile-memory allocation failure while preserving the 32×32 predecessor, successful retry after one 64×64 blocker was freed, and cleanup. Test-only implementation: `tests/followers/inject_native_graphics_probe.py`.
- Previously passed run 42533894429 rendered all six authentic Steelix source frames and Shiny colors through the emulated GBA GPU; no Steelix sprites or ROM data were committed.
- **Never use vanilla `ObjectEventSetGraphicsId` as a 32→64 follower loader.** It frees old tile ownership and mutates graphics fields before allocating the replacement, then retries using the *old* tile count if the new allocation fails. Its result is not failure-atomic.
- Production `main` continues advancing independently. The audit branch is historical and must not be casually merged into `main`; only reviewed, intentionally ported code should be integrated.

## Required production transaction

1. **Resolve without side effects:** determine selected Pokémon *instance*, species/form/Shiny state and required frame size. Refuse an unsupported or out-of-roster value. Do not change party selection or saved state here.
2. **Plan graphics resource ownership:** target asset descriptor, active image bytes (512 or 2048), six frame addresses, normal/Shiny 16-color palette and 32/64 OAM/subsprite table. Check source/palette hash and range metadata. Separate graphics identity from party-slot index.
3. **Acquire before release:** obtain a fresh object/sprite slot and a contiguous **16- or 64-tile** 4bpp allocation. Preserve the existing visible sprite and its resources during staging. Check palette reservation; avoid overwriting another NPC's palette bank, especially NPC-special/reflection palettes.
4. **Stage and validate:** load the exact chosen 4bpp frame and palette into the new reserved resources; verify all source indices <16, dimensions, tile count, sprite geometry, and assigned resource ranges. Complete reflections/subsprite support before exposing the new OAM object.
5. **Commit atomically at a graphics-safe boundary:** enable the staged replacement with appropriate position/elevation, palette, layer, direction and frame; only then hide/destroy the predecessor and release its exclusively owned tiles/palette. If commit cannot finish, clean up the staged resources and retain the prior sprite unchanged.
6. **Failure is not reassignment:** out-of-tiles, out-of-sprites, palette conflict, forbidden map or other transient graphics conditions must **not** change which party Pokémon is selected. The follower may be temporarily hidden only under a specified fail-safe state, with a retry upon a safe later event.
7. **Dispose reliably:** on map transition, battle/cutscene mode or replacement, release only resources owned by the graphics loader. Never invoke global `AllocSpriteTiles(0)` in production; this is used solely by the isolated native test to reset its test arena.
8. **Recreate on map load:** old sprite IDs and tile pointers are stale after field map reloads; ensure a fresh graphics allocation and event-slot check on the destination map. Do not persist raw sprite IDs, tile numbers or palette bank indices in save data.

## Required tests before production acceptance

| Area | Required observation |
|---|---|
| Baseline | Existing 252/252 PNG, indexed pixel, normal/Shiny and 4bpp validations remain green |
| Geometry | 32×32/64×64 correct OAM shape, 16/64 tile allocation and distinct frame paths |
| True renderer | Real Steelix six-frame GPU hash/visible-pixel and Shiny palette test passes |
| Resource failure | Force contiguous VRAM exhaustion; old object remains valid, tile bitmap uncorrupted, no leaked new object/palette |
| Recovery | Release competing resources; retry succeeds; old allocation freed exactly once |
| Map transitions | Real map callbacks with existing NPC graphics, A→B and B→A; no stale handles or resource leaks |
| Crowding | 16-object event budget, 64-sprite OAM budget and non-follower sprite usage validated independently; fail by hiding, never by stealing another sprite's reservation |
| World-state suppression | doors, elevation, reflection, surfing/bicycle/cutscenes/weather, heal/faint and box behavior tested after the separately authorized gameplay layer exists |
| Reproducibility | Pin tested source/main SHA and GitHub Actions run; no ROM, external artwork or saves committed/uploaded |

**Important boundary:** A `CreateVirtualObject` test that swaps built-in Snorlax and Seagallop proves the ROM's allocation and object machinery, **not** automatically a complete 252-species dynamic gfx resolver or the real follower object-event lifecycle. Pass those separately.

## Next implementation checkpoint

Build the graphics-only production candidate with a single dynamic follower object-event identity, explicit size-aware tile/palette resource ownership, staged sprite creation and failure rollback. Test this module in a controlled current-main checkout against real 32×32 and 64×64 source assets and native field transitions **before** connecting it to party menus, save data or movement. Preserve this PR as audit provenance, not an unreviewed automatic merge.

**Phase-One overall gate:** Remains BLOCKED until the complete approved dynamic graphics implementation and applicable map-state acceptance checks have executed, even though the isolated CPU/VRAM/OAM/GPU subgates passed.


## 2026-11-09 implemented module and accepted native tests

**Implementation:** `include/follower_graphics.h` and `src/follower_graphics.c` on the existing WRL-010 audit branch. The current-main test CI copies these C/header sources into a disposable checkout, compiles and links a real FireRed ROM, and invokes their exported functions on ARM under libmGBA. The actual pinned Bulbasaur six **32×32** source frames (Git blob SHA `3eb99143e0f2b7e1df044b19374419c4a977b874`) and genuine pinned Steelix six **64×64** source frames (Git blob SHA `1a0ff067983d294ce1534674efa91c697c4db794`) plus their normal palettes, and Steelix Shiny palette for the GPU test, are fetched **only temporarily in CI**; no graphics or ROM bytes were committed.

**Acceptance:** [GitHub Actions run 42542468450](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42542468450) completed **SUCCESS**, executing **8,195 native assertions** against temporary FireRed `main@37230a9c513ed013fc6b402ec800bd1d13960492`. The native run proves:
- The production module's **acquire-before-release** 32×32 Bulbasaur → 64×64 Steelix → 32×32 Bulbasaur graphics swaps, with real image transfer of six 64×64 source cells and correct tile cleanup;
- Ownership of each candidate's separate sprite ID, contiguous 16- or 64-tile range, temporary tagged normal palette reservation, allocation-failure rollback preserving the previous sprite and palette, and retry after tile pressure is relieved;
- Exactly one **graphics-only FireRed `ObjectEvent` slot** reserved from the 16-slot pool using internal local ID 126, updated to the new sprite ID/OAM geometry only after successful acquisition, and released before resources are destroyed;
- Refusal when **all 16 `ObjectEvent` slots are active**. The implementation drops unbound staged graphics (rather than leaving a ghost/stealing an NPC's slot). This is not a loss of any future selected Pokémon identity, which is not managed in this module;
- Live 30-frame world-update periods with the graphics-owned event on each real **Route 5 Day Care** and **Four Island Day Care** map, using normal native field callbacks, plus explicit event teardown before a real map warp and safe recreation after map reload;
- Separate real mGBA GPU rendering of all six pinned Steelix 64×64 frames and Shiny palette, plus hardware OBJ/OAM native allocation, exhaustion and recovery.

**Public API:** `FollowerGraphics_Swap`, `FollowerGraphics_SetFrame`, `FollowerGraphics_BindObjectEvent`, `FollowerGraphics_Release`, `FollowerGraphics_DetachForMapTransition`, `FollowerGraphics_InvalidateAfterSpriteReset`, and introspection getters. A caller must present already-converted six 4bpp frames and one 16-color normal or Shiny BGR555 palette. The 32/64-size native source OAM descriptor is reused from FireRed, no static sprite tile indices or NPC palette banks are stolen. The graphics copy must be invoked at a graphics-safe VBlank phase: this source does not add an automatic main-loop/VBlank hook.

**Limitations preventing gameplay acceptance:** The owned `ObjectEvent` is intentionally graphics-only and has no player-following movement, elevation/priority dynamic synchronization, reflection behavior, invisible/cutscene mode policy, party-instance identity, permanent saved state, or 252-species runtime resolver. No production map lifecycle hook currently calls `DetachForMapTransition`/ `InvalidateAfterSpriteReset`; the focused test calls these explicitly. It currently proves the **tested Route 5/Four Island callback transitions**, not all maps or all graphics crowding patterns. The branch is hundreds of commits behind main and cannot be blindly merged. The current graphics-loader milestone has passed its scoped native checks; the overall WRL-010 follower feature and broad asset/integration gate remain **BLOCKED** until the remaining canonical requirements pass.

**Reproduction:** `tests/followers/prepare_steelix_native_frame.py`, `tests/followers/inject_native_graphics_probe.py`, and `.github/workflows/follower-native-graphics.yml` compile/test the real module and assets via disposable current-main checkout. No ROM, BIOS, save, copyrighted PNG or compiled payload is uploaded or committed.
