#!/usr/bin/env python3
"""Build a PRIVATE owner-playtested ROM from the approved Pichu/Sue + follower sources.

No gameplay tests, no auto-publishing, no ROM or source artwork committed to Git.
This script must run in an isolated, permission-cleared, GBA build-capable checkout.
"""
import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PR256_ANCHOR = "1ea7c53c3ec0044e2343a96611c95a6dec07e4c7"
ROM_NAME = "pokefirered.gba"


def command(*args):
    return subprocess.check_output(args, cwd=ROOT, text=True).strip()


def sha256_file(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--attribution", type=Path, required=True,
                        help="Existing local artist/source permissions-and-credits file")
    parser.add_argument("--output-dir", type=Path, required=True,
                        help="Private output folder outside the Git working tree")
    parser.add_argument("--offline", action="store_true",
                        help="Use only already cached, pinned and permission-cleared art")
    args = parser.parse_args()

    # Do not mislabel an ordinary or obsolete game build as Playtest004.
    # The exact approved Pichu/Sue commit must be an ancestor of this checkout.
    subprocess.run(["git", "merge-base", "--is-ancestor", PR256_ANCHOR, "HEAD"],
                   cwd=ROOT, check=True)
    source = command("git", "rev-parse", "HEAD")
    if not (ROOT / "docs/followers/expected-source-blobs.json").is_file():
        parser.error("252-follower pinned asset inventory missing from this checkout")
    if not (ROOT / "docs/playtest/2026-11-15-playtest-004-staging.json").is_file():
        parser.error("Playtest004 Pichu/Sue staging record missing")

    notice = args.attribution.resolve()
    if not notice.is_file() or len(notice.read_text(encoding="utf-8").strip()) < 40:
        parser.error("a complete local artist permission/attribution record is required")
    devkitarm = os.environ.get("DEVKITARM")
    tool_dirs = [str(Path(devkitarm) / "bin")] if devkitarm else []
    tool_path = os.pathsep.join(tool_dirs + [os.environ.get("PATH", "")])
    missing = [name for name in ("make", "arm-none-eabi-as", "arm-none-eabi-ld",
                                "arm-none-eabi-objcopy") if shutil.which(name, path=tool_path) is None]
    if missing:
        parser.error("missing required native GBA tools: " + ", ".join(missing))

    output = args.output_dir.resolve()
    if os.path.commonpath([str(ROOT), str(output)]) == str(ROOT):
        parser.error("private game output must be OUTSIDE the repository")
    output.mkdir(parents=True, exist_ok=True)
    target = output / "Playtest004.gba"
    manifest_file = output / "Playtest004.build.json"
    if target.exists() or manifest_file.exists():
        parser.error("refusing to overwrite an existing owner's Playtest004 ROM/manifest")

    env = os.environ.copy()
    if tool_dirs:
        env["PATH"] = tool_path
    env["FOLLOWER_ART_PERMISSION_CONFIRMED"] = "1"
    env["FOLLOWER_ATTRIBUTION_FILE"] = str(notice)
    if args.offline:
        env["FOLLOWER_CACHE_ONLY"] = "1"

    started = time.time()
    print("Building private Playtest004 from source commit", source, flush=True)
    subprocess.run(["make", "private-followers-rom"], cwd=ROOT, env=env, check=True)

    rom = ROOT / ROM_NAME
    generated = ROOT / "src/follower_assets_generated.inc"
    art_meta = ROOT / "docs/followers/local-build-metadata.json"
    if not rom.is_file() or not generated.is_file() or not art_meta.is_file():
        raise RuntimeError("ROM, generated follower artwork registry or local metadata absent")
    if rom.stat().st_mtime < started - 2:
        raise RuntimeError("ROM timestamp predates private build; refusing to ship a stale binary")
    meta = json.loads(art_meta.read_text(encoding="utf-8"))
    registry_digest = sha256_file(generated)
    if meta.get("generated_sha256") != registry_digest or meta.get("graphics_records") != 255:
        raise RuntimeError("generated follower registry does not match the build input metadata")
    if meta.get("source_file_count") != 765 or "sFollowerArtCount = 0" in generated.read_text():
        raise RuntimeError("private ROM appears to use the zero-art fallback")
    if not (2 * 1024 * 1024 <= rom.stat().st_size <= 32 * 1024 * 1024):
        raise RuntimeError("compiled game is not a plausible native GBA ROM size")

    # Keep the playable binary and its evidence outside Git. The owner alone
    # performs gameplay testing; a checksum is not a visibility/movement test.
    try:
        shutil.copyfile(rom, target)
        details = {
            "build": "Playtest004",
            "source_head": source,
            "source_includes_pichu_sue_pr256_head": PR256_ANCHOR,
            "artwork_source_tree": meta.get("upstream_git_tree"),
            "artwork_source_count": meta["source_file_count"],
            "follower_graphics_descriptors": meta["graphics_records"],
            "follower_registry_sha256": registry_digest,
            "rom_file": target.name,
            "rom_bytes": target.stat().st_size,
            "rom_sha256": sha256_file(target),
            "gameplay_tested": False,
            "private_owner_playtest_only": True,
            "rights_note": "Art reuse permission/attribution supplied locally; not independently certified by this build",
        }
        manifest_file.write_text(json.dumps(details, indent=2) + "\n", encoding="utf-8")
    except Exception:
        target.unlink(missing_ok=True)
        manifest_file.unlink(missing_ok=True)
        raise

    print("Private Playtest004 ROM created at", target)
    print("ROM SHA-256:", details["rom_sha256"])
    print("Owner gameplay playtesting has NOT been performed.")
    print("Never commit or publish ROM/art/source cache through GitHub.")


if __name__ == "__main__":
    try:
        main()
    except subprocess.CalledProcessError as exc:
        print("Private build stopped:", exc, file=sys.stderr)
        sys.exit(1)
