# Elite Four implementation — GYM-009

Authority: issue50, eight merged `implementation/{lorelei,bruno,agatha,lance}-*`
contracts (November4/5), November6 autonomous execution. Approved teams never
substituted. Current branch implements the **six independently executable Lorelei,
Bruno and Lance initial/rematch parties**,32 members/128 ordered moves. Both
Agatha parties remain unchanged pending explicit alternate-native-slot resolution.

Only production file changed: `src/data/trainer_parties.h`, exactly six arrays.
Preserve all other trainer parties (including Champion/rival/Agatha), metadata,
250 initial/255 rematch IV fields, Singles, two Full Restores, original three AI
flags, native scripts/door flags/rematch gating/sprites/music. Lance has six entries
both times, same approved species/moves/items/abilities, rematch levels only.

## Genuine Agatha dependency

`CreateNPCTrainerParty` builds every personality with low byte0x78/0x88 (0x80 for
Doubles) and only adds nameHash<<8. Thus all trainer PIDs are even.
`CreateBoxMon` selects native slot by personality&1. Alakazam's slot0 is
Synchronize,slot1 is approved Inner Focus. Existing TrainerMonItemCustomMoves has
no native ability selector. All six implemented parties' selected abilities are
slot0/single-slot and native constructor tests verify exact results, including
Aerodactyl Rock Head, Snorlax Immunity and Blissey Natural Cure.

Agatha's contracts specifically require reporting an unsupported alternate-slot
conflict rather than changing species abilities or adding unapproved customization.
A narrowly scoped native-slot selector for only her two Alakazam is proposed to
the owner; no such helper or species change is included until that is resolved.
Both approved Agatha specifications/party rows remain in the expected-data JSON,
explicitly `implemented:false`. Do not call this eight completed battles.

## Validation / remaining acceptance

`tests/run_elite_four_tests.py` compares six exact arrays against pinned approved
specification hashes and freezes all other source/metadata/save/ID bytes using an
explicit delta normalizer. All existing20 hosts plus this new audit and3 checkers
pass (one direct rival preservation path was extended to reverse only these exact
approved arrays, retaining its original expected hash).

Native constructor acceptance: exact32 species/order/level/items/abilities, all six
IV fields under250/255 policies and128 moves/PP. Actual League-room battle tests
exercise native initial/rematch flags, trainer IDs, full send-outs, victory, door/
field return and flash save with a controlled strong Mewtwo. Player battle stats
are deliberately boosted; opponent source stats/AI are not modified. These are
progression/engine tests, **not HARD/Very Hard balance certification** or proof that
all item/status strategies trigger in every fight.

Initial local run passed Lorelei/Bruno's four fights and saw all six Lance members,
but Lance's postbattle text overflowed the invalid fixture's unterminated rival
name. The original dialogue contains `{RIVAL}`; normal campaign saves initialize
that name. Initial stale-input/HP hypotheses did not fix it. The fixture now sets
a valid EOS-terminated rival name (same pattern as existing rival acceptance) and
clears unused party slots. No game source, assertion or timeout was weakened.
Lance initial now passes; rematch/full latest six-battle rerun and CI remain pending
until their actual recorded results. Controlled boosted-player progression tests
are not balance/complete strategy or real-peer/hardware certification.

Rollback: revert six data arrays only; original saved trainer/League flags and
Pokémon IDs/layout unchanged. Never clear defeated flags to manufacture a rematch.
No ROM/art/save artifacts committed or released.

## November 6 Agatha completion branch — pending acceptance (append-only update)

Branch `feature/agatha-native-inner-focus` starts at main `767bb39c4fb107418f86ecc247572630b3adf014`. The first six merged battles and historical validation claims above remain unchanged; their status descriptions are historical, not the state of this proposed branch.

**Source-confirmed restriction:** `CreateNPCTrainerParty` in `src/battle_main.c` seeds PIDs with even low bytes 0x78/0x88/0x80 and only adds `nameHash << 8`. `CreateBoxMon` in `src/pokemon.c` derives the ability number from `personality & 1` when species slot 1 exists; Alakazam maps ability 0 to Synchronize and ability 1 to Inner Focus. The standard four Trainer party formats share the even-PID logic; there is no ability-selection field in `TrainerMonItemCustomMoves`.

**Implemented on branch for review:** a narrow native ability setter in the existing held-item/custom-moves constructor calls `SetMonData(&party[i], MON_DATA_ABILITY_NUM, &abilityNum)` with `abilityNum = 1` **only** for `TRAINER_ELITE_FOUR_AGATHA` and `TRAINER_ELITE_FOUR_AGATHA_2` when species is `SPECIES_ALAKAZAM`. This reuses an existing supported Pokémon data field, retaining the native `GetMonAbility` lookup and Alakazam's ordinary species table. Crucially, this does **not** change personality, nature, gender, shiny constraints, Trainer party structures, save formats, other Trainer Pokémon, Trainer flags, AI, or battle metadata. Opponents are constructed at battle initialization; the selected native ability is stored in each instantiated opponent's ordinary mon data and reused on switches and rematch creation. Confirm actual runtime behavior in emulator.

Both `sParty_EliteFourAgatha[]` and `sParty_EliteFourAgatha2[]` are replaced with exactly the five ordered approved members from the merged specifications, with 250 and 255 IV settings respectively. The pinned specification fixture now lists all eight battles as implemented on **the proposed branch**, not yet integrated into main. The prior six-battle delta remains hashed and frozen; regression tests reverse precisely the new two Agatha data hunks and the one native ability setter before checking preservation. The native emulator expected-data table includes all eight parties (42 members, 168 move slots); Agatha's actual scripted room is included in the two-battle scenario and checks ability, items, moves, IVs, switching, victory and room progress flags.

**Validation state at authoring:** static GitHub source review and GitHub file writes only. No local checkout/compiler or mGBA run available in this ChatGPT environment; no passing host tests, CI, build or native acceptance claimed yet. PR review and GitHub Actions must verify all exact arrays, preservation audits and build/test results, followed by full mGBA acceptance including all eight battles. No merge is authorized by this branch.

**Potential blocker and rollback:** if any regression or unexpected PID effect is observed, revert the scope-limited native ability field assignment and the two Agatha arrays (and corresponding tests) without touching the previous six merged Elite Four arrays. Do not replace Inner Focus with Synchronize or introduce species-wide ability changes. GYM-009 remains **implemented in draft PR / pending tests and merge**, not a completed integrated milestone.
