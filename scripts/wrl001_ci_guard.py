#!/usr/bin/env python3
"""Fail-closed launcher for one reviewed WRL-001 source commit only."""
import argparse
import ctypes
import hashlib
import os
from pathlib import Path
import subprocess


SOURCE_SHA = "0f693dd7bef4219e161b299f08a9a225ecdac9d6"
PINNED_FILES = {
    "tests/emulator/runner.c": "64c6f6cd5910bea53dba13d7dc6a591e77449888c9d5a3c323a4903dfe7dd897",
    "tests/emulator/run.py": "0d4ee5225f3507efb14314035ea4ab06c77a251695c359fabb78f215f655d9ff",
    "tests/emulator/wrl001_identity.c": "a7ee187fe56af7fdd93c4002e4b11c4297b92cbbb13f50a5f59e4142ba8f9e82",
    "scripts/prepare-baseline.sh": "b4ec70861f848da2368a6a177ddafa43c2d2860faa5408b03d7ec141858daf0f",
}
SELECTOR = ('if(getenv("LIGHTNING_WRL001_ONLY")){wrl001Tests();'
            'printf("PASS: %u focused WRL-001 assertions\\n",checks);'
            'core->deinit(core);free(pixels);free(bootState);return 0;}')
PASS_LINE = "PASS: 681 focused WRL-001 assertions"


def verify_source(source):
    actual = subprocess.check_output(
        ["git", "rev-parse", "HEAD"], cwd=source, text=True).strip()
    if actual != SOURCE_SHA:
        raise SystemExit(f"WRL-001 source SHA mismatch: {actual}")
    subprocess.run(["git", "diff", "--exit-code", "HEAD", "--"],
                   cwd=source, check=True)
    for name, expected in PINNED_FILES.items():
        path = source / name
        if not path.is_file() or hashlib.sha256(path.read_bytes()).hexdigest() != expected:
            raise SystemExit(f"WRL-001 reviewed input missing or changed: {name}")
    runner = (source / "tests/emulator/runner.c").read_text()
    if runner.count(SELECTOR) != 1:
        raise SystemExit("WRL-001 focused selector unavailable; refusing native execution")
    print(f"WRL001_SUPPLEMENTAL_SOURCE_SHA={actual}", flush=True)


def native_environment(prefix):
    # The title-capture selector precedes WRL-001; erase every inherited selector,
    # capture destination and alternate mGBA path before setting this one path.
    env = {k: v for k, v in os.environ.items()
           if not k.startswith("LIGHTNING_") and k != "MGBA_PREFIX"}
    env.update(LIGHTNING_WRL001_ONLY="1", MGBA_PREFIX=str(prefix),
               PYTHONDONTWRITEBYTECODE="1", TZ="UTC")
    return env


def run_native(source, prefix):
    # Repeat the preflight immediately before execution, after the game build.
    verify_source(source)
    library = prefix / "usr/lib/x86_64-linux-gnu/libmgba.so"
    if not library.is_file():
        raise SystemExit("Pinned mGBA library unavailable; refusing system fallback")
    version = ctypes.c_char_p.in_dll(ctypes.CDLL(str(library)), "projectVersion").value
    if version != b"0.10.5":
        raise SystemExit(f"Expected mGBA 0.10.5, received {version!r}")
    print("WRL001_SUPPLEMENTAL_MGBA=0.10.5", flush=True)
    passed = False
    with subprocess.Popen(["python3", "tests/emulator/run.py"], cwd=source,
                          env=native_environment(prefix), stdout=subprocess.PIPE,
                          stderr=subprocess.STDOUT, text=True) as proc:
        for line in proc.stdout:
            print(line, end="", flush=True)
            passed |= line.strip() == PASS_LINE
        status = proc.wait()
    if status:
        raise SystemExit(status)
    if not passed:
        raise SystemExit("WRL-001 focused completion marker absent; supplemental run failed")
    print("Supplemental WRL-001 evidence only; required CI is not replaced.", flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--phase", choices=("preflight", "native"), required=True)
    parser.add_argument("--mgba-prefix", type=Path)
    args = parser.parse_args()
    source = args.source.resolve(strict=True)
    if args.phase == "preflight":
        verify_source(source)
    else:
        if args.mgba_prefix is None:
            parser.error("--mgba-prefix is required for native execution")
        run_native(source, args.mgba_prefix.resolve(strict=True))


if __name__ == "__main__":
    main()
