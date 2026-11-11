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
    parser.add_argument("--output", type=Path, default=ROOT/"src/follower_assets_generated.inc")
    parser.add_argument("--report", type=Path, default=ROOT/"docs/followers/local-build-metadata.json")
    args = parser.parse_args()
    if not args.confirmed_scope:
        parser.error("art import requires an explicit --confirmed-scope invocation")
    with (ROOT/"docs/followers/252-species-coverage.csv").open(newline="") as f:
        rows = list(csv.DictReader(f))
    if len(rows) != 252 or rows[232]["species"] != "CASTFORM":
        raise ValueError("wrong approved roster; refusing sprite build")
    records = []
    generated = ["// GENERATED LOCALLY; do not commit this file with source-art bytes.",
                 "// Pinned source: " + audit.BASE,
                 "// Frames: south0/1 north0/1 west0/1; east mirrors west.",
                 "// NORMAL and SHINY share the same pixel index table."]
    for index, row in enumerate(rows, 1):
        name = row["species"]
        species = "SPECIES_" + name
        asset_name = "sFollowerArt_%03d" % index
        png = audit.fetch(row["kasenn_sheet"])
        if audit.git_blob_sha(png) != row["kasenn_blob_sha"]:
            raise ValueError("pinned blob mismatch for " + name)
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
            palettes[variant] = audit.read_pal(audit.fetch(row["kasenn_" + variant + "_palette"]))
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
        raw=audit.fetch(prefix+".png")
        details=audit.read_png(raw,include_gba_frames=True)
        if details["frame_cell_size"] != [32,32]:
            raise ValueError("unexpected Castform form size "+form)
        blob=b"".join(details["gba_frames"])
        name="sFollowerCastform_"+form
        normal=audit.read_pal(audit.fetch(prefix+"_normal.pal"))
        shiny=audit.read_pal(audit.fetch(prefix+"_shiny.pal"))
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
    args.output.parent.mkdir(parents=True,exist_ok=True)
    args.output.write_text(content)
    report={"base_source":audit.BASE,"base_species_count":252,"castform_form_count":3,
            "graphics_records":len(records),"generated_sha256":hashlib.sha256(content.encode()).hexdigest(),
            "notice":"metadata only; does not certify an individual artist's permission or native gameplay acceptance"}
    args.report.parent.mkdir(parents=True,exist_ok=True)
    args.report.write_text(json.dumps(report,indent=2)+"\n")
    print(json.dumps(report,indent=2))
if __name__=="__main__":
    main()
