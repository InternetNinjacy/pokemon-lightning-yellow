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

## Playtest 001 build status

Compilation SUCCESS. Build ID: LY-playtest-001-20261115. The output is Lightning_Yellow_Playtest_001.gba (16,777,216 bytes). Automated tests and emulator execution were deliberately not run. Compilation does not establish boot or playthrough success.

The pinned [61-PR inclusion manifest](playtest/2026-11-15-inclusion-manifest.json) records exact reviewed source heads, existing-main/superseded implementations, selected production deltas, conflict resolutions, known gaps and the ROM SHA-256. Production deltas came from #173, #235, #158, #181, #183 and #54. Original PR branches, main and the exact recovery branch remain unchanged. Existing tests, workflows and formal task accounting are byte-identical to the starting baseline; new PR validation files were not imported for this experimental build.

The first compilation rejected duplicate move description declarations, which were resolved by retaining accepted main descriptions. The second compilation completed successfully. The shared all-other-recipient loop and existing main spread replay guard were composed explicitly so the added Discharge/Petal Blizzard do not use both recipient controllers. These are source integration decisions, not gameplay test evidence.

Follower code is present, but its external art registry remains empty. Move IDs 402, 404 and 405 remain inert/unimplemented. No missing design, artwork permission or distribution was inferred.

No ROM, fetched build payload, generated binary or credential is committed to Git or Actions. The owner explicitly requested a private downloadable playtest ROM in the originating conversation.

## Laptop walkthrough

The owner uses Windows without administrator access and already has working emulators. Integration and compilation were completed in this environment. No laptop build-tool installation or emulator setup is needed. Download the ROM, start a fresh new game, retain separate saves and report the build ID with any reproduction steps. Distribution and proprietary asset constraints still apply; do not upload ROMs or proprietary assets into Git or Actions artifacts.


## Playtest 002 — snow persistence correction

Owner continued the source-identified #248 correction on November 15, 2026.
Gameplay source commit: `b9af0975c4ba9a1b5d486c324928148686d14aa6`; parent Playtest 001 `07158082fb9dfc5c711b4732c9d7878f56d6fb8e`.

Only `src/field_weather_effects.c` changes gameplay: advance the existing shared
`snowflakeTimer` once in `Snow_Main`. Flakes can then pass the original `>18`
restart gate and use their unchanged reset/movement/cleanup. Update the obsolete
never-incremented comment. No rain, sunlight, palette, Hail, map weather weights,
field-to-battle mechanics, particle artwork/count, tests, workflows or assertions
changed. Main, recovery and original feature branches remain untouched.

Compilation `make -j2` SUCCESS with the already-prepared pinned agbcc and binutils.
No automated gameplay tests, emulator execution or rain retest. Build ID
`LY-playtest-002-20261115`, file `Lightning_Yellow_Playtest_002.gba`,
16,777,216 bytes, SHA-256 `230c9ec7d1e5376fb1489b10d5d88427cad56e7f8693670e42a83d89616081a3`.
Exact details: [Playtest 002 manifest](playtest/2026-11-15-playtest-002-snow.json).
Compilation is not a successful snow playtest: #248 remains open awaiting owner
confirmation of sustained snowfall. Existing owner-approved successful rain/Sun,
snow palette/battle Hail and Parcel/Pokedex/Eevee delivery remain accepted.
WRL-010 and Pikachu/SUE OT corrections stay queued. No points awarded; 50 IDs,
461 earned / 251 remaining / 712 total unchanged.
