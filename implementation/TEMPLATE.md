# Feature: <descriptive feature title>

**Project:** Pokémon Lightning Yellow (not a separate Pokémon ROM-hack project)  
**Status:** Draft / Blocked / Ready for implementation / Implemented / Verified  
**Approval:** <explicit approval and date; do not infer approval>  
**Handoff revision:** <date or revision ID>  
**Dependencies:** <other approved features, or none>  

## 1. Goal and player-facing behavior

<Exactly what changes in the game. Include trigger, availability, interaction, success/failure and repeat behavior as relevant.>

## 2. Approved design decisions

<Literal, testable requirements. Tables for precise Pokémon, levels, moves, items, dialogue, event flags, locations, etc. only where approved. Distinguish "must" from recommendations.>

## 3. Authoritative sources and provenance

- Master Plan: <link, section, revision or snapshot date>
- Approved Pokédex / type audits / project rules: <links and relevant sections>
- Feature-specific decisions: <approval record links and dates>
- Source-code references: <verified paths and symbols, otherwise state "Codex to locate">

<Describe any discrepancies and their resolution. Do not substitute older, unrelated Pokémon canon.>

## 4. Implementation boundaries

**In scope:** <exact changed systems>  
**Out of scope:** <prohibited or deferred work>  
**Existing behavior to preserve:** <specific regression-sensitive behavior>

## 5. Technical implementation notes

<Confirmed source paths, data tables, event scripts, engine constraints, known dependencies. Mark proposals as hypotheses for Codex to verify; never invent paths or capabilities.>

## 6. Acceptance criteria and validation

- [ ] <Observable in-game requirement>
- [ ] <Deterministic data or event verification>
- [ ] <Regression check for unaffected behaviors>
- [ ] <Build/lint/static checks appropriate to this PR, observing AGENTS.md>
- [ ] <Explicit manual emulator test steps if needed>

## 7. Open questions and blockers

<None, or a checklist. If any blocker prevents the approved scope from being implemented faithfully, keep status Blocked.>

## 8. Codex delivery requirements

Implement only approved scope on a topic branch. Open a PR; list changed files, testing evidence and any tests not run. Do not merge automatically, alter baseline checks to conceal failures, introduce proprietary files, or improvise unspecified gameplay decisions.

## 9. One-line Codex handoff

`Implement implementation/<feature-slug>.md following AGENTS.md. Run appropriate checks and open a PR; report blockers rather than guessing.`
