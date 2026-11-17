"""Exact current-main WRL010 reversal; never rewrites old hashes or drops drift."""
import hashlib
import json
import re
from pathlib import Path


def prior_follower_source(path, text):
    row = json.loads(Path('tests/specifications/follower_current_main_delta.json').read_text())['files'].get(str(path))
    if row is None:
        return text
    # Earlier callers can already have reversed this feature and then other
    # approved projections, so its anchored before-context may have changed.
    # Token-free input is returned byte-for-byte, never substituted/normalized;
    # callers retain their frozen whole-file guards and current-source tests
    # independently require each approved after hash.
    if not re.search('follower', text, re.I):
        return text
    states = []
    for hunk in row['hunks']:
        before, after = text.count(hunk['before']), text.count(hunk['after'])
        # For append/delete hunks the shorter anchored context is contained
        # in the longer one; identify the complete longer state first.
        if len(hunk['after']) >= len(hunk['before']):
            if after == 1:
                states.append(True)
            else:
                assert after == 0 and before == 1, 'WRL010 protected source drift: ' + str(path)
                states.append(False)
        else:
            if before == 1:
                states.append(False)
            else:
                assert before == 0 and after == 1, 'WRL010 protected source drift: ' + str(path)
                states.append(True)
    assert all(states) or not any(states), 'Partially projected WRL010 source: ' + str(path)
    if not any(states):
        return text
    digest = hashlib.sha256(text.encode()).hexdigest()
    for hunk in row['hunks']:
        text = text.replace(hunk['after'], hunk['before'], 1)
    if digest == row['after_sha256']:
        assert hashlib.sha256(text.encode()).hexdigest() == row['before_sha256']
    return text
