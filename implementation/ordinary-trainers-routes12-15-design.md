# Delegated Kanto Route 12-15 ordinary Trainer design

**Status:** Species teams curated under 2026-11-06 owner delegation. **Not code-ready; level/move/item/AI balance and map script integration remain to validate.**

**Coverage:** 40 distinct original Trainer source IDs across 43 original trainer NPC map objects: Route 12 = 8 records/9 NPCs, Route 13 = 10/10, Route 14 = 11/12, Route 15 = 11/12. The differences arise from three shared two-NPC Double Battle encounters. Five rematch-style variants of these base encounters may be present and must be reviewed separately; see companion rematch table. Existing owner-approved Trainer designs are not changed.

**Authority:** source map objects `data/maps/Route12/map.json` through `Route15/map.json`, `src/data/trainers.h`, `src/data/trainer_parties.h`; source party original level references and the locked 252-species roster; owner-approved 41 class eligibility pools in the [Trainer Master Register](https://docs.google.com/document/d/14FX3eDLXPCYbyxun-3oREFftSdaM25O0NhJaCXP7ZGY/edit). Design delegated by user November 6, 2026. The source map's named event scripts may not resolve in the checked per-map scripts.inc; coding stage MUST inspect how scripts are actually generated or linked and treat missing scripts as a blocker.

**Selection and balance:** Most source party counts preserved, no extra Pokémon, class-eligible lines only. At levels roughly 21-33, predominantly unevolved or middle-stage species and moderate single-stage Pokémon are preferred. Late evolved forms in source at inappropriate levels are scaled back. Designers retain intentional distinct roles: Fishermen have plausible catches, Bird Keepers only birds, Bikers industrial/road species, Picnickers social companions, and relationship pairs use exact class-approved companion groups. Source level values listed below are reference placeholders, not final approved levels. Where a class has approved high-BST singleton Pokémon, moveset/power must be specifically checked at the intended level.

### Route 12

| Source Trainer ID | Delegated species lineup | Source provisional levels | Battle format |
|---|---|---|---|
| `TRAINER_FISHERMAN_NED` | Goldeen / Krabby / Magikarp | 22 / 22 / 22 | Single |
| `TRAINER_FISHERMAN_CHIP` | Tentacool / Goldeen | 24 / 24 | Single |
| `TRAINER_FISHERMAN_HANK` | Goldeen | 27 | Single |
| `TRAINER_FISHERMAN_ELLIOT` | Shellder / Goldeen / Horsea / Spheal | 21 / 21 / 21 / 21 | Single |
| `TRAINER_ROCKER_LUCA` | Voltorb / Magby | 29 / 29 | Single |
| `TRAINER_CAMPER_JUSTIN` | Hoothoot / Teddiursa | 29 / 29 | Single |
| `TRAINER_FISHERMAN_ANDREW` | Magikarp / Magikarp | 24 / 24 | Single |
| `TRAINER_YOUNG_COUPLE_GIA_JES` | Nidorino / Nidorina | 24 / 24 | Double |

### Route 13

| Source Trainer ID | Delegated species lineup | Source provisional levels | Battle format |
|---|---|---|---|
| `TRAINER_PICNICKER_ALMA` | Poliwag / Pikachu / Shroomish | 28 / 28 / 28 | Single |
| `TRAINER_BIRD_KEEPER_SEBASTIAN` | Pidgeotto / Fearow | 29 / 29 | Single |
| `TRAINER_PICNICKER_SUSIE` | Pidgey / Pikachu / Shroomish / Clefairy / Poliwhirl | 24 / 24 / 24 / 24 / 24 | Single |
| `TRAINER_BEAUTY_SHEILA` | Meowth / Kirlia | 29 / 29 | Single |
| `TRAINER_BEAUTY_LOLA` | Skitty / Meowth / Kirlia | 27 / 27 / 27 | Single |
| `TRAINER_PICNICKER_VALERIE` | Poliwag / Poliwhirl | 30 / 30 | Single |
| `TRAINER_PICNICKER_GWEN` | Pidgey / Pidgeotto / Pikachu / Tangela | 27 / 27 / 27 / 27 | Single |
| `TRAINER_BIRD_KEEPER_ROBERT` | Pidgey / Pidgeotto / Spearow / Fearow | 26 / 26 / 26 / 26 | Single |
| `TRAINER_BIRD_KEEPER_PERRY` | Spearow / Pidgey / Hoothoot / Doduo / Fearow | 25 / 25 / 25 / 25 / 25 | Single |
| `TRAINER_BIKER_JARED` | Koffing / Koffing / Grimer | 28 / 28 / 28 | Single |

### Route 14

| Source Trainer ID | Delegated species lineup | Source provisional levels | Battle format |
|---|---|---|---|
| `TRAINER_BIKER_GERALD` | Koffing / Grimer | 29 / 29 | Single |
| `TRAINER_BIRD_KEEPER_DONALD` | Farfetch'd | 33 | Single |
| `TRAINER_BIRD_KEEPER_BECK` | Pidgeotto / Fearow | 29 / 29 | Single |
| `TRAINER_BIRD_KEEPER_MARLON` | Spearow / Doduo / Fearow | 28 / 28 / 28 | Single |
| `TRAINER_BIKER_ISAAC` | Grimer / Magnemite / Koffing | 28 / 28 / 28 | Single |
| `TRAINER_BIKER_MALIK` | Koffing / Grimer | 29 / 29 | Single |
| `TRAINER_BIRD_KEEPER_MITCH` | Pidgey / Spearow / Hoothoot / Fearow | 26 / 26 / 26 / 26 | Single |
| `TRAINER_BIRD_KEEPER_CARTER` | Pidgeotto / Doduo / Farfetch'd | 28 / 28 / 28 | Single |
| `TRAINER_BIKER_LUKAS` | Koffing / Magnemite / Grimer / Voltorb | 26 / 26 / 26 / 26 | Single |
| `TRAINER_BIRD_KEEPER_BENNY` | Spearow / Fearow | 29 / 29 | Single |
| `TRAINER_TWINS_KIRI_JAN` | Nidorina / Nidorino | 29 / 29 | Double |

### Route 15

| Source Trainer ID | Delegated species lineup | Source provisional levels | Battle format |
|---|---|---|---|
| `TRAINER_PICNICKER_YAZMIN` | Tangela / Pikachu / Shroomish | 29 / 29 / 29 | Single |
| `TRAINER_BIRD_KEEPER_EDWIN` | Pidgeotto / Farfetch'd / Doduo / Hoothoot | 26 / 26 / 26 / 26 | Single |
| `TRAINER_BIRD_KEEPER_CHESTER` | Dodrio / Doduo / Doduo | 28 / 28 / 28 | Single |
| `TRAINER_PICNICKER_KINDRA` | Eevee / Vaporeon / Jolteon | 28 / 28 / 28 | Single |
| `TRAINER_BEAUTY_OLIVIA` | Feebas / Kirlia | 29 / 29 | Single |
| `TRAINER_BIKER_ALEX` | Koffing / Grimer / Magnemite | 28 / 28 / 28 | Single |
| `TRAINER_BIKER_ERNEST` | Koffing / Koffing / Voltorb / Grimer / Magnemite | 25 / 25 / 25 / 25 / 25 | Single |
| `TRAINER_PICNICKER_BECKY` | Pikachu / Raichu | 29 / 29 | Single |
| `TRAINER_BEAUTY_GRACE` | Kirlia / Persian | 29 / 29 | Single |
| `TRAINER_PICNICKER_CELIA` | Clefairy | 33 | Single |
| `TRAINER_CRUSH_KIN_RON_MYA` | Hitmonchan / Hitmonlee | 29 / 29 | Double |

## Shared Double Battles and ownership

- `TRAINER_YOUNG_COUPLE_GIA_JES`: Nidorino (Jes) / Nidorina (Gia); preserve shared event, both NPC dialogue, and requirement for two player Pokémon. Source first slot belongs to male partner Jes, second to female Gia; verify event ownership in script before code.
- `TRAINER_TWINS_KIRI_JAN`: Nidorina (Kiri) / Nidorino (Jan), two complementary Nidoran families; preserve shared original Double and both individual NPCs.
- `TRAINER_CRUSH_KIN_RON_MYA`: Hitmonchan (Ron) / Hitmonlee (Mya); preserve exact source party order, original Double and dialogue.

## Rematches and further validation

- Preserve these exact base species families for each source-linked rematch whenever class rules permit evolutionary upgrades. Rematches should gain levels, moves and a modest ace-only held item as separately checked against event progression; do not automatically add six Pokémon.
- Check final in-game move learnsets and TMs against owner-approved mechanics; physical/special split must be respected.
- Verify source original levels and any level-stage mismatches, e.g. Lv29 electrode-like outliers; choose final party levels based on adjacent routes and Gym benchmarks rather than silently copying impossible source evolution staging.
- This is 40 delegated species-team decisions, **not an assertion that all ordinary Trainer work or battle balancing is complete**. Rematch links and a further 25 historical owner approvals still require reconciliation. No gameplay code changes or merge authorization.

## Curated family-preserving rematches for Routes 12-15

The 24 numbered rematch variants tied to these routes have now been individually revised. Their exact source levels remain provisional. Stronger party members are evolutions of the base Trainer's approved species where eligible, not random class replacements. At high levels, a party may retain its species while upgrading moves, AI and ace held item. The lineups are recorded in the separate `implementation/ordinary-trainer-rematch-candidates.md`. These variant source IDs have not been event-link verified, and all final movesets require a later legal learnset check.

| Variant ID | Rematch party | Working source levels | Ace item candidate |
|---|---|---|---|
| `TRAINER_FISHERMAN_ELLIOT_2` | Shellder / Goldeen / Horsea / Spheal | 28 / 28 / 28 / 28 | `ITEM_ORAN_BERRY` |
| `TRAINER_ROCKER_LUCA_2` | Electrode / Magmar | 33 / 33 | `ITEM_ORAN_BERRY` |
| `TRAINER_BEAUTY_SHEILA_2` | Persian / Gardevoir | 49 / 49 | `ITEM_SITRUS_BERRY` |
| `TRAINER_BIRD_KEEPER_ROBERT_2` | Pidgeotto / Pidgeotto / Fearow / Fearow | 28 / 28 / 28 / 28 | `ITEM_ORAN_BERRY` |
| `TRAINER_BIRD_KEEPER_ROBERT_3` | Pidgeot / Pidgeot / Fearow / Fearow | 47 / 47 / 47 / 47 | `ITEM_SITRUS_BERRY` |
| `TRAINER_PICNICKER_SUSIE_2` | Pidgeotto / Pikachu / Breloom / Clefairy / Poliwhirl | 27 / 27 / 27 / 27 / 27 | `ITEM_ORAN_BERRY` |
| `TRAINER_PICNICKER_SUSIE_3` | Pidgeot / Raichu / Breloom / Clefable / Poliwrath | 47 / 47 / 47 / 47 / 47 | `ITEM_SITRUS_BERRY` |
| `TRAINER_PICNICKER_SUSIE_4` | Pidgeot / Raichu / Breloom / Clefable / Poliwrath | 52 / 52 / 52 / 52 / 52 | `ITEM_SITRUS_BERRY` |
| `TRAINER_BIKER_LUKAS_2` | Weezing / Magneton / Muk / Electrode | 47 / 47 / 47 / 47 | `ITEM_SITRUS_BERRY` |
| `TRAINER_BIRD_KEEPER_BENNY_2` | Fearow / Fearow | 32 / 32 | `ITEM_ORAN_BERRY` |
| `TRAINER_BIRD_KEEPER_BENNY_3` | Fearow / Fearow | 49 / 49 | `ITEM_SITRUS_BERRY` |
| `TRAINER_BIRD_KEEPER_MARLON_2` | Fearow / Doduo / Fearow | 30 / 30 / 30 | `ITEM_ORAN_BERRY` |
| `TRAINER_BIRD_KEEPER_MARLON_3` | Fearow / Dodrio / Fearow | 48 / 48 / 48 | `ITEM_SITRUS_BERRY` |
| `TRAINER_BEAUTY_GRACE_2` | Gardevoir / Persian | 49 / 49 | `ITEM_SITRUS_BERRY` |
| `TRAINER_BIRD_KEEPER_CHESTER_2` | Dodrio / Doduo / Doduo | 30 / 30 / 30 | `ITEM_ORAN_BERRY` |
| `TRAINER_BIRD_KEEPER_CHESTER_3` | Dodrio / Dodrio / Dodrio | 48 / 48 / 48 | `ITEM_SITRUS_BERRY` |
| `TRAINER_PICNICKER_BECKY_2` | Pikachu / Raichu | 32 / 32 | `ITEM_ORAN_BERRY` |
| `TRAINER_PICNICKER_BECKY_3` | Raichu / Raichu | 49 / 49 | `ITEM_SITRUS_BERRY` |
| `TRAINER_PICNICKER_BECKY_4` | Raichu / Raichu | 54 / 54 | `ITEM_SITRUS_BERRY` |
| `TRAINER_CRUSH_KIN_RON_MYA_2` | Hitmonchan / Hitmonlee | 33 / 33 | `ITEM_ORAN_BERRY` |
| `TRAINER_CRUSH_KIN_RON_MYA_3` | Hitmonchan / Hitmonlee | 51 / 51 | `ITEM_SITRUS_BERRY` |
| `TRAINER_CRUSH_KIN_RON_MYA_4` | Hitmonchan / Hitmonlee | 56 / 56 | `ITEM_LUM_BERRY` |
| `TRAINER_YOUNG_COUPLE_GIA_JES_2` | Nidorino / Nidorina | 30 / 30 | `ITEM_ORAN_BERRY` |
| `TRAINER_YOUNG_COUPLE_GIA_JES_3` | Nidoking / Nidoqueen | 55 / 55 | `ITEM_LUM_BERRY` |
