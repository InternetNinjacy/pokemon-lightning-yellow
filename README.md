# Pokémon Lightning Yellow

A planned FireRed-based project using [pret/pokefirered](https://github.com/pret/pokefirered).
This branch implements the approved Generation IV-style Physical/Special/Status
split on the English FireRed revision 0 base. See the [split implementation,
testing, and compatibility notes](docs/PHYSICAL_SPECIAL_SPLIT.md). The evolution branch also adds the approved [evolution methods](docs/EVOLUTION_OVERHAUL.md)
with a separately reviewed [RTC dependency](docs/RTC_SUPPORT.md). This branch extends
that work with the approved [evolution mechanics and Tyrogue menu](docs/EVOLUTION_MECHANICS.md). Other planned
features remain unimplemented; [AGENTS.md](AGENTS.md) requires design approval.

This branch also implements the approved [252-species custom Pokédex](docs/CUSTOM_POKEDEX.md),
including Castform at #233. Trainer placement is pending finalized design.

The [wild double battle capability test](docs/WILD_DOUBLE_BATTLES.md) enables ordinary
land/cave doubles on Route 1 and Mt. Moon 1F, with a 5% Onix/Cloyster special pair.
These are experimental test locations on a separate branch; see the report for
party/Repel/capture rules, executed evidence and remaining release checks.

This branch adds [approved Phase One stabilization](docs/PHASE_ONE_STABILIZATION.md):
the original twelve-rule typing snapshot, roster-aware evolution access before National Dex, and the
06:00–18:00 functional day/night boundary. It extends acceptance tests while
preserving the existing unmerged feature stack. The canonical [project dashboard](https://github.com/InternetNinjacy/pokemon-lightning-yellow/blob/main/docs/PROJECT_DASHBOARD.md)
and [implementation ledger](https://github.com/InternetNinjacy/pokemon-lightning-yellow/blob/main/docs/MASTER_IMPLEMENTATION_CHANGE_LEDGER.md)
live on the independent coordination branch; Drive synchronization remains pending.

[Route5 daycare breeding](docs/ROUTE5_DAYCARE.md) is implemented on this stacked branch: independent two-parent storage, safe legacy migration and persistent50% Shiny eggs. Read its validation status and rollback limitations before using old saves.

[Fossil rewards and species audit](docs/FOSSIL_REWARDS_AND_SPECIES_AUDIT.md) extend the existing aide/lab scripts and report unresolved off-Dex assignments. Blaine and Poké Ball dependencies remain explicitly documented.

The [Mystery Pokémon Vendor Network](docs/MYSTERY_VENDOR_NETWORK.md) implements ten existing sellers, a unique ten-species pool, foreign OT Mystery and genuine90% Shiny purchases. It remains separate from pending starter integration.

The approved [Poké Ball redesign](docs/POKE_BALL_REDESIGN.md) adds Wet/Dust/Snow capture conditions, Premier purchase rewards and Oak's initial Nest gift while preserving existing item IDs and shared artwork.

## Build entirely on GitHub

No local Python, compiler, or development tools are required. Open this repository's
**Actions → Battle split validation** workflow. Pushes to main/feature/setup branches and pull
requests trigger it; **Run workflow** starts a manual run once the workflow is on
the default branch. For a feature PR, use its Checks tab or the feature branch's run.
Click **modified** for regression tests and game builds, or **upstream-baseline**
for the unchanged upstream SHA-1 comparison. The modified job also runs
[headless mGBA integration tests](docs/EMULATOR_VALIDATION.md). Both require a clean rebuild. A green
run confirms the automated scenarios that actually ran; it does not certify a full
playthrough or visual acceptance.

Actions installs all Linux dependencies, builds pinned agbcc, obtains upstream
build inputs, and compiles on the runner. ROMs are neither committed nor uploaded;
there is intentionally no downloadable ROM artifact. Logs and checksums are the
validation results. GitHub Actions must be enabled for the repository, and first
runs from forks may require maintainer approval.

## Authorized cloud/Linux development

Follow the unchanged upstream [INSTALL.md](INSTALL.md). On Debian/Ubuntu:

```sh
sudo apt-get update
sudo apt-get install -y build-essential binutils-arm-none-eabi git libpng-dev python3 libmgba-dev
# From this repository root:
scripts/prepare-baseline.sh
python3 tests/run_split_tests.py
python3 tests/run_rtc_tests.py
python3 tests/run_evolution_tests.py
python3 tests/run_pokedex_tests.py
python3 tests/run_wild_double_tests.py
python3 tests/run_daycare_tests.py
python3 tests/run_world_reward_tests.py
python3 tests/run_vendor_tests.py
python3 tests/run_capture_tests.py
make -j2
python3 tests/emulator/run.py
make clean
make -j2
```

Python 3 and host GCC run the regression tests; Actions installs both. See [upstream provenance and input
limitations](docs/UPSTREAM.md) before building. Never download or upload a
reference ROM. The checksum comparison needs no reference ROM file.

## Organization and review

- `src/`, `include/`, `constants/`: game code, headers, and constants.
- `data/`, `asm/`, `graphics/`, `sound/`: upstream scripts, maps, assembly and assets.
- `tools/`, `Makefile`, `*.mk`: retained upstream build system and host tools.
- `scripts/prepare-baseline.sh`: pinned compiler and transient input preparation.
- `.github/workflows/build.yml`: server-side split and independent baseline checks; no artifact publishing.
- `docs/`: provenance and preserved upstream documentation.

Use topic branches and pull requests; keep main stable. Maintainers should enable
branch protection with required review and the **modified** and **upstream-baseline** checks (workflow files
do not enforce branch protection themselves). Gameplay implementation requires
approved design specifications first. Do not change the baseline hash just to
make an unexplained failure pass.

The [current typing reconciliation](docs/TYPING_CORRECTIONS.md) implements 24
remaining approved type pairs and validates all 35 effective explicit typing rules.
Run `python3 tests/run_typing_tests.py` alongside the other regression suites.

[Ponyta’s approved level-30 evolution](docs/PONYTA_LEVEL30_EVOLUTION.md) is tracked
as PKM-008. Canonical tracking now lives on main following the owner-authorized
feature-stack consolidation; the old coordination branch is historical.
