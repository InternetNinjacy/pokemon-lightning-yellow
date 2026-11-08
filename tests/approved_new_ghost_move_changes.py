"""Reverse exact approved Ghost additions/conditional power for older audits."""
import json
from pathlib import Path


def prior_new_ghost_move_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    from approved_skitter_slash_changes import prior_skitter_slash_source
    text = prior_skitter_slash_source(path, text)
    m = json.loads(Path('tests/specifications/approved_new_ghost_move_delta.json').read_text())
    for h in m['files'].get(str(path), {}).get('hunks', []):
        if text.count(h['after']) == 1:
            text = text.replace(h['after'], h['before'], 1)
        else:
            assert text.count(h['before']) == 1, 'Unreviewed new Ghost move delta: ' + str(path)
    return text
