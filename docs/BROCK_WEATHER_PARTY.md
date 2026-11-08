# Brock weather-party integration

Branch `feature/brock-approved-weather-party`, stacked on PR118 / `368b3119c7c50c251324f76573d232cdb035e8bd`. No merge authorized. This completes the missing **party** integration for issue47 weather abilities, not all Pewter reward/presentation design.

## Exact recovered approval

Owner-supplied November3 authority export, `02_GYMS/Pokémon Lightning Yellow — Eight-Gym Leader & Trainer Roster — Final Design Handoff (2026-11-01).txt`, GYM1:

> Leader Brock (2, SINGLE): lead SNORUNT Lv14 PURE ICE, Ice Body, Powder Snow / Headbutt / Leer (slot4 EMPTY), no held item. Ace ONIX Lv16 PURE ROCK, Overcoat, Screech / Rock Throw / Harden / Tackle; held PECHA BERRY. Brock trainer-use bag: exactly ONE POTION.

The earlier weather progress report incorrectly called these party details missing. They were available in the supplied export. The party now implements them without guessing or asking for reconfirmation.

## Changes

Replace only Brock's original Geodude12/Onix14 party and empty bag with the exact approved team/bag above. Change the party format to held-item/custom-moves so Onix's Pecha Berry is actually represented. Keep original IV0 values, original Single format, AI flags, trainer ID, appearance and map scripts. Abilities come from PR118's scoped entry helper; no species slots change. Existing field Snow/permanent battle Hail stays intact.

Other leaders, ordinary Trainers, species, maps and encounter data are byte-preserved. Badge naming, TM/reward/dialogue redesign is outside this weather-party slice; this is not a full Pewter Gym completion claim.

## Validation

- `python3 tests/run_brock_weather_tests.py`: exact data/items/format and strict reverse delta with all unrelated Trainer/party bytes preserved. Original frozen manifests and expected digests are unchanged.
- All43 host runners passed after applying the exact Brock reverse adapter **before** the ordinary-Trainer projection's final byte comparison. No assertions removed or bypassed.
- `LIGHTNING_WEATHER_MOVES_ONLY=1 LIGHTNING_WEATHER_GYM_ABILITIES_ONLY=1 python3 tests/emulator/run.py`:118 focused assertions passed before the broader final run. Real leader scripts construct exact species/levels/four moves/held items; real battle history contains exactly one Potion; Snorunt heals1/16 during a real Hail turn; Onix enters with Overcoat and takes no Hail residual damage. Misty's reserve Drizzle still passes.
- Full weather-move regression, clean reproducibility and CI results are recorded in the PR.

Test fixtures use one player Pokémon to avoid optional Shift prompts. Move selection uses actual Left input when returning from Splash to Swift; the shared older chooseMove helper's Right-only assumption is not changed.

## Remaining weather scope

Special-event levels/moves/positions/final Corphish farewell are explicitly deferred in the supplied Special Overworld final handoff. Global species slot replacements/inheritance remain unapproved in issue47. Persistent captured-individual ability storage and actual installed-event capture/defeat/retry acceptance remain unimplemented; this party change does not resolve those tasks.

Rollback: revert this branch's gameplay/test commit after removing dependent consumers, leaving PR118's battle engine intact. No save layout or Pokémon storage changes. Save backups remain recommended before ROM version changes; downgrade/hardware/link acceptance was not executed.
