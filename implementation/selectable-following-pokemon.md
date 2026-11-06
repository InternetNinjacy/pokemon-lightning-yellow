# Feature: Selectable Following Pokémon, all 252 Lightning Yellow species

**Project:** Pokémon Lightning Yellow (FireRed revision 0 decompilation; NOT any other Pokémon hack)
**Status:** Ready for implementation AFTER review and merge of this specification PR; not gameplay-implemented
**Approval:** Owner's November 5, 2026 conversation: all roster species followable, explicit party-menu selection, no fallback, scripted initial Pikachu assignment, unconscious/boxed rules.
**Handoff revision:** 2026-11-05 v1
**Dependencies:** The approved 252-species regional Pokédex, approved opening Pikachu gift and Oak's Parcel -> Eevee gift event, current repository implementation and AGENTS.md. Do not invent unapproved changes to those event rewards.

## 1. Goal and player-facing behavior

Any of the EXACT 252 species in Lightning Yellow's approved regional Pokédex, when held in the player's current party, can be explicitly selected to follow the protagonist on traversable overworld maps. No requirement to occupy slot 1 or lead battles. No automatic fallback to lead or another Pokémon. One follower at most.

The initial gift Pikachu is the **sole forced/automatic follower assignment**: as part of the approved opening gift scene Pikachu emerges from its Poké Ball on the overworld and is assigned through the SAME persistent follower-selection mechanism used by the party menu. Thereafter the player may choose a different party Pokémon or no follower. When the player returns Oak's Parcel from Viridian and obtains the already-approved gift Eevee, the player must be able to assign Eevee immediately from the party menu; this Eevee event must NOT force a replacement of Pikachu.

## 2. Approved behavior and state transitions

- Introduce a party context-menu action "Set as Follower" for a selectable party Pokémon, and an unambiguous way to select "Stop Following"/clear assignment. The current selected party Pokémon should be visually identifiable in the menu where practical. Prefer reusing existing party menu constructs; do not redesign unrelated menus.
- The selection identifies an actual Pokémon **instance**, not merely species, nickname, or party slot. Party reorder, party lead swaps, battle lead, and nickname changes do not change the assigned Pokémon. Selecting a different party member changes the assignment.
- A party Pokémon assigned as follower and conscious (HP > 0) normally has its species-appropriate overworld follower sprite. A Pokémon that faints remains ASSIGNED but is HIDDEN. Healing through Revive, Pokémon Center, or any other normal means automatically makes the SAME Pokémon reappear as soon as overworld display is safe. The player need not reassign it. There is no replacement follower while it is fainted.
- Depositing/otherwise removing the selected Pokémon from the active party clears the follower assignment and removes its sprite. Withdrawing it later does not implicitly restore the selection. Depositing an unrelated party member must not clear the assignment. If the selected Pokémon is traded, released, or otherwise lost, likewise clear it; never bind the slot to a replacement instance.
- The explicit assignment persists across save/load and permitted map transitions while the Pokémon remains in the party. An unset assignment also persists; no automatic re-selection upon loading or first entry into a new map.
- A Pokémon's evolution preserves its selection and changes the displayed follower artwork to the evolved species if that evolution remains on the approved roster. Nickname, shininess, and changes to form (where supported by approved game data) must not silently bind to another instance.
- Conceal, suspend, or safely park the overworld object in incompatible activities and cutscenes (e.g. surfing, bicycle, tight scripted scenes); restore its appearance when compatible. This is visual suppression, not assignment clearing. Never let the follower obstruct movement, cause unwanted interactions or duplicate itself.
- Upon entering a battle or a script scene, follower visibility must not desynchronize the player, party or event states. A follower is visual/interactive only and is NOT an additional party Pokémon or battler.
- The first scripted Pikachu emergence and automatic assignment happen exactly once as part of the opening Pikachu gift, never on ordinary map reload, faint recovery, or a second party acquisition. Do not create a second Pokémon.
- Follower may be interacted with via normal overworld controls when safe; minimally support an ordinary response/cry. Elaborate Pokémon Yellow happiness/emotion face UI and hundreds of bespoke dialogue lines are NOT approved scope.

### Explicit selection examples
1. Starter Pikachu is assigned automatically; Pikachu appears outside its ball after opening gift.
2. Oak parcel returned -> Eevee acquired -> player manually selects Eevee; Eevee appears, Pikachu is no longer rendered, battle lead unchanged.
3. Eevee faints -> Eevee vanishes but remains chosen; healing -> Eevee returns; no substitute.
4. Selected Eevee deposited -> none appears and assignment is cleared. Withdrawing later -> still none until player manually selects.
5. Chosen Pokémon evolves -> evolved form follows without requiring another selection.

## 3. Roster and asset constraints

