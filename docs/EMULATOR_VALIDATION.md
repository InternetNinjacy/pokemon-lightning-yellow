# Automated mGBA ROM integration tests

The modified Actions job installs Ubuntu's `libmgba-dev` and runs
`python3 tests/emulator/run.py` against the freshly built `pokefirered.gba` and
its matching ELF. No GUI, local desktop tools, proprietary BIOS, downloaded
reference ROM, persistent save fixture or ROM artifact upload is needed.
The runner uses mGBA's open-source BIOS implementation. Dependency versions are
printed in the log; the cloud development machine uses mGBA 0.10.5 and the Ubuntu
24.04 runner supplies its packaged 0.10.x version.

## What actually executes

This is distinct from the host C regression tests. The game boots for 600 emulated
frames. The runner obtains ROM function/global addresses from the ELF and ABI
constants from the game's own ARM compiler. It then calls compiled Thumb routines
in the actual ROM and executes the normal scene callbacks, IRQs, graphics, sound,
text and task loops for animation tests. No production evolution, Pokémon crypto,
RTC, bag or scene functions are replaced by host stubs.

Controlled fixtures initialize save-block pointers, bag pockets and a nonzero bag
encryption key using game data/functions. Pokémon are constructed with CreateMon
and modified/read with the game's encrypted-data accessors. National Dex is enabled
except in its explicitly blocked test. The title-screen tasks are cleared before
starting a scene, and each scene restores the same in-memory boot snapshot. These
are isolated integration fixtures, not a claim of a complete new-game playthrough.

Routine injection preserves the paused CPU's registers, flags, prefetch and banked
state, and uses guarded unused EWRAM for a short test stack/return marker. ELF checks
fail if future globals occupy that space; runtime checks fail if the test stack
would collide. No game heap is reserved for the harness, so evolution graphics keep
their normal memory budget. Input uses emulated A/B button presses. The test-only
return destination is a `bx lr` marker in unused RAM, not a game logic replacement.

The suite checks:

- ROM boot and RTC autodetection from the compiled signature.
- Real ROM GPIO reads of all 24 hours against mGBA's fixed 24-hour RTC (UTC fixtures).
- Eevee friendship 219/220 and both stone alternatives with real Pokémon data.
- Absent emulated RTC: no invented timed evolution; stone fallback remains valid.
- All eight held-item trade targets, removal of the traded held item, wrong item
  rejection and Everstone behavior through actual target lookup.
- All four level-40 alternatives, level-39 rejection and retained trade lookup.
- **21 actual scene scenarios:** success and B cancellation for each of the eight
  trade-item combinations and both new Eevee stones (20), plus one automatic
  National-Dex cancellation. Species must match the outcome; a bag count of three
  must remain three on cancellation/blocking and become two only on success.
- Post-evolution move-learning prompts encountered by the fixtures are handled via
  normal buttons by declining replacement and confirming stop-learning. Evolution
  and item consumption must still finish correctly.

Every failed expectation, ROM assertion/illegal instruction, routine timeout or
scene timeout returns a nonzero process status and fails Actions. Raw emulator
assertion counts are printed, but scene coverage above is the meaningful measure.

## Defect found and corrected

The initial emulator run booted and detected RTC but rejected the original driver's
LSB-first command bytes. Stable mGBA 0.10.x expects the S-3511 command bytes MSB first,
with response bytes LSB first. Correcting the command loop made actual ROM clock
reads pass. The host serial test had shared the original assumption; its expected
wire bytes were corrected too. This demonstrates why those host tests were not
sufficient to claim emulator compatibility. The correction is also in RTC PR #3.

During harness development, scene trials also exposed fixture issues: title-menu
input tasks still active during transition, excessive reserved heap space, and an
unanswered move-learning dialog. Those were fixed in the harness, not by changing
game evolution/animation logic. Failed development trials are not counted as passes.

## Remaining acceptance work

These tests do **not** navigate the Bag selection/highlighting UI from an overworld
save, visually approve category labels/animations, perform a full new-game
playthrough, emulate a second link peer, or test real RTC cartridge timing. They
exercise trade target/held-item logic, not the link-trade network handshake. The
physical/special damage suite remains host-level; this evolution-focused emulator
suite does not establish full live-battle or AI scoring coverage. The real-emulator
RTC tests use its default 24-hour mode; 12-hour decoding is covered by host tests,
not a physical device or a 12-hour emulator fixture.

ROMs, save states, snapshots and generated runner binaries are not committed or
uploaded as artifacts. The bootstrap snapshot exists only in the runner's memory;
helper build files are temporary. Read the PR/Actions logs for the results of the
current revision. No merge or auto-merge is authorized by passing these checks.
