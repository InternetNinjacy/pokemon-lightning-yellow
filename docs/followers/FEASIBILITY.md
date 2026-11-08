# Selectable followers — required preimplementation audit

WRL-010 · 2026-11-06 · audited main `69c8240654e742603d9868717c03c204f4bdacf7`.
Contract: [implementation/selectable-following-pokemon.md](../../implementation/selectable-following-pokemon.md).

**BLOCKED for external reuse; gameplay is not implemented.** This PR records the
contract's bounded inventory before bulk graphics imports or save migration.
No external code/art, ROM, save, new Pokémon, or production changes are included.

## Sources, coverage and permissions

Pins and counts: [source-inventory.json](source-inventory.json).
Every approved species, in custom order: [252-species-coverage.csv](252-species-coverage.csv).
The matrix is an inventory of exact paths/blob IDs, not a walking-animation or
emulator certification. All 252 rows require further frame/palette and runtime tests.

| Source / pinned commit | Verified result | Reuse decision |
|---|---|---|
| chale1342/pokefirered `d3ba8da3ca29ef6ad24684219998fa65e53aa1aa` (`followers`) | FireRed source contains `src/follower_helper.c`, `include/follower_helper.h`, `data/scripts/follower.inc`; `src/data/object_events/object_event_graphics_info_followers.h` defines nine Kanto starter/evolution graphics structs. The helper contains extensive conditional/emotion messages, not a self-contained movement engine. | Useful FireRed integration reference; no repository-wide license/explicit follower permission located in README/tree. Tool LICENSE files do not license this feature or artwork. Do not copy wholesale. |
| EmceeMikey/pokeemerald `70341382c071a7cb46de4e8d071f3eb56ec35d60` (`main`) | Current branch API lists seven branches; neither `followers` nor `followers-expanded-id` exists there. Current main README is the ordinary Emerald decomp README. | Historical handoff description is not reproducible at this current pin. No complete follower source verified here. |
| aarant/pokeemerald `acc75ef3fec2a952ac6a423b67dc6bea0f0014c1` (`followers-expanded-id`) | 252/252 matching PNG paths; species-indexed graphics table, dynamic palettes and compressed graphics in `src/event_object_movement.c`. README claims 386 including forms/Shinies. | **Explicit permission required**, below. No code/art imported. |
| Kasenn/pokeemerald-expansion-kasen `cbe04a7a2ef645bee19d159876da074154ccf1f2` (`toggle_follower_from_party_menu`) | 252/252 base sheets plus 252 normal and 252 Shiny palette paths; follower pic-table/graphics-table entries. README credits aarant's Followers & Expanded IDs. | Wider later-generation expansion is not FireRed compatible by default. Downstream credit/permission claim does not establish permission for this project. No import. |

