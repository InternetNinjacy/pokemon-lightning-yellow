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
