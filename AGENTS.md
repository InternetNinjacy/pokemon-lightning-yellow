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
ROM code, evolution/Dex scenes and selected wild-battle scenarios. It does not
certify every battle mechanic, full playthroughs, link peers or physical cartridges. Preserve the guarded test RAM and target-generated
ABI checks; never disable assertions to make the suite pass. No proprietary BIOS,
ROM or save-state artifacts may be committed/uploaded. Passing CI does not authorize
merging or enabling auto-merge.

The newer evolution-mechanics specification supersedes PR #4 for this branch.
Read docs/EVOLUTION_MECHANICS.md. Only six direct trade-item combinations are now
approved; Clamperl direct-use alternatives are excluded. Keep excluded families'
original data and preserve all unrelated behavior. Tyrogue uses the level-choice
table at >=15 with Hitmonchan/Hitmonlee/Hitmontop in that order. Do not bypass
National-Dex gates or claim custom-Dex integration until that feature is supplied.
Run all host suites and mGBA integration tests after scene/menu changes. Keep this
work separately reviewable on feature/evolution-mechanics; never merge dependencies.

Standing owner authorization supersedes older blanket preparation-only restrictions:
implement finalized specifications on reviewable feature branches without repeated
permission requests. Do not invent pending designs, merge PRs, or enable auto-merge.
The roster is now 252: insert Castform at #233 and shift old #233–251 by one.
Read docs/CUSTOM_POKEDEX.md. Preserve canonical IDs and save flags. Castform keeps
Generation III Normal typing, Forecast and original forms. Its exactly-one Celadon
Gym trainer assignment is pending trainer/level/moveset details; do not choose them.
Run tests/run_pokedex_tests.py as well as existing suites and mGBA after Dex changes.

The owner authorized the wild-double capability test. Read docs/WILD_DOUBLE_BATTLES.md.
Work on feature/wild-double-battle-capability stacked on PR #6. Route 1 and Mt. Moon
1F are experimental test maps, not finalized production placements. Preserve the
first-draw Repel policy, independent second draw, 5% weighted pair, single fallback,
roamer priority and survivor-based captures. Run tests/run_wild_double_tests.py and
all earlier suites, including full mGBA, after encounter/battle changes. Do not
claim compatibility with unfinished dynamic weather or every battle move. No merges.

The approved consolidated stabilization plan supersedes older instructions to retain
National-Dex evolution gates and noon-based friendship boundaries. Read
`docs/PHASE_ONE_STABILIZATION.md`: twelve approved typing overrides, roster-aware
pre-National evolution access and functional DAY [06:00,18:00) are authorized.
Keep excluded-species gates, timed-evolution failure on invalid RTC, and all parent
features intact. Run every host suite and the full compiled-ROM mGBA suite.
Canonical project tracking lives separately on `docs/project-coordination` in
`docs/PROJECT_DASHBOARD.md`, `docs/MASTER_IMPLEMENTATION_CHANGE_LEDGER.md` and
`docs/project-tracking.json`. Update it at milestones/focus switches/session end;
Drive synchronization is pending, not completed. Do not invent unavailable Route 5,
move redesign, weather, visual or Phase Two details. No merges or auto-merge.

The complete owner-supplied November2 Route5 handoff authorizes DAY-001..004.
Read docs/ROUTE5_DAYCARE.md. Use versioned400-byte reserved storage without moving
existing save fields; never erase unknown/corrupt daycare data or re-import the
read-only legacy recovery copy after migration. Preserve independent Four Island
parents/pending state, one party hatch clock, ordinary breeding and untagged eggs.
Only Route5 eggs carry their persistent Shiny intention through matching-build
ownership changes. Run all host suites including run_daycare_tests.py and the
complete mGBA suite. Do not invent Route5 Ditto rate/levels or claim a real link
handshake from ownership-data fixtures. No merges, auto-merge or ROM release.

The November2 world/fossil update authorizes the Route10 Claw reward and Cinnabar
unchosen fossil/revival extension. Read docs/FOSSIL_REWARDS_AND_SPECIES_AUDIT.md.
Preserve immutable MtMoon choice flags, existing reward bit0x2FA and independent
daycare/save layout. Run tests/run_world_reward_tests.py plus all prior suites
and complete mGBA. Species audit findings are unresolved conflicts, never approved
off-Dex exceptions; --check prevents new findings while --strict still fails.
Do not invent Blaine levels/moves or revert Gyarados Water/Dragon without resolving
the explicit Water/Flying strategy conflict. Missing ZIP/Poke Ball specifications
and draft encounter values are not inferred. No merges, auto-merge or ROM release.

Mystery Vendor Network is authorized independently of pending starter changes.
Read docs/MYSTERY_VENDOR_NETWORK.md. Keep the exact approved ten NPC objects,
level5/700/uniform-without-replacement pool and90% genuine Shiny policy.
Preserve foreign Mystery identity with correctly rekeyed secure data, ordinary
traded EXP/obedience, existing flags/save layout and legacy Magikarp reservation.
Run tests/run_vendor_tests.py and all preceding suites, plus full mGBA.
The new specials must stay inside gSpecials..gSpecialsEnd; never disable ABI or
assertion guards. Do not mix starter/Poke Ball work into this PR. No merges.

BTL-006 Poké Ball implementation is explicitly authorized beyond readiness.
Read docs/POKE_BALL_REDESIGN.md. Preserve old item IDs, four-bit capture codes and
all artwork; Snow item375 uses separate stored code13. Do not widen save fields
or assume every ball is <=Premier. Keep exact current-weather/origin/cave rules,
nonstacking bonuses, eight-species Premier whitelist, atomic purchase bonuses,
Oak pending gift and unchanged emergency refill. New capture tests join all old
host/full mGBA suites; old feature preservation hashes remain with only frozen
approved deltas normalized. No unrelated trainer/starter/weather redesign, merges,
auto-merge or ROM release. Owner's latest instruction supersedes readiness stop.

PKM-007 supersedes the older twelve-rule typing snapshot: use all35 explicit
rules in tests/specifications/approved_type_overrides.json, preserving the other
species fields. Read docs/TYPING_CORRECTIONS.md. The supplied authority exports
are available; unavailable direct Drive writing does not mean missing designs.
Pure Bug Pinsir supersedes Bug/Steel; Gyarados stays Water/Dragon. Run the new
run_typing_tests.py, all host suites and full mGBA. Keep the24-pair delta isolated
from pending stats/ability/learnset, encounter and weather redesigns. Preserve
historical hashes via the explicit approved normalization. No merges/auto-merge.
`make -j2 compare`; use `make clean` before repeatability validation.

## Approved feature implementation handoffs

Use `implementation/README.md` for the approved-feature workflow and `implementation/TEMPLATE.md` when creating new handoffs. An implementation file is authoritative for Codex only when its exact scope is fully approved, its status is `Ready for implementation`, and its review is complete. Do not treat a draft, merely suggested behavior, or an unmerged handoff as permission to implement gameplay. If requirements conflict or are missing, stop and report the blocker.

Read the referenced handoff from the repository before changing code. Google Drive source links document provenance but are not prerequisites for Codex to execute an implementation task. Work in a separate topic branch, open a PR, document verification, and never auto-merge.

For the November4 consolidation, the owner explicitly authorized merging all
existing project PRs and continuing approved unfinished work. This supersedes
older merge prohibitions for that consolidation only. Keep the repository
auto-merge setting enabled, validate integrated heads, preserve recovery history
and do not invent missing feature designs. Future unreviewed releases remain
unauthorized. Approved-feature handoffs supplement, rather than erase, supplied
exports and explicit owner approvals.
