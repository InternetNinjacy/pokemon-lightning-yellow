"""Reverse exactly the reviewed weather integration, retaining old audit hashes."""
from pathlib import Path
import json

def prior_dynamic_weather_source(path, text=None):
    if text is None:
        text=Path(path).read_text()
    for hunk in json.loads(Path('tests/specifications/approved_dynamic_weather_delta.json').read_text())['hunks'].get(str(path),[]):
        assert text.count(hunk['after'])==1,'Unreviewed dynamic weather change: '+str(path)
        text=text.replace(hunk['after'],hunk['before'],1)
    return text
