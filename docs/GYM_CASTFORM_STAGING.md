# Three-Gym Castform — GYM-007

Owner authorized completing existing PR31 against PR30 on November4, 2026.
[Exact approved contract](../implementation/three-gym-castform.md) preserves all
six Pokémon, levels and ordered move slots. PR30's earlier preparation-only
status is superseded for this specifically authorized implementation, not for
other Gym teams or redesigns.

## Scope and dependency

- Liam142: Castform11 (Weather Ball/Powder Snow/Tackle/Water Gun), Geodude12
  (Tackle/Defense Curl/Rock Throw/empty), Single.
- Diana150: Castform19 (Rain Dance/Weather Ball/Water Gun/Tackle), Slowpoke19
  (Water Gun/Confusion/Headbutt/Curse), mandatory native Double. Both trainer
  metadata and event command agree. Native two-usable-mon refusal reuses the
  existing neutral Ranger technical prompt; existing intro/defeat/post text stays.
- Mary402: Cacnea27 (Pin Missile/Leech Seed/Growth/Ingrain), Castform27
  (Weather Ball/Ember/Water Gun/Tackle), Single. Her legacy Super Potion is removed
  because the approved ordinary Gym1–7 no-bag-item rule expressly applies. No
  other trainer inventory or AI is changed.
- No ordinary held items. Forecast, Castform's natural Weather Ball level30,
  other species data, AI/IV metadata, trainer IDs, save flags, leaders, rewards,
  map layouts and other trainer parties remain unchanged.

This branch integrates the exact tested weather PR29 head
`061f87805763025a494d55803f9021a6cd7f6325` in merge `cd7be97e`, without merging
or modifying that PR. PR31 currently includes that unmerged dependency. Review
its additional Gym delta against that weather head; no combined rival PR27
integration is claimed. Later changes to PR29 must be reconciled before merging.

Pewter's map header uses native snow and starts every battle in permanent Hail.
Celadon's header starts every battle in permanent Sun, including nighttime.
Cerulean stays clear: Castform's actual Rain Dance produces temporary native
rain. Only Diana is converted here; unfinished Luis/Misty or other Gym designs
are deliberately not implemented by this limited three-Trainer contract.

`DynamicWeatherForBattle` reads fixed Gym headers before the outdoor policy and
before renderer fades. This does not add Gyms to random outdoor tables or apply
nighttime outdoor Sun suppression indoors. The field updater adopts these two
headers after old inside-Gym saves Continue, without save expansion or erasing
stored outdoor weather. Indoor visits preserve PR29's outdoor restore/reroll
context. Moves and abilities retain native ability suppression/replacement and
weather duration rules; permanent means the starting Gym weather does not expire
on its own, not that a weather-changing move is prohibited.

Sun reuses PR29's existing clear renderer; no final sunlight tint, new palettes,
new artwork or day/night lighting is implied. Snow uses existing GenIII assets.

## Validation

Run all `tests/run_*tests.py`, Museum checks, clean `make -j2` reproducibility,
and `python3 tests/emulator/run.py` (official toolchain instructions apply).
Focused: `LIGHTNING_GYM_CASTFORM_ONLY=1 python3 tests/emulator/run.py`.
Actual conclusions/counts and CI links are maintained in PR31 and the ledger.
Static exact-data preservation is explicitly distinct from executed battle tests.
Native fixtures use controlled player parties but unchanged opponent parties,
AI, abilities, levels and moves. Weather Ball's actual opcode is tested separately
from the naturally selected enemy Rain Dance and native form/battle animations.

A full natural playthrough, manual balance review, every legal move combination,
physical cartridges and real link peers remain outside scoped emulator coverage.
Saved legacy defeat flags remain defeated; this does not reset beaten trainers.
Back up saves before switching builds. Follow PR29's indoor-save precaution before
downgrading to code with its old broken drought loader. Revert this Gym delta
while retaining PR29 if rolling back only this feature; do not revert unrelated
trainer, weather, rival or save-system work.
