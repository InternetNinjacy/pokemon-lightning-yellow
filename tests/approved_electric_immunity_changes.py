"""Reverse only the frozen owner-approved Electric paralysis immunity delta."""
import json
from pathlib import Path

def prior_electric_immunity_source(path,text=None):
    if text is None:text=Path(path).read_text()
    for h in json.loads(Path('tests/specifications/approved_electric_immunity_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed Electric immunity delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
