# Physical / Special / Status split

## Approved scope and mapping

This change applies Generation IV move categories to the existing Generation
I–III move set. It does not implement other Generation IV mechanics. Shadow Ball
is **Special**, as explicitly clarified by the user during implementation.

`gBattleMoves[move].category` is authoritative. Named constants are in
`include/constants/move_categories.h`. Every one of the 355 table entries has an
explicit initializer: **139 Physical, 77 Special, 139 Status** (Status includes
`MOVE_NONE`, so 138 real Status moves). Zero is invalid to expose omissions.
`MOVE_UNAVAILABLE` is a sentinel, not an index into the move table; the new AI
result command checks its bounds before looking up a category.

The full expected mapping is `tests/move_categories.json`, seeded from the
[PokeAPI move-category dataset](https://github.com/PokeAPI/pokeapi/blob/2bf639e5572812a67d2b94903fdcb9d095f5aeb2/data/v2/csv/moves.csv)
at commit `2bf639e5572812a67d2b94903fdcb9d095f5aeb2` (damage_class_id 1=Status,
2=Physical, 3=Special, move IDs 1–354). Only category facts were used; no code or
other game project was imported. Reviewed Generation IV assignments include:

| Category | Examples and unusual cases |
| --- | --- |
| Physical | Fire/Ice/Thunder Punch, Crunch, Shadow Punch, Struggle, Beat Up, Counter, Bide, Seismic Toss, Guillotine, Horn Drill, Fissure |
| Special | Shadow Ball, Flamethrower, Thunderbolt, Hyper Beam, Hidden Power, Future Sight, Doom Desire, Mirror Coat, Night Shade, SonicBoom, Dragon Rage, Psywave, Sheer Cold |
| Status | Calm Mind, Swords Dance, Pain Split, Nature Power, Metronome, and MOVE_NONE |

A category is not a damage algorithm. Existing fixed/level/random damage, OHKO,
Bide, Counter/Mirror Coat, Future Sight and Beat Up scripts retain their specialized
behavior. Calling moves still dispatch to the chosen move's effect. Category does
not replace contact flags. No power, accuracy, PP, type, priority, target, flags,
secondary effect or effect ID in the move table has changed.

## Battle and AI behavior

Physical uses Attack/Defense, physical stages, burn, Reflect, and the relevant
stat boosts. Special uses Sp. Atk/Sp. Def, special stages and Light Screen.
Critical-stage bypass and doubles screen/spread reductions retain Generation III
rules. Status immediately returns zero from ordinary base-damage calculation;
its scripts may still heal, redistribute HP, or cause indirect damage.

Hustle accuracy and Counter/Mirror Coat damage history now use move category.
Passive damage and Pain Split do not become counterable ordinary attacks. Fire
thawing accepts damaging Fire moves from either category.

STAB, weaknesses, resistances, immunities, weather, sport effects, Blaze/Torrent/
Overgrow/Swarm and Flash Fire remain elemental rules. Weather and Flash Fire were
moved out of the old special-only branch so newly physical Fire/Water moves still
receive them. Thick Fat reduces the selected attacking stat for Fire/Ice moves.
Those are necessary corrections to support the split, not new ability mechanics.

Stat-specific items/abilities remain Generation III: Huge/Pure Power, Hustle,
Guts, Choice Band and Thick Club affect Attack; Soul Dew and DeepSeaTooth affect
Sp. Atk; Light Ball still boosts **only Pikachu's Sp. Atk**. Defense/Sp. Def items
and Marvel Scale retain their existing stat effects. Type boosters boost the
selected stat for the matching elemental type. Explosion/Self-Destruct retain
the Generation III Defense-halving rule. Dynamic type changes do not change the
move's category; Hidden Power remains Special for every type.

AI damage estimates share the production damage calculator. Two formerly unused
AI opcodes (`0x52`, `0x53`) provide category queries. Defense/Sp. Def boosts,
Attack/Sp. Atk drops, screens, Counter and Mirror Coat use categories instead of
lists of old physical/special types. Target-moveset queries use observed move
history, never hidden enemy moves. They keep the existing BUGFIX-dependent
history indexing and inspect its first four slots; no enemy-category information
means no positive category match. All other type queries remain elemental.

## UI

The battle type window shows e.g. `FIRE/PHYS`, `GHOST/SPEC`, or `NORMAL/STAT`.
PHYS=Physical, SPEC=Special, STAT=Status. Summary lists place the same short label
below each type icon, including a fifth move being learned. The detail pane shows
the full category between accuracy and description. All labels read the same
move table; no new graphics/assets were imported. Actual font widths and layout
bounds are checked automatically; visual/emulator acceptance is still required.

## Validation and honest limits

Run from the repository root:

```sh
python3 tests/run_split_tests.py
scripts/prepare-baseline.sh
make -j2
sha256sum pokefirered.gba > /tmp/lightning-first.sha256
make clean
make -j2
sha256sum -c /tmp/lightning-first.sha256
```

Host regression tests require Python 3 and GCC, provided by Actions. They compile
and **execute the current production C** calculator, AI damage wrapper, HP update,
Counter/Mirror Coat, level/random/half-HP commands, category AI opcodes and label
helpers, against actual move data and struct definitions. Only environment hooks
(badges, ability field queries, item lookup, RNG, live battler count and controller
output) are stubbed. Item tests inject hold-effect IDs into the test item field;
this exercises the production modifier branches, not the actual item database.

The 1,299 numeric/category/helper assertions cover independently varied attacking
and defensive stats; Fire and Ghost categories; all Status entries; fixed-damage
commands; screens, burn, critical stages, doubles, weather, Flash Fire, Thick Fat,
Huge/Pure Power, Hustle, Guts, Marvel Scale, Plus, held stat/type items, Hidden
Power, Explosion, AI damage/category queries, real HP damage tracking, Counter/
Mirror Coat, and UI-helper consistency for every move. Special Ghost normal-power
coverage uses both existing Shadow Ball and an isolated type-override fixture;
Night Shade executes the unchanged level-damage command.

Six **static** audits separately check mapping completeness, unchanged non-category
move properties, preserved specialized scripts/commands, category consumer wiring,
actual font widths/layout bounds, and AI strategy wiring. Static hashes demonstrate
preservation, not successful battle-script execution. The untouched OHKO, Bide,
Beat Up, Future Sight, SonicBoom and Dragon Rage end-to-end script paths are not
emulated by these tests. Neither are Hustle accuracy rolls, thaw animations,
full AI script execution/scoring, live type-effectiveness processing, UI rendering,
or link battles. No emulator is installed in this cloud machine, and **emulator
gameplay testing remains outstanding**. Compilation alone is not acceptance.

Actions runs `modified` (tests, compilation, clean rebuild with the first build's
SHA-256) and `upstream-baseline` (separate pinned upstream checkout, original SHA-1
comparison twice). The modified SHA-256 is observed per run, not a fabricated
replacement expected checksum. `firered.sha1` and upstream provenance stay intact.
No ROM artifacts are uploaded. Check the PR's Checks tab and Actions run for the
actual current revision's results.

## Compatibility, review, and rollback

BattleMove grows from 9 to 10 bytes, alignment 1: existing field offsets 0–8 stay
unchanged, category occupies offset 9. Its consumers are C field/array accesses;
no assembly references or fixed record strides were found. Save Pokémon structs
are unchanged. External tools/patches assuming 9-byte ROM records must adapt.
Do not mix this build with vanilla or mismatched link-battle peers. Existing AI
estimation approximations, including incomplete dynamic-power/type knowledge and
legacy history behavior, are not overhauled here. Future category/data edits must
update the reviewed manifest and preserve the explicit-initializer rule; approved
balance changes must deliberately update the non-category-data audit.

Keep main stable and require review. Maintainers should require the `modified`
and `upstream-baseline` checks. No merge or auto-merge is authorized by this task.
To roll back after a future authorized merge, open a separate PR reverting the
split commit (or squash commit); do not reset or force-push main. This restores
the original baseline workflow and move layout. Preserve any subsequently approved
features by reviewing revert conflicts, and rerun the original checksum baseline.
