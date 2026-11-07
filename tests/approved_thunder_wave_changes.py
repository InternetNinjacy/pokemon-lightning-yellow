"""Reverse exact approved Thunder Wave target/context deltas for old audits."""
import json
from pathlib import Path

def prior_thunder_wave_source(path,text=None):
    if text is None:text=Path(path).read_text()
    for h in json.loads(Path('tests/specifications/approved_thunder_wave_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed Thunder Wave delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
