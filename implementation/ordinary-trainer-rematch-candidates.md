# Delegated ordinary Trainer rematch level and held-item candidates

**Project:** Pokémon Lightning Yellow. **Status: DRAFT / NOT CODEX READY.** November 6, 2026.

This is a first-pass mechanical table for 132 numeric-suffix variants linked to a base Trainer ID. It does not establish map event triggers, gameplay approval for every variant, or rematch source-code readiness. Read together with `implementation/ordinary-trainers-delegated-audit-draft.md` and the locked Drive registers. The user's Nov 6 delegation authorizes independent design choices but does not authorize game code implementation or merging.

## Binding policy

- Copy each original source Trainer's approved species families to its rematches, preserving trainer identity. When the later battle can support it, evolve species using the actual approved eligibility and progression restrictions. The listed evolution suggestions require exact evolution-rule verification.
- Preferred rematch levels follow the source rematch level while being at least 5 or more above the base Trainer's source level; these numeric choices are proposed, not tested. Respect user-approved separately designed rematches above this rule.
- Item tier: no item below Lv20, Oran Berry around Lv20-34, Sitrus Berry around Lv35-54 and Lum Berry from Lv55. Equip **only the final-slot/ace Pokémon** by default, not every party member; confirm item availability and technical legality before implementation.
- Moves: strictly select from the eventual owner-approved final learnsets/TM compatibility, covering one reliable STAB, coverage attack, and meaningful utility when available. Do not copy original FireRed source custom moves blindly. Later rematches should improve their existing role, not replace species or add competitive gimmicks.
- AI: prioritize viability and bad-move avoidance for later rematches if available in source engine, without changing the original event or single/double format.
- Source variants with differing party sizes remain **flagged exceptions** until reconciled. This draft preserves existing source slot counts, sometimes reusing a base-family species to fit them, and does not authorize new species families.

