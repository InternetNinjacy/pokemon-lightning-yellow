# Feature: Lance initial Elite Four, Flying-type Master of the Skies

**Project:** Pokémon Lightning Yellow only. Not any other ROM-hack project.  
**Status:** Owner-approved design; documentation-only specification awaiting review and merge; NOT gameplay implemented or tested.  
**Approval:** November 5, 2026, dedicated Elite Four & Postgame Battle Design thread (final two Full Restores / AI approval after roster, moves, items, abilities).  
**Handoff revision:** 2026-11-05.  
**Dependencies:** Approved 252-species Pokédex, move/TM register, per-move Physical/Special split, other separately approved initial Elite Four battles.

## 1. Goal and player-facing behavior

Lance remains the original **fourth Elite Four member** in FireRed with original name, sprite, trainer class, encounter music, room, League event trigger and post-battle progression. He is **Flying-type specialist**: EVERY Pokémon must be a Flying type, with six Pokémon instead of original five, and is the ONLY Elite Four member allowed to use the three legendary birds and Dragonite. Three birds battle before his final Dragonite ace. Single Battle, intended VERY HARD. Do not add scripted forced send-out order or new AI.

## 2. Exact owner-approved initial battle

Trainer: `TRAINER_ELITE_FOUR_LANCE`. Party: `sParty_EliteFourLance[]`. Five source-native species abilities are not required to be distinct; original owner explicitly prioritizes natural abilities over avoiding duplicates. All six held items distinct within this roster.

| Slot | Pokémon | Level | Move 1 | Move 2 | Move 3 | Move 4 | Native ability | Held item |
|---|---|---:|---|---|---|---|---|---|
| 1 | CROBAT | 58 | CONFUSE_RAY | POISON_FANG | AERIAL_ACE | SHADOW_BALL | INNER_FOCUS | POISON_BARB |
| 2 | AERODACTYL | 59 | ANCIENT_POWER | AERIAL_ACE | EARTHQUAKE | IRON_TAIL | ROCK_HEAD | HARD_STONE |
| 3 | ARTICUNO | 60 | ICE_BEAM | BLIZZARD | REFLECT | WATER_PULSE | PRESSURE | NEVER_MELT_ICE |
| 4 | ZAPDOS | 60 | THUNDERBOLT | THUNDER | DRILL_PECK | THUNDER_WAVE | PRESSURE | MAGNET |
| 5 | MOLTRES | 61 | FLAMETHROWER | FIRE_BLAST | OVERHEAT | AERIAL_ACE | PRESSURE | CHARCOAL |
| 6 | DRAGONITE (ACE) | 62 | OUTRAGE | EARTHQUAKE | THUNDERBOLT | ICE_BEAM | INNER_FOCUS | DRAGON_FANG |

**Trainer items:** exactly two `ITEM_FULL_RESTORE`, original FireRed trainer item behavior; no added scripted item decisions.  
**Battle format:** `.doubleBattle = FALSE`.  
**AI:** exactly `AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY`. No custom AI.  
**IVs:** preserve original initial encounter party `.iv = 250` (not rematch 255).  
**Legendary status:** legendary birds owned by Lance only among Elite Four members, not a global availability change. Do not remove, duplicate or relocate any overworld legendary encounters.

## 3. Source authority and provenance

