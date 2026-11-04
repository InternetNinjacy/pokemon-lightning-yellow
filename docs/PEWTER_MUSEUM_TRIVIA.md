# Pewter Museum anime trivia: shiny Feebas Egg

Status: owner-approved gameplay; code on this feature branch; GitHub feature-source-build compile PASSED on November 3, 2026 (workflow run 41757743873); emulator acceptance and merge pending.
Project: Pokémon Lightning Yellow only. Do not import unrelated Pokémon project decisions.

## Player experience
- Location: first-floor paid exhibition area of Pewter Museum, on the far side of the ¥50 admission attendant. No Gym-badge check.
- Added stationary NPC reuses the existing admissions clerk's `OBJ_EVENT_GFX_WORKER_M` overworld graphic (existing male-coded asset; character dialogue does not specify gender).
- Free optional challenge, Yes/No entry, fixed three questions in order. Wrong answer immediately exits; unlimited retries restart at question one.
- Q1: "Was Rattata the first Pokémon Ash ever caught in the anime?" NO. Correct explanation: Caterpie was first.
- Q2: "Have we ever seen Brock's eyes open in the Pokémon anime?" YES. Correct explanation: they have opened a few times.
- Q3: "Does Misty have exactly two sisters?" NO. Correct explanation: Daisy, Violet, Lily are her three sisters.
- Answer sequence NO / YES / NO.
- Completing all three stores a permanent quiz-win flag immediately.
- A full party leaves the reward pending, without rerunning the questions; player must return with an empty party position.
- One Shiny Feebas Egg per save file, ever. Once awarded, a separate permanent received flag prevents duplication.
- The NPC refers to a mysterious Egg and never reveals its species or Shiny status.
- Standard species egg cycles and standard hatch process; no customized hatch distance. Existing Feebas level-20 evolution remains unchanged.
- Winning/receiving independently persist through save/load and map exits.

## Technical design
- Two persistent flags allocated from previously unnamed slots 0x0AF and 0x0B0 on this branch; check stacked project PRs before integration.
- New `CreateShinyEgg` creates a standard egg with fixed personality PID whose upper/lower halves produce true Gen III shiny calculation against player OT. Preserve egg cycles, nickname, ball, language, original special-egg met location and hatch system. No change to normal gift/daycare/Four Island/Route 5 Shiny odds.
- `ScriptGiveShinyFeebasEgg` rejects full parties rather than sending to PC, and uses ordinary `GiveMonToPlayer`.
- Existing museum clerk admission fee, original Old Amber event, map scripts, and other visitors must remain intact.

## Acceptance checklist
1. Entry is gated only by the existing ¥50 admission flow; NPC is reachable before the Pewter Gym.
2. "No" to the initial participation prompt exits without changing flags.
3. Wrong answer at each of Q1, Q2 and Q3 ends immediately; retalking begins at Q1, indefinitely.
4. Correct NO/YES/NO displays correct explanation after every answer, then victory.
5. Full party at victory: win flag set, received flag unset; saving/reloading and changing maps retains pending reward.
6. Empty party slot: an already-won challenge awards exactly one Egg without rerunning questions; further conversations never award more.
7. Save/reload before hatch: exact stored OT/personality verify as truly Shiny using engine predicate; egg is Feebas, standard egg cycles/hatch level.
8. Hatch yields Shiny Feebas; evolution yields Shiny Milotic. Normal eggs, bred eggs and wild Shiny odds unchanged.
9. Validate NPC tile accessibility and text-window overflow in emulator; sprite intentionally matches existing admission clerk.
10. Build using existing GitHub workflow and perform emulator acceptance. No merge without owner approval.

## Integration warning
This branch starts from `main`. Other feature PRs are independently open. Reconcile flag allocations, sprite/event placement and C script specials against the integration branch before merging. No ROM file, release patch, emulator capture, or passing build is claimed by the existence of this document.

## Verified build
- GitHub Actions Feature source build passed: https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41757743873
- This compiles the modified FireRed source. It does not prove the map NPC is reachable or the egg survives save/load and hatches Shiny. Emulator acceptance remains required.
- The separate FireRed baseline workflow compares against unmodified vanilla SHA and is not a feature-build acceptance test.

## November 3 static acceptance progress
- Added scripts/check-pewter-museum-trivia.py and invoked it in GitHub Actions. Map-data collision and adjacency assertions, trivia answer branches, text label references, state flags and ordering passed on workflow run 41758818297.
- Compared flag definitions 0x0AF and 0x0B0 with active feature branches 5, 7, 9, 10, 11 and 12; no competing allocations in their flag headers at review time. Other changes still require re-audit before integration.
- True in-game sprite tile reachability and dialogue display still require emulator acceptance. Static checks alone are insufficient.

## November 4 integration repair and native acceptance

The original `(23,6)` placement was in the isolated back room, not the paid
exhibition. Collision-path inspection and actual admission/walking tests identified
and fixed that error: the existing worker now stands at `(11,4)`, reachable from
the paid exhibit at `(11,5)` after the unchanged50 ticket. No badge requirement,
new artwork, alternate entrance or museum reward changes were introduced.

The feature is now stacked on the complete approved typing/Blaine/vendor/ball,
fossil/daycare/evolution/RTC/split stack. Existing specials retain their indices;
`ScriptGiveShinyFeebasEgg` is appended after them and before `gSpecialsEnd`.
Older preservation hashes remain frozen, with only exact reviewed Museum hunks
normalized. The prior empty label-validation regex now checks real script labels.

Native mGBA tests execute paid admission and real walking/A interaction, decline,
each wrong answer and retry, all correct answers, full-party deferral, pending
flash save/load, exactly-one receipt with no quiz fee, stored true-Shiny identity,
ordinary hatch animation and controlled-level evolution to Shiny Milotic followed
by flash reload.32 varied OT/RNG fixtures additionally verify actual encrypted
Shiny Eggs. Run `LIGHTNING_MUSEUM_ONLY=1 python3 tests/emulator/run.py` for focused
coverage or the complete default suite. Host assertions remain source-level
checks; no full-world playthrough, link peer or physical cartridge is claimed.
