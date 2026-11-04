# Mystery Pokémon Vendor Network

## Current effective implementation

Authority: owner-supplied Mystery Vendor Codex Implementation Handoff v1.0,
November2; supplied pool CSV independently confirms the ten species, level5,
price700,90% and OT Mystery. The owner explicitly approved implementing this
network independently of the unfinished starter changes. November3 confirmation
of Gyarados Water/Dragon resolves the prior typing question; no typing is changed
here. The subsequent40-file authority export is available for later work; it is
not permission to invent unresolved designs or lift the special-overworld hold.

Task WRL-008. Branch `feature/mystery-vendor-network`, based on fossil PR#11
`b0bd3b86f4e03dd3f540f0776a5de87f207e84e4`. Parent PRs remain unmerged.
Core implementation checkpoint: `b9efabb2`; subsequent commits contain dispatch,
UI and complete regression coverage. No worktrees, main changes or parent edits.

## Behavior and integration boundary

Ten original NPCs sell one level5 Pokémon each for ¥700. The game font's yen
symbol represents the approved700 price. The remaining pool is sampled uniformly
without replacement: Bulbasaur, Squirtle, Charmander, Kangaskhan, Pinsir, Feebas,
Chansey, Dratini, Absol and Magikarp. Prior caught/seen flags and unrelated gifts,
wild captures, trades, evolution or release do not deplete the network.

|Seller|Existing map / local ID|Existing coordinates|
|---|---|---|
|01|Route4_PokemonCenter_1F /2|1,3|
|02|CeruleanCity_House1 /1|6,5|
|03|VermilionCity /1|22,11|
|04|Route10_PokemonCenter_1F /3|2,3|
|05|LavenderTown /2|12,12|
|06|CeladonCity /6|30,24|
|07|FuchsiaCity /8|36,20|
|08|SaffronCity /10|21,23|
|09|CinnabarIsland /2|11,11|
|10|OneIsland /3|13,9|

Objects, sprites, coordinates, movement, visibility and map transitions remain
unchanged. In particular, Saffron's civilian hide flag still controls seller08;
Bill/Celio, Nurse, aide, tutors, fossils and other NPC interactions are preserved.
Only the approved sellers' optional interactions are replaced. The exact approved
intro/shared dialogue is checked independently against a handoff transcription.
No explanation of Shiny odds, traded ownership or EXP is added to dialogue.

Oak's original starter choices remain in this separate PR, with owner approval.
Pikachu starter, Oak Eevee/Nest Balls and project-wide starter exclusivity require
later integration. Existing off-Dex assignments elsewhere remain unresolved;
`audit_species_assignments.py --check` is a no-new-findings guard, not global
compliance. Poké Ball rules are a separate subsequent feature.

## Transaction, ownership and probability

`src/mystery_vendor.c` owns the central pool and reusable transaction. Existing
entry scripts select a vendor and call `data/scripts/mystery_vendor.inc`.
Native party-first/PC delivery, actual delivered-record naming and correct-box
transfer messages are reused. No/poor/full/invalid paths do not charge or deplete
the pool. Preflight capacity and allocation occur before random draws. After
successful delivery, both depletion flags and payment are committed without
exposing a nickname UI or yielding control first. A pre-purchase save may reroll
an unpurchased offer; a completed saved purchase may not be bought again.

`MysteryRandomBelow` rejects the incomplete upper interval of the16-bit RNG range.
A separate ten-way draw gives values0..8 Shiny and9 non-Shiny, the approved exact
90% rule. The Shiny branch samples valid personalities conditional on the normal
GenerationIII XOR<8 criterion; the other branch rejects accidentally Shiny PIDs.
This is real stored identity, not a sprite override. Normal `CreateMon` supplies
IVs, level5 moves, nature/gender/ability derived from the final PID, friendship,
HP and PP, with no held item. No global Shiny, EXP or obedience routine changes.
Statistical tests check the actual native decrypted Pokémon fields.

The stable foreign trainer ID is the32-bit complement of the player's ID and is
therefore different even if the player is named Mystery. OT name is exactly
Mystery, language remains the game default, OT gender is male. Native traded
EXP and badge obedience apply. No fateful-encounter flag or custom multiplier.
Native `GiveMonToPlayer` replaces OT data, so the delivered record is rekeyed:
XOR every secure word with oldOT xor newOT, then set the foreign header/name.
Plaintext, checksum, PID, IVs and moves remain intact. Simply setting OT_ID would
leave encrypted data invalid; native tests caught and corrected that issue.

## Persistence and compatibility

No save-block or Pokémon-structure layout changes. Seller01 retains original
`FLAG_BOUGHT_MAGIKARP`0x249; sellers02..10 consume previously unused0x300..308,
and species00..09 consume0x309..312. These19 new bits are checked for collision.
A historical bought-Magikarp save reserves that original seller and Magikarp in
the pool without changing the existing Pokémon, old price, ownership or Shiny
status. It consequently has nine new offers left, not ten replacement gifts.
Seller/species depletion counts must agree; inconsistent state fails closed.

