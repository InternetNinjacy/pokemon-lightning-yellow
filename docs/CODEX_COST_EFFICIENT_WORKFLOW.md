# Codex Cloud: low-usage development workflow

Status: workflow guidance only. This document does not supersede approved gameplay specifications or by itself authorize new feature design. Check `AGENTS.md` and the current owner instructions for applicable merge authority.

## Goal

Spend Codex usage implementing and testing source changes, rather than repeatedly planning or auditing the entire project. Canonical tracking stays in `docs/project-tracking.json`, `docs/PROJECT_DASHBOARD.md`, and `docs/MASTER_IMPLEMENTATION_CHANGE_LEDGER.md`. Current implementation code, PRs and CI evidence override stale status prose.

## Startup: one narrow task

1. Read `AGENTS.md` and `docs/CODEX_EXECUTION_HANDOFF.md` (if present). Read only the relevant approved `implementation/*.md` and applicable source/test files.
2. Verify the active task still needs work by checking `main`, the relevant open PR and CI state. Do not repeat a full repository audit unless records conflict.
3. Select the highest-priority **approved and unblocked** small task with the fewest outstanding dependencies. Do not use a broad master prompt repeatedly.
4. Prefer an existing implementation branch or PR over duplicate source development. Use a narrowly scoped branch and PR if needed.

## Task sizing

- One task = one independently reviewable behavior or closely coupled group, with specific acceptance tests.
- For data-only changes, prefer a small set of exact approved trainer records, dialogue or fixtures.
- For engine/save/battle changes, keep the technical dependency boundary intact and test the whole affected mechanism.
- Do not spend a large coding session generating speculative designs, reviewing unrelated projects, or repeatedly regenerating dashboards.

## Validation

## Risk-based batch validation — owner directive 2026-11-12

This implements the **existing owner-approved November 7 TEST EQUIVALENCE POLICY** from the canonical Master Control succession handoff. It changes development/review process only. The currently enabled `Battle split validation` workflow still runs both matrix jobs on PRs and pushes; **this policy change does not remove, skip, disable, alter or re-label any CI job, assertion, mGBA group, checksum, GitHub status, protection or merge gate.** Branch-protection details could not be read through the GitHub integration (403), so requirements must be confirmed with a properly authorized maintainer *before* an independent workflow-optimization PR changes gate topology.

### Impact tiers and minimum local review evidence

| Tier | Scope | During PR development | Additional acceptance |
| --- | --- | --- | --- |
| 0 | Documentation, index, tracking metadata only | Link/readback, canonical references, accounting, lint where applicable | No local ROM build; obey automatic/required GitHub checks |
| 1 | Data-only, same previously verified native effect/path (trainer party, allowed item price, original machine parity) | Exact approved source delta/normalizers, focused host/source checks; minimal ROM smoke when typing, target, AI, animation or damage path differs | Reuse *unchanged* mechanism evidence with exact source SHA + workflow run; run every currently required check |
| 2 | New composition or changed type/flag/status/target interactions | Build, focused source/host and targeted native Singles/Doubles, changed conditional behavior | Cross-system test on genuinely changed route; required CI |
| 3 | Novel battle engine/effect, RNG, selected-target or save/event persistence, new assets | Build + deterministic native positive/negative, ABI/source guards, affected regressions | Full impacted integration and required clean/upstream checks |
| 4 | Map-system/progression, RTC, weather/lighting, rendering hooks, global save layout, releases | Full affected native map/asset/traversal and integration checks; release includes full suite/upstream | Real path/manual or hardware/playthrough evidence where contract demands; no waiver based on a compile |

When a PR combines tiers, use the highest tier and protect each affected contract. A PR must state **changed path, exact source/owner authority, equivalence precedent SHA/run, newly affected interactions, focused tests executed, CI status, tests not performed, and release/organic acceptance still pending**. Copy this short impact note into the PR description. Do not treat previous test success for a different changed head as exact-head success.

### Integration and CI economy

