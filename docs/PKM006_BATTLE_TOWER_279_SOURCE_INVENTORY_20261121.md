# PKM-006 Battle Tower 279-entry source inventory — PR #288

## Evidence and implementation gate

Both `src/data/battle_tower/level_50_mons.h` and `level_100_mons.h` contain 300 ordered Pokémon templates. `src/battle_tower.c` includes them directly, selects a level 50 or 100 pool, chooses from tier-dependent offset/windows, filters teamFlags and duplicate species/items, then uses fixed trainer-tier IVs, EV spread, nature, four explicitly assigned moves and held item. **279 templates are outside the approved 252-species roster: 131 level 50 and 148 level 100.** They are real code-backed opponent candidates, though the facility's game-world accessibility is unverified.

Existing user delegation covered 54 *Trainer Tower* entries, not these separate 279 *Battle Tower* entries. No exact Battle Tower template mapping or exclusive ownership was found in the inspected PRs and canonical wild/SOW registers. **Do not substitute species before owner extends approval.** No production template edits made.

## Defensible replacement contract

- Keep exactly 300 entries per pool and preserve every template index, teamFlags, held-item index, EV spread, nature, and trainer tier. Candidate species must belong to the approved 252 and approximate original type, stat tier, ability and strategic role.
- Review all four moves per template against the candidate's approved level-up/TM/HM/tutor/egg learnset and game-specific move behavior; the engine's direct `SetMonMoveSlot` does not guarantee legality. Any necessary move changes require explicit reasons and balance parity.
- Review each trainer tier's index window and teamFlags masks for at least three eligible distinct species and nonduplicated items. Preserve original selection weight and rising difficulty. Special care for Wobbuffet counter sets, Shedinja gimmicks, legendary templates and weather strategies.
- Decide whether the Battle Tower should be accessible in this ROM, and separately account for e-Reader and record-mixing opponents. Code presence alone does not prove in-game accessibility.
- Protected WRL wild encounters and three SOW/story legendary events remain untouched. No builds, tests, emulators, CI or merges. 50 IDs; 461/251/712 unchanged.

## Exact off-Dex source inventory

