# Mystery Pokemon Vendor Network — implementation checkpoint

Authority: owner-supplied Mystery Vendor Codex Implementation Handoff v1.0,
prepared November 2, 2026; read in full from the uploaded DOCX. The owner then
explicitly approved implementing the network independently of starter changes.

Base: fossil PR #11, b0bd3b86f4e03dd3f540f0776a5de87f207e84e4.
Branch: feature/mystery-vendor-network. No worktrees or parent changes.

## Approved integration boundary

Preserve Oak's current original starter scripts in this PR. The source still
offers Bulbasaur, Squirtle or Charmander; Pikachu starter and the separately
specified Oak Eevee parcel/Nest Ball feature are not implemented here. Starter
exclusivity therefore remains an explicit integration dependency. Do not claim
full project-level exclusivity or alter rival/story progression to hide this.
The owner approved this boundary after the concrete source conflict was reported.

## Locked implementation requirements

Exactly ten existing NPCs, unchanged objects/movement/visibility: Route4 Center,
Cerulean House1 BadgeGuy, Vermilion Woman, Route10 Center FatMan, Lavender WorkerM,
Celadon OldMan2, Fuchsia OldMan, Saffron WorkerM, Cinnabar OldMan, OneIsland BaldingMan.
Use their exact approved individual/shared dialogue; game-supported currency700.
Pool: Bulbasaur, Squirtle, Charmander, Kangaskhan, Pinsir, Feebas, Chansey, Dratini,
Absol, Magikarp. Each level5, uniform remaining-species draw without replacement,
one sale per vendor, unbiased90% genuinely Shiny/10% genuinely non-Shiny.
Shared OT name Mystery, stable foreign ID distinct even for player named Mystery;
real traded EXP and badge obedience, no invented multipliers or fateful status.
Party-first/PC fallback, actual delivered-mon nickname and correct-box transfer.
No funds/state/RNG draw on declined/poor/full-capacity paths; commit money and
both depletion flags only after successful delivery, before nickname UI.
Preserve parent fossil/daycare features and other NPC rewards/story events.

## Next technical checkpoints

1. Audit all ten current scripts and objects, persistent flags, native delivery,
   storage/nickname/EXP/obedience paths; preserve source fingerprints.
2. Resolve legacy Route4 bought-Magikarp state safely: its flag is used only by
   the old salesman script. A migrated old purchase must not break seller/pool
   count invariants or erase/duplicate an existing Pokemon.
3. Implement one reusable transaction helper, central seller/species data and
   shared scripts. Verify free persistent flag allocation before editing.
4. Execute T01–T17 with host and real-ROM evidence, clean rebuild and Actions.
   Include statistical draw/Shiny tests, actual party/PC naming, ownership,
   boosted battle EXP, obedience and save failures. Never substitute static
   inspection for runtime acceptance.
5. Update canonical dashboard/append-only ledger on docs/project-coordination;
   prepare separate unmerged PR when implementation/evidence are reviewable.

Current status: handoff read; base/source starter conflict investigated; owner
boundary approved; branch created. Vendor code NOT implemented, built or tested;
no vendor PR yet. Existing PR #11 results do not validate this future feature.
No merge, auto-merge or ROM release is authorized.

## Additional supplied documents — November 2

Read all five subsequent uploads: vendor pool CSV; two byte-identical Poké Ball
rules CSVs; DOCUMENT_CHECKLIST.md; FEATURE_STATUS_AND_AUTHORITY.md. The vendor
CSV independently confirms all ten species, level5, price700,90% and OT Mystery.
The status document explicitly confirms starter exclusivity is an integration
dependency, not a vendor implementation blocker. Gyarados remains unresolved;
the special-overworld subsystem has an explicit separate implementation hold.
Poké Ball work follows the current vendor task and remains isolated. The file
checklist describes source exports; those described TXT exports were not among
these five attached files and are not claimed read. A checklist is not evidence
that its listed files are accessible. No merge/release authorization is added.
