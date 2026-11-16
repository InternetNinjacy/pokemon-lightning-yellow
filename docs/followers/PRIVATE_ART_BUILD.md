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
