# Playtest 004 — private owner build

This source is **not a built ROM**. No visible follower behavior has been accepted
by the owner. Keep the existing Playtest003 ROM unchanged.

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

## Current status

Prepared source and build wrapper only. The private native compiler is not
available in the assistant's container. Real permission evidence and a
permission-cleared local source-art cache were not available here. No ROM
or checksum exists yet, and prior Playtest003 remains the only known
existing owner playtest binary. No points earned for this draft.