| Pool | Zero-based index | Current species | Team flags | Held item | EV spread | Nature | Four moves | Replacement |
|---|---:|---|---|---|---|---|---|---|
| 50 | 1 | BEAUTIFLY | 0x41 | MIRACLE_SEED | ? | ? | ABSORB, STUN_SPORE, GUST, SAFEGUARD | BUTTERFREE (staged) |
| 50 | 5 | TORCHIC | 0x42 | KINGS_ROCK | ? | ? | FIRE_SPIN, PROTECT, SAND_ATTACK, SLASH | CHARMANDER (staged) |
| 50 | 6 | MASQUERAIN | 0x41 | WHITE_HERB | ? | ? | STUN_SPORE, WATER_SPORT, QUICK_ATTACK, GUST | VENOMOTH (staged) |
| 50 | 7 | ILLUMISE | 0x41 | NONE | ? | ? | FLATTER, COVET, WISH, FACADE | BUTTERFREE (staged) |
| 50 | 10 | MIGHTYENA | 0x05 | PECHA_BERRY | ? | ? | BITE, TAKE_DOWN, ROAR, HOWL | HOUNDOOM (staged) |
| 50 | 11 | SLAKING | 0x05 | BLACK_GLASSES | ? | ? | FAINT_ATTACK, COUNTER, SCRATCH, TAUNT | URSARING (staged) |
| 50 | 15 | ZIGZAGOON | 0x0A | KINGS_ROCK | ? | ? | FLAIL, SAND_ATTACK, REST, SURF | RATTATA (staged) |
| 50 | 17 | WHISCASH | 0x0B | QUICK_CLAW | ? | ? | WATER_GUN, AMNESIA, MAGNITUDE, MUD_SLAP | KINGLER (staged) |
| 50 | 18 | NINJASK | 0x41 | SILVER_POWDER | ? | ? | SAND_ATTACK, FURY_CUTTER, AGILITY, HARDEN | YANMA (staged) |
| 50 | 21 | SPINDA | 0x4A | CHERI_BERRY | ? | ? | TEETER_DANCE, FLAIL, PSYCH_UP, DOUBLE_EDGE | DELCATTY (staged) |
| 50 | 22 | SEVIPER | 0x09 | POISON_BARB | ? | ? | SCREECH, GLARE, DIG, POISON_FANG | ARBOK (staged) |
| 50 | 24 | AZUMARILL | 0x42 | WHITE_HERB | ? | ? | BUBBLE_BEAM, TAIL_WHIP, RAIN_DANCE, ROLLOUT | POLIWRATH (staged) |
| 50 | 25 | ZANGOOSE | 0x07 | MENTAL_HERB | ? | ? | STRENGTH, TAUNT, CRUSH_CLAW, DETECT | PERSIAN (staged) |
| 50 | 27 | ROSELIA | 0x0A | MIRACLE_SEED | ? | ? | GROWTH, MEGA_DRAIN, TOXIC, CUT | VILEPLUME (staged) |
| 50 | 28 | SWALOT | 0x0B | LUM_BERRY | ? | ? | STOCKPILE, SPIT_UP, SLUDGE, POISON_GAS | MUK (staged) |
| 50 | 31 | RELICANTH | 0x01 | MYSTIC_WATER | ? | ? | SURF, ANCIENT_POWER, ROCK_TOMB, HARDEN | OMASTAR (staged) |
| 50 | 34 | GIRAFARIG | 0x03 | KINGS_ROCK | ? | ? | STRENGTH, PSYBEAM, AGILITY, EARTHQUAKE | HYPNO (staged) |
| 50 | 35 | PELIPPER | 0x0B | SITRUS_BERRY | ? | ? | MIST, STOCKPILE, SWALLOW, SPIT_UP | SWELLOW (staged) |
| 50 | 36 | SABLEYE | 0x09 | CHERI_BERRY | ? | ? | SHADOW_BALL, NIGHT_SHADE, SNATCH, FAKE_OUT | BANETTE (staged) |
| 50 | 37 | LUNATONE | 0x0B | HARD_STONE | ? | ? | COSMIC_POWER, SANDSTORM, PSYCHIC, ROCK_THROW | XATU (staged) |
| 50 | 38 | LUDICOLO | 0x0B | NONE | ? | ? | THIEF, FAKE_OUT, SURF, RAIN_DANCE | POLITOED (staged) |
| 50 | 39 | GRUMPIG | 0x0B | SALAC_BERRY | ? | ? | ODOR_SLEUTH, CONFUSE_RAY, MAGIC_COAT, PSYCHIC | HYPNO (staged) |
| 50 | 41 | WAILORD | 0x03 | MYSTIC_WATER | ? | ? | WATER_SPOUT, ROLLOUT, MIST, RAIN_DANCE | LAPRAS (staged) |
| 50 | 47 | LOUDRED | 0x05 | SITRUS_BERRY | ? | ? | UPROAR, STOMP, SCREECH, EARTHQUAKE | LICKITUNG (staged) |
| 50 | 48 | SOLROCK | 0x03 | LEFTOVERS | ? | ? | ROCK_THROW, SUNNY_DAY, SOLAR_BEAM, SANDSTORM | MAGCARGO (staged) |
| 50 | 49 | CLAYDOL | 0x03 | BRIGHT_POWDER | ? | ? | SANDSTORM, REFLECT, ROCK_TOMB, STRENGTH | GOLEM (staged) |
| 50 | 54 | NOSEPASS | 0x0B | PERSIM_BERRY | ? | ? | SANDSTORM, ROCK_SLIDE, BLOCK, THUNDER_WAVE | SUDOWOODO (staged) |
| 50 | 58 | DUSTOX | 0x01 | SILVER_POWDER | ? | ? | CONFUSION, SILVER_WIND, PSYBEAM, WHIRLWIND | VENOMOTH (staged) |
| 50 | 61 | SKARMORY | 0x07 | SITRUS_BERRY | ? | ? | STEEL_WING, SAND_ATTACK, SWIFT, AGILITY | STEELIX (staged) |
| 50 | 62 | TORKOAL | 0x43 | FOCUS_BAND | ? | ? | FLAMETHROWER, IRON_DEFENSE, BODY_SLAM, PROTECT | MAGCARGO (staged) |
| 50 | 66 | WOBBUFFET | 0x0B | SALAC_BERRY | ? | ? | COUNTER, MIRROR_COAT, SAFEGUARD, DESTINY_BOND | BLISSEY (staged) |
| 50 | 68 | TROPIUS | 0x05 | LEFTOVERS | ? | ? | SUNNY_DAY, SOLAR_BEAM, SYNTHESIS, RAZOR_LEAF | TANGELA (staged) |
| 50 | 69 | MAWILE | 0x43 | METAL_COAT | ? | ? | IRON_DEFENSE, SLUDGE_BOMB, CRUNCH, STRENGTH | SCIZOR (staged) |
| 50 | 72 | LINOONE | 0x07 | PECHA_BERRY | ? | ? | HEADBUTT, TAIL_WHIP, SURF, DIG | RATICATE (staged) |
| 50 | 77 | CORSOLA | 0x0A | SCOPE_LENS | ? | ? | RECOVER, MIRROR_COAT, HAIL, SURF | STARMIE (staged) |
| 50 | 79 | EXPLOUD | 0x41 | LEFTOVERS | ? | ? | HYPER_BEAM, HOWL, UPROAR, EARTHQUAKE | SNORLAX (staged) |
| 50 | 83 | VIGOROTH | 0x05 | FOCUS_BAND | ? | ? | FOCUS_PUNCH, REVERSAL, COUNTER, TAUNT | PRIMEAPE (staged) |
| 50 | 85 | LANTURN | 0x07 | QUICK_CLAW | ? | ? | FLAIL, SURF, SPARK, TAKE_DOWN | AMPHAROS (staged) |
| 50 | 86 | HARIYAMA | 0x05 | KINGS_ROCK | ? | ? | STRENGTH, FAKE_OUT, FACADE, SAND_ATTACK | MACHAMP (staged) |
| 50 | 93 | SWAMPERT | 0x41 | NEVER_MELT_ICE | ? | ? | PROTECT, ICE_BEAM, HYPER_BEAM, SURF | BLASTOISE (staged) |
| 50 | 94 | SCEPTILE | 0x41 | MIRACLE_SEED | ? | ? | LEAF_BLADE, DRAGON_CLAW, AGILITY, PROTECT | VENUSAUR (staged) |
| 50 | 95 | BLAZIKEN | 0x41 | CHARCOAL | ? | ? | BLAZE_KICK, MIRROR_MOVE, EARTHQUAKE, PROTECT | CHARIZARD (staged) |
| 50 | 96 | ALTARIA | 0x43 | DRAGON_FANG | ? | ? | DRAGON_CLAW, DRAGON_DANCE, TOXIC, DOUBLE_TEAM | DRAGONITE (staged) |
| 50 | 97 | LUDICOLO | 0x43 | BLACK_BELT | ? | ? | BRICK_BREAK, WATER_PULSE, RAIN_DANCE, FOCUS_PUNCH | POLITOED (staged) |
| 50 | 98 | SABLEYE | 0x49 | SPELL_TAG | ? | ? | SHADOW_BALL, SNATCH, DIG, CONFUSE_RAY | BANETTE (staged) |
| 50 | 99 | ZANGOOSE | 0x0F | BLACK_GLASSES | ? | ? | FACADE, THUNDERBOLT, CRUSH_CLAW, DETECT | PERSIAN (staged) |
| 50 | 100 | LINOONE | 0x0F | CHESTO_BERRY | ? | ? | IRON_TAIL, HEADBUTT, ATTRACT, ICE_BEAM | RATICATE (staged) |
| 50 | 101 | WHISCASH | 0x07 | SOFT_SAND | ? | ? | MUD_SLAP, EARTHQUAKE, SURF, AMNESIA | KINGLER (staged) |
| 50 | 102 | SOLROCK | 0x43 | HARD_STONE | ? | ? | EARTHQUAKE, LIGHT_SCREEN, ROCK_SLIDE, CONFUSION | MAGCARGO (staged) |
| 50 | 109 | METANG | 0x05 | METAL_COAT | ? | ? | EARTHQUAKE, SHADOW_BALL, PSYCHIC, METEOR_MASH | MAGNETON (staged) |
| 50 | 110 | CLAYDOL | 0x43 | PECHA_BERRY | ? | ? | EARTHQUAKE, SELF_DESTRUCT, SANDSTORM, PROTECT | GOLEM (staged) |
| 50 | 111 | MASQUERAIN | 0x0A | PERSIM_BERRY | ? | ? | TOXIC, SILVER_WIND, ATTRACT, ICE_BEAM | VENOMOTH (staged) |
| 50 | 112 | RELICANTH | 0xC5 | LIECHI_BERRY | ? | ? | ROCK_TOMB, DOUBLE_EDGE, SURF, EARTHQUAKE | OMASTAR (staged) |
| 50 | 114 | WAILORD | 0x0B | APICOT_BERRY | ? | ? | WATER_SPOUT, REST, AMNESIA, HAIL | LAPRAS (staged) |
| 50 | 115 | GIRAFARIG | 0x4F | GANLON_BERRY | ? | ? | DOUBLE_TEAM, CRUNCH, PSYCHIC, EARTHQUAKE | HYPNO (staged) |
| 50 | 117 | TROPIUS | 0xC7 | MENTAL_HERB | ? | ? | EARTHQUAKE, SYNTHESIS, RAZOR_LEAF, HYPER_BEAM | TANGELA (staged) |
| 50 | 118 | NINJASK | 0x05 | SCOPE_LENS | ? | ? | DOUBLE_TEAM, SLASH, SWORDS_DANCE, LEECH_LIFE | YANMA (staged) |
| 50 | 122 | SEVIPER | 0x05 | POISON_BARB | ? | ? | SLUDGE_BOMB, HAZE, CRUNCH, FLAMETHROWER | ARBOK (staged) |
| 50 | 123 | CRADILY | 0xC1 | LEFTOVERS | ? | ? | ROCK_TOMB, CONFUSE_RAY, INGRAIN, EARTHQUAKE | ARMALDO (staged) |
| 50 | 126 | LANTURN | 0x06 | QUICK_CLAW | ? | ? | THUNDER, SURF, ICE_BEAM, RAIN_DANCE | AMPHAROS (staged) |
| 50 | 127 | SWALOT | 0x8B | CHERI_BERRY | ? | ? | SLUDGE_BOMB, SNATCH, ENCORE, BODY_SLAM | MUK (staged) |
| 50 | 128 | SKARMORY | 0x81 | METAL_COAT | ? | ? | STEEL_WING, AGILITY, TORMENT, FACADE | STEELIX (staged) |
| 50 | 134 | GRUMPIG | 0x8B | TWISTED_SPOON | ? | ? | MAGIC_COAT, PSYCHIC, REFLECT, CONFUSE_RAY | HYPNO (staged) |
| 50 | 138 | TORKOAL | 0x09 | LEFTOVERS | ? | ? | FLAMETHROWER, AMNESIA, IRON_TAIL, REST | MAGCARGO (staged) |
| 50 | 145 | EXPLOUD | 0x05 | CHOICE_BAND | ? | ? | BLIZZARD, FLAMETHROWER, EARTHQUAKE, RETURN | SNORLAX (staged) |
| 50 | 151 | SLAKING | 0x03 | LEFTOVERS | ? | ? | CURSE, AMNESIA, YAWN, FOCUS_PUNCH | URSARING (staged) |
| 50 | 153 | HARIYAMA | 0x21 | SALAC_BERRY | ? | ? | BELLY_DRUM, ENDURE, EARTHQUAKE, REVERSAL | MACHAMP (staged) |
| 50 | 155 | ALTARIA | 0x83 | SITRUS_BERRY | ? | ? | DRAGON_DANCE, MIRROR_MOVE, DRAGON_CLAW, EARTHQUAKE | DRAGONITE (staged) |
| 50 | 158 | BLAZIKEN | 0x33 | PETAYA_BERRY | ? | ? | COUNTER, ROCK_SLIDE, FOCUS_PUNCH, FLAMETHROWER | CHARIZARD (staged) |
| 50 | 160 | HUNTAIL | 0xA1 | MYSTIC_WATER | ? | ? | RAIN_DANCE, HYDRO_PUMP, CRUNCH, BLIZZARD | SHARPEDO (staged) |
| 50 | 162 | GOREBYSS | 0x83 | PECHA_BERRY | ? | ? | PSYCHIC, SURF, BLIZZARD, SHADOW_BALL | MILOTIC (staged) |
| 50 | 163 | SCEPTILE | 0x53 | SCOPE_LENS | ? | ? | LEAF_BLADE, ENDEAVOR, EARTHQUAKE, ROCK_TOMB | VENUSAUR (staged) |
| 50 | 164 | SWAMPERT | 0x53 | LEFTOVERS | ? | ? | EARTHQUAKE, SURF, MUD_SHOT, BLIZZARD | BLASTOISE (staged) |
| 50 | 166 | CRADILY | 0xE1 | BRIGHT_POWDER | ? | ? | STOCKPILE, SPIT_UP, SWALLOW, DOUBLE_TEAM | ARMALDO (staged) |
| 50 | 169 | LUDICOLO | 0x83 | PETAYA_BERRY | ? | ? | RAIN_DANCE, SURF, PROTECT, TOXIC | POLITOED (staged) |
| 50 | 171 | TROPIUS | 0xC3 | APICOT_BERRY | ? | ? | SUNNY_DAY, SYNTHESIS, SOLAR_BEAM, LEECH_SEED | TANGELA (staged) |
| 50 | 172 | GRUMPIG | 0x23 | MACHO_BRACE | ? | ? | TRICK, SKILL_SWAP, PSYCHIC, REST | HYPNO (staged) |
| 50 | 173 | TORKOAL | 0x81 | LUM_BERRY | ? | ? | IRON_DEFENSE, TOXIC, REST, FLAMETHROWER | MAGCARGO (staged) |
| 50 | 181 | REGICE | 0x11 | CHESTO_BERRY | ? | ? | REST, CURSE, BLIZZARD, EARTHQUAKE | ARTICUNO (staged) |
| 50 | 182 | REGISTEEL | 0x11 | LUM_BERRY | ? | ? | REST, IRON_DEFENSE, THUNDERBOLT, EARTHQUAKE | FORRETRESS (staged) |
| 50 | 183 | REGIROCK | 0x91 | QUICK_CLAW | ? | ? | ROCK_TOMB, EARTHQUAKE, THUNDERBOLT, EXPLOSION | GOLEM (staged) |
| 50 | 187 | LANTURN | 0x82 | SALAC_BERRY | ? | ? | THUNDERBOLT, SURF, THUNDER_WAVE, BLIZZARD | AMPHAROS (staged) |
| 50 | 189 | WHISCASH | 0x41 | PETAYA_BERRY | ? | ? | EARTHQUAKE, SURF, ROCK_TOMB, ATTRACT | KINGLER (staged) |
| 50 | 194 | WAILORD | 0x23 | LEPPA_BERRY | ? | ? | HYDRO_PUMP, EARTHQUAKE, ATTRACT, FISSURE | LAPRAS (staged) |
| 50 | 196 | LUDICOLO | 0xA2 | LEFTOVERS | ? | ? | RAIN_DANCE, SURF, TOXIC, DOUBLE_TEAM | POLITOED (staged) |
| 50 | 197 | WOBBUFFET | 0x23 | LEFTOVERS | ? | ? | ENCORE, COUNTER, MIRROR_COAT, DESTINY_BOND | BLISSEY (staged) |
| 50 | 200 | SCEPTILE | 0x13 | SCOPE_LENS | ? | ? | LEAF_BLADE, EARTHQUAKE, DRAGON_CLAW, CRUNCH | VENUSAUR (staged) |
| 50 | 201 | BLAZIKEN | 0x17 | LIECHI_BERRY | ? | ? | BLAZE_KICK, ROCK_TOMB, ENDURE, REVERSAL | CHARIZARD (staged) |
| 50 | 202 | SWAMPERT | 0x17 | SHELL_BELL | ? | ? | SURF, EARTHQUAKE, MUD_SHOT, BLIZZARD | BLASTOISE (staged) |
| 50 | 207 | HARIYAMA | 0x85 | SCOPE_LENS | ? | ? | CROSS_CHOP, EARTHQUAKE, FACADE, FAKE_OUT | MACHAMP (staged) |
| 50 | 221 | SALAMENCE | 0x9C | LUM_BERRY | ? | ? | DRAGON_CLAW, EARTHQUAKE, CRUNCH, REST | DRAGONITE (staged) |
| 50 | 222 | METAGROSS | 0x15 | SHELL_BELL | ? | ? | METEOR_MASH, PSYCHIC, EARTHQUAKE, LIGHT_SCREEN | TYRANITAR (staged) |
| 50 | 223 | REGIROCK | 0xB5 | BRIGHT_POWDER | ? | ? | EARTHQUAKE, ANCIENT_POWER, FOCUS_PUNCH, EXPLOSION | GOLEM (staged) |
| 50 | 224 | REGICE | 0x96 | CHESTO_BERRY | ? | ? | ICE_BEAM, THUNDERBOLT, DOUBLE_TEAM, REST | ARTICUNO (staged) |
| 50 | 225 | REGISTEEL | 0x94 | QUICK_CLAW | ? | ? | METAL_CLAW, EARTHQUAKE, THUNDERBOLT, EXPLOSION | FORRETRESS (staged) |
| 50 | 226 | LATIAS | 0x96 | LUM_BERRY | ? | ? | MIST_BALL, THUNDERBOLT, REST, DRAGON_CLAW | ESPEON (staged) |
| 50 | 227 | LATIOS | 0x95 | APICOT_BERRY | ? | ? | LUSTER_PURGE, REFLECT, RECOVER, DRAGON_CLAW | ALAKAZAM (staged) |
| 50 | 229 | SLAKING | 0x05 | CHOICE_BAND | ? | ? | HYPER_BEAM, EARTHQUAKE, NONE, NONE | URSARING (staged) |
| 50 | 230 | SCEPTILE | 0x1F | LEFTOVERS | ? | ? | GIGA_DRAIN, DOUBLE_TEAM, LEECH_SEED, ATTRACT | VENUSAUR (staged) |
| 50 | 231 | BLAZIKEN | 0x17 | CHESTO_BERRY | ? | ? | BLAZE_KICK, EARTHQUAKE, ROCK_SLIDE, REST | CHARIZARD (staged) |
| 50 | 232 | SWAMPERT | 0x17 | SHELL_BELL | ? | ? | MUDDY_WATER, MUD_SHOT, MIRROR_COAT, ICE_BEAM | BLASTOISE (staged) |
| 50 | 236 | HARIYAMA | 0x05 | FOCUS_BAND | ? | ? | CROSS_CHOP, ROCK_TOMB, ENDURE, REVERSAL | MACHAMP (staged) |
| 50 | 251 | SALAMENCE | 0x9D | KINGS_ROCK | ? | ? | DRAGON_CLAW, EARTHQUAKE, FLAMETHROWER, ROCK_TOMB | DRAGONITE (staged) |
| 50 | 252 | METAGROSS | 0x15 | QUICK_CLAW | ? | ? | METEOR_MASH, PSYCHIC, EARTHQUAKE, HYPER_BEAM | TYRANITAR (staged) |
| 50 | 253 | REGIROCK | 0x95 | QUICK_CLAW | ? | ? | EARTHQUAKE, ROCK_TOMB, FACADE, EXPLOSION | GOLEM (staged) |
| 50 | 254 | REGICE | 0x96 | SHELL_BELL | ? | ? | BLIZZARD, THUNDERBOLT, HAIL, EXPLOSION | ARTICUNO (staged) |
| 50 | 255 | REGISTEEL | 0x94 | QUICK_CLAW | ? | ? | METAL_CLAW, EARTHQUAKE, ANCIENT_POWER, EXPLOSION | FORRETRESS (staged) |
| 50 | 256 | LATIAS | 0x96 | APICOT_BERRY | ? | ? | MIST_BALL, REFLECT, RECOVER, DRAGON_CLAW | ESPEON (staged) |
| 50 | 257 | LATIOS | 0x95 | SHELL_BELL | ? | ? | LUSTER_PURGE, ICE_BEAM, RECOVER, DRAGON_CLAW | ALAKAZAM (staged) |
| 50 | 260 | SCEPTILE | 0x5F | CHESTO_BERRY | ? | ? | LEAF_BLADE, EARTHQUAKE, REST, CRUNCH | VENUSAUR (staged) |
| 50 | 261 | BLAZIKEN | 0x7F | LUM_BERRY | ? | ? | FOCUS_PUNCH, COUNTER, SWAGGER, ROCK_TOMB | CHARIZARD (staged) |
| 50 | 262 | SWAMPERT | 0x57 | LUM_BERRY | ? | ? | IRON_TAIL, CURSE, DOUBLE_TEAM, REST | BLASTOISE (staged) |
| 50 | 266 | HARIYAMA | 0x65 | SALAC_BERRY | ? | ? | REVENGE, COUNTER, FOCUS_PUNCH, REVERSAL | MACHAMP (staged) |
| 50 | 267 | LUDICOLO | 0x54 | LEFTOVERS | ? | ? | RAIN_DANCE, PROTECT, TOXIC, SURF | POLITOED (staged) |
| 50 | 275 | LUDICOLO | 0x1A | LEFTOVERS | ? | ? | RAIN_DANCE, LEECH_SEED, DIVE, PROTECT | POLITOED (staged) |
| 50 | 281 | SALAMENCE | 0x9C | PETAYA_BERRY | ? | ? | DRAGON_CLAW, EARTHQUAKE, FIRE_BLAST, HYDRO_PUMP | DRAGONITE (staged) |
| 50 | 282 | METAGROSS | 0xB5 | LIECHI_BERRY | ? | ? | METEOR_MASH, PSYCHIC, PURSUIT, ROCK_TOMB | TYRANITAR (staged) |
| 50 | 283 | REGIROCK | 0x95 | APICOT_BERRY | ? | ? | EARTHQUAKE, TOXIC, SUPERPOWER, REST | GOLEM (staged) |
| 50 | 284 | REGICE | 0x96 | LAX_INCENSE | ? | ? | ICE_BEAM, THUNDERBOLT, ANCIENT_POWER, EXPLOSION | ARTICUNO (staged) |
| 50 | 285 | REGISTEEL | 0xB4 | LEFTOVERS | ? | ? | CURSE, AMNESIA, RETURN, REST | FORRETRESS (staged) |
| 50 | 286 | LATIAS | 0x92 | SHELL_BELL | ? | ? | MIST_BALL, THUNDERBOLT, EARTHQUAKE, ICE_BEAM | ESPEON (staged) |
| 50 | 287 | LATIOS | 0x91 | SHELL_BELL | ? | ? | LUSTER_PURGE, THUNDERBOLT, EARTHQUAKE, SURF | ALAKAZAM (staged) |
| 50 | 289 | SLAKING | 0x65 | SALAC_BERRY | ? | ? | YAWN, SLASH, COUNTER, HYPER_BEAM | URSARING (staged) |
| 50 | 290 | SCEPTILE | 0xDF | SHELL_BELL | ? | ? | LEAF_BLADE, DETECT, DRAGON_CLAW, CRUNCH | VENUSAUR (staged) |
| 50 | 291 | BLAZIKEN | 0xD7 | SALAC_BERRY | ? | ? | BLAZE_KICK, SKY_UPPERCUT, EARTHQUAKE, ROCK_SLIDE | CHARIZARD (staged) |
| 50 | 292 | SWAMPERT | 0x5F | PETAYA_BERRY | ? | ? | SURF, EARTHQUAKE, ICE_BEAM, ROCK_TOMB | BLASTOISE (staged) |
| 50 | 293 | SHEDINJA | 0xDB | BRIGHT_POWDER | ? | ? | CONFUSE_RAY, SHADOW_BALL, TOXIC, HYPER_BEAM | MISDREAVUS (staged) |
| 50 | 294 | METAGROSS | 0x94 | QUICK_CLAW | ? | ? | METEOR_MASH, PSYCHIC, SHADOW_BALL, EARTHQUAKE | TYRANITAR (staged) |
| 50 | 295 | GRUMPIG | 0x4B | MACHO_BRACE | ? | ? | PSYCHIC, CONFUSE_RAY, TRICK, REFLECT | HYPNO (staged) |
| 50 | 296 | LATIAS | 0x92 | LUM_BERRY | ? | ? | MIST_BALL, RECOVER, ATTRACT, DRAGON_CLAW | ESPEON (staged) |
| 50 | 297 | LATIOS | 0x91 | KINGS_ROCK | ? | ? | LUSTER_PURGE, THUNDERBOLT, ICE_BEAM, DRAGON_CLAW | ALAKAZAM (staged) |
| 100 | 0 | LINOONE | 0x42 | RAWST_BERRY | ? | ? | SLASH, GROWL, TAIL_WHIP, SAND_ATTACK | RATICATE (staged) |
| 100 | 1 | MIGHTYENA | 0x41 | SITRUS_BERRY | ? | ? | BITE, HOWL, ODOR_SLEUTH, SCARY_FACE | HOUNDOOM (staged) |
| 100 | 2 | BEAUTIFLY | 0x41 | PERSIM_BERRY | ? | ? | GIGA_DRAIN, GUST, STUN_SPORE, PROTECT | BUTTERFREE (staged) |
| 100 | 3 | DUSTOX | 0x41 | PECHA_BERRY | ? | ? | PSYBEAM, GUST, DOUBLE_TEAM, SILVER_WIND | VENOMOTH (staged) |
| 100 | 4 | LOMBRE | 0x03 | CHESTO_BERRY | ? | ? | ASTONISH, GROWL, MEGA_DRAIN, SURF | POLIWHIRL (staged) |
| 100 | 7 | PELIPPER | 0x02 | MYSTIC_WATER | ? | ? | SURF, WATER_SPORT, PROTECT, SUPERSONIC | SWELLOW (staged) |
| 100 | 9 | NINJASK | 0x41 | RAWST_BERRY | ? | ? | SCRATCH, SCREECH, LEECH_LIFE, FLASH | YANMA (staged) |
| 100 | 11 | MAWILE | 0x03 | METAL_COAT | ? | ? | VICE_GRIP, SWEET_SCENT, ASTONISH, STRENGTH | SCIZOR (staged) |
| 100 | 12 | NOSEPASS | 0x01 | SITRUS_BERRY | ? | ? | ROCK_THROW, SANDSTORM, HARDEN, PROTECT | SUDOWOODO (staged) |
| 100 | 17 | SPINDA | 0x42 | SILK_SCARF | ? | ? | UPROAR, WATER_PULSE, THRASH, SAFEGUARD | DELCATTY (staged) |
| 100 | 18 | WHISCASH | 0x01 | CHESTO_BERRY | ? | ? | TICKLE, MUD_SPORT, WATER_GUN, WATER_SPORT | KINGLER (staged) |
| 100 | 21 | MARILL | 0x42 | MYSTIC_WATER | ? | ? | BUBBLE_BEAM, DEFENSE_CURL, RAIN_DANCE, IRON_TAIL | POLIWAG (staged) |
| 100 | 30 | LUVDISC | 0x0A | SITRUS_BERRY | ? | ? | ATTRACT, FLAIL, SWEET_KISS, WATER_PULSE | GOLDEEN (staged) |
| 100 | 31 | LANTURN | 0x0A | CHERI_BERRY | ? | ? | THUNDER_WAVE, SUPERSONIC, WATER_GUN, SPARK | AMPHAROS (staged) |
| 100 | 32 | CORSOLA | 0x0A | SITRUS_BERRY | ? | ? | MIRROR_COAT, BUBBLE, HARDEN, LIGHT_SCREEN | STARMIE (staged) |
| 100 | 33 | WAILMER | 0x05 | SITRUS_BERRY | ? | ? | SURF, ROAR, GROWL, MIST | SEALEO (staged) |
| 100 | 38 | TROPIUS | 0x05 | KINGS_ROCK | ? | ? | STOMP, RAZOR_LEAF, GUST, SYNTHESIS | TANGELA (staged) |
| 100 | 39 | SABLEYE | 0x0A | SPELL_TAG | ? | ? | NIGHT_SHADE, FAKE_OUT, PSYCH_UP, DETECT | BANETTE (staged) |
| 100 | 42 | CLAYDOL | 0x05 | SOFT_SAND | ? | ? | ANCIENT_POWER, MUD_SLAP, RAPID_SPIN, CONFUSION | GOLEM (staged) |
| 100 | 43 | GRUMPIG | 0x0A | SITRUS_BERRY | ? | ? | PSYBEAM, PSYCH_UP, BOUNCE, MAGIC_COAT | HYPNO (staged) |
| 100 | 45 | SEVIPER | 0x01 | POISON_BARB | ? | ? | POISON_TAIL, GLARE, WRAP, THIEF | ARBOK (staged) |
| 100 | 46 | ZANGOOSE | 0x05 | SITRUS_BERRY | ? | ? | FURY_CUTTER, LEER, QUICK_ATTACK, DETECT | PERSIAN (staged) |
| 100 | 47 | ALTARIA | 0x03 | DRAGON_FANG | ? | ? | DRAGON_BREATH, SING, SAFEGUARD, TAKE_DOWN | DRAGONITE (staged) |
| 100 | 48 | ROSELIA | 0x0A | CHESTO_BERRY | ? | ? | LEECH_SEED, MEGA_DRAIN, GROWTH, STUN_SPORE | VILEPLUME (staged) |
| 100 | 49 | VOLBEAT | 0x09 | BRIGHT_POWDER | ? | ? | SIGNAL_BEAM, CONFUSE_RAY, DOUBLE_TEAM, LIGHT_SCREEN | YANMA (staged) |
| 100 | 50 | ILLUMISE | 0x09 | SITRUS_BERRY | ? | ? | CHARM, ENCORE, SOLAR_BEAM, ATTRACT | BUTTERFREE (staged) |
| 100 | 52 | MAWILE | 0x0A | SITRUS_BERRY | ? | ? | CRUNCH, IRON_DEFENSE, TORMENT, SANDSTORM | SCIZOR (staged) |
| 100 | 53 | HARIYAMA | 0x05 | KINGS_ROCK | ? | ? | ARM_THRUST, FAKE_OUT, WHIRLWIND, REVERSAL | MACHAMP (staged) |
| 100 | 54 | SKARMORY | 0x01 | METAL_COAT | ? | ? | STEEL_WING, AGILITY, TAUNT, FLY | STEELIX (staged) |
| 100 | 55 | TORKOAL | 0x03 | PECHA_BERRY | ? | ? | FIRE_SPIN, SMOKESCREEN, BODY_SLAM, AMNESIA | MAGCARGO (staged) |
| 100 | 57 | MIGHTYENA | 0x05 | SITRUS_BERRY | ? | ? | CRUNCH, HOWL, IRON_TAIL, TORMENT | HOUNDOOM (staged) |
| 100 | 58 | LINOONE | 0x06 | CHERI_BERRY | ? | ? | SLASH, ATTRACT, SHOCK_WAVE, TOXIC | RATICATE (staged) |
| 100 | 59 | MASQUERAIN | 0x0A | SITRUS_BERRY | ? | ? | TOXIC, STUN_SPORE, SUNNY_DAY, SOLAR_BEAM | VENOMOTH (staged) |
| 100 | 60 | SCEPTILE | 0x06 | LUM_BERRY | ? | ? | BULLET_SEED, SLAM, SCREECH, DETECT | VENUSAUR (staged) |
| 100 | 61 | BLAZIKEN | 0x06 | KINGS_ROCK | ? | ? | DOUBLE_KICK, FLAMETHROWER, DOUBLE_TEAM, ROAR | CHARIZARD (staged) |
| 100 | 62 | SWAMPERT | 0x49 | SITRUS_BERRY | ? | ? | SURF, MUD_SHOT, MUD_SPORT, RAIN_DANCE | BLASTOISE (staged) |
| 100 | 63 | SALAMENCE | 0x49 | DRAGON_FANG | ? | ? | DRAGON_BREATH, FLY, TOXIC, FACADE | DRAGONITE (staged) |
| 100 | 65 | CORSOLA | 0x03 | PERSIM_BERRY | ? | ? | ANCIENT_POWER, MIRROR_COAT, SAFEGUARD, REFLECT | STARMIE (staged) |
| 100 | 68 | GIRAFARIG | 0x09 | LUM_BERRY | ? | ? | PSYCHIC, CRUNCH, BATON_PASS, AGILITY | HYPNO (staged) |
| 100 | 81 | LUNATONE | 0x43 | BRIGHT_POWDER | ? | ? | PSYCHIC, ROCK_THROW, HYPNOSIS, LIGHT_SCREEN | XATU (staged) |
| 100 | 82 | SOLROCK | 0x43 | PECHA_BERRY | ? | ? | SOLAR_BEAM, SUNNY_DAY, COSMIC_POWER, CALM_MIND | MAGCARGO (staged) |
| 100 | 83 | BALTOY | 0x43 | SITRUS_BERRY | ? | ? | PSYCHIC, EARTHQUAKE, REFLECT, SELF_DESTRUCT | GEODUDE (staged) |
| 100 | 85 | WHISCASH | 0x06 | ASPEAR_BERRY | ? | ? | EARTHQUAKE, TICKLE, AMNESIA, SURF | KINGLER (staged) |
| 100 | 86 | SEVIPER | 0x09 | SITRUS_BERRY | ? | ? | POISON_TAIL, CRUNCH, GIGA_DRAIN, HAZE | ARBOK (staged) |
| 100 | 89 | SWALOT | 0x86 | SITRUS_BERRY | ? | ? | YAWN, WATER_PULSE, SHADOW_BALL, SLUDGE_BOMB | MUK (staged) |
| 100 | 90 | SCEPTILE | 0x0C | SITRUS_BERRY | ? | ? | LEAF_BLADE, GIGA_DRAIN, FURY_CUTTER, DETECT | VENUSAUR (staged) |
| 100 | 91 | BLAZIKEN | 0x0C | SCOPE_LENS | ? | ? | BLAZE_KICK, FLAMETHROWER, PROTECT, QUICK_ATTACK | CHARIZARD (staged) |
| 100 | 92 | SWAMPERT | 0x4C | BRIGHT_POWDER | ? | ? | SURF, MUD_SHOT, MUD_SLAP, DOUBLE_TEAM | BLASTOISE (staged) |
| 100 | 93 | MIGHTYENA | 0x04 | BLACK_GLASSES | ? | ? | CRUNCH, HOWL, SWAGGER, SHADOW_BALL | HOUNDOOM (staged) |
| 100 | 94 | LINOONE | 0x04 | CHESTO_BERRY | ? | ? | SLASH, REST, BELLY_DRUM, THUNDERBOLT | RATICATE (staged) |
| 100 | 95 | BEAUTIFLY | 0x08 | LUM_BERRY | ? | ? | GIGA_DRAIN, ATTRACT, MORNING_SUN, STUN_SPORE | BUTTERFREE (staged) |
| 100 | 96 | DUSTOX | 0x08 | SITRUS_BERRY | ? | ? | PSYCHIC, SILVER_WIND, MOONLIGHT, TOXIC | VENOMOTH (staged) |
| 100 | 97 | LUDICOLO | 0x0C | LUM_BERRY | ? | ? | FAKE_OUT, HYDRO_PUMP, ICE_BEAM, UPROAR | POLITOED (staged) |
| 100 | 102 | VIGOROTH | 0x04 | QUICK_CLAW | ? | ? | SLASH, UPROAR, ENCORE, FACADE | PRIMEAPE (staged) |
| 100 | 103 | NINJASK | 0x04 | SILVER_POWDER | ? | ? | FURY_CUTTER, GIGA_DRAIN, SWORDS_DANCE, DOUBLE_TEAM | YANMA (staged) |
| 100 | 104 | SHEDINJA | 0xCA | QUICK_CLAW | ? | ? | CONFUSE_RAY, SOLAR_BEAM, SUNNY_DAY, GRUDGE | MISDREAVUS (staged) |
| 100 | 105 | LOUDRED | 0x04 | PECHA_BERRY | ? | ? | HYPER_VOICE, HOWL, SHADOW_BALL, TORMENT | LICKITUNG (staged) |
| 100 | 106 | HARIYAMA | 0x04 | KINGS_ROCK | ? | ? | ARM_THRUST, BULK_UP, BRICK_BREAK, HIDDEN_POWER | MACHAMP (staged) |
| 100 | 109 | MAWILE | 0x04 | METAL_COAT | ? | ? | CRUNCH, IRON_DEFENSE, FAKE_TEARS, POISON_FANG | SCIZOR (staged) |
| 100 | 113 | VOLBEAT | 0x08 | SILVER_POWDER | ? | ? | CONFUSE_RAY, SIGNAL_BEAM, MOONLIGHT, TAIL_GLOW | YANMA (staged) |
| 100 | 114 | ILLUMISE | 0x08 | SILVER_POWDER | ? | ? | THUNDERBOLT, FLATTER, WISH, ENCORE | BUTTERFREE (staged) |
| 100 | 115 | MASQUERAIN | 0x88 | PERSIM_BERRY | ? | ? | GIGA_DRAIN, SILVER_WIND, STUN_SPORE, TOXIC | VENOMOTH (staged) |
| 100 | 116 | ROSELIA | 0x08 | MIRACLE_SEED | ? | ? | PETAL_DANCE, GROWTH, SYNTHESIS, GRASS_WHISTLE | VILEPLUME (staged) |
| 100 | 120 | SWALOT | 0x42 | LEFTOVERS | ? | ? | STOCKPILE, SWALLOW, SPIT_UP, YAWN | MUK (staged) |
| 100 | 123 | SPINDA | 0x42 | SILK_SCARF | ? | ? | DIZZY_PUNCH, TEETER_DANCE, PSYCH_UP, FACADE | DELCATTY (staged) |
| 100 | 124 | ALTARIA | 0x03 | DRAGON_FANG | ? | ? | DRAGON_CLAW, DRAGON_BREATH, DRAGON_DANCE, REFRESH | DRAGONITE (staged) |
| 100 | 125 | ZANGOOSE | 0x03 | SITRUS_BERRY | ? | ? | CRUSH_CLAW, TAUNT, SWORDS_DANCE, DETECT | PERSIAN (staged) |
| 100 | 126 | SEVIPER | 0x41 | LUM_BERRY | ? | ? | SWAGGER, TAUNT, GLARE, POISON_TAIL | ARBOK (staged) |
| 100 | 127 | GRUMPIG | 0x42 | SITRUS_BERRY | ? | ? | TOXIC, BOUNCE, CONFUSE_RAY, ATTRACT | HYPNO (staged) |
| 100 | 129 | CLAYDOL | 0x03 | CHESTO_BERRY | ? | ? | EARTHQUAKE, SANDSTORM, COSMIC_POWER, EXPLOSION | GOLEM (staged) |
| 100 | 132 | AZUMARILL | 0xC2 | CHERI_BERRY | ? | ? | SURF, ATTRACT, DEFENSE_CURL, ROLLOUT | POLIWRATH (staged) |
| 100 | 144 | RELICANTH | 0x41 | LEFTOVERS | ? | ? | YAWN, MUD_SPORT, DOUBLE_EDGE, SANDSTORM | OMASTAR (staged) |
| 100 | 146 | CRADILY | 0x43 | LEFTOVERS | ? | ? | CONFUSE_RAY, INGRAIN, ANCIENT_POWER, GIGA_DRAIN | ARMALDO (staged) |
| 100 | 148 | METAGROSS | 0x41 | METAL_COAT | ? | ? | METAL_CLAW, PSYCHIC, LIGHT_SCREEN, AGILITY | TYRANITAR (staged) |
| 100 | 149 | SALAMENCE | 0x42 | LUM_BERRY | ? | ? | DRAGON_BREATH, CRUNCH, FLAMETHROWER, AERIAL_ACE | DRAGONITE (staged) |
| 100 | 150 | MIGHTYENA | 0x01 | LIECHI_BERRY | ? | ? | CRUNCH, YAWN, FACADE, HOWL | HOUNDOOM (staged) |
| 100 | 151 | LINOONE | 0x02 | SALAC_BERRY | ? | ? | HYPER_BEAM, FACADE, ATTRACT, TRICK | RATICATE (staged) |
| 100 | 152 | LUDICOLO | 0x03 | LUM_BERRY | ? | ? | SURF, FOCUS_PUNCH, FAKE_OUT, SYNTHESIS | POLITOED (staged) |
| 100 | 153 | EXPLOUD | 0x01 | LIECHI_BERRY | ? | ? | UPROAR, EARTHQUAKE, ICE_BEAM, FLAMETHROWER | SNORLAX (staged) |
| 100 | 155 | PELIPPER | 0x82 | QUICK_CLAW | ? | ? | STOCKPILE, SWALLOW, SPIT_UP, ATTRACT | SWELLOW (staged) |
| 100 | 157 | SHEDINJA | 0x01 | LUM_BERRY | ? | ? | RETURN, DOUBLE_TEAM, TOXIC, SHADOW_BALL | MISDREAVUS (staged) |
| 100 | 158 | EXPLOUD | 0x01 | SITRUS_BERRY | ? | ? | HYPER_BEAM, SUPERSONIC, FLAMETHROWER, ICE_BEAM | SNORLAX (staged) |
| 100 | 159 | HARIYAMA | 0x01 | SCOPE_LENS | ? | ? | CROSS_CHOP, COUNTER, FACADE, FAKE_OUT | MACHAMP (staged) |
| 100 | 162 | SABLEYE | 0x83 | LEFTOVERS | ? | ? | TOXIC, DETECT, RECOVER, TORMENT | BANETTE (staged) |
| 100 | 163 | MAWILE | 0x02 | CHESTO_BERRY | ? | ? | CRUNCH, SWORDS_DANCE, IRON_DEFENSE, REST | SCIZOR (staged) |
| 100 | 166 | NOSEPASS | 0x81 | SITRUS_BERRY | ? | ? | THUNDER_WAVE, ROCK_SLIDE, TORMENT, ATTRACT | SUDOWOODO (staged) |
| 100 | 168 | ROSELIA | 0x82 | LEFTOVERS | ? | ? | GRASS_WHISTLE, LEECH_SEED, GIGA_DRAIN, GROWTH | VILEPLUME (staged) |
| 100 | 170 | TROPIUS | 0xC2 | SCOPE_LENS | ? | ? | RAZOR_LEAF, EARTHQUAKE, ATTRACT, SYNTHESIS | TANGELA (staged) |
| 100 | 171 | SWALOT | 0x03 | LIECHI_BERRY | ? | ? | YAWN, SLUDGE_BOMB, ATTRACT, ICE_BEAM | MUK (staged) |
| 100 | 176 | SPINDA | 0x82 | CHOICE_BAND | ? | ? | TRICK, TEETER_DANCE, FOCUS_PUNCH, FACADE | DELCATTY (staged) |
| 100 | 177 | SKARMORY | 0x01 | SALAC_BERRY | ? | ? | DRILL_PECK, STEEL_WING, TORMENT, PROTECT | STEELIX (staged) |
| 100 | 178 | ALTARIA | 0x02 | QUICK_CLAW | ? | ? | SING, DRAGON_CLAW, ICE_BEAM, EARTHQUAKE | DRAGONITE (staged) |
| 100 | 179 | GRUMPIG | 0x82 | LUM_BERRY | ? | ? | PSYCHIC, CONFUSE_RAY, CALM_MIND, REST | HYPNO (staged) |
| 100 | 181 | CLAYDOL | 0x20 | LIECHI_BERRY | ? | ? | COSMIC_POWER, PSYCHIC, EARTHQUAKE, EXPLOSION | GOLEM (staged) |
| 100 | 182 | HARIYAMA | 0x33 | CHESTO_BERRY | ? | ? | EARTHQUAKE, COUNTER, BELLY_DRUM, REST | MACHAMP (staged) |
| 100 | 183 | NOSEPASS | 0x20 | LIECHI_BERRY | ? | ? | ROCK_SLIDE, THUNDER_WAVE, EARTHQUAKE, EXPLOSION | SUDOWOODO (staged) |
| 100 | 187 | LANTURN | 0x13 | LUM_BERRY | ? | ? | THUNDERBOLT, SURF, THUNDER_WAVE, CONFUSE_RAY | AMPHAROS (staged) |
| 100 | 189 | WHISCASH | 0x20 | LEPPA_BERRY | ? | ? | EARTHQUAKE, SURF, SPARK, FISSURE | KINGLER (staged) |
| 100 | 194 | WAILORD | 0x72 | LEPPA_BERRY | ? | ? | HYDRO_PUMP, EARTHQUAKE, ATTRACT, FISSURE | LAPRAS (staged) |
| 100 | 196 | SABLEYE | 0x32 | LEFTOVERS | ? | ? | SHADOW_BALL, TOXIC, RECOVER, DOUBLE_TEAM | BANETTE (staged) |
| 100 | 197 | WOBBUFFET | 0x20 | LEFTOVERS | ? | ? | ENCORE, COUNTER, MIRROR_COAT, DESTINY_BOND | BLISSEY (staged) |
| 100 | 200 | SCEPTILE | 0x8C | SCOPE_LENS | ? | ? | LEAF_BLADE, EARTHQUAKE, DRAGON_CLAW, CRUNCH | VENUSAUR (staged) |
| 100 | 201 | BLAZIKEN | 0x8C | SALAC_BERRY | ? | ? | BLAZE_KICK, COUNTER, ENDURE, REVERSAL | CHARIZARD (staged) |
| 100 | 202 | SWAMPERT | 0x8C | SHELL_BELL | ? | ? | SURF, EARTHQUAKE, MIRROR_COAT, BLIZZARD | BLASTOISE (staged) |
| 100 | 206 | HARIYAMA | 0x15 | SCOPE_LENS | ? | ? | CROSS_CHOP, EARTHQUAKE, FACADE, FAKE_OUT | MACHAMP (staged) |
| 100 | 221 | SALAMENCE | 0x0D | LUM_BERRY | ? | ? | DRAGON_CLAW, EARTHQUAKE, CRUNCH, FIRE_BLAST | DRAGONITE (staged) |
| 100 | 222 | METAGROSS | 0x3D | SALAC_BERRY | ? | ? | METEOR_MASH, PSYCHIC, EARTHQUAKE, ROCK_TOMB | TYRANITAR (staged) |
| 100 | 223 | REGIROCK | 0x3C | BRIGHT_POWDER | ? | ? | EARTHQUAKE, ANCIENT_POWER, THUNDERBOLT, EXPLOSION | GOLEM (staged) |
| 100 | 224 | REGICE | 0x3C | LAX_INCENSE | ? | ? | ICE_BEAM, THUNDERBOLT, EARTHQUAKE, EXPLOSION | ARTICUNO (staged) |
| 100 | 225 | REGISTEEL | 0x3C | QUICK_CLAW | ? | ? | METAL_CLAW, EARTHQUAKE, THUNDERBOLT, EXPLOSION | FORRETRESS (staged) |
| 100 | 226 | LATIAS | 0x0E | LUM_BERRY | ? | ? | MIST_BALL, THUNDERBOLT, RECOVER, DRAGON_CLAW | ESPEON (staged) |
| 100 | 227 | LATIOS | 0x0D | PETAYA_BERRY | ? | ? | LUSTER_PURGE, THUNDERBOLT, RECOVER, DRAGON_CLAW | ALAKAZAM (staged) |
| 100 | 229 | SLAKING | 0x15 | LUM_BERRY | ? | ? | YAWN, FOCUS_PUNCH, COUNTER, SLACK_OFF | URSARING (staged) |
| 100 | 230 | SCEPTILE | 0x8C | LEFTOVERS | ? | ? | GIGA_DRAIN, DOUBLE_TEAM, LEECH_SEED, ATTRACT | VENUSAUR (staged) |
| 100 | 231 | BLAZIKEN | 0x8C | SCOPE_LENS | ? | ? | BLAZE_KICK, EARTHQUAKE, ROCK_SLIDE, QUICK_ATTACK | CHARIZARD (staged) |
| 100 | 232 | SWAMPERT | 0x8C | LEFTOVERS | ? | ? | MUDDY_WATER, MUD_SHOT, MIRROR_COAT, ICE_BEAM | BLASTOISE (staged) |
| 100 | 236 | HARIYAMA | 0x04 | FOCUS_BAND | ? | ? | CROSS_CHOP, COUNTER, ENDURE, REVERSAL | MACHAMP (staged) |
| 100 | 251 | SALAMENCE | 0x1D | KINGS_ROCK | ? | ? | DRAGON_CLAW, EARTHQUAKE, CRUNCH, ROCK_TOMB | DRAGONITE (staged) |
| 100 | 252 | METAGROSS | 0x3D | QUICK_CLAW | ? | ? | METEOR_MASH, PSYCHIC, EARTHQUAKE, SHADOW_BALL | TYRANITAR (staged) |
| 100 | 253 | REGIROCK | 0x3C | QUICK_CLAW | ? | ? | EARTHQUAKE, THUNDERBOLT, FOCUS_PUNCH, EXPLOSION | GOLEM (staged) |
| 100 | 254 | REGICE | 0x3C | BRIGHT_POWDER | ? | ? | ICE_BEAM, THUNDERBOLT, HAIL, EXPLOSION | ARTICUNO (staged) |
| 100 | 255 | REGISTEEL | 0x3C | SALAC_BERRY | ? | ? | METAL_CLAW, EARTHQUAKE, ANCIENT_POWER, EXPLOSION | FORRETRESS (staged) |
| 100 | 256 | LATIAS | 0x1E | BRIGHT_POWDER | ? | ? | MIST_BALL, ICE_BEAM, RECOVER, DRAGON_CLAW | ESPEON (staged) |
| 100 | 257 | LATIOS | 0x1D | LAX_INCENSE | ? | ? | LUSTER_PURGE, ICE_BEAM, RECOVER, DRAGON_CLAW | ALAKAZAM (staged) |
| 100 | 259 | SLAKING | 0x15 | LUM_BERRY | ? | ? | YAWN, FOCUS_PUNCH, COUNTER, PURSUIT | URSARING (staged) |
| 100 | 260 | SCEPTILE | 0x8C | PETAYA_BERRY | ? | ? | LEAF_BLADE, ROCK_TOMB, DRAGON_CLAW, CRUNCH | VENUSAUR (staged) |
| 100 | 261 | BLAZIKEN | 0x8C | GANLON_BERRY | ? | ? | FOCUS_PUNCH, COUNTER, SWAGGER, ROCK_TOMB | CHARIZARD (staged) |
| 100 | 262 | SWAMPERT | 0x8C | SHELL_BELL | ? | ? | SURF, ICE_BEAM, ATTRACT, REST | BLASTOISE (staged) |
| 100 | 266 | HARIYAMA | 0x05 | FOCUS_BAND | ? | ? | REVENGE, COUNTER, FOCUS_PUNCH, REVERSAL | MACHAMP (staged) |
| 100 | 267 | LUDICOLO | 0x0A | LEFTOVERS | ? | ? | RAIN_DANCE, LEECH_SEED, GIGA_DRAIN, DIVE | POLITOED (staged) |
| 100 | 275 | LUDICOLO | 0x4A | LEFTOVERS | ? | ? | LEECH_SEED, ATTRACT, DOUBLE_TEAM, PROTECT | POLITOED (staged) |
| 100 | 276 | SKARMORY | 0x4B | LEFTOVERS | ? | ? | DRILL_PECK, STEEL_WING, ATTRACT, PURSUIT | STEELIX (staged) |
| 100 | 281 | SALAMENCE | 0x1D | PETAYA_BERRY | ? | ? | DRAGON_CLAW, EARTHQUAKE, FIRE_BLAST, HYDRO_PUMP | DRAGONITE (staged) |
| 100 | 282 | METAGROSS | 0x3D | LIECHI_BERRY | ? | ? | METEOR_MASH, PSYCHIC, EARTHQUAKE, HYPER_BEAM | TYRANITAR (staged) |
| 100 | 283 | REGIROCK | 0x3C | SALAC_BERRY | ? | ? | EARTHQUAKE, THUNDERBOLT, SUPERPOWER, EXPLOSION | GOLEM (staged) |
| 100 | 284 | REGICE | 0x3C | LAX_INCENSE | ? | ? | ICE_BEAM, THUNDERBOLT, ANCIENT_POWER, EXPLOSION | ARTICUNO (staged) |
| 100 | 285 | REGISTEEL | 0x3C | LEFTOVERS | ? | ? | METAL_CLAW, THUNDERBOLT, HYPER_BEAM, EXPLOSION | FORRETRESS (staged) |
| 100 | 286 | LATIAS | 0x1E | APICOT_BERRY | ? | ? | MIST_BALL, THUNDERBOLT, EARTHQUAKE, ICE_BEAM | ESPEON (staged) |
| 100 | 287 | LATIOS | 0x1D | GANLON_BERRY | ? | ? | LUSTER_PURGE, THUNDERBOLT, EARTHQUAKE, ICE_BEAM | ALAKAZAM (staged) |
| 100 | 289 | ALTARIA | 0x0A | GANLON_BERRY | ? | ? | DRAGON_CLAW, ICE_BEAM, SING, FLAMETHROWER | DRAGONITE (staged) |
| 100 | 290 | SCEPTILE | 0x9C | BRIGHT_POWDER | ? | ? | LEAF_BLADE, DETECT, DRAGON_CLAW, CRUNCH | VENUSAUR (staged) |
| 100 | 291 | BLAZIKEN | 0x9C | SALAC_BERRY | ? | ? | BLAZE_KICK, SKY_UPPERCUT, EARTHQUAKE, ROCK_SLIDE | CHARIZARD (staged) |
| 100 | 292 | SWAMPERT | 0x9C | PETAYA_BERRY | ? | ? | SURF, EARTHQUAKE, ICE_BEAM, ROCK_TOMB | BLASTOISE (staged) |
| 100 | 293 | SHEDINJA | 0x20 | BRIGHT_POWDER | ? | ? | CONFUSE_RAY, SHADOW_BALL, TOXIC, GRUDGE | MISDREAVUS (staged) |
| 100 | 294 | METAGROSS | 0x1D | LEFTOVERS | ? | ? | METEOR_MASH, EARTHQUAKE, DOUBLE_TEAM, PROTECT | TYRANITAR (staged) |
| 100 | 296 | LATIAS | 0x1E | LEFTOVERS | ? | ? | MIST_BALL, THUNDERBOLT, ICE_BEAM, DRAGON_CLAW | ESPEON (staged) |
| 100 | 297 | LATIOS | 0x1D | SHELL_BELL | ? | ? | LUSTER_PURGE, THUNDERBOLT, ICE_BEAM, DRAGON_CLAW | ALAKAZAM (staged) |
| 100 | 298 | SALAMENCE | 0x1D | SHELL_BELL | ? | ? | DRAGON_CLAW, FLAMETHROWER, EARTHQUAKE, AERIAL_ACE | DRAGONITE (staged) |

