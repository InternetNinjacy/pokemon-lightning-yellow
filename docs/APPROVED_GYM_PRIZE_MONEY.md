# Approved Gym prize money — Brock and Misty

Source branch: `implementation/approved-brock-misty-prize-money-20261116`, based on main `197d85ee1a61ac319f95fa65ab298bb126fe937d`.

## Approved behavior

The canonical [Master Plan](https://docs.google.com/document/d/1P4TuXViTpB0BycldB1SzRZTRX8sV_Rd0lFuPwyGDJQk/edit) explicitly fixes Brock's prize at1400 and Misty's at3000. Misty's final Cerulean closeout retains3000 for her approved Double Battle. These are base trainer-victory prizes; existing player moneyMultiplier/Amulet Coin behavior remains intact.

## Implementation

Only `src/battle_script_commands.c` changes gameplay. Existing `Cmd_getmoneyreward` receives two opponent-ID branches inside its original victory path. Brock selects1400 times the existing moneyMultiplier; Misty selects3000 times that multiplier. They bypass only the generic last-party-level/class/Double formula, which does not encode the approved fixed totals. No additional Double multiplier is applied to Misty's explicitly approved full-battle prize.

The existing secret-base branch, generic formula for all other trainers, AddMoney cap, result-message number, script-pointer progression, whiteout loss and separate Pay Day command are unchanged. No new money engine, event grant, save flag or repeatable reward is introduced. No old-save payment or migration. Original battle routing/parties/reward items remain with their existing owners; this applies to the original Brock/Misty IDs independently of their current/pending team changes.

## TM51 decision and ownership

Current main has NUM_TECHNICAL_MACHINES50 and no ITEM_TM51. TM Case display/index logic assumes original contiguous TM/HM IDs, and the compatibility table has no TM51 rights. The [Vermilion handoff](https://docs.google.com/document/d/1E3_0OL53WsYfN_Dvq_bJhuGMHxpokJ0p0XFGW2zzkqM/edit) explicitly leaves compatible species TBD; `docs/PKM009_TMHM_252_SOURCE_PARITY.md` preserves that dependency. Do not create speculative machine learners or change Surge's reward to an undefined item. New TM51 engineering remains separate, alongside PR175's parity ownership. Source implementing native Zap Cannon's rain accuracy already exists; it is not duplicated here.

No open PR found owns these fixed payouts. Source-hunk scope is separate from BTL005 move effects in the same large battle-command file; integration must retain both. PR259 Route6, PR260 Pewter prose/Hail/badge, PR261 Cerulean prose and PR262 Vermilion prose are untouched.

## Execution status

No tests, builds, emulator runs, diagnostics, failed-test investigation or workflow dispatch initiated. No tests/workflows/gates altered; [skip ci] prevents automatic push/PR test activation. No passing CI or runtime behavior is claimed. Required checks and owner merge authorization remain unresolved; draft/unmerged, no points. Preserve50 task IDs and461 earned/251 remaining/712 total.

Minimal implementation inspection: canonical exact prizes, current native payout routine, original Trainer IDs, active feature ownership and current TM constants/UI/compatibility. This is source evidence only. The executor is unavailable; all code changes are made through GitHub, and the Python dashboard renderer has not been run. Its existing GYM002 row is refreshed directly from the changed task record while preserving all other dashboard rows and historical snapshot notices.
