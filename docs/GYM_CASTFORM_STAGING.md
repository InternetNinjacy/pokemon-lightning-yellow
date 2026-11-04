# Gym Castform implementation staging: Pewter and Cerulean

Status: PARTIAL GAMEPLAY CHANGE, DRAFT ONLY; **not merge-ready**. Authority: `implementation/three-gym-castform.md` on `docs/gym-castform-three-weather-balls-20261104` and owner-approved Gym Drive register (2026-11-04).

Implemented in this branch:
- Pewter original Camper Liam, explicit party: Castform Lv11 (Weather Ball, Powder Snow, Tackle, Water Gun), followed by pure-Rock Geodude Lv12 (Tackle, Defense Curl, Rock Throw, empty fourth).
- Cerulean original Picnicker Diana, explicit party: Castform Lv19 (Rain Dance, Weather Ball, Water Gun, Tackle), paired in party data with Slowpoke Lv19 (Water Gun, Confusion, Headbutt, Curse). Converted her Trainer party macro to NO_ITEM_CUSTOM_MOVES.
- No species-wide Castform learnset alterations, no Trainer item or AI modifications.

NOT IMPLEMENTED / NOT VERIFIED:
- Cerulean's existing script is still a Single Battle; approved mandatory Double Battle conversion requires separate implementation and testing. Trainer `.doubleBattle` remains FALSE in this draft. **Do not merge pretending the battle is a Double.**
- Pewter permanent battle Hail and indoor snow, and Celadon permanent sunlight, are part of broader Gym/weather integration, not implemented by this staging branch.
- Celadon Cooltrainer Mary currently has an unrelated five-member original FireRed party. Approved replacement is exactly Cacnea + Castform Lv27 (Weather Ball, Ember, Water Gun, Tackle). The level and moves of Cacnea have not been approved. Do not invent them or silently retain five legacy Pokémon as the final Celadon configuration.
- No clean build, host tests, emulator tests, Gym event progression, or Weather Ball weather-form acceptance has been executed by this GitHub-only documentation/source edit.

Follow `AGENTS.md` and `implementation/README.md`. Do not merge until the full approved encounter contract, existing weather infrastructure, and per-feature tests are reconciled. No unrelated Gym or story changes.

Approved record: https://docs.google.com/document/d/18jVindV66zc_2_6xPqZajCiXf-1738lADjShESrDxU0/edit
