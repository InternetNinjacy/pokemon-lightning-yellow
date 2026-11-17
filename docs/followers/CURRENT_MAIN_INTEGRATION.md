# WRL-010: current-main follower integration

Current engineering checkpoint: November 17, 2026. This page supersedes stale
integration instructions in the historical follower reports; it does not erase
their evidence or certify gameplay acceptance.

## Scope and provenance

The implementation PR is [PR54](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/54),
branch `feature/selectable-following-pokemon`, refreshed from main
`b45e6c6e9cebb24ce8b7c36599f388375243c31b`. It reuses PR54 runtime source and PR257
asset tooling (`e7d388810de0b0964a46afaaf3d012f7e666aad5`). No private Playtest004
Pichu opening, experimental species stats or obsolete natural-learnset snapshots
are promoted. Main's optional Pikachu naming, securely rekeyed OAK-owned SUE,
146 newly approved natural learnsets and exact shared Snow timer are preserved.
`src/approved_gifts.c` remains byte-identical to the starting main.

Authority is `implementation/selectable-following-pokemon.md`, subsequent explicit
owner implementation authorization and owner-confirmed use of the follower
artwork. This records the owner's statement, not a fabricated license or letter.
Source identities and attribution are recorded separately from that statement.

## Implemented source

- `follower_state.c`: a versioned 16-byte PID/OT selection in existing
  `SaveBlock1.unused_3A94[64]`. The other 48 bytes and all existing save offsets are
  unchanged. Missing, corrupt or ambiguous identities resolve to no follower.
  Exact PID/OT clones fail closed; duplicate species alone are supported.
- Party menu: select or stop following without changing the battle lead. Only
  canonical 252-roster non-egg, non-Bad-Egg party members are eligible. HP zero
  hides the assigned member without selecting a replacement. Reordering,
  nickname changes and ordinary in-roster evolution retain identity.
- PC, trade, daycare and Rocket rescue return removal hooks clear the selected instance at actual
  removal, preserving cancellation and unrelated Pokémon. Withdrawal never
  implicitly reselects it.
- Controller and graphics: one nonblocking object, directional six-frame art
  with east mirroring, camera offsets, normal field weather palette application,
  script/bicycle/Surf/forced-movement suppression and safe map teardown.
  Resource failure hides the follower without clearing selection. Ownership
  checks prevent stale sprite/object IDs from destroying unrelated resources.
- Saved object snapshots omit only the follower-owned transient object, leaving
  live objects and normal NPC saves intact. Conservative legacy-placeholder
  recovery requires the selected identity and absence of an authored NPC.
- Opening: separate once-only specials select the actual gifted Pikachu and
  stage a short emergence using the existing Poké Ball artwork and cry. No extra
  Pokémon or forced nickname is created. Resource exhaustion and interruption
  time out safely. Oak's Eevee gift does not replace the follower.

The normal Castform field descriptor is form zero: battle Forecast forms are not
persistent party forms. The source registry additionally covers all three
weather-form assets for explicit future form resolution; no new weather rule is
introduced here.

## Artwork and build variants

The public source contains an intentionally empty generated-art registry. It
compiles and safely suppresses graphics; **it is not a visible all-252 build**.
The private converter verifies 765 pinned PNG/palette inputs at Kasenn source
`cbe04a7a2ef645bee19d159876da074154ccf1f2`, then generates 252 base descriptors plus
three Castform forms. Registry SHA256:
`196a74da8a37623c7178d9e976b585e8b592038ccbba2ea22af86654d061d0e3`.

See `PRIVATE_ART_BUILD.md`, `source-inventory.json`, `expected-source-blobs.json`
and `252-species-coverage.csv`. The coverage matrix records source/tooling
verification for every species; it does not claim 252 runtime playtests.
Artwork/cache files, generated pixel arrays, private correspondence, ROMs, ELF
files and save files are not committed or uploaded as Actions artifacts.

## Executed validation and its limits

The following checks have actually run in the isolated source snapshot:

- 48 selected pure-source regression runners: PASS. Executable C/gameplay/RNG
  models were deliberately excluded under the current owner playtesting policy.
- New current-main follower integration contract: 9 source cases PASS.
- New lifecycle contract: 5 source cases PASS.
- PNG/palette/converter validation: 15 tooling cases PASS.
- Graphics allocation/source ownership checks: PASS.
- Frozen history projections: 25 cases PASS; WRL001 identity projection: 5 PASS;
  exact newly approved 146-learnset contract: 5 PASS.
- Registration checker: PASS, with 60 host suites and 33 native groups retained.
  Registration is not execution. Existing ABI probes, assertions, source-history
  hashes and the independent upstream comparison remain in place.
- Follower-state host harness compiled with `-fsyntax-only`; its executable was
  not run. No state-model result is claimed.
- Pinned-agbcc source-only and private full-art modified-ROM builds both compile
  and link to 16 MiB. EWRAM: 261068/262144 bytes (1076 free); IWRAM:
  30032/32768 bytes (2736 free). This is a tight EWRAM budget, not an overflow.

Final incremental build SHA256 values (including daycare and Rocket removal
hooks): source-only stub `6733fb2cf0b9fe4734b9b2c39e9df42bd0162ef17c6d5aad9d746e754b646693`;
full255-art `add0b0151c0a69a82b1061146b9c73ef05a4d005469c012465505f36ebc11b30`.
These identify local, uncommitted ROMs; they are not a commercial-ROM baseline.

Commands, from the snapshot with `GIT_DIR` pointing at the existing project Git
object store where pinned historical sources are required:

```sh
python3 tests/test_follower_integration_source.py
python3 tests/test_follower_lifecycle_source.py
python3 tests/run_follower_asset_validator_tests.py
python3 tests/run_follower_graphics_resource_tests.py
python3 -m unittest discover -s tests -p '*history_projection.py' -q
python3 tests/test_wrl001_identity_projection.py
python3 tests/test_pkm009_approved146_source.py
python3 scripts/check-integration-registrations.py
make -j2
```

No gameplay/emulator/RNG test, link peer, hardware session or CI workflow dispatch
has been performed for this checkpoint. Skipped/pending CI is not green CI. A
successful ROM link is not completion of the follower acceptance criteria.

## Next gates and rollback

The modified-ROM Actions job now generates its ephemeral artwork from the exact
765 pinned inputs, retains four pinned upstream credit texts, gates all255
descriptors before compilation and checks their linked ELF table after the clean
rebuild. It restores only the generated registry stub before the existing Git
source-diff guard and verifies the ROM hash again. No artwork/cache/ROM upload
is added. The independent vanilla job stays unchanged. This workflow source is
prepared and structurally checked; it has not been dispatched or proven in CI. Keep
all native assertions and registrations. Do not merge a zero-art default build
as the finished visible follower feature.

Owner playtesting remains required: both avatars' initial gift and naming,
all-party-slot selection/stop, same-species duplicates, faint/heal, save/reload,
PC/daycare/trade removal and cancellation, evolution, normal NPC interactions,
map connections/warps/ledges, bike/Surf/cutscenes and crowded resource conditions.
Representative tiny/large/flying/water/shiny sprites and all-252 coverage need
actual in-game inspection. Native fixtures remain available for a later
explicitly authorized emulator-testing run, not a claimed result now.

Keep PR54 draft/unmerged until the remaining gates are documented. Rollback is
reverting the follower PR; no Pokémon/save-layout migration is required. Older
builds ignore the reserved selection record. Do not erase party, PC or daycare
records during rollback. All 50 permanent tasks and 461/712 earned accounting
remain unchanged pending acceptance. Drive synchronization has not been performed.
