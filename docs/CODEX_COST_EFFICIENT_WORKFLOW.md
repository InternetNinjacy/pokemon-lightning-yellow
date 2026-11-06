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
