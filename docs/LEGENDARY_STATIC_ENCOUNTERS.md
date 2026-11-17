# Four approved post-League stationary encounters

Source authority: owner [issue234](https://github.com/InternetNinjacy/pokemon-lightning-yellow/issues/234),
which separately authorizes these four encounters despite the older general
stationary-encounter design hold. Existing [PR235](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/235)
is refreshed from merged follower main `28dd82b5d9a684455a36f6458ac88a3e7e45d4ed`,
reusing prepared source `cefd2a46d9b97b39a30b2f1b1903ec845979a258`.

| Pokémon | Level | Existing map | Locked tile | Elevation |
| --- | ---: | --- | --- | ---: |
| Raikou | 50 | Route10, Power Plant exterior bank | 8,41 | 3 |
| Entei | 50 | MtEmber_SummitPath_1F | 4,8 | 3 |
| Suicune | 50 | SeafoamIslands_B4F | 30,9 | 1 |
| Mew | 50 | SevenIsland_TanobyRuins | 105,12 | 3 |

All four are independent stationary encounters, available only after the League
(`FLAG_SYS_GAME_CLEAR`). They use existing species overworld graphics, map assets,
`setwildbattle` and `StartLegendaryBattle`; no new artwork, distribution, weather,
Trainer, learnset or species changes are included.

## Persistence and integration

Each encounter has separate fought and visibility flags. Capture/defeat resolves
only that creature; Run, loss and Teleport preserve retry. Transition callbacks
restore appropriate visibility from saved completion flags and the post-League
gate. Four unused flag numbers become `FLAG_FOUGHT_LIGHTNING_*` at0x325–328 and
`FLAG_HIDE_LIGHTNING_*` at0x329–32C. No save field or checksum layout moves.
Later Heracross4AD/4AE and Route6 pickup4AF claims are preserved byte-for-byte.

The obsolete starter-dependent beast roamer is suppressed for new games and old
saves. Existing roamer storage/layout remains; only the Raikou/Entei/Suicune
legacy path is superseded by these approved statics. Other roamer infrastructure
is retained. The wild-double native fixture now checks that approved suppression
instead of requiring the retired beast roamer to become active.

The ten production files are four maps' `map.json`/`scripts.inc`, `flags.h` and
`roamer.c`. All nine map/roamer files matched the prepared235 source baseline
before reuse; their resulting bytes exactly match the prepared implementation.
Only the eight approved unused flag names are changed in the current flags file.
Original NPCs, map geometry, Seafoam currents/Articuno and seven Tanoby chamber
warps remain. Authored object counts including each new legend are11/1/5/5,
leaving room below the16-object limit for the player and selected follower.

Do not import PR243's complete tree: it also contains nine unrelated NPC scripts.
Only its exact prepared wild-double compatibility fixture is reused here. No
old diagnostic workflows or Surf workaround is imported.

## Executed source/build evidence

- PASS strict four-map placement, collision/elevation, script and flag contracts:
  `python3 tests/check_legendary_statics.py`.
- PASS seven current-main source/provenance contracts:
  `python3 tests/test_legendary_current_main_source.py`.
- PASS48 selected pure-source runners, ten follower integration/pipeline cases,
  five follower lifecycle cases and25 historical projection cases.
- PASS registration source checker:60 host suites and34 default native groups.
  Seven prepared focused legendary selectors remain, and the core legendary
  group joins the default runner. Existing ABI guards/groups are preserved.
- PASS fresh object-free pinned-agbcc modified-ROM build, with all255 follower
  art descriptors supplied privately:16MiB, SHA256
  `4fd065a5bef6243c45b135b0af33d6cec887e2aa4ba6a7895667f03d96e8b1ac`.
  EWRAM1076bytes free and IWRAM2736bytes free, unchanged.
- PASS target-generated ABI probe and libmGBA runner compilation only. A wrapper
  explicitly stopped before launching the runner. **No emulator/gameplay/C-model
  execution occurred.** Compile-only log:
  `/workspace/scratch/stationary-native-compile-only.log`.

Pinned-history checks use the existing project Git object store in archive
snapshots (`GIT_DIR=/workspace/pokemon-lightning-yellow/.git`). The exact reviewed
inverse is `tests/specifications/legendary_current_main_delta.json`; older hashes
and assertions are not rewritten. The modified workflow adds only two source
checks; the existing full native and independent upstream jobs remain intact.
No workflow was dispatched and current-head CI is not claimed green. No
independent upstream comparison or repeated clean reproducibility run was
performed for this checkpoint; those workflow requirements remain configured.

## Historical evidence and owner physical-device checklist

Old PR235 headcefd had seven focused native groups pass in run43545788951:
visibility/Run, adjacent button interaction, sequential captures including PC,
KO/save, map-grid access, opponent-caused loss and real Teleport retry. Its old
inherited AI/history failures have since received separate main repairs. These
results belong to that old head and are not new combined runtime acceptance.

**Material unresolved access risk:** later isolated PR244 physical probes
activated Surf but failed water entry at Seafoam29,7 and Tanoby67,4 (runs
43630665010 and43631949077). The cause was not established. Grid-based Surfable
reachability and adjacent staged interaction do not prove natural HM traversal.
No assertion is removed and no production fix is guessed from that observation.

Under the owner's November17 delegation, pending testing alone is nonblocking
for engineering approval/normal merge. It remains outstanding, not passed:

- [ ] Before League: no encounter visibility/interaction.
- [ ] After League: naturally travel from real entrances to all four placements,
  especially Seafoam/Tanoby using Surf and their ordinary puzzle/access gates.
- [ ] Interact, Run/retry, Teleport/retry and whiteout/retry for each.
- [ ] Capture and defeat independently; save/reload and reenter; other legends
  remain available. Check captures into party and full-party PC delivery.
- [ ] Old beast-roamer save no longer spawns a duplicate; no unrelated birds,
  Mewtwo, currents, Unown chambers, followers or weather regressions.

Engineering integration, physical-device acceptance, earned points and release
certification remain separate. All50 permanent task histories and461/712
accounting stay unchanged. Rollback is reverting the implementation merge; no
party/PC/save-layout migration is required. A previously caught Pokémon is not
removed by a source rollback. Generated art, ROM, BIOS and saves stay outside Git.
