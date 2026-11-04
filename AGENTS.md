# Pokémon Lightning Yellow

This project uses pret/pokefirered's English FireRed revision 0 decompilation as
its base. The initial milestone is an unchanged, checksum-matching baseline.

Planned design (not implemented or fully specified):
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
The vanilla checksum is the baseline gate; any future approved gameplay work
must explicitly replace it with suitable agreed validation in its reviewed PR.

Read INSTALL.md, README.md and docs/UPSTREAM.md before building. Preserve upstream
attribution and all tool licensing notices. Do not commit ROMs, fetched payloads,
generated build outputs, separately obtained proprietary assets, or credentials.
Do not upload ROM artifacts to Actions. Do not download a reference ROM.
Use the existing isolated checkout in cloud tasks; do not create worktrees unless
explicitly requested. Build with `scripts/prepare-baseline.sh`, then
`make -j2 compare`; use `make clean` before repeatability validation.

## Approved feature implementation handoffs

Use `implementation/README.md` for the approved-feature workflow and `implementation/TEMPLATE.md` when creating new handoffs. An implementation file is authoritative for Codex only when its exact scope is fully approved, its status is `Ready for implementation`, and its review is complete. Do not treat a draft, merely suggested behavior, or an unmerged handoff as permission to implement gameplay. If requirements conflict or are missing, stop and report the blocker.

Read the referenced handoff from the repository before changing code. Google Drive source links document provenance but are not prerequisites for Codex to execute an implementation task. Work in a separate topic branch, open a PR, document verification, and never auto-merge.
