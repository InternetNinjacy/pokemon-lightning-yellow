# Feature: Agatha initial Elite Four battle, special offense and defense

**Project:** Pokémon Lightning Yellow only. No other Pokémon hack's decisions apply.  
**Status:** Owner-approved design complete; documentation PR pending review and merge before gameplay implementation. No gameplay code changes or emulator tests claimed.  
**Approval:** November 4, 2026. Last explicit approval confirmed two Full Restores and exact original FireRed AI settings, completing earlier party approvals.  
**Handoff revision:** 2026-11-04  
**Dependencies:** Approved 252-species Pokédex, current typing and TM/move authorizations, Lightning Yellow per-move physical/special split, existing Trainer-party code.

## 1. Goal and player-facing behavior
Replace only the **initial** Agatha party in the existing third Elite Four encounter. Preserve her original FireRed sprite, name, class, music, room, event trigger, Singles format and progress flags. The specialty is **either high Special Attack OR high Special Defense**, using Pokémon whose typings include Psychic, Ghost, or Dark. Visually the roster is sinister, frightening, or supernatural. A Pokémon need not excel at both special stats. Gameplay features offensive sweepers plus special defensive pressure and recovery, target HARD difficulty without nonnative AI or unapproved mechanics.

## 2. Approved exact battle requirements
This table is literal. Preserve slot, level, all four move-slot order, ability, held item, and final ace.

| Slot | Pokémon | Lv | Move 1 | Move 2 | Move 3 | Move 4 | Ability | Held Item |
|---|---|---:|---|---|---|---|---|---|
| 1 | CACTURNE | 56 | GIGA_DRAIN | FAINT_ATTACK | LEECH_SEED | SPIKES | SAND_VEIL | MIRACLE_SEED |
| 2 | ALAKAZAM | 56 | PSYCHIC | SHOCK_WAVE | CALM_MIND | RECOVER | INNER_FOCUS | TWISTED_SPOON |
| 3 | UMBREON | 57 | TOXIC | MOONLIGHT | CONFUSE_RAY | FAINT_ATTACK | SYNCHRONIZE | LEFTOVERS |
| 4 | DUSCLOPS | 58 | WILL_O_WISP | NIGHT_SHADE | ICE_BEAM | PROTECT | PRESSURE | SITRUS_BERRY |
| 5 | GENGAR, ACE | 60 | SHADOW_BALL | THUNDERBOLT | PSYCHIC | GIGA_DRAIN | LEVITATE | SPELL_TAG |

Cacturne, Grass/Dark and Sp. Atk 115, replaces rejected Slowking, granting special Grass offense and sinister imagery. Alakazam, Psychic and Sp. Atk 135, is a fragile special sweeper; Umbreon, Dark and Sp. Def 130, is an attrition wall; Dusclops, Ghost and Sp. Def 130, is the burn/PP defensive wall; Gengar, Ghost/Poison and Sp. Atk 130, is signature ace with four direct-damage moves. All stats here are source Gen III baseline reference points, not permission for stat edits.

All five abilities are **distinct FireRed-native abilities**; do not choose Alakazam Synchronize accidentally, since INNER_FOCUS is specifically approved. Abilities do not repeat within the team; all five items distinct within the team. No across-team/Elite Four item-uniqueness requirement was authorized. Cacturne's SAND_VEIL only helps in appropriate weather; no automatic Sandstorm is approved.

Exactly **two ITEM_FULL_RESTORE** in Trainer bag, maintaining original FireRed item-use mechanics.

**AI:** `AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY` exactly.  
**Battle:** `.doubleBattle = FALSE`.  
No custom move sequencing, forced switches, extra items, scripted strategy, changed species abilities or battle effects.

