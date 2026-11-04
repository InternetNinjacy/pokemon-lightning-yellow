# Evolution mechanics

This branch implements the user's supplied “Evolution Mechanics Implementation”
specification. Google Drive documents were inaccessible in this cloud task; the
explicit supplied requirements are authoritative, as they permit. No roster has
been reconstructed from partial descriptions.

## Integration and provenance

Starting commit: `426415d853e141960a5ecfc60eb8798fdd3cb2dd`, existing evolution
PR #4. New branch: `feature/evolution-mechanics`, reviewed as a stacked PR against
`feature/evolution-system-overhaul`. PR #2 (move split), #3 (RTC), and #4 were open
and unmerged at inspection. Their branches and PRs are not changed by this task.
Main was still `e04252ea0783510000e634486c0f2526a736f506`.

No custom Pokédex branch or PR exists in the repository at inspection, contrary to
the supplied statement that it was completed. Custom-number registration testing
is therefore blocked. This patch preserves existing National-Dex registration and
save structures; it does not create or change a roster/number mapping. Integrating
an eventual custom Pokédex requires separate review and rerunning registration
and save tests against that implementation.

Upstream remains pret/pokefirered `ce6406d8c2eaf46d543c8a2606de0c3cb1951982`,
English FireRed revision 0. All upstream notices and baseline SHA-1 are retained.

## Approved behavior

- Kadabra, Machoke, Graveler and Haunter retain trading and gain level >=40.
- The six approved trade-item pairs (Poliwhirl/Slowpoke + King's Rock,
  Onix/Scyther + Metal Coat, Seadra + Dragon Scale, Porygon + Up-Grade) use
  the existing generalized item lookup, Bag routing and evolution scene.
  Successful scenes consume exactly one Bag item; cancellation, invalid targets
  and National-Dex blocking consume none. Original held-item trade remains.
- Eevee retains its original three stones, Sun Stone to Espeon, Moon Stone to
  Umbreon, and friendship >=220 with the separately reviewed RTC dependency.
  Existing time boundaries remain noon–23:59 for day and midnight–11:59 for night.
  An absent/invalid clock disables timed routes safely; stones still work.
  No visual lighting, encounters, weather or clock-setting UI is added.
- Pichu evolves at 20; Cleffa and Igglybuff at 15; Smoochum at 30;
  Elekid and Magby at 25; Feebas at 20 without Beauty. Later stages are unchanged.
- Tyrogue qualifies at level >=15 and offers exactly HITMONCHAN, HITMONLEE,
  HITMONTOP in that order. Attack/Defense no longer select the evolution.
  Up/Down moves the cursor without wrapping, A confirms and B backs out.
  After selection, the ordinary evolution animation, cancellation, move-learning,
  stat recalculation, rename and Pokédex-registration code handles the result.
  A canceled Tyrogue can qualify again on a later level-up.
- Golbat/Chansey friendship and all unlisted evolution rows are unchanged.

The latest species exclusions supersede the older PR #4 specification: direct
Deep Sea Tooth/Scale use for Clamperl is disabled and their original field-use
metadata restored. Its original held-item trade table and battle item effects
remain. Wurmple/Nincada families and all other unlisted rows are unchanged. No
new changes are introduced for excluded species.

## Implementation

`EVO_LEVEL_CHOICE` is a table method; target lookup returns the first eligible
candidate only to launch the scene. The scene uses the same table to populate a
standard FireRed multichoice window before any target announcement. Confirmation
updates the target sprite/name and the existing task target. B exits through the
ordinary fade/cleanup without changing species or registration. No permanent
selection, new save field, or species-ID change is needed.

The window uses BG0, tiles 0x300–0x33B, frame tiles 0x2E0, palette 14 and existing
fonts/frame graphics. It closes before animation and move-learning menus. No
external interface code or new graphics were imported. Evolution-table record
size and seven-row stride remain unchanged from PR #4; saves retain their layout.

Existing Everstone restrictions, National-Dex evolution gating and legacy stone
cancellation policy are preserved. The newly approved Tyrogue menu does not bypass
that gating: before National Dex, Hitmontop is still blocked by the original
scene guard. Changing progression restrictions is outside this task.

## Validation and limits

Run the established three host test runners, a clean build, and
`python3 tests/emulator/run.py`, following README dependencies. Actions repeats a
clean modified build with byte-identical output and checks the unchanged pinned
upstream twice against the original SHA-1. No ROM/BIOS/save artifacts are uploaded.

Host tests execute production target lookup and deferred-consumption code with
stubbed state/scene. Separate static audits check exact approved table values,
choice ordering, excluded-item routing and all unchanged rows/item properties.
They are not presented as scene execution.

The mGBA runner executes compiled ARM code and normal scene/input loops. See
EMULATOR_VALIDATION.md and the PR for actual results, including all Tyrogue options,
stat independence, menu/animation cancellation, newly approved level thresholds,
registration and flash save/reload. It does not establish full overworld Bag
navigation, a complete playthrough, physical RTC or a second link-trade peer.
Custom-number integration is untested because the custom Dex is unavailable.

## Rollback and compatibility

Before merging, close this PR or discard only this feature branch to retain PR #4.
After an owner-approved integration, revert this PR's changes through another
reviewed PR, preserving its dependencies. No save migration is introduced. Saves
containing already evolved Pokemon retain those species after rollback; rollback
does not reverse completed evolutions. Future Dex and day/night changes must retain
the species-ID contract and review the existing National-Dex gating/time boundary.

No merge or automatic merge is authorized by this implementation or passing CI.
