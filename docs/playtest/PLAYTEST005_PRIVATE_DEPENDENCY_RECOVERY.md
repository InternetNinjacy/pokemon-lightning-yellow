# Playtest 005 — owner-side private dependency recovery

**Status:** Source-only procedure. No ROM, artwork, compiler, or cache was recovered in the current assistant environment. Do not commit generated assets, private permission correspondence, ROMs, or build outputs. This is not a license certification, compilation result, or gameplay acceptance.

## Exact source anchors and requirements

- Checkout branch: `integration/playtest005-source-staging-20261118` at its latest exact head (record `git rev-parse HEAD` before building).
- Production ancestry guard: `4fd313fe4dfed0412f2e91ed7e876a9bf0b717fc`.
- Artwork upstream: `Kasenn/pokeemerald-expansion-kasen` pinned to `cbe04a7a2ef645bee19d159876da074154ccf1f2`.
- Pinned inventory: `docs/followers/expected-source-blobs.json`, 765 Git blobs and byte lengths; `docs/followers/252-species-coverage.csv` lists the 252 base species; 3 Castform form descriptors are additional.
- Expected source inventory: 252 × (PNG, normal palette, shiny palette) + 3 Castform forms × 3 = **765 files**. Expected generated descriptors: **255**.
- Generated registry: `src/follower_assets_private.inc` (ignored by Git); metadata: `docs/followers/local-build-metadata.json` (ignored by Git). Normal builds use the tracked zero-art registry.

## A. Recover the original owner cache first (no download necessary)

1. On the **original private build machine or backup**, inspect `/tmp/lightning-follower-assets` and the location of `FOLLOWER_CACHE`. The historical build location `/tmp/ly-owner-playtest004-r3` may contain only a ROM/manifest; **a ROM cannot reconstruct 765 original pinned PNG/palette files**. Also check the previous private checkout's `src/follower_assets_private.inc` and `docs/followers/local-build-metadata.json` for evidence, not as a substitute for original source bytes.
2. Copy the complete directory tree of pinned PNG/palette files into a **private, owner-controlled folder**. Preserve their relative `graphics/pokemon/.../overworld...` paths. Never upload this folder to GitHub or Drive merely to make it available to this chat.
3. In the isolated checkout, set `FOLLOWER_CACHE` to that directory. The existing converter checks **each of the 765 files** against its pinned Git blob SHA-1 and expected length. A wrong or incomplete cache must fail closed.

## B. Reconstruct missing cache from the exact upstream revision (only if the permission scope allows retrieval and local conversion)

The pinned manifest and the source-only converter already specify all 765 source paths and expected bytes. No replacement artwork needs to be invented. The existing `scripts/verify-follower-assets.py` retrieves exact files from `https://raw.githubusercontent.com/Kasenn/pokeemerald-expansion-kasen/cbe04a7a2ef645bee19d159876da074154ccf1f2/` when `FOLLOWER_CACHE_ONLY` is **not** set. The converter verifies each downloaded or cached file against `docs/followers/expected-source-blobs.json` before generating the private registry. A public upstream Git tree does **not** establish permission to redistribute or adapt the art. Confirm the specific existing grant and attribution record locally before using this route.

Do not use another upstream commit, approximate sprites, AI-redrawn sprites, or a historical ROM as substitutes for pinned inputs. If upstream retrieval fails or permissions cannot be confirmed, stop; preserve the source branch.

## C. Recover or prepare the ARM/agbcc toolchain

1. On the original build machine, inspect `/workspace/lightning-toolchain/root`, `DEVKITARM`, `PATH`, and `tools/agbcc/bin/agbcc`. Preserve the compiler version/build provenance where available. These locations are historical clues, not evidence that the tools exist in this session.
2. If missing, use the repository's `INSTALL.md` as the canonical host-specific instructions. For Debian/Ubuntu it documents `build-essential binutils-arm-none-eabi git libpng-dev`. It also documents building `pret/agbcc` with `./build.sh` and installing it into the **isolated Lightning Yellow checkout** with `./install.sh /path/to/checkout`. The host tools must match the repository's actual build requirements; do not claim binary equivalence with the historical Playtest004-r3 toolchain without version evidence.
**Exact compiler pin recovered from repository provenance:** `docs/UPSTREAM.md` identifies `pret/agbcc` commit `da598c1d918402c42c0c0d7128ba14567f3175e9`. Do not use an unpinned current `pret/agbcc` HEAD. In a private owner-controlled environment, check out that commit before running `./build.sh` and `./install.sh /path/to/isolated-checkout`. The repository's CI documents Ubuntu 24.04 and `build-essential binutils-arm-none-eabi git libpng-dev python3`, but package versions are not hermetically pinned; record local versions for the manifest/handoff. The compiler source pin is known; its compiled executable is still missing here.\n\n3. Before authorizing a build, check the presence of `make`, `arm-none-eabi-as`, `arm-none-eabi-ld`, `arm-none-eabi-objcopy`, and `tools/agbcc/bin/agbcc`. `MODERN=1` instead requires `arm-none-eabi-gcc`; do not switch compiler modes just to bypass a missing pinned compiler.

## D. Owner-only future build command — NOT executed

After confirming the permission/credit record and recovering toolchain and cache, use a **separate private checkout**, never the public source staging checkout. Replace placeholders with real local paths. The driver checks source ancestry, cleanliness, input counts, metadata, registry digest and ROM output; it calls `make tidy` in the isolated checkout and generates private ROM/manifest outside Git.

```sh
cd /private/path/to/pokemon-lightning-yellow
export FOLLOWER_CACHE=/private/path/to/lightning-follower-assets
python3 scripts/build-private-playtest005.py \\
  --attribution /private/path/to/artist-permissions-and-credits.txt \\
  --output-dir /private/path/to/playtest005-output \\
  --offline
```

Omit `--offline` **only** if pinned upstream fetching is authorized and necessary. The resulting outputs, if successful, are `Playtest005.gba` and `Playtest005.build.json`. No runtime/gameplay claim follows from their existence. Do not run this command under the current source-only instruction.

## E. Fail-closed handoff checks

- Source branch is exact and clean; no PR #258 wholesale overlay or main merge.
- 765 pinned original files are available and hash-verified; no synthetic replacements.
- Local attribution record covers the actual reuse and conversion scope; do not publish it.
- Native tools and compiler are present with documented provenance.
- The private build uses `src/follower_assets_private.inc`, not the zero-art fallback.
- ROM, generated art, local metadata and permission records stay outside public Git commits and CI artifacts.
- The owner separately decides when to authorize compilation and gameplay testing. Accounting stays 50 IDs and 461 earned / 251 remaining / 712 total without verified acceptance.
