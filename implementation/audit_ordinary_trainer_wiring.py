#!/usr/bin/env python3
"""Read-only, standard-library source wiring audit for Lightning Yellow ordinary Trainers.

Run from repository root:
    python3 implementation/audit_ordinary_trainer_wiring.py
    python3 implementation/audit_ordinary_trainer_wiring.py --json

No game assets or generated source files are modified.
"""
import argparse
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LEDGER = ROOT / "implementation/ordinary-trainers-delegated-audit-draft.md"
REMATCH = ROOT / "implementation/ordinary-trainer-rematch-candidates.md"
TRAINER_SOURCE = ROOT / "src/data/trainers.h"
PARTY_SOURCE = ROOT / "src/data/trainer_parties.h"
VS_SEEKER = ROOT / "src/vs_seeker.c"
SCRIPT_DIR = ROOT / "data/scripts"
MAP_DIR = ROOT / "data/maps"

ROW = re.compile(r"^\| `(TRAINER_[A-Z0-9_]+)` \|([^|]*)\|([^|]*)\|([^|]*)\| ([A-Z_]+) \|([^|]*)\|([^|]*)\|", re.M)
RVAR = re.compile(r"^\| `(TRAINER_[A-Z0-9_]+)` \| `(TRAINER_[A-Z0-9_]+)` \|", re.M)
RE_TRAINER = re.compile(r"\bTRAINER_[A-Z0-9_]+\b")
RE_SCRIPT = re.compile(r"^([A-Za-z_][A-Za-z_0-9]*)::", re.M)
RE_BATTLE = re.compile(r"\btrainerbattle_(?:single|double|rematch|rematch_double|two_trainers|continue_script|no_intro|no_intro_double)\s+(TRAINER_[A-Z0-9_]+)")
RE_INIT = re.compile(r"\[(TRAINER_[A-Z0-9_]+)\]\s*=\s*\{([\s\S]*?)\n    \},")
RE_CLASS = re.compile(r"\.trainerClass\s*=\s*TRAINER_CLASS_([A-Z0-9_]+)")
RE_PARTY_REF = re.compile(r"\.party\s*=\s*[A-Z_]+\((sParty_[A-Za-z0-9_]+)\)")
RE_PARTY_DEF = re.compile(r"^static const struct TrainerMon\w+\s+(sParty_\w+)\[\]\s*=\s*\{(.*?)\};", re.M | re.S)
RE_BATTLE_COMMAND = re.compile(r"\btrainerbattle_(?:single|double|rematch|rematch_double|two_trainers|continue_script|no_intro|no_intro_double)\s+(TRAINER_[A-Z0-9_]+)")
RE_VS = re.compile(r"\{\s*\{([^{}]+)\}\s*,\s*MAP\(MAP_([A-Z0-9_]+)\)\s*\}")


def rel(path):
    return str(path.relative_to(ROOT))