- Scope is all 252 species in the project's **approved** regional list, not arbitrary 386 or later-generation species. The official roster is https://docs.google.com/document/d/1Bv5Ssu_D6AxEDvr6xM3YECPRRAxO9yO3HVSNIos1gQc/edit . Derive the full roster from existing repo data and approved contract. Do not assume species indexes 1..252 map to the original National Dex or include excluded families.
- Reuse existing licensed/appropriately attributed follower movement engines and Gen I-III overworld sprite sheets to the greatest technically practical extent. FireRed already contains an OBJ_EVENT_GFX_PIKACHU sprite and related event graphics; assess its usable frames/animations instead of replacing art unnecessarily.
- External RESEARCH references, not code authority:
  - https://github.com/chale1342/pokefirered : FireRed-derived follower_helper.c, follower scripts, save/load logic, object graphics. Its graphics-info follower header inspected at handoff explicitly defines only the first nine starters, NOT verified coverage of 252. Audit full asset set rather than asserting completeness.
  - https://github.com/pret/pokefirered : upstream FireRed.
  - https://github.com/Kasenn/pokeemerald-expansion-kasen and Team Aqua's Hideout preferred follower feature: party-menu selection ideas, Emerald-derived and NOT drop-in FireRed compatibility.
  - Other Emerald follower sprite repositories may contain much wider roster coverage, but Codex must inspect source versions, frames, license and availability rather than blindly importing.
- Produce a species-by-species coverage audit for **all 252** with asset origin, direction/animation availability, shiny support if present, and any gaps. The finished scope requires artwork for all 252. If missing art or licensing blocks completion, report concrete missing species and stop affected scope for owner guidance rather than inventing Nintendo asset redistribution or pretending done.
- Preserve licensing/attributions and repository rules against uploading unrelated proprietary ROM/graphics/binaries. Do not distribute copyrighted game ROMs.

## 4. Technical implementation boundary

In scope: follower selection and persistent instance identity; NPC/object sprite loading and following movement; party menu entry and stop command; opening Pikachu scripted emergence; map transition safety; battle/overworld sync; faint/heal/PC/evolution handlers; graphic lookup limited to approved 252; automated and emulator validation.

Out of scope: changing approved 252 roster, starter species or gift levels, parcel/Eevee gift timing, unrelated trainer teams and rivals, gameplay battle order or battle logic, Pokémon stats/moves/evolution rules, new weather/clock mechanics, non-roster follower support, affection/emotion subsystem, multiple simultaneous followers, surfing sprite redesign. Only narrowly necessary script changes to integrate approved opening Pikachu and Eevee gift are authorized.

Check existing repository main and all relevant open PRs before altering source. Avoid work already implemented elsewhere; do not overwrite tests or branch dependencies. Do not copy large code wholesale without auditing compatibility and license.

## 5. Save persistence / identity safety

Prefer a stable identity or durable party-instance mechanism that can distinguish duplicate species, party reorder, trading, storing and evolution without conflating the chosen Pokémon with party index. Preserve existing saved-game layout and other approved allocated state, especially daycare and vendor blocks. Determine a safe versioned or reserved storage solution based on the CURRENT main checkout. If no safe persistence mechanism exists without conflict, stop and explain the alternative instead of modifying save layout blindly. Validate older save states, missing/invalid follower references and PC transfers. Invalid or absent selection resolves to no follower, never slot 1.

## 6. Source and provenance

- Master Plan: https://docs.google.com/document/d/1P4TuXViTpB0BycldB1SzRZTRX8sV_Rd0lFuPwyGDJQk/edit
- Project index and workflow: https://docs.google.com/document/d/14bV_mRB-IgcyWqGcIH2tgmvNoYLY1Ip_zSqSbdHGYtw/edit
- Change Intake: https://docs.google.com/document/d/1NpCjx3uboBMdBVqoFpMwXEyVkDquD37Kcp-FJBbYWnc/edit
- Approved roster: https://docs.google.com/document/d/1Bv5Ssu_D6AxEDvr6xM3YECPRRAxO9yO3HVSNIos1gQc/edit
- Exact owner approval: November 5, 2026, interactive conversation on select-any-party-member, no fallback, faint remains assigned until revival, deposited clears, Pikachu automatically assigned ONCE in its scripted gift, Eevee selectable after Parcel.
- Source authority: AGENTS.md and the present repository checkout. Current exact function/file integration points for user's new follower support must be inspected by Codex, not presumed.

## 7. Acceptance criteria and validation

