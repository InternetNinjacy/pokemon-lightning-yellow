# Combined rival, weather and Gym acceptance — INT-004

This is a review branch, not a merge into main. It combines exactly:

- Rival PR27 `62c994194b9bb1032e74b2ae42d0871c669c5215`.
- Weather PR29 `061f87805763025a494d55803f9021a6cd7f6325`.
- Castform PR31 `02c97b7ffd3dfca48485f3f0663e0725cca5ec62`, which already includes PR29.

Recovery: initial Castform head02c97b7f; merge checkpoint51b38c36; corrected
complete runners and Blaine acceptance754350e8. No incoming PR or main modified,
merged, retargeted or given auto-merge. No new Pokémon, party, balance, event or
save behavior is introduced by the integration resolution. Production files
merge cleanly: three exact rival-family learnsets and the approved trainer tables.

## Reconciliation

Nine files conflicted: workflow, AGENTS, README, master ledger, dashboard, tracker,
Ranger normalizer, native runner and its Python builder. Both sets of ABI fields,
required symbols, fixture generators, C includes, focus commands and full-suite
calls are retained. The newer consolidated tracker retains both features' prior
history; generated dashboard reflects it. Existing expected preservation hashes
are unchanged; normalizers reverse only frozen approved rival/Gym/weather hunks.
Tests of unchanged files first remove the other approved feature's exact delta.
A registration audit checks all16 host runners and17 native groups remain enabled.

An initial conflict-resolution script truncated runner/workflow tails; Python
syntax checking caught this before any runtime pass was claimed. The complete
three-way files were restored, then compilation and the registration guard checked.
History retains the failed checkpoint and correction; final evidence applies to
the corrected complete tree, not that checkpoint.

## Additional Blaine coverage — GYM-004

No Blaine game data changes. A fresh actual Double Battle with two controlled
level100 player Mewtwo observes every one of the unchanged five opponents, wins,
executes the original defeat/badge/TM38 script, saves/reloads flash, Continues and
revisits without another reward. Original refusal/type-immunity fixtures remain;
the injected type-check state is discarded before victory. Focused75 assertions
PASS on local mGBA0.10.5. This is controlled acceptance, not balanced progression.

## Evidence and limits

All16 host suites, Museum audit and registration audit pass. Source builds; clean
repeatability/full native/CI conclusions are recorded in live PR35:
https://github.com/InternetNinjacy/pokemon-lightning-yellow/pull/35

Run `python3 tests/run_*tests.py` individually, both checker scripts, the documented
pinned build, and `python3 tests/emulator/run.py`. Focus Blaine with
`LIGHTNING_BLAINE_ONLY=1 python3 tests/emulator/run.py`. Native tests retain their
ARM-derived ABI, protected test memory and assertions. Do not distribute build,
BIOS, ROM, save or screenshot artifacts.

No complete natural story progression, every battle mechanic/map/quest-log path,
power interruption, physical cartridge or real link peer is certified. The global
252-only world audit still has its disclosed legacy findings; its no-new-findings
guard is not global compliance. Final lighting/tints/Mansion fog and unreviewed
sign/League specs are not implemented here. PR28 and PR32–34 require their own
reviewed handoff/execution path. Other Gym parties and main are unchanged.

Rollback by abandoning this integration branch leaves all original feature heads
and main intact. Before loading saves on older code, retain PR29's indoor-save/
backup precaution for its old drought loader. No save-layout migration introduced.
