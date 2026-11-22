# PKM-005 / PKM-006 / PKM-009 implementation-first closeout (2026-11-21)

## Owner policy
Owner performs gameplay playtesting. Do not run builds, emulators, tests, or broad CI for this implementation pass. Never translate deferred testing into gameplay acceptance. This document records actual source changes separately from unverified design/source reconciliation.

## PKM-005 — implementation complete, hardware acceptance deferred
Existing implementation and historic scoped acceptance are recorded in project-tracking.json. Owner directs implementation closeout; real peer link-trade/hardware acceptance is a separate playtest/release checklist item, not a production implementation blocker. Existing earned 7/remaining 1 retained pending formal point-policy decision; no points retroactively awarded.

## PKM-009 — concrete approved production correction
The default-branch source read on 2026-11-21 showed Pichu ThunderShock at level 1 and Pichu evolution at level 20. Owner's latest locked requirements: ThunderShock at level 9; Pichu evolves to Pikachu at level 15. This branch updates only the Pichu natural learnset entry and Pichu evolution entry. No Pikachu/Raichu evolution edits. No build or runtime acceptance claimed.

Previously recorded source integration: 146 adopted natural arrays and 14 locked profiles. Historical TM/HM source contract: all 252 regional species and 5,831 original TM/HM memberships across TM01–TM50 and HM01–HM08. This is not proof that every new machine, especially TM51/TM52, is complete. Owner approved 22 TM compatibility decisions in recent conversation; exact species-to-machine rows must be retrieved from canonical approval before further edits; do not infer or fabricate a matrix. TM51 Zap Cannon compatibility and reward integration remain independently reconciled; do not overwrite the TM52 work or original 58-machine compatibility bits.

## PKM-006 — outstanding roster compliance
Historic audit recorded 281 authored off-Dex occurrences. This is an occurrence count, not 281 species or automatically 281 production fixes. The audit guard is merged. Current production-role review and approved replacements must be compared with latest source, including ordinary trainers, special encounters, wild ecology, scripts, and harmless reference-only occurrences. No off-Dex exceptions authorized. Do not modify separately owned WRL-002/009, GYM, or stationary-overworld work without reconciling ownership. No claim that 281 are still live in current main or all corrected.

## Accounting and next implementation queue
Preserve 50 permanent IDs, 461 earned / 251 remaining / 712 total until evidenced accounting acceptance. PKM-005 is implementation-complete with external acceptance deferred; PKM-006 remains source-policy reconciliation open; PKM-009 remains source reconciliation open. Next: retrieve exact approved 22 compatibility rows, reconcile current TM51/TM52 code and Pichu source, enumerate current off-Dex role occurrences against latest main and ownership, then implement only approved unowned corrections. No tests, builds, emulators, merges, or release claims.

## 2026-11-21 follow-up: exact owner-approved TM51 species matrix

The exact owner approval is TM51 Zap Cannon compatibility for Pichu, Pikachu, Raichu, Magnemite, Magneton, Voltorb, Electrode, Elekid, Electabuzz, Jolteon, Mareep, Flaaffy, Ampharos, Electrike, Manectric, Zapdos, Raikou, Porygon, Porygon2, Forretress, Staryu and Starmie (22 total); Mew explicitly excluded. This PR stages exactly 22 source eligibility entries in tmhm_learnsets.h and aliases reserved ITEM_15B (347) as TM51; TM52 remains item 348. These edits alone do not implement a functional TM item, its TM Case/move mapping, or Surge reward. No build or test executed. Do not duplicate the separate private implementation commit 71ea8dca5f3c4aed5bfcf93a73649cff94ab0211 without comparing actual source; that commit was not fetchable from the public repository.

## PKM-006 historical inventory classification (read-only, no audit run)

The committed unresolved inventory has 572 total findings, divided into 281 historical authored off-Dex assignments (199 wild, 24 ordinary Trainer, 54 Trainer Tower, four direct event scripts), 279 legacy Battle Tower references, eight script species-variable review candidates and four C reference candidates. This is the old audited-base snapshot, not a fresh count of current source. The Water Labyrinth direct Togepi egg finding is demonstrably stale: current script now refers to MAGBY in its post-egg species text and no longer contains TOGEPI. Conversely, current Birth Island and Navel Rock scripts still contain DEOXYS and LUGIA event assignments, so they are not universally resolved. Trainer and wild counts cannot be asserted current without comparing current tables. No off-Dex species exception is granted. Source changes to wild/Trainer Tower/ordinary Trainers are deferred pending precise current mapping and ownership with WRL/GYM specialists.

