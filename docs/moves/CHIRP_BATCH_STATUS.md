# BTL-005 #365 Chirp — implementation evidence

Parent: `feature/btl005-bug-statdrops-363-364` (draft PR #91). Child branch: `feature/btl005-chirp-365` (draft PR #92). This is not merged to main.

The approved Chirp source is Bug/Special 40 power, 100 accuracy, 25 PP, neutral priority, selected target, no secondary status. `EFFECT_HIT` implements ordinary damage. `Move_CHIRP` routes directly to the original Growl visual. Sound semantics require `MOVE_CHIRP` in `src/battle_util.c`'s Soundproof table AND `data/battle_ai_scripts.s`'s bad-move sound-immunity branch. No new artwork, species learnsets, TMs or trainer move edits.

`tests/run_chirp_tests.py` protects exact data, sound/AI lists and old-source reverse manifest. `tests/emulator/new_chirp.c` exercises actual battle turns, Singles/Doubles, immunity, PP, move animation termination, and Soundproof AI. `tests/emulator/original_effects.c` registers native tests in the full suite. Required acceptance: all current host checks, clean reproducible modified build, complete native mGBA suite and independently successful upstream-baseline workflow, on the **final exact branch head**. Those gates have not been certified at the time this file was written.

Do not merge this child until #82, #85, #89 and #91 are accepted and integrated in order. Do not award BTL-005 effort credit or claim release readiness for a new move based solely on source presence. The next approved ID is #366 Swarm Cloud, which requires **independent** confusion 50% and poison 50% rolls and must not be implemented as one combined native proc.
