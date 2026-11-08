"""Reverse only exact approved weather-ability source for older frozen audits."""
from pathlib import Path
import json


def prior_weather_ability_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    from approved_special_glalie_changes import prior_special_glalie_source
    text = prior_special_glalie_source(path, text)
    for h in json.loads(Path('tests/specifications/weather_ability_delta.json').read_text())['files'].get(str(path), {}).get('hunks', []):
        assert text.count(h['after']) == 1, 'Unreviewed weather ability delta: ' + str(path)
        text = text.replace(h['after'], h['before'], 1)
    return text