## Follow-up: source-only TM51 implementation staged in PR288

The branch now supplies TM51 item data in existing item slot 347, Electric TM icon, party menu move ID, TM Case numeric display, teach/consume paths, and the approved 22 eligibility bits. Lt. Surge's existing Double leader script gives TM51 instead of TM34, with a new persistent FLAG_GOT_TM51_FROM_SURGE at previously unused 0x2E7, preserving FLAG_GOT_TM34_FROM_SURGE for old saves. Existing players who already claimed the former TM34 reward can claim TM51 on revisit. The approved Route6 TM34 placement and TM52 ID348 remain untouched. No test/build/emulator/merge executed; functional behavior is not accepted until owner playtest. The separate private implementation hash was not available in the public GitHub repository, so no claim of exact equivalence to private code.

## Read-only current-world roster comparison (not an audit-script run)

Compared the committed historical 572-finding snapshot against source at the PR288 branch. The approved 252-entry custom Pokedex roster was used for eligibility membership.

* Wild FireRed source: 2,176 slots inspected; 148 off-Dex slots remain. Historical 199 off-Dex positions include 51 since changed to approved species and 148 still off-Dex; no new off-Dex slots found outside those historical positions. The 148 are not blindly modified because WRL-002/009 owns ecology and per-map weights/levels.
* Ordinary Trainer party definitions: 644 parsed static party blocks; all 24 historical off-Dex assignments absent and zero current off-Dex species in these parsed blocks. This is a source-table comparison, not proof of all runtime parties.
* Trainer Tower: 54 current off-Dex species entries remain, matching historical count. No approved exact replacement mapping was found in the inspected records; changing species would silently change gameplay balance.
* Direct event scripts: current Water Labyrinth egg uses MAGBY, superseding historical TOGEPI. Birth Island DEOXYS, Navel Rock LUGIA and HO_OH remain; defer to stationary encounter owner and approved special policy. Legacy Battle Tower 279 and 12 script/C reference candidates require separate runtime-role classification, not automatic species substitution.

This source comparison reduces demonstrated current authored off-Dex assignments from historical 281 to 205 (148 wild + 54 Trainer Tower + 3 direct events), within inspected categories. It does not certify global eligibility, external/dynamic dataflow, or hardware. PKM-006 remains implementation-open for approved replacements; the historical audit snapshot is intentionally unchanged for reproducibility. Existing 461/251/712 accounting is unchanged.


### 2026-11-21 owner-approved delegated Trainer Tower replacements

Owner approved equivalent 252-species roster substitutions preserving battle role and difficulty. Draft PR288 source changes in src/trainer_tower_sets.c replace all 54 previously off-Dex Trainer Tower species slots, with corresponding nickname changes. Levels, team positions, held items, EVs, IVs, personalities and friendship were left untouched. Two OCTILLERY→SEADRA entries had signature OCTAZOOKA replaced with WATER_PULSE. No build/test/emulator/merge or gameplay claim.

Exact source replacement map (original → approved roster substitute): TOGETIC→CLEFABLE; FURRET→RATICATE; GIRAFARIG→HYPNO; TOGEPI→CLEFAIRY; SNUBBULL→JIGGLYPUFF; SHUCKLE→ONIX; GLIGAR→AERODACTYL; QUAGSIRE→SLOWBRO; SKARMORY→STEELIX; MANTINE→LAPRAS; JUMPLUFF→BELLOSSOM; LEDIAN→BUTTERFREE; MEGANIUM→VENUSAUR; DUNSPARCE→KANGASKHAN; FERALIGATR→BLASTOISE; GRANBULL→TAUROS; CORSOLA→STARMIE; AZUMARILL→POLIWRATH; CHINCHOU→STARYU; WOOPER→SLOWPOKE; TOTODILE→SQUIRTLE; LANTURN→VAPOREON; OCTILLERY→SEADRA; QWILFISH→TENTACRUEL; REMORAID→HORSEA.