279 entries inventoried and staged under owner approval; battle-role/selection viability remains unverified.


### 2026-11-21 — Battle Tower tactical source refinement and trainer configuration blocker

Focused source-only review revised 12 weak level-50 move selections in `src/data/battle_tower/level_50_mons.h` using actual replacement-species level-up/TM/HM availability. Examples: Magneton template index 109 was four generic support moves (Protect/Toxic/Double Team/Facade), now Thunderbolt/Tri Attack/Metal Sound/Thunder Wave; two Wobbuffet→Blissey templates gained Soft-Boiled rather than Brick Break to improve defensive endurance; Yanma, Swellow, Ampharos, Ursaring and Tyranitar received more purposeful source-legal moves. Full per-index corrections and rationales recorded in `docs/PKM006_BATTLE_TOWER_279_MOVE_REPLACEMENTS_20261121.md`. These do **not** demonstrate gameplay parity. Keep original 285 flagged heuristic swaps as conservative acceptance-review scope.

**Structural blocker discovered in `src/battle_tower.c`:** `static const struct BattleTowerTrainer sBattleTowerTrainers[] = { }; ` is empty, but `FillBattleTowerTrainerParty` indexes `sBattleTowerTrainers[battleTowerTrainerId].teamFlags` for normal opponents. Its selection loop requires three distinct species and nonduplicated held items from a tier-specific 60-slot window (or 100-slot final window) matching that trainer's flag mask. Source-only template inspection found four distinct moves in every 300-entry pool template, but the missing trainer definitions prevent a grounded per-trainer flag/window viability conclusion. No trainer definitions were invented, no battle generation or map access was altered. This blocker predates the 279 substitutions and requires owner decision/approved trainer roster restoration or explicit retirement of the feature.

