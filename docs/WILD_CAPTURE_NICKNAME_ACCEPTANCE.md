# Wild Double capture nicknames — BTL-004-NICK

Acceptance-only follow-up, stacked on integration PR35 at
`aaa71bb21077c8af4ea38dffe3e0017b56529621`. Source branch:
`feature/wild-capture-nickname-acceptance`; review PR36. No production source,
Pokémon, party, capture rule, encounter or save data is changed. Parent PR35 and
all original feature PRs remain open/unmerged. Retarget only after reviewing the
parent integration outcome; no merge or auto-merge is requested.

## Four executed focused scenarios

| Survivor | Fainted opponent | Delivery | Nickname path |
|---|---|---|---|
| Right Cloyster | Left Onix | party | actual keyboard: clear name, enter A, OK |
| Left Onix | Right Cloyster | party | same |
| Right Cloyster | Left Onix | full-party PC | same, including PC destination notice |
| Left Onix | Right Cloyster | full-party PC | same |

Each starts the real natural encounter trigger on a real grass attribute tile,
selecting its approved experimental Onix/Cloyster pair. Opponent moves/stats are
unchanged; real player attacks faint one opponent. Native Protect/misses may
require bounded retries and must actually produce the selected faint. Native Bag
USE and a Master Ball catch the survivor. No capture-success opcode is stubbed.

Assertions check captured species/personality/OT identity, nickname A, encrypted
bad-Egg validity, original party identities/names, correct party/box destination,
exactly one ball, only the caught species registered as caught, field return and
flash save/load retaining nickname/personality/species/consumption. Full party
members are controlled fixtures; other battle/encounter functionality is unchanged.

The original fixed-frame confirmation timed out in PC naming, captured in a local
screenshot. Driving the actual confirmation until the naming callback exits
resolved the fixture; no production patch or disabled assertion was used.

## Repeatability and evidence

- `LIGHTNING_WILD_NICKNAME_ONLY=1 python3 tests/emulator/run.py`:367 assertions PASS
  on local mGBA0.10.5 (four scenarios, includes invocation/memory safety checks).
- All16 host suites, Museum and registration check PASS. New native group is added
  to the complete default suite; all17 prior groups remain, plus the new eighteenth.
- `python3 tests/emulator/run.py`: full conclusions/counts recorded in live PR36.
- Existing unchanged production ROM/ELF matches PR35's clean build, SHA256
  `923fba779dbfdc05529e40abb36ec4f8442172d7b74ff65bb5010a065724bf35`.
  Local validation reused that identical build; CI performs its own fresh build,
  clean comparison and independent upstream baseline. No new local rebuild claim.

No other successful-ball matrix, failed capture probability distribution, naming
cancellation/blank/maximum-length/Unicode/Name Rater, complete whiteout/replacement,
all forced-end/spread/weather combinations, interrupted save, real link peer,
hardware or natural playthrough coverage is claimed. These remain separate gaps.
No ROM/save/BIOS/screenshot artifacts are committed or uploaded.

Reverting this test/docs-only follow-up restores parent PR35 without changing
ROM behavior or saved Pokémon. Preserve all parent frozen normalizers/ABI guards.
