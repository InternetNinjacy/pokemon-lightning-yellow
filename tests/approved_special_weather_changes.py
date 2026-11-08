"""Undo exact approved special-weather hunks for immutable earlier audits."""
import json
from pathlib import Path

def prior_special_weather_source(path,text=None):
    if text is None:text=Path(path).read_text()
    for h in json.loads(Path('tests/specifications/special_weather_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed special weather delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
