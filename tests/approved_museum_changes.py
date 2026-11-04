"""Reverse only the frozen approved Museum deltas for older feature snapshots."""
from pathlib import Path
import json

def prior_museum_source(path):
    text=Path(path).read_text()
    for hunk in json.loads(Path('tests/specifications/approved_museum_delta.json').read_text())['hunks'].get(str(path),[]):
        assert text.count(hunk['after'])==1,'Unreviewed Museum change: '+str(path)
        text=text.replace(hunk['after'],hunk['before'],1)
    return text
