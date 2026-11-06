"""Reverse only the reviewed Roar delta for historical source audits."""
import json
from pathlib import Path

def prior_roar_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    manifest = json.loads(Path('tests/specifications/approved_roar_delta.json').read_text())
    for hunk in manifest['files'].get(str(path), {}).get('hunks', []):
        assert text.count(hunk['after']) == 1, 'Unreviewed Roar delta: ' + str(path)
        text = text.replace(hunk['after'], hunk['before'], 1)
    return text
