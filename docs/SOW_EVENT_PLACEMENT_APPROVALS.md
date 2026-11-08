# SOW-004 / SOW-007 / SOW-008 — owner-approved encounter content and proposed object tiles

2026-11-08 direct owner approval recorded in [weather issue #47](https://github.com/InternetNinjacy/pokemon-lightning-yellow/issues/47#issuecomment-6722103214). This document is a reviewable **source/placement candidate handoff**, not evidence that the event objects, their graphics, or actual battle scripts have been installed. The three object positions below are **selected native-tested coordinates**, to be accepted only if the compiled native no-Surf access/collision checks pass, followed by in-game reachability and sprite/guard review.

| SOW | Map and coordinates (0-based map-local x,y) | Level | Four moves in battle | Item | Unique ability |
| --- | --- | ---: | --- | --- | --- |
| SOW-004 | `MAP_ROUTE12` **(19,87)** — eastern land-side approach to fishing house dock (house warp at 12,86) | 30 | Bubble Beam, Knock Off, Protect, Vice Grip | None | Drizzle, individual marker retained when caught and later evolved to Crawdaunt |
| SOW-007 | `MAP_ROUTE21_NORTH` **(13,7)** — northern grass strip on the Pallet-side mainland | 40 | Ice Beam, Crunch, Headbutt, Protect | None | Snow Warning, individual marker retained if caught |
| SOW-008 | `MAP_ROUTE23` **(13,145)** — Southern overland portion between Cascade checkpoint y149 and Thunder checkpoint y140 | 55 | Crunch, Rock Slide, Thrash, Scary Face | None | native Sand Stream |

All three require **zero Surf use to reach from the appropriate adjacent land entrance**. This is the latest explicit owner instruction and supersedes the older requirement for Glalie to stand in a Surf-accessible area. Tyranitar's site is intentionally south of Route23's later Surf crossing, between the first two badge guards; preserve all checkpoint flags, scripts, patrols and pass-through paths. Corphish must remain associated with the dock/fishing house, not Route12 Snorlax, and its graphic should not block the fishing-house entrance or ordinary pathing.

## Grounding and acceptance

- Current main Route12 layout `data/layouts/Route12/map.bin` has size 24×120, house warp (12,86), and land-side walking approach in row87 near x19. No current map object occupies (19,87).
- Current main Route21 North layout `data/layouts/Route21_North/map.bin` has size 24×50; tile (13,7) is within the northern grass block, which connects toward Pallet Town at y0. It is not on the water channel. No current map object occupies (13,7).
- Current main Route23 layout `data/layouts/Route23/map.bin` has size 24×160; badge guards at y149,140,123,112,94,61,35, with adjacent event triggers. The candidate (13,145) lies between the first two guard rows, below the major Surf-required part of Route23. No current map object occupies (13,145).
- `tests/emulator/sow_placement.c` checks **actual loaded ROM metatile collision and Surfable behavior plus a four-neighbor land-only path** to each object tile, from Route12 (12,87), Route21 North (13,1), and Route23 (9,151). These tests are not substitutes for movement/guard/scene and screenshot verification. A failed check means the candidate must be changed, **not** that the test should be weakened.
- Dedicated graphics are not yet allocated: `OBJ_EVENT_GFX_CORPHISH`, `OBJ_EVENT_GFX_GLALIE`, `OBJ_EVENT_GFX_TYRANITAR` do not exist on main. Reuse only project-authorized existing art and validate correct object-event OAM/palette/animation; do not show another Pokémon as placeholder.

## Lifecycle when the actual producers are installed

Use already merged PR #128 opt-in event flags and `SpecialWeatherActivateScript`/after-aftermath resolver, not a new weather engine. Force Rain on all Route12, Snow on both Route21 technical maps, and Sandstorm on Route23 while unresolved. **Capture OR defeat**, then any approved aftermath, then exactly one new current-map baseline weather roll before control. Flee/loss preserve unresolved event and native retry behavior. The haunted Corphish's existing ghost farewell is verbatim **“At last... I can rest in peace...”** after either successful outcome, before restoring weather. Preserve fishing brother's original one-time Super Rod award and original post-gift Magikarp measurements.

Each event requires actual held-item-none, exact moveset, unique ability capture/storage, object hiding, save/continue, native overworld access and RTC/weather transitions verified in a reviewable separate gameplay PR. Do not award final WTH-001 completion merely from approved content, proposed coordinates, or opt-in weather lifecycle fixture tests.

This handoff is confined to WTH-001 special overworld producers. It does not modify ordinary wild ecology WRL-002/009, Codex-owned BTL-005 move source, or owner-approved TIM-003 lighting.

2026-11-08 native correction: Route23 (12,145) was blocked by the compiled ROM's map collision; the real-ROM BFS selected **(13,145)**, with 3 walkable neighbors and a 10-step walk from (9,151) without Surf. All three stationary object templates are installed in the stacked PR #138 and remain pending script, graphic, persistence, and exact-head integration acceptance.