Vendor species is carried in script-only VAR_0x8008. It never overwrites
VAR_TEMP_1: that map variable controls Cinnabar Gym locking. Native PC transfer
message leaves are reused after buffering the species directly, rather than
using the generic wrapper which assumes VAR_TEMP_1. Source review caught this
progression risk; new native checks cover lock state before/after purchases and
PC nickname/save/load both with and without the Secret Key.

Both new special helpers are appended before `gSpecialsEnd`, preserving every
old special index. Actual script execution caught and corrected an early version
which placed them after that dispatch-table boundary. Existing test RAM/ABI guards
remain enabled. Production adds a temporary100-byte heap allocation and small
ROM tables; no static EWRAM/IWRAM allocation or new graphics assets.

## Executed acceptance evidence

Focused tests use actual compiled ROM code in mGBA0.10.5, no supplied commercial
ROM or proprietary BIOS. Each independent fixture initializes a real flash save.
These controlled emulator scenarios are distinct from a manual full playthrough.

|Acceptance area|Evidence / status|
|---|---|
|Ten locations and dialogue|All ten original maps loaded, original actor selected, actual NPC script entry executed; each intro, No/699/700/repeat and money-window cleanup passed. Source font widths <=183px against208px available.|
|Unique complete pool|Ten complete purchases in ten seeded varied vendor orders; every pool species once, exact700 charge, seller/species flags. Prior caught/seen flags do not deplete offers.|
|Party / PC|Actual party delivery and full-party PC fallback, all420 PC slots occupied rejection, retry after freeing box3 slot7. Correct species/box transfer text and default nickname checked.|
|Nickname|Actual keyboard accepted name A on party and outdoor PC records; No paths preserve defaults; no extra award or charge.|
|Shiny probability|10,000 independent actual purchases with seeds mix32(0x705A9D13+i):8,989 genuinely Shiny (89.89%). Species chi-square18.434; joint species/Shiny chi-square27.442. Fixed thresholds8800..9200,40 and60, respectively; no seed selection to force a result.|
|Uniform mapping|655,425 host assertions executing extracted production RNG helper exhaust all accepted16-bit intervals for counts1..10 and rejection/wrap, plus count0. This proves mapping, not independence of the underlying deterministic PRNG.|
|Ownership / stored integrity|Actual native OT/name, decrypted fields, IsTradedMon and IsMonShiny checked, including player named Mystery and PC-to-party conversion.|
|EXP / obedience|Native Cmd_getexp controlled wild calculation100/150, trainer150/225, switched recipient and actual boosted engine text. Native disobedience checks at10/30/50/70/all badges and above-threshold cases; owner level99 control. This is not a whole post-victory leveling playthrough.|
|Save/load|Real flash save/load preserves purchases, flags, money, named party/PC records. Pre-sale reload restores unsold status/money/party and permits a fresh identity. Historical Magikarp migration tested.|
|Story / engine regression|Static fingerprints preserve unrelated script blocks, starter/battle/daycare/save/species/world tables and original objects. Full existing native suite is required separately; fingerprints are not executed story progression.|

Focused native transaction result:152,183 assertions PASS. The earlier UI run
passed6,283 assertions; expanded Cinnabar map-state checks are included in the
final full regression result recorded in the PR.
Full regression, clean rebuild and GitHub Actions results are recorded in the PR;
focused passes alone are not a claim of complete release acceptance.

Reproduce from an authorized cloud checkout after upstream build preparation:

```sh
python3 tests/run_vendor_tests.py
# Also run every preceding host suite listed in README/workflow.
make -j2
python3 tests/emulator/run.py
# Diagnostic subsets (do not replace the required full CI run):
LIGHTNING_VENDORS_ONLY=1 python3 tests/emulator/run.py
LIGHTNING_VENDOR_UI_ONLY=1 python3 tests/emulator/run.py
sha256sum pokefirered.gba > /tmp/vendor.sha256
make clean
make -j2
sha256sum -c /tmp/vendor.sha256
```

## Outstanding release checks and rollback

No physical cartridge testing, two-peer link handshake, manual full-world
progression, arbitrary power-loss injection or exhaustive quest-log replay has
been executed. Traded identity uses native data and behavior but does not certify
cross-build link compatibility. Shiny summary visuals are not independently
approved by these data tests. Old starter exclusivity remains an explicit future
integration check. Saved seller state uses ordinary engine save checksums; this
feature does not add transactional guarantees for abrupt flash write failure.

Review the separate PR without merging parents. Before testing saves, keep an
external backup. To roll back code, revert this branch's commits together or use
base b0bd3b86; do not reset unrelated work or main. Reverting code leaves newly
used flag bits in saves and the old Route4 flag indicates sold, so an original
pre-feature save is required for a fully reversible gameplay test. Purchased
Pokémon themselves use ordinary GenerationIII data with no new formats. Do not
clear sale flags to reclaim offers or release ROM/save binaries as artifacts.

Canonical dashboard and append-only ledger are maintained separately on
`docs/project-coordination` (PR#8). Direct Drive writing remains unavailable;
repository tracking requires later Drive synchronization. No merge, auto-merge
or release is authorized.
