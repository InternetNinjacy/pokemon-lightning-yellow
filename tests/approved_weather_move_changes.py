"""Reverse only frozen approved issue47 weather-move deltas for historical audits."""
import json
from pathlib import Path

def prior_weather_move_source(path,text=None):
    if text is None:
        from approved_original_effect_changes import prior_original_effect_source
        text=prior_original_effect_source(path)
    for h in json.loads(Path('tests/specifications/approved_weather_move_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed weather move delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
