"""Launcher safety checks only: no game compiler, ROM or emulator execution."""
import contextlib
import hashlib
import importlib.util
import io
from pathlib import Path
import subprocess
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch, MagicMock

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("wrl001_ci_guard", ROOT / "scripts/wrl001_ci_guard.py")
guard = importlib.util.module_from_spec(spec)
spec.loader.exec_module(guard)


class Wrl001GuardTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.source = Path(self.temp.name)
        for name in guard.PINNED_FILES:
            path = self.source / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(subprocess.check_output(
                ["git", "show", f"{guard.SOURCE_SHA}:{name}"], cwd=ROOT))
        self.library = self.source / "usr/lib/x86_64-linux-gnu/libmgba.so"
        self.library.parent.mkdir(parents=True)
        self.library.touch()
        # Metadata and the native process are mocked; reviewed file bytes are real.
        self.head = self.enterContext(patch.object(guard.subprocess, "check_output", return_value=guard.SOURCE_SHA))
        self.diff = self.enterContext(patch.object(guard.subprocess, "run"))
        self.native = self.enterContext(patch.object(guard.subprocess, "Popen"))
        self.enterContext(contextlib.redirect_stdout(io.StringIO()))

    def reject_before_native(self):
        with self.assertRaises((SystemExit, subprocess.CalledProcessError)):
            guard.run_native(self.source, self.source)
        self.native.assert_not_called()

    def test_reviewed_source_preflight(self):
        guard.verify_source(self.source)
        self.native.assert_not_called()

    def test_wrong_source_sha(self):
        self.head.return_value = "02240690581eb70d70219024fb7f55da2882308b"
        self.reject_before_native()

    def test_dirty_tracked_source(self):
        self.diff.side_effect = subprocess.CalledProcessError(1, ["git", "diff"])
        self.reject_before_native()

    def test_missing_selector_file(self):
        (self.source / "tests/emulator/runner.c").unlink()
        self.reject_before_native()

    def test_changed_python_launcher(self):
        (self.source / "tests/emulator/run.py").write_text("raise SystemExit(0)\n")
        self.reject_before_native()

    def test_selector_must_return_without_full_suite_fallback(self):
        name = "tests/emulator/runner.c"
        path = self.source / name
        path.write_text(path.read_text().replace(guard.SELECTOR, 'if(0){wrl001Tests();}'))
        # Isolate the independent selector guard after the digest guard.
        with patch.dict(guard.PINNED_FILES, {name: hashlib.sha256(path.read_bytes()).hexdigest()}):
            self.reject_before_native()

    def test_missing_pinned_mgba_has_no_system_fallback(self):
        self.library.unlink()
        self.reject_before_native()

    def version(self, value):
        return SimpleNamespace(CDLL=lambda _: None, c_char_p=SimpleNamespace(
            in_dll=lambda *_: SimpleNamespace(value=value)))

    def test_wrong_mgba_version(self):
        with patch.object(guard, "ctypes", self.version(b"0.10.2")):
            self.reject_before_native()

    def result(self, lines, status=0):
        proc = MagicMock()
        proc.stdout = iter(lines)
        proc.wait.return_value = status
        self.native.return_value.__enter__.return_value = proc

    def test_zero_exit_without_focused_completion_fails(self):
        self.result(["PASS: unrelated suite\n"])
        with patch.object(guard, "ctypes", self.version(b"0.10.5")):
            with self.assertRaises(SystemExit):
                guard.run_native(self.source, self.source)

    def test_nonzero_exit_even_with_completion_fails(self):
        self.result([guard.PASS_LINE + "\n"], status=1)
        with patch.object(guard, "ctypes", self.version(b"0.10.5")):
            with self.assertRaises(SystemExit):
                guard.run_native(self.source, self.source)

    def test_native_command_and_environment_are_focused(self):
        self.result([guard.PASS_LINE + "\n"])
        with patch.object(guard, "ctypes", self.version(b"0.10.5")), patch.dict(
                guard.os.environ, {"LIGHTNING_TITLE_CAPTURE": "/tmp/forbidden",
                                   "LIGHTNING_BALL_WEATHER_ONLY": "1", "MGBA_PREFIX": "/bad"}):
            guard.run_native(self.source, self.source)
        args, kwargs = self.native.call_args
        self.assertEqual(args[0], ["python3", "tests/emulator/run.py"])
        env = kwargs["env"]
        self.assertEqual({k: v for k, v in env.items() if k.startswith("LIGHTNING_")},
                         {"LIGHTNING_WRL001_ONLY": "1"})
        self.assertEqual(env["MGBA_PREFIX"], str(self.source))


if __name__ == "__main__":
    unittest.main()
