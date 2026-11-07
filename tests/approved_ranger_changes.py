"""Reverse only the frozen approved Ranger hunks for older-feature preservation."""
from pathlib import Path
import json
from approved_dynamic_weather_changes import prior_dynamic_weather_source

def prior_ranger_source(path, text=None):
    if text is None:
        text=prior_dynamic_weather_source(path)
    for hunk in json.loads(Path('tests/specifications/approved_ranger_delta.json').read_text())['hunks'].get(str(path),[]):
        assert text.count(hunk['after'])==1,'Unreviewed Ranger delta: '+str(path)
        text=text.replace(hunk['after'],hunk['before'],1)
    return text
