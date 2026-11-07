"""Exact approved move #362 reversal before other historical baseline checks."""
import json
from pathlib import Path


def prior_venom_web_source(path, content=None):
    if content is None:
        content = Path(path).read_text()
    manifest = json.loads(Path('tests/specifications/approved_venom_web_delta.json').read_text())
    for h in manifest['files'].get(str(path), {}).get('hunks', []):
        assert content.count(h['after']) == 1, 'Unreviewed Venom Web delta: ' + str(path)
        content = content.replace(h['after'], h['before'], 1)
    return content