- [Current living Elite Four and Postgame Battle Design Register](https://docs.google.com/document/d/1qEdSSW26GkY3NmF-Na3_bIAMIgYFuVr4tGTKruBKUMI/edit), initial Lance owner approvals Nov 5 2026.
- [Master Plan](https://docs.google.com/document/d/1P4TuXViTpB0BycldB1SzRZTRX8sV_Rd0lFuPwyGDJQk/edit), [START HERE](https://docs.google.com/document/d/14bV_mRB-IgcyWqGcIH2tgmvNoYLY1Ip_zSqSbdHGYtw/edit), [Change Intake](https://docs.google.com/document/d/1NpCjx3uboBMdBVqoFpMwXEyVkDquD37Kcp-FJBbYWnc/edit).
- [Approved 252-species Pokédex](https://docs.google.com/document/d/1Bv5Ssu_D6AxEDvr6xM3YECPRRAxO9yO3HVSNIos1gQc/edit), [TM and Move Register](https://docs.google.com/document/d/1EHByhspPMrn_H0ard-wbdZLRqInZVschQqXTWSiP5fY/edit).
- Owner accepted Crobat first, Aerodactyl second, three legendary birds before Dragonite last; exact levels, exact 24 moves, exact abilities/held items, and finally original two Full Restores/AI. Early suggestion of applying Static to Zapdos, Flame Body to Moltres or Shed Skin to Dragonite was explicitly **rejected and superseded**, because those are unnatural to these species in original FireRed and user wants no special ability programming.

## 4. Implementation boundaries

**In scope:** modify only initial Lance party `sParty_EliteFourLance[]` data to six exact entries and ensure the approved naturally selected Aerodactyl ability is ROCK_HEAD with no global species alteration. Preserve existing Lance trainer metadata and related encounter scripts. Verify six-Pokémon initial Trainer battle supported by existing game.

**Out of scope:** Lance postgame party `sParty_EliteFourLance2[]` (separate approved spec, `implementation/lance-postgame-rematch.md` in PR #46), any other initial/rematch Elite Four member, Champion or rivals, Gym Trainers, species-wide abilities/stats/typing, overworld legendary encounters, new AI, forced Pokémon sending, engine feature expansion not explicitly approved.

**Existing behavior to preserve:** original Elite Four story/room/name/sprite/music/trainer class/transition; original two Full Restores and three AI flags; Singles; all unrelated trainer parties and event scripts.

## 5. Verified source checkpoint and technical requirements

- `src/data/trainers.h`: `TRAINER_ELITE_FOUR_LANCE` has the original trainer picture/name, `.items = {ITEM_FULL_RESTORE, ITEM_FULL_RESTORE}`, `.doubleBattle = FALSE`, the exact three AI flags, and `.party = ITEM_CUSTOM_MOVES(sParty_EliteFourLance)`. Do not change its metadata.
- `src/data/trainer_parties.h`: `sParty_EliteFourLance[]` is the existing vanilla five member custom-moves party. Replace with six approved member entries retaining `.iv=250`. Keep `sParty_EliteFourLance2[]` untouched in this implementation task.
- `src/data/pokemon/species_info.h` verified ability options: Crobat `INNER_FOCUS`; Aerodactyl `ROCK_HEAD, PRESSURE`; Articuno/Zapdos/Moltres `PRESSURE`; Dragonite `INNER_FOCUS`. Aerodactyl must actually receive ROCK_HEAD, one of its naturally allowed options, without adding trainer-specific alternative ability tables or species-wide changes. If deterministic selection cannot be achieved with preexisting trainer data mechanics, report a blocker for owner and do not silently switch to Pressure.
- `src/data/pokemon/level_up_learnsets.h` and `src/data/pokemon/tmhm_learnsets.h`: approved 24 move names originate from original FireRed natural learned moves and TM compatibility. Confirm final integrated Lightning Yellow move category/type effects and any approved TM reassignment rather than relying only on baseline vanilla definitions.
- `include/constants/items.h` or current item constants file: confirm item symbol spellings and effectiveness. These are existing FireRed held items.
- Natural repeats of PRESSURE and INNER_FOCUS are owner-approved. Never use proposed nonnative Static/Flame Body/Shed Skin on birds/Dragonite.

## 6. Acceptance criteria and validation

- [ ] Only initial Lance roster replaced; six Pokémon in exact specified order, types all Flying, exact levels and Dragonite slot 6 ace.
- [ ] All 24 moves, natural abilities including Rock Head for Aerodactyl, held items, `.iv=250` verified both in source and actual in-game.
- [ ] No changes in event triggering, dialogue, portrait, name, room, League fourth-member position, Singles, original two Full Restores or three AI flags.
- [ ] Ordinary trainer engine supports six usable party entries and normal fainting/sending behavior; no forced send-out strategy scripts.
- [ ] Legendary birds accessible in Lance's team without changing static encounters or enabling extra species.
- [ ] Run `AGENTS.md` required host build/regressions and relevant mGBA checks, log full results and limitations. No compiled ROM/proprietary data committed.
- [ ] Separate approved rematch battle remains unchanged by this initial-spec implementation.

## 7. Open questions / blockers

No owner DESIGN choices remain. Technical validation pending: six-Pokémon trainer array support; ability slot deterministic selection for Aerodactyl; integrated move data; engine battle/event behavior. Escalate engine conflicts rather than inventing changes.

## 8. Codex delivery requirements

After owner reviews and merges this **documentation-only** PR, implement the exact contract in a separate gameplay PR on a topic branch. Report source modifications and build/emulator evidence. Never auto-merge, alter baseline checks, or redesign unapproved content.

## 9. One-line Codex handoff

`Implement implementation/lance-initial-elite-four.md following AGENTS.md. Preserve all approved Lance data and encounters, validate six-party support and Aerodactyl's Rock Head, run required checks, and open a separate gameplay PR; report blockers rather than guessing.`
