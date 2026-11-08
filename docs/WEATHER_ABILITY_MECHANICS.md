# WTH: Ice Body, Overcoat and Snow Warning mechanics

Authority: issue47 (LY-LW-055/056 and SOW-007), dedicated weather assignment under issue105. Branch `feature/weather-ability-mechanics` starts at main `3fbc7b941dcbf88676d3fce645b9151669e2f8fa`, after Earth Power PR110. This is independent of special-route lifecycle PR113; neither PR is merged by this task.

## Implemented engine behavior

- Append Ice Body78, Overcoat79 and Snow Warning80; preserve original ability IDs0–77. All have names and descriptions within the existing UI capacity.
- Ice Body heals1/16 maximum HP (minimum1, standard healing cap) at end of turn in effective Hail. It prevents Hail residual damage even on a non-Ice test battler. It neither heals in Rain/Sun/Sand/clear nor protects against Sandstorm. Fainted/full-health battlers do not receive healing. Cloud Nine/Air Lock suppress weather effects.
- Overcoat blocks Sandstorm and Hail residual damage, irrespective of type. No unrelated powder, status or damage immunity is introduced.
- Snow Warning starts indefinite, replaceable Hail on switch-in, using the existing GenIII Hail text, animation and weather-form script. No new artwork or later-generation Snow defense bonus. Repeating entry under already permanent Hail does not restart it. A weather move can replace it; that move's expiry does not restore Hail.
- Ice Body reuses the existing Rain Dish healing script and ability-name buffer. The healing ability identity is saved before nested weather-suppression checks, which can overwrite `gLastUsedAbility`, then restored for the message/ability recording. Both player and opponent are tested.

No global species slots, Trainer parties, individual Pokémon storage, save offsets, link record sizes, move IDs, distributions, encounters, lighting or field-weather tables change. This engine slice does **not** make any particular species or special individual acquire these abilities.

## Executable validation

```sh
python3 tests/run_weather_ability_tests.py
LIGHTNING_WEATHER_MOVES_ONLY=1 LIGHTNING_WEATHER_ABILITIES_ONLY=1 python3 tests/emulator/run.py
LIGHTNING_WEATHER_MOVES_ONLY=1 python3 tests/emulator/run.py
```

The host runner checks exact approved delta reversals, complete81-ID/name/description coverage, preserved78 original IDs and frozen species/save-source bytes. These are **static checks**, not executed battle mechanics. All historical manifests remain unchanged; a strict newest-source reverse adapter protects older audits.

Native mGBA tests execute the compiled ROM's residual opcode over all18 monotypes with ordinary/Sand Veil/Ice Body/Overcoat fixtures; actual healing dispatcher with both battler sides, all weather categories, healthy/full/fainted/low-HP cases and both weather suppressors; actual Snow Warning callback/message/animation; and full Splash turns measuring party and battle HP. Real turns also cover six-turn permanent Hail, Sunny Day replacement and expiry without Hail restoration. Test abilities are explicitly injected fixtures, not installed species assignments or captured-individual persistence.

Executed locally: all42 host runners passed; final focused native ability run passed967 assertions. The broader weather-move run and clean rebuild evidence are recorded in the PR.

CI runs the new host guard and the native tests through the retained full weather-move group. Independent upstream checksum and clean modified reproducibility remain required. No ROM artifacts are published.

## Remaining integration work and decisions

1. Individual ability persistence for special Corphish/Glalie remains unimplemented. Do not substitute species-wide slots. Validate catch, party→PC→party, save/load, evolution and ownership/link effects when implemented.
2. Approved global eligibility is not an approved two-slot replacement/inheritance table. That design reconciliation is still required before changing species arrays.
3. Current main's Brock party is still Geodude12/Onix14, not the desired Snorunt14/Onix16. No movesets were invented or parties changed here. Integrate finalized Gym party data separately, then test individual Ice Body/Overcoat on the actual team. Misty's Starmie-specific Drizzle also needs source integration and real switch-in acceptance.
4. Special-route producer placements, levels/moves and finalized Corphish aftermath are still absent. PR113 provides opt-in lifecycle mechanics only; do not enable an unresolvable override.
5. Complete story progression, physical RTC, real hardware and link-peer testing remain external acceptance. The engine fixtures are not evidence those scenarios passed.

Rollback: revert this feature commit. It allocates no save storage and has no production species assignments. If later consumers assign these IDs, revert those consumers first; never renumber already stored IDs casually.
