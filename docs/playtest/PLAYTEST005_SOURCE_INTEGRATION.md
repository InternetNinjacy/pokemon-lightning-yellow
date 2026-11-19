# Playtest 005 — source integration staging (2026-11-18)

This branch is PRIVATE SOURCE STAGING only. No ROM, artwork, permission records, or binary assets may be committed. No emulator, gameplay tests, broad CI, or main merge is authorized.

## Exact starting points
- Production main snapshot: `4fd313fe4dfed0412f2e91ed7e876a9bf0b717fc` (includes PR #286 Misty four-member Double Battle and PR #285 Viridian School TM52).
- Playtest004 private PR #258 source: `beac5390d49b4717582ab05dfad3a6d9276a3795` (open draft, not merged).
- Historical Playtest004-r3 compiled source: `579ef016def8116a2ecf44acccd0e5e72f501247` (historical build evidence only).

## Confirmed differing files (blob comparisons)
| File | main blob | private PR #258 blob | treatment |
|---|---|---|---|
| src/follower_controller.c | 3eb56279b94c8936bb28877141705c626c228654 | 153345fc7172ef887a81956e498ff3ca2fe6c8e1 | Manual functional merge; do not overwrite main wholesale |
| src/follower_graphics.c | 1d747b8d31373cb45d6f61be1a82db922d36f7e9 | 054f74a4d584c7db802dd5a945b4537257b3f27f | Manual merge with private 255-descriptor asset handling |
| src/follower_assets.c | 4aef3e672432e3927f2c15c9d9ff9202d6face3e | e527ec0bdeca45407a7734dc109f376a7e20d9e6 | Manual merge, preserve private cache requirements |
| src/approved_gifts.c | 1508fe9eb4952566dec4adede6446d62c57521d3 | 8004ab9124f6e88ebab339b789e26a8bca360786 | Preserve approved production gifts and private Oak Sue |
| src/data/pokemon/level_up_learnsets.h | 5d10152055b7f4bd1180642ecb9f7e5214e246bd | 9643516433d77b2e413b0355b893245fce36814e | Preserve final approved learnsets; do not bulk-replace |
| include/constants/flags.h | 76cbec996d86e1c680775c5c39bcb0b33cab6e9b | e6c6ff73edfe265a9b71f372df01c55ba837c266 | Audit for event-flag collisions before combining |

Main contains `src/follower_starter.c`; it is absent on the private PR #258 branch. Do not delete the production file.

## Preserved source artifact
- `scripts/build-private-playtest004.py` copied byte-for-byte from PR #258 as a reference for a future Playtest005-specific script. It is NOT a Playtest005 builder yet and has not been executed.

## Open blockers
- Private 765 artwork inputs and prior ROM/build artifacts unavailable in the current environment.
- ARM/agbcc toolchain unavailable in the current environment.
- File-by-file production/private runtime integration not yet completed.
- No Playtest005 ROM or manifest exists.

Accounting remains 50 task IDs, 461 earned / 251 remaining / 712 total; no acceptance claimed.

## Source-only integration batch (2026-11-18)
- Commit `2644188011d4d26108a9b69f42ddf8bd730c0c26`: production follower resolver now accepts an opt-in `FOLLOWER_ART_REGISTRY` include override, defaulting to the existing tracked zero-art include. This is a source switch only; it does not create artwork or activate private art by itself.
- Commit `d7041ad7fabc0971db196df29eb46d98fea062a6`: preserve PR #258 private pinned-art converter at `scripts/build-follower-assets-private-playtest005.py`, without overwriting main's production converter. It still requires private cached assets and permission records. It has not been executed or wired into a Playtest005-specific make target.
- Confirmed main already defines `ScriptGiveSueEevee` with the SUE nickname and a follower starter implementation. Main's Oak lab and follower-starter logic still select `SPECIES_PIKACHU`, whereas the owner requires private Pichu behavior. Do not blindly replace main's lab or starter files; a coordinated private source overlay and updated opening flow is needed.
- Main's tracked `src/follower_assets_generated.inc` is the zero-art registry; it does not embed 255 descriptors. The private 255-descriptor registry must be regenerated locally from the 765 pinned private inputs. Neither the descriptors nor artwork are present as compiled private bytes in GitHub.
- Main follower controller and graphics implement additional object ownership/lifecycle logic absent in the older private copies. Retain main and merge only specific private behavior after examining each delta.
- No tests, builds, emulator, CI, merges, or public binary uploads performed. No gameplay acceptance claimed.
