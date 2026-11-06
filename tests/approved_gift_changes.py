"""Reverse only the frozen issue38 approved gift deltas, preserving older audits."""
import json
from pathlib import Path

def prior_gift_source(path,text=None):
    if text is None:text=Path(path).read_text()
    for h in json.loads(Path('tests/specifications/approved_gift_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed gift delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
