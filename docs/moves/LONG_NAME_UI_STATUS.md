# Long move-name UI — BTL-005-M1B

[PR41](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/41).
Production source715e1dff255a6aacaa22498ca5fb62063a26d3dd. Separate implementation on `feature/move-engine-long-name-ui`, stacked on
PR40 (`feature/move-engine-foundations`, parent ee3a0e3c). Main and other PRs
are unchanged. This completes the two layout gaps identified in M1a, not the
410-move engine redesign. No new playable move IDs or placeholder effects.

## Implementation

The battle move pane uses one168×32px canvas, two84px columns and an8px cursor
indent. All410 approved names fit in the remaining76px using FireRed's existing
small font (widest74px). The frame is rearranged in RAM using existing tiles;
original graphics, palettes and animation assets are untouched. The three unused
name-window IDs retain small offscreen allocations for existing callers.

The40px side pane retains PP and category. Types use unique three-character tags:
NRM/FGT/FLY/PSN/GND/RCK/BUG/GHO/STL/???/FIR/WTR/GRS/ELE/PSY/ICE/DRG/DRK,
followed by /PHYS, /SPEC or /STAT. Both values come from the same authoritative
move data used by battles. Full type names remain unchanged elsewhere. Move
reordering retains the original cursor artwork and behavior. Every active BG0
window owns a disjoint tile range, below the next character bank. The total
BG0 window pixel-buffer allocation remains344 tiles/11008 bytes, equal to M1a;
the larger name canvas is offset by smaller side/unused windows.

The relearner list grows80→88px, uses the existing small font and preserves all
seven visible rows. Numeric stats use a16px pane and small font so power250 and
accuracy100 fit. Window tile allocations are shifted to avoid overlap; the M1a
four-byte BG alignment remains. No move data, PP, balance, save fields, wire
records, artwork or gameplay distribution changes.

Production files: `include/battle_bg.h`, `include/move_categories.h`,
`src/battle_bg.c`, `src/battle_controller_player.c`, `src/battle_message.c`,
`src/move_categories.c`, `src/learn_move.c`.

## Executed acceptance and reproducible commands

- `python3 tests/run_move_ui_tests.py`: PASS static widths for410 approved names,
  all54 type/category combinations, PP/three-digit stats, disjoint battle tile
  allocations and frozen original move/save/art assets. Not runtime proof.
- All18 workflow host suites and3 static checkers: PASS locally.
- `LIGHTNING_LONG_MOVE_UI_ONLY=1 python3 tests/emulator/run.py`: mGBA0.10.5
  **PASS797** guarded assertions. Actual natural Single/Double encounter entry,
  Fight, four-name rendering, D-pad selection, SELECT reorder preserving actual
  move IDs/PP, Run/field return, flash save/load, partner menu, relearner entry,
  scrolling/cancel and unchanged Pokémon moves. All54 type/category combinations
  pass through the actual renderer using isolated ROM-copy data fixtures.
- Screenshot fixture names SKITTER SLASH, PETAL BLIZZARD, DUAL WINGBEAT and
  SCORCHING SANDS were inspected in battle and the relearner; PP/category,
  power120 and accuracy100 are legible. Test substitutions restore original ROM
  bytes; they do not enable new moves. Optional captures:
  `LIGHTNING_LONG_MOVE_CAPTURE_PREFIX=/tmp/long-move` with the focused command.
- Full21-group native suite: **PASS513858 guarded assertions**, all parent
  groups retained. `make clean` then `make -j2`: PASS, identical SHA256
  d6b1a818de5441d399bba63152b3aede8617c9e3a14f6a8314aadbb5176206a0.
- GitHub Actions remote conclusions are recorded in PR41 Checks/review description,
  separately from these executed local results. No remote pass is inferred.

The initial focused test used inputs faster than list scrolling accepts; paced
inputs now assert each exact next index rather than relaxing the expected result.
Screenshot review also exposed overlapping PP/type tile allocations; corrected
ranges and a new disjoint-allocation audit guard against recurrence.

## Limits and rollback

Real link peers, physical hardware, every battle-menu variant and a full
playthrough remain unverified. Names using the full approved character set are
statically measured; only named fixtures are rendered natively. The56 new moves,
effect/animation execution, AI and TM metadata remain subsequent milestones.
No ROM/BIOS/save/screenshot binaries are committed or uploaded. Drive sync pending.

Revert this child PR through review to restore M1a layouts; preserve PR40 and all
parent features. No save migration is needed: no IDs or saved fields changed.
