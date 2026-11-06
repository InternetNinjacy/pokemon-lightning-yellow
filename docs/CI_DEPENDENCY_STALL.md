# CI dependency-installation stall — INT-001-CI

Observed run42233960801 / PR62, head36f6d89b. Modified job ran14:26:42–14:57:00 UTC
November6 and was cancelled at30m18s. The `Install headless mGBA core` package step
ran14:32:22–14:56:57 (24m35s); emulator tests were skipped, not failed. Upstream
baseline succeeded. The available step metadata establishes an installation stall;
it does not identify a particular mirror/lock root cause or a game regression.

Move libmgba-dev into initial dependencies for modified jobs only, before expensive
clean builds. Bound APT transport waits at30 seconds with3 retries and package-lock
waits at120 seconds. Keep the later named mGBA step as dpkg verification, without
starting another late network transaction. Both matrix jobs, every host/native
registration, two clean builds, upstream comparison, assertions/API guards and
20-minute native limit remain unchanged. The overall job limit is45 minutes;
this accommodates measured network transfer time without relaxing a test guard. No security/protection or
automatic-merge settings change. APT failure still fails the job; no cached fake
library or skipped test path is introduced.

Validation: registration checker and workflow diff audit; actual new CI run must
succeed before merging this maintenance fix. This is a reliability change, not a
claim that every future mirror/runner stall is prevented. Roll back workflow-only
change if needed; game/save/ROM data unaffected.

Follow-up evidence from actual job128215173523 / run42311920113: APT fetched
62.7 MB in19min7s (54.7 kB/s), including libmgba codec dependencies. Its live log
proves slow continuing downloads rather than an idle APT lock; per-request idle
timeouts cannot bound a transfer that continues making progress. The build then
started. Increase the overall job envelope30→45 minutes so clean builds and the
unchanged20-minute native gate can finish after this measured installation cost.
No particular permanent mirror fault is inferred; new exact-head CI is required.
