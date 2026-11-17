# PKM-009 — All-252 TM/HM Source-Parity Acceptance Ledger

**Status:** source parity verified; *NOT* final TM/HM feature acceptance or approval of new compatibility rights.  
**Repository baseline:** current `main` at branch creation, November 9, 2026.  
**Source:** `src/data/pokemon/tmhm_learnsets.h` Git blob `0aa21e85be2f970536c8ed901c556013c16327e5`.  
**Historical comparison:** PKM-009 source reconciliation PR #154's four frozen 63-species shards; historical source commit `37230a9c513ed013fc6b402ec800bd1d13960492`.  
**Authoritative later decisions:** existing Lightning Yellow TM/Move Register (TM01–TM50 maintained; TM51 Zap Cannon approved as new separate machine; original TM34 Shock Wave remains distinct).

## Complete original-source verification

- Exact 252/252 unique regional-Dex species, all numbers #001–252 present.
- 5,831/5,831 original species-to-TM/HM eligibility records match the prior source snapshot. Zero discrepancies or machine eligibility edits were made.
- 58 distinct original machine names: TM01–TM50 plus HM01–HM08 (HM08 exists as original data even though normally unobtainable in FireRed).
- Per-species ordered machine lists are frozen in `tests/specifications/pkm009_tmhm_252_source_contract.json`. The automated test `tests/test_pkm009_tmhm_252_source.py` extracts each independently from the production source, ensures 252 exact arrays, distinct legal machine names, 5,831 memberships, verifies the original full-file Git blob and no added TM51 item or compatibility.
- This PR intentionally changes **no** TM/HM bits, species stats, learnsets, trainer parties, move mechanics, encounter rules, evolution methods or saves.

## Distinguish owner approval from unfinished implementation

**TM34 Shock Wave** retains its original identity. Latest owner-approved placement is an ordinary visible item ball on Route 6 after Misty defeat; location/tile/event implementation belongs to the relevant world/quest workstream, not an inferred TM compatibility extension.

**TM51 Zap Cannon** is a new separately approved machine intended as Surge's reward, not a repurposing of TM34. Current source lacks `ITEM_TM51` and all `TM51` species bits. The 64-bit original TM/HM compatibility representation currently addresses 58 machines, but the proposed bit 58, UI/item routing, redemption, behavior, saves, full final eligibility set, and engine tests cannot be marked complete from source parity alone. The exact TM51 species learnability matrix is **NOT established** in the retrieved approved source register; do not guess by Electric typing or original Zap Cannon learnsets.

**Custom moves blocking 12 species:** `SEED_BOMB`, `POISON_JAB`, `VOLT_SWITCH`, `DISCHARGE`, `SNARL`, `PAYBACK`, `FOUL_PLAY` remain undefined on current main; this is separately owned by BTL-005. Not an authorization to implement engine mechanics here.

## Gate to final PKM-009 acceptance

1. Preserve existing successful source implementation validation: #171 (146 delegated) → #173 (12 owner-locked, 158 combined), no duplicate merge of standalone #172 or earlier feature PRs.
2. Obtain exact owner-approved **TM51 species eligibility** and BTL-005 mainline custom moves; implement without speculative rights or move IDs.
3. After exact dependencies land, validate 12 outstanding species and their natural moves, rerun all 252 source-contract and original TM/HM checks, TM51 item/bits/UI/SURGE reward native mGBA acceptance, and verify merged-main exact-head CI.
4. Do not claim whole-workstream acceptance, merged status or effort points from this preservation-only ledger.
