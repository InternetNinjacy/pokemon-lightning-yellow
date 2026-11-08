"""Undo only approved Brock party/bag edits for older frozen source audits."""
import json
from pathlib import Path


def prior_brock_weather_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    for h in json.loads(Path('tests/specifications/brock_weather_party_delta.json').read_text())['files'].get(str(path), {}).get('hunks', []):
        assert text.count(h['after']) == 1, 'Unreviewed Brock weather party delta: ' + str(path)
        text = text.replace(h['after'], h['before'], 1)
    return text