No tests, builds, emulators, diagnostics, CI or merges. 148 WRL-protected wild slots and 3 SOW/story legendary event scripts unchanged. No acceptance points; 50 IDs and 461/251/712 retained.


### 2026-11-21 — Authorized original-source investigation: Battle Tower trainer roster

Compared the exact `src/battle_tower.c` declaration with upstream `pret/pokefirered` default source via GitHub file retrieval. Both contain the same empty `static const struct BattleTowerTrainer sBattleTowerTrainers[] = { }; ` declaration. Upstream source: https://github.com/pret/pokefirered/blob/master/src/battle_tower.c . Thus the missing normal-trainer roster **predates** Lightning Yellow's 279 owner-approved species substitutions. The FireRed `include/battle_tower.h` structure requires `trainerClass`, three-byte `name`, `teamFlags` and six `greeting` words per record; the normal opponent party builder indexes the roster for `teamFlags`. An empty array does not provide 100 legitimate trainer definitions or their intended flag/difficulty distribution.

Also inspected the related `Deokishisu/FRLG-Plus`, `CharlesAverill/pokefirered`, and `pret/pokeruby` source paths. No exact, authenticated FireRed-compatible roster mapping was established. **No trainer records were created or imported.** This is a provenance/ownership blocker, not evidence that the 279 replacement templates failed gameplay. Owner must provide or authorize a concrete original roster dataset (including identity, class, greeting and team flags) or approve separate authored design. The already-staged 279 template substitutions, 12 tactical move refinements, WRL-protected 148 wild slots and three SOW/story legendary events remain unchanged. No tests/builds/emulators/diagnostics/CI/merges. No earned-point changes; 50 IDs, 461/251/712 retained.