1. Prepare compatible, already approved changes on scoped branches; consolidate through a clearly reviewed integration PR and single combined-head validation where dependencies overlap. Do not double-merge original stacked/specialist PRs or silently absorb unrelated edits.
2. While iterating, run only relevant focused host/native tests. Avoid repeating long random-sample suites when the exact compiled behavior and pathways are already natively evidenced; do not assume type, immunity, weather, AI or animation are irrelevant.
3. **Do not poll running workflow jobs in a repeated conversation loop.** Record CI links once, progress independent approved work, then return only at a meaningful status or true failure. An automation may report terminal events if explicitly configured.
4. Every GitHub required status still must succeed; no manual green label, no permissive error handling, no `continue-on-error`, no disabled assertions, no green test stand-in or workflow-filter bypass. A completed PR run does not certify a subsequent merged-main SHA when merged-main CI is explicitly required.
5. Native Cut-tree removal and physical Route 8 grass arrival (WRL-002/009), other route access, real map events, save persistence, ABI, SOW visual/asset and link/hardware contracts remain distinct and cannot be inferred from data, build, or unrelated tests.
6. Full production integration/regression is performed for release candidates, tier 3/4 changes, and any currently mandated CI; isolated tier 0/1 work should not initiate additional manual full-suite executions **beyond** the repository workflows unless an explicit contract demands them.
7. **Separate workflow optimization PR required** to make full-suite native/upstream checks conditional for low-risk PRs. Before implementation, inspect actual required check names/rulesets with authorized access; choose a strategy that leaves required status names and security policies intact, retains full-suite validation for tier 3/4/release, compares changed paths against trusted base and fails closed on unknown impacts. Supply a measured run-time saving and green before/after evidence; rollback if protections or acceptance break. Until that PR is reviewed/merged, current automatic CI is authoritative.

### Merge and reporting discipline

One merge per proven dependency-safe step with SHA guard, owner-approved source, updated live main, and existing required status checks. Do not claim owner acceptance, task closure, gameplay completion or ROM release from a passing workflow alone. Keep the 50 task IDs and 461 earned / 251 remaining / 712 total unless separately verified acceptance warrants an accounting change. Report actual merges and blockers, not recurring 'still running' checks.

- Run focused tests while iterating; run the applicable full host/mGBA/CI gates before a production merge.
- Preserve baseline, ABI guards, save layouts, licensing and the no-ROM-artifact rule.
- A CI workflow can run without an agent manually inspecting each interim build; only invoke Codex again for failures that need judgment.
- Log exact commands, passed/failed/skipped outcomes, and current SHA. Never mistake a build for full gameplay acceptance.

## Completion and handoff

After a completed logical unit, commit, open/update the PR and integrate only when allowed by current owner authorization and required checks. Update tracking at milestones rather than every exploratory turn.

Write/update `docs/CODEX_EXECUTION_HANDOFF.md` at the end of each session. Keep it under roughly 100 lines and maintain:

- `main` commit SHA and date checked
- Last completed task and PR/commit link
- Work in progress, branch, source SHA, next exact action
- Tests already executed and the precise failing/remaining checks
- Next 3 approved unblocked tasks, ordered by dependency
- Human design approvals or other blockers (do not guess)
- Any uncommitted changes requiring preservation

Avoid copying the entire dashboard or lengthy specifications into this file. Link to their canonical paths.

## Reusable web Codex prompt

```text
Continue InternetNinjacy/pokemon-lightning-yellow. Read AGENTS.md,
docs/CODEX_COST_EFFICIENT_WORKFLOW.md and docs/CODEX_EXECUTION_HANDOFF.md
if present. Verify current main/PR state. Pick the next approved,
unblocked small implementation task, prioritizing dependency readiness.
Read only its exact spec and touched source/tests. Implement, run focused
checks, then all required merge checks. Commit and integrate only under
the standing owner authorization and existing branch protections.
Update the short handoff with exact SHA, PR, test evidence and next task.
Do not re-audit the full project unless records conflict. Do not invent
gameplay data. Keep going to the next independent ready task if capacity
remains.
```

## Cloud environment efficiency

Use one published reusable Codex Cloud environment with the repository's approved toolchain and the documented mGBA test dependencies. Set it up once rather than rebuilding setup instructions in every prompt. Each new cloud task is isolated; committed Git state and a checked-in handoff are the durable cross-task memory. Do not put secrets, game ROM binaries or build artifacts into the environment configuration.

For routine small tasks use the lowest-cost model that reliably passes required checks; escalate to stronger reasoning for source migration, save compatibility, battle engine, integration conflicts or repeated failures. Track actual usage before purchasing credits or upgrading a plan.
