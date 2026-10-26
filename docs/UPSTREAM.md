# Upstream provenance and baseline

Source: https://github.com/pret/pokefirered

Imported commit: `ce6406d8c2eaf46d543c8a2606de0c3cb1951982`.

Compiler: https://github.com/pret/agbcc at
`da598c1d918402c42c0c0d7128ba14567f3175e9`.
This is a build dependency, not another imported game project.

The upstream source tree and build structure are retained. Upstream README is
preserved in UPSTREAM_README.md; INSTALL.md and embedded license/copyright notices
remain intact. The root README and CI are project-specific, AGENTS.md and scripts
are added, and three upstream multiboot ROM binaries are deliberately omitted.
No new blanket license is asserted over upstream game code or assets. Tool
licenses remain in their respective tools directories. Attribution does not
imply Nintendo or pret endorsement or grant rights beyond applicable licenses.

The build script fetches the three multiboot payloads (`data/mb_berry_fix.gba`,
`data/mb_ereader.gba`, `data/mb_colosseum.gba`) from the exact upstream commit as
ignored, ephemeral build inputs. They are required by upstream assembly and are
not committed or published as artifacts. Source graphics/audio already present
in the requested upstream are retained; no separately obtained Nintendo assets
are imported. Run builds only where you are authorized to use these inputs.

No full reference ROM or secret is required for this upstream's source build.
`make compare` checks the generated FireRed revision 0 ROM against the upstream
known SHA-1 `41cb23d8dccc8ebd7c649cd8fbb58eeace6e2fdc`, without downloading an
original ROM. If a later tool needs a reference ROM, it must be supplied privately
in an authorized environment, never committed, uploaded, or placed in CI logs.
Public/fork CI cannot depend on access to such a private input.

Source and compiler revisions and the comparison hash are pinned. The runner is
Ubuntu 24.04, but security-updated apt package versions may change; the workflow
records installed versions. Byte identity is enforced against the upstream hash
on each of two clean builds, rather than claiming a hermetic toolchain image.
