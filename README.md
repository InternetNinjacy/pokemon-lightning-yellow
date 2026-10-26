# Pokémon Lightning Yellow

A planned FireRed-based project using [pret/pokefirered](https://github.com/pret/pokefirered).
This setup establishes an unchanged English FireRed revision 0 baseline. No
Lightning Yellow gameplay changes have been implemented. See [AGENTS.md](AGENTS.md)
for planned features and the required design approval process.

## Build entirely on GitHub

No local Python, compiler, or development tools are required. Open this repository's
**Actions → FireRed baseline** workflow. Pushes to main/setup branches and pull
requests trigger it; **Run workflow** starts a manual run once the workflow is on
the default branch. For this setup PR, use its Checks tab or the setup branch's run.
Click the run and **baseline** job to inspect installation and compilation logs.
Success requires `pokefirered.gba: OK` from the upstream checksum comparison on
both the initial build and a clean rebuild. A green run verifies the vanilla
baseline, not Lightning Yellow gameplay or emulator playability.

Actions installs all Linux dependencies, builds pinned agbcc, obtains upstream
build inputs, and compiles on the runner. ROMs are neither committed nor uploaded;
there is intentionally no downloadable ROM artifact. Logs and checksums are the
validation results. GitHub Actions must be enabled for the repository, and first
runs from forks may require maintainer approval.

## Authorized cloud/Linux development

Follow the unchanged upstream [INSTALL.md](INSTALL.md). On Debian/Ubuntu:

```sh
sudo apt-get update
sudo apt-get install -y build-essential binutils-arm-none-eabi git libpng-dev
# From this repository root:
scripts/prepare-baseline.sh
make -j2 compare
make clean
make -j2 compare
```

Python is not needed by this baseline. See [upstream provenance and input
limitations](docs/UPSTREAM.md) before building. Never download or upload a
reference ROM. The checksum comparison needs no reference ROM file.

## Organization and review

- `src/`, `include/`, `constants/`: game code, headers, and constants.
- `data/`, `asm/`, `graphics/`, `sound/`: upstream scripts, maps, assembly and assets.
- `tools/`, `Makefile`, `*.mk`: retained upstream build system and host tools.
- `scripts/prepare-baseline.sh`: pinned compiler and transient input preparation.
- `.github/workflows/build.yml`: server-side baseline checks; no artifact publishing.
- `docs/`: provenance and preserved upstream documentation.

Use topic branches and pull requests; keep main stable. Maintainers should enable
branch protection with required review and the **baseline** check (workflow files
do not enforce branch protection themselves). Gameplay implementation requires
approved design specifications first. Do not change the baseline hash just to
make an unexplained failure pass.
