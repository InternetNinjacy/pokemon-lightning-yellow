"""Reverse exactly the reviewed weather integration, retaining old audit hashes."""
from pathlib import Path
import json
from approved_gym_castform_changes import prior_gym_castform_source
from approved_lighting_preview_changes import prior_lighting_preview_source
from approved_follower_changes import prior_follower_source

def prior_dynamic_weather_source(path, text=None):
    if text is None:
        text=prior_gym_castform_source(path)
    text=prior_lighting_preview_source(path, text)
    text=prior_follower_source(path, text)
    for hunk in json.loads(Path('tests/specifications/approved_dynamic_weather_delta.json').read_text())['hunks'].get(str(path),[]):
        assert text.count(hunk['after'])==1,'Unreviewed dynamic weather change: '+str(path)
        text=text.replace(hunk['after'],hunk['before'],1)
    return text
