# WRL-010 × Playtest004: exact-head private source integration gate

**Source heads inspected (no merge performed):**

| Layer | Exact head | Role |
|---|---|---|
| Follower implementation PR #54 | `ed250d6c95a7293159ca192297a6ae926bb9a4eb` | follower state, menu, graphics object lifecycle, movement |
| Stacked art/build PR #257 | `44281df93db51bc0f14084c82484e476ba45d52d` | full 765 input SHA pinning, local art conversion/build gate |
| Playtest004-staged PR #256 | `1ea7c53c3ec0044e2343a96611c95a6dec07e4c7` | approved Pichu and Sue updates; no ROM |
| Playtest003 baseline | `0e1b2f584ab3961d15e0080cec4e596f0aa862c5` | prior owner ROM content |

**Exact filenames cross-reviewed using GitHub PR change lists:**
PR #54 changes 44 paths; PR #256 changes six paths. The only shared path is
`src/approved_gifts.c`. PR #257's seven paths overlap PR #54's two
expected build-source paths (`scripts/build-follower-assets.py` and
`scripts/verify-follower-assets.py`) by design; PR #257 changes **none**
of PR #256's six paths.

**CRITICAL: do not resolve `src/approved_gifts.c` by choosing old PR #54.**
PR #54 has `ScriptNameSocketStarterPikachu`, assigns its follower
using `SPECIES_PIKACHU`, and makes Sue with the *player's* OT.
These are historical implementations superseded by PR #256's approved
new-playtest behavior. PR #256 has
`ScriptSelectStarterPikachuFollower` using `SPECIES_PICHU`
(the function's old identifier remains for exported script compatibility),
the owner's native Pikachu/Pichu naming editor, and the **secure OAK
foreign-OT delivery of Sue**, including party and PC storage.
Preserve the entire PR #256 implementation of this file when combining,
including its C header dependencies. Never reintroduce the old forced
SOCKET nickname, player-owned Sue, or the zero-art fallback in a
follower-enabled *private* build.

Also preserve PR #256's approved lab scripts and text: Oak gives Lv5
Pichu with Tackle/Thunder Wave/Growl; after Parcel he grants permission
for a physical Sue pickup from the leftmost remaining lab ball. Successful
pickup only clears the ball and retains the securely foreign-owned
EeVee. Preserve Pichu/Pikachu/Raichu 309/405/534 base stat totals and
the unchanged rival Dratini. The staged Playtest004 manifest is
`docs/playtest/2026-11-15-playtest-004-staging.json`, status **NOT BUILT**.

**Protected sequencing for a future private integration:**

1. Re-read **live** heads of PR #54, #257, and #256 before any source
   integration, and refuse stale commits or concurrent owned edits.
2. Fork a new *private integration source branch*, rather than rebasing,
   force-pushing or merging these three owner/topic branches or main.
3. Start from Playtest003's approved source, layer the current PR #54
   follower engine, PR #257's build/provenance changes, then explicitly
   retain PR #256's complete `src/approved_gifts.c`, Oak lab scripts/text,
   learnsets, species stats and staging note. Reconcile future changes
   by actual hunks; GitHub's file-name intersection is not semantic
   compilation or API compatibility proof.
4. Only in an art-permitted isolated local checkout, populate the
   original pinned cache, record real artist/source permission scope,
   and use the explicit `make private-followers-rom` target. Verify
   every source against the independently fetched 765-blob Git-tree
   manifest and a nonzero 255-entry registry. Never commit generated
   pixels, ROM, save, BIOS or proprietary binary.
5. Hand the actual private ROM and SHA-256 to the owner for **owner-run
   gameplay playtesting** only when the build really succeeds.
   This document does not claim compilation, UI, graphic visibility
   or 252 follow-behavior acceptance.

**Open blockers:** accessible records describe owner-reported permission
but do not contain the underlying per-contributor grant and attribution;
a native GBA build environment containing the permission-cleared assets
is unavailable here. The current Playtest003 ROM and Playtest004 staging
remain unchanged; no gameplay testing, merge or points awarded. Preserve
50 IDs and 461 earned / 251 remaining / 712 total.
