# Approved gift corrections — WRL-011-GIFTS

Authority: owner-approved issue38 (November5), reiterated explicit November6
execution directive. This supersedes old Museum Feebas100% design, not existing
saved Pokémon. No starter/Oak gift assumptions are made.

- Museum: Aron Egg, exactly50% independently selected true Shiny outcome. Original
  admission/NO-YES-NO/retry/full-party deferred award/one-time bits retained.
- Water Labyrinth: Magby Egg, same50% policy; original max-friendship classification6,
  space/pending/one-time and post-gift comments retained (Magby/Magmar). Award flag
  now commits after successful delivery, including allocation failure safety.
- Celadon roof: TangelaLv25 with exact fixed nicknameTANGO, ordinary own OT, native
  party/PC/full-capacity flow, same one-time flag/object and transfer message.
  Removes nickname prompt to preserve mandatory TANGO. Existing claimed saves do
  not receive another gift; no Pokémon already owned is rewritten.

## Implementation and save safety

CreateGiftEggWithShininess uses normal playerOT and final PID at CreateMon time:
low PID half random; high half XORs low/trainer halves with0 (Shiny value0) or
SHINY_ODDS (value8, guaranteed non-Shiny). One `Random() & 1` at eligible gift
creation selects either half. Check party/allocation before RNG/delivery. No PID
key mutation after encryption, cosmetic Shiny flags or hatch rerolls. Engine
creates stats/nature/gender/ability from final identity; ordinary randomIV and
standard egg metadata/cycles/hatch naming retained. Old guaranteed-Shiny helper
remains a wrapper for separately explicit callers, not the current Museum reward.
Normal daycare, Route5 persistent-intention tags, wild odds and unrelated gifts
unchanged. These ordinary gift eggs do not acquire Route5 special ownership tags;
real peer trades/other-owner hatching are not certified by this feature.

Existing special index for Museum remains; two new specials appended before
`gSpecialsEnd`. Names/flags retaining EEVEE/TOGEPI are stable historical script/save
identifiers, not current awarded species. No save schema/migration or new flag.
Exact frozen source delta and reverse normalization preserve previous feature
hashes. Source files: daycare.c/header, script_pokemon_util.c, approved_gifts.c,
three map scripts/roof text, special table. No new artwork.

## Executed evidence at initial commit

- Existing20 hosts/3 checkers PASS after exact normalizer; new gift host audit PASS.
- Build PASS.
- mGBA0.10.5 focused **80112 assertions PASS**: actual stored identities, each species
  **5030/10000 Shiny**, seed0xA7020000+i*7919 and varied playerOT; acceptance4700–5300,
  actual IsMonShiny, no sprites-only counting or Route5 tag. Exact50% rule is the
  single fair binary RNG branch, not a requirement for exactly5000 observed outcomes.
- Native Water Labyrinth script: low friendship, full party/no RNG direct failure,
  saved pending claim, one-time Magby, actual hatch/nickname refusal and true Shiny
  persistence. Native roof script: party and PC TANGO identity/save, no-capacity
  direct failure leaves award flag clear.
- Revised native Museum **1306 assertions PASS**: admission, three wrong-answer
  exits/retries, full-party saved deferred claim, Aron cycles35, actual hatch and
  evolution into Lairon, same PID/outcome after flash, no duplicate award.
- Full latest24-group native, clean reproducibility and remote CI pending initial
  commit. Passing focused fixtures does not certify a campaign or real link peers.

Rollback restores old Feebas/Togepi/Eevee source gifts; existing acquired Aron,
Magby,TANGO and claim flags remain untouched. Never clear old claim flags to give
new gifts again. No ROM/BIOS/save binaries committed or published.
