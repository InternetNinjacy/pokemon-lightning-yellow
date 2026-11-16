#!/usr/bin/env python3
"""Fail closed before a *private* follower ROM is compiled.

This is a build-input check, not gameplay testing or license certification.
The contributor's actual permission scope still has to be reviewed separately.
"""
import argparse
import csv
import hashlib
import json
import re
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--registry", type=Path, default=Path("src/follower_assets_generated.inc"))
    parser.add_argument("--metadata", type=Path, default=Path("docs/followers/local-build-metadata.json"))
    parser.add_argument("--attribution", type=Path, required=True,
                        help="Local nonempty source/artist attribution and permission-scope record; never committed")
    args = parser.parse_args()

    if not args.attribution.is_file():
        parser.error("local attribution/permission record missing; refusing follower ROM build")
    notice = args.attribution.read_text(encoding="utf-8").strip()
    if len(notice) < 40:
        parser.error("local attribution/permission record is empty or incomplete")

    if not args.registry.is_file() or not args.metadata.is_file():
        parser.error("generated follower registry or metadata missing")
    generated = args.registry.read_bytes()
    metadata = json.loads(args.metadata.read_text(encoding="utf-8"))
    if metadata.get("base_species_count") != 252 or metadata.get("castform_form_count") != 3:
        parser.error("generated follower roster is not exactly 252 base species + 3 Castform forms")
    if metadata.get("graphics_records") != 255:
        parser.error("generated follower registry missing one or more assets")
    if metadata.get("generated_sha256") != hashlib.sha256(generated).hexdigest():
        parser.error("generated follower registry differs from its pinned metadata")
    source = generated.decode("utf-8")
    if "sFollowerArtCount = 0" in source:
        parser.error("zero-art fallback cannot be used in a follower-enabled build")
    if "static const struct FollowerArtEntry sFollowerArtTable[]" not in source:
        parser.error("follower table was not generated")
    records = re.findall(r"^\s*\{SPECIES_([A-Z0-9_]+),\s*([0-3]),\s*(32|64),",
                         source, flags=re.MULTILINE)
    if len(records) != 255:
        parser.error("expected 255 generated base/form graphics descriptors, got %d" % len(records))
    bases = [(species, form, int(size)) for species, form, size in records if form == "0"]
    with Path("docs/followers/252-species-coverage.csv").open(newline="", encoding="utf-8") as stream:
        roster = [row["species"] for row in csv.DictReader(stream)]
    if len(roster) != 252 or [species for species, _, _ in bases] != roster:
        parser.error("generated base species do not match the exact approved 252 roster/order")
    if any(size != (64 if species == "STEELIX" else 32) for species, _, size in bases):
        parser.error("one or more sprite frame dimensions differ from approved native art sizes")
    if [(species, form, size) for species, form, size in records if form != "0"] != [
        ("CASTFORM", "1", "32"), ("CASTFORM", "2", "32"), ("CASTFORM", "3", "32")]:
        parser.error("unexpected Castform form descriptors or frame dimensions")
    print("BUILD INPUT READY: 252 species plus three Castform forms are registered.")
    print("Source attribution/permission record was supplied locally.")
    print("Not a runtime follower PASS or an independent grant/rights certification.")


if __name__ == "__main__":
    main()
