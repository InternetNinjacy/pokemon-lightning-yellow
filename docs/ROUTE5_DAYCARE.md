# Route 5 daycare breeding — DAY-001 through DAY-004

Authority: owner's complete **Route 5 Daycare Breeding — Complete Codex Implementation Handoff**, consolidated November 2, 2026 and supplied in this conversation. It unblocks the former missing-register dependency. Linked Drive originals have not been read directly; this supplied complete handoff is the available design authority. Implementation/testing/review/merge are separate statuses.

Audited base: `feature/phase-one-stabilization`, `6ff574dc205a007a93a4724fd6eda867eeff8fdf` (open PR #9). Main is `e04252ea0783510000e634486c0f2526a736f506`. No newer daycare branch existed in the remote branch/PR audit. Recoverable reference: `recovery/pre-route5-daycare-2026-11-02`; implementation branch `feature/route5-daycare-breeding` is stacked on #9. Parent PRs are unchanged and unmerged.

## Approved scope

Two deposited Pokemon and ordinary Generation III breeding before the Elite Four; same compatibility, offspring selection, inheritance, leveling, withdrawal cost and hatching rules. Independent Four Island/Route 5 parents, progress and pending eggs. One male and one female caretaker, presentation only with no gender-specific deposits. Each newly generated Route 5 egg gets an independent exact50% intended Shiny outcome, persistent and recognized by the ordinary engine calculation. Wild Pokemon, Four Island eggs and other gifts/eggs retain ordinary behavior.

Route 5 wild Ditto existence is separately approved; rate, levels and the rest of that encounter table are not. This patch must not invent those values. Track that dependency separately from the daycare implementation.

## Storage and migration

The target compiler enforces `DaycareMon=0x8C`, `DayCare=0x11C`, `SaveBlock1=0x3D68`; Four Island remains at0x2F80, legacy Route5 at0x3C98 and the following ramScript at0x361C. A versioned `Route5DaycareSave` consumes exactly the formerly unused400 bytes at0x348C. No field after it moves, no runtime save allocation grows, and no sector layout/checksum algorithm changes. Existing full-save sector checksums cover this area.

First access accepts only an entirely zero legacy padding region, copies the **complete** old `DaycareMon` into upgraded slot0, and publishes version/magic last. The old record remains byte-for-byte as a read-only forensic recovery copy; it is never stepped, used as a second deposit or re-imported after successful migration. Empty migrated state cannot resurrect that archived mon. Original mail, nickname, OT, PID, moves, encrypted data and accumulated steps are preserved. New saves have zeroed padding and migrate empty.

Unknown occupied padding, unknown recognized versions and inconsistent pending identity metadata fail closed: no erase/reset and no fallback into Four Island storage. Route5 service displays a recovery notice. Preserve the save and investigate rather than force initialization. Arbitrary third-party modified saves using the old padding are not automatically compatible. Sector redundancy handles interrupted writes as before; no claim of power-cut hardware testing is made.

Shared daycare wrappers resolve the current map's site; Route5 interior selects its own record, all original contexts retain Four Island. Natural parent-step processing takes explicit facility pointers and never uses current-map routing to generate the other facility's egg. The original global party hatch clock executes once per step. Route5's local counter does not hatch party eggs a second time. The original Four Island pending flag and scripts remain exclusive to Four Island.

## Persistent real Shiny identity

Generation time is safest: pickup-time rolls allow repeated attempts to change an already pending egg. Route5 generation consumes the original-style low16 draw, a high16 draw, and **one new fair RNG bit**. Both outcomes are explicitly enforced: tails cannot accidentally remain naturally Shiny. Full PID, original trainer ID and intended outcome are saved while pending. Pickup/cancellation/full party do not roll a new identity. The low16 zero sentinel remains reserved.

`Route5EggPersonality` keeps the chosen nature (`PID%25`), low8 gender/ability bits and bit15's Nidoran/Illumise branch. A bounded128×8 search produces a legal Shiny PID using ordinary trainer-ID/PID XOR; non-Shiny construction chooses a congruent high half and verifies XOR>=8. A reduced exhaustive modular proof covers all25 natures for every128 XOR block. This is not a palette override or a change to the global Shiny threshold. IV and move inheritance routines remain unchanged; GenIII Everstone nature inheritance is not added.

FireRed normally assigns the receiving/hatching trainer's OT ID, which could change a traded egg's Shiny result. Route5 eggs carry an encrypted/checksummed marker in the otherwise unused4-bit ribbon nibble (A=non-Shiny, B=Shiny) while eggs only. On hatch, only marked Route5 eggs may have their PID re-expressed against the hatching trainer's ID to retain their saved outcome, nature, gender and ability. Normal OT assignment remains. The marker is cleared by the ordinary hatch reconstruction and is not a visible ribbon. Untagged Four Island/gift eggs retain their original PID and behavior. A matching Lightning Yellow build is required to understand this marker; vanilla/other-hack hatchers do not preserve the guarantee. A real link handshake remains distinct from native ownership-change tests.

PID bits that have no approved inherited gameplay meaning can differ when constructing a valid outcome. Unown is normally unbreedable; decorative Spinda spots and Wurmple's PID-derived branch outside the approved roster are not promised identical to a discarded preliminary random PID. No player-visible generated egg is rerolled. Nature/gender/ability and special offspring branches are explicitly preserved.

## NPC and script integration

Technical placement uses the owner's conditional both-inside preference: existing floor tiles provide reachable positions without any map-art change. Existing deposit object/localID1 stays at(4,4), now using the existing OLD_WOMAN sprite; localID2 is the existing GENTLEMAN at(2,4), handling eggs. Both face down and remain stationary, with interaction tiles(4,5)/(2,5) connected to entrance(3,7). Coordinates are implementation choices presented in the reviewable PR, not claimed as a separately approved final interior design.

Route5 reuses Four Island's existing service text and menu/payment scripts, under distinct labels. Four Island's furniture-specific retrieval movement is omitted on Route5, so the caretaker cannot walk into its different furniture. Either parent's sex is accepted in either deposit slot; compatibility alone governs breeding. Declining Route5 pickup keeps the same pending egg; full party and cancellation retain it. Four Island's existing rejection/discard behavior is unchanged. Only a technical recovery notice is new text; final narrative dialogue remains available for owner review.

## Validation and status

Build instructions remain in README/INSTALL.md. Run all prior suites plus:

```sh
python3 tests/run_daycare_tests.py
make -j2
python3 tests/emulator/run.py
# Focused diagnostic only; CI must run the full default suite:
LIGHTNING_DAYCARE_ONLY=1 python3 tests/emulator/run.py
```

Host policy tests exercise real migration and PID routines with stubbed save/RNG state. They do not pretend host pointer widths equal GBA save layout: target assertions remain enabled in the ROM build and the emulator generates offsets with agbcc. Reproducible statistical seeds use `mix32(0x2A551701+i)` with xor16/mul7FEB352D/xor15/mul846CA68B/xor16; an additional10,000-egg unreseeded stream starts0x51E832A9. Test actual XOR state, not sprites. Final build/host/emulator/statistical outcomes and CI links must be recorded in the PR/ledger after execution.

The harness must use installed `mgba/flags.h` before core headers to match the library's optional debugger vtable slots. CPU snapshots do not substitute for flash snapshots; independent daycare fixtures explicitly create and verify blank flash, while save/load cycles within a fixture retain its real flash contents. Save addresses follow current `gSaveBlock*Ptr`, because ordinary map loading relocates save blocks. Never disable ABI/stack/save assertions to pass a test.

At this checkpoint code builds and host policy tests pass; expanded emulator acceptance is ongoing. Do not mark production-complete based on that statement. Required evidence includes actual service/menu scenes, both facilities, legacy mail/step migration, full party/refusal/cancellation, experience/payment, original compatibility/inheritance, pending/party/hatched save states, actual shiny/hatch behavior and10,000 real stored egg outcomes. No ROM/BIOS/save/screenshot artifacts may be uploaded or committed.

## Recovery and limitations

Before changing ROM versions, retain a private authorized pre-upgrade save backup. Old ROMs do not understand the second daycare or egg marker. Reverting code is **not** a safe save migration: the archived legacy record is stale after upgraded play, and blindly exposing it can duplicate/resurrect a previously withdrawn deposit while hiding later deposits. Return/recover upgraded parents and pending eggs on the new build and use an appropriate backup or a separately reviewed reverse migration. Do not clear unknown storage. Git rollback preserves parent PRs; no merge or auto-merge is authorized.

Direct Drive synchronization remains unavailable. Physical cartridges, real two-peer link trades, foreign ROM peers, power interruption and a full progression playthrough require separate evidence. Pending Ditto rate/levels do not block independent daycare engine work.

## Implemented validation milestones

- All previous host suites continue to pass; new host policy suite460,161 assertions plus separate preservation audits. Varied-seed10,000 pending identities:5066 Shiny/4934 normal; unreseeded10,000 stream:4998 Shiny. Fifty thousand varied PID/OT inputs test both outcomes with nature and low-bit preservation; all128 reduced XOR blocks cover all25 natures.
- Focused compiled-ROM core run passed103,114 assertions before adding final UI/timing cases: legacy mail/steps migration; independent parents/pending eggs and flash; standard compatibility/offspring/incense/egg+TM moves/IVs; Shiny ownership/hatching and a real744-frame hatch scene;10,000 actual Route5 eggs5066/4934 and10,000 actual Four Island controls1 Shiny with no marker. These counts are execution evidence for that milestone, not the final full-suite count.
- Focused native UI run passed1,359 assertions: actual walking and caretaker interactions, party-menu cancellation, female+male deposits through one caretaker, usage statistic, insufficient-funds rejection, standard experience growth and1800 paid withdrawal, one/last-healthy refusal, full-party egg refusal, pending identity retention and pickup; original Four Island NPC deposit and native withdrawal.
- A real rev0 menu defect was found: `DaycarePrintMonLvl` used C `strcpy` on game-EOS text, stalling/corrupting the withdrawal level menu. It now uses `StringCopy`, matching the upstream later-revision path. This shared formatter repair necessarily also applies to Four Island, while preserving its breeding/state/payment behavior; its original map/scripts remain byte-identical. The same failing UI case passes after the fix.
- Harness corrections were separate from game fixes: honor relocated save pointers; include installed mGBA build flags before ABI-sensitive headers; verify fresh flash per independent fixture; use A for field messages and B only for nickname refusal; avoid reentering Pokemon crypto getters while a live callback may be between decrypt/re-encrypt; hold movement through the avatar's turn before checking a step; initialize encrypted game-stat counters with the engine setter. No assertions were disabled or failed trial counted as passing.

Final clean rebuild, full combined regression and GitHub Actions conclusions will be recorded in the PR and canonical ledger. Real two-peer link traffic, hardware power interruption and a full progression playthrough are not established by these fixtures.

The final save helper clears both in-RAM save blocks **and the separate global party/count** before reload, so party-egg and post-hatch checks require serialized flash restoration rather than leftover Pokemon RAM. Excluded-species cases (for example Illumise or incense families) occur only in isolated original-engine test fixtures; this patch adds no species, encounters or Dex entries. Standard breeding of externally imported eligible species is not silently replaced with a roster filter.
