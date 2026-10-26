# Pokémon Lightning Yellow

This project uses pret/pokefirered's English FireRed revision 0 decompilation as
its base. The unchanged checksum baseline is preserved independently in Actions. The
Physical/Special/Status split is authorized and implemented on this feature branch;
see docs/PHYSICAL_SPECIAL_SPLIT.md for its approved scope and validation limits.

Project direction (only the approved move-category split is implemented):
- Pikachu as the starter.
- A custom Pokédex drawn from Generations I–III.
- Redesigned wild encounters and trainer teams.
- Reassigned gym types and themes.
- A required Generation IV-style physical/special/status split, with physical
  and special categories assigned per move rather than by type.

All gameplay changes require explicit approved design specifications before
implementation. This list is project direction, not authorization to change game
behavior. Obtain approval for exact species, moves/categories, encounters,
trainer teams, gyms, progression, balance, and acceptance checks as applicable.
Do not implement gameplay changes as part of setup or baseline maintenance.

Keep main stable. Use topic branches and reviewable pull requests. Never push
changes directly to main. Require the FireRed baseline workflow before merging
setup changes; maintainers should enable branch protection and required review.
Require both modified and upstream-baseline checks for split work. Preserve the
vanilla checksum in the independent upstream job; never apply it to modified ROMs.

Read INSTALL.md, README.md and docs/UPSTREAM.md before building. Preserve upstream
attribution and all tool licensing notices. Do not commit ROMs, fetched payloads,
generated build outputs, separately obtained proprietary assets, or credentials.
Do not upload ROM artifacts to Actions. Do not download a reference ROM.
Use the existing isolated checkout in cloud tasks; do not create worktrees unless
explicitly requested. Build with `scripts/prepare-baseline.sh`, then
`make -j2`; run `python3 tests/run_split_tests.py` and use `make clean` before
repeatability validation. The independent upstream job still uses `make compare`.

The evolution overhaul is separately authorized: four level-40 trade alternatives,
direct use of existing trade-held evolution items, and Eevee stones/timed friendship.
Read docs/EVOLUTION_OVERHAUL.md and docs/RTC_SUPPORT.md. PRs are stacked on the split
and RTC work; do not merge dependencies or retarget without reviewing the stack.
Run all three test runners, including run_evolution_tests.py and run_rtc_tests.py.
Do not change National-Dex gating, other evolution data or balance without approval.

For emulator validation, read docs/EMULATOR_VALIDATION.md. After building the ROM,
run python3 tests/emulator/run.py (requires packaged libmgba-dev). It executes real
ROM code and evolution scenes but does not certify full battles, visual UI, link
peers or physical cartridges. Preserve the guarded test RAM and target-generated
ABI checks; never disable assertions to make the suite pass. No proprietary BIOS,
ROM or save-state artifacts may be committed/uploaded. Passing CI does not authorize
merging or enabling auto-merge.
