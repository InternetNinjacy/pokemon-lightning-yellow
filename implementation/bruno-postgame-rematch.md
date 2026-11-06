# Feature: Bruno Monster Tamer postgame rematch

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
**Target:** `TRAINER_ELITE_FOUR_BRUNO_2`, party array `sParty_EliteFourBruno2[]` (REMATCH ONLY). **Target difficulty:** Very Hard.

Monster Tamer specialist, NOT mono Fighting type. Five physically imposing monsters with exactly FOUR damaging moves each, without setup, recovery, protection, or status-only moves. Electabuzz replaced by Tyranitar, Ursaring replaced by Machamp, and Magmar, Gyarados and Nidoking retained. Nidoking remains ace.

| Order | Species | Level | Move 1 | Move 2 | Move 3 | Move 4 | Natural ability | Held item |
|---|---|---:|---|---|---|---|---|---|
| 1 | TYRANITAR | 65 | ROCK_SLIDE | CRUNCH | EARTHQUAKE | HYPER_BEAM | SAND_STREAM | BLACK_GLASSES |
| 2 | MAGMAR | 66 | FIRE_BLAST | PSYCHIC | BRICK_BREAK | IRON_TAIL | FLAME_BODY | CHARCOAL |
| 3 | MACHAMP | 66 | CROSS_CHOP | EARTHQUAKE | ROCK_TOMB | HYPER_BEAM | GUTS | BLACK_BELT |
| 4 | GYARADOS | 67 | HYDRO_PUMP | EARTHQUAKE | BLIZZARD | HYPER_BEAM | INTIMIDATE | MYSTIC_WATER |
| 5 | NIDOKING (ACE) | 70 | MEGAHORN | EARTHQUAKE | SLUDGE_BOMB | THUNDER | POISON_POINT | SOFT_SAND |

## Individual behavioral notes and blockers
Tyranitar Sand Stream is native and automatically invokes ordinary sandstorm; this can damage later nonimmune team members, and is not permission to override sandstorm behavior. Keep its opening slot. Four-damaging-move invariant is mandatory. In rematch Magmar, Gyarados, Nidoking retain their four approved initial attacks; do not add custom AI.

## Scope-specific acceptance
- [ ] Only `sParty_EliteFourBruno2[]` rewritten from vanilla FireRed rematch; no first encounter or other Elite Four rematches touched by THIS implementation task.
- [ ] All exact slots, 4 moves, abilities, items and `.iv=255`, original trainer metadata and existing rematch script, in-game verification completed.
- [ ] Monster Tamer specialist, NOT mono Fighting type. Five physically imposing monsters with exactly FOUR damaging moves each, without setup, recovery, protection, or status-only moves. Electabuzz replaced by Tyranitar, Ursaring replaced by Machamp, and Magmar, Gyarados and Nidoking retained. Nidoking remains ace.

## One-line Codex handoff
`Implement implementation/bruno-postgame-rematch.md following AGENTS.md, preserve all approved data, run appropriate tests and open a separate implementation PR; report blockers instead of guessing.`
