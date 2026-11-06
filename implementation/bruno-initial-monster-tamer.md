# Feature: Bruno initial Elite Four battle, Monster Tamer

**Project:** Pokémon Lightning Yellow only  
**Status:** Owner-approved full design; implementation handoff ready only upon review and merge of this documentation PR. No gameplay code implemented or tested.  
**Approval:** November 4, 2026, owner approved final two Full Restores and original AI flags after approving all party/move/ability/item decisions  
**Handoff revision:** 2026-11-04  
**Dependencies:** 252-species Pokédex, existing approved types/abilities, current Lightning Yellow per-move category split, original Trainer-party framework. Must reconcile active integrated code and any other approved feature branches before implementation.

## 1. Goal and player-facing behavior

Bruno remains the original **second** Elite Four opponent with his original sprite, identity, class, dialogue, room, battle event, and League advancement. His first encounter changes to a multi-type **Monster Tamer** theme: physically intimidating creatures of various types, not a Fighting specialist. Every Pokémon has exactly four **damaging** moves and fights aggressively without setup, recovery, screens, status-only or protective moves. Some risky moves and limitations are intentional. Target HARD challenge through approved roster and original Trainer behavior, not invented buffs or custom AI.

## 2. Approved design decisions

Exact team and fixed move-slot order; last slot is ace and intended last party member:

| Slot | Pokémon | Lv | Move 1 | Move 2 | Move 3 | Move 4 | Ability | Held item |
|---|---|---:|---|---|---|---|---|---|
| 1 | ELECTABUZZ | 54 | THUNDER | PSYCHIC | BRICK_BREAK | IRON_TAIL | STATIC | MAGNET |
| 2 | MAGMAR | 54 | FIRE_BLAST | PSYCHIC | BRICK_BREAK | IRON_TAIL | FLAME_BODY | CHARCOAL |
| 3 | URSARING | 55 | THRASH | EARTHQUAKE | BRICK_BREAK | AERIAL_ACE | GUTS | CHOICE_BAND |
| 4 | GYARADOS | 56 | HYDRO_PUMP | EARTHQUAKE | BLIZZARD | HYPER_BEAM | INTIMIDATE | MYSTIC_WATER |
| 5 | NIDOKING (ACE) | 58 | MEGAHORN | EARTHQUAKE | SLUDGE_BOMB | THUNDER | POISON_POINT | SOFT_SAND |

- Distinct original Gen III abilities and held items as listed, no duplicate abilities, no post-FireRed ability additions.
- The owner expressly replaced Charizard with Nidoking to avoid duplicating Magmar's Fire theme, declined Aggron, selected Ursaring, and later locked Nidoking as ace. Do not include Charizard, Aggron, or a sixth Pokémon.
- Electabuzz and Magmar embrace powerful, sometimes inaccurate moves; Ursaring uses forced Thrash and Choice Band lock; Gyarados Hydro Pump/Blizzard may miss and Hyper Beam has a recharge drawback; Nidoking has powerful varied coverage. Preserve approved move/category behavior even if a move is suboptimal for its stats.
- Single Battle, `.doubleBattle = FALSE`.
- Trainer-use items: **exactly two `ITEM_FULL_RESTORE`**. Original FireRed item AI and use rules unchanged. This does not authorize defensive moves for party members.
- Trainer AI flags unchanged: `AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY`.
- Do not change original sprite, class, encounter soundtrack, name, route/map, dialogue, scripts or Elite Four progress flags.

## 3. Authoritative sources and provenance

