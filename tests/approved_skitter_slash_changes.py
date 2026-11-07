"""Exact approved #361 reverse restoration for historical source audits.

Every hunk must appear once. Unknown changes are not silently normalized.
"""
import json
from pathlib import Path


def prior_skitter_slash_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    manifest = json.loads(Path('tests/specifications/approved_skitter_slash_delta.json').read_text())
    for h in manifest['files'].get(str(path), {}).get('hunks', []):
        assert text.count(h['after']) == 1, 'Unreviewed Skitter Slash delta: ' + str(path)
        text = text.replace(h['after'], h['before'], 1)
    return text
