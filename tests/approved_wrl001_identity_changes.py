"""Strict reversal of prepared WRL001 ownership/naming correction for old audits."""
import hashlib
import json
from pathlib import Path


def prior_wrl001_identity_source(path, text):
    row = json.loads(Path('tests/specifications/wrl001_identity_delta.json').read_text())['files'].get(str(path))
    if row is None:
        return text
    if str(path) == 'data/specials.inc':
        # Older gift guards may already have removed the entire Oak registration.
        # Accept only the two existing frozen whole-file states, not a missing hook.
        frozen = json.loads(Path('tests/specifications/approved_gift_delta.json').read_text())['files'][str(path)]
        if hashlib.sha256(text.encode()).hexdigest() in (frozen['before_sha256'], frozen['after_sha256']):
            return text
    states = []
    for hunk in row['hunks']:
        before = text.count(hunk['before'])
        after = text.count(hunk['after'])
        assert (before, after) in ((1, 0), (0, 1)), 'Unapproved WRL001 identity source: ' + str(path)
        states.append(after == 1)
    assert all(states) or not any(states), 'Partially projected WRL001 identity source: ' + str(path)
    if not any(states):
        return text
    digest = hashlib.sha256(text.encode()).hexdigest()
    for hunk in row['hunks']:
        text = text.replace(hunk['after'], hunk['before'], 1)
    if digest == row['after_sha256']:
        assert hashlib.sha256(text.encode()).hexdigest() == row['before_sha256']
    return text
