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
