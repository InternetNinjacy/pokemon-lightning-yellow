"""Reverse exact approved appended Fire-move source for historical audits only."""
import json
from pathlib import Path


def prior_new_fire_move_source(path, text=None):
    from approved_new_ghost_move_changes import prior_new_ghost_move_source
    text = prior_new_ghost_move_source(path, text)
    manifest = json.loads(Path('tests/specifications/approved_new_fire_move_delta.json').read_text())
    for h in manifest['files'].get(str(path), {}).get('hunks', []):
        if text.count(h['after']) == 1:
            text = text.replace(h['after'], h['before'], 1)
        else:
            assert text.count(h['before']) == 1, 'Unreviewed new Fire move delta: ' + str(path)
    return text


def prior_new_fire_move_bytes(path):
    manifest = json.loads(Path('tests/specifications/approved_new_fire_move_delta.json').read_text())
    return prior_new_fire_move_source(path).encode() if str(path) in manifest['files'] else Path(path).read_bytes()
