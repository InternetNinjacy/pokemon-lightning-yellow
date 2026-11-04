# Approved feature handoffs for Codex

This folder holds implementation-ready specifications derived from approved Pokémon Lightning Yellow design decisions. **A conversation or a Google Drive draft is not itself implementation authorization.** A feature is ready only when its handoff states its approval and identifies authoritative sources and exact scope.

## Workflow

1. Collaboratively design a feature; reconcile the Master Plan, approved project records, and any existing code. Record unresolved questions rather than inventing details.
2. Once the user explicitly approves the feature, prepare or update `implementation/<feature-slug>.md` using `TEMPLATE.md`. Keep the written spec in the repository so Codex can read it without Google Drive access.
3. Mark `Status: Ready for implementation` only if all gameplay-changing decisions needed for the identified scope are approved, dependencies are clear, and acceptance criteria are measurable. Otherwise mark `Blocked / design incomplete` and do not instruct Codex to implement that scope.
4. Publish the handoff on a topic branch and open a reviewable PR. Do not write directly to `main`. The user reviews and merges the specification before treating its default-branch version as the implementation contract.
5. Give the user a one-line Codex instruction referencing the **merged exact path** (or a deliberately named branch, when appropriate).
6. Codex implements on a separate topic branch and opens a PR, linking the handoff and reporting changed files, validations, test limits and remaining questions. No automatic merging.
7. Review implementation against the approved spec and project-specific checks. Amend the spec through review if requirements change.

## Authority and conflict handling

- Existing `AGENTS.md` safety, baseline and source restrictions apply to every task.
- Approved project records and the currently reviewed handoff govern feature design; the source repository governs actual code structure. A mismatch is a blocker for human review, not permission for Codex to improvise.
- Keep a dated source ledger linking the authoritative Master Plan, species list, relevant audits, and precise approval records. Google Drive links are provenance references, **not** required Codex runtime dependencies.
- Never import content from another Pokémon project.
- An approval for one feature does not authorize adjacent gameplay changes.
- Specifications are instructions, not evidence that code is implemented. Label planned, approved, implemented and verified separately.
- Baseline checksum validation was designed for unchanged FireRed. An approved gameplay PR must propose the appropriate new validation; never silently weaken or redefine an existing passing gate.

## Suggested Codex prompt

> Implement `implementation/<feature-slug>.md` from the repository's default branch. Follow `AGENTS.md`, stay within the approved scope, run the applicable checks, and open a pull request. Report any blocker without inventing missing design decisions.

If the handoff is not merged, explicitly cite the reviewed branch/ref and do not assume Codex can access it.
