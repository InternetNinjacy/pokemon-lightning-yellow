"""Exact opt-in Glalie storage reverse layer; old save/source hashes stay frozen."""
import json
from pathlib import Path


def prior_special_glalie_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    from approved_weather_species_evolution_changes import prior_weather_species_evolution_source
    text = prior_weather_species_evolution_source(path, text)
    for h in json.loads(Path('tests/specifications/special_glalie_delta.json').read_text())['files'].get(str(path), {}).get('hunks', []):
        assert text.count(h['after']) == 1, 'Unreviewed special Glalie delta: ' + str(path)
        text = text.replace(h['after'], h['before'], 1)
    return text
