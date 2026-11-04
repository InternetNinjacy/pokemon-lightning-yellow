# Fossil rewards and world-species eligibility

Authority: owner-supplied **Codex Implementation Update — Wild Encounters, Gym
Roster, Fossils, and Pokédex Compliance**, November 2, 2026. This supplied update
is the available approved specification. Its seven linked Drive documents each
returned HTTP401; none is claimed as read. The separately uploaded start prompt
refers to a ZIP export that has not been supplied. No contents of that missing
package, including Poké Ball mechanics, are inferred.

Base audited: `feature/route5-daycare-breeding`,
`596265d41f92fd3e42ee6b0a08d2cb8e0ed62e5d`, PR #10. PRs #2–#10 are still open;
only baseline #1 is merged. This work uses a separate branch
`feature/fossil-rewards-and-roster-audit` and preserves its parents. No merges,
automatic merging, ROM release or destructive changes are authorized.

## Approved implementation

The existing Oak aide inside Route10's Pokémon Center still requires **20 caught
species** and retains his location and saved one-time reward bit `0x2FA`. His
reward is now one Claw Fossil, with matching dialogue. No Everstone or replacement
Everstone location is added. Old saves where that reward was already collected
stay collected: this change does not grant a second reward retroactively.
The historical flag name remains an alias for source compatibility. A full bag
does not publish the reward bit; all other aides are unchanged.

The existing scientist in Cinnabar Lab's experimental room also offers the fossil
left unchosen in MtMoon, once. Reuse the existing NPC rather than add an object
that old inside-room saves would not contain. His ordinary restoration and the
other lab NPCs remain available. A pending restoration takes priority over the
new gift. The new dialogue preserves the approved study/revival/care meaning.

The original `FLAG_GOT_FOSSIL_FROM_MT_MOON`, `FLAG_GOT_HELIX_FOSSIL` and
`FLAG_GOT_DOME_FOSSIL` retain their original choice meaning and are never changed
by the new gift. Helix choice gives Dome; Dome choice gives Helix. No completed
choice, no choice bit, or inconsistent both-choice bits produces no gift rather
than guessing. Refusal or insufficient bag space retains availability. New bit
`0x2E5`, formerly unused, records successful unchosen-fossil collection only after
inventory success. No MtMoon scripts or map objects are changed.

The old revival selector assumed the player could have only one MtMoon fossil
and would stop after that fossil plus Amber. It now offers the four supported
fossils and Exit, rejecting unavailable/already-revived selections before item
consumption. Menu ID65 is appended; all old IDs remain stable. The original
Omanyte, Kabuto, Aerodactyl, nickname, party/PC delivery and pending-restoration
blocks remain unchanged. Claw adds Anorith through the same service at its
existing level5 convention, with new completion bit `0x2E6`. This technical
extension is necessary to make the approved new fossil restorable; it introduces
no new species, items, artwork or unrelated fossil acquisition.

All fossils keep their original key-item pocket. Normal saving/checksums and the
existing entrance transition that completes restoration remain unchanged. Both
new bits were verified unused before allocation. No save size, item/species ID or
daycare layout is modified. The original per-kind one-time revival convention
remains. Full party/PC failure keeps a ready restoration retryable as before;
claim runtime coverage of that edge only if a corresponding test executes.

## Eligibility audit and unresolved conflicts

`scripts/audit_species_assignments.py` reads the authoritative252-entry source
roster without renumbering species. It audits FireRed wild tables, trainer parties,
in-game trades, Trainer Tower sets, direct gift/egg/wild/event script commands,
species-valued script variables, and selected C-authored starters/roamers/special
pairs. Script variables and C references include display/check-only cases and
therefore require role review. The legacy battle-tower tables are separately
classified; their inclusion does not prove runtime reachability. Dynamic
assignments from external/link data and every possible script dataflow are not
proved by this static scanner.

The first complete inventory is
`tests/specifications/unresolved_species_assignments.json`. These are **findings,
not approved exceptions**. It retains file paths, line references, context and
species. Some references occur multiple times; counts are source occurrences,
not that many different species or encounters. Current scan:4,913 assignments/review candidates;281 authored off-Dex occurrences (199 wild,24 ordinary trainer,54 Trainer Tower,4 direct events),279 legacy battle-tower references and12 variable/C review candidates. There are572 findings overall, not572 demonstrated live encounters.

Commands:

```sh
python3 scripts/audit_species_assignments.py --json
python3 scripts/audit_species_assignments.py --strict
python3 tests/run_world_reward_tests.py
```

