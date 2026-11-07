# Full validation events — INT-001-CI3

Each opened/reopened/updated PR (including drafts) runs the full modified and
upstream-baseline matrix. Main and setup/** pushes and workflow_dispatch remain.
A feature push with an open PR therefore validates through the PR update once,
rather than starting an additional identical workflow for the same source head.
No host/native/build/clean/checksum/ABI/assertion/frame or instruction check changes.
Job names, permissions, concurrency and branch protections remain unchanged.

For a branch without a PR, open a draft PR early, or use Actions → Battle split
validation → Run workflow → select that branch. Feature pushes alone no longer
start this workflow. Setup auto-validation is retained for environment onboarding.
All implementation merges still require successful applicable PR checks; this
policy does not allow skipping failed tests or substituting static inspection.

Observed example: headf05c37f2 runs42379743632/42379748023 both passed the same code.
Their modified jobs took19m08/24m01, baseline jobs2m45/2m44: about49 runner-minutes
of job time for one head. One PR workflow retains both jobs and their complete
coverage. This describes measured runtime, not a promise about billing or costs.

Validation of this configuration: parse before/after YAML and compare every key
except the push branch list; job definitions must be byte-semantically equal.
Existing registration checker still requires every registered host/native suite
and clean/upstream steps. No repository security or protection setting is edited.