The precise upstream permission gate is in
[aarant README lines 143–152 at the inspected pin](https://github.com/aarant/pokeemerald/blob/acc75ef3fec2a952ac6a423b67dc6bea0f0014c1/README.md#L143-L152):

> **Please ask for permission** before using any of these branches for a project outside the `pokeemerald` ecosystem.
>
> **Please ask for permission** before using any of these branches in a project reliant on AI assistance.

Lightning Yellow is FireRed based and uses AI assistance, so both conditions apply.
The same section asks that issues/PRs contain no AI-produced text/code. **Do not
submit an AI-written permission issue to that repository.** The owner can contact
the maintainer personally through an acceptable channel, seek permission covering
this FireRed/AI-assisted project and the specific code/assets, and supply the grant.
If another artist owns some sheets, their applicable permission/provenance is also
needed. An independently licensed complete sprite set is an alternative. Public
visibility, GitHub's license API returning 404, or downstream credit alone is not a
grant. This audit is not a legal ownership determination.

Exact art gaps: zero missing *base paths* in the two broad candidates. Conversely,
**all 252 lack verified permission for import here**; all 252 lack completed
per-sheet direction/frame/runtime verification. Do not call this “252 usable followers.”
Only 36 matching local Pokémon PNGs exist. Existing Pikachu is 16×16 and its nine
frame records repeat only three unique images (`src/data/object_events/object_event_pic_tables.h:1672`),
so a standard animation pointer does not prove a full directional follower sheet.
Existing local sheets do not independently solve all-252 coverage.

### Forms and Shiny boundaries

The actual approved roster includes Castform #233 but excludes Unown, Deoxys and
Spinda. Do not import all386 or later-generation families. Kasenn has Castform base,
`sunny`, `rainy`, `snowy` sheet/palette paths and corresponding pic-table entries.
These are candidate form assets, not authorization to introduce persistent
weather-form evolution or new field weather behavior. All252 base Shiny palette
paths exist there, but matching indexed pixels/direction sequences and FireRed
palette loading remain unverified. Normal and Shiny instances must retain identity.

## FireRed integration findings / proposed architecture

**SOURCE VERIFIED:** `NUM_OBJ_EVENT_GFX` is152, saved/runtime graphics IDs are8-bit,
variable IDs occupy240–255 (`include/constants/event_objects.h:159`). Object budget
is16 (`include/constants/global.h:41`). Adding252 ordinary IDs is unsafe. Upstream
Emerald expands IDs/structs and adds dynamic palettes; copying that changes saved
object/quest-log ABI and is not a drop-in port.

**PROPOSED:** preserve those ABIs and reserve a single runtime follower graphics ID,
resolve species/form/Shiny through a ROM-side descriptor lookup, load one follower
palette/frame set at a time. Fail safe without evicting NPCs when object/OAM/tile or
palette capacity is exhausted. Verify reflection, shadows, weather tints, doors,
ledges, elevations and scripted movement before claiming this approach works.
Movement hooks belong in `src/event_object_movement.c`, `src/field_player_avatar.c`,
`src/overworld.c`, and field input/script interaction modules. Existing standard
NPC animation tables alone are insufficient.

**SOURCE VERIFIED:** `include/global.h:828` retains `unused_3A94[64]`; repository
search found no consumer. `unused_3D24` already stores weather state; Route5 daycare
and legacy recovery fields are allocated. **No space is claimed by this audit.**

**PROPOSED:** separately ABI/checksum-test a versioned follower record inside the64-byte
candidate region, without moving fields. PID+OTID can survive reorder/nickname/
evolution but is not sufficient for identical cloned identities; deterministic
instance tracking/removal hooks must resolve ambiguity to none, never another mon.
Keep empty/invalid/legacy states unassigned. Test fresh and older saves before
migration. Do not expand `struct Pokemon`, save blocks or serialized quest-log
objects as a shortcut.

Selection and Stop entries belong in `src/party_menu.c`; leave battle lead/order
alone. Hook removal at PC deposit/release (`src/pokemon_storage_system_data.c`,
including `CompactPartySlots`), daycare deposit (`src/daycare.c`), trade completion
(`src/trade.c`), and scripted party removal. A scan only on returning to the field
would miss deposit/withdraw within one PC session. Reorder/evolution update the
same chosen instance; faint only suppresses rendering; no first-live fallback.
Upstream aarant `GetFirstLiveFollowMon` scans for the first conscious party mon;
that behavior directly conflicts with this contract and must not be reused.

## Opening events: real dependency, not an implemented feature

**SOURCE VERIFIED:** Oak's Lab still sets Bulbasaur/Squirtle/Charmander at lines
1074–1077,1217–1233 and gives the selected species at line1124. The Parcel scene
starts near603 and contains no Eevee gift. No Pikachu/Eevee identifiers occur in
that lab script. Rival handoff section5 explicitly preserved Kanto starter
progression and deferred the separate Pikachu-player feature.

The follower contract permits narrow integration into approved gifts; it does not
supply a replacement complete opening story/levels/coordinates. Do not silently
replace all three starter scripts or manufacture an Eevee level, dialogue, scene,
full-party policy or rival routing. Locate the approved opening-event handoff in
supplied authority exports, reconcile it with current scripts, and implement that
dependency in its own approved scope. If no exact event specification exists,
request only those missing event details. Manual follower UI/state work can be
independent; final opening acceptance cannot pass until gift integration exists.

## Resume plan and acceptance gates

1. Resolve explicit code/art reuse permission or identify an independently permitted
   complete replacement; pin source and preserve notices. Inspect all252 sheets'
   dimensions, frame ordering, palettes and direction coverage, plus Castform forms.
2. Establish save ABI/reserved-region ownership and identity tests, including duplicates,
   all removal paths, faint/heal, reorder/evolution and old invalid saves.
3. Implement party UI and small runtime movement/graphics resolver, then grow the
   validated whitelist to all252. Do not ship a Pikachu demo as completion.
4. Resolve exact approved opening gift dependency; one-time Pikachu emergence, Eevee
   manual selection only; both avatars and all relevant party-capacity paths.
5. Automated lifecycle/save/252-lookup checks, clean/reproducible build, all existing
   20 host suites/3 checkers and full native mGBA regression; add follower native tests.
6. Emulator maps/movement/interaction screenshots: all252 graphics smoke checks;
   small/large/flying/water/baby/evolved representatives; NPC-full maps; palette/OAM
   limits; stairs/doors/ledges/warps; bike/Surf; script/battle/faint/heal/PC/trade;
   save/load and quest log. Real peer/hardware remains separate if unavailable.

## Evidence and limitations

`python3 scripts/check-follower-inventory.py` validates the252-row inventory against
current custom roster, uniqueness/order, pinned blob identifiers, complete candidate
paths, and explicit unverified status. It is **not a battle or follower runtime test**.
`python3 scripts/render-project-dashboard.py` regenerates live tracking.
No production code changed; no follower ROM build or emulator test is claimed.
Consolidated main's independent full build/native regression run:
https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42213317755
(check its recorded conclusion before treating it as passed).

Rollback: this audit-only feature branch/PR can be closed without changing gameplay
or save data. Prior merged consolidation is recoverable through Git merge history
and tag `recovery/main-before-consolidation-20261106`. Follower implementation stays
open for review; no auto-merge or ROM release.

## 2026-11-08 continuation — owner reports permissions resolved; asset verification still pending

**Owner update:** The project owner stated on November 8, 2026: “Permission was resolved. Perfect, lets continue.” Treat this as owner confirmation that the previously raised permission matter has been resolved; do not require the owner to repeat the same decision. This audit does not possess the written grant, the particular licensed sprite source, or a contributor-by-contributor scope inventory, and consequently does not independently certify redistribution rights. Preserve original notices and confirm coverage of chosen assets before import. No artwork or code was copied in this continuation.

**Pinned candidate sampled:** `Kasenn/pokeemerald-expansion-kasen` at `cbe04a7a2ef645bee19d159876da074154ccf1f2`. Direct connector reads confirmed 16 RGB palette entries with `JASC-PAL / 0100 / 16` headers for normal and Shiny `bulbasaur`, `pikachu`, `eevee`, `castform`, and `wailord` (10 palettes). Also confirmed the six Castform alternate-form palettes at:
- `graphics/pokemon/castform/sunny/overworld_{normal,shiny}.pal`
- `graphics/pokemon/castform/rainy/overworld_{normal,shiny}.pal`
- `graphics/pokemon/castform/snowy/overworld_{normal,shiny}.pal`

**Important:** This is sixteen sample palette-file reads, **not** a 252/252 palette-content validation, image/frame verification, pixel-to-palette crosscheck, or conversion/ROM test. The prior inventory's 252 normal/Shiny *paths* remains unchanged. Unown and Spinda palettes were also readable, but those species are expressly outside Lightning Yellow's approved roster and are not follower candidates.

**Binary asset access blocker:** GitHub connector's `fetch_blob` on exact pinned Bulbasaur PNG blob IDs for both Kasenn and aarant returned a `UnicodeDecodeError` on the PNG's `0x89` signature, rather than raw image bytes. A container-side Git fetch failed DNS resolution; no bytes were available for PNG dimensions, directions, animation, transparency or palette-index testing. **Do not infer frame completeness from PNG path existence.**

**Gate result:** STILL BLOCKED on independent all-252 binary/frame and FireRed technical verification. Owner-reported permission resolution is acknowledged, but no asset gate PASS is claimed. No follower gameplay, save code, conversion import, PR merge or effort-point award occurred. Resume from this existing matrix once binary PNG read/download is available; validate 252 images and all applicable forms, palette indices, animation sequence, budgets and conversion in isolation before any gameplay coding. The preexisting matrix's permission column records the historical November 6 audit and is not newly adjudicated by this addendum.

## 2026-11-08 binary access correction — working retrieval method

The earlier statement that PNG bytes were inaccessible is **superseded**. The connected GitHub `fetch_file` action succeeds on pinned PNGs when called with `encoding: "base64"`; its default `utf-8` decoding fails because PNG is binary. The returned Base64 begins `iVBORw0KGgo` (PNG signature). This is a viable binary asset acquisition route and does **not** require asking the owner for an archive.

- Pinned source: `Kasenn/pokeemerald-expansion-kasen@cbe04a7a2ef645bee19d159876da074154ccf1f2`.
- Tested 15 consecutive approved-roster sprite PNGs (#1–15), all downloadable via Base64, all exact Git blob SHA matches against the existing `252-species-coverage.csv`, and all parse as indexed PNG 192×32 pixels, bit depth 4, PNG color type 3.
- Tested twelve additional spread-roster samples (#1, #25, #50, #75, #100, #125, #150, #175, #200, #225, #233 Castform, #252 Mew); all exact blob SHA matches and the same 192×32, 4-bit indexed PNG format. Sample #1 was also in the consecutive group, so **26 distinct approved species** were checked here.
- This establishes reproducible binary access and a preliminary uniform source format. It **does not** certify all252 sheets, actual distinct directional walking frames, transparent index, source image palette matching, Shiny pixel compatibility, GBA VRAM feasibility, game-engine conversion or full 252-runtime acceptance.
- Bulk requests exceeded a single tool-invocation call quota and must be chunked. Direct shell HTTP requests in the execution container failed DNS, but the connected GitHub Base64 route works. No PNG files or artwork were imported into the project repository.

**Gate:** STILL BLOCKED on exhaustive sprites/frame/FireRed verification. Import/gameplay implementation must not begin merely from these successful samples. No progress-point award, merge or save modification.

## 2026-11-08 exhaustive binary audit execution path — committed, not yet green

A reproducible **all-252 actual-PNG decoding** script is now committed at `scripts/verify-follower-assets.py`; the isolated `.github/workflows/follower-assets.yml` workflow runs it on this follower branch/PR. This supersedes the previous technical-access limitation and uses the existing exact 252-species CSV and pinned Kasenn commit. The validator downloads each original PNG and both normal/Shiny JASC palettes to ephemeral runner storage, verifies the exact Git blob SHA for each base sprite, parses PNG CRC and 4-bit indexed image data including decompression/filter reconstruction, records six 32×32 pixel cells and cell fingerprints, and checks Castform's three alternate overworld sprites plus their six palette files. It does **not** commit external artwork or ROMs.

**Evidence status at write:** The script/workflow were committed and read back from GitHub, but no finished GitHub Actions result was returned for the commit in the connector's available pull-request-run query. **Do not report 252/252 pixel/frame success or mark the asset gate passed until the actual workflow run concludes and is inspected.** A passing PNG structural audit would still require mapping the six frame cells to all facing/walking actions, and isolated FireRed conversion/graphics-resource evidence. The owner's permission resolution is accepted as stated; exact redistribution conditions and attribution must be preserved when actual assets are later selected.

Implementation remains intentionally out of scope until all asset-gate criteria pass. No gameplay, save, graphics import, merge or planning-credit changes in this continuation.

## 2026-11-08 additional direct pinned-PNG batch

Direct GitHub connector `fetch_file(encoding="base64")` checks on Kasenn's pinned `cbe04a7a2ef645bee19d159876da074154ccf1f2` source verified approved custom-Dex positions **#17–30**, Pidgey through Sandslash: each returned a PNG byte representation with **exact matching pinned blob SHA**, `192×32` IHDR geometry, `4`-bit indexed pixels (PNG color type `3`). No missing/mismatched paths or header-format exceptions in that 14-entry batch. These are image-header/hash checks only, not complete pixel/animation testing.

The existing verification script and dedicated workflow are present on this audit branch. The connector's commit-run lookup returned **no PR-triggered Actions runs**, not evidence that CI succeeded or failed. Check the actual workflow run conclusion or execute the script in an environment with access to GitHub raw assets before closing the gate. Still do not import assets or implement follower gameplay before full 252 frame/technical readiness and owner-reported permission scope are reconciled.

## 2026-11-08 integration incompatibility confirmed — Kasenn default fallback

The Team Aqua's Hideout feature-branch catalog describes Kasenn's *Set preferred follower* branch as falling back to the default **first conscious party Pokémon** if no preferred Pokémon is selected or the preferred follower is fainted: https://github-wiki-see.page/m/TeamAquasHideout/Team-Aquas-Asset-Repo/wiki/Feature-Branches . This is directly **incompatible** with Lightning Yellow's approved manual-select/no-fallback behavior. **Do not import that selection logic unchanged.** Its sprite collection is being investigated separately from its gameplay implementation. The target will need an independent FireRed-safe instance assignment and hide-on-faint state machine.

At the pinned Kasenn revision, `src/data/object_events/object_event_graphics_info.h` defines one `gObjectEventGraphicsInfo_Follower` with a 32×32 OAM footprint, dynamic palette tag, palette slot NPC_1, and `sAnimTable_Following`. This confirms the upstream intent for runtime dynamics and a six-frame sprite-sheet format but **does not** prove a FireRed port or the animation directions. Source-aware animation table mapping and palette/tile budget tests are still mandatory.

No gameplay code imported. Asset gate remains open.

## 2026-11-08 full custom-roster PNG header/hash verification — 252/252

**Executed via connected GitHub binary/Base64 reads during this session**, independently of the pending GitHub Actions full-pixel test. Used each exact custom Pokédex row's `kasenn_sheet` and `kasenn_blob_sha` against pinned Kasenn revision `cbe04a7a2ef645bee19d159876da074154ccf1f2`. All **252 of 252** approved roster sprite paths returned image data with **matching recorded Git blob SHA**, PNG IHDR readable, 4-bit indexed color type 3. **251** sheets are `192×32` (six `32×32` cells). Exactly **one** is different: **#103 Steelix** at `384×64` (six `64×64` cells). No image identity/availability or IHDR failures were found. Existing `252-species-coverage.csv` was augmented in-place with precise result columns, preserving historical statuses and pinned blob references.

**Alternate Steelix:** The other pre-audited complete source, `aarant/pokeemerald@acc75ef3fec2a952ac6a423b67dc6bea0f0014c1`, has a directly fetched **192×32**, 4-bit indexed Steelix PNG whose blob SHA matches the existing matrix. This is a promising replacement, **not yet an approved or verified interchangeable palette/frame asset**. Source-specific reuse scope is owner-reported resolved but written terms are not held by this technical audit. Do not rescale Steelix or import the alternate until normal/Shiny pixel-index palette mapping and runtime fit are checked.

**Actual source animation mapping reviewed:** Kasenn's pinned `src/data/object_events/object_event_anims.h` maps south frames to cells `0/1`, north to `2/3`, west to `4/5`, and east to horizontally flipped `4/5`. Its `gObjectEventGraphicsInfo_Follower` currently uses a **32×32** sprite object and `sAnimTable_Following`, so Kasenn's 64×64 Steelix sheet is **not compatible with that default graphic record without a deliberate different renderer or replacement**. The source also defines an asymmetric follower animation table with extra east-facing cells, but this 32×32 record points to the ordinary six-cell table. The pinned table contract has been added to the asset validator.

**Isolated conversion gate prepared, not yet executed in CI:** `scripts/verify-follower-assets.py` now reads all PNG chunks and pixels, validates six cells, tracks Steelix as a nonstandard geometry exception, checks 252 normal/Shiny palettes plus Castform variants, validates the source facing table, and performs a reversible GBA 4-bpp 8×8 tile encoding/decoding check for each frame. Expected raw tiles: `32×32` = 512 bytes/frame or 3,072 bytes/six-frame sheet; `64×64` = 2,048 bytes/frame or 12,288 bytes/sheet. These are asset-level uncompressed data sizes; **not measured VRAM/runtime allocations**. The validation script and dedicated workflow are committed, but its complete 252-pixel decoding run and exact-head Actions conclusion were **not observed**; do not call the asset gate PASS based on script existence or header results.

**Correction to earlier addendum:** the earlier batch described as custom-Dex #17–30 used zero-based CSV slicing with an off-by-one displayed label. The actual species-file checks were #16–29; #30 was separately fetched and verified in the final full roster pass. No roster identity was changed.

**Phase One status:** 252/252 source PNGs *exist and have independently matching headers/hashes*; runtime, artwork-frame uniqueness, full palette-pixel compatibility, explicit alternative Steelix selection and resource limits remain open. No follower gameplay code, save change, imported artwork, merge or effort-point award occurred.

## 2026-11-08 Steelix alternative palette incompatibility — exact binary result

Compared the PNG `PLTE` chunks for both pinned Steelix sprite options against Kasenn's `graphics/pokemon/steelix/overworld_normal.pal`. Aarant's hash-matching `192×32` PNG has **16 indexed palette entries**, including magenta placeholders and a distinct color ordering; Kasenn's `384×64` PNG has **9 indexed palette entries**, with the first nine matching the corresponding normal JASC palette positions. Aarant PNG palette colors/index order **do not equal** Kasenn's normal JASC palette. Consequently **do not switch Steelix to the aarant artwork with Kasenn's normal/Shiny palettes without explicit pixel-index remapping and both-palette checks**. This is an actual technical incompatibility, not a license finding or a missing sprite.

The original Kasenn Steelix is six 64×64 cells, i.e., a 2,048-byte uncompressed 4bpp frame versus the upstream standard 512-byte 32×32 frame. An explicit 64×64 dynamic graphics configuration is another possible conversion candidate, but FireRed object/OAM/tile/VRAM/rendering safety has NOT been proved. Neither solution has passed the asset gate; no art was altered or imported.

## 2026-11-08 owner approval and isolated Steelix technical direction

The owner explicitly approved continuation (“Approve it and continue”). Owner previously reported the required external permissions resolved. Approval is recorded on PR #54, without counting an unverified 252-sprite technical PASS or authorizing any merge. The historical permission-status strings in the CSV are preserved for audit provenance; no new request for the owner to repeat their permission conversation is needed.

**Source-level Steelix feasibility improvement:** Current Lightning Yellow `main` already contains `gObjectEventGraphicsInfo_Seagallop` in `src/data/object_events/object_event_graphics_info.h` with a **64×64 sprite**, `.size = 2048`, `gObjectEventBaseOam_64x64` and `gObjectEventSpriteOamTables_64x64`. This demonstrates that a 64×64 source object descriptor and OAM configuration exist in this FireRed tree. It does **not** prove that the follower subsystem can dynamically load or safely display six 64×64 Steelix frames at all maps/VRAM budgets.

**Preferred isolated verification path:** Keep the original same-source Kasenn Steelix artwork at 384×64 and test a 64×64 dynamic follower descriptor in isolation, rather than mixing the differently indexed aarant Steelix palette without conversion. This is a test direction only, not a new gameplay feature. Keep the 32×32 implementation for the other 251 species and do not silently scale, crop or replace Steelix.

**Additional audit tests committed:** The pinned asset validator now logs embedded PNG palette indices and checks their normal/Shiny JASC palette compatibility, records potential quantized normal-palette discrepancies, checks reversible GBA 4bpp tile packing, and verifies the existing FireRed Seagallop descriptor as a source precedent. New offline deterministic synthetic-PNG tests exercise 32×32 and 64×64 encoding, color-index rejection, PNG corruption rejection and JASC parsing; the isolated asset workflow runs them before attempting network downloads. No executed CI proof has yet been obtained for the new test head. Retain the asset gate as BLOCKED until full 252 pixel/frame test and runtime resource conversion are actually demonstrated.

**No artwork imported, no gameplay/save-system edits, no merge, no earned-point changes.**

## 2026-11-08 live CI recovery, 252 pixel decode, and Steelix gate (verified)

**Branch reconciliation:** As of this checkpoint, follower branch common ancestor is `69c8240654e742603d9868717c03c204f4bdacf7`, and current `main` is `90d0811b46bfab6f054d9b9c9b9c64240b6f3562`. The follower branch is **545 commits behind main and 24 commits ahead** before the latest audit additions. Never merge stale snapshot tracking or production code into main without an explicit, separately reviewed reconciliation. No rebase/merge was performed.

**CI discovered:** The old `fetch_commit_workflow_runs` connector filters only pull-request-triggered runs, so it hid *push*-triggered follower CI. GitHub's authenticated Actions run collection and job logs provided actual executed evidence. On [run 42530264122](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42530264122), the **252/252** pinned Kasenn base PNGs decoded, all normal/Shiny palette files parsed, GBA 4-bpp six-frame tile conversion ran, and all six indexed Castform weather-form palettes and alternate sprites passed after 8-bit-input support; result: `SUMMARY verified=252/252 failures=0 oversized=1`. The workflow intentionally exits **1** because dynamic 64×64 Steelix runtime handling has **not** passed. This exit is not a PNG corruption finding.

**Rainy Castform:** `graphics/pokemon/castform/rainy/overworld.png` is an **8-bit indexed PNG**, 192×32, with **17 PLTE entries but only indices 0–15 used**. An independent Pillow decode of its exact pinned Base64 bytes confirmed 16 used color indices, so conversion to 4-bit indexed pixel values is lossless and requires no image color-index remapping. The validator was corrected to accept 8-bit source indexed images *only when pixels actually fit the GBA 0–15 index range*; another source using index 16+ fails safely. The pinned 252 **base** PNGs remain native 4-bit indexed, including base Castform.

**Exact machine-readable CI evidence:** [run 42530585991](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42530585991) uploaded the **text-only** artifact `wrl010-follower-audit-metadata`, available from GitHub Actions. The downloaded JSON documents all 252 base species, SHA-1 Git blob identity, 4-bpp conversion hashes, dimensions, distinct frame counts, palette mismatches, and forms, without redistributing sprite art or ROMs. Its successful content-level checks are separate from the intentionally blocked dynamic Steelix gate.

**Frame uniqueness:** Exactly two base sheets have fewer than six distinct 32×32 frame cells: **Abra #69 (5 distinct)** and **Glalie #241 (3 distinct)**. Direct visual inspection of these pinned sheets shows sensible directional pose groups, including repeated Glalie stationary/hovering images; duplicate frames by themselves are not corruption or a blocker. Live sprite animation/emulator visual quality still untested.

**Palette mismatch triage:** Embedded PNG PLTE colors initially differed from `overworld_normal.pal` at used indices for **Weedle #13, Slowbro #86, Kingler #107, Scyther #135, Kabutops #160, Castform #233, Absol #239**. Exact RGB comparisons confirmed that **all seven** differing PNG indexes match `overworld_shiny.pal` instead. This is a *Shiny-embedded source palette variant*, not evidence that the JASC normal palette lacks indexed colors. Preserve both provided variant palettes; never copy arbitrary PNG PLTE blindly as the normal palette.

**Transparency limitation:** All 252 indexed PNGs lack an explicit `tRNS` alpha chunk. GBA sprite transparency convention ordinarily uses palette index 0, but source PNG metadata alone does not certify that their background pixels use index 0. An explicit corner-index audit has been added; inspect its executed report and actual art before treating transparency as proven.

**FireRed runtime-specific safety blocker:** Current `main` includes `gObjectEventGraphicsInfo_Seagallop` with **64×64**, **2048 bytes/frame**, 64×64 OAM/subsprite tables, so hardware graphics primitives exist. However the ordinary `ObjectEventSetGraphicsId` implementation in `src/event_object_movement.c` frees old sprite tiles and retries `AllocSpriteTiles` with the **old tile count** if the larger new allocation fails, without demonstrating a fail-safe 32→64 switch. A new dynamic follower implementation must **not** blindly call that path for size increases. It must acquire enough tile memory, correct OAM and palette ownership, explicitly handle failure by hiding (not clearing) the selected follower, and safely transition back to 32×32. None of those behaviors has been proven in a compiled ROM or native emulator.

**Observed status:** Base/all-form binary pixel and tile conversion PASS; Steelix dynamic 64×64 graphics/loading test NOT EXECUTED; 252 runtime follower rendering NOT IMPLEMENTED. Thus **WRL-010 PHASE ONE — ASSET GATE BLOCKED** despite successful asset-level tests. No earned points, gameplay/save code, sprite imports or merges were authorized by this progress.


## 2026-11-08 native mGBA graphics allocation AND 32/64 OAM render-object feasibility PASS (scoped)

**Isolated ROM-native graphics proof executed and PASS:** [GitHub Actions run 42532517738](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42532517738), audit branch `1bd50e16b04a383c37826d8140f46c3d9d6d633b`, **current production main** `90d0811b46bfab6f054d9b9c9b9c64240b6f3562`. Runner installed pinned agbcc and compiled the current FireRed ROM/ELF in an ephemeral checkout, booted **600 emulated frames** using libmGBA, injected ARM calls into original ROM routines, and executed **361 focused native graphics memory/OAM assertions** successfully. All build, native test and checkout verification steps completed successfully, with no ROM/artwork uploaded. No production code, save layout, overworld follower logic or existing ROM assets were modified or committed.

**Precisely what the passed native test proved:**
- Real ROM `ResetSpriteData` + `AllocSpriteTiles(16)` and `AllocSpriteTiles(64)` succeeded simultaneously; first blocks at tile **0** and **16** on reset graphics memory.
- Actual `SpriteTileAllocBitmapOp` confirmed allocations occupy distinct correctly sized contiguous tile ranges (32×32 = 16 tiles, 64×64 = 64 tiles).
- Exhaustion in this isolated reset environment occurred at **16 simultaneous 64-tile allocations**, with deterministic `-1` from additional 64-tile or 16-tile allocation and no bitmap corruption after reset/reallocation.
- ROM `CreateVirtualObject` created two *real engine sprite objects* using existing native art, **Snorlax** (32×32, gfx ID 109) and **Seagallop** (64×64, gfx ID 108), simultaneously. Tests accessed `gSprites` and compiled ARM ABI `sizeof/offsetof` to check native **OAM size code 2** (32×32) and **size code 3** (64×64); checked their non-overlapping 16-/64-tile allocation ranges and invoked native `BuildOamBuffer`.
- Native `DestroySprite` of each object released exactly the tile range; native recreation of 64×64 Seagallop succeeded and released resources again.

**Reproduction:** `tests/followers/inject_native_graphics_probe.py` inserts a test-only probe into the disposable current-main checkout's `tests/emulator/runner.c` and ABI symbol generator; `.github/workflows/follower-native-graphics.yml` builds current main then runs `LIGHTNING_FOLLOWER_GFX_ONLY=1 python3 tests/emulator/run.py`. See exact CI run and logs rather than relying on historical generic ROM suites.

**Important boundaries / pending:** This **PASSES the isolated emulator graphics-engine allocator, 32/64 object creation/OAM, and cleanup feasibility subgate**. It is **NOT** a proof that the Kasenn Steelix PNG itself was rendered on-screen, that a genuine 252-species dynamic graphics loader has been implemented, that its six directional frames animate correctly, that normal/Shiny palette swaps or reflections are correct, that object slots and sprite VRAM remain safe on crowded production maps, or that 32↔64 change through vanilla `ObjectEventSetGraphicsId` is safe. The old-size allocation fallback in that routine remains a real hazard and must **never** be used without a size-aware failure-atomic replacement. Run game-map/actual-art/native dynamic loading tests before declaring the entire WRL-010 asset gate PASS.

**Result:** **native engine graphics RESOURCE feasibility PASS (isolated)**; **WRL-010 PHASE ONE — ASSET GATE BLOCKED (full integration conditions unproven)**. The complete 252 PNG/4bpp asset validation remains separately PASS. No gameplay changes or earned-point awards.

## 2026-11-08 real pinned Steelix 64×64 emulated OBJ graphics + palette test — PASS (1,402 native assertions)

**ACTUALLY EXECUTED PASS**: [WRL-010 native graphics CI run 42533391116](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42533391116) at feature commit `875cfd7684295ff6ad7fc78bf0ed9e17265171cb`, temporarily built from current main `90d0811b46bfab6f054d9b9c9b9c64240b6f3562`. GitHub Actions installed agbcc, built real FireRed `.gba` and `.elf`, booted **600 frames in libmGBA**, and completed **1,402 focused native ROM/emulator assertions**. All workflow steps passed. No ROM, sprite or save-state was committed or uploaded.

**Proven in this isolated emulator test (not merely source inspection):**
- The temporary `tests/followers/prepare_steelix_native_frame.py` fetched `Kasenn/pokeemerald-expansion-kasen@cbe04a7a2ef645bee19d159876da074154ccf1f2` Steelix sprite to ephemeral CI storage and verified **exact Git blob SHA `1a0ff067983d294ce1534674efa91c697c4db794`**; decoded its first true `64×64` frame into **2,048 bytes of native 4-bpp 8×8-tiled pixel data** and its paired 16-color normal `JASC-PAL` into GBA BGR555.
- Real ARM ROM `CreateVirtualObject` set up simultaneous FireRed Snorlax **32×32** and Seagallop **64×64** sprites with correct OAM sizes and non-overlapping native tile allocations, and `BuildOamBuffer` plus ROM `LoadOam` transferred **both OAM entries into hardware OAM**, verified by mGBA bus readback.
- The true pinned Steelix `64×64` frame was staged into the **actual emulated OBJ VRAM** at the 64-tile allocation using GBA-native 16-bit writes, and all **1,024 16-bit words** were read back unchanged; the 16 converted normal palette words were staged in the assigned **OBJ palette bank** and read back unchanged. CI log specifically reports `PASS: exact pinned Steelix 64x64 frame and normal palette staged with readback from emulated OBJ VRAM/PLTT`.
- ROM-native `AllocSpriteTiles`, `SpriteTileAllocBitmapOp`, `DestroySprite`, and object recreation verified 32/64 allocation, simultaneous capacity, deterministic exhaustion, 64×64 reallocation and non-corrupt cleanup.

**Accepted technical subgate:** **native emulator CPU/OAM/OBJ tile and palette hardware compatibility for one true Kasenn Steelix 64×64 frame = PASS.** This is substantially stronger than the prior source-only Seagallop precedent.

**Still not demonstrated and must not be claimed:** Actual Steelix pixels visibly drawn to framebuffer (the current probe verifies OAM and hardware VRAM/PLTT but does not compare rendered pixels); other five Steelix directional frames animated on-screen; Shiny swap/reflections; real map entry/crowded-NPC resource pressure; or a failure-atomic 32↔64 *follower-specific* graphics loader. The game has no implemented follower state machine yet. Its current generic `ObjectEventSetGraphicsId` old-size fallback is not safe to assume for dynamically growing artwork. The asset gallery's all-252 source pixel and reversible 4-bpp tests separately remain PASS. No Phase Two gameplay or save change was made.

**Gate declaration:** `WRL-010 PHASE ONE — ASSET GATE BLOCKED` until the above remaining real-world rendering/dynamic-loader proof satisfies the canonical acceptance criteria. Do not import art into source, merge, or award points solely from emulator hardware-memory success.

## 2026-11-08 actual SIX-FRAME Steelix mGBA GPU framebuffer and Shiny rendering PASS — 1,415 assertions

**AUTHORITATIVE EXECUTED NATIVE GRAPHICS EVIDENCE:** [GitHub Actions run 42533894429](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/42533894429), head audit `b80cb7bf26258af30d3a38a383e748da46482163`, **tested current `main` commit `37230a9c513ed013fc6b402ec800bd1d13960492`** (the source advanced since the earlier 90d081 run). The dedicated workflow compiled the current real FireRed ROM/ELF in a disposable checkout, booted 600 mGBA frames, executed actual ROM-native sprite allocator and graphics/OAM functions, and passed **1,415 assertions**. All CI steps PASS. No ROM/save or source-art artifact uploaded or committed.

**Real Steelix artwork and hardware GPU evidence, NOT only file conversion:**
- Downloaded all six frame cells from the **exact pinned Kasenn Steelix PNG SHA `1a0ff067983d294ce1534674efa91c697c4db794`** at `cbe04a7a2ef645bee19d159876da074154ccf1f2`, transiently in CI only. Each `64×64` indexed cell converted losslessly into **2,048 GBA 4bpp bytes**; both source normal and Shiny `JASC-PAL` palettes converted into 16 GBA BGR555 words.
- Verified actual FireRed graphics engine handles simultaneous Snorlax 32×32 and Seagallop 64×64 `CreateVirtualObject` instances, `BuildOamBuffer` → ROM `LoadOam` → real emulated hardware OAM, disjoint sprite tile ranges, allocator exhaustion, cleanup and recreation.
- Staged the real Steelix `2,048` sprite bytes into the allocated emulated OBJ VRAM and 16 normal palette words into the emulated OBJ palette bank; checked all **1,024 16-bit tile words** plus all **16 16-bit palette words** by readback.
- **Actual GBA GPU framebuffer proof:** halted unrelated game callbacks in the disposable emulator fixture, enabled native 1D 4bpp OBJ LCD mode with one 64×64 OAM sprite, rasterized **all six real Steelix cells**, and inspected pixel output for visible non-background pixels and distinct directional frame hashes. CI evidence:
  - Cell 0: 595 visible pixels, hash `24394218214293506`
  - Cell 1: 574, `2910299914365420266`
  - Cell 2: 588, `1841888362547400609`
  - Cell 3: 571, `3442959672311279448`
  - Cell 4: 523, `3862118727643236520`
  - Cell 5: 509, `10780177131382775317`
- Same indexed artwork frame 0 was then rendered using the pinned **Shiny** palette: visible-pixel count was preserved and framebuffer hash differed from normal. CI printed `PASS: mGBA GPU framebuffer rendered all six actual Steelix directional cells and distinguished Shiny palette`.
- All **1,415 native ROM/video/memory assertions passed**.

**Scoped acceptance:** **Steelix true 64×64 source-art + normal/Shiny palette GBA GPU rendering and resource/OAM feasibility: PASS in a controlled, isolated emulator environment.** The 252/252 base pixel/4bpp asset conversion is separately PASS, plus Castform alternative form decoding.

**Remaining boundary:** This is not yet proof of player-facing follow behavior, live overworld party instance state, on-map warp/bicycle/battle visibility, reflections, map crowding, real-time 32↔64 species swapping or fail-safe allocation under concurrent map NPCs. The generic `ObjectEventSetGraphicsId` old-size retry remains a known unsafe implementation path; Phase Two must implement an explicit size-aware failure-atomic loader instead. Real graphics renderer support is proven, but **WRL-010 PHASE ONE — ASSET GATE remains BLOCKED** until the owner's agreed acceptance boundary explicitly permits separating controlled graphics feasibility from on-map dynamic follower integration, or those remaining conditions are actually tested. No gameplay/save code, merges, or earned points from these audit-only tests.
