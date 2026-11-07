"""Reverse only the seven approved Bellow / Power Gem / Stone Edge source edits."""
from pathlib import Path
import json


def prior_bellow_rock_source(path, content=None):
    if content is None:
        content = Path(path).read_text()
    manifest = json.loads(Path('tests/specifications/approved_bellow_rock_delta.json').read_text())
    for h in manifest['files'].get(str(path), {}).get('hunks', []):
        assert content.count(h['after']) == 1, 'Unreviewed Bellow/Rock delta: ' + str(path)
        content = content.replace(h['after'], h['before'], 1)
    return content
