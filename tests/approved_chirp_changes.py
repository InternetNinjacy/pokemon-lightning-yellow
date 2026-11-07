"""Restore exactly approved #365 Chirp for the historical source checks."""
import json
from pathlib import Path


def prior_chirp_source(path, content=None):
    if content is None:
        content = Path(path).read_text()
    manifest = json.loads(Path('tests/specifications/approved_chirp_delta.json').read_text())
    for h in manifest['files'].get(str(path), {}).get('hunks', []):
        assert content.count(h['after']) == 1, 'Unreviewed Chirp delta: ' + str(path)
        content = content.replace(h['after'], h['before'], 1)
    return content
