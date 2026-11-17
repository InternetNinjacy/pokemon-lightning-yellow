## Updated private Playtest004 — current-main integration

This revision combines main `351932f8fe7c439b84a7c7fc555a914ba56b18e8` (PR265 complete410-move source and PR254
Heracross) with the preserved private Pichu/SUE/follower source from `b2e212b408af0e597d7b9b8d66398e3ccb53e531`.
The prior Playtest004 ROM remains unchanged. This is a separate owner build,
not a public release or accepted gameplay result.

Compilation **PASS**:16777216-byte ROM, SHA256 `26e52c729d822de37e1c53c5136f38669ed8c5e79a94728c722a2eab4e330b47`.255 follower descriptors
(252 base species plus3Castform forms),765 hash-pinned inputs checked offline.
Generated art registry SHA256 `196a74da8a37623c7178d9e976b585e8b592038ccbba2ea22af86654d061d0e3`. Linked tables have
BattleMoves0x1344 bytes (411x12),MoveNames0x19B0 (411x16),FollowerArtTable0xFF0
(255x16). No emulator/gameplay tests,simulation or CI dispatch performed.

Source comparisons preserve the exact existing Pichu/Oak-ownedSUE gift and lab
files, private species stats and learnsets, follower controller, snowfall fix,
private asset converter and resolver. Main's completed engine supersedes the
older experimental partial-move prototypes: no duplicate spread traversal, real
damage-gated VoltSwitch, shared ThunderFang and current Hurricane/Acrobatics.
Private Misty ability selections and existing experimental content remain.
Existing private legendary flags0x325..0x32C are retained alongside Route6 and
Heracross flags; no new placements or battle designs are introduced.

An actual link failure exposed the missing `map_events.o` dependency on
`include/constants/flags.h`. Adding that input forces map-object reassembly on
flag changes, fixing stale undefined private legendary symbols on build retry.
The final incremental build reassembled those objects and linked successfully.
Earlier failed logs remain local evidence, not reported as successful runs.

Private local owner output is `/tmp/ly-owner-playtest004-r2/Playtest004.gba` with
its build metadata and local attribution. Previous output under
`/tmp/ly-owner-playtest004` is preserved. Art,ROM,BIOS,saves,cache and private
permission records are excluded from Git. Workspace output is temporary and is
not a public/durable release. Owner-reported artwork permissions remain private,
not independently certified; no further permission-upload request is made.

EWRAM261068/262144 (1076 bytes headroom),IWRAM30032/32768. Owner gameplay
acceptance remains pending:252 sprite visibility/colors/movement,Steelix,party/PC,
maps,battles,410 mechanics,Pichu/SUE,new rewards and inherited experimental
legendary content. No natural-progression/hardware/link certification claimed.
The branch remains draft/unmerged;461/712 accounting unchanged.

# Playtest 004 — private owner build

A private ROM has now been compiled from the exact source recorded below.
No visible follower behavior has been accepted by the owner. The original
Playtest003 ROM remains unchanged; this is a separate owner playtest build.

## Exact integrated starting point

The private integration draft PR #258 was forked from the approved PR #256
source at `1ea7c53c3ec0044e2343a96611c95a6dec07e4c7`.
It contains all approved Pichu starter and Oak-owned Sue pickup source, along
with the already-existing follower engine, plus the source-only converter,
255-entry artwork registry generation tool, and 765 exact upstream source
blob pins from PR #257. The tracked sprite registry remains a zero-art
fallback until the private converter runs with permission-cleared input.

Do not replace `src/approved_gifts.c` with older PR #54 versions: that
would restore an obsolete forced SOCKET name and player-owned Eevee.
Preserve Pichu Tackle/Thunder Wave/Growl, the 309/405/534 electric-line
base stats, Oak's permission for Sue's leftmost Poké Ball, foreign Oak
Original Trainer, and rival Dratini.

## Private build on an authorized machine

Requirements: a local checkout of the **current PR #258 integration source**,
a working FireRed build toolchain (including the GBA assembler/linker/
object-copy utility), any required repository setup from the native project,
a permitted source-art cache or connectivity to the exact upstream revision,
and a local text file recording real artist/source credits and applicable
reuse-permission scope. A statement of permission without specific grant
provenance is not independent certification.

Run in the checkout:

```sh
python3 scripts/build-private-playtest004.py \
  --attribution /private/credits-and-permissions.txt \
  --output-dir /private/pokemon-lightning-yellow/playtest004
```

Add `--offline` to read **only** previously obtained, pinned asset files.
Set `FOLLOWER_CACHE` to the local pinned cache root when needed.

