"""Reverse approved Bug Bite and Chitin Crush appended source hunks, fail closed."""
from pathlib import Path
import json


def prior_bug_statdrop_source(path, content=None):
    if content is None:
        content = Path(path).read_text()
    manifest = json.loads(Path('tests/specifications/approved_bug_statdrop_delta.json').read_text())
    for h in manifest['files'].get(str(path), {}).get('hunks', []):
        assert content.count(h['after']) == 1, 'Unreviewed Bug statdrop delta: ' + str(path)
        content = content.replace(h['after'], h['before'], 1)
    return content
