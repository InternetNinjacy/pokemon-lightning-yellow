# Approved trainer class, format and overworld conversion engineering handoff

Project: Pokémon Lightning Yellow. Source audit: November 6, 2026. **OWNER APPROVED DESIGN; GAMEPLAY IMPLEMENTATION NOT AUTHORIZED BY THIS DOCUMENT.**

## Source-backed census

All 510 source Trainer IDs are present in current `src/data/trainers.h`. Comparing their source `.trainerClass` and `.doubleBattle` against the delegated census yields **exactly four approved class-conversion differences and one approved battle-format difference**. These are expected changes rather than errors in the design register:

| Source Trainer ID | Current source class | Approved target class | Current battle | Approved battle |
| --- | --- | --- | --- | --- |
| `TRAINER_CAMPER_SHANE` | `TRAINER_CLASS_CAMPER` | Bug Catcher | Single | **Double**, four ordered Pokémon |
| `TRAINER_YOUNGSTER_NASH` | `TRAINER_CLASS_YOUNGSTER` | Ranger (male) | Single | Single, two Pokémon |
| `TRAINER_LASS_DALIA` | `TRAINER_CLASS_LASS` | Ranger (female) | Single | Single, two Pokémon; display **Dahlia** |
| `TRAINER_LASS_JOANA` | `TRAINER_CLASS_LASS` | Ranger (female) | Single | Single, two Pokémon; display **Joanna** |

## Original party source-size exceptions

Eight **owner-approved original encounter** size changes relative to current source must remain intact. These are distinct from ten unapproved/draft rematch size deviations.

| Source ID | Source slots | Approved slots | Approved species in order |
| --- | --- | --- | --- |
| CAMPER_SHANE | 2 | 4 | Oddish / Spinarak / Paras / Venonat |
| PKMN_RANGER_JACKSON | 3 | 2 | Golduck / Noctowl |
| PKMN_RANGER_BETH | 3 | 2 | Persian / Golduck |
| PKMN_RANGER_KATELYN | 1 | 2 | Persian / Manectric |
| PKMN_BREEDER_BETHANY | 1 | 2 | Smoochum / Jynx |
| YOUNGSTER_NASH | 3 | 2 | Houndoom / Noctowl |
| LASS_DALIA | 4 | 2 | Golduck / Manectric |
| LASS_JOANA | 1 | 2 | Arcanine / Fearow |

Do not interpret a later `TRAINER_PKMN_RANGER_JACKSON_2` or `...KATELYN_2` rematch source size as overriding the locked two-member Ranger class policy. Modified-size encounter levels need their own approved slot mapping rather than mechanically copying an incompatible original array.

## Shane: verified source changes and map effects

- `data/scripts/trainers.inc` currently exposes `Route24_EventScript_Shane` with `trainerbattle_single TRAINER_CAMPER_SHANE`; its rematch path currently uses `trainerbattle_rematch`, also Single.
- `data/maps/Route24/map.json` has the mapped original NPC `Route24_EventScript_Shane`, overworld `OBJ_EVENT_GFX_CAMPER` at (5,21). Keep map coordinates, sight event, persistent defeat flags and Trainer source ID unchanged. **Approved Bug Catcher conversion requires compatible Bug Catcher overworld artwork**, not a new NPC.
- `src/data/trainers.h` retains Camper class, Camper front picture and `.doubleBattle = FALSE` until implementation. Change class/front picture and double flag together with the party and battle script.
- Change both original and rematch interaction behavior consistently for a legal Double Battle. Confirm two-usable-Pokémon gate, script result, battle entry, rematch availability, and original map event continuity. Do not let a format change make the NPC unbattleable or skip rewards.
- Preserve four owner-approved species/order; stage/timing/moves/items/AI require technical review. Do not infer a brand-new fifth species.

## Pattern Bush Ranger conversions: verified source and visuals

All three map scripts are local to `data/maps/SixIsland_PatternBush/scripts.inc`, with existing `trainerbattle_single` (keep Single). Their positions and current overworld sprites in `map.json` are:
- Nash: (32,5), `OBJ_EVENT_GFX_YOUNGSTER`, script `SixIsland_PatternBush_EventScript_Nash`
- Dalia: (8,21), `OBJ_EVENT_GFX_LASS`, script `SixIsland_PatternBush_EventScript_Dalia`
- Joana: (39,6), `OBJ_EVENT_GFX_LASS`, script `SixIsland_PatternBush_EventScript_Joana`

Change the trainer classes to `TRAINER_CLASS_PKMN_RANGER`, assign the existing male/female Ranger **battle trainer pictures** where compatible, update displayed names Dahlia and Joanna, set approved two-member parties and preserve each source ID, flags, coordinates, dialogue access and battle type. Check text for class/name grammar and replace only owner-approved or necessary inconsistent references.

**Overworld artwork is a separate engineering dependency**: current `include/constants/event_objects.h` defines Youngster, Lass, Camper and Bug Catcher graphics but does not define a `OBJ_EVENT_GFX_POKEMON_RANGER` graphic. Source Ranger battle front pictures do not automatically supply overworld map sprites. First search the existing project's object-event assets and compatible open-source FireRed/Gen III Ranger sprites. If none can be reused appropriately, new male/female Ranger overworld art, constants, palettes, sprite table integration and visual tests require explicit implementation work. Avoid claiming the art already exists; do not silently leave Youngster/Lass if the accepted design needs Ranger representation.

## Technical acceptance gates

1. Reconfirm the eight original party-size exceptions and the separately configured rematches do not exceed the engine's six-party-member limit; preserve ID ordering and Single/Double structure.
2. Build success, battle data validation, event-script compilation and recorded trainer class/portrait lookups.
3. Use an emulator to enter each of the four original encounters, see appropriate overworld/battle sprites, inspect name and team order, finish the fight, verify flags/rewards and avoid duplicate battles.
4. For Shane, specifically test with one and with two usable party Pokémon, trainer approach/rematch triggering and post-battle access. For Ranger conversions verify both genders' artwork and dialogue.
5. Keep original event access, flags and map coordinates unchanged. These are design conditions, not completed tests.

This is a source-backed handoff only. No gameplay code was altered or merged by preparing it.
