"""Reverse only the reviewed Roar delta for historical source audits."""
import json
from pathlib import Path

def prior_roar_source(path, text=None):
    if text is None:
        from approved_weather_move_changes import prior_weather_move_source
        text = prior_weather_move_source(path)
    manifest = json.loads(Path('tests/specifications/approved_roar_delta.json').read_text())
    for hunk in manifest['files'].get(str(path), {}).get('hunks', []):
        assert text.count(hunk['after']) == 1, 'Unreviewed Roar delta: ' + str(path)
        text = text.replace(hunk['after'], hunk['before'], 1)
    return text
