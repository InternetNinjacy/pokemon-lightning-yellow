# Codex execution handoff — verified November6,2026

Main audited: `15dacb95424ffd394113849d10c3aa2f78c343ba`.
Standing owner authorization is in AGENTS.md (PR55): implement and merge verified
approved work without routine permission. Preserve specific design/asset holds,
all checks, existing protections and no ROM release. No worktrees or force pushes.

## Completed / merged

- Historical27/29/30/31/35/36/39–43/53 are merged, not pending implementation stacks.
  Main already retains252 Dex/types/evolutions/RTC/daycare/vendors/Poké Balls/
  fossils/Ranger/rival/weather/Gym Castform and move foundations/UI/PP work.
-55 policy→b43f92cf;56 Roar→844b450e;58 Viridian sign→d97712dc.
-59 Aron50%/Magby50% eggs and TangelaLv25TANGO→708f197a.
-60 six Lorelei/Bruno/Lance initial/rematches→66117462 (Agatha NOT implemented).
-61 Ground-only Sandstorm/Rain Zap Cannon→15dacb95.
- Main tree exactly equals verified PR61 head22314c35 tree
  `d6af90108bce62c464aca34297a026378a307419` (history differs, content identical).
- Exact-head CI runs SUCCESS:56=42215345347;58=42217742516;
  59=42220799681;60=42221063828;61=42222085136. Each retained both jobs.
- Local combined latest28-group native PASS753726;24 hosts/3 checkers PASS;
  clean identical SHA256bba80adbe5de31b285e704f202cdda61e8aa4f4ca2256c1d8762082c73984b77.
  See individual feature docs/append-only ledger for actual limits. No full campaign,
  fair-difficulty balance, real link-peer or cartridge certification.

## Work in progress (do not duplicate)

-63 `feature/pin-missile-dragon-rage`, sourcefbe59595: approved single55/30% poison
  Pin Missile, native user-level Dragon Rage, no ID/PP-max/learner changes.
 25 hosts/3 checkers PASS; focused native314 PASS; clean identical
 `f8d6d10d190bbb3e497406105954360713dce1a9371b94e8fe3122b83531a415`.
  Full29-group local and live CI pending at this snapshot. Check current outcomes,
  integrate workflow64 if merged, then merge63 only on passing applicable checks.
-64 `maintenance/early-mgba-dependency`,143f05c0: install libmgba-dev with initial
  modified dependencies, bounded APT network/retry/lock waits, later dpkg check.
  Actual CI pending. All host/native/clean/upstream gates and30/20-minute limits
  unchanged. Fix was prompted by PR62 cancellation during24m35 package install;
  emulator tests never began, not an assertion failure. See CI_DEPENDENCY_STALL.md.
-62 this documentation PR: workflow guidance plus populated handoff/tracker.
  Includes64 dependency until64 merges; do not restore the empty template.

## Immediate resume commands (existing cloud checkout)

```
git status --short --branch
gh pr checks 63
gh pr checks 64
gh run list --branch main --limit 3
python3 scripts/check-integration-registrations.py
```

For game changes, follow README/toolchain setup; this worker has packaged tools at
`/workspace/lightning-toolchain/root`, agbcc already prepared. Set PATH to its
usr/bin, CPATH to usr/include, LD_LIBRARY_PATH/LIBRARY_PATH to usr/lib/x86_64-linux-gnu,
MGBA_PREFIX to that root; PKG_CONFIG_PATH to its usr/lib/x86_64-linux-gnu/pkgconfig.
Then `make -j2`; focus `LIGHTNING_ORIGINAL_EFFECTS_ONLY=1 python3 tests/emulator/run.py`;
full `python3 tests/emulator/run.py`; clean rebuild/repro; run every registered host.
Never alter the4-million ARM-call instruction guard or normalize runtime ROM data.

## Next three approved work units, after current PRs

1. Finish/merge63 and64 with actual CI; validate main content/CI and update tracking.
2. Approved remaining original move effects/targets/global Electric paralysis rule:
   SCARY_FACE,SCREECH,FAKE_TEARS,THUNDER_WAVE,SPITE,GRUDGE after63. Exact register,
   overlays and docs/moves/README.md are available; no new design question implied.
3. Begin first complete new-move engine slice from56 approved additions (no
   placeholder IDs/effects). Or independent issue45 Rocket subset/prototype51 if
   engine dependency blocks. All56 additions remain unimplemented at this snapshot.

## Genuine holds / unimplemented scope

- Agatha two Alakazam: approved Inner Focus is slot1, current even-PID trainer path
  selects slot0. Specific contracts forbid unapproved customization; owner question
  pending for a narrow native-slot selector. Continue other tasks; no substitute.
- Follower54 audit:252 candidate base paths verified, permission for FireRed/AI use
  explicitly required by upstream aarant; no imported assets or follower gameplay.
  Opening Pikachu and Parcel/Eevee gifts are not integrated. Supplied export's
  encounter audit line240 leaves Eevee level/full-party handling unapproved; issue38
  confirms SUE nickname. Do not invent these details or ask to reupload available exports.
- Draft57 Trainer candidates are NOT CODEX READY;47/282 historical teams unresolved,
  heuristic510 candidates are not approved exact parties. Do not merge/implement them.
- Issue47 global Ice Body/Overcoat/Drizzle slot replacements/inheritance deferred;
  code/events/individual persistence and SOW exact positions/aftermath still need work.
- Rocket45/44, item shop/placements,252 stats/abilities/learnsets, encounter slots/
  levels/weights, broader Gyms/quests remain only to extent exact approved specs exist.
- Lighting49/51 authorizes reversible real-emulator prototype/screenshots; final
  RGB production values remain owner hold. No time-exclusive encounter design.
- Direct Drive writing unavailable; supplied40 authority exports are available.
  Canonical dashboard/ledger is Git; Drive sync pending. No ROM artifacts/releases.
