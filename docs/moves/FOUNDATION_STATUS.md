# Move engine foundations — BTL-005-M1A

Owner authorized beginning implementation after resolving the three decisions in
[November5 authority](OWNER_DECISIONS_2026-11-05.md). This is an incremental M1a
implementation, not completion of the410-move redesign or the whole M1 milestone.
[PR40](https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/40), branch `feature/move-engine-foundations`, is stacked on tested PR39; main and the
existing rival/weather/Gym feature PRs are unchanged. No merge or auto-merge.

## Implemented

- Curse's type is Ghost; category remains Status. Every other battle-move byte,
  category, effect script and existing move ID is preserved by frozen audits.
- MOVE_NAME_LENGTH grows12→15, retaining the canonical names without abbreviation.
  gMoveNames row stride becomes16; the summary, move-relearner and TM-animation
  RAM buffers use the same named capacity. No saved Pokémon/SaveBlock field moves.
- Used-move messages have17 local bytes, allowing15 characters + `!` + EOS. The
  local arrays explicitly retain four-byte alignment for ARM halfword consumers. The
  controller wire payload remains16 bytes per text buffer, BattleMsgData64 bytes;
  raw name/ID transport and sizeof-based controller transfers are unchanged.
  BufferStringBattle reconstructs EOS at the expanded tail after copying the
  wire payload, so a full16-byte text payload cannot become unterminated.
- Summary names that exceed the pane's normal-font width use the existing small
  font. Native screenshots of all four long-name fixtures were visually inspected:
  complete names, PP and category labels remain legible, with no new artwork.
- Former50 animation proposals are now approved for implementation/playback, while
  runtime/composition flags remain false. Generation IV Payback/Dragon Rush policy
  is recorded, not misrepresented as implemented code.

## Verification and honest limits

`python3 tests/run_move_foundation_tests.py` audits eight precise source changes,
unchanged original IDs/names/save definitions and current owner provenance. It
normalizes only the frozen approved delta for historical checks; no prior expected
hash is replaced. All16 original host suites are retained alongside this17th.

`LIGHTNING_MOVE_FOUNDATION_ONLY=1 python3 tests/emulator/run.py`: native mGBA0.10.5
PASS330 guarded assertions after alignment/termination/DMA refinement; focused vendor battle309 PASS. Final full results in PR40. Test-only ROM-copy substitutions exercise13–15-char
names in real ARM StringCopy, used-move message expansion, actual TM allocation,
relearner list construction and summary buffers. Adjacent fields/canaries and all
original patched ROM bytes are checked/restored. No new moves are added as fixtures.
Screenshot reviewed locally; no ROM/save/screenshot binaries committed/uploaded.
Default suite retains all19 earlier groups plus the20th foundation group. Final
clean-build/full-suite remote conclusions are recorded in this PR's Checks and
review description; do not infer them from this focused result.

## Remaining M1 work / next implementation boundary

The original battle name cells are64px, but SCORCHING SANDS is74px in FONT_SMALL;
the relearner list is80px with an8px cursor indent. Those two layouts still need
adaptation and native visual/navigation testing before new long-name move rows
are enabled. This is a technical task, not an unresolved owner design decision.
No truncation, invented abbreviation, placeholder effect or incorrect playable
move is used to pretend M1 is finished.

IDs355–410, descriptions, effect/animation wiring and complete table coverage are
still pending; existing MOVES_COUNT355 remains correct. Original redesigns beyond
Curse, AI/new-effect implementation, new mechanics, TM metadata/distribution and
full M2–M8 acceptance remain pending. No56-move runtime/animation or real link/
hardware compatibility claim is made. Matching-build link testing is still needed;
unchanged wire/save widths do not prove compatibility with vanilla/foreign ROMs.

Rollback through a reviewed revert of this PR restores12-character capacity and
Curse???; no save migration is needed because no new move IDs or save fields have
been introduced. Preserve all parent feature commits. Drive sync remains pending.

The generated reconciliation now records the live battle-data SHA separately
from its historical baseline commit; the implemented Curse type is attributed
to the current source, not backdated to the old baseline.

Pre-tail-reconstruction full integration run: PASS513067 guarded assertions.
Final tail-reconstruction source has focused314/vendor309 PASS and all17 host
suites/3 checkers PASS; its separate full native/clean CI conclusions are in PR40.

Actual relearner public entry now opens, scrolls, cancels and returns to field
without changing moves. Its BG1 tilemap explicitly retains four-byte alignment
after larger name rows; the rendered original-name screen was inspected. This
is not a claim that future long relearner names fit the unmodified list geometry.
The prior full513082 pass predates this final DMA alignment safeguard; the final
code has its own clean/full validation recorded in PR40.

## Final local acceptance — production source aaf898e4

Full default20-group mGBA0.10.5 **PASS513097** guarded assertions. All17 host
suites/3 static checkers PASS; focused330 and vendor309 PASS. Final clean rebuild
identical SHA256e9fe4bd0034278dc289eef2a6a61dc1b4f7ef2c9ed32834dc836e4f5d6b68b1e.
Target ABI measured offsets0x264/0x2AF/0x2B0 for original/expanded/aligned BG.
This supersedes intermediate local counts above. Remote conclusions for the
source and any evidence-only tip commit are linked in PR40, not inferred.

## Subsequent M1b child

The separate `feature/move-engine-long-name-ui` child now adapts the battle and
relearner layouts. See [its status and evidence](LONG_NAME_UI_STATUS.md). The
remaining-layout section above describes PR40 in isolation; M1b does not change
PR40 or enable the56 new move IDs/effects.