`--strict` intentionally fails while confirmed authored off-Dex assignments remain. Legacy/reference candidates are reported separately and still require review before global certification. CI's `--check`
mode blocks newly introduced findings relative to the explicit unresolved
inventory; it does **not** certify global compliance or authorize historical
violations. Line-only movements do not create new findings; duplicate occurrences
are counted. Do not silently refresh the inventory to approve new assignments.
Approved replacement/removal specifications are required before changing
unrelated encounters, gifts or teams. Existing tables remain untouched here.

The source still gifts Togepi at Water Labyrinth and includes excluded stationary
Deoxys/Lugia/Ho-Oh assignments. These conflict with the global eligibility rule
and potentially the unavailable locked special-overworld specification. Flag the
conflict; do not remove the encounters, invent a substitute or expand the Dex.
Ordinary trainer conflicts include Marill, Hoppip/Skiploom, Smeargle, Qwilfish,
Chinchou/Lanturn, Girafarig and Snubbull. Full lists and wild slots are in the
inventory. This audit is not a claim all252 species are obtainable.

## Blaine, Ditto and missing package dependencies

Approved Blaine roster: Donphan, Gyarados(custom#146), Sandslash, Aerodactyl,
Flygon(ace); Donphan/Gyarados open together in a Double Battle. All five are in
the custom roster. The actual branch still has original Growlithe/Ponyta/Rapidash/
Arcanine, `.doubleBattle=FALSE` and a single-battle event. There is no Skarmory
entry to replace in that source. The full approved levels/moves/items and all
seven ordinary trainer specifications are unavailable. No new configuration or
guessed levels/moves are written. Original Gym files are fingerprinted unchanged.

There is also a concrete typing conflict: PR#9 implements the earlier approved
Gyarados Water/Dragon override, while this update's strategy relies on Water/
Flying. An owner clarification is pending; do not silently revert global typing
or claim Ground immunity. Record any returned decision as a separate effective
design update before implementing it.

Route5 wild Ditto existence remains approved under WRL-005. Its rate, levels and
remaining encounter table are not specified. Ordinary encounter ecology remains
a draft; do not implement it or select final wild-double locations/pairs before
the underlying tables are approved. No Rock Tunnel dig site or Everstone location
is added. Poké Ball work is explicitly queued but blocked on the missing exported
mechanics and authorization files named in the uploaded start prompt.

## Validation and review

Run all prior host suites plus `python3 tests/run_world_reward_tests.py`; build
with `make -j2`, then execute the complete `python3 tests/emulator/run.py` default
suite. `LIGHTNING_FOSSILS_ONLY=1` is diagnostic only. The new preservation audits
are explicitly static; real rewards, menus, flags, inventory and save/load are
exercised separately in compiled-ROM emulator fixtures. These start the existing
NPC script entry points on the actual maps, rather than claim a manual walk to
every NPC. Final execution counts/CI links and failed trials belong in the PR and
canonical ledger; compilation alone is not acceptance.

Canonical dashboard, permanent task IDs and append-only ledger remain on the
separate `docs/project-coordination` branch in PR#8. Drive synchronization is still
pending. Roll back this unmerged code series without rewriting its parent.
Already collected rewards, consumed items and revived Pokémon are persistent
gameplay and are not undone by reverting code. Keep private authorized backups;
old builds do not understand a Claw restoration in progress. Existing Route5
upgrade rollback warnings continue to apply. No ROM/save/screenshot artifacts
are committed or uploaded.

## Executed development checkpoint

All seven host suites pass, including the explicitly static world-preservation
and no-new-conflicts audit. Build and clean rebuild pass with identical output.
A focused real-ROM fossil run passed395 assertions before final diagnostic cleanup
and additional membership checks: aide19/20/full bag/old reward bit; both opposite
fossil gifts with refusal/full bag/save/retry; all four restorations in ONE SAVE,
including cancellation, exact-once consumption, entrance-triggered readiness,
registration and flash persistence. Final complete-suite counts and Actions
conclusions will be recorded in the PR and canonical ledger for their exact head.

The new full-bag scientist path initially crashed because it reused an aide text
without buffering STR_VAR_2. Buffer the actual fossil name before displaying that
shared text; the same native full-bag scenario now passes and preserves the gift.
This was a new implementation defect, not an upstream defect. No failed trial
is counted as passing. Separate fixture corrections used the item's actual
key-item pocket, initialized both seen/caught anticheat copies, honored Thumb task
pointers and advanced final text only at the engine's actual input-wait states.
A longer completion bound accommodates the existing multi-page aide dialogue;
all inventory/flag/persistence assertions remain required.
