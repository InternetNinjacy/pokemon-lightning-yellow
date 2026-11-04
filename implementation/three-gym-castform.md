# Approved Castform Gym Trainer configurations

Project: Pokémon Lightning Yellow
Approval: 2026-11-04
Status: Approved design, NOT standalone implementation-ready; documentation only.

| Gym trainer | Level | Ability | Held item | Ordered move slots |
|---|---:|---|---|---|
| Pewter sole ordinary Trainer | 11 | Forecast | None | Weather Ball / Powder Snow / Tackle / Water Gun |
| Cerulean ordinary Trainer 2 | 19 | Forecast | None | Rain Dance / Weather Ball / Water Gun / Tackle |
| Celadon ordinary Trainer 7 | 27 | Forecast | None | Weather Ball / Ember / Water Gun / Tackle |

Weather Ball is trainer-specific early access on all three, with no changes to Castform's level-30 natural Weather Ball acquisition or player-owned learnsets. Preserve standard AI, ordinary Gym Trainer held-item and bag-item restrictions.

Pewter retains permanent battle Hail (Snowy Form), Single Battles, and Geodude Lv12 with Tackle/Defense Curl/Rock Throw/empty; Castform replaces Swinub. Cerulean retains mandatory Doubles and no automatic starting rain: Castform can use Rain Dance for Rainy Form; its partner is Slowpoke Lv19 with Water Gun/Confusion/Headbutt/Curse, replacing the previously approved Staryu slot. Celadon retains permanent sunlight (Sunny Form) and Single Battles. Its approved Trainer 7 party is Cacnea Lv27 (Pin Missile / Leech Seed / Growth / Ingrain) followed by Castform Lv27 (Weather Ball / Ember / Water Gun / Tackle). Cacnea's exact level and four moves were subsequently owner-approved on 2026-11-04. The old five-member Cooltrainer Mary party is superseded. Do not change other Trainer or Leader parties. Do not change other Trainer or Leader parties.

Future engineering must verify source IDs, approved Gym-weather/Double mechanics and Forecast, and balance of early weather-boosted Weather Ball. This is not permission for independent Gym/weather redesign, gameplay changes, or merge. Implement only after a separate complete reviewed Codex contract. Follow AGENTS.md and independent baseline checks.

Approved source: https://docs.google.com/document/d/18jVindV66zc_2_6xPqZajCiXf-1738lADjShESrDxU0/edit
Change register: https://docs.google.com/document/d/1NpCjx3uboBMdBVqoFpMwXEyVkDquD37Kcp-FJBbYWnc/edit
