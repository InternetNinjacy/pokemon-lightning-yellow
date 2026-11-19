#!/usr/bin/env python3
"""Build pinned, native-GBA 252-species follower registry in the LOCAL checkout.

Source code only is committed. Generated artwork and ROM files must never be
committed, uploaded, or included in GitHub Actions artifacts. Record the
specific source grant/attribution before invoking --confirmed-scope.
"""
import argparse
import csv
import hashlib
import importlib.util
import json
import os
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("follower_verifier", ROOT / "scripts/verify-follower-assets.py")
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)

def words(data, n=24):
    return "\n".join("    " + ", ".join("0x%02X" % b for b in data[i:i+n]) + "," for i in range(0, len(data), n))

def palette_words(rgb):
    return ", ".join("0x%04X" % ((r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10)) for r,g,b in rgb)

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--confirmed-scope", action="store_true",
                        help="The owner confirmed that selected pinned art may be converted locally for this noncommercial project; verify contributor permissions independently")
    parser.add_argument("--output", type=Path, default=ROOT/"src/follower_assets_private.inc")
    parser.add_argument("--report", type=Path, default=ROOT/"docs/followers/local-build-metadata.json")
    args = parser.parse_args()
    if not args.confirmed_scope:
        parser.error("art import requires an explicit --confirmed-scope invocation")
    with (ROOT/"docs/followers/252-species-coverage.csv").open(newline="") as f:
        rows = list(csv.DictReader(f))
    if len(rows) != 252 or rows[232]["species"] != "CASTFORM":
        raise ValueError("wrong approved roster; refusing sprite build")
    pins_path = ROOT / "docs/followers/expected-source-blobs.json"
    pins = json.loads(pins_path.read_text(encoding="utf-8"))
    if (pins.get("pinned_commit") != audit.REV or pins.get("tree_sha") != audit.REV
        or pins.get("tree_truncated") is not False or pins.get("entries_count") != 765):
        raise ValueError("full artwork source-blob manifest does not match pinned revision")
    expected_entries = pins["entries"]
    expected_map = {item["path"]: item for item in expected_entries}
    if len(expected_map) != 765:
        raise ValueError("duplicate paths in pinned source-blob manifest")
    records = []
    input_files = []

    def get_source(path, inventory_blob=None):
        pin = expected_map.get(path)
        if pin is None:
            raise ValueError("source asset is not in pinned Git tree manifest: " + path)
        expected_blob = pin["git_blob_sha1"]
        if inventory_blob is not None and inventory_blob != expected_blob:
            raise ValueError("existing 252-species PNG pin conflicts with upstream tree: " + path)
        raw = audit.fetch(path)
        actual_blob = audit.git_blob_sha(raw)
        if actual_blob != expected_blob or len(raw) != pin["bytes"]:
            raise ValueError("source bytes do not match the independently pinned upstream Git tree: " + path)
        input_files.append({
            "path": path,
            "git_blob_sha1": actual_blob,
            "sha256": hashlib.sha256(raw).hexdigest(),
            "bytes": len(raw),
            "pinned_blob_verified": True,
        })
        return raw

    generated = ["// GENERATED LOCALLY; do not commit this file with source-art bytes.",
                 "// Pinned source: " + audit.BASE,
                 "// Frames: south0/1 north0/1 west0/1; east mirrors west.",
                 "// NORMAL and SHINY share the same pixel index table."]
    for index, row in enumerate(rows, 1):
        name = row["species"]
        species = "SPECIES_" + name
        asset_name = "sFollowerArt_%03d" % index
        png = get_source(row["kasenn_sheet"], row["kasenn_blob_sha"])
        details = audit.read_png(png, include_gba_frames=True)
        size = details["frame_cell_size"][0]
        if size not in (32,64):
            raise ValueError("unhandled sprite size for " + name)
        if size == 64 and name != "STEELIX":
            raise ValueError("new oversized species needs native allocation review: " + name)
        if any(details["corner_indices"]):
            raise ValueError("nonzero transparent-background corner: " + name)
        palettes = {}
        for variant in ("normal", "shiny"):
            palettes[variant] = audit.read_pal(get_source(row["kasenn_" + variant + "_palette"]))
        blob = b"".join(details["gba_frames"])
        if len(blob) != size*size*3:
            raise ValueError("bad six-frame size: "+name)
        generated += [
            "static const u8 %s_pixels[%d] __attribute__((aligned(4))) = {\n%s\n};" %
                (asset_name, len(blob), words(blob)),
            "static const u16 %s_normal[16] = {%s};" % (asset_name, palette_words(palettes["normal"])),
            "static const u16 %s_shiny[16] = {%s};" % (asset_name, palette_words(palettes["shiny"])),
        ]
        records.append((species,0,size,asset_name))
        print(f"{index}/252 {name}: {size}x{size}",flush=True)
    # Only Castform has approved form-specific source art. No new weather logic.
    for form_id, form in enumerate(("sunny","rainy","snowy"),1):
        prefix="graphics/pokemon/castform/"+form+"/overworld"
        raw=get_source(prefix+".png")
        details=audit.read_png(raw,include_gba_frames=True)
        if details["frame_cell_size"] != [32,32]:
            raise ValueError("unexpected Castform form size "+form)
        blob=b"".join(details["gba_frames"])
        name="sFollowerCastform_"+form
        normal=audit.read_pal(get_source(prefix+"_normal.pal"))
        shiny=audit.read_pal(get_source(prefix+"_shiny.pal"))
        generated += [
            "static const u8 %s_pixels[%d] __attribute__((aligned(4))) = {\n%s\n};" %
                (name,len(blob),words(blob)),
            "static const u16 %s_normal[16] = {%s};" % (name,palette_words(normal)),
            "static const u16 %s_shiny[16] = {%s};" % (name,palette_words(shiny)),
        ]
        records.append(("SPECIES_CASTFORM",form_id,32,name))
    generated += [
        "static const struct FollowerArtEntry sFollowerArtTable[] = {",
        *("    {%s, %d, %d, %s_pixels, %s_normal, %s_shiny}," %
          (species,form,size,name,name,name) for species,form,size,name in records),
        "};",
        "static const u16 sFollowerArtCount = sizeof(sFollowerArtTable)/sizeof(sFollowerArtTable[0]);",
    ]
    content="\n".join(generated)+"\n"
    # Validate the complete approved source inventory BEFORE replacing the
    # ignored private registry. A failed conversion must not leave a partial
    # follower registry that could be compiled by the next private build.
    if len(records) != 255 or len(input_files) != 765 or len({item["path"] for item in input_files}) != 765:
        raise ValueError("expected 255 descriptors and 765 distinct pinned source files")
    if [item["path"] for item in input_files] != [item["path"] for item in expected_entries]:
        raise ValueError("source asset order differs from exact pinned upstream inventory")
    report={"base_source":audit.BASE,"base_species_count":252,"castform_form_count":3,
            "graphics_records":len(records),"generated_sha256":hashlib.sha256(content.encode()).hexdigest(),
            "source_file_count":len(input_files), "source_files":input_files,
            "source_blob_manifest":"docs/followers/expected-source-blobs.json",
            "upstream_git_tree":pins["tree_sha"],
            "notice":"all 765 source blobs checked against pinned upstream Git tree; no contributor license grant or gameplay acceptance asserted"}
    # Keep source and metadata in sync; only publish the generated registry
    # after its metadata has been written successfully. The metadata is local,
    # ignored by Git, and is rechecked by the private build gate.
    args.report.parent.mkdir(parents=True,exist_ok=True)
    args.report.write_text(json.dumps(report,indent=2)+"\n")
    args.output.parent.mkdir(parents=True,exist_ok=True)
    # Atomic replacement avoids leaving a partially written private include
    # if the file system fails partway through a large private art write.
    temporary = None
    try:
        with tempfile.NamedTemporaryFile(
            mode="w", encoding="utf-8", dir=args.output.parent,
            prefix=".follower-registry-", suffix=".tmp", delete=False
        ) as stream:
            temporary = Path(stream.name)
            stream.write(content)
        os.replace(temporary, args.output)
    finally:
        if temporary is not None:
            temporary.unlink(missing_ok=True)
    print(json.dumps(report,indent=2))
if __name__=="__main__":
    main()
