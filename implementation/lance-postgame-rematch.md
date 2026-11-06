# Feature: Lance Flying-specialist postgame rematch, levels-only

**Project:** Pokémon Lightning Yellow ONLY  
**Status:** OWNER-APPROVED DESIGN; documentation-only PR awaiting review. Gameplay NOT implemented or tested.  
**Approval:** November 5, 2026, Elite Four & Postgame Battle Design thread.  
**Handoff revision:** 2026-11-05  
**Dependencies:** Approved 252-species Pokédex, move and TM register, approved initial Elite Four designs, integrated per-move Physical/Special split.

## Source of authority
- [Canonical living Elite Four & Postgame Battle Design Register](https://docs.google.com/document/d/1qEdSSW26GkY3NmF-Na3_bIAMIgYFuVr4tGTKruBKUMI/edit), postgame Elite Four rematches 2026-11-05.
- [Project Master Plan](https://docs.google.com/document/d/1P4TuXViTpB0BycldB1SzRZTRX8sV_Rd0lFuPwyGDJQk/edit) and [START HERE workflow](https://docs.google.com/document/d/14bV_mRB-IgcyWqGcIH2tgmvNoYLY1Ip_zSqSbdHGYtw/edit)
- [Approved Pokédex](https://docs.google.com/document/d/1Bv5Ssu_D6AxEDvr6xM3YECPRRAxO9yO3HVSNIos1gQc/edit), [Move and TM register](https://docs.google.com/document/d/1EHByhspPMrn_H0ard-wbdZLRqInZVschQqXTWSiP5fY/edit), [change-intake log](https://docs.google.com/document/d/1NpCjx3uboBMdBVqoFpMwXEyVkDquD37Kcp-FJBbYWnc/edit)
- November 5 owner approval of the complete proposed postgame rematch settings in the Elite Four design conversation.
- Never import alternate Pokémon ROM hack canon, or alter the separately governed Champion / Champion rematch.

## Shared existing FireRed rematch trainer settings
Preserve Trainer class, sprite, character name, encounter music, original rematch unlock/progression/event scripts, `.doubleBattle = FALSE`, exactly `.items = {ITEM_FULL_RESTORE, ITEM_FULL_RESTORE}`, and precisely `.aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY`, retaining `ITEM_CUSTOM_MOVES(...)` party format. Every rematch Pokémon uses original FireRed rematch `.iv = 255`. No additional bag items, custom trainer AI, forced party switching, hidden type changes or other mechanics. All ability choices must be natural species abilities in source; if a chosen alternate ability cannot be selected without unapproved trainer customization, report blocker, not an alternative design. No changes to initial first-playthrough Elite Four or unrelated trainers.

## Boundaries / implementation workflow
**In scope:** Only the approved corresponding `sParty_EliteFour...2[]` rematch TrainerMon array in `src/data/trainer_parties.h`, and only tightly necessary preapproved source adaptations to reliably instantiate natural ability choice. Original rematch trainer metadata in `src/data/trainers.h` should be preserved.  
**Out of scope:** Initial Elite Four encounters, Champion and rival, world maps and scripts, random wild/spawn data, Pokémon species global abilities or stats, item effects, rematch trigger changes, new AI, unrelated battle systems.  
Current code paths observed in repository `src/data/trainers.h` and `src/data/trainer_parties.h`; consult `src/data/pokemon/species_info.h`, `src/data/pokemon/level_up_learnsets.h`, `src/data/pokemon/tmhm_learnsets.h` for underlying compatibility and original abilities. The approved Lightning Yellow move system may change damage category compared with vanilla Gen III. All records must be reconciled with its authoritative move changes.

## Shared verification / acceptance
- [ ] Approved Trainer ID, rematch trigger, sprite, name, event flags, Singles battle preserved; first-encounter array untouched.
- [ ] Correct party count, order, levels, all move slot IDs, held items, selected abilities, and 255 IVs verified in source and emulator.
- [ ] Trainer bag exactly two Full Restores; exactly three original AI flags, unchanged.
- [ ] Verify each Pokémon's approved move compatibility and Lightning Yellow move-specific Physical/Special category.
- [ ] No unapproved global abilities, moves, weather, special programming or other battle changes.
- [ ] Follow `AGENTS.md` for build, host regression, and relevant mGBA checks; compare with source baseline; report any test not run.
- [ ] Game code implemented only after specification PR owner review and merge, on separate implementation branch and PR. No auto-merge, proprietary ROM binary, bypassed tests or modified baseline.

## Codex delivery
Implement exact approved data only after doc PR is merged. Report conflicts rather than inventing substitutions. Provide a separate implementation PR with tested changes and any emulator limitations.

## Approved exact party
**Target:** `TRAINER_ELITE_FOUR_LANCE_2`, party array `sParty_EliteFourLance2[]` (REMATCH ONLY). **Target difficulty:** Very Hard.

Flying-only master, sole Elite Four member permitted legendary birds and Dragonite. Postgame is LEVEL-ONLY change to six previously approved initial-battle species in exact same order and with EXACT same 24 moves, six natural abilities, and six held items. Original FireRed rematch 255 IVs and original rematch trainer metadata are preserved. Dragonite ace remains last.

| Order | Species | Level | Move 1 | Move 2 | Move 3 | Move 4 | Natural ability | Held item |
|---|---|---:|---|---|---|---|---|---|
| 1 | CROBAT | 70 | CONFUSE_RAY | POISON_FANG | AERIAL_ACE | SHADOW_BALL | INNER_FOCUS | POISON_BARB |
| 2 | AERODACTYL | 71 | ANCIENT_POWER | AERIAL_ACE | EARTHQUAKE | IRON_TAIL | ROCK_HEAD | HARD_STONE |
| 3 | ARTICUNO | 72 | ICE_BEAM | BLIZZARD | REFLECT | WATER_PULSE | PRESSURE | NEVER_MELT_ICE |
| 4 | ZAPDOS | 72 | THUNDERBOLT | THUNDER | DRILL_PECK | THUNDER_WAVE | PRESSURE | MAGNET |
| 5 | MOLTRES | 73 | FLAMETHROWER | FIRE_BLAST | OVERHEAT | AERIAL_ACE | PRESSURE | CHARCOAL |
| 6 | DRAGONITE (ACE) | 75 | OUTRAGE | EARTHQUAKE | THUNDERBOLT | ICE_BEAM | INNER_FOCUS | DRAGON_FANG |

## Individual behavioral notes and blockers
No substitutions, no move/ability/item edits relative to approved initial Lance battle. Three legendary birds all naturally Pressure; Crobat and Dragonite naturally Inner Focus. Repeats intentional, no special ability overrides. Aerodactyl Rock Head is one of its two natural Gen III options; verify selected ability in actual Trainer battle, report blocker if natural selection isn't deterministically guaranteed. Trainer party must support six entries; do not alter original initial battle.

## Scope-specific acceptance
- [ ] Only `sParty_EliteFourLance2[]` rewritten from vanilla FireRed rematch; no first encounter or other Elite Four rematches touched by THIS implementation task.
- [ ] All exact slots, 4 moves, abilities, items and `.iv=255`, original trainer metadata and existing rematch script, in-game verification completed.
- [ ] Flying-only master, sole Elite Four member permitted legendary birds and Dragonite. Postgame is LEVEL-ONLY change to six previously approved initial-battle species in exact same order and with EXACT same 24 moves, six natural abilities, and six held items. Original FireRed rematch 255 IVs and original rematch trainer metadata are preserved. Dragonite ace remains last.

## One-line Codex handoff
`Implement implementation/lance-postgame-rematch.md following AGENTS.md, preserve all approved data, run appropriate tests and open a separate implementation PR; report blockers instead of guessing.`
