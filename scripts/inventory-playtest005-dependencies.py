#!/usr/bin/env python3
"""Read-only owner-side Playtest005 dependency inventory; does not build or download.

No private bytes are printed or copied. Exit nonzero if any prerequisite is
missing or mismatched. A successful inventory is NOT a license or gameplay PASS.
"""
import argparse
import hashlib
import json
import os
import shutil
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PINNED_ART = "cbe04a7a2ef645bee19d159876da074154ccf1f2"
PINNED_AGBCC = "da598c1d918402c42c0c0d7128ba14567f3175e9"


def blob_sha(data):
    return hashlib.sha1(b"blob " + str(len(data)).encode() + b"\0" + data).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, default=Path(os.environ.get("FOLLOWER_CACHE", "/tmp/lightning-follower-assets")))
    parser.add_argument("--attribution", type=Path, help="Local permission/credit record; never print its contents")
    parser.add_argument("--agbcc-source", type=Path, help="Optional local pret/agbcc Git source checkout for exact commit verification")
    args = parser.parse_args()
    failures = []
    manifest = json.loads((ROOT / "docs/followers/expected-source-blobs.json").read_text(encoding="utf-8"))
    entries = manifest.get("entries", [])
    if (manifest.get("pinned_commit") != PINNED_ART or manifest.get("tree_sha") != PINNED_ART
        or manifest.get("tree_truncated") is not False or manifest.get("entries_count") != 765
        or len(entries) != 765 or len({e.get("path") for e in entries}) != 765):
        failures.append("pinned artwork manifest is incomplete or unexpected")
    missing, mismatched, invalid = 0, 0, 0
    cache = args.cache.resolve()
    for entry in entries:
        rel = entry.get("path")
        if (not isinstance(rel, str) or not rel.startswith("graphics/pokemon/")
            or Path(rel).is_absolute() or ".." in Path(rel).parts):
            invalid += 1
            continue
        asset = cache / rel
        # Reject symlinked files/directories escaping the private cache root.
        if cache not in asset.resolve().parents:
            invalid += 1
            continue
        if not asset.is_file():
            missing += 1
            continue
        data = asset.read_bytes()
        if len(data) != entry.get("bytes") or blob_sha(data) != entry.get("git_blob_sha1"):
            mismatched += 1
    matched = len(entries) - missing - mismatched - invalid
    print(f"Pinned art cache: {matched}/{len(entries)} files hash-matched; missing={missing}; mismatched={mismatched}; invalid={invalid}")
    if missing or mismatched or invalid:
        failures.append("private artwork cache incomplete or mismatched")
    if not args.attribution or not args.attribution.is_file() or len(args.attribution.read_text(encoding="utf-8").strip()) < 40:
        failures.append("local permission/attribution record not supplied or incomplete")
    devkitarm = os.environ.get("DEVKITARM")
    path = os.pathsep.join(([str(Path(devkitarm) / "bin")] if devkitarm else []) + [os.environ.get("PATH", "")])
    tools = ("make", "arm-none-eabi-as", "arm-none-eabi-ld", "arm-none-eabi-objcopy")
    missing_tools = [name for name in tools if shutil.which(name, path=path) is None]
    if missing_tools:
        failures.append("missing host tools: " + ", ".join(missing_tools))
    compiler = ROOT / "tools/agbcc/bin/agbcc"
    if not compiler.is_file() or not os.access(compiler, os.X_OK):
        failures.append("installed legacy agbcc executable absent")
    if args.agbcc_source:
        try:
            revision = subprocess.check_output(
                ["git", "-C", str(args.agbcc_source), "rev-parse", "HEAD"],
                text=True, stderr=subprocess.DEVNULL).strip()
        except (OSError, subprocess.CalledProcessError):
            revision = ""
        if revision != PINNED_AGBCC:
            failures.append("agbcc source checkout does not match exact pinned revision")
        else:
            print("agbcc source checkout: exact pinned revision")
    else:
        print("agbcc source revision: not independently verified (supply --agbcc-source)")
    if failures:
        for issue in failures:
            print("BLOCKER:", issue)
        raise SystemExit(1)
    print("Dependency files present and hash-checked. Compiler provenance, rights and runtime still require independent review.")


if __name__ == "__main__":
    main()
