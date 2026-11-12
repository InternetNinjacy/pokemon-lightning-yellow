"""Strict #373 source reversal, preserving all frozen earlier expectations."""
from pathlib import Path
import json


def prior_avalanche_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    from approved_moves374_changes import prior_moves374_source
    text = prior_moves374_source(path, text)
    for h in json.loads(Path('tests/specifications/avalanche_delta.json').read_text())['files'].get(str(path), {}).get('hunks', []):
        assert text.count(h['after']) == 1, 'Unreviewed Avalanche #373 delta: ' + str(path)
        text = text.replace(h['after'], h['before'], 1)
    return text
