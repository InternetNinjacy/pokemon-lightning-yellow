#!/usr/bin/env python3
"""Audit authored world assignments, not the engine's full species database.

--strict fails for ANY current off-Dex assignment. --check fails for new findings
relative to the explicitly unresolved inventory; that inventory is not approval
or an exception to the owner's global eligibility rule. Runtime reachability of
every branch is not inferred from source references.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
SNAPSHOT = ROOT / 'tests/specifications/unresolved_species_assignments.json'


def audit():
    roster = re.findall(r'CUSTOM_DEX_ENTRY\((\d+),\s*(\w+)\)',
                        (ROOT / 'src/data/pokemon/custom_pokedex.h').read_text())
    assert len(roster) == 252 and [int(n) for n, _ in roster] == list(range(1, 253))
    allowed = {name for _, name in roster}
    assert len(allowed) == 252 and dict(roster)['233'] == 'CASTFORM'
    records = []

    def add(path, context, kind, species, line=None):
        records.append(dict(path=str(path), context=context, kind=kind,
                            species=species.removeprefix('SPECIES_'), line=line))

    path = Path('src/data/wild_encounters.json')
    for group in json.loads((ROOT / path).read_text())['wild_encounter_groups']:
        for enc in group.get('encounters', []):
            label = enc.get('base_label', enc.get('map', '?'))
            if 'LeafGreen' in label:
                continue  # audit the project's FireRed target, not an alternate build
            for table, data in enc.items():
                if not isinstance(data, dict) or 'mons' not in data:
                    continue
                for i, mon in enumerate(data['mons']):
                    add(path, f'{label}/{table}/{i}', 'wild', mon['species'])

    paths = [Path('src/data/trainer_parties.h'), Path('src/data/ingame_trades.h'), Path('src/trainer_tower_sets.c')]
    paths += sorted(Path(p).relative_to(ROOT) for p in (ROOT / 'src/data/battle_tower').glob('*mons.h'))
    for path in paths:
        context = 'unclassified'
        for line_no, line in enumerate((ROOT / path).read_text().splitlines(), 1):
            m = re.search(r'(?:sParty_\w+|INGAME_TRADE_\w+)', line)
            if m:
                context = m[0]
            for field, species in re.findall(r'\.(species|requestedSpecies)\s*=\s*SPECIES_(\w+)', line):
                kind = ('trainer' if 'trainer_parties' in str(path) else
                        'trade_' + field if 'ingame_trades' in str(path) else
                        'trainer_tower' if 'trainer_tower_sets' in str(path) else 'legacy_battle_tower_reference')
                add(path, context, kind, species, line_no)

    # Direct scripts plus species-valued variables, including dynamic prize and
    # encounter paths. Variable references are conservative review candidates;
    # not every one is an obtainable Pokemon (some are display/check-only).
    for directory in ['data/maps', 'data/scripts']:
        for full in sorted((ROOT / directory).rglob('*.inc')):
            path = full.relative_to(ROOT)
            context = 'unclassified'
            for line_no, line in enumerate(full.read_text().splitlines(), 1):
                m = re.match(r'(\w+)::?', line)
                if m:
                    context = m[1]
                m = re.match(r'\s*(givemon|giveegg|setwildbattle|seteventmon|setvar|setorcopyvar)\b(.*)', line)
                if m:
                    for species in re.findall(r'\bSPECIES_(\w+)', m[2].split('@')[0]):
                        kind = 'script_assignment' if m[1] in ['givemon', 'giveegg', 'setwildbattle', 'seteventmon'] else 'script_species_variable_review'
                        add(path, context, kind, species, line_no)

    # Include C-authored special pairs and starter choices. These references
    # require review when excluded; do not scan species/evolution/learnset data
    # that deliberately retain canonical engine IDs outside the custom roster.
    for path in [Path('src/mystery_vendor.c'), Path('src/wild_double_battle.c'), Path('src/wild_encounter.c'), Path('src/roamer.c'), Path('src/battle_setup.c'), Path('src/field_specials.c')]:
        if not (ROOT / path).exists():
            continue
        for line_no, line in enumerate((ROOT / path).read_text().splitlines(), 1):
            for species in re.findall(r'\bSPECIES_(\w+)', line.split('//')[0]):
                if species not in ['NONE', 'EGG']:
                    add(path, 'C species reference; inspect role', 'c_species_reference_review', species, line_no)

    violations = [r for r in records if r['species'] not in allowed and r['species'] != 'NONE']
    return records, violations


def signatures(records):
    return Counter(json.dumps({k: v for k, v in r.items() if k != 'line'}, sort_keys=True)
                   for r in records)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--strict', action='store_true')
    p.add_argument('--check', action='store_true')
    p.add_argument('--json', action='store_true')
    args = p.parse_args()
    records, findings = audit()
    confirmed = [r for r in findings if r['kind'] in ['wild', 'trainer', 'trainer_tower', 'script_assignment', 'trade_species']]
    if args.json:
        print(json.dumps({'assignments_and_review_candidates': len(records), 'confirmed_assignment_findings': len(confirmed), 'findings': findings}, indent=2))
    else:
        print(f'AUDIT: 252-species roster unchanged; {len(records)} assignments/review candidates, {len(findings)} unresolved off-Dex findings')
        print(json.dumps(dict(Counter(r['kind'] for r in findings)), sort_keys=True))
        print(f'{len(confirmed)} authored off-Dex assignments; legacy/reference candidates are separately classified, not inferred runtime assignments.')
    if args.strict and confirmed:
        print('FAIL: global world eligibility is NOT yet compliant; approved replacements/removals are required.')
        return 1
    if args.check:
        old = json.loads(SNAPSHOT.read_text())
        added = signatures(findings) - signatures(old['findings'])
        if added:
            print('FAIL: new off-Dex findings; do not expand the roster or invent replacements.')
            for key, count in added.items():
                print(count, key)
            return 1
        print('PASS: no new off-Dex findings beyond the explicitly UNRESOLVED inventory; not a global compliance pass.')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
