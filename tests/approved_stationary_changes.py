"""Reverse only the reviewed WRL002 four-stationary delta; preserve older guards."""
import hashlib
import json
import re
from pathlib import Path


def prior_stationary_source(path, text):
    row = json.loads(Path('tests/specifications/legendary_current_main_delta.json').read_text())['files'].get(str(path))
    if row is None:
        return text
    # Earlier projections can have already removed this feature and changed
    # surrounding context. Leave token-free input untouched for frozen guards.
    if not re.search(r'LIGHTNING_(?:RAIKOU|ENTEI|SUICUNE|MEW)|Lightning(?:Raikou|Entei|Suicune|Mew)|StaticBeastsSupersedeRoamer|legendary_statics', text, re.I):
        return text
    states = []
    for hunk in row['hunks']:
        before, after = text.count(hunk['before']), text.count(hunk['after'])
        if len(hunk['after']) >= len(hunk['before']):
            if after == 1:
                states.append(True)
            else:
                assert after == 0 and before == 1, 'Stationary source drift: ' + str(path)
                states.append(False)
        else:
            if before == 1:
                states.append(False)
            else:
                assert before == 0 and after == 1, 'Stationary source drift: ' + str(path)
                states.append(True)
    assert all(states) or not any(states), 'Partially projected stationary source: ' + str(path)
    if not any(states):
        return text
    digest = hashlib.sha256(text.encode()).hexdigest()
    for hunk in row['hunks']:
        text = text.replace(hunk['after'], hunk['before'], 1)
    if digest == row['after_sha256']:
        assert hashlib.sha256(text.encode()).hexdigest() == row['before_sha256']
    return text