### 2026-11-21 — Newly authored 100-trainer Battle Tower roster staged

Owner explicitly authorized creating a new normal Battle Tower roster, not pretending it was restored from FireRed. `src/battle_tower.c` now defines 100 records at normal trainer IDs 0–99, each with an existing `FACILITY_CLASS_*` class constant, a newly authored three-character name, existing Easy Chat greeting words, and one compatible `teamFlags` mask (0x01, 0x02 or 0x40). New explicit includes: `constants/trainers.h` and `constants/easy_chat.h`. Original normal-opponent tier windows and IV progression unchanged; IDs 100–199 remain record-mixing reserved and ID 200 remains e-Reader reserved. Source-level tier progression: IDs 0–19 IV6 / slots 0–59; 20–29 IV9 / slots 30–89; 30–39 IV12 / slots 60–119; 40–49 IV15 / slots 90–149; 50–59 IV18 / slots 120–179; 60–69 IV21 / slots 150–209; 70–79 IV31 / slots 180–239; 80–99 IV31 / slots 200–299. 

Source-only combinatorial inspection of the 100 authored flag masks against both staged 300-entry Battle Tower template arrays found **200 of 200 trainer/pool combinations** with at least one possible three-Pokémon selection satisfying the engine's species-uniqueness, held-item uniqueness and teamFlags requirements. Smallest eligible template count in any window: 7; smallest distinct-species count: 7. This is a structural review of source values, **not** a runtime diagnostic, compilation, statistical balance proof, or guarantee that randomized selection always terminates promptly. Trainer class, name and greeting records are newly authored. Normal opponent map gfx selection in `SetBattleTowerTrainerGfxId` still uses a fixed Youngster sprite in the upstream source, and facility accessibility remains unconfirmed. No additional gameplay code changed; all 279 Battle Tower species substitutions and 12 earlier tactical move refinements preserved.

No tests/builds/emulators/diagnostics/CI/merges. Protected WRL 148 wild and three SOW/story legendary assignments untouched. No earned acceptance points; 50 IDs and 461/251/712 retained.
