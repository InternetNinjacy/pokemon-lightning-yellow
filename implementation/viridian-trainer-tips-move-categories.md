# Feature: Viridian City Trainer Tips sign — move categories

**Project:** Pokémon Lightning Yellow  
**Status:** Ready for implementation, subject to specification PR review/merge  
**Approval:** Owner explicitly approved the exact proposed sign message on November 4, 2026, in this conversation  
**Handoff revision:** 2026-11-04  
**Dependencies:** None required to change sign text. The per-move Physical/Special/Status split is an independently tracked gameplay feature; do not alter it in this task.

## 1. Goal and player-facing behavior

Replace the **northern existing Trainer Tips sign** in Viridian City with the approved instructional message explaining that a move's Physical/Special classification is not necessarily dictated by Pokémon type. Interacting with that same existing sign displays the new text repeatedly, as before.

## 2. Approved design decisions

Approved user-facing message, faithfully retaining the wording:

> TRAINER TIPS!
>
> A Pokémon's type doesn't always tell you whether its moves are physical or special!
>
> Check each move carefully when building your team!

The author-approved words and meaning must be retained. Adapt capitalization, wrapping, embedded game text tokens, and page breaks to FireRed text box conventions, as necessary to make all text legible without overflow. Do not silently rewrite its meaning or add new hints.

Target the sign at **Viridian City map (23, 1), elevation 3**, which invokes `ViridianCity_EventScript_TrainerTips1`. It currently displays `ViridianCity_Text_CatchMonsForEasierBattles`. **Do not** replace the second Trainer Tips sign at (20, 31), which invokes `ViridianCity_EventScript_TrainerTips2` and teaches PP.

## 3. Authoritative sources and provenance

- Owner approval: November 4, 2026, current design conversation, explicit "Approve" response to the exact proposed Viridian Trainer Tips text.
- Master Plan and Lightning Yellow project rules: https://docs.google.com/document/d/1P4TuXViTpB0BycldB1SzRZTRX8sV_Rd0lFuPwyGDJQk/edit
- Current project startup and source authority: https://docs.google.com/document/d/14bV_mRB-IgcyWqGcIH2tgmvNoYLY1Ip_zSqSbdHGYtw/edit
- Current change intake policy: https://docs.google.com/document/d/1NpCjx3uboBMdBVqoFpMwXEyVkDquD37Kcp-FJBbYWnc/edit
- Verified current main source: `data/maps/ViridianCity/map.json` includes both sign events; `data/maps/ViridianCity/scripts.inc` maps TrainerTips1 to `ViridianCity_Text_CatchMonsForEasierBattles`; `data/maps/ViridianCity/text.inc` contains the original text label.
- Lightning Yellow approves a Gen IV-style per-move category split with an explicit special-case exception for Shadow Ball. This sign is a general tip only and does not modify any mechanical decisions.

## 4. Implementation boundaries

**In scope:** Replace text displayed by the northern Viridian City Trainer Tips sign, using the verified existing event/text label; minimal line wrapping and pagination adjustments.

**Out of scope:** Physical/Special/Status engine implementation, move data, Pokédex, trainer teams, sign placement, map editing, object graphics, dialogue on other signs, new flags, battle/quest state changes, or changes to any other Pokémon game.

**Preserve:** The southern Viridian PP sign, original interaction type (`MSGBOX_SIGN`), existing map connection/events, repeatable reading, and unrelated scripts/text.

## 5. Technical implementation notes

Existing source checked on main on 2026-11-04:

- `data/maps/ViridianCity/map.json`, `bg_events`: north sign (x 23, y 1, elevation 3) -> `ViridianCity_EventScript_TrainerTips1`.
- `data/maps/ViridianCity/scripts.inc`: `ViridianCity_EventScript_TrainerTips1` uses `msgbox ViridianCity_Text_CatchMonsForEasierBattles, MSGBOX_SIGN`.
- `data/maps/ViridianCity/text.inc`: change the contents of `ViridianCity_Text_CatchMonsForEasierBattles` while retaining the symbol name if feasible. Existing sign wording is obsolete after this change, but the stable symbol is convenient and avoids unnecessary modifications.
- FireRed text constants (such as `\n`, `\p`, and `$`) and literal in-game font glyph conventions must be respected; Codex should use adjacent sign scripts as examples and verify text length and readability.

## 6. Acceptance criteria and validation

- [ ] Reading north sign at (23,1) displays the approved Trainer Tips message in full, with no clipped text, unintended glyphs or box overflow.
- [ ] Reading the same sign again behaves identically; no state flags or one-time behavior.
- [ ] Southern Trainer Tips sign at (20,31) still explains PP exactly as before.
- [ ] No unexpected differences to other city sign scripts or map data.
- [ ] Diff stays minimal and focused on sign text.
- [ ] Run available build/static checks appropriate to approved gameplay changes and report results. Follow repository `AGENTS.md` and do not change validation gates merely to make checks green.
- [ ] If practical, verify the message in a FireRed emulator and report whether it was actually tested; do not claim emulator verification unless performed.

## 7. Open questions and blockers

None for design. Codex may make routine layout and formatting choices required by the original FireRed text engine. If a technical blocker prevents faithful display, report it rather than altering approved wording on its own.

## 8. Codex delivery requirements

Implement on a separate topic branch after the reviewed specification is available to the Codex workspace. Open an implementation PR; report affected files, exact tests, any unrun emulator checks and remaining blockers. Never auto-merge, commit ROMs, or alter unrelated content. If main already differs, reconcile rather than overwrite concurrent work.

## 9. One-line Codex handoff

`Implement implementation/viridian-trainer-tips-move-categories.md from main following AGENTS.md. Replace only the northern Viridian City Trainer Tips sign text as specified, run relevant checks, and open a PR. Report blockers rather than guessing.`
