# Celadon Rocket Hideout — #45 implementation record

Status: **candidate gameplay implementation, draft PR #74, acceptance pending**. Owner-authorized scope is GitHub issues [#45](https://github.com/InternetNinjacy/pokemon-lightning-yellow/issues/45) and [#44](https://github.com/InternetNinjacy/pokemon-lightning-yellow/issues/44). This record does not assert a passing ROM build or emulator validation.

## Exact existing trainer IDs and event scopes

All 11 non-Giovanni IDs `TRAINER_TEAM_ROCKET_GRUNT_8` through `_18` retain their source event associations. B1F has five (8–12); B2F has one four-member party (13); B3F has two (14,15); B4F has Lift Key grunt 18 and two Executives 16,17. Total exactly 32 ordered slots. All use `TrainerMonNoItemDefaultMoves` and `NO_ITEM_DEFAULT_MOVES`, zero held items, and empty battle-item inventories. Existing IV and AI settings remain.

The two guards use class ID 107 `TRAINER_CLASS_ROCKET_EXECUTIVE`, class-title text `ROCKET`, and trainer-name text `EXECUTIVE`. The composed title is intended to display `ROCKET EXECUTIVE`; runtime title/UI has not been observed. Existing 12-character class-name storage is unchanged, Giovanni untouched.

## Recovered Pokémon identity and storage

Each unique gift is created *fresh* at level 27 with standard engine level-up moves and full native default stats/PP. Issuer/owner is PAIGE (Ralts, fixed OT ID 0x7AC10459) or JULIA (Snorunt, fixed OT ID 0x4C3908B7). Non-shiny personality generated once per gift. The native `GiveMonToPlayer` overwrites foreign OT, so the implementation follows the already accepted Mystery Vendor pattern: temporarily deliver player-keyed mon to party or PC, rekey secure data in the actual destination, replace OT ID, name, and gender, and finally record collection.

Persistent reserved save flags `0x316`–`0x31B` track collection, returns, and ball visibility. Four reserved save variables `0x40F8`–`0x40FB` hold both exact gift personality values (32 bits each). Return eligibility matches (saved personality, fixed original OT ID, original OT display name, permitted evolution family). Match ignores level, nickname, and current party/box; searches every party slot and all PC boxes. A substitute of the same species is not accepted; a legitimately traded-away-and-back individual remains eligible. This is stable against normal transformations and save/load assuming ordinary native preservation of Pokémon OT/personality values.

Return operations execute synchronously: check collected and not previously returned, locate actual individual, protect the final party member, check item capacity, award item, remove precisely the matched party/PC mon, mark returned. Reward: one Rare Candy to Paige; one NeverMeltIce to Julia. No automatic reissue for released/traded Pokémon. Save interruption between C statements is not supported by the field-script engine, but reset/power interruption or link anomalies still require native acceptance tests.

## Map and script coexistence

B3F objects added at (3,20) Ralts and (18,8) Snorunt. They are independent of the original three B3F visible pickups, map warps, spinner layout, and Giovanni hide flags. The original proposed Snorunt location (19,10) has collision=1 and elevation=0 in the binary map and was corrected to (18,8), a collision=0/elevation=3 tile with adjacent walkable tiles. Both ball placements now pass source-level binary-layout collision/elevation assertions. Complete event-interaction and movement verification in mGBA remain separate acceptance tasks. Existing Grunt defeat flags determine which item balls appear on map reload. Route 8 Paige and Julia normal trainer/rematch entrypoints preserved with postbattle return choices and post-return acknowledgment.

## Source inventory and reasons

- `src/data/trainer_parties.h`: 11 exact parties.
- `src/data/trainers.h`: default moves for source-custom parties, two Executive class/name changes.
- `include/constants/trainers.h`, `src/data/text/trainer_class_names.h`: distinct class ID/title.
- `src/rocket_rescue.c`: native foreign OT delivery, identity records and exact return transactions.
- `include/constants/flags.h`, `include/constants/vars.h`: unused persistent save bits/vars for two independent quests.
- `data/specials.inc`: three native special registrations.
- `data/maps/RocketHideout_B3F/map.json`, `scripts.inc`, `text.inc`: two objects and drops, source Grunt battle callbacks, dialogue.
- `data/scripts/trainers.inc`, `data/maps/Route8/text.inc`: two owner quest interactions.
- `tests/run_rocket_hideout_roster_tests.py`, `tests/run_rocket_hideout_quest_tests.py`, `.github/workflows/rocket-hideout-validation.yml`: source contract checks and CI.
- `tests/run_approved_gift_tests.py`: narrowly normalizes only three appended native registrations while preserving prior exact source hashes.
- This document: implementation notes and acceptance limitations.

## Testing and integration

Dedicated source-contract workflow: `python3 tests/run_rocket_hideout_roster_tests.py` and `python3 tests/run_rocket_hideout_quest_tests.py`. Run those plus repository's full host suites and `scripts/prepare-baseline.sh && make -j2`, `make clean && make -j2`, and `python3 tests/emulator/run.py`. The project-wide Battle split validation workflow already orchestrates the latter gates, subject to its preceding frozen-source audits. Preserve upstream checksum check.

Native acceptance still needed: all 11 actual battles, two Executive displayed titles; two ball locations and arrival after respective wins, reload/Giovanni persistence, party/full PC fallback, foreign OT traded EXP/obedience, same-PID same-family detection after evolution and nickname, all PC boxes, lose/trade/retrade, full bag, only party Pokémon, return from party and PC, exactly-once rewards, old Lift Key/barriers/Giovanni/Silph Scope.

## Concurrency / Master Control

PR #71 overlaps only `src/data/trainer_parties.h`, `src/data/trainers.h`, and `data/scripts/trainers.inc`, not the Rocket identity core. Resolve overlaps by comparing changed hunks (Trainer IDs vs ordinary Trainer IDs) rather than wholesale replacing either file. Do not merge unreviewed gameplay, and let Master Control own shared project tracking and Drive records until acceptance; avoid concurrent edits to `docs/project-tracking.json` and `docs/PROJECT_DASHBOARD.md`.
