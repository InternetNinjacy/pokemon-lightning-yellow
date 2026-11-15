# Experimental playtest branch

Owner direction recorded 2026-11-15: create a separate branch for combining existing Lightning Yellow work, compile an experimental build without automated gameplay testing, then fix problems reported during owner playtesting. Preserve the original development state for returning to the prior process.

## Exact recovery point

- Repository: InternetNinjacy/pokemon-lightning-yellow
- Original branch: main
- Pinned starting commit: 02240690581eb70d70219024fb7f55da2882308b
- Recovery branch: recovery/pre-playtest-2026-11-15
- Experimental branch: experimental/playtest-2026-11-15
- The recovery branch must remain at its starting commit. Main may advance independently; it is not the frozen snapshot.

To resume the original development state, use the recovery branch or create a new topic branch from its exact commit. Do not reset or force-push main. Keep experimental saves separate; switching code does not roll back or guarantee compatibility with newer save data.

## Experimental workflow

1. Inventory live PR heads and dependencies. Identify changes already in main, stacked duplicates and superseded implementations before merging anything.
2. Combine existing owner-approved gameplay implementations on this experimental branch. Preserve the original PR branches. Incomplete features remain labeled incomplete; do not invent missing designs or include unlicensed assets.
3. Resolve source conflicts and compilation errors. Do not run automated test suites as a prerequisite to this private experimental build. Do not weaken or remove tests, alter repository protections or imply a failed or unexecuted check passed.
4. Record every included PR, exact source SHA, conflict resolution, build command, build result and output hash in this file before handing off a build.
5. Owner plays a fresh new game and reports the build version, location, steps, expected result and actual result. Keep normal in-game saves and save states separate from previous builds; keep a copy of the save before reproducing a fault.
6. Fix the reproduced problem on this branch, compile a new version and document it. Compile success establishes only that the source built, not that the game boots or is fully playable.

Existing main acceptance rules remain applicable to any later promotion back to main. This experiment does not award completion points or certify a release. Preserve all 50 permanent IDs and the current 461 earned / 251 remaining / 712 total accounting.

## Setup status

Both remote branches were created at the pinned main commit and read back through git ls-remote. This setup commit changes instructions and documentation only. No feature integrations, automated tests, compilation or ROM generation have occurred yet. No PR was opened for this setup because existing pull_request workflows would initiate tests. Existing workflows remain unchanged; their observed push filters do not match experimental/** or recovery/**.

Live open-PR inspection found overlapping gameplay stacks and validation-only drafts. The next engineering task is a pinned inclusion manifest, with duplicate/superseded changes resolved before integration. The next owner setup input is the laptop operating system, to select appropriate emulator and build instructions.

## Laptop walkthrough

The immediate owner task is to open ChatGPT on the laptop, sign in to GitHub if needed, and identify Windows, macOS or Linux. Then select the matching setup instructions. No source upload or terminal work is required to create these branches; they already exist remotely. Build tooling is needed only if compiling locally. Distribution and proprietary asset constraints still apply; do not upload ROMs or proprietary assets into Git or Actions artifacts.
