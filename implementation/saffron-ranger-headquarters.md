# Feature: Saffron City Pokémon Ranger Headquarters

**Project:** Pokémon Lightning Yellow (FireRed-based; never import another Pokémon project's canon)  
**Status:** Ready for implementation **after owner review and merge of this specification PR**  
**Approval:** Owner approved the teams, abilities, moves, dialogue, one-time reward selection, mandatory four-Ranger progression, PC, and Trainer-bag items in the November 3, 2026 Ranger Headquarters design conversation. This is approved gameplay **design**, not evidence of code authorization or implementation.  
**Handoff revision:** 2026-11-03  
**Dependencies:** Project-wide 252 Pokédex/typing, independent physical/special/status split, approved move effects, repository branch-integration rules.

## 1. Goal and player-facing behavior

Replace the existing Saffron City Fighting Dojo with **Saffron City Pokémon Ranger Headquarters**, an optional five-Trainer challenge, while preserving the original interior room footprint/layout insofar as physical additions permit. It is **not** an official Gym. It awards no badge. Four regular Pokémon Rangers each offer a one-time Single Battle; all four must be defeated (any order) before Ranger Park, the commander at the rear, offers a one-time four-member Double Battle. Winning against Park unlocks three behind-the-commander Poké Balls; the player may receive exactly one Level 30 Pokémon: Arcanine, Noctowl, or Golduck. Add a usable Pokémon Storage PC by an entrance-side wall so a player with six Pokémon can deposit one. Gift collection requires a vacant party slot; no automatically sent-to-box gift and no lost gift on a full party.

Use existing FireRed Pokémon Ranger overworld and battle graphics, without new art. Existing NPC positions should be reused (four ordinary positions, Park in rear); add third gift ball, PC object/tile with unobstructed walkways. Exact placements must be determined by verifying the map's tileset and events, not guessed from this document.

This is a longstanding Kanto search-and-rescue organization: Rangers locate Trainers who have blacked out in the wild, return them to a Pokémon Center, and charge a sliding-scale rescue fee, typically roughly half a Trainer's cash. **Dialogue is lore; do not change the global blackout money formula** in this feature.

## 2. Approved Trainer rosters and battle data

All teams use the existing Pokémon Ranger Trainer class/presentation. **No Pokémon holds an item.** Trainer-bag items ARE approved: **one Super Potion for each regular Ranger; one Full Restore for Park.** These are per-battle inventories, AI-selected according to normal engine behavior, not scripted forced turns. No rematches or repeatable battles.

### Regular Ranger 1: Ground Search, Single Battle
Party order:
1. Persian Lv34, Normal/Dark, **Return / Faint Attack / Screech / Bite**, **high friendship** so Return has meaningful power. Ability: select a valid species ability according to approved global species data (not separately owner-locked).
2. Arcanine Lv35, Fire, **Flame Wheel / Take Down / Bite / Dig**, **Intimidate**.
Trainer bag: 1 Super Potion.

### Regular Ranger 2: Aerial Search, Single Battle
1. Fearow Lv35, Flying, **Aerial Ace / Steel Wing / Return / Pursuit**, **Keen Eye**. Ensure Return is meaningfully powered using appropriate friendship data.
2. Noctowl Lv36, Flying/Psychic, **Confusion / Hypnosis / Reflect / Aerial Ace**, **Insomnia**.
Trainer bag: 1 Super Potion.

### Regular Ranger 3: Water Rescue, Single Battle
1. Golduck Lv36, Water/Psychic, **Surf / Confusion / Calm Mind / Disable**, **Cloud Nine**.
2. Manectric Lv37, Electric, **Thunderbolt / Thunder Wave / Bite / Quick Attack**, **Static**.
Trainer bag: 1 Super Potion.

### Regular Ranger 4: Hazardous Terrain, Single Battle
1. Houndoom Lv37, Dark/Fire, **Sunny Day / Flamethrower / SolarBeam / Bite**, **Flash Fire**.
2. Arcanine Lv38, Fire, **Flame Wheel / Take Down / Dig / Iron Tail**, **Intimidate**.
Trainer bag: 1 Super Potion.

### Ranger Park: Final Challenge, Double Battle
First two party slots must enter together, in listed order; later replacements follow **native FireRed trainer Double Battle AI and switching**, no custom scripted replacement order or novel AI routines.
1. Noctowl Lv39, Flying/Psychic, **Reflect / Hypnosis / Psychic / Aerial Ace**, **Insomnia**.
2. Manectric Lv39, Electric, **Thunderbolt / Bite / Quick Attack / Protect**, **Static**.
3. Golduck Lv40, Water/Psychic, **Surf / Ice Beam / Confusion / Protect**, **Cloud Nine**.
4. Arcanine Lv41, Fire, **Flame Wheel / Take Down / Helping Hand / Protect**, **Intimidate**. **Ace**.
Trainer bag: 1 Full Restore.

**Move-category/engine boundary:** Apply owner-approved Lightning Yellow move-specific split, not vanilla Gen III type-based categories. Flame Wheel and Blaze Kick are **physical Fire**. Trainer sets take precedence over ordinary natural learn timing; use explicitly configured custom moves rather than accidentally defaulting to level-up moves. Do not alter species-global movesets simply to support individual Trainer sets. Park's Double Battle requires source-native battle format/event setup. No trainer level scaling.

## 3. Full approved NPC dialogue

Preserve wording below as source of player-facing text. Format to the target game's text/line lengths without altering meaning, and flag if forced to meaningfully revise.

### Ranger 1: Ground Search
**Before:** "We're Pokémon Rangers! When Trainers get stranded in the wild, we're the ones who find them!\n\nLet's see how well you handle a rescue team!"
**Defeat:** "You've got some good instincts!"
**After / repeat talk:** "Arcanine can track a missing Trainer by scent, even through rough terrain. It's one of our most dependable rescue Pokémon!"

### Ranger 2: Aerial Search
**Before:** "Our Flying Pokémon patrol the skies, looking for Trainers who need help.\n\nThink you can handle an aerial rescue team?"
**Defeat:** "You spotted every opening!"
**After / repeat talk:** "Fearow covers enormous distances during the day. Noctowl takes over at night.\n\nSomeone's always watching out for Trainers!"

### Ranger 3: Water Rescue
**Before:** "Rivers, lakes, oceans... Trainers can get into trouble anywhere!\n\nOur Pokémon train to handle even the toughest rescues!"
**Defeat:** "You're a strong Trainer!"
**After / repeat talk:** "Golduck is an incredible swimmer. It can reach stranded Trainers before anyone else.\n\nManectric helps us search along the shoreline!"

### Ranger 4: Hazardous Terrain
**Before:** "Rangers have been protecting Trainers for generations!\n\nWe've earned our reputation by going where others can't. Show me what you've got!"
**Defeat:** "You've earned my respect!"
**After / repeat talk:** "Caves, mountains, storms... No matter where a Trainer falls, we do our best to bring them home.\n\nThat's what being a Ranger means!"

### Ranger Park
**Initial exposition / introduction:** "Welcome to the Pokémon Ranger Headquarters!\n\nI'm Ranger Park. I've spent many years helping Trainers across Kanto.\n\nYou know, when a Trainer blacks out in the wild, they don't just wake up at a Pokémon Center by magic!\n\nThat's our job! We find them, get them to safety, and make sure their Pokémon receive treatment.\n\nWe do charge a rescue fee, but we work on a sliding scale. Usually, it's about half the money a Trainer has on hand.\n\nNobody gets left behind just because they're short on cash!\n\nNow, I've heard some impressive things about your journey.\n\nYou've got the makings of a fine Ranger yourself!\n\nHow about a battle? Show me what you've learned!"

**If four regular Rangers not yet defeated (blocks any battle regardless of YES/NO):** "You've got potential, but I'd like you to train with my four Rangers first. Come back after you've defeated them all!"

**After all four defeated, challenge YES/NO:**  
NO: "No pressure! A good Ranger knows when they're ready."  
YES: "Excellent! Let's see how you handle a real rescue team!"

**On defeat:** "Outstanding! You've got the courage and judgment of a true Ranger!"

**Talking afterward, before receiving gift:** "That was an impressive battle!\n\nYou've got a real talent for working with Pokémon.\n\nI think you'd make a fine Ranger someday!\n\nTake a look behind me. Those three Poké Balls contain Pokémon we've trained for rescue work.\n\nI'd like you to take one along on your journey. Consider it a gift from all of us!"

**After gift selection:** "Excellent choice! That Pokémon has helped us through countless rescue exercises.\n\nTake good care of it, and it'll take good care of you!\n\nRemember, a Ranger's greatest strength isn't power. It's knowing when someone needs help!"

**Dialogue flow clarification for Codex:** Intro lore and four-Ranger gate must both be represented. If first speaking before clearing four Rangers, Park explains Ranger rescue lore but does not offer a functional battle until the victories are recorded. The approved dialogue contains the challenge question in the same introduction, so the implementer must use conditional text segmentation / appropriate branching to avoid falsely promising a battle while it is locked. Do not invent fresh narrative lines to bridge this.

## 4. Gift event and exact Pokémon

Three Poké Ball objects behind Park, same concept as original two-ball Fighting Dojo. Interact to identify each Pokémon and ask for confirmation. A player can receive **exactly one**; all others become permanently unclaimable. If party has six, warn the player and allow them to use new PC to make room; **do not consume the reward flag**. Persist Park-defeat and gift-claimed flags through save/reload, map changes and resets; prevent multiple rewards from simultaneous/alternate interactions. No badge needed to use PC or earn gift.

All three gift Pokémon are **Level 30**, OT display name **PARK** and valid outsider/foreign Pokémon identity data so **1.5× traded experience applies** and normal FireRed obedience badges govern behavior. OT Trainer ID must not equal the player's for normal use; implement collision-safe logic if needed and test. Pokémon are otherwise ordinary (no shiny guarantee, no unapproved fixed IVs, natures, held items, ribbons, or abilities). Use project-approved species typing/abilities. Do not accidentally use Park as a battle Trainer's gift-mon data only; OT metadata must be in the resulting Pokémon.

| Gift | Lv | Exactly four moves in order | Special one-off nonstandard move |
|---|---:|---|---|
| Arcanine | 30 | Blaze Kick; Bite; Take Down; Odor Sleuth | Blaze Kick |
| Noctowl | 30 | Extrasensory; Hypnosis; Reflect; Aerial Ace | Extrasensory |
| Golduck | 30 | Muddy Water; Confusion; Calm Mind; Disable | Muddy Water |

All exclusive moves are **existing Gen III move IDs**, not invented attacks. These are specifically preconfigured one-off gift Pokémon move assignments, not general species learnset or TM access. Golduck Lv30 gift may exist below Psyduck's normal evolution level; this is intentional direct gift creation, not a global evolution rule change. Do not give any Ranger's battle Pokémon these gift-only signature moves unless independently approved (none are approved above).

No party auto-box for the claimed gift. Storage PC gives convenient normal deposit access.

## 5. Source authority and provenance

- Project Master Plan (Nov 3 amendments and isolation): https://docs.google.com/document/d/1P4TuXViTpB0BycldB1SzRZTRX8sV_Rd0lFuPwyGDJQk/edit
- Project START HERE index: https://docs.google.com/document/d/14bV_mRB-IgcyWqGcIH2tgmvNoYLY1Ip_zSqSbdHGYtw/edit
- Change Intake & Handoff: https://docs.google.com/document/d/1NpCjx3uboBMdBVqoFpMwXEyVkDquD37Kcp-FJBbYWnc/edit
- Approved 252 Pokédex: https://docs.google.com/document/d/1Bv5Ssu_D6AxEDvr6xM3YECPRRAxO9yO3HVSNIos1gQc/edit
- Ordinary trainer-class roster: https://docs.google.com/document/d/14FX3eDLXPCYbyxun-3oREFftSdaM25O0NhJaCXP7ZGY/edit
- Approved move register and changes: https://docs.google.com/document/d/1EHByhspPMrn_H0ard-wbdZLRqInZVschQqXTWSiP5fY/edit
- Source/conflict register: https://docs.google.com/document/d/1g0nqO-hN7tVaI_b-g1Sb7gRHbOljksfbcBVp3QUOaxM/edit
- **Precise approvals:** The November 3, 2026 owner conversation in the Lightning Yellow Ranger Headquarters design thread, culminating in owner approving Ranger 4, Park's team, all NPC dialogue, all three gift movesets, four-Ranger requirement, one-time battles, storage PC, and finally Trainer-bag Super Potions and Full Restore. This file embeds all decisions needed to implement this feature without Codex accessing Google Drive.
- Repo authority: `AGENTS.md`, `implementation/README.md`, `implementation/TEMPLATE.md`.

## 6. Scope boundaries and source work

**In scope:** Fighting Dojo replacement room and five NPC scripts; Ranger party definitions, abilities, item and move data; Park double-battle flag/event; flags to enforce prerequisites and one-time wins; three Poké Ball gift events and OT custom creation; storage PC; localized signage/dialogue appropriate to the new organization.

**Out of scope:** new gym/badge, global blackout penalty changes, new sprite or artwork, wholesale city redesign, automatic rematches, new AI, party level scaling, cross-project rival/Elite Four canon, species-global natural learnset additions, unapproved TM changes, changing battle move mechanics or broad species balancing.

**Preserve:** existing Saffron City map connections and normal visit accessibility; FireRed standard PC/boxes; non-headquarters Trainer battles; game-wide planned weather/day-night rules; non-headquarters gift encounters. Ranger HQ must not imply Gym clearance or affect Sabrina's Gym.

**Source investigation required:** Locate exact existing dojo map scripts/events/map.json, tileset and Poké Ball scripts; trainer flags and battle party/data files; script commands for four victory flags and Trainer double start; gift Pokémon creation API with OT/ID support; PC object/interface support and map tile collisions. Do not guess source paths solely from external pret upstream naming; actual integration branch may differ from main. Read applicable integration dependencies first.

## 7. Acceptance criteria and validation

- [ ] Room is consistently Ranger Headquarters, not Fighting Dojo, with five existing-style Ranger NPCs, four standard positions plus Park at rear.
- [ ] Every Trainer exactly matches approved species, order, level, moves, abilities, Trainer-bag item count/type, no held items; Persian/Fearow Return receives appropriately high friendship.
- [ ] Four regular Single Battles in any order; victory flags persist after save and reload; no rematches.
- [ ] Park refuses battle until all four victories; accepts only then; one-time Double Battle with proper opening pair and four members.
- [ ] Park uses the normal four-member Double Battle system, natural replacements and approved moves, not custom pair order/AI.
- [ ] Correct dialogue for every before/after/no/yes/gift state, text windows do not overflow or softlock.
- [ ] Three selectable reward balls, unique gift claim, no claim before Park defeat, gift remains claimable after full-party rejection, only one ever obtained, state persists after reload.
- [ ] Each gift correct level, moves, OT PARK and nonmatching identity, 1.5× foreign/traded EXP and normal badge obedience; no inadvertent species-global learnset unlock.
- [ ] Normal PC storage available by entrance before any battle; with six Pokémon can deposit to five and receive chosen gift; path to NPCs/balls remains traversable.
- [ ] No original dojo Fighting-type battles/gift Pokémon remain; unaffected Saffron and Sabrina functionality stable.
- [ ] Run build and relevant static checks, report branch baseline and move/category/typing dependencies; no silent baseline checksum gate changes.
- [ ] Manual emulator: test zero/three/four Ranger flags, all YES/NO dialogs, losing/rechallenging Park, six-party edge, each gift choice on separate saves, trade EXP/obedience, PC use, leave/re-enter, save/reload.
- [ ] Integration: test against the actual project-approved modified types, moves, AI flags, and future merged branches; distinguish isolated build pass from integrated emulator pass.

## 8. Open questions and implementation conditions

**No further owner gameplay choices are required for the above behavior.** These are engineer verifications, not permission to improvise:
1. Determine safe map coordinates for extra reward ball and existing-art PC, and verify tileset support. If original layout cannot be preserved, report deviation for owner review.
2. Determine local unique Trainer IDs/flags without colliding with existing Trainer or rematch events.
3. Determine whether the current native Double Battle trainer scripting properly handles player with only one usable Pokémon; use supported behavior or report a blocker, not a silent Single Battle fallback.
4. Confirm player/OT ID collision handling and legitimate ability/default stat initialization; do not replace approved traded Pokémon requirement with cosmetic name alone.
5. Confirm text encoding/window capacity and exact dialogue branching for Park's exposition/gate.
6. Confirm integration dependency branch/commit, as global modified moves and type changes may not be merged into main.
7. Regular Ranger 1 Persian's ability was not explicitly approved. Use the valid baseline/project-approved species ability; if an exact choice is needed beyond existing normal generation defaults, request approval rather than invent a design exception.

## 9. Codex delivery requirements

Implement only this feature on a separate topic branch **after owner review and merge of this specification PR**. Follow `AGENTS.md` and repository practices, do not merge any implementation PR. Show exact source changed, tests and limits, map collision screenshots where appropriate, and report unresolved source/integration issues. If technical constraints would require deviation from approved dialogue, move selections, gifts, double-battle behavior, or source setup, stop and request owner resolution.

## 10. One-line Codex handoff (ONLY after spec PR is merged)

`Implement implementation/saffron-ranger-headquarters.md from main following AGENTS.md. Run applicable tests and open an implementation PR; report blockers without changing approved gameplay decisions.`