## 3. Authority and provenance
- [Living Elite Four & Postgame Battle Design Register](https://docs.google.com/document/d/1qEdSSW26GkY3NmF-Na3_bIAMIgYFuVr4tGTKruBKUMI/edit), latest November 4 approval section for Agatha.
- [Master Plan](https://docs.google.com/document/d/1P4TuXViTpB0BycldB1SzRZTRX8sV_Rd0lFuPwyGDJQk/edit)
- [START HERE workflow](https://docs.google.com/document/d/14bV_mRB-IgcyWqGcIH2tgmvNoYLY1Ip_zSqSbdHGYtw/edit)
- [Approved 252-species Pokédex](https://docs.google.com/document/d/1Bv5Ssu_D6AxEDvr6xM3YECPRRAxO9yO3HVSNIos1gQc/edit)
- [Move & TM register](https://docs.google.com/document/d/1EHByhspPMrn_H0ard-wbdZLRqInZVschQqXTWSiP5fY/edit)
- [Change intake](https://docs.google.com/document/d/1NpCjx3uboBMdBVqoFpMwXEyVkDquD37Kcp-FJBbYWnc/edit)
- Owner approvals in November 4 **Pokémon Lightning Yellow Elite Four & Postgame Battle Design** conversation: either-special-stat criterion; only Psychic/Ghost/Dark families; Cacturne over Slowking; Gengar as ace; five ordered levels; movesets; distinct abilities and held items; finally two Full Restores and original AI.

No other project, historical retired handoff, or rematch roster may override this decision.

## 4. Scope
**In scope:** Rewrite the five first-party entries of `sParty_EliteFourAgatha[]` to the approved exact data, retaining original FireRed trainer metadata, two Full Restores and original AI. Ensure actual selected ability for Alakazam is INNER_FOCUS and all other abilities match, without species-wide modifications.

**Out of scope:** Agatha rematch (`TRAINER_ELITE_FOUR_AGATHA_2`), Lorelei, Bruno, Lance, Champion/rematches, maps and scripts, Trainer abilities globally, new mechanics, item effects, move definitions, dialogue or sprite changes, extra IV/EV tuning, enforced event sequencing. Do not redesign unapproved fights.

Preserve original third-member position, reward progression, trainer identity, and default first party `.iv = 250` policy unless a separately approved authoritative source overrides it.

## 5. Verified technical references and implementation checks
- `src/data/trainers.h`: `TRAINER_ELITE_FOUR_AGATHA` contains `.items = {ITEM_FULL_RESTORE, ITEM_FULL_RESTORE}`, `.doubleBattle = FALSE`, the exact three approved AI flags, and `.party = ITEM_CUSTOM_MOVES(sParty_EliteFourAgatha)`. Preserve.
- `src/data/trainer_parties.h`: first-party `sParty_EliteFourAgatha[]` still vanilla five slots (Gengar, Golbat, Haunter, Arbok, Gengar), all IV 250; change only the initial array. Keep separate rematch array untouched.
- `src/data/pokemon/species_info.h`: Cacturne `{ABILITY_SAND_VEIL, ABILITY_NONE}`; Alakazam `{ABILITY_SYNCHRONIZE, ABILITY_INNER_FOCUS}`; Umbreon `{ABILITY_SYNCHRONIZE, ABILITY_NONE}`; Dusclops `{ABILITY_PRESSURE, ABILITY_NONE}`; Gengar `{ABILITY_LEVITATE, ABILITY_NONE}`. **Alakazam's alternate ability selection is a technical blocker until deterministic trainer spawning is validated.** If impossible within already-approved mechanics/scope, STOP, report to owner, do not substitute Synchronize or edit the global species profile.
- `src/data/pokemon/level_up_learnsets.h`: Cacturne Faint Attack/Leech Seed/Spikes; Alakazam Psychic/Calm Mind/Recover; Umbreon Moonlight/Confuse Ray/Faint Attack; Dusclops Will-O-Wisp/Night Shade; Gengar Shadow Ball confirmed in source. Remaining respective TM compatibility entries are in `src/data/pokemon/tmhm_learnsets.h`, including Giga Drain, Shock Wave, Toxic, Ice Beam, Protect, Thunderbolt and Psychic. Reconcile approved move/TM overrides in integrated code.
- `include/constants/items.h`: existing Miracle Seed, Twisted Spoon, Leftovers, Sitrus Berry, Spell Tag and Full Restore constants. Confirm exact source identifiers before editing, e.g. `ITEM_MIRACLE_SEED`, `ITEM_TWISTED_SPOON`, `ITEM_LEFTOVERS`, `ITEM_SITRUS_BERRY`, `ITEM_SPELL_TAG`.
- Confirm special physical/category assignments in integrated Lightning Yellow per-move split, notably Gengar Shadow Ball category, and correct effective move behavior. Do not impose vanilla type-based split.
- Confirm existing Trainer monster generation actually permits the approved Alakazam INNER_FOCUS without hidden species-wide edits. Preserve original map script references to first battle.
- Owner approved actual Pokémon party order and ace identity. FireRed switching behavior may not guarantee ace sent out only at the very end if engine picks replacements. Do not add scripted forced order beyond existing behavior.

## 6. Acceptance and validation
- [ ] Original Agatha third Elite Four Single Battle, name, sprite, room, scripting, event flags retained.
- [ ] Exactly five Pokémon at levels 56, 56, 57, 58, 60 in specified party order; final slot Gengar ace.
- [ ] All 20 exact moves by slot, native five held items, and exact selected abilities; Alakazam explicitly INNER_FOCUS.
- [ ] Verify Cacturne seed/spikes/drain, Alakazam Calm Mind and Recover, Umbreon Toxic/Moonlight/Leftovers, Dusclops Pressure/Will-O-Wisp/Protect, Gengar Ghost special offense and coverage as intended; no custom tactics scripts.
- [ ] Two Full Restores, existing Trainer-item AI, original three source AI flags.
- [ ] All approved Psychic/Ghost/Dark membership and appropriate Special Attack or Special Defense in integrated Pokédex; preserve stats/type approved elsewhere.
- [ ] No edits to Lorelei, Bruno, Lance, Champion, Agatha rematch, scripts or global mechanical tables unless scope approved separately.
- [ ] Run current `AGENTS.md` prescribed prepare/build, host regression suites and full relevant mGBA emulator checks. Preserve upstream comparison and checksums; do not commit compiled ROM or proprietary artifacts.
- [ ] Gameplay implementation done on separate PR after documentation spec review and merge, with tests/results recorded.

## 7. Open questions / blockers
All Agatha **design choices** are owner approved. Engine validation remains: alternate ability assignment for Alakazam INNER_FOCUS, integrated move changes, battle script routing and emulator behavior. Stop on an unapproved conflict. No code edit, compile or emulator test claimed in this documentation handoff.

## 8. Codex delivery
Once the specification PR is reviewed and merged, Codex may implement only the reviewed contract in its own feature branch and open a separate gameplay PR; report conflicts rather than silently improvising. Do not auto-merge the new PR or redesign other fights.

## 9. Short Codex prompt (after spec review/merge)
`Implement implementation/agatha-initial-specialist.md following AGENTS.md. Preserve all approved battle data and original events, validate Alakazam Inner Focus, run required checks, and open a separate gameplay PR; report blockers rather than guessing.`