| Variant source ID | Base source ID | Class | Family-preserving proposed party | Proposed levels | Ace item | Flags |
|---|---|---|---|---|---|---|
| `TRAINER_YOUNGSTER_BEN_2` | `TRAINER_YOUNGSTER_BEN` | YOUNGSTER | Phanpy / Houndour | 17/17 | `ITEM_NONE` | Base family preserved; Moves and AI validation pending |
| `TRAINER_YOUNGSTER_BEN_3` | `TRAINER_YOUNGSTER_BEN` | YOUNGSTER | Phanpy / Houndour | 28/28 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_YOUNGSTER_BEN_4` | `TRAINER_YOUNGSTER_BEN` | YOUNGSTER | Phanpy / Houndour | 48/48 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_YOUNGSTER_CHAD_2` | `TRAINER_YOUNGSTER_CHAD` | YOUNGSTER | Houndour / Electrike | 20/20 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_LASS_RELI_2` | `TRAINER_LASS_RELI` | LASS | Nidorina / Flaaffy | 20/20 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_LASS_RELI_3` | `TRAINER_LASS_RELI` | LASS | Nidorina / Flaaffy | 28/28 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_YOUNGSTER_TIMMY_2` | `TRAINER_YOUNGSTER_TIMMY` | YOUNGSTER | Horsea / Bellsprout | 19/19 | `ITEM_NONE` | Original base party size replaces source variant size; Moves and AI validation pending |
| `TRAINER_YOUNGSTER_TIMMY_3` | `TRAINER_YOUNGSTER_TIMMY` | YOUNGSTER | Horsea / Bellsprout | 27/27 | `ITEM_ORAN_BERRY` | Original base party size replaces source variant size; Moves and AI validation pending |
| `TRAINER_YOUNGSTER_TIMMY_4` | `TRAINER_YOUNGSTER_TIMMY` | YOUNGSTER | Horsea / Bellsprout | 52/52 | `ITEM_SITRUS_BERRY` | Original base party size replaces source variant size; Moves and AI validation pending |
| `TRAINER_YOUNGSTER_CHAD_3` | `TRAINER_YOUNGSTER_CHAD` | YOUNGSTER | Houndour / Electrike | 28/28 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_LASS_JANICE_2` | `TRAINER_LASS_JANICE` | LASS | Skitty / Flaaffy | 20/20 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_LASS_JANICE_3` | `TRAINER_LASS_JANICE` | LASS | Skitty / Flaaffy | 28/28 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_YOUNGSTER_CHAD_4` | `TRAINER_YOUNGSTER_CHAD` | YOUNGSTER | Houndour / Electrike | 48/48 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_HIKER_FRANKLIN_2` | `TRAINER_HIKER_FRANKLIN` | HIKER | Machop / Snorunt | 25/25 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BIKER_GOON_2` | `TRAINER_BIKER_GOON` | BIKER | Weezing / Muk | 40/40 | `ITEM_SITRUS_BERRY` | Original base party size replaces source variant size; Moves and AI validation pending |
| `TRAINER_BIKER_GOON_3` | `TRAINER_BIKER_GOON` | BIKER | Weezing / Muk | 40/40 | `ITEM_SITRUS_BERRY` | Original base party size replaces source variant size; Moves and AI validation pending |
| `TRAINER_TWINS_ELI_ANNE_2` | `TRAINER_TWINS_ELI_ANNE` | TWINS | Flaaffy / Flaaffy | 28/28 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_POKEMANIAC_MARK_2` | `TRAINER_POKEMANIAC_MARK` | POKEMANIAC | Lickitung / Aerodactyl | 33/33 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PICNICKER_KELSEY_2` | `TRAINER_PICNICKER_KELSEY` | PICNICKER | Eevee / Pikachu | 21/21 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PICNICKER_KELSEY_3` | `TRAINER_PICNICKER_KELSEY` | PICNICKER | Eevee / Pikachu | 29/29 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PICNICKER_KELSEY_4` | `TRAINER_PICNICKER_KELSEY` | PICNICKER | Jolteon / Raichu | 49/49 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CAMPER_RICKY_2` | `TRAINER_CAMPER_RICKY` | CAMPER | Psyduck | 23 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CAMPER_RICKY_3` | `TRAINER_CAMPER_RICKY` | CAMPER | Psyduck | 30 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CAMPER_RICKY_4` | `TRAINER_CAMPER_RICKY` | CAMPER | Golduck | 55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CAMPER_JEFF_2` | `TRAINER_CAMPER_JEFF` | CAMPER | Fearow / Nuzleaf | 21/21 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CAMPER_JEFF_3` | `TRAINER_CAMPER_JEFF` | CAMPER | Fearow / Nuzleaf | 29/29 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CAMPER_JEFF_4` | `TRAINER_CAMPER_JEFF` | CAMPER | Fearow / Shiftry | 54/54 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PICNICKER_ISABELLE_2` | `TRAINER_PICNICKER_ISABELLE` | PICNICKER | Skitty / Clefairy / Pidgeotto | 21/21/19 | `ITEM_NONE` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PICNICKER_ISABELLE_3` | `TRAINER_PICNICKER_ISABELLE` | PICNICKER | Skitty / Clefairy / Pidgeotto | 29/29/26 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PICNICKER_ISABELLE_4` | `TRAINER_PICNICKER_ISABELLE` | PICNICKER | Delcatty / Clefable / Pidgeot | 47/47/50 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_YOUNGSTER_YASU_2` | `TRAINER_YOUNGSTER_YASU` | YOUNGSTER | Phanpy / Tyrogue / Electrike | 22/22/22 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_YOUNGSTER_YASU_3` | `TRAINER_YOUNGSTER_YASU` | YOUNGSTER | Phanpy / Tyrogue / Electrike | 47/47/47 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_ENGINEER_BERNIE_2` | `TRAINER_ENGINEER_BERNIE` | ENGINEER | Magnemite / Sandslash / Voltorb | 28/28/28 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_GAMER_DARIAN_2` | `TRAINER_GAMER_DARIAN` | GAMER | Castform / Porygon | 29/29 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CAMPER_CHRIS_2` | `TRAINER_CAMPER_CHRIS` | CAMPER | Psyduck / Teddiursa | 24/24 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CAMPER_CHRIS_3` | `TRAINER_CAMPER_CHRIS` | CAMPER | Psyduck / Teddiursa | 29/29 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CAMPER_CHRIS_4` | `TRAINER_CAMPER_CHRIS` | CAMPER | Golduck / Ursaring | 54/54 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PICNICKER_ALICIA_2` | `TRAINER_PICNICKER_ALICIA` | PICNICKER | Clefairy / Breloom / Poliwhirl / Pikachu | 25/25/25/25 | `ITEM_ORAN_BERRY` | Original base party size replaces source variant size; Moves and AI validation pending |
| `TRAINER_PICNICKER_ALICIA_3` | `TRAINER_PICNICKER_ALICIA` | PICNICKER | Clefairy / Breloom / Poliwhirl / Pikachu | 30/30/30/30 | `ITEM_ORAN_BERRY` | Original base party size replaces source variant size; Moves and AI validation pending |
| `TRAINER_PICNICKER_ALICIA_4` | `TRAINER_PICNICKER_ALICIA` | PICNICKER | Clefable / Breloom / Poliwrath / Raichu | 55/55/55/55 | `ITEM_LUM_BERRY` | Original base party size replaces source variant size; Moves and AI validation pending |
| `TRAINER_HIKER_JEREMY_2` | `TRAINER_HIKER_JEREMY` | HIKER | Machoke / Aron | 30/28 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_POKEMANIAC_MARK_3` | `TRAINER_POKEMANIAC_MARK` | POKEMANIAC | Lickitung / Aerodactyl | 54/54 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_POKEMANIAC_HERMAN_2` | `TRAINER_POKEMANIAC_HERMAN` | POKEMANIAC | Kecleon / Porygon | 29/29 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_POKEMANIAC_HERMAN_3` | `TRAINER_POKEMANIAC_HERMAN` | POKEMANIAC | Kecleon / Porygon2 | 54/54 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_HIKER_TRENT_2` | `TRAINER_HIKER_TRENT` | HIKER | Onix / Swinub | 31/31 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_LASS_MEGAN_2` | `TRAINER_LASS_MEGAN` | LASS | Vulpix / Flaaffy / Nidorina / Meowth / Poliwag | 22/22/23/22/22 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_LASS_MEGAN_3` | `TRAINER_LASS_MEGAN` | LASS | Ninetales / Ampharos / Nidoqueen / Persian / Poliwrath | 46/47/47/47/48 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_SUPER_NERD_GLENN_2` | `TRAINER_SUPER_NERD_GLENN` | SUPER NERD | Grimer / Muk / Koffing | 28/28/28 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_GAMER_RICH_2` | `TRAINER_GAMER_RICH` | GAMER | Pineco / Ditto | 30/30 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BIKER_JAREN_2` | `TRAINER_BIKER_JAREN` | BIKER | Grimer / Electrode | 28/30 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_FISHERMAN_ELLIOT_2` | `TRAINER_FISHERMAN_ELLIOT` | FISHERMAN | Shellder / Goldeen / Horsea / Spheal | 28/28/28/28 | `ITEM_ORAN_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_ROCKER_LUCA_2` | `TRAINER_ROCKER_LUCA` | ROCKER | Electrode / Magmar | 34/34 | `ITEM_ORAN_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_BEAUTY_SHEILA_2` | `TRAINER_BEAUTY_SHEILA` | BEAUTY | Persian / Gardevoir | 49/49 | `ITEM_SITRUS_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_BIRD_KEEPER_ROBERT_2` | `TRAINER_BIRD_KEEPER_ROBERT` | BIRD KEEPER | Pidgeotto / Pidgeotto / Fearow / Fearow | 31/31/31/31 | `ITEM_ORAN_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_BIRD_KEEPER_ROBERT_3` | `TRAINER_BIRD_KEEPER_ROBERT` | BIRD KEEPER | Pidgeot / Pidgeot / Fearow / Fearow | 47/47/47/47 | `ITEM_SITRUS_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_PICNICKER_SUSIE_2` | `TRAINER_PICNICKER_SUSIE` | PICNICKER | Pidgeotto / Pikachu / Breloom / Clefairy / Poliwhirl | 29/29/29/29/29 | `ITEM_ORAN_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_PICNICKER_SUSIE_3` | `TRAINER_PICNICKER_SUSIE` | PICNICKER | Pidgeot / Raichu / Breloom / Clefable / Poliwrath | 47/47/47/47/47 | `ITEM_SITRUS_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_PICNICKER_SUSIE_4` | `TRAINER_PICNICKER_SUSIE` | PICNICKER | Pidgeot / Raichu / Breloom / Clefable / Poliwrath | 52/52/52/52/52 | `ITEM_SITRUS_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_BIKER_LUKAS_2` | `TRAINER_BIKER_LUKAS` | BIKER | Weezing / Magneton / Muk / Electrode | 47/47/47/47 | `ITEM_SITRUS_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_BIRD_KEEPER_BENNY_2` | `TRAINER_BIRD_KEEPER_BENNY` | BIRD KEEPER | Fearow / Fearow | 34/34 | `ITEM_ORAN_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_BIRD_KEEPER_BENNY_3` | `TRAINER_BIRD_KEEPER_BENNY` | BIRD KEEPER | Fearow / Fearow | 49/49 | `ITEM_SITRUS_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_BIRD_KEEPER_MARLON_2` | `TRAINER_BIRD_KEEPER_MARLON` | BIRD KEEPER | Fearow / Doduo / Fearow | 33/33/33 | `ITEM_ORAN_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_BIRD_KEEPER_MARLON_3` | `TRAINER_BIRD_KEEPER_MARLON` | BIRD KEEPER | Fearow / Dodrio / Fearow | 48/48/48 | `ITEM_SITRUS_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_BEAUTY_GRACE_2` | `TRAINER_BEAUTY_GRACE` | BEAUTY | Gardevoir / Persian | 49/49 | `ITEM_SITRUS_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_BIRD_KEEPER_CHESTER_2` | `TRAINER_BIRD_KEEPER_CHESTER` | BIRD KEEPER | Farfetch'd / Doduo / Doduo | 33/33/33 | `ITEM_ORAN_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_BIRD_KEEPER_CHESTER_3` | `TRAINER_BIRD_KEEPER_CHESTER` | BIRD KEEPER | Farfetch'd / Dodrio / Dodrio | 48/48/48 | `ITEM_SITRUS_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_PICNICKER_BECKY_2` | `TRAINER_PICNICKER_BECKY` | PICNICKER | Pikachu / Raichu | 34/34 | `ITEM_ORAN_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_PICNICKER_BECKY_3` | `TRAINER_PICNICKER_BECKY` | PICNICKER | Raichu / Raichu | 49/49 | `ITEM_SITRUS_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_PICNICKER_BECKY_4` | `TRAINER_PICNICKER_BECKY` | PICNICKER | Raichu / Raichu | 54/54 | `ITEM_SITRUS_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_CRUSH_KIN_RON_MYA_2` | `TRAINER_CRUSH_KIN_RON_MYA` | CRUSH KIN | Hitmonchan / Hitmonlee | 34/34 | `ITEM_ORAN_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_CRUSH_KIN_RON_MYA_3` | `TRAINER_CRUSH_KIN_RON_MYA` | CRUSH KIN | Hitmonchan / Hitmonlee | 51/51 | `ITEM_SITRUS_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_CRUSH_KIN_RON_MYA_4` | `TRAINER_CRUSH_KIN_RON_MYA` | CRUSH KIN | Hitmonchan / Hitmonlee | 56/56 | `ITEM_LUM_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_BIKER_RUBEN_2` | `TRAINER_BIKER_RUBEN` | BIKER | Weezing / Muk / Weezing | 48/48/48 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CUE_BALL_CAMRON_2` | `TRAINER_CUE_BALL_CAMRON` | CUE BALL | Primeape / Sneasel | 49/49 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BIKER_JAXON_2` | `TRAINER_BIKER_JAXON` | BIKER | Weezing / Magneton | 49/49 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CUE_BALL_ISAIAH_2` | `TRAINER_CUE_BALL_ISAIAH` | CUE BALL | Rhydon / Primeape | 49/49 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CUE_BALL_COREY_2` | `TRAINER_CUE_BALL_COREY` | CUE BALL | Primeape / Sneasel | 49/49 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BIRD_KEEPER_JACOB_2` | `TRAINER_BIRD_KEEPER_JACOB` | BIRD KEEPER | Fearow / Noctowl / Swellow / Fearow | 29/29/29/29 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BIRD_KEEPER_JACOB_3` | `TRAINER_BIRD_KEEPER_JACOB` | BIRD KEEPER | Fearow / Noctowl / Swellow / Fearow | 47/47/47/47 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_SWIMMER_FEMALE_ALICE_2` | `TRAINER_SWIMMER_FEMALE_ALICE` | SWIMMER F | Starmie / Vaporeon | 49/49 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_SWIMMER_MALE_DARRIN_2` | `TRAINER_SWIMMER_MALE_DARRIN` | SWIMMER M | Blastoise / Sharpedo / Starmie / Slowbro | 52/52/52/52 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PICNICKER_MISSY_2` | `TRAINER_PICNICKER_MISSY` | PICNICKER | Poliwrath / Jolteon | 49/49 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PICNICKER_MISSY_3` | `TRAINER_PICNICKER_MISSY` | PICNICKER | Poliwrath / Jolteon | 54/54 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_FISHERMAN_WADE_2` | `TRAINER_FISHERMAN_WADE` | FISHERMAN | Gyarados / Gyarados / Gyarados / Gyarados / Gyarados / Gyarados | 47/47/47/47/47/47 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_SWIMMER_MALE_JACK_2` | `TRAINER_SWIMMER_MALE_JACK` | SWIMMER M | Sharpedo | 50 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_SIS_AND_BRO_LIL_IAN_2` | `TRAINER_SIS_AND_BRO_LIL_IAN` | SIS AND BRO | Persian / Arcanine | 50/50 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_SIS_AND_BRO_LIL_IAN_3` | `TRAINER_SIS_AND_BRO_LIL_IAN` | SIS AND BRO | Persian / Arcanine | 55/55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_SWIMMER_MALE_FINN_2` | `TRAINER_SWIMMER_MALE_FINN` | SWIMMER M | Blastoise | 50 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CRUSH_GIRL_SHARON_2` | `TRAINER_CRUSH_GIRL_SHARON` | CRUSH GIRL | Golem / Glalie | 50/50 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CRUSH_GIRL_SHARON_3` | `TRAINER_CRUSH_GIRL_SHARON` | CRUSH GIRL | Golem / Glalie | 55/55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CRUSH_GIRL_TANYA_2` | `TRAINER_CRUSH_GIRL_TANYA` | CRUSH GIRL | Aggron / Gyarados | 50/50 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CRUSH_GIRL_TANYA_3` | `TRAINER_CRUSH_GIRL_TANYA` | CRUSH GIRL | Aggron / Gyarados | 55/55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BLACK_BELT_SHEA_2` | `TRAINER_BLACK_BELT_SHEA` | BLACK BELT | Primeape / Medicham | 50/50 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BLACK_BELT_SHEA_3` | `TRAINER_BLACK_BELT_SHEA` | BLACK BELT | Primeape / Medicham | 55/55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BLACK_BELT_HUGH_2` | `TRAINER_BLACK_BELT_HUGH` | BLACK BELT | Hitmonlee / Hitmonchan | 50/50 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BLACK_BELT_HUGH_3` | `TRAINER_BLACK_BELT_HUGH` | BLACK BELT | Hitmonlee / Hitmonchan | 55/55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CRUSH_KIN_MIK_KIA_2` | `TRAINER_CRUSH_KIN_MIK_KIA` | CRUSH KIN | Pinsir / Scyther | 51/51 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CRUSH_KIN_MIK_KIA_3` | `TRAINER_CRUSH_KIN_MIK_KIA` | CRUSH KIN | Pinsir / Scyther | 56/56 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_TUBER_AMIRA_2` | `TRAINER_TUBER_AMIRA` | TUBER | Poliwag / Spheal / Horsea | 47/47/47 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_TWINS_JOY_MEG_2` | `TRAINER_TWINS_JOY_MEG` | TWINS | Delcatty / Delcatty | 49/49 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PAINTER_RAYNA_2` | `TRAINER_PAINTER_RAYNA` | PAINTER | Castform | 54 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_YOUNGSTER_DESTIN_2` | `TRAINER_YOUNGSTER_DESTIN` | YOUNGSTER | Taillow / Electrike | 53/53 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PKMN_BREEDER_ALIZE_2` | `TRAINER_PKMN_BREEDER_ALIZE` | PKMN BREEDER | Pichu / Pikachu / Raichu | 53/53/53 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_YOUNG_COUPLE_GIA_JES_2` | `TRAINER_YOUNG_COUPLE_GIA_JES` | YOUNG COUPLE | Nidorino / Nidorina | 30/30 | `ITEM_ORAN_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_YOUNG_COUPLE_GIA_JES_3` | `TRAINER_YOUNG_COUPLE_GIA_JES` | YOUNG COUPLE | Nidoking / Nidoqueen | 55/55 | `ITEM_LUM_BERRY` | Curated family progression; source levels provisional; moves pending |
| `TRAINER_BIRD_KEEPER_MILO_2` | `TRAINER_BIRD_KEEPER_MILO` | BIRD KEEPER | Fearow / Dodrio | 53/55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BIRD_KEEPER_CHAZ_2` | `TRAINER_BIRD_KEEPER_CHAZ` | BIRD KEEPER | Swellow / Pidgeot | 53/55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BIRD_KEEPER_HAROLD_2` | `TRAINER_BIRD_KEEPER_HAROLD` | BIRD KEEPER | Noctowl / Noctowl | 53/55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_SWIMMER_FEMALE_NICOLE_2` | `TRAINER_SWIMMER_FEMALE_NICOLE` | SWIMMER F | Sharpedo | 54 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PSYCHIC_JACLYN_2` | `TRAINER_PSYCHIC_JACLYN` | PSYCHIC | Xatu / Slowbro / Alakazam | 52/52/54 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_SWIMMER_MALE_SAMIR_2` | `TRAINER_SWIMMER_MALE_SAMIR` | SWIMMER M | Gyarados | 55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_HIKER_EARL_2` | `TRAINER_HIKER_EARL` | HIKER | Steelix / Machamp | 54/54 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_RUIN_MANIAC_LARRY_2` | `TRAINER_RUIN_MANIAC_LARRY` | RUIN MANIAC | Omastar / Kabutops | 54/54 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_POKEMANIAC_HECTOR_2` | `TRAINER_POKEMANIAC_HECTOR` | POKEMANIAC | Kecleon / Kangaskhan | 55/55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PSYCHIC_DARIO_2` | `TRAINER_PSYCHIC_DARIO` | PSYCHIC | Espeon | 56 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PSYCHIC_RODETTE_2` | `TRAINER_PSYCHIC_RODETTE` | PSYCHIC | Xatu / Hypno / Gardevoir | 53/53/53 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_JUGGLER_MASON_2` | `TRAINER_JUGGLER_MASON` | JUGGLER | Primeape / Kecleon / Lickitung / Jynx | 52/52/52/52 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PKMN_RANGER_NICOLAS_2` | `TRAINER_PKMN_RANGER_NICOLAS` | PKMN RANGER | Arcanine / Noctowl | 55/55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PKMN_RANGER_MADELINE_2` | `TRAINER_PKMN_RANGER_MADELINE` | PKMN RANGER | Golduck / Fearow | 55/55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_CRUSH_GIRL_CYNDY_2` | `TRAINER_CRUSH_GIRL_CYNDY` | CRUSH GIRL | Golem / Glalie / Camerupt | 54/54/54 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_TAMER_EVAN_2` | `TRAINER_TAMER_EVAN` | TAMER | Ursaring / Kangaskhan / Absol | 52/52/55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_PKMN_RANGER_JACKSON_2` | `TRAINER_PKMN_RANGER_JACKSON` | PKMN RANGER | Golduck / Noctowl | 53/54 | `ITEM_SITRUS_BERRY` | Original base party size replaces source variant size; Moves and AI validation pending |
| `TRAINER_PKMN_RANGER_KATELYN_2` | `TRAINER_PKMN_RANGER_KATELYN` | PKMN RANGER | Persian / Manectric | 56/56 | `ITEM_LUM_BERRY` | Original base party size replaces source variant size; Moves and AI validation pending |
| `TRAINER_COOLTRAINER_LEROY_2` | `TRAINER_COOLTRAINER_LEROY` | COOLTRAINER | Rhydon / Gardevoir / Weezing / Machamp / Steelix | 52/54/52/52/55 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_COOLTRAINER_MICHELLE_2` | `TRAINER_COOLTRAINER_MICHELLE` | COOLTRAINER | Arcanine / Milotic / Electabuzz / Glalie / Absol | 53/53/54/54/56 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_COOL_COUPLE_LEX_NYA_2` | `TRAINER_COOL_COUPLE_LEX_NYA` | COOL COUPLE | Magmar / Cloyster | 57/57 | `ITEM_LUM_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BUG_CATCHER_COLTON_2` | `TRAINER_BUG_CATCHER_COLTON` | BUG CATCHER | Butterfree / Beedrill / Paras | 19/19/19 | `ITEM_NONE` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BUG_CATCHER_COLTON_3` | `TRAINER_BUG_CATCHER_COLTON` | BUG CATCHER | Butterfree / Beedrill / Parasect | 27/27/27 | `ITEM_ORAN_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_BUG_CATCHER_COLTON_4` | `TRAINER_BUG_CATCHER_COLTON` | BUG CATCHER | Butterfree / Beedrill / Parasect | 51/54/51 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_SWIMMER_MALE_MATTHEW_2` | `TRAINER_SWIMMER_MALE_MATTHEW` | SWIMMER M | Blastoise / Walrein | 49/49 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_SWIMMER_MALE_TONY_2` | `TRAINER_SWIMMER_MALE_TONY` | SWIMMER M | Slowbro / Starmie | 49/49 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |
| `TRAINER_SWIMMER_FEMALE_MELISSA_2` | `TRAINER_SWIMMER_FEMALE_MELISSA` | SWIMMER F | Starmie / Castform | 49/49 | `ITEM_SITRUS_BERRY` | Base family preserved; Moves and AI validation pending |

## Blockers and checks

- [ ] Resolve rematch variants whose actual event linkage cannot be inferred from numeric suffixes, especially suffixes with no base ID.
- [ ] Distinguish special separately owner-approved rematches from these default continuations, and restore their exact approved details.
- [ ] Confirm all suggested evolved stages remain eligible in the locked class pool; e.g. Youngsters and Tubers never gain evolved forms.
- [ ] Validate all moves after 252-species species-wide learnsets are finalized, and inspect actual source battle AI and level access.
- [ ] Verify held item constant usability, encounter rematch availability, party sizes, battle format, NPC flags and expected player levels.
- [ ] Test difficulty with emulator encounters after separately authorized code implementation; do not claim balanced or tested from these tables alone.

**No gameplay code changed; no merge permission.**