def parse_ledger():
    return {m.group(1): {
        "class": m.group(2).strip(), "source_party": m.group(3).strip(),
        "team": m.group(4).strip(), "status": m.group(5), "format": m.group(6).strip(),
        "flags": m.group(7).strip(),
    } for m in ROW.finditer(LEDGER.read_text(encoding="utf-8"))}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--json", action="store_true", help="print machine-readable audit")
    args = parser.parse_args()

    ledger_text = LEDGER.read_text(encoding="utf-8")
    ledger_ids = re.findall(r"^\| `(TRAINER_[A-Z0-9_]+)` \|", ledger_text, re.M)
    duplicate_ledger_ids = sorted({name for name in ledger_ids if ledger_ids.count(name) > 1})
    rows = parse_ledger()
    unparsed_ledger_ids = sorted(set(ledger_ids) - set(rows))
    source = TRAINER_SOURCE.read_text(encoding="utf-8")
    source_defs = {m.group(1): m.group(2) for m in RE_INIT.finditer(source)}
    # Validate the actual source party-array wiring, not just Trainer initializer IDs.
    # The source_party ledger field records the *current FireRed* party, not the
    # approved replacement. A proposed size change is handled separately.
    party_source = PARTY_SOURCE.read_text(encoding="utf-8")
    party_defs = {m.group(1): len(re.findall(r"\.species\s*=", m.group(2)))
                  for m in RE_PARTY_DEF.finditer(party_source)}
    missing_party_references = []
    missing_party_arrays = []
    dummy_source_party_ids = []
    source_party_size_inconsistencies = []
    proposed_party_size_differences = []
    for trainer_id, row in rows.items():
        entry = source_defs.get(trainer_id, "")
        party_match = RE_PARTY_REF.search(entry)
        if not party_match:
            missing_party_references.append(trainer_id)
            continue
        symbol = party_match.group(1)
        if symbol not in party_defs:
            missing_party_arrays.append({"id": trainer_id, "symbol": symbol})
            continue
        actual_size = party_defs[symbol]
        if actual_size == 0:
            dummy_source_party_ids.append(trainer_id)
            continue
        source_size = len([slot for slot in row["source_party"].split(";") if slot.strip()])
        draft_size = len([slot for slot in row["team"].split(";") if slot.strip()])
        if actual_size != source_size:
            source_party_size_inconsistencies.append({
                "id": trainer_id, "symbol": symbol,
                "source_array_size": actual_size, "ledger_source_size": source_size})
        if actual_size != draft_size:
            proposed_party_size_differences.append({
                "id": trainer_id, "source_array_size": actual_size,
                "draft_size": draft_size})

    scripted_story = {k: v for k, v in rows.items() if v["status"] == "DELEGATED_SCRIPTED_STORY_CANDIDATE"}
    original = {k: v for k, v in rows.items() if v["status"] not in ("DELEGATED_REMATCH_FAMILY_LOCKED", "DELEGATED_SCRIPTED_STORY_CANDIDATE")}
    variants = {k: v for k, v in rows.items() if v["status"] == "DELEGATED_REMATCH_FAMILY_LOCKED"}

    # Read both centrally shared and every individual map script. A base ID in
    # a Trainer definition is not sufficient to prove an encounter is reachable.
    scripts = list(SCRIPT_DIR.glob("*.inc")) + list(MAP_DIR.glob("*/scripts.inc"))
    references = {}
    script_labels = set()
    script_blocks = {}
    for path in scripts:
        content = path.read_text(encoding="utf-8", errors="replace")
        script_labels.update(RE_SCRIPT.findall(content))
        locations = list(RE_SCRIPT.finditer(content))
        for i, label_match in enumerate(locations):
            end = locations[i + 1].start() if i + 1 < len(locations) else len(content)
            label = label_match.group(1)
            block_text = content[label_match.end():end]
            commands = RE_BATTLE_COMMAND.findall(block_text)
            executable_lines = [line.strip() for line in block_text.splitlines()
                                if line.strip() and not line.lstrip().startswith("@")]
            first_opcode = executable_lines[0].split()[0] if executable_lines else None
            if commands:
                script_blocks.setdefault(label, []).append({
                    "path": rel(path), "trainer_ids": commands,
                    "first_opcode": first_opcode})
        for trainer_id in RE_TRAINER.findall(content):
            references.setdefault(trainer_id, set()).add(rel(path))

    # Original trainers with no direct script reference need investigation;
    # a trainer may still be reached through dynamically selected scripts.
    missing_original = sorted(set(original) - set(references))
    source_missing = sorted(set(rows) - set(source_defs))
    source_double_mismatch = []
    source_class_mismatch = []
    for trainer_id, row in rows.items():
        entry = source_defs.get(trainer_id, "")
        mm = RE_CLASS.search(entry)
        actual = mm.group(1) if mm else None
        intended = re.sub(r"[^A-Z0-9]", "", row["class"].upper())
        if actual and actual.replace("_", "") != intended:
            source_class_mismatch.append({"id": trainer_id, "source": actual, "design": row["class"]})
        if bool(re.search(r"\.doubleBattle\s*=\s*TRUE", entry)) != (row["format"].lower() == "double"):
            source_double_mismatch.append(trainer_id)

    vs = VS_SEEKER.read_text(encoding="utf-8")
    start = vs.find("static const struct RematchData sRematches[]")
    stop = vs.find("};", start)
    rematch_links = {}
    for m in RE_VS.finditer(vs[start:stop] if start >= 0 and stop >= 0 else ""):
        entries = RE_TRAINER.findall(m.group(1))
        if not entries:
            continue
        for variant in entries[1:]:
            if variant != entries[0]:
                rematch_links[variant] = {"base": entries[0], "map": m.group(2)}
    declared_links = {m.group(1): m.group(2) for m in RVAR.finditer(REMATCH.read_text(encoding="utf-8"))}
    missing_vs = sorted(set(variants) - set(rematch_links))
    incorrect_vs = sorted(k for k in variants
                          if k in rematch_links and declared_links.get(k) != rematch_links[k]["base"])

    # Confirm that each VS Seeker base has a scripted rematch path. Special
    # paired battles use trainerbattle_rematch_double rather than the single form.
    all_script_text = "\n".join(p.read_text(encoding="utf-8", errors="replace") for p in scripts)
    missing_rematch_script = sorted({
        link["base"] for key, link in rematch_links.items() if key in variants
        and not re.search(r"\btrainerbattle_rematch(?:_double)?\s+" + re.escape(link["base"]) + r"\b", all_script_text)
    })

    # A map object event script label should resolve somewhere in the loaded
    # script tables. Special and dynamic labels are reported, not auto-fixed.
    missing_map_labels = []
    trainer_map_objects = 0
    direct_map_trainer_ids = set()
    scripted_nontrainer_objects = []
    map_object_trainer_references = []
    ambiguous_map_label_paths = []
    direct_battle_map_objects = 0
    trainer_object_first_opcode_warnings = []
    trainer_object_source_format_warnings = []
    unresolved_direct_battle_map_objects = []
    map_count = 0
    for path in MAP_DIR.glob("*/map.json"):
        data = json.loads(path.read_text(encoding="utf-8"))
        map_count += 1
        for event in data.get("object_events", []):
            if not str(event.get("trainer_type", "")).startswith("TRAINER_TYPE_") or event.get("trainer_type") == "TRAINER_TYPE_NONE":
                label = event.get("script", "")
                allowed = [block for block in script_blocks.get(label, [])
                           if block["path"] == rel(path.parent / "scripts.inc")
                           or block["path"] == "data/scripts/trainers.inc"]
                linked_ids = sorted({trainer_id for block in allowed
                                     for trainer_id in block["trainer_ids"]
                                     if trainer_id in original})
                if linked_ids:
                    scripted_nontrainer_objects.append({
                        "map": path.parent.name, "script": label,
                        "trainer_type": event.get("trainer_type"),
                        "ids": linked_ids, "x": event.get("x"), "y": event.get("y")})
                continue
            trainer_map_objects += 1
            name = event.get("script", "")
            # Attribute the battle to the actual object's own map script, or
            # a central shared trainer script. A same-named label defined in
            # some unrelated map is not sufficient to verify this object.
            eligible = [block for block in script_blocks.get(name, [])
                        if block["path"] == rel(path.parent / "scripts.inc")
                        or block["path"] == "data/scripts/trainers.inc"]
            if eligible:
                direct_battle_map_objects += 1
                battle_ids = sorted({trainer_id for block in eligible
                                     for trainer_id in block["trainer_ids"]})
                direct_map_trainer_ids.update(battle_ids)
                for block in eligible:
                    opcode = block["first_opcode"]
                    if not (opcode or "").startswith("trainerbattle_"):
                        trainer_object_first_opcode_warnings.append({
                            "map": path.parent.name, "script": name,
                            "opcode": opcode, "ids": block["trainer_ids"]})
                    if opcode not in ("trainerbattle_single", "trainerbattle_double"):
                        continue  # Paired / scripted formats require separate engine checks.
                    for trainer_id in block["trainer_ids"]:
                        entry = source_defs.get(trainer_id, "")
                        if not entry:
                            continue
                        source_double = bool(re.search(r"\\.doubleBattle\\s*=\\s*TRUE", entry))
                        if source_double != (opcode == "trainerbattle_double"):
                            trainer_object_source_format_warnings.append({
                                "map": path.parent.name, "script": name,
                                "trainer_id": trainer_id,
                                "source_double": source_double, "script_opcode": opcode})
                map_object_trainer_references.append({
                    "map": path.parent.name, "script": name,
                    "ids": battle_ids, "x": event.get("x"), "y": event.get("y")})
            elif name in script_blocks:
                ambiguous_map_label_paths.append({
                    "map": path.parent.name, "script": name,
                    "found_elsewhere": [b["path"] for b in script_blocks[name]]})
            else:
                # No direct trainerbattle in this label's body. This can be
                # intentional indirection; queue for inspection, not a defect.
                unresolved_direct_battle_map_objects.append({
                    "map": path.parent.name, "script": name,
                    "x": event.get("x"), "y": event.get("y"),
                    "trainer_type": event.get("trainer_type")})
            if name and name not in script_labels:
                missing_map_labels.append({"map": path.parent.name, "script": name})

    # Cross-check the VS Seeker map against the actual base-Trainer NPC map.
    # This is a reference-level check; save-state progression still needs ROM QA.
    vs_bases_without_map_object = []
    vs_base_map_mismatches = []
    vs_base_object_script_not_starting_battle = []
    for variant, link in rematch_links.items():
        if variant not in variants:
            continue
        base = link["base"]
        actual_maps = sorted({entry["map"] for entry in map_object_trainer_references
                              if base in entry["ids"]})
        if not actual_maps:
            vs_bases_without_map_object.append({"variant": variant, "base": base})
            continue
        norm = lambda value: re.sub(r"[^A-Z0-9]", "", value.upper())
        for encounter in map_object_trainer_references:
            if base not in encounter["ids"]:
                continue
            allowed = [block for block in script_blocks.get(encounter["script"], [])
                       if block["path"] == "data/scripts/trainers.inc"
                       or block["path"] == "data/maps/" + encounter["map"] + "/scripts.inc"]
            if not any((block["first_opcode"] or "").startswith("trainerbattle_")
                       and base in block["trainer_ids"] for block in allowed):
                vs_base_object_script_not_starting_battle.append({
                    "base": base, "map": encounter["map"], "script": encounter["script"]})
        if norm(link["map"]) not in {norm(value) for value in actual_maps}:
            vs_base_map_mismatches.append({
                "variant": variant, "base": base,
                "vs_map": link["map"], "object_maps": actual_maps})

    # Literal references outside the conventional map/script corpus can explain
    # some archival IDs, but are not proof of a reachable encounter.
    other_engine_references = {}
    for folder in (ROOT / "src", ROOT / "data"):
        for source_file in folder.rglob("*"):
            if not source_file.is_file() or source_file.suffix not in (".c", ".s"):
                continue
            content = source_file.read_text(encoding="utf-8", errors="replace")
            for trainer_id in set(RE_TRAINER.findall(content)) & set(missing_original):
                other_engine_references.setdefault(trainer_id, []).append(rel(source_file))

    report = {
        "ledger_records": len(rows),
        "duplicate_ledger_ids": duplicate_ledger_ids,
        "unparsed_ledger_ids": unparsed_ledger_ids,
        "party_arrays_parsed": len(party_defs),
        "missing_party_references": missing_party_references,
        "missing_party_arrays": missing_party_arrays,
        "dummy_source_party_ids": dummy_source_party_ids,
        "source_party_size_inconsistencies": source_party_size_inconsistencies,
        "proposed_party_size_differences": proposed_party_size_differences,
        "original_or_standalone_records": len(original),
        "numeric_rematch_candidates": len(variants),
        "scripted_story_candidates": len(scripted_story),
        "scripted_story_candidate_battle_commands": {
            trainer_id: sorted(references.get(trainer_id, []))
            for trainer_id in scripted_story
            if re.search(r"\btrainerbattle_no_intro\s+" + re.escape(trainer_id) + r"\b", all_script_text)},
        "source_trainer_definitions_found": len(rows) - len(source_missing),
        "script_files_scanned": len(scripts),
        "map_json_files_scanned": map_count,
        "trainer_map_objects": trainer_map_objects,
        "objects_with_direct_battle_command": direct_battle_map_objects,
        "trainer_object_first_opcode_warnings": trainer_object_first_opcode_warnings,
        "trainer_object_source_format_warnings": trainer_object_source_format_warnings,
        "unreferenced_original_ids_with_other_engine_references": other_engine_references,
        "objects_whose_label_only_resolves_in_other_map": ambiguous_map_label_paths,
        "ordinary_original_ids_with_direct_map_object": len(set(original) & direct_map_trainer_ids),
        "ordinary_original_ids_without_direct_map_object": sorted(set(original) - direct_map_trainer_ids),
        "nontrainer_objects_with_direct_ordinary_battle": scripted_nontrainer_objects,
        "unmapped_original_ids_still_referenced_in_scripts": {trainer_id: sorted(references[trainer_id]) for trainer_id in sorted(set(original) - direct_map_trainer_ids) if trainer_id in references},
        "map_objects_with_multiple_direct_trainer_ids": [r for r in map_object_trainer_references if len(r["ids"]) > 1],
        "objects_requiring_indirect_script_review": unresolved_direct_battle_map_objects,
        "original_ids_referenced_in_scripts": len(original) - len(missing_original),
        "unreferenced_original_ids": missing_original,
        "source_ids_missing": source_missing,
        "approved_class_conversion_differences": source_class_mismatch,
        "approved_battle_format_differences": source_double_mismatch,
        "vs_seeker_rematches_linked": len(variants) - len(missing_vs),
        "vs_seeker_missing_variant_ids": missing_vs,
        "vs_seeker_wrong_base_ids": incorrect_vs,
        "vs_seeker_base_without_direct_map_object": vs_bases_without_map_object,
        "vs_seeker_base_map_mismatches_unused_metadata": vs_base_map_mismatches,
        "vs_seeker_base_script_not_starting_battle": vs_base_object_script_not_starting_battle,
        "vs_seeker_base_missing_rematch_script": missing_rematch_script,
        "map_trainer_object_unresolved_script_labels": missing_map_labels,
    }
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        for field, value in report.items():
            if isinstance(value, list):
                print(f"{field}: {len(value)}")
                for item in value[:40]:
                    print(f"  {item}")
                if len(value) > 40:
                    print(f"  ... and {len(value)-40} more")
            else:
                print(f"{field}: {value}")

if __name__ == "__main__":
    main()