This entry point refuses an unprepared toolchain or an absent permission
record. It converts the exactly pinned 765 source files into a private
generated sprite table, checks the full 252 roster plus Castform forms,
compiles the ROM using the existing private follower Makefile target,
and copies the binary to a separate output directory **outside Git**.
It refuses to overwrite an existing Playtest004 ROM or build manifest.

The private output is:
- `Playtest004.gba`, only if a real native ROM build succeeded
- `Playtest004.build.json`, recording actual Git source commit, 255
  generated artwork descriptors, source tree, ROM byte count and ROM SHA-256

The wrapper does **not** run gameplay tests or simulations. Owner conducts
all in-game playtesting. A successful build is not evidence that Pokémon
visibly follow in every map, or that artist permission was legally granted.

**Do not commit, attach to GitHub Actions, or publicly upload** ROM, save,
BIOS, fetched sprite assets, generated source containing art pixels,
or private permission documents.

## Actual private artwork/build checkpoint

- Exact gameplay/build source: `e94cba8e9257e9e9c41bb0ee797c66d25f556b16` on `integration/playtest004-pichu-sue-follower-private-build-20261116` (PR258).
- Owner explicitly confirmed permission from Nintendo and relevant fan creators;
  correspondence stays private at their request. Recorded as owner-reported,
  not independently certified. Attribution retained with private build inputs.
- Downloaded **765/765** exact pinned PNG/palette inputs; all byte lengths and Git
  blob hashes matched commit `cbe04a7a2ef645bee19d159876da074154ccf1f2`.
- Converter and readiness gate completed: **252 base species + 3 Castform forms,
  255 descriptors**. Generated pixels remain only in ignored private files.
- Private ROM compilation **PASS**. Built snapshot derived from a Git archive,
  preserving the unrelated dirty working checkout and creating no Git worktree.
  The snapshot's10362 tracked files matched the committed source except2614
  palette files whose line endings were rewritten LF -> CRLF by the build tools;
  normalized bytes match exactly. No palette colors or gameplay source changed.
- ROM byte count: **16777216**. SHA-256:
  `efb4aadb2c2d32eefc0033998454489c012d7f295d509dd6413e8f188afbf010`.
- Linked ELF: `FollowerAssets_Count` returns255; `sFollowerArtTable` is0xFF0
  bytes; Steelix's six64x64 native frames occupy0x3000 bytes. This is compiled
  presence evidence, not a claim of runtime visibility or movement acceptance.
- EWRAM261068/262144 bytes (**1076 bytes headroom**), IWRAM30032/32768;
  ROM used16230488 bytes. Shared engine growth must respect the tight RAM margin.
- No emulator, gameplay tests, simulations, CI dispatch or public ROM/art upload.
  Owner manual playtesting remains pending. Accounting **461/712 unchanged**.

Build invocation used cached toolchain binaries/PNG headers under
`/workspace/lightning-toolchain/root`, pinned installed agbcc and the exact
upstream multiboot inputs already present in the workspace:

```sh
PATH=/workspace/lightning-toolchain/root/usr/bin:$PATH \
PKG_CONFIG_PATH=/workspace/lightning-toolchain/root/usr/lib/x86_64-linux-gnu/pkgconfig \
PKG_CONFIG_SYSROOT_DIR=/workspace/lightning-toolchain/root \
make -j2 private-followers-rom \
  FOLLOWER_ART_PERMISSION_CONFIRMED=1 \
  FOLLOWER_ATTRIBUTION_FILE=/tmp/ly-follower-owner-permission.txt \
  FOLLOWER_CACHE_ONLY=1
```

Initial compilation exposed the legacy scanner rejecting a macro `#include`.
Source commit `e94cba8e9257e9e9c41bb0ee797c66d25f556b16` uses a standard spaced directive with explicit Makefile
registry dependencies; final `make -j2 rom PRIVATE_FOLLOWER_ART=1` passed without
that diagnostic. Final log: `/tmp/ly-playtest004-private-final-build.log`.
Private owner output is `/tmp/ly-owner-playtest004/Playtest004.gba`, with its
`Playtest004.build.json` and local credits record. These local files are temporary
workspace output, not a public release or durable external backup.

Next acceptance is owner playtesting: follower visibility/selection across the
252 roster, normal/Shiny colors, Steelix ground contact, maps/doors/warps,
party/PC and faint/revive behavior, Pichu/SUE and snowfall regressions. Compilation
alone does not close WRL-010 or certify every follower animation.