- [Living Elite Four & Postgame Battle Design Register](https://docs.google.com/document/d/1qEdSSW26GkY3NmF-Na3_bIAMIgYFuVr4tGTKruBKUMI/edit), November 4, 2026 owner approval.
- [Master Plan](https://docs.google.com/document/d/1P4TuXViTpB0BycldB1SzRZTRX8sV_Rd0lFuPwyGDJQk/edit)
- [START HERE index](https://docs.google.com/document/d/14bV_mRB-IgcyWqGcIH2tgmvNoYLY1Ip_zSqSbdHGYtw/edit)
- [Approved 252-species Pokédex](https://docs.google.com/document/d/1Bv5Ssu_D6AxEDvr6xM3YECPRRAxO9yO3HVSNIos1gQc/edit)
- [Global TM and Move register](https://docs.google.com/document/d/1EHByhspPMrn_H0ard-wbdZLRqInZVschQqXTWSiP5fY/edit)
- [Change-intake approvals](https://docs.google.com/document/d/1NpCjx3uboBMdBVqoFpMwXEyVkDquD37Kcp-FJBbYWnc/edit)
- Original source: `src/data/trainers.h`, `src/data/trainer_parties.h`, `src/data/pokemon/species_info.h`, `src/data/pokemon/level_up_learnsets.h`, `src/data/pokemon/tmhm_learnsets.h`, `include/constants/items.h`, `include/constants/opponents.h`.
- Approval trail: November 4, 2026, ongoing **Elite Four & Postgame Battle Design** conversation, final owner message: "Approved" explicitly in response to retaining Bruno's two Full Restores and original AI flags. All earlier exact species/moves/levels/abilities/held items separately approved.

Important isolation: these decisions belong specifically to Pokémon Lightning Yellow, not another FireRed hack. Other Elite Four members' design from different projects is not authoritative.

## 4. Implementation boundaries

**In scope:** Rewrite only Bruno's **first-match** party `sParty_EliteFourBruno` to the exact table, retain original Trainer metadata and item/AI inventory, ensuring correct five abilities in battle.

**Out of scope:** Bruno rematch `sParty_EliteFourBruno2` / second Trainer ID, any other League member or rival, species-wide changes, new abilities or moves, custom AI, editing Gym teams, new scripts/events/maps, changes to item engine, or optimization of approved moves.

**Preserve:** Elite Four order and original Bruno identity/sprite; normal completion and save flags; original two Full Restores; default IV policy (original party `.iv = 250` on all five) unless directly contradicted by a separately approved authoritative source. Do not raise IVs/EVs for extra difficulty without approval.

## 5. Technical implementation notes

- `src/data/trainers.h`: `TRAINER_ELITE_FOUR_BRUNO` currently has original `ITEM_CUSTOM_MOVES(sParty_EliteFourBruno)`, `.items = {ITEM_FULL_RESTORE, ITEM_FULL_RESTORE}`, `.doubleBattle = FALSE`, and exact approved three AI flags. Preserve each field.
- `src/data/trainer_parties.h`: source vanilla party five mons (Onix, Hitmonchan, Hitmonlee, Onix, Machamp) with 250 IVs; replace first-match array only, not rematch. Original trainer struct already supports fixed moves and held items.
- `src/data/pokemon/species_info.h`: current vanilla source confirms unique native abilities ELECTABUZZ STATIC, MAGMAR FLAME_BODY, URSARING GUTS, GYARADOS INTIMIDATE, NIDOKING POISON_POINT; all have `ABILITY_NONE` alternate, so normal first-slot instantiation is expected. Verify actual trainer creation and any integrated ability overrides.
- `src/data/pokemon/level_up_learnsets.h`: confirms Electabuzz THUNDER at Lv58 (therefore specifically assigned early to this Lv54 Trainer by approved custom move slot), Magmar FIRE_BLAST at Lv57 (similarly assigned), Ursaring THRASH at Lv49, Gyarados HYDRO_PUMP at Lv40 and HYPER_BEAM at Lv55, Nidoking MEGAHORN at Lv43. Approved fixed Trainer moves are not constrained to natural level-up age.
- `src/data/pokemon/tmhm_learnsets.h`: confirms relevant Thunder/Psychic/Brick Break/Iron Tail/Fire Blast/Earthquake/Aerial Ace/Blizzard/Sludge Bomb etc by species. The complete 20-move compatibility check was done in design thread; reconcile any integrated TM remaps before coding.
- `include/constants/items.h`: all five item constants (MAGNET, CHARCOAL, CHOICE_BAND, MYSTIC_WATER, SOFT_SAND) and FULL_RESTORE exist.
- Move-specific Physical/Special split already approved for Lightning Yellow. Verify exact integrated categories and effects rather than assuming vanilla by type. Avoid mechanical changes to improve Bruno.
- Check exact Trainer constant and event routing in integrated code; do not edit original maps/scripts without approved requirement.
- Existing AI is not guaranteed to use high-risk attacks or Trainer Full Restores strategically. This is acceptable and does not authorize custom AI.
- GYARADOS is present on another Gym team elsewhere; cross-League species duplication was **not** forbidden. Do not silently substitute.

## 6. Acceptance criteria and validation

- [ ] First-match Bruno triggered as the second Elite Four encounter with original sprite, identity, room and progression, still Singles.
- [ ] Exactly five Pokémon in order, precise levels 54,54,55,56,58, 20 exact move slots, five distinct abilities, held items, Nidoking last ace.
- [ ] All 20 moves damaging and offensive, no setup, self-healing or protective moves, including in actual tested battle.
- [ ] Preserve risk costs: move accuracy, Thrash confusion/lock, Choice Band choice lock, Hyper Beam recharge; do not change battle engine mechanics to circumvent.
- [ ] Exactly two Trainer Full Restores, unchanged source three AI flags and stock FireRed Trainer AI.
- [ ] Confirm Guts, Intimidate and other abilities, original FireRed item effects, and approved per-move split through battle/equivalent deterministic test.
- [ ] No change to Bruno rematch or other Elite Four, Champion, Gym, map, script, dialogue, abilities globally or event flags.
- [ ] Confirm all approved species present in integrated 252-Pokédex build.
- [ ] Build and test according to current `AGENTS.md`: `scripts/prepare-baseline.sh`, `make -j2`, applicable host regression suites, and relevant mGBA integration; record actual tests and untested areas; keep baseline checksum distinct from gameplay builds.
- [ ] Gameplay changes in separate reviewable PR; no generated ROM committed or released.

## 7. Open questions and blockers

No remaining owner **design** decisions for initial Bruno battle. Before implementation, verify integrated move/category overrides, exact trainer creation/ability behavior, source trigger routing, and whether any branches alter source schema. Report conflicts rather than change approved content. No actual gameplay code, compile, or emulator run was done to produce this handoff.

## 8. Codex delivery requirements

After owner reviews/merges this documentation-only spec PR, Codex should implement this exact scoped party on a new branch and open a separate gameplay PR. Must follow `AGENTS.md`, compare source and authoritative decisions, run required checks, and report blockers. Never auto-merge or implement other Elite Four.

## 9. One-line Codex handoff (only after spec PR reviewed/merged)

`Implement implementation/bruno-initial-monster-tamer.md following AGENTS.md. Keep the exact approved five-member all-offense party, original Trainer AI and two Full Restores; run required checks and open a gameplay PR, reporting blockers rather than guessing.`
