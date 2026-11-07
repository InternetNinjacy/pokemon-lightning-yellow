# BTL-006 independent acceptance evidence (2026-11-07)

Inspected `main` at `21a3630b3d51481ab10a1af9637fa1ced73722cc`. Historical production feature: PR #13, merged as `d54f7a8e85d38b8fa304399dcb4d14778396d5cd`. This document adds acceptance evidence without changing authorized gameplay.

## Evidence already executed

- [PR #13 Actions run](https://github.com/InternetNinjacy/pokemon-lightning-yellow/actions/runs/41688991039): modified and independent upstream jobs succeeded; compiled-ROM mGBA reported **274,675 assertions**. This count includes API/register safety checks; it is not 274,675 manual playthroughs.
- Host `tests/run_capture_tests.py`: production helper cases, 11 ordinary shop stocks, unchanged early prices and 13-ball identity/art mapping. Historical Actions confirms this runner passed.
- Native `tests/emulator/capture_rules.c` via `tests/emulator/run.py`: weather/origin/cave policy, exact Premier whitelist, 13 ball capture codes, Snow/Potion flash persistence; atomic bonuses at quantities 1/9/10/19/20/30/99, failure when Premier stack is full, no cross-type pooling; Snow throw/animation, both survivor sides, PC storage, one-use consumption and flash restore; actual Vermilion Mart buy quantity/confirm/return (10 Poké + 1 Premier, money charged once, reload); Oak NPC full-pocket retry giving 5 Nest; six-turn permanent Hail, weather replacement and temporary expiry.
- **Additional independent source verification** on November 7: 13 of 13 checks passed for all-ball item metadata/UI descriptions/handlers, Wet/Dust/Snow labels, three early-Mart specialty exclusions, Two Island staged shop integrity, Oak receipt versus Route 22 refill, Snow item-icon reuse, summary decode and battle animation decode.
- Added deterministic static regression assertions to `tests/run_capture_tests.py` so future Actions checks preserve those presentation/progression boundaries. Review CI for this PR before counting them as executed on GitHub.

## Acceptance matrix

| Requirement | Result | Evidence/limits |
|---|---|---|
| Capture multipliers and permitted conditions | PASS scoped | Host/native helper tests; not each real-time random weather path in an ordinary playthrough |
| Inventory IDs, encoded Snow identity, flash reload | PASS scoped | Host/native save fixtures and all 13 encoded ball values |
| Shop stock/prices and bonus fulfillment | PASS scoped | Eleven stocks; native single transaction; boundary quantities via actual ARM helper |
| Oak gift, full-pocket retry and Route 22 refill separation | PASS scoped | Native NPC/persistence; separate script/text regression |
| Text, Ball icon, summary/throw mapping | PASS static + selected native | Every ball has UI metadata; Snow throw and summary decode; no exhaustive screen-capture review of 13 displayed ball variants |
| Exceptional retailer progression | PASS preservation scope | Two Island four stages preserved by static regression; no full story-gated progression walk, Trainer Tower manual visit |
| Weather and other feature integration | PASS scoped | Native permanent/temporary Hail, Rain/Sun/Sand, double-battle Snow survivor, PC delivery |
| Hardware, two-peer, interrupted-power save, exhaustive quest-log replay | NOT EXECUTED | External environment not supplied; do not claim certification |

## Disposition / Master Control

No defect established by reviewed source and the original execution evidence. No gameplay files changed. **Do not automatically award the final 1/16 point or claim full visual, external, hardware, or manual progression acceptance on the basis of static checks.** Current tracker remains 15 earned / 1 remaining while the owner/Master Control determines whether the unexecuted final presentation/exceptional-retailer verification is genuinely required for BTL-006, versus reserved for shared integration/release qualification. If a task-specific acceptance requirement is confirmed, run and record the smallest real emulator journey for all 13 Ball summaries and the exceptional shop path, and then reassess closure. No Codex needed.

Historical ledger and centralized dashboard untouched pending Master Control coordination.
