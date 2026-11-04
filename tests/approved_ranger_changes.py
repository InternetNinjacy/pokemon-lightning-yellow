"""Reverse only the frozen approved Ranger hunks for older-feature preservation."""
from pathlib import Path
import json

def prior_ranger_source(path, text=None):
    if text is None:
        text=Path(path).read_text()
    for hunk in json.loads(Path('tests/specifications/approved_ranger_delta.json').read_text())['hunks'].get(str(path),[]):
        assert text.count(hunk['after'])==1,'Unreviewed Ranger delta: '+str(path)
        text=text.replace(hunk['after'],hunk['before'],1)
    return text
