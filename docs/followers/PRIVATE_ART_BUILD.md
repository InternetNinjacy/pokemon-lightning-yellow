# WRL-010: private follower-art build integration

**Scope:** Build-source integration only; no artwork or ROM in Git, no gameplay-testing claim.

## Why ordinary ROMs currently have no following Pokémon

The tracked `src/follower_assets_generated.inc` is intentionally a fallback with
`sFollowerArtCount = 0`. The running controller asks
`FollowerAssets_Resolve()` for the selected Pokémon's six-frame descriptor;
the empty registry always returns false, so it cannot display any follower.

The existing `scripts/build-follower-assets.py` can generate a table for the
project's 252-species matrix plus three Castform alternate-form descriptors,
but a plain `make` does **not** run that generator.

## Opt-in private follower build

The new target `private-followers-rom` invokes the existing pinned converter,
checks the generated table and metadata, and then invokes the normal `rom`
build in the same checkout. It deliberately refuses to compile the empty table.

Only after the person responsible for the art has recorded the relevant
permission scope and credit **locally**:

```sh
make private-followers-rom \
  FOLLOWER_ART_PERMISSION_CONFIRMED=1 \
  FOLLOWER_ATTRIBUTION_FILE=/private/path/to/attribution-and-permission.txt
```

The local text record should identify each selected artwork origin and
contributing artist where known, the **specific permission covering this
FireRed-derived, AI-assisted, noncommercial project**, the grant reference,
the exact pinned Kasenn source revision
`cbe04a7a2ef645bee19d159876da074154ccf1f2`, and any redistribution
conditions. This is not a template asserting that permission was granted.
The earlier owner statement that permission was resolved is respected; its
exact contributor coverage is still not captured in the source records.

The existing converter downloads only exact pinned source paths, checks each
base sheet against its recorded Git blob ID, converts the six indexed frames
and both palettes, and writes:

- `src/follower_assets_generated.inc` — **temporary generated art-filled
  source** replacing the tracked zero-art fallback in the private checkout
- `docs/followers/local-build-metadata.json` — metadata and generated SHA-256

The new `scripts/check-follower-build-readiness.py` refuses a missing notice,
zero-art fallback, hash mismatch, non-255 entry count, duplicate base
species or unexpected extra forms. It is only a build-input guard; it does
**not** prove visual behavior, authorship, licensing, movement, or full ROM
operation. The existing converter still has a strict dedicated 64x64 Steelix
exception and no silent downscale.

**Do not commit** the filled generated file, fetched image cache, local notice,
private ROM, saves, BIOS, or graphics binaries. Because the fallback
`src/follower_assets_generated.inc` is already *tracked*, ordinary
`.gitignore` cannot protect generated pixels from an accidental commit.
Before committing unrelated source changes, inspect `git status` and
restore that tracked file **only after the private ROM build is complete**.
The next clean checkout will again have the zero-art fallback until the
explicit local generation step runs.

## Remaining blocker

The target is new source wiring only, not a built ROM. An actual private build
still requires the pinned assets to be accessible from a build-capable machine,
the real permission/attribution record, and a successful ROM compilation.
The owner, not the assistant, performs gameplay playtesting. This change
does not claim the 252-species follower behavior works yet.

Integrate this narrow build hook into the existing WRL-010 implementation
branch **after** checking for head drift; do not duplicate or replace its
follower state, movement, menu, PC, save or sprite-loader code. PR #256
Pichu/Sue changes and Playtest004 staging remain independent.

### Offline private builds from a previously populated pinned cache

To avoid network dependency when the permission-cleared source files are already
stored in the build machine's local cache, add `FOLLOWER_CACHE_ONLY=1` to the
`make private-followers-rom` invocation. The existing converter's verifier will
then refuse any missing source file without trying GitHub. Its pinned PNG Git blob
checks still run on cached source bytes. Configure `FOLLOWER_CACHE` to the local
cache directory if the default temporary cache does not contain the files.

The Makefile now checks that the local permission/attribution record exists and
is nonempty **before** downloading or converting any art. Its content must still
be examined for actual artist, source, and scope information; a text-file size
check does not itself prove legal authorization. Cache-only cannot fabricate
missing pixels or permission and does not produce a ROM in this environment.

### Incremental compilation and 765-input provenance

The Makefile explicitly declares `src/follower_assets_generated.inc` as a
dependency of the compiled `follower_assets.o` object. The private target
updates that generated file before calling `make rom`, preventing a previously
compiled zero-art follower resolver object from being reused merely because
the main C source did not change.

The converter's **local-only** metadata now inventories 765 distinct inputs
(252 base sprites and 3 Castform forms, each with one PNG and two palette files).
For each input, it records path, observed Git blob SHA-1, SHA-256, byte count,
and whether the input was checked against a pinned upstream Git blob. The new
text-only `docs/followers/expected-source-blobs.json` records all **765** expected
blob SHA-1 hashes and lengths from the exact upstream Git tree at commit
`cbe04a7a2ef645bee19d159876da074154ccf1f2`. The GitHub tree returned
`truncated=false` with 25,529 entries; the original 252 base PNG pins matched
that tree exactly. Thus every palette and Castform-form asset now has an
independently obtained expected Git blob hash, rather than relying only on path.

The pre-build readiness gate checks that the 765 source paths are complete,
unique, and in the exact approved inventory/form order, plus source digests,
recorded byte counts, and affirmative per-file upstream pin verification.
It checks the generated table SHA-256 and all 255 entries. The converter
compares the bytes of every fetched or locally cached source file against
its expected Git blob SHA-1 and expected byte length, including palettes.
These checks verify file identity **only**; they do not grant permission,
prove artist ownership, validate gameplay, or guarantee a ROM build.

## Private output isolation — current pipeline

The converter now writes `src/follower_assets_private.inc`, an explicitly ignored
local-only pixel registry. It no longer overwrites the tracked zero-art
`src/follower_assets_generated.inc`. The checker and Playtest004 wrapper use the
private filename. `make private-followers-rom` selects it only for its recursive
`make rom PRIVATE_FOLLOWER_ART=1` compilation.

Ordinary `make` uses the tracked zero-art include even if private pixels remain
locally. The follower-assets object is rebuilt on each invocation, in both agbcc
and modern configurations, so changing modes cannot reuse the other mode's
compiled registry. This intentionally costs one C-object compilation per build.
Generated pixels and metadata stay ignored, allowing a subsequent private build
to pass the clean-tracked-source gate without manually restoring source files.
The earlier tracked-file restoration instructions above describe the superseded
pipeline. Existing old art-filled tracked includes must still be restored before
using this new pipeline; no automatic reset is performed.

This source change does not import sprites, supply missing permission references,
produce a ROM, or execute gameplay tests. Exact 765-input pins, 255-descriptor
checks and local permission/credit requirements remain intact. `gitignore` reduces
accidental staging; it is not a security boundary against force-adding assets.

## Permission/build status update

Owner reconfirmed the relevant creator permissions and requested private
correspondence remain private. A local owner-reported scope/credit record was
used, without claiming independent certification or requiring a letter upload.
All765 pinned inputs were converted and a private255-descriptor ROM compiled.
The earlier permission-evidence/toolchain absence is historical, not the current
blocker. Actual build evidence and remaining owner-playtest scope are recorded
in `docs/playtest/PLAYTEST004_PRIVATE_BUILD.md`. No pixel/ROM files are in Git.