- [ ] Every approved roster species has usable follower graphics and direction/walking behavior, with audited gaps explicitly listed. Test a representative set of small, huge, flying, water, baby, unusual-shape and evolved species in emulator.
- [ ] Gift Pikachu enters party once, emerges from Poké Ball, is automatically selected in the very first gift scene; scene works for both player avatars.
- [ ] Parcel->Eevee gift remains as approved, permits Eevee assignment immediately, does NOT implicitly change the current follower.
- [ ] Party-menu selection works for ANY slot; lead slot stays unchanged; nonselected fainted party members never become fallback.
- [ ] Faint HP0 hides but preserves assignment across battles, saving/loading and map changes; Revive and full healing cause same selected instance to reappear without player action.
- [ ] Deposit selected member clears; withdraw does not reselect. Deposit unselected member retains existing selection. Party reshuffling and identical-species duplicates do not redirect follower.
- [ ] Evolutions, including Eevee's branches, preserve selection and update sprite.
- [ ] Save/reload with selected, fainted, and cleared states works without corrupting party, box, daycare, RTC, Dex, gifts or other serialized systems.
- [ ] Map and movement stress: doors, elevators, caves, stairs, ledges, narrow paths, warps, cutscenes, bicycle, Surf, scripted moves, forced movements, battle transitions, post-battle healing.
- [ ] Never spawn a duplicate, block movement, strand an NPC, softlock, or exceed object/palette/tile resource budgets. Check map/object limits.
- [ ] Run clean build and all applicable project test suites per AGENTS.md and mGBA real-ROM tests. Add targeted automated follower-state tests plus manual emulator checklist. Record exact tests run/not run, licensing and remaining caveats.
- [ ] Keep gameplay implementation on a NEW feature branch/PR after this specification is merged, with no auto-merge or ROM release authorization inferred from this request.

## 8. Open technical questions (Codex research, not permission to invent gameplay)

Determine compatible preexisting movement engine, licensing of each sprite set, complete 252 coverage, legacy save migration and robust party-instance identity, rendering during Surf/bicycle, available palette/tile/object budgets and forms/shiny support. These are implementation research and engineering choices within approved behavior. If a required design choice remains that cannot be resolved without changing the approved behavior, report the blocker and seek owner decision.

## 9. Codex delivery

Implement exact owner-approved behavior after spec review/merge. Open an implementation PR. Supply file list, tested release baseline, sprite coverage matrix, licenses, automated outcomes, emulator results, remaining blockers. Never change unrelated project features or falsely declare 252/252 complete with missing graphics.

## 10. One-line Codex handoff

`Implement implementation/selectable-following-pokemon.md from main following AGENTS.md. Reuse and audit existing FireRed/Emerald follower code and sprite assets, satisfy all 252 approved species and exact selection/faint/box/opening rules, test, and open a separate PR. Report blockers without guessing.`


## Appendix A: Candidate reuse audit and preimplementation decision gate (2026-11-05)

This is a source-verified comparison of candidates, NOT a claim that any can be merged into FireRed unchanged:

| Source | Verified evidence | Engineering consequence |
| --- | --- | --- |
| `chale1342/pokefirered` | `src/follower_helper.c`, `data/scripts/follower.inc`, `include/follower_helper.h` and save/load adaptations exist. The inspected `src/data/object_events/object_event_graphics_info_followers.h` defines **nine** Kanto starter/evolution graphics structs, not a verified full Dex. | Strong FireRed behavioral starting point, but NOT independently proven a 252-art source. Trace its transitive dependencies and license. |
| `EmceeMikey/pokeemerald` | README explicitly describes `followers` for all 386 Gen I-III species and recommends `followers-expanded-id` for expanded overworld graphic IDs and compressed graphics. https://github.com/EmceeMikey/pokeemerald | More promising all-roster artwork/large sprite resource reference; this is Emerald, so port carefully. README statement is not a completed per-252 asset/license audit. |
| `Kasenn/pokeemerald-expansion-kasen` | Public Emerald Expansion variant; Team Aqua's feature directory describes preferred-follower party-menu selection, but its **existing faint behavior falls back to first conscious party member**. https://github-wiki-see.page/m/TeamAquasHideout/Team-Aquas-Asset-Repo/wiki/Feature-Branches | Reuse UI patterns only with the explicitly approved Lightning Yellow override: faint stays assigned but hidden, NO fallback. |

**Preimplementation gate:** Codex should first create a bounded, read-only feasibility inventory before bulk imports: (a) derive exact approved 252 roster, including edge forms; (b) map 252 expected sprite sets and per-source provenance/permission; (c) evaluate reused FireRed follower engine hooks; (d) check whether 8-bit object graphic IDs, palette and VRAM limits need expanded handling; (e) locate current save allocations and existing PC/party/event hooks; (f) locate current Pikachu gift and post-Parcel Eevee gift scripts and their actual integration status. Record blockers in implementation PR before committing massive graphics or migrating save fields. If the discovered path requires a new gameplay rule, report for owner approval instead of selecting it independently.

**Recommended code implementation increments within the single approved feature:** basic following with Pikachu and persistent instance identity; explicit party-menu assignment and no fallback; opening gift/Parcel-Eevee integration; robust transitions/faint/box/evolution/save; complete 252 graphics mapping; all-species validation. These are engineering milestones, not a reduction of approved final 252 scope.

**Release gate:** Automated tests, real emulator coverage, full 252-art coverage and provenance verification must all be reported separately. A working Pikachu demo is not completion of the all-252 feature.
