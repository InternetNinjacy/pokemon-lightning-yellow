"""Reverse only the exact newly owner-approved146 natural-learnset arrays.

Whole current/baseline states are idempotent; mixed/modified protected arrays
fail. Other historical projections may already have changed unrelated arrays;
their caller's frozen whole-file hashes remain mandatory in that case.
"""
import hashlib
import json
import re
from pathlib import Path

PATH = 'src/data/pokemon/level_up_learnsets.h'
CONTRACT = 'tests/specifications/pkm009_approved146.json'


def contract():
    return json.loads(Path(CONTRACT).read_text())


def prior_146_learnset_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    if str(path) != PATH:
        return text
    spec = contract()
    assert len(spec['rows']) == len({r['pointer'] for r in spec['rows']}) == 146
    states = []
    for row in spec['rows']:
        found = list(re.finditer(r'static const u16 ' + re.escape(row['pointer'])
                                + r'\[\]\s*=\s*\{.*?\n\};', text, re.S))
        assert len(found) == 1, 'Missing/duplicate approved146 array: ' + row['species']
        actual = found[0].group()
        assert actual in (row['before_body'], row['after_body']), 'Unapproved146 array drift: ' + row['species']
        states.append(actual == row['after_body'])
    assert all(states) or not any(states), 'Partially projected approved146 source'
    if not any(states):
        return text
    digest = hashlib.sha256(text.encode()).hexdigest()
    for row in spec['rows']:
        assert text.count(row['after_body']) == 1
        text = text.replace(row['after_body'], row['before_body'], 1)
    if digest == spec['after_sha256']:
        assert hashlib.sha256(text.encode()).hexdigest() == spec['before_sha256']
    return text