Read-only source inventory after this change: 148 wild ecology slots and three direct legendary event assignments remain demonstrated authored off-Dex occurrences in inspected categories (151 total); ordinary Trainer and Trainer Tower parsed source assignments now zero. The historical snapshot remains immutable. Runtime and move legality/competitive balance of replacement parties require owner review. 279 legacy Battle Tower references and 12 script/C candidates still require role classification. Preserve 50 IDs and 461/251/712 until evidence-based acceptance.


### 2026-11-21 PKM-006 protected wild-map and dynamic reference classification (read-only)

The 148 current off-Dex ordinary wild source slots are restricted to 12 maps: seven Seven Island Tanoby Unown chambers (84 slots), Six Island Altering Cave event variants (48), Three Island Port Dunsparce (12), Five Island Memorial Pillar (2), Five Island town (1), and Seven Island Trainer Tower exterior (1). The canonical WRL-002/009 specialist handoff explicitly safeguards Tanoby glyph populations, Altering Cave event variants, Three Island Port, and the Memorial Pillar/Trainer Tower coastal Surf/fishing tables; exact source replacements for these map-method tables were not confirmed as approved. No wild table was edited. Preserving existing rates/levels alone would not preserve encounter mechanics or habitat identity. This is an ownership/design hold, not permission for off-Dex exceptions.

Direct authored story events: Birth Island DEOXYS Lv30, Navel Rock Base LUGIA Lv70 and Navel Rock Summit HO_OH Lv70 remain active seteventmon scripts; replacing species alone would break their cries, sprites, narrative, flags and post-battle outcomes. They are separate from the four approved post-League Lv50 Raikou/Entei/Suicune/Mew statics. No SOW/story-authorized replacement found; no event scripts changed.

**Correction to earlier legacy Battle Tower shorthand:** 279 off-Dex template entries (148 in src/data/battle_tower/level_100_mons.h, 131 in level_50_mons.h) are selected by src/battle_tower.c when building opponents for level-100 or level-50 challenge types. They are active production code data, not automatically harmless references. Reachability in this game is not established by this source review. This is additional to the 151 unresolved authored wild/story slots, yielding **430 potential off-Dex production assignment slots** if Battle Tower is accessible. No blind 279 replacements were made because the full trainer template fields/team flags and challenge eligibility must be preserved under an owner-approved mapping.

Eight script species-variable review candidates: old Water Labyrinth TOGEPI/TOGETIC comments are superseded by MAGBY/MAGMAR checks; the remaining DEOXYS/LUGIA/HO_OH species-variable references are post-battle/fled messages tied to the three still-active legendary encounters, not independent extra encounters. Four C references: wild_encounter.c SPECIES_UNOWN is the special Unown glyph generation branch, not a separate species placement; battle_setup.c DEOXYS/LUGIA/HO_OH are battle transition music/effects cases, not extra species assignments. Do not delete these merely to lower inventory counts.

No build, tests, emulator, workflow dispatch or merge. Source-only documentation and canonical tracker corrected, 50 task IDs and 461 earned/251 remaining/712 total preserved.


### 2026-11-21 — PKM-006 distinct Battle Tower source inspection and approval gate

Exact 279-row per-template inventory added: `docs/PKM006_BATTLE_TOWER_279_SOURCE_INVENTORY_20261121.md`. The level-50 header has 300 entries / 131 off-Dex; level-100 header 300 / 148 off-Dex. `src/battle_tower.c` includes both arrays, selects by level type, picks from tier-dependent index windows, filters teamFlags and duplicate species/items and constructs a three-mon opponent party with fixed tier IVs, template EV spread, nature, held item and four moves assigned directly. Therefore 279 are actual code-backed candidate templates, not merely harmless references. Facility game-world reachability remains unconfirmed without gameplay execution.

The owner-delegated equivalent species approval applied to **54 Trainer Tower** entries only. Battle Tower is separate; inspected canonical wild ecology/SOW and PR ownership sources do not provide an exact 279-slot replacement matrix or explicit extension of authority. No Battle Tower template source edits made. Inventory records all indices, species, flags, items, EV spreads, natures and moves, with replacement pending. Safe future implementation must retain 300 slots/pool, original order/flags/item/EV/nature and difficulty-tier access; examine each four-move set for legality/role and ensure eligible distinct candidates for the selection loops. E-reader/record mixing require separate policy. No tests/builds/emulators/CI/merges. Wild 148 protected WRL and 3 SOW/story legendary events untouched. 50 IDs, 461/251/712 unchanged.
