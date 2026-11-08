# Remaining approved-weather design inputs

Source snapshot: PR127 parent stack; issue47 explicitly approves eligibility but defers exact slots and evolution inheritance. These are missing design values, not requests for routine implementation permission. Brock's exact party was recovered and implemented in PR126; it is no longer a missing-spec blocker.

## Global ability slots

Return final slot0 and slot1 for each row, naming which existing ability is replaced. Do not infer a replacement from eligibility alone.

|Species|New approved eligibility|Current slot0|Current slot1|Approved final slots|
|---|---|---|---|---|
|SNORUNT|Ice Body|INNER_FOCUS|NONE|Not supplied|
|GLALIE|Ice Body|INNER_FOCUS|NONE|Not supplied|
|SPHEAL|Ice Body|THICK_FAT|NONE|Not supplied|
|SEALEO|Ice Body|THICK_FAT|NONE|Not supplied|
|WALREIN|Ice Body|THICK_FAT|NONE|Not supplied|
|ONIX|Overcoat|ROCK_HEAD|STURDY|Not supplied|
|STEELIX|Overcoat|ROCK_HEAD|STURDY|Not supplied|
|PINECO|Overcoat|STURDY|NONE|Not supplied|
|FORRETRESS|Overcoat|STURDY|NONE|Not supplied|
|MAGIKARP|Drizzle|SWIFT_SWIM|NONE|Not supplied|
|GOLDEEN|Drizzle|SWIFT_SWIM|WATER_VEIL|Not supplied|
|SEAKING|Drizzle|SWIFT_SWIM|WATER_VEIL|Not supplied|
|FEEBAS|Drizzle|SWIFT_SWIM|NONE|Not supplied|
|POLIWAG|Drizzle|WATER_ABSORB|DAMP|Not supplied|
|POLIWHIRL|Drizzle|WATER_ABSORB|DAMP|Not supplied|
|POLITOED|Drizzle|WATER_ABSORB|DAMP|Not supplied|

Also specify evolution slot/remapping for these families, especially Magikarp→Gyarados, Feebas→Milotic and Poliwhirl→Poliwrath (those evolved species must NOT receive species-wide Drizzle). Existing Pokémon ability migration, if intended, needs an explicit rule.

## Special individuals and encounter producers

- Corphish→Crawdaunt: retain individual Drizzle or revert to which approved native ability? Do not invent the answer. Glalie has no further GenIII evolution; its independent persistent Snow Warning implementation is in PR127. Tyranitar already has native Sand Stream, now tested through actual party/PC captures.
- SOW-004/007/008: approved levels and four move slots (including deliberate empty slots). The Special Overworld final handoff explicitly defers all three.
- Exact approved event tiles/placement source for Route12 dock, Route21 grass and Route23 checkpoints. Preserve existing guards/progression.
- Final Corphish sailor-ghost farewell and approved aftermath sequence. The supplied wording is provisional.

Once supplied: generate/mark each specific individual, connect route flags and approved aftermath to the existing native-wait resolver, then run actual installed-event capture, defeat, fleeing, loss/retry, save/load, weather/RTC-boundary and no-leakage acceptance. No normal gameplay encounters are installed with placeholder levels, moves or dialogue.
